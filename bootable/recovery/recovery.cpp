/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <linux/input.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef GMT_RECOVERY_MODE_ROCK_GOTA_SUPPORT
#include <sys/wait.h>
#include <fs_mgr.h>	// for android 4.4
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "bootloader.h"
#include "common.h"
#include "cutils/properties.h"
#include "cutils/android_reboot.h"
#include "install.h"
#include "minui/minui.h"
#include "minzip/DirUtil.h"
#include "roots.h"
#include "ui.h"
#include "screen_ui.h"
#include "device.h"
#include "adb_install.h"
extern "C" {
#include "minadbd/adb.h"
#include "fuse_sideload.h"
#include "fuse_sdcard_provider.h"
}

//Tinno:CJ  
#define TINNO_MULTI_UPDATE_PACKAGE

struct selabel_handle *sehandle;

static const struct option OPTIONS[] = {
  { "send_intent", required_argument, NULL, 's' },
  { "update_package", required_argument, NULL, 'u' },
  { "wipe_data", no_argument, NULL, 'w' },
  { "wipe_cache", no_argument, NULL, 'c' },
  { "show_text", no_argument, NULL, 't' },
  { "just_exit", no_argument, NULL, 'x' },
  { "locale", required_argument, NULL, 'l' },
  { "stages", required_argument, NULL, 'g' },
  { "shutdown_after", no_argument, NULL, 'p' },
  { "reason", required_argument, NULL, 'r' },
  { NULL, 0, NULL, 0 },
};

#define LAST_LOG_FILE "/cache/recovery/last_log"

static const char *CACHE_LOG_DIR = "/cache/recovery";
static const char *COMMAND_FILE = "/cache/recovery/command";
static const char *INTENT_FILE = "/cache/recovery/intent";
static const char *LOG_FILE = "/cache/recovery/log";
static const char *LAST_INSTALL_FILE = "/cache/recovery/last_install";
static const char *LOCALE_FILE = "/cache/recovery/last_locale";
static const char *CACHE_ROOT = "/cache";
static const char *SDCARD_ROOT = "/sdcard";
static const char *TEMPORARY_LOG_FILE = "/tmp/recovery.log";
static const char *TEMPORARY_INSTALL_FILE = "/tmp/last_install";

#define KEEP_LOG_COUNT 10

RecoveryUI* ui = NULL;
char* locale = NULL;
char recovery_version[PROPERTY_VALUE_MAX+1];
char* stage = NULL;
char* reason = NULL;

/*
 * The recovery tool communicates with the main system through /cache files.
 *   /cache/recovery/command - INPUT - command line for tool, one arg per line
 *   /cache/recovery/log - OUTPUT - combined log file from recovery run(s)
 *   /cache/recovery/intent - OUTPUT - intent that was passed in
 *
 * The arguments which may be supplied in the recovery.command file:
 *   --send_intent=anystring - write the text out to recovery.intent
 *   --update_package=path - verify install an OTA package file
 *   --wipe_data - erase user data (and cache), then reboot
 *   --wipe_cache - wipe cache (but not user data), then reboot
 *   --set_encrypted_filesystem=on|off - enables / diasables encrypted fs
 *   --just_exit - do nothing; exit and reboot
 *
 * After completing, we remove /cache/recovery/command and reboot.
 * Arguments may also be supplied in the bootloader control block (BCB).
 * These important scenarios must be safely restartable at any point:
 *
 * FACTORY RESET
 * 1. user selects "factory reset"
 * 2. main system writes "--wipe_data" to /cache/recovery/command
 * 3. main system reboots into recovery
 * 4. get_args() writes BCB with "boot-recovery" and "--wipe_data"
 *    -- after this, rebooting will restart the erase --
 * 5. erase_volume() reformats /data
 * 6. erase_volume() reformats /cache
 * 7. finish_recovery() erases BCB
 *    -- after this, rebooting will restart the main system --
 * 8. main() calls reboot() to boot main system
 *
 * OTA INSTALL
 * 1. main system downloads OTA package to /cache/some-filename.zip
 * 2. main system writes "--update_package=/cache/some-filename.zip"
 * 3. main system reboots into recovery
 * 4. get_args() writes BCB with "boot-recovery" and "--update_package=..."
 *    -- after this, rebooting will attempt to reinstall the update --
 * 5. install_package() attempts to install the update
 *    NOTE: the package install must itself be restartable from any point
 * 6. finish_recovery() erases BCB
 *    -- after this, rebooting will (try to) restart the main system --
 * 7. ** if install failed **
 *    7a. prompt_and_wait() shows an error icon and waits for the user
 *    7b; the user reboots (pulling the battery, etc) into the main system
 * 8. main() calls maybe_install_firmware_update()
 *    ** if the update contained radio/hboot firmware **:
 *    8a. m_i_f_u() writes BCB with "boot-recovery" and "--wipe_cache"
 *        -- after this, rebooting will reformat cache & restart main system --
 *    8b. m_i_f_u() writes firmware image into raw cache partition
 *    8c. m_i_f_u() writes BCB with "update-radio/hboot" and "--wipe_cache"
 *        -- after this, rebooting will attempt to reinstall firmware --
 *    8d. bootloader tries to flash firmware
 *    8e. bootloader writes BCB with "boot-recovery" (keeping "--wipe_cache")
 *        -- after this, rebooting will reformat cache & restart main system --
 *    8f. erase_volume() reformats /cache
 *    8g. finish_recovery() erases BCB
 *        -- after this, rebooting will (try to) restart the main system --
 * 9. main() calls reboot() to boot main system
 */

//Tinno:CJ  
#ifdef TINNO_MULTI_UPDATE_PACKAGE      
static const int MAX_ARG_LENGTH = 4096 * 30;
#else
static const int MAX_ARG_LENGTH = 4096;
#endif

static const int MAX_ARGS = 100;

// open a given path, mounting partitions as necessary
FILE*
fopen_path(const char *path, const char *mode) {
    if (ensure_path_mounted(path) != 0) {
        LOGE("Can't mount %s\n", path);
        return NULL;
    }

    // When writing, try to create the containing directory, if necessary.
    // Use generous permissions, the system (init.rc) will reset them.
    if (strchr("wa", mode[0])) dirCreateHierarchy(path, 0777, NULL, 1, sehandle);

    FILE *fp = fopen(path, mode);
    return fp;
}

