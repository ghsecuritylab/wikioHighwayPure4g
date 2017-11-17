/**
* Entry point of Android FileSystem Update.
* 2012-12-26, hufeng.mao@generalmobi.com
*
* #1,2013-05-30,notmmao,Add recovery.img update feature.
* #2,2013-12-12,notmmao,Remove uafs.log
* #3,2014-08-04,notmmao,support Intel platform
*/
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/reboot.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <fs_mgr.h>
#include "uafs.h"
#include "../common.h"
#include "../device.h"
#include "../ui.h" //ADD BY EVA
#include "../roots.h"
#if RK_IMAGE_UPDATE 
#include "../rkimage.h"
#endif
#include "gs.h"

extern "C" {
	int run_scripter_gmobi();
	int run_img_update(const char* dev, const char* file);
	int run_intel_osip_img_update(const char* path, const char* file);
}

char* locale = NULL;//ADD BY EVA

static void show_progress(int p) {
	float d = (float)p / 100.0f;
	ui_set_progress(d);
	printf("%s:%s:%f\n", __FILE__, __func__, d);
}
// 2014.8.1
int FSMain(void) {
	int argc =2;
	char* argv[argc];
	argv[0] = "uafs";
	argv[1] = DELTA_FILE;
	return gs_main(argc, argv, show_progress);
}

static void
check_and_fclose(FILE *fp, const char *name) {
	fflush(fp);
	if (ferror(fp)) printf("Error in %s\n(%s)\n", name, strerror(errno));
	fclose(fp);
}

// open a given path, mounting partitions as necessary
FILE*
fopen_path(const char *path, const char *mode) {

	FILE *fp = fopen(path, mode);
	return fp;
}

