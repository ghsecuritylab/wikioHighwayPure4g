/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
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

#ifndef _PANEL_OTM1284A_720P_VIDEO_H_
#define _PANEL_OTM1284A_720P_VIDEO_H_
/*---------------------------------------------------------------------------*/
/* HEADER files                                                              */
/*---------------------------------------------------------------------------*/
#include "panel.h"

/*---------------------------------------------------------------------------*/
/* Panel configuration                                                       */
/*---------------------------------------------------------------------------*/
static struct panel_config otm1284a_720p_video_panel_data = {
	"qcom,mdss_dsi_otm1284a_720p_video", "dsi:0:", "qcom,mdss-dsi-panel",
	10, 0, "DISPLAY_1", 0, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Panel resolution                                                          */
/*---------------------------------------------------------------------------*/
static struct panel_resolution otm1284a_720p_video_panel_res = {
	720, 1280, 44, 42, 2, 0, 16, 14, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Panel color information                                                   */
/*---------------------------------------------------------------------------*/
static struct color_info otm1284a_720p_video_color = {
	24, 0, 0xff, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Panel on/off command information                                          */
/*---------------------------------------------------------------------------*/
static char otm1284a_720p_video_on_cmd0[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x00, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd1[] = {
	0x04, 0x00, 0x39, 0xC0,
	0xFF, 0x12, 0x84, 0x01,
};

static char otm1284a_720p_video_on_cmd2[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x80, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd3[] = {
	0x03, 0x00, 0x39, 0xC0,
	0xFF, 0x12, 0x84, 0xFF,
};

static char otm1284a_720p_video_on_cmd4[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x91, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd5[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xB0, 0x92, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd6[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x80, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd7[] = {
	0x0A, 0x00, 0x39, 0xC0,
	0xC0, 0x00, 0x64, 0x00,
	0x0F, 0x11, 0x00, 0x64,
	0x0F, 0x11, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd8[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x90, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd9[] = {
	0x07, 0x00, 0x39, 0xC0,
	0xC0, 0x00, 0x5C, 0x00,
	0x01, 0x00, 0x04, 0xFF,
};

static char otm1284a_720p_video_on_cmd10[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xA4, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd11[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xC0, 0x00, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd12[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB3, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd13[] = {
	0x03, 0x00, 0x39, 0xC0,
	0xC0, 0x00, 0x55, 0xFF,
};

static char otm1284a_720p_video_on_cmd14[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x81, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd15[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xC1, 0x55, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd16[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x90, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd17[] = {
	0x05, 0x00, 0x39, 0xC0,
	0xF5, 0x02, 0x11, 0x02,
	0x15, 0xFF, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd18[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x90, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd19[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xC5, 0x50, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd20[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x94, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd21[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xC5, 0x66, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd22[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB2, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd23[] = {
	0x03, 0x00, 0x39, 0xC0,
	0xF5, 0x00, 0x00, 0xFF,
};

static char otm1284a_720p_video_on_cmd24[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB4, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd25[] = {
	0x03, 0x00, 0x39, 0xC0,
	0xF5, 0x15, 0x11, 0xFF,	//<Line><FFBAKK-105><20141118>update parameter of LCD;xiongdajun
};

static char otm1284a_720p_video_on_cmd26[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB6, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd27[] = {
	0x03, 0x00, 0x39, 0xC0,
	0xF5, 0x00, 0x00, 0xFF,
};

static char otm1284a_720p_video_on_cmd28[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB8, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd29[] = {
	0x03, 0x00, 0x39, 0xC0,
	0xF5, 0x15, 0x11, 0xFF,	//<Line><FFBAKK-105><20141118>update parameter of LCD;xiongdajun
};

static char otm1284a_720p_video_on_cmd30[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x94, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd31[] = {
	0x03, 0x00, 0x39, 0xC0,
	0xF5, 0x00, 0x00, 0xFF,
};

static char otm1284a_720p_video_on_cmd32[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xD2, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd33[] = {
	0x03, 0x00, 0x39, 0xC0,
	0xF5, 0x06, 0x15, 0xFF,
};

static char otm1284a_720p_video_on_cmd34[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB4, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd35[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xC5, 0xCC, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd36[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xA0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd37[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xC4, 0x05, 0x10, 0x06,
	0x02, 0x05, 0x15, 0x10,
	0x05, 0x10, 0x07, 0x02,
	0x05, 0x15, 0x10, 0xFF,
};

static char otm1284a_720p_video_on_cmd38[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd39[] = {
	0x03, 0x00, 0x39, 0xC0,
	0xC4, 0x00, 0x00, 0xFF,
};

static char otm1284a_720p_video_on_cmd40[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x91, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd41[] = {
	0x03, 0x00, 0x39, 0xC0,
	0xC5, 0x19, 0x52, 0xFF,
};

static char otm1284a_720p_video_on_cmd42[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x00, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd43[] = {
	0x03, 0x00, 0x39, 0xC0,
	0xD8, 0xBC, 0xBC, 0xFF,
};

static char otm1284a_720p_video_on_cmd44[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB3, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd45[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xC5, 0x84, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd46[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xBB, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd47[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xC5, 0x8A, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd48[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x82, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd49[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xC4, 0x0A, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd50[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB2, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd51[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xC5, 0x40, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd52[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x81, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd53[] = {
	0x03, 0x00, 0x39, 0xC0,
	0xC4, 0x82, 0x0A, 0xFF,
};

static char otm1284a_720p_video_on_cmd54[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xC6, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd55[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xB0, 0x03, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd56[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xC2, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd57[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xF5, 0x40, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd58[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xC3, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd59[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xF5, 0x85, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd60[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x00, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd61[] = {
	0x15, 0x00, 0x39, 0xC0,
	0xE1, 0x00, 0x09, 0x10,
	0x1B, 0x27, 0x36, 0x38,
	0x68, 0x59, 0x78, 0x8B,
	0x76, 0x85, 0x5D, 0x55,
	0x47, 0x35, 0x20, 0x10,
	0x00, 0xFF, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd62[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x00, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd63[] = {
	0x15, 0x00, 0x39, 0xC0,
	0xE2, 0x00, 0x09, 0x10,
	0x1B, 0x27, 0x36, 0x38,
	0x68, 0x59, 0x78, 0x8b,
	0x76, 0x85, 0x5d, 0x55,
	0x47, 0x35, 0x20, 0x10,
	0x00, 0xFF, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd64[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x80, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd65[] = {
	0x0C, 0x00, 0x39, 0xC0,
	0xCB, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

static char otm1284a_720p_video_on_cmd66[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x90, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd67[] = {
	0x10, 0x00, 0x39, 0xC0,
	0xCB, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

static char otm1284a_720p_video_on_cmd68[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xA0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd69[] = {
	0x10, 0x00, 0x39, 0xC0,
	0xCB, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

static char otm1284a_720p_video_on_cmd70[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd71[] = {
	0x10, 0x00, 0x39, 0xC0,
	0xCB, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

static char otm1284a_720p_video_on_cmd72[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xC0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd73[] = {
	0x10, 0x00, 0x39, 0xC0,
	0xCB, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

static char otm1284a_720p_video_on_cmd74[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xD0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd75[] = {
	0x10, 0x00, 0x39, 0xC0,
	0xCB, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x00, 0x00,
};

static char otm1284a_720p_video_on_cmd76[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xE0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd77[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xCB, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x05, 0x05, 0xFF,
};

static char otm1284a_720p_video_on_cmd78[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xF0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd79[] = {
	0x0C, 0x00, 0x39, 0xC0,
	0xCB, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,

};

static char otm1284a_720p_video_on_cmd80[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x80, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd81[] = {
	0x10, 0x00, 0x39, 0xC0,
	0xCC, 0x0A, 0x0C, 0x0E,
	0x10, 0x02, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

static char otm1284a_720p_video_on_cmd82[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x90, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd83[] = {
	0x10, 0x00, 0x39, 0xC0,
	0xCC, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x2E, 0x2D,
	0x09, 0x0B, 0x0D, 0x0F,
	0x01, 0x03, 0x00, 0x00,
};

static char otm1284a_720p_video_on_cmd84[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xA0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd85[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xCC, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x2E, 0x2D, 0xFF,
};

static char otm1284a_720p_video_on_cmd86[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd87[] = {
	0x10, 0x00, 0x39, 0xC0,
	0xCC, 0x0F, 0x0D, 0x0B,
	0x09, 0x03, 0x01, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

static char otm1284a_720p_video_on_cmd88[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xC0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd89[] = {
	0x10, 0x00, 0x39, 0xC0,
	0xCC, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x2D, 0x2E,
	0x10, 0x0E, 0x0C, 0x0A,
	0x04, 0x02, 0x00, 0x00,
};

static char otm1284a_720p_video_on_cmd90[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xD0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd91[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xCC, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x2D, 0x2E, 0xFF,
};

static char otm1284a_720p_video_on_cmd92[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x80, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd93[] = {
	0x0D, 0x00, 0x39, 0xC0,
	0xCE, 0x8D, 0x03, 0x00,
	0x8C, 0x03, 0x00, 0x8B,
	0x03, 0x00, 0x8A, 0x03,
	0x00, 0xFF, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd94[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x90, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd95[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xCE, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF,
};

static char otm1284a_720p_video_on_cmd96[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xA0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd97[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xCE, 0x38, 0x0B, 0x85,
	0x00, 0x8D, 0x03, 0x00,	//<Line><FFBAKK-105><20141118>update parameter of LCD;xiongdajun
	0x38, 0x0A, 0x85, 0x01,
	0x8D, 0x03, 0x00, 0xFF,	//<Line><FFBAKK-105><20141118>update parameter of LCD;xiongdajun
};

static char otm1284a_720p_video_on_cmd98[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd99[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xCE, 0x38, 0x09, 0x85,
	0x02, 0x8D, 0x03, 0x00,	//<Line><FFBAKK-105><20141118>update parameter of LCD;xiongdajun
	0x38, 0x08, 0x85, 0x03,
	0x8D, 0x03, 0x00, 0xFF,	//<Line><FFBAKK-105><20141118>update parameter of LCD;xiongdajun
};

static char otm1284a_720p_video_on_cmd100[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xC0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd101[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xCE, 0x38, 0x07, 0x85,
	0x04, 0x8D, 0x03, 0x00,	//<Line><FFBAKK-105><20141118>update parameter of LCD;xiongdajun
	0x38, 0x06, 0x85, 0x05,
	0x8D, 0x03, 0x00, 0xFF,	//<Line><FFBAKK-105><20141118>update parameter of LCD;xiongdajun
};

static char otm1284a_720p_video_on_cmd102[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xD0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd103[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xCE, 0x38, 0x05, 0x85,
	0x06, 0x8D, 0x03, 0x00,	//<Line><FFBAKK-105><20141118>update parameter of LCD;xiongdajun
	0x38, 0x04, 0x85, 0x07,
	0x8D, 0x03, 0x00, 0xFF,	//<Line><FFBAKK-105><20141118>update parameter of LCD;xiongdajun
};

static char otm1284a_720p_video_on_cmd104[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x80, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd105[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xCF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF,
};

static char otm1284a_720p_video_on_cmd106[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x90, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd107[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xCF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF,
};

static char otm1284a_720p_video_on_cmd108[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xA0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd109[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xCF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF,
};

static char otm1284a_720p_video_on_cmd110[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd111[] = {
	0x0F, 0x00, 0x39, 0xC0,
	0xCF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF,
};

static char otm1284a_720p_video_on_cmd112[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xC0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd113[] = {
	0x0C, 0x00, 0x39, 0xC0,
	0xCF, 0x01, 0x01, 0x20,
	0x20, 0x00, 0x00, 0x01,
	0x01, 0x00, 0x00, 0x00,
};

static char otm1284a_720p_video_on_cmd114[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xB5, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd115[] = {
	0x07, 0x00, 0x39, 0xC0,
	0xC5, 0x33, 0xF1, 0xFF,
	0x33, 0xF1, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd116[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x92, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd117[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xb3, 0x06, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd118[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x90, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd119[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xf6, 0x00, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd120[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0xa0, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd121[] = {
	0x02, 0x00, 0x39, 0xC0,
	0xc1, 0x02, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd122[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x00, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd123[] = {
	0x04, 0x00, 0x39, 0xC0,
	0xFF, 0xFF, 0xFF, 0xFF,

};

static char otm1284a_720p_video_on_cmd124[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x00, 0x00, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd125[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x36, 0x00, 0xFF, 0xFF,
};

static char otm1284a_720p_video_on_cmd126[] = {
	0x11, 0x00, 0x05, 0x80
};

static char otm1284a_720p_video_on_cmd127[] = {
	0x29, 0x00, 0x05, 0x80
};

static struct mipi_dsi_cmd otm1284a_720p_video_on_command[] = {
	{0x8, otm1284a_720p_video_on_cmd0, 0x00},
	{0x8, otm1284a_720p_video_on_cmd1, 0x00},
	{0x8, otm1284a_720p_video_on_cmd2, 0x00},
	{0x8, otm1284a_720p_video_on_cmd3, 0x00},
	{0x8, otm1284a_720p_video_on_cmd4, 0x00},
	{0x8, otm1284a_720p_video_on_cmd5, 0x00},
	{0x8, otm1284a_720p_video_on_cmd6, 0x00},
	{0x10, otm1284a_720p_video_on_cmd7, 0x00},
	{0x8, otm1284a_720p_video_on_cmd8, 0x00},
	{0xc, otm1284a_720p_video_on_cmd9, 0x00},
	{0x8, otm1284a_720p_video_on_cmd10, 0x00},
	{0x8, otm1284a_720p_video_on_cmd11, 0x00},
	{0x8, otm1284a_720p_video_on_cmd12, 0x00},
	{0x8, otm1284a_720p_video_on_cmd13, 0x00},
	{0x8, otm1284a_720p_video_on_cmd14, 0x00},
	{0x8, otm1284a_720p_video_on_cmd15, 0x00},
	{0x8, otm1284a_720p_video_on_cmd16, 0x00},
	{0xc, otm1284a_720p_video_on_cmd17, 0x00},
	{0x8, otm1284a_720p_video_on_cmd18, 0x00},
	{0x8, otm1284a_720p_video_on_cmd19, 0x00},
	{0x8, otm1284a_720p_video_on_cmd20, 0x00},
	{0x8, otm1284a_720p_video_on_cmd21, 0x00},
	{0x8, otm1284a_720p_video_on_cmd22, 0x00},
	{0x8, otm1284a_720p_video_on_cmd23, 0x00},
	{0x8, otm1284a_720p_video_on_cmd24, 0x00},
	{0x8, otm1284a_720p_video_on_cmd25, 0x00},
	{0x8, otm1284a_720p_video_on_cmd26, 0x00},
	{0x8, otm1284a_720p_video_on_cmd27, 0x00},
	{0x8, otm1284a_720p_video_on_cmd28, 0x00},
	{0x8, otm1284a_720p_video_on_cmd29, 0x00},
	{0x8, otm1284a_720p_video_on_cmd30, 0x00},
	{0x8, otm1284a_720p_video_on_cmd31, 0x00},
	{0x8, otm1284a_720p_video_on_cmd32, 0x00},
	{0x8, otm1284a_720p_video_on_cmd33, 0x00},
	{0x8, otm1284a_720p_video_on_cmd34, 0x00},
	{0x8, otm1284a_720p_video_on_cmd35, 0x00},
	{0x8, otm1284a_720p_video_on_cmd36, 0x00},
	{0x14, otm1284a_720p_video_on_cmd37, 0x00},
	{0x8, otm1284a_720p_video_on_cmd38, 0x00},
	{0x8, otm1284a_720p_video_on_cmd39, 0x00},
	{0x8, otm1284a_720p_video_on_cmd40, 0x00},
	{0x8, otm1284a_720p_video_on_cmd41, 0x00},
	{0x8, otm1284a_720p_video_on_cmd42, 0x00},
	{0x8, otm1284a_720p_video_on_cmd43, 0x00},
	{0x8, otm1284a_720p_video_on_cmd44, 0x00},
	{0x8, otm1284a_720p_video_on_cmd45, 0x00},
	{0x8, otm1284a_720p_video_on_cmd46, 0x00},
	{0x8, otm1284a_720p_video_on_cmd47, 0x00},
	{0x8, otm1284a_720p_video_on_cmd48, 0x00},
	{0x8, otm1284a_720p_video_on_cmd49, 0x00},
	{0x8, otm1284a_720p_video_on_cmd50, 0x00},
	{0x8, otm1284a_720p_video_on_cmd51, 0x00},
	{0x8, otm1284a_720p_video_on_cmd52, 0x00},
	{0x8, otm1284a_720p_video_on_cmd53, 0x00},
	{0x8, otm1284a_720p_video_on_cmd54, 0x00},
	{0x8, otm1284a_720p_video_on_cmd55, 0x00},
	{0x8, otm1284a_720p_video_on_cmd56, 0x00},
	{0x8, otm1284a_720p_video_on_cmd57, 0x00},
	{0x8, otm1284a_720p_video_on_cmd58, 0x00},
	{0x8, otm1284a_720p_video_on_cmd59, 0x00},
	{0x8, otm1284a_720p_video_on_cmd60, 0x00},
	{0x1c, otm1284a_720p_video_on_cmd61, 0x00},
	{0x8, otm1284a_720p_video_on_cmd62, 0x00},
	{0x1c, otm1284a_720p_video_on_cmd63, 0x00},
	{0x8, otm1284a_720p_video_on_cmd64, 0x00},
	{0x10, otm1284a_720p_video_on_cmd65, 0x00},
	{0x8, otm1284a_720p_video_on_cmd66, 0x00},
	{0x14, otm1284a_720p_video_on_cmd67, 0x00},
	{0x8, otm1284a_720p_video_on_cmd68, 0x00},
	{0x14, otm1284a_720p_video_on_cmd69, 0x00},
	{0x8, otm1284a_720p_video_on_cmd70, 0x00},
	{0x14, otm1284a_720p_video_on_cmd71, 0x00},
	{0x8, otm1284a_720p_video_on_cmd72, 0x00},
	{0x14, otm1284a_720p_video_on_cmd73, 0x00},
	{0x8, otm1284a_720p_video_on_cmd74, 0x00},
	{0x14, otm1284a_720p_video_on_cmd75, 0x00},
	{0x8, otm1284a_720p_video_on_cmd76, 0x00},
	{0x14, otm1284a_720p_video_on_cmd77, 0x00},
	{0x8, otm1284a_720p_video_on_cmd78, 0x00},
	{0x10, otm1284a_720p_video_on_cmd79, 0x00},
	{0x8, otm1284a_720p_video_on_cmd80, 0x00},
	{0x14, otm1284a_720p_video_on_cmd81, 0x00},
	{0x8, otm1284a_720p_video_on_cmd82, 0x00},
	{0x14, otm1284a_720p_video_on_cmd83, 0x00},
	{0x8, otm1284a_720p_video_on_cmd84, 0x00},
	{0x14, otm1284a_720p_video_on_cmd85, 0x00},
	{0x8, otm1284a_720p_video_on_cmd86, 0x00},
	{0x14, otm1284a_720p_video_on_cmd87, 0x00},
	{0x8, otm1284a_720p_video_on_cmd88, 0x00},
	{0x14, otm1284a_720p_video_on_cmd89, 0x00},
	{0x8, otm1284a_720p_video_on_cmd90, 0x00},
	{0x14, otm1284a_720p_video_on_cmd91, 0x00},
	{0x8, otm1284a_720p_video_on_cmd92, 0x00},
	{0x14, otm1284a_720p_video_on_cmd93, 0x00},
	{0x8, otm1284a_720p_video_on_cmd94, 0x00},
	{0x14, otm1284a_720p_video_on_cmd95, 0x00},
	{0x8, otm1284a_720p_video_on_cmd96, 0x00},
	{0x14, otm1284a_720p_video_on_cmd97, 0x00},
	{0x8, otm1284a_720p_video_on_cmd98, 0x00},
	{0x14, otm1284a_720p_video_on_cmd99, 0x00},
	{0x8, otm1284a_720p_video_on_cmd100, 0x00},
	{0x14, otm1284a_720p_video_on_cmd101, 0x00},
	{0x8, otm1284a_720p_video_on_cmd102, 0x00},
	{0x14, otm1284a_720p_video_on_cmd103, 0x00},
	{0x8, otm1284a_720p_video_on_cmd104, 0x00},
	{0x14, otm1284a_720p_video_on_cmd105, 0x00},
	{0x8, otm1284a_720p_video_on_cmd106, 0x00},
	{0x14, otm1284a_720p_video_on_cmd107, 0x00},
	{0x8, otm1284a_720p_video_on_cmd108, 0x00},
	{0x14, otm1284a_720p_video_on_cmd109, 0x00},
	{0x8, otm1284a_720p_video_on_cmd110, 0x00},
	{0x14, otm1284a_720p_video_on_cmd111, 0x00},
	{0x8, otm1284a_720p_video_on_cmd112, 0x00},
	{0x10, otm1284a_720p_video_on_cmd113, 0x00},
	{0x8, otm1284a_720p_video_on_cmd114, 0x00},
	{0xc, otm1284a_720p_video_on_cmd115, 0x00},
	{0x8, otm1284a_720p_video_on_cmd116, 0x00},
	{0x8, otm1284a_720p_video_on_cmd117, 0x00},
	{0x8, otm1284a_720p_video_on_cmd118, 0x00},
	{0x8, otm1284a_720p_video_on_cmd119, 0x00},
	{0x8, otm1284a_720p_video_on_cmd120, 0x00},
	{0x8, otm1284a_720p_video_on_cmd121, 0x00},
	{0x8, otm1284a_720p_video_on_cmd122, 0x00},
	{0x8, otm1284a_720p_video_on_cmd123, 0x00},
	{0x8, otm1284a_720p_video_on_cmd124, 0x00},
	{0x8, otm1284a_720p_video_on_cmd125, 0x00},
	{0x4, otm1284a_720p_video_on_cmd126, 0x96},
	{0x4, otm1284a_720p_video_on_cmd127, 0x1E}
};

#define OTM1284A_720P_VIDEO_ON_COMMAND 128


static char otm1284a_720p_videooff_cmd0[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x28, 0x00, 0xFF, 0xFF,
};

static char otm1284a_720p_videooff_cmd1[] = {
	0x02, 0x00, 0x39, 0xC0,
	0x10, 0x00, 0xFF, 0xFF,
};

static struct mipi_dsi_cmd otm1284a_720p_video_off_command[] = {
	{0x8, otm1284a_720p_videooff_cmd0, 0x14},
	{0x8, otm1284a_720p_videooff_cmd1, 0x78}
};

#define OTM1284A_720P_VIDEO_OFF_COMMAND 2


static struct command_state otm1284a_720p_video_state = {
	0, 1
};

/*---------------------------------------------------------------------------*/
/* Command mode panel information                                            */
/*---------------------------------------------------------------------------*/
static struct commandpanel_info otm1284a_720p_video_command_panel = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Video mode panel information                                              */
/*---------------------------------------------------------------------------*/
static struct videopanel_info otm1284a_720p_video_video_panel = {
	1, 0, 0, 0, 1, 1, 2, 0, 0x9
};

/*---------------------------------------------------------------------------*/
/* Lane configuration                                                        */
/*---------------------------------------------------------------------------*/
static struct lane_configuration otm1284a_720p_video_lane_config = {
	4, 0, 1, 1, 1, 1
};

/*---------------------------------------------------------------------------*/
/* Panel timing                                                              */
/*---------------------------------------------------------------------------*/
static const uint32_t otm1284a_720p_video_timings[] = {
	0x75, 0x18, 0x10, 0x00, 0x3A, 0x3E, 0x14, 0x1A, 0x13, 0x03, 0x04, 0x00
};

static struct panel_timing otm1284a_720p_video_timing_info = {
	0, 4, 0x2c, 0x20
};

/*---------------------------------------------------------------------------*/
/* Panel reset sequence                                                      */
/*---------------------------------------------------------------------------*/
static struct panel_reset_sequence otm1284a_720p_video_reset_seq = {
	{1, 0, 1, }, {20, 20, 120, }, 2
};

/*---------------------------------------------------------------------------*/
/* Backlight setting                                                         */
/*---------------------------------------------------------------------------*/
static struct backlight otm1284a_720p_video_backlight = {
	1, 1, 255, 0, 0, "PMIC_8916"
};

#endif /*_PANEL_OTM1284A_720P_VIDEO_H_*/