static void redirect_stdio(const char* filename) {
    // If these fail, there's not really anywhere to complain...
    freopen(filename, "a", stdout); setbuf(stdout, NULL);
    freopen(filename, "a", stderr); setbuf(stderr, NULL);
}

// close a file, log an error if the error indicator is set
static void
check_and_fclose(FILE *fp, const char *name) {
    fflush(fp);
    if (ferror(fp)) LOGE("Error in %s\n(%s)\n", name, strerror(errno));
    fclose(fp);
}

// command line args come from, in decreasing precedence:
//   - the actual command line
//   - the bootloader control block (one per line, after "recovery")
//   - the contents of COMMAND_FILE (one per line)
static void
get_args(int *argc, char ***argv) {
    struct bootloader_message boot;
    memset(&boot, 0, sizeof(boot));
    get_bootloader_message(&boot);  // this may fail, leaving a zeroed structure
    stage = strndup(boot.stage, sizeof(boot.stage));

    if (boot.command[0] != 0 && boot.command[0] != 255) {
        LOGI("Boot command: %.*s\n", (int)sizeof(boot.command), boot.command);
    }

    if (boot.status[0] != 0 && boot.status[0] != 255) {
        LOGI("Boot status: %.*s\n", (int)sizeof(boot.status), boot.status);
    }

    // --- if arguments weren't supplied, look in the bootloader control block
    if (*argc <= 1) {
        boot.recovery[sizeof(boot.recovery) - 1] = '\0';  // Ensure termination
        const char *arg = strtok(boot.recovery, "\n");
        if (arg != NULL && !strcmp(arg, "recovery")) {
            *argv = (char **) malloc(sizeof(char *) * MAX_ARGS);
            (*argv)[0] = strdup(arg);
            for (*argc = 1; *argc < MAX_ARGS; ++*argc) {
                if ((arg = strtok(NULL, "\n")) == NULL) break;
                (*argv)[*argc] = strdup(arg);
            }
            LOGI("Got arguments from boot message\n");
        } else if (boot.recovery[0] != 0 && boot.recovery[0] != 255) {
            LOGE("Bad boot message\n\"%.20s\"\n", boot.recovery);
        }
    }

    // --- if that doesn't work, try the command file
    if (*argc <= 1) {
        FILE *fp = fopen_path(COMMAND_FILE, "r");
        if (fp != NULL) {
            char *token;
            char *argv0 = (*argv)[0];
            *argv = (char **) malloc(sizeof(char *) * MAX_ARGS);
            (*argv)[0] = argv0;  // use the same program name

            char buf[MAX_ARG_LENGTH];
            for (*argc = 1; *argc < MAX_ARGS; ++*argc) {
                if (!fgets(buf, sizeof(buf), fp)) break;
                token = strtok(buf, "\r\n");
                if (token != NULL) {
                    (*argv)[*argc] = strdup(token);  // Strip newline.
                } else {
                    --*argc;
                }
            }

            check_and_fclose(fp, COMMAND_FILE);
            LOGI("Got arguments from %s\n", COMMAND_FILE);
        }
    }

    // --> write the arguments we have back into the bootloader control block
    // always boot into recovery after this (until finish_recovery() is called)
    strlcpy(boot.command, "boot-recovery", sizeof(boot.command));
    strlcpy(boot.recovery, "recovery\n", sizeof(boot.recovery));
    int i;
    for (i = 1; i < *argc; ++i) {
        strlcat(boot.recovery, (*argv)[i], sizeof(boot.recovery));
        strlcat(boot.recovery, "\n", sizeof(boot.recovery));
    }
    set_bootloader_message(&boot);
}

static void
set_sdcard_update_bootloader_message() {
    struct bootloader_message boot;
    memset(&boot, 0, sizeof(boot));
    strlcpy(boot.command, "boot-recovery", sizeof(boot.command));
    strlcpy(boot.recovery, "recovery\n", sizeof(boot.recovery));
    set_bootloader_message(&boot);
}

// How much of the temp log we have copied to the copy in cache.
static long tmplog_offset = 0;

static void
copy_log_file(const char* source, const char* destination, int append) {
    FILE *log = fopen_path(destination, append ? "a" : "w");
    if (log == NULL) {
        LOGE("Can't open %s\n", destination);
    } else {
        FILE *tmplog = fopen(source, "r");
        if (tmplog != NULL) {
            if (append) {
                fseek(tmplog, tmplog_offset, SEEK_SET);  // Since last write
            }
            char buf[4096];
            while (fgets(buf, sizeof(buf), tmplog)) fputs(buf, log);
            if (append) {
                tmplog_offset = ftell(tmplog);
            }
            check_and_fclose(tmplog, source);
        }
        check_and_fclose(log, destination);
    }
}

// Rename last_log -> last_log.1 -> last_log.2 -> ... -> last_log.$max
// Overwrites any existing last_log.$max.
static void
rotate_last_logs(int max) {
    char oldfn[256];
    char newfn[256];

    int i;
    for (i = max-1; i >= 0; --i) {
        snprintf(oldfn, sizeof(oldfn), (i==0) ? LAST_LOG_FILE : (LAST_LOG_FILE ".%d"), i);
        snprintf(newfn, sizeof(newfn), LAST_LOG_FILE ".%d", i+1);
        // ignore errors
        rename(oldfn, newfn);
    }
}

static void
copy_logs() {
    // Copy logs to cache so the system can find out what happened.
    copy_log_file(TEMPORARY_LOG_FILE, LOG_FILE, true);
    copy_log_file(TEMPORARY_LOG_FILE, LAST_LOG_FILE, false);
    copy_log_file(TEMPORARY_INSTALL_FILE, LAST_INSTALL_FILE, false);
    chmod(LOG_FILE, 0600);
    chown(LOG_FILE, 1000, 1000);   // system user
    chmod(LAST_LOG_FILE, 0640);
    chown(LAST_LOG_FILE, 1000, 2000); // modified by chb, for ota read.
    chmod(LAST_INSTALL_FILE, 0644);
    sync();
}

