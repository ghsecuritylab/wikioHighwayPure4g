/* Copyright (c) 2013-2014, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of The Linux Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*---------------------------------------------------------------------------
 * This file is autogenerated file using gcdb parser. Please do not edit it.
 * Update input XML file to add a new entry or update variable in this file
 * VERSION = "1.0"
 *---------------------------------------------------------------------------*/

#ifndef _PANEL_SHARP_QHD_VIDEO_H_

#define _PANEL_SHARP_QHD_VIDEO_H_
/*---------------------------------------------------------------------------*/
/* HEADER files                                                              */
/*---------------------------------------------------------------------------*/
#include "panel.h"

/*---------------------------------------------------------------------------*/
/* Panel configuration                                                       */
/*---------------------------------------------------------------------------*/

static struct panel_config sharp_qhd_video_panel_data = {
  "qcom,mdss_dsi_sharp_qhd_video", "dsi:0:", "qcom,mdss-dsi-panel",
  10, 0, "DISPLAY_1", 0, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Panel resolution                                                          */
/*---------------------------------------------------------------------------*/
static struct panel_resolution sharp_qhd_video_panel_res = {
  540, 960, 48, 80, 32, 0, 3, 15, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Panel Color Information                                                   */
/*---------------------------------------------------------------------------*/
static struct color_info sharp_qhd_video_color = {
  24, 2, 0xff, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Panel Command information                                                 */
/*---------------------------------------------------------------------------*/
static char sharp_qhd_video_on_cmd0[] = {
0x01, 0x00, 0x05, 0x80 };


static char sharp_qhd_video_on_cmd1[] = {
0x11, 0x00, 0x05, 0x80 };


static char sharp_qhd_video_on_cmd2[] = {
0x53, 0x0c, 0x15, 0x80 };


static char sharp_qhd_video_on_cmd3[] = {
0x29, 0x00, 0x05, 0x80 };


static char sharp_qhd_video_on_cmd4[] = {
0xae, 0x03, 0x15, 0x80 };


static char sharp_qhd_video_on_cmd5[] = {
0x3a, 0x77, 0x15, 0x80 };


static struct mipi_dsi_cmd sharp_qhd_video_on_command[] = {
{ 0x4 , sharp_qhd_video_on_cmd0},
{ 0x4 , sharp_qhd_video_on_cmd1},
{ 0x4 , sharp_qhd_video_on_cmd2},
{ 0x4 , sharp_qhd_video_on_cmd3},
{ 0x4 , sharp_qhd_video_on_cmd4},
{ 0x4 , sharp_qhd_video_on_cmd5}
};

#define SHARP_QHD_VIDEO_ON_COMMAND 6

static char sharp_qhd_videooff_cmd0[] = {
0x28, 0x00, 0x05, 0x80 };


static char sharp_qhd_videooff_cmd1[] = {
0x10, 0x00, 0x05, 0x80 };




static struct mipi_dsi_cmd sharp_qhd_video_off_command[] = {
{ 0x4 , sharp_qhd_videooff_cmd0},
{ 0x4 , sharp_qhd_videooff_cmd1}
};
#define SHARP_QHD_VIDEO_OFF_COMMAND 2


static struct command_state sharp_qhd_video_state = {
  0, 1
};

/*---------------------------------------------------------------------------*/
/* Command mode panel information                                            */
/*---------------------------------------------------------------------------*/

static struct commandpanel_info sharp_qhd_video_command_panel = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Video mode panel information                                              */
/*---------------------------------------------------------------------------*/

static struct videopanel_info sharp_qhd_video_video_panel = {
  1, 0, 0, 0, 1, 1, 0, 0, 0x9
};

/*---------------------------------------------------------------------------*/
/* Lane Configuration                                                        */
/*---------------------------------------------------------------------------*/

static struct lane_configuration sharp_qhd_video_lane_config = {
  2, 0, 1, 1, 0, 0
};


/*---------------------------------------------------------------------------*/
/* Panel Timing                                                              */
/*---------------------------------------------------------------------------*/
const uint32_t sharp_qhd_video_timings[] = {
  0x46, 0x1d, 0x20, 0x00, 0x39, 0x3a, 0x21, 0x21, 0x32, 0x03, 0x04, 0x00
};



static struct mipi_dsi_cmd sharp_qhd_video_rotation[] = {

};
#define SHARP_QHD_VIDEO_ROTATION 0


static struct panel_timing sharp_qhd_video_timing_info = {
  4, 4, 0x04, 0x1c
};

static struct panel_reset_sequence sharp_qhd_video_panel_reset_seq = {
{ 1, 0, 1, }, { 20, 200, 20, }, 2
};

/*---------------------------------------------------------------------------*/
/* Backlight Settings                                                        */
/*---------------------------------------------------------------------------*/

static struct backlight sharp_qhd_video_backlight = {
  1, 1, 4095, 100, 1, "PMIC_8941"
};


#endif /*_PANEL_SHARP_QHD_VIDEO_H_*/