static long tmplog_offset = 0;
static void
copy_log_file(const char* source, const char* destination, int append) {
	FILE *log = fopen_path(destination, append ? "a" : "w");
	if (log == NULL) {
		printf("Can't open %s\n", destination);
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


static int save_result(int ret) {
	FILE* fp;

	printf("%s: ret: %d\n", __func__, ret);

	fp = fopen(RESULT_FILE, "w+");
	if (!fp)
	{
		printf("%s: open result file %s failed.\n", __func__, RESULT_FILE);
		return -1;
	}
	fprintf(fp, "%d", ret);
	fclose(fp);
	// Android 4.1.x, we need apply 666 for DMC.
	chmod(RESULT_FILE, 0666);
	return 0;
} 

RecoveryUI* ui = NULL;
void ui_init() {
	Device* device = make_device();
	ui = device->GetUI();
	ui->Init();
	ui->SetBackground(RecoveryUI::INSTALLING_UPDATE); //MODIFY BY EVA
	ui->SetProgressType(RecoveryUI::DETERMINATE); // notmmao 20140114
	//ui->SetProgressType(RecoveryUI::INDETERMINATE); // notmmao 20140114
	
}
void ui_show_progress(float portion, float seconds) {
	ui->ShowProgress(portion, seconds);
}
void ui_set_progress(float portion) {
	ui->SetProgress(portion);
}


// path = recovery.fstab path
// file = image file(or bin file)
int update_path_form_file(const char* path, const char* file) {
	int result = -1;
	if (access(file, F_OK) == 0) {
		printf("try update %s from %s\n", path, file);
		Volume* v = volume_for_path(path);
		if(v) {
#if MTK_IMAGE_UPDATE
			const char* device = v->device;
#else
			const char* device = v->blk_device;
#endif
			printf("need update %s\n", file);
			result = run_img_update(device, file);
			printf("%s update result...%d\n", file, result);
		}
		else {
			printf("can not load %s volume\n", path);
		}
	}
	return result;
}

#if INTEL_OSIP_IMAGE_UPDATE
int update_osip_form_file(const char* path, const char* file) {
	int result = -1;
	if (access(file, F_OK) == 0) {
		printf("try update %s from %s\n", path, file);
		result = run_intel_osip_img_update(path, file);
	}
	return result;
}
#endif


void
ui_print(const char* format, ...) {
    char buffer[1024];

    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, sizeof(buffer), format, ap);
    va_end(ap);


    fputs(buffer, stdout);

}

#if QCOM_IMAGE_UPDATE
static int write_file(const char *path, const char *value)
{
	int fd, ret, len;

	fd = open(path, O_WRONLY|O_CREAT, 0622);

	if (fd < 0)
		return -errno;

	len = strlen(value);

	do {
		ret = write(fd, value, len);
	} while (ret < 0 && errno == EINTR);

	close(fd);
	if (ret < 0) {
		return -errno;
	} else {
		return 0;
	}
}
#endif

int main(int argc, char* argv[]) {
	time_t start = time(NULL);
	int result;

	freopen(TEMPORARY_LOG_FILE, "a", stdout); setbuf(stdout, NULL);
	freopen(TEMPORARY_LOG_FILE, "a", stderr); setbuf(stderr, NULL);
	
	chmod(TEMPORARY_LOG_FILE, 0644);

	ui_init();

	printf("Starting %s(%s) on %s\n", argv[0], UAFS_VERSION, ctime(&start));
#if QCOM_IMAGE_UPDATE
	/*enable the backlight*/
    write_file("/sys/class/leds/lcd-backlight/brightness", "128");
#endif
	// load volume table
	load_volume_table();
	// backup uafs if yun uafs in normal mode 
	if(strstr(argv[0], "uafs.backup") == NULL) {
		FILE *uafs = fopen(argv[0], "r");
		FILE *uafs_backup = fopen(GMT_DEFAULT_UA_LOCATION_B, "w");
		if (uafs != NULL) {
			char buf[4096];
			int readn;
		
			while ((readn = fread(buf, 1, sizeof(buf), uafs))>0) {
				fwrite(buf, 1, readn, uafs_backup);
			}
			check_and_fclose(uafs, argv[0]);
		}
		else {
			printf("Open %s error.\n", argv[0]);
		}
		check_and_fclose(uafs_backup, GMT_DEFAULT_UA_LOCATION_B);
			
		printf("Backup %s to %s success.\n", argv[0], GMT_DEFAULT_UA_LOCATION_B);
		struct stat statbuf;
		stat(argv[0], &statbuf);	
		chmod(GMT_DEFAULT_UA_LOCATION_B, statbuf.st_mode);
		printf("Chmod %s to %04o success.\n", GMT_DEFAULT_UA_LOCATION_B, statbuf.st_mode);
		sync();
	}
	
	ui_show_progress(1, 0);
	result = FSMain();
	printf("Uafs result...%d\n\n", result);

	/* after filesystem updated */
	if(result == 0) {
	
#if BRCM_BP_IMAGE_UPDATE
	struct partition* p = bcm_bp_partitions;
	while(p && p->path) {
		char file_buffer[PATH_MAX] = {0}; 
		int result = 0;
		sprintf(file_buffer, "/tmp/%s", p->img);
		result = update_path_form_file(p->path, file_buffer);
		if(result != 0) {
			memset(file_buffer, 0, PATH_MAX);
			sprintf(file_buffer, "/system/%s", p->img);
			result = update_path_form_file(p->path, file_buffer);
		}
		p++;
	}
#endif


#if QCOM_IMAGE_UPDATE
	struct partition* p = qcom_partitions;
	while(p && p->path) {
		char file_buffer[PATH_MAX] = {0}; 
		int result = 0;
		sprintf(file_buffer, "/tmp/%s", p->img);
		result = update_path_form_file(p->path, file_buffer);
		if(result != 0) {
			memset(file_buffer, 0, PATH_MAX);
			sprintf(file_buffer, "/system/%s", p->img);
			result = update_path_form_file(p->path, file_buffer);
		}
		p++;
	}
#endif

#if RK_IMAGE_UPDATE
	struct partition* p = rk_partitions;
	while(p && p->path) {
		char file_buffer[PATH_MAX] = {0}; 
		int result = 0;
		sprintf(file_buffer, "/tmp/%s", p->img);
		result = write_image_from_file(file_buffer, p->path, 0);
		if(result != 0) {
			memset(file_buffer, 0, PATH_MAX);
			sprintf(file_buffer, "/system/%s", p->img);
			result = write_image_from_file(file_buffer, p->path, 0);
		}
		p++;
	}
#endif


#if INET_IMAGE_UPDATE
	struct partition* p = inet_partitions;
	while(p && p->path) {
		char file_buffer[PATH_MAX] = {0}; 
		int result = 0;
		sprintf(file_buffer, "/tmp/%s", p->img);
		result = update_path_form_file(p->path, file_buffer);
		if(result != 0) {
			memset(file_buffer, 0, PATH_MAX);
			sprintf(file_buffer, "/system/%s", p->img);
			result = update_path_form_file(p->path, file_buffer);
		}
		p++;
	}
#endif

#if SPRD_IMAGE_UPDATE
	struct partition* p = sprd_partitions;
	while(p && p->path) {
		char file_buffer[PATH_MAX] = {0}; 
		int result = 0;
		sprintf(file_buffer, "/tmp/%s", p->img);
		result = update_path_form_file(p->path, file_buffer);
		if(result != 0) {
			memset(file_buffer, 0, PATH_MAX);
			sprintf(file_buffer, "/system/%s", p->img);
			result = update_path_form_file(p->path, file_buffer);
		}
		p++;
	}
#endif

#if MTK_IMAGE_UPDATE
	struct partition* p = mtk_partitions;
	while(p && p->path) {
		char file_buffer[PATH_MAX] = {0}; 
		int result = 0;
		sprintf(file_buffer, "/tmp/%s", p->img);
		if(access(file_buffer, F_OK) != 0) {
			memset(file_buffer, 0, PATH_MAX);
			sprintf(file_buffer, "/system/%s", p->img);
		}
		if(access(file_buffer, F_OK) == 0) {
			result = update_path_form_file(p->path, file_buffer);
			if(result !=0) {
				result = run_img_update(p->path, file_buffer);
			}
		}
		p++;
	}
#endif

#if INTEL_OSIP_IMAGE_UPDATE
		struct partition* p = intel_osip_img_partitions;
		while(p && p->path) {
			char file_buffer[PATH_MAX] = {0}; 
			int result = 0;
			sprintf(file_buffer, "/tmp/%s", p->img);
			result = update_osip_form_file(p->path, file_buffer);
			if(result != 0) {
				memset(file_buffer, 0, PATH_MAX);
				sprintf(file_buffer, "/system/%s", p->img);
				result = update_osip_form_file(p->path, file_buffer);
			}
			p++;
		}
#endif

		/* run updater_main */
#if SCRIPTER_SUPPORT
		run_scripter_gmobi();
#endif
		
		unlink(DELTA_FILE);
//for not can not mount sdcard
		//unlink(DELTA_FILE_SDCARD);
	}
	save_result(result);
	unlink(GMT_DEFAULT_UA_LOCATION_B);

	//copy_log_file(TEMPORARY_LOG_FILE, LOG_FILE, 1);
	copy_log_file(TEMPORARY_LOG_FILE, LAST_LOG_FILE, 0);
	chmod(LAST_LOG_FILE, 0644);
	chown(LAST_LOG_FILE, 1000, 1000);   // system user
	unlink(TEMPORARY_LOG_FILE);
	//chmod(LAST_LOG_FILE, 0640);
	sync();
	return result;
}