// clear the recovery command and prepare to boot a (hopefully working) system,
// copy our log file to cache as well (for the system to read), and
// record any intent we were asked to communicate back to the system.
// this function is idempotent: call it as many times as you like.
static void
finish_recovery(const char *send_intent) {
    // By this point, we're ready to return to the main system...
    if (send_intent != NULL) {
        FILE *fp = fopen_path(INTENT_FILE, "w");
        if (fp == NULL) {
            LOGE("Can't open %s\n", INTENT_FILE);
        } else {
            fputs(send_intent, fp);
            check_and_fclose(fp, INTENT_FILE);
        }
    }

    // Save the locale to cache, so if recovery is next started up
    // without a --locale argument (eg, directly from the bootloader)
    // it will use the last-known locale.
    if (locale != NULL) {
        LOGI("Saving locale \"%s\"\n", locale);
        FILE* fp = fopen_path(LOCALE_FILE, "w");
        fwrite(locale, 1, strlen(locale), fp);
        fflush(fp);
        fsync(fileno(fp));
        check_and_fclose(fp, LOCALE_FILE);
    }

    copy_logs();

    // Reset to normal system boot so recovery won't cycle indefinitely.
    struct bootloader_message boot;
    memset(&boot, 0, sizeof(boot));
    set_bootloader_message(&boot);

    // Remove the command file, so recovery won't repeat indefinitely.
    if (ensure_path_mounted(COMMAND_FILE) != 0 ||
        (unlink(COMMAND_FILE) && errno != ENOENT)) {
        LOGW("Can't unlink %s\n", COMMAND_FILE);
    }

    ensure_path_unmounted(CACHE_ROOT);
    sync();  // For good measure.
}

typedef struct _saved_log_file {
    char* name;
    struct stat st;
    unsigned char* data;
    struct _saved_log_file* next;
} saved_log_file;

static int
erase_volume(const char *volume) {
    bool is_cache = (strcmp(volume, CACHE_ROOT) == 0);

    ui->SetBackground(RecoveryUI::ERASING);
    ui->SetProgressType(RecoveryUI::INDETERMINATE);

    saved_log_file* head = NULL;

    if (is_cache) {
        // If we're reformatting /cache, we load any
        // "/cache/recovery/last*" files into memory, so we can restore
        // them after the reformat.

        ensure_path_mounted(volume);

        DIR* d;
        struct dirent* de;
        d = opendir(CACHE_LOG_DIR);
        if (d) {
            char path[PATH_MAX];
            strcpy(path, CACHE_LOG_DIR);
            strcat(path, "/");
            int path_len = strlen(path);
            while ((de = readdir(d)) != NULL) {
                if (strncmp(de->d_name, "last", 4) == 0) {
                    saved_log_file* p = (saved_log_file*) malloc(sizeof(saved_log_file));
                    strcpy(path+path_len, de->d_name);
                    p->name = strdup(path);
                    if (stat(path, &(p->st)) == 0) {
                        // truncate files to 512kb
                        if (p->st.st_size > (1 << 19)) {
                            p->st.st_size = 1 << 19;
                        }
                        p->data = (unsigned char*) malloc(p->st.st_size);
                        FILE* f = fopen(path, "rb");
                        fread(p->data, 1, p->st.st_size, f);
                        fclose(f);
                        p->next = head;
                        head = p;
                    } else {
                        free(p);
                    }
                }
            }
            closedir(d);
        } else {
            if (errno != ENOENT) {
                printf("opendir failed: %s\n", strerror(errno));
            }
        }
    }

    ui->Print("Formatting %s...\n", volume);

    ensure_path_unmounted(volume);
    int result = format_volume(volume);

    if (is_cache) {
        while (head) {
            FILE* f = fopen_path(head->name, "wb");
            if (f) {
                fwrite(head->data, 1, head->st.st_size, f);
                fclose(f);
                chmod(head->name, head->st.st_mode);
                chown(head->name, head->st.st_uid, head->st.st_gid);
            }
            free(head->name);
            free(head->data);
            saved_log_file* temp = head->next;
            free(head);
            head = temp;
        }

        // Any part of the log we'd copied to cache is now gone.
        // Reset the pointer so we copy from the beginning of the temp
        // log.
        tmplog_offset = 0;
        copy_logs();
    }

    return result;
}

static const char**
prepend_title(const char* const* headers) {
    // count the number of lines in our title, plus the
    // caller-provided headers.
    int count = 3;   // our title has 3 lines
    const char* const* p;
    for (p = headers; *p; ++p, ++count);

    const char** new_headers = (const char**)malloc((count+1) * sizeof(char*));
    const char** h = new_headers;
    *(h++) = "Android system recovery <" EXPAND(RECOVERY_API_VERSION) "e>";
    *(h++) = recovery_version;
    *(h++) = "";
    for (p = headers; *p; ++p, ++h) *h = *p;
    *h = NULL;

    return new_headers;
}

static int
get_menu_selection(const char* const * headers, const char* const * items,
                   int menu_only, int initial_selection, Device* device) {
    // throw away keys pressed previously, so user doesn't
    // accidentally trigger menu items.
    ui->FlushKeys();

    ui->StartMenu(headers, items, initial_selection);
    int selected = initial_selection;
    int chosen_item = -1;

    while (chosen_item < 0) {
        int key = ui->WaitKey();
        int visible = ui->IsTextVisible();

        if (key == -1) {   // ui_wait_key() timed out
            if (ui->WasTextEverVisible()) {
                continue;
            } else {
                LOGI("timed out waiting for key input; rebooting.\n");
                ui->EndMenu();
                return 0; // XXX fixme
            }
        }

        int action = device->HandleMenuKey(key, visible);

        if (action < 0) {
            switch (action) {
                case Device::kHighlightUp:
                    --selected;
                    selected = ui->SelectMenu(selected);
                    break;
                case Device::kHighlightDown:
                    ++selected;
                    selected = ui->SelectMenu(selected);
                    break;
                case Device::kInvokeItem:
                    chosen_item = selected;
                    break;
                case Device::kNoAction:
                    break;
            }
        } else if (!menu_only) {
            chosen_item = action;
        }
    }

    ui->EndMenu();
    return chosen_item;
}

static int compare_string(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

// Returns a malloc'd path, or NULL.
static char*
browse_directory(const char* path, Device* device) {
    ensure_path_mounted(path);

    const char* MENU_HEADERS[] = { "Choose a package to install:",
                                   path,
                                   "",
                                   NULL };
    DIR* d;
    struct dirent* de;
    d = opendir(path);
    if (d == NULL) {
        LOGE("error opening %s: %s\n", path, strerror(errno));
        return NULL;
    }

    const char** headers = prepend_title(MENU_HEADERS);

    int d_size = 0;
    int d_alloc = 10;
    char** dirs = (char**)malloc(d_alloc * sizeof(char*));
    int z_size = 1;
    int z_alloc = 10;
    char** zips = (char**)malloc(z_alloc * sizeof(char*));
    zips[0] = strdup("../");

    while ((de = readdir(d)) != NULL) {
        int name_len = strlen(de->d_name);

        if (de->d_type == DT_DIR) {
            // skip "." and ".." entries
            if (name_len == 1 && de->d_name[0] == '.') continue;
            if (name_len == 2 && de->d_name[0] == '.' &&
                de->d_name[1] == '.') continue;

            if (d_size >= d_alloc) {
                d_alloc *= 2;
                dirs = (char**)realloc(dirs, d_alloc * sizeof(char*));
            }
            dirs[d_size] = (char*)malloc(name_len + 2);
            strcpy(dirs[d_size], de->d_name);
            dirs[d_size][name_len] = '/';
            dirs[d_size][name_len+1] = '\0';
            ++d_size;
        } else if (de->d_type == DT_REG &&
                   name_len >= 4 &&
                   strncasecmp(de->d_name + (name_len-4), ".zip", 4) == 0) {
            if (z_size >= z_alloc) {
                z_alloc *= 2;
                zips = (char**)realloc(zips, z_alloc * sizeof(char*));
            }
            zips[z_size++] = strdup(de->d_name);
        }
    }
    closedir(d);

    qsort(dirs, d_size, sizeof(char*), compare_string);
    qsort(zips, z_size, sizeof(char*), compare_string);

    // append dirs to the zips list
    if (d_size + z_size + 1 > z_alloc) {
        z_alloc = d_size + z_size + 1;
        zips = (char**)realloc(zips, z_alloc * sizeof(char*));
    }
    memcpy(zips + z_size, dirs, d_size * sizeof(char*));
    free(dirs);
    z_size += d_size;
    zips[z_size] = NULL;

    char* result;
    int chosen_item = 0;
    while (true) {
        chosen_item = get_menu_selection(headers, zips, 1, chosen_item, device);

        char* item = zips[chosen_item];
        int item_len = strlen(item);
        if (chosen_item == 0) {          // item 0 is always "../"
            // go up but continue browsing (if the caller is update_directory)
            result = NULL;
            break;
        }

        char new_path[PATH_MAX];
        strlcpy(new_path, path, PATH_MAX);
        strlcat(new_path, "/", PATH_MAX);
        strlcat(new_path, item, PATH_MAX);

        if (item[item_len-1] == '/') {
            // recurse down into a subdirectory
            new_path[strlen(new_path)-1] = '\0';  // truncate the trailing '/'
            result = browse_directory(new_path, device);
            if (result) break;
        } else {
            // selected a zip file: return the malloc'd path to the caller.
            result = strdup(new_path);
            break;
        }
    }

    int i;
    for (i = 0; i < z_size; ++i) free(zips[i]);
    free(zips);
    free(headers);

    return result;
}

static void
wipe_data(int confirm, Device* device) {
    if (confirm) {
        static const char** title_headers = NULL;

        if (title_headers == NULL) {
            const char* headers[] = { "Confirm wipe of all user data?",
                                      "  THIS CAN NOT BE UNDONE.",
                                      "",
                                      NULL };
            title_headers = prepend_title((const char**)headers);
        }

        const char* items[] = { " No",
                                " No",
                                " No",
                                " No",
                                " No",
                                " No",
                                " No",
                                " Yes -- delete all user data",   // [7]
                                " No",
                                " No",
                                " No",
                                NULL };

        int chosen_item = get_menu_selection(title_headers, items, 1, 0, device);
        if (chosen_item != 7) {
            return;
        }
    }

    ui->Print("\n-- Wiping data...\n");
    device->WipeData();
    erase_volume("/data");
    erase_volume("/cache");
    erase_persistent_partition();
    ui->Print("Data wipe complete.\n");
}

static void file_to_ui(const char* fn) {
    FILE *fp = fopen_path(fn, "re");
    if (fp == NULL) {
        ui->Print("  Unable to open %s: %s\n", fn, strerror(errno));
        return;
    }
    char line[1024];
    int ct = 0;
    redirect_stdio("/dev/null");
    while(fgets(line, sizeof(line), fp) != NULL) {
        ui->Print("%s", line);
        ct++;
        if (ct % 30 == 0) {
            // give the user time to glance at the entries
            ui->WaitKey();
        }
    }
    redirect_stdio(TEMPORARY_LOG_FILE);
    fclose(fp);
}

static void choose_recovery_file(Device* device) {
    int i;
    static const char** title_headers = NULL;
    char *filename;
    const char* headers[] = { "Select file to view",
                              "",
                              NULL };
    char* entries[KEEP_LOG_COUNT + 2];
    memset(entries, 0, sizeof(entries));

    for (i = 0; i < KEEP_LOG_COUNT; i++) {
        char *filename;
        if (asprintf(&filename, (i==0) ? LAST_LOG_FILE : (LAST_LOG_FILE ".%d"), i) == -1) {
            // memory allocation failure - return early. Should never happen.
            return;
        }
        if ((ensure_path_mounted(filename) != 0) || (access(filename, R_OK) == -1)) {
            free(filename);
            entries[i+1] = NULL;
            break;
        }
        entries[i+1] = filename;
    }

    entries[0] = strdup("Go back");
    title_headers = prepend_title((const char**)headers);

    while(1) {
        int chosen_item = get_menu_selection(title_headers, entries, 1, 0, device);
        if (chosen_item == 0) break;
        file_to_ui(entries[chosen_item]);
    }

    for (i = 0; i < KEEP_LOG_COUNT + 1; i++) {
        free(entries[i]);
    }
}

// Return REBOOT, SHUTDOWN, or REBOOT_BOOTLOADER.  Returning NO_ACTION
// means to take the default, which is to reboot or shutdown depending
// on if the --shutdown_after flag was passed to recovery.
static Device::BuiltinAction
prompt_and_wait(Device* device, int status) {
    const char* const* headers = prepend_title(device->GetMenuHeaders());

    for (;;) {
        finish_recovery(NULL);
        switch (status) {
            case INSTALL_SUCCESS:
            case INSTALL_NONE:
                ui->SetBackground(RecoveryUI::NO_COMMAND);
                break;

            case INSTALL_ERROR:
            case INSTALL_CORRUPT:
                ui->SetBackground(RecoveryUI::ERROR);
                break;
        }
        ui->SetProgressType(RecoveryUI::EMPTY);

        int chosen_item = get_menu_selection(headers, device->GetMenuItems(), 0, 0, device);

        // device-specific code may take some action here.  It may
        // return one of the core actions handled in the switch
        // statement below.
        Device::BuiltinAction chosen_action = device->InvokeMenuItem(chosen_item);

        int wipe_cache = 0;
        switch (chosen_action) {
            case Device::NO_ACTION:
                break;

            case Device::REBOOT:
            case Device::SHUTDOWN:
            case Device::REBOOT_BOOTLOADER:
                return chosen_action;

            case Device::WIPE_DATA:
                wipe_data(ui->IsTextVisible(), device);
                if (!ui->IsTextVisible()) return Device::NO_ACTION;
                break;

            case Device::WIPE_CACHE:
                ui->Print("\n-- Wiping cache...\n");
                erase_volume("/cache");
                ui->Print("Cache wipe complete.\n");
                if (!ui->IsTextVisible()) return Device::NO_ACTION;
                break;

            case Device::APPLY_EXT: {
                ensure_path_mounted(SDCARD_ROOT);
                char* path = browse_directory(SDCARD_ROOT, device);
                if (path == NULL) {
                    ui->Print("\n-- No package file selected.\n", path);
                    break;
                }

                ui->Print("\n-- Install %s ...\n", path);
                set_sdcard_update_bootloader_message();
                void* token = start_sdcard_fuse(path);

                int status = install_package(FUSE_SIDELOAD_HOST_PATHNAME, &wipe_cache,
                                             TEMPORARY_INSTALL_FILE, false);

                finish_sdcard_fuse(token);
                ensure_path_unmounted(SDCARD_ROOT);

                if (status == INSTALL_SUCCESS && wipe_cache) {
                    ui->Print("\n-- Wiping cache (at package request)...\n");
                    if (erase_volume("/cache")) {
                        ui->Print("Cache wipe failed.\n");
                    } else {
                        ui->Print("Cache wipe complete.\n");
                    }
                }

                if (status >= 0) {
                    if (status != INSTALL_SUCCESS) {
                        ui->SetBackground(RecoveryUI::ERROR);
                        ui->Print("Installation aborted.\n");
                    } else if (!ui->IsTextVisible()) {
                        return Device::NO_ACTION;  // reboot if logs aren't visible
                    } else {
                        ui->Print("\nInstall from sdcard complete.\n");
                    }
                }
                break;
            }

            case Device::APPLY_CACHE:
                ui->Print("\nAPPLY_CACHE is deprecated.\n");
                break;

            case Device::READ_RECOVERY_LASTLOG:
                choose_recovery_file(device);
                break;

            case Device::APPLY_ADB_SIDELOAD:
                status = apply_from_adb(ui, &wipe_cache, TEMPORARY_INSTALL_FILE);
                if (status >= 0) {
                    if (status != INSTALL_SUCCESS) {
                        ui->SetBackground(RecoveryUI::ERROR);
                        ui->Print("Installation aborted.\n");
                        copy_logs();
                    } else if (!ui->IsTextVisible()) {
                        return Device::NO_ACTION;  // reboot if logs aren't visible
                    } else {
                        ui->Print("\nInstall from ADB complete.\n");
                    }
                }
                break;
        }
    }
}

static void
print_property(const char *key, const char *name, void *cookie) {
    printf("%s=%s\n", key, name);
}

static void
load_locale_from_cache() {
    FILE* fp = fopen_path(LOCALE_FILE, "r");
    char buffer[80];
    if (fp != NULL) {
        fgets(buffer, sizeof(buffer), fp);
        int j = 0;
        unsigned int i;
        for (i = 0; i < sizeof(buffer) && buffer[i]; ++i) {
            if (!isspace(buffer[i])) {
                buffer[j++] = buffer[i];
            }
        }
        buffer[j] = 0;
        locale = strdup(buffer);
        check_and_fclose(fp, LOCALE_FILE);
    }
}

static RecoveryUI* gCurrentUI = NULL;

void
ui_print(const char* format, ...) {
    char buffer[256];

    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, sizeof(buffer), format, ap);
    va_end(ap);

    if (gCurrentUI != NULL) {
        gCurrentUI->Print("%s", buffer);
    } else {
        fputs(buffer, stdout);
    }
}


// hufeng.mao@generalmobi.com 2014-08-14 add for ROCK GOTA
#ifdef GMT_RECOVERY_MODE_ROCK_GOTA_SUPPORT
/*
 * GeneralMobile ROCK GOTA Integration Functions
 */
extern int gmt_skip_verify;
int gmt_uafs_result = 0;
#define GMT_PATH_MAX						1024
#define GMT_DEFAULT_DP_PARTTION		"/cache"
#define GMT_DEFAULT_DP_LOCATION 	"/cache/rock/update"
#define GMT_DEFAULT_RESULT_LOCATION 	"/cache/rock/result"
#define GMT_DEFAULT_DP_PARTTION_SDCARD		"/sdcard"
#define GMT_DEFAULT_DP_LOCATION_SDCARD 	"/sdcard/rock/update"
#define GMT_DEFAULT_UA_PARTTION	 	"/system"
#define GMT_DEFAULT_UA_LOCATION 	"/system/bin/uafs"
#define GMT_DEFAULT_UA_LOCATION_B 	"/system/bin/uafs.backup"
#define GMT_DEFAULT_LOG_PARTTION	"/cache"

#define GMT_DEFAULT_LOCAL_UP_LOCATION	"/tmp/update-gmobi.zip"
#define GMT_DEFAULT_WIPE_FILE_LOCATION	"/tmp/wipe-gmobi.cfg"

#define GMT_STATE_PARTTION			"/cache"
#define GMT_STATE_PATH				"/cache/rock"
//#define GMT_STATE_BACKUP_PARTTION	"/sdcard"
//#define GMT_STATE_BACKUP_PATH		"/sdcard/gmt_backup"

//for not can not mount sdcard
#define GMT_STATE_BACKUP_PARTTION	"/cache"
#define GMT_STATE_BACKUP_PATH		"/cache/gmt_backup"
#define GMT_BUFFER_SIZE				1024*10

static void gmt_set_recovery_bootmessage(void) {
    struct bootloader_message boot;
    memset(&boot, 0, sizeof(boot));

    LOGI("Setting recovery boot...\n");

    strlcpy(boot.command, "boot-recovery", sizeof(boot.command));
    strlcpy(boot.recovery, "recovery\n", sizeof(boot.recovery));

    set_bootloader_message(&boot);

    LOGI("boot.command=%s\n",boot.command);
    LOGI("boot.recovery=%s\n",boot.recovery);
}

static void gmt_reset_recovery_bootmessage(void) {
    struct bootloader_message boot;

    memset(&boot, 0, sizeof(boot));

    LOGI("Resetting recovery boot...\n");

    set_bootloader_message(&boot);

    LOGI("boot.command=%s\n",boot.command);
    LOGI("boot.recovery=%s\n",boot.recovery);
}

static int gmt_copy_file(const char* src_file, const char* dst_file) {
	FILE* in = fopen(src_file, "rb");
	FILE* out = fopen(dst_file, "wb");
	char buf[GMT_BUFFER_SIZE] = {0};
	int readn = 0;
	if(in && out) {
		while((readn = fread(buf, 1, GMT_BUFFER_SIZE, in)) > 0) {
			fwrite(buf, 1, readn, out);
		}
		fclose(in);
		fclose(out);
		return 0;
	}
	LOGI("gmt_copy_file error: in=%p, out=%p", in, out);
	return -1;
}

static int gmt_copy_dir(const char* src_path, const char* dst_path) {
	int count = 0;
	LOGI("copy_dir:%s to %s\n", src_path, dst_path);
	struct dirent **namelist;
	int file_num = scandir(src_path, &namelist, 0, 0);
	if(file_num < 0) {
		;
	}
	else {
		int i = 0;
		for(; i<file_num; i++) {
			char src_name[PATH_MAX];
			char dst_name[PATH_MAX];
			struct stat st;
			
			sprintf(src_name, "%s/%s", src_path, namelist[i]->d_name);
			sprintf(dst_name, "%s/%s", dst_path, namelist[i]->d_name);
			
			lstat(src_name, &st);
			if(S_ISREG(st.st_mode)) {
				if(strstr(dst_name, "vdm_update")) {
					LOGI("skip cp %s", dst_name);
				}
				else {
					dirCreateHierarchy(dst_name, 0777, NULL, 1, sehandle);
					gmt_copy_file(src_name, dst_name);
					LOGI("cp %s\n to %s\n", src_name, dst_name);
					if(access(dst_name, F_OK) == 0) {
						count++;
					}
					else {
						LOGI("copy %s fail \n", dst_name);
					}
				}
			}
			free(namelist[i]);
		}
		free(namelist);
	}
	sync();
	LOGI("[%d] files copy success! \n", count);
	return count;
}
static int gmt_wipe_data = 0;
static int gmt_backup_state() {
	char dp_path[PATH_MAX];
	strlcpy(dp_path, GMT_STATE_PARTTION, PATH_MAX);
	if (ensure_path_mounted(dp_path) != 0) {
        	LOGI("Cannot mount %s\n", dp_path);
        	return -1;
    }
	
	strlcpy(dp_path, GMT_STATE_BACKUP_PARTTION, PATH_MAX);
	if (ensure_path_mounted(dp_path) != 0) {
        	LOGI("Cannot mount %s\n", dp_path);
        	return -1;
    }
	
	gmt_copy_dir(GMT_STATE_PATH, GMT_STATE_BACKUP_PATH);
	
	return 0;
}


static int gmt_check_wipe_file(void) {
	char dp_path[PATH_MAX];
	
	strlcpy(dp_path, GMT_DEFAULT_WIPE_FILE_LOCATION, PATH_MAX);
	
	if (access(dp_path, F_OK) != 0) {
        LOGI("wipe file not exist %s\n", dp_path);
        return -1;
    }

    LOGI("wipe file name: %s\n", dp_path);
    return 0;
}


static int gmt_check_local_up(void) {
	char dp_path[PATH_MAX];

	strlcpy(dp_path, GMT_DEFAULT_LOCAL_UP_LOCATION, PATH_MAX);
	
	if (access(dp_path, F_OK) != 0) {
        LOGI("local update package not exist %s\n", dp_path);
        return -1;
    }

    LOGI("local update package name: %s\n", dp_path);
    return 0;
}

static int gmt_check_dp(void) {
	char dp_path[PATH_MAX];
	strlcpy(dp_path, GMT_DEFAULT_UA_PARTTION, PATH_MAX);
	if (ensure_path_mounted(dp_path) != 0) {
        	LOGI("Cannot mount %s\n", dp_path);
        	return -1;
    	}

	strlcpy(dp_path, GMT_DEFAULT_LOG_PARTTION, PATH_MAX);
	if (ensure_path_mounted(dp_path) != 0) {
            LOGI("Cannot mount %s\n", dp_path);
        }

	strlcpy(dp_path, GMT_DEFAULT_DP_PARTTION, PATH_MAX);
	if (ensure_path_mounted(dp_path) != 0) {
            LOGI("Cannot mount %s\n", dp_path);
	    return -1;
        }
//for not can not mount sdcard
	/*strlcpy(dp_path, GMT_DEFAULT_DP_PARTTION_SDCARD, PATH_MAX);
	if (ensure_path_mounted(dp_path) != 0) {
	    LOGI("Cannot mount %s\n", dp_path);
	    return -1;
	}
	*/
	strlcpy(dp_path, GMT_DEFAULT_DP_LOCATION, PATH_MAX);
	
	if (access(dp_path, F_OK) != 0) {
        LOGI("Delta package does not exist %s\n", dp_path);

//for not can not mount sdcard
		/*strlcpy(dp_path, GMT_DEFAULT_DP_LOCATION_SDCARD, PATH_MAX);
		if (access(dp_path, F_OK) != 0){
		LOGI("Delta package does not exist %s\n", dp_path);
		return -1;
		}*/
			return -1;
    }

    LOGI("Delta package path name: %s\n", dp_path);
    return 0;
}
static int gmt_start_fota_ua(void) {
	pid_t pid;
	int status;
	char ua_path[PATH_MAX];
	strlcpy(ua_path, GMT_DEFAULT_UA_PARTTION, PATH_MAX);
	// mount GMT_DEFAULT_DP_PARTTION
	if (ensure_path_mounted(ua_path) != 0) {
        	LOGI("Cannot mount %s\n", ua_path);
        	return -1;
    	}
	// test uafs.backup 
	strlcpy(ua_path, GMT_DEFAULT_UA_LOCATION_B, PATH_MAX);
	if (access(ua_path, F_OK) != 0) {
		strlcpy(ua_path, GMT_DEFAULT_UA_LOCATION, PATH_MAX);
	}
	if((pid = fork()) < 0) {
		LOGE("fork error:%s", strerror(errno));
		return -1;
	}
	else if(pid == 0) {		// child
		const char *child_arg = NULL; 
	//	Volume* v = volume_for_path("/boot");
	//	if(v) {
	//		child_arg = v->device;
	//	}
		execlp(ua_path, ua_path, child_arg, NULL);
		LOGE("couldn't execute: %s, %s", ua_path, strerror(errno));
		return -1;
	}
	
	if((pid = ::waitpid(pid, &status, 0)) < 0) {
		LOGE("waitpid error:%s", strerror(errno));
	}
	LOGI("child return code:%d", WEXITSTATUS(status));
	gmt_uafs_result = WEXITSTATUS(status);
	return 0;
}

static int gmt_handle_fota(void) {
	
	// Proceed with normal GOTA if return is -1
	if (gmt_check_dp() == -1) {  
		ensure_path_unmounted(GMT_DEFAULT_UA_PARTTION);
		ensure_path_unmounted(GMT_DEFAULT_DP_PARTTION);
		ensure_path_unmounted(GMT_DEFAULT_LOG_PARTTION);
		return -1;
	}
	
	// set boot CMD to reocvery mode next time
	gmt_set_recovery_bootmessage();
	LOGI("gabriel1...\n");
        int ret = gmt_start_fota_ua();
	LOGI("gabriel2..%d.\n",ret);
	// if gmt_start_fota_ua() return, whatever return value,retset boot CMD.
	gmt_reset_recovery_bootmessage();
	
		
	sync(); // For good measure.
	
	ensure_path_unmounted(GMT_DEFAULT_UA_PARTTION);
	ensure_path_unmounted(GMT_DEFAULT_DP_PARTTION);
	ensure_path_unmounted(GMT_DEFAULT_LOG_PARTTION);
	
	return 0;
	
}
#endif
int
main(int argc, char **argv) {
    time_t start = time(NULL);

    redirect_stdio(TEMPORARY_LOG_FILE);

    // If this binary is started with the single argument "--adbd",
    // instead of being the normal recovery binary, it turns into kind
    // of a stripped-down version of adbd that only supports the
    // 'sideload' command.  Note this must be a real argument, not
    // anything in the command file or bootloader control block; the
    // only way recovery should be run with this argument is when it
    // starts a copy of itself from the apply_from_adb() function.
    if (argc == 2 && strcmp(argv[1], "--adbd") == 0) {
        adb_main();
        return 0;
    }

    printf("Starting recovery (pid %d) on %s", getpid(), ctime(&start));

    load_volume_table();
    ensure_path_mounted(LAST_LOG_FILE);
    rotate_last_logs(KEEP_LOG_COUNT);
    get_args(&argc, &argv);

    const char *send_intent = NULL;
    const char *update_package = NULL;

//Tinno:CJ	
#ifdef TINNO_MULTI_UPDATE_PACKAGE
    const char *update_packages[100] = {NULL};
    int update_packages_num = 0;
#endif

    int wipe_data = 0, wipe_cache = 0, show_text = 0;
    bool just_exit = false;
    bool shutdown_after = false;

    int arg;
    while ((arg = getopt_long(argc, argv, "", OPTIONS, NULL)) != -1) {
        switch (arg) {
        case 's': send_intent = optarg; break;
//Tinno:CJ
#ifdef TINNO_MULTI_UPDATE_PACKAGE        
	case 'u': update_packages[update_packages_num++] = optarg; break;
#else		
        case 'u': update_package = optarg; break;
#endif
        case 'w': wipe_data = wipe_cache = 1; break;
        case 'c': wipe_cache = 1; break;
        case 't': show_text = 1; break;
        case 'x': just_exit = true; break;
        case 'l': locale = optarg; break;
        case 'g': {
            if (stage == NULL || *stage == '\0') {
                char buffer[20] = "1/";
                strncat(buffer, optarg, sizeof(buffer)-3);
                stage = strdup(buffer);
            }
            break;
        }
        case 'p': shutdown_after = true; break;
        case 'r': reason = optarg; break;
        case '?':
            LOGE("Invalid command argument\n");
            continue;
        }
    }

    if (locale == NULL) {
        load_locale_from_cache();
    }
    printf("locale is [%s]\n", locale);
    printf("stage is [%s]\n", stage);
    printf("reason is [%s]\n", reason);

    Device* device = make_device();
    ui = device->GetUI();
    gCurrentUI = ui;

    ui->SetLocale(locale);
    ui->Init();

    int st_cur, st_max;
    if (stage != NULL && sscanf(stage, "%d/%d", &st_cur, &st_max) == 2) {
        ui->SetStage(st_cur, st_max);
    }

    ui->SetBackground(RecoveryUI::NONE);
    if (show_text) ui->ShowText(true);

    struct selinux_opt seopts[] = {
      { SELABEL_OPT_PATH, "/file_contexts" }
    };

    sehandle = selabel_open(SELABEL_CTX_FILE, seopts, 1);

    if (!sehandle) {
        ui->Print("Warning: No file_contexts\n");
    }

    device->StartRecovery();

    printf("Command:");
    for (arg = 0; arg < argc; arg++) {
        printf(" \"%s\"", argv[arg]);
    }
    printf("\n");

#ifdef GMT_RECOVERY_MODE_ROCK_GOTA_SUPPORT
    // check delta update
if(argc == 1) {

#ifdef SUPPORT_SBOOT_UPDATE
	LOGI("sec_mark_status(true)\n");
	sec_mark_status(true);
#endif

	if(gmt_handle_fota() == 0) {
	LOGI("gabriel3...\n");
		int result = gmt_uafs_result;
		if(result == 0 && gmt_check_local_up() == 0) {
			update_package = strdup(GMT_DEFAULT_LOCAL_UP_LOCATION);
			LOGI("update_package = %s", update_package);
			gmt_skip_verify = 1;
			
			if(gmt_check_wipe_file() == 0) {
				gmt_backup_state();
				gmt_wipe_data = 1;
				wipe_data = 1;
			}
			else {
				wipe_data = 0;
			}
			
			sync();
		}
		else {
		
#ifdef SUPPORT_SBOOT_UPDATE
		LOGI("sec_update(true)\n");
		sec_update(true);
#endif
			
			LOGI("android_reboot(ANDROID_RB_RESTART)\n");

			LOGI("Rebooting...\n");
			sleep(1);
			
			finish_recovery(NULL);
			//#! Reboot function for 5.0
			property_set(ANDROID_RB_PROPERTY, "reboot,");
			//#! Reboot function for 4.X
			//android_reboot(ANDROID_RB_RESTART, 0, 0);
			
			//#! Reboot function for 2.X
			//reboot(RB_AUTOBOOT);
			
			return EXIT_SUCCESS;
		}
	}
}
#endif

//Tinno:CJ
#ifdef TINNO_MULTI_UPDATE_PACKAGE        
update_package_again:
	printf("update_packages_num:%d\n",update_packages_num); 	
	if(update_packages_num > 0)
	{
		update_package = update_packages[--update_packages_num];
		printf("update_package:%s\n",update_package);		
	}
#endif


    if (update_package) {
        // For backwards compatibility on the cache partition only, if
        // we're given an old 'root' path "CACHE:foo", change it to
        // "/cache/foo".
        if (strncmp(update_package, "CACHE:", 6) == 0) {
            int len = strlen(update_package) + 10;
            char* modified_path = (char*)malloc(len);
            strlcpy(modified_path, "/cache/", len);
            strlcat(modified_path, update_package+6, len);
            printf("(replacing path \"%s\" with \"%s\")\n",
                   update_package, modified_path);
            update_package = modified_path;
        }
    }
    printf("\n");

    property_list(print_property, NULL);
    property_get("ro.build.display.id", recovery_version, "");
    printf("\n");

    int status = INSTALL_SUCCESS;

    if (update_package != NULL) {
        status = install_package(update_package, &wipe_cache, TEMPORARY_INSTALL_FILE, true);
        if (status == INSTALL_SUCCESS && wipe_cache) {
            if (erase_volume("/cache")) {
                LOGE("Cache wipe (requested by package) failed.");
            }
        }
        if (status != INSTALL_SUCCESS) {
            ui->Print("Installation aborted.\n");

            // If this is an eng or userdebug build, then automatically
            // turn the text display on if the script fails so the error
            // message is visible.
            char buffer[PROPERTY_VALUE_MAX+1];
            property_get("ro.build.fingerprint", buffer, "");
            if (strstr(buffer, ":userdebug/") || strstr(buffer, ":eng/")) {
                ui->ShowText(true);
            }
        }
    } else if (wipe_data) {
        if (device->WipeData()) status = INSTALL_ERROR;
        if (erase_volume("/data")) status = INSTALL_ERROR;
        if (wipe_cache && erase_volume("/cache")) status = INSTALL_ERROR;
        if (erase_persistent_partition() == -1 ) status = INSTALL_ERROR;
        if (status != INSTALL_SUCCESS) ui->Print("Data wipe failed.\n");
    } else if (wipe_cache) {
        if (wipe_cache && erase_volume("/cache")) status = INSTALL_ERROR;
        if (status != INSTALL_SUCCESS) ui->Print("Cache wipe failed.\n");
    } else if (!just_exit) {
        status = INSTALL_NONE;  // No command specified
        ui->SetBackground(RecoveryUI::NO_COMMAND);
    }

    if (status == INSTALL_ERROR || status == INSTALL_CORRUPT) {
        copy_logs();
        ui->SetBackground(RecoveryUI::ERROR);
    }
    Device::BuiltinAction after = shutdown_after ? Device::SHUTDOWN : Device::REBOOT;
    if (status != INSTALL_SUCCESS || ui->IsTextVisible()) {
        ui->ShowText(true);
        Device::BuiltinAction temp = prompt_and_wait(device, status);
        if (temp != Device::NO_ACTION) after = temp;
    }

//Tinno:CJ
#ifdef TINNO_MULTI_UPDATE_PACKAGE        
	if(update_packages_num > 0 && status == INSTALL_SUCCESS)
		goto update_package_again;
#endif


    // Save logs and clean up before rebooting or shutting down.
    finish_recovery(send_intent);

    switch (after) {
        case Device::SHUTDOWN:
            ui->Print("Shutting down...\n");
            property_set(ANDROID_RB_PROPERTY, "shutdown,");
            break;

        case Device::REBOOT_BOOTLOADER:
            ui->Print("Rebooting to bootloader...\n");
            property_set(ANDROID_RB_PROPERTY, "reboot,bootloader");
            break;

        default:
            ui->Print("Rebooting...\n");
            property_set(ANDROID_RB_PROPERTY, "reboot,");
            break;
    }
    sleep(5); // should reboot before this finishes
    return EXIT_SUCCESS;
}