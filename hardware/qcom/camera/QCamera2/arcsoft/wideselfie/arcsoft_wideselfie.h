/*----------------------------------------------------------------------------------------------
*
* This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary and 		
* confidential information. 
* 
* The information and code contained in this file is only for authorized ArcSoft employees 
* to design, create, modify, or review.
* 
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER AUTHORIZATION.
* 
* If you are not an intended recipient of this file, you must not copy, distribute, modify, 
* or take any action in reliance on it. 
* 
* If you have received this file in error, please immediately notify ArcSoft and 
* permanently delete the original and any copy of any file and any printout thereof.
*
*-----------------------------------------------------------------------------------------------*/
/*
* 
*
* Description:  arcsoft_wideselfie.h
*
*
* History
*
*-----------------------------------------------------------------------------------------------*/

#ifndef ARCSOFT_WIDE_SELFIE_H
#define ARCSOFT_WIDE_SELFIE_H

//#define ARCSOFT_WIDESELFIE_SUPPORT

#include "amcomdef.h"
#include "asvloffscreen.h"


#ifdef __cplusplus
extern "C" {
#endif

// AWS: ArcSoft WideSelfie

#define IN
#define OUT
#define INOUT

#ifdef ARCSOFT_WIDESELFIE_EXPORTS/**/
#define AWS_API	__declspec(dllexport)
#else
#define AWS_API
#endif

// ******************** Constants ******************** //
// Error Code
#define AWS_MERR_BASE					0X7000
#define AWS_ERR_DIRECTION				(AWS_MERR_BASE + 1)
#define	AWS_ERR_ABNORMAL_OFFSET			(AWS_MERR_BASE + 3)
#define AWS_ERR_MOVE_TOO_FAST			(AWS_MERR_BASE + 4)
#define	AWS_ERR_BLURRED_IMAGE			(AWS_MERR_BASE + 5)
#define	AWS_ERR_PARALLAX_OVERFLOW		(AWS_MERR_BASE + 6)
#define	AWS_ERR_UNABLE_CALCULATE		(AWS_MERR_BASE + 7)

// Warning Code
#define AWS_MWARN_BASE					0X8000
#define	AWS_WARN_MOVE_SLOPE_TO_UP		(AWS_MWARN_BASE + 1)
#define	AWS_WARN_MOVE_SLOPE_TO_DOWN		(AWS_MWARN_BASE + (1<<1))
#define	AWS_WARN_DEVICE_ROTATE			(AWS_MWARN_BASE + (1<<4))
#define	AWS_WARN_DEVICE_MOVE_FRONT_BACK	(AWS_MWARN_BASE + (1<<5))
#define	AWS_WARN_FACE_TOO_CLOSE			(AWS_MWARN_BASE + (1<<6))
#define	AWS_WARN_FACE_CLOSE_TO_BODER	(AWS_MWARN_BASE + (1<<7))

// Mode
#define AWS_MODE_TWO_DERECTION			(1<<6)
#define AWS_MODE_CAPTURE				(1<<12)

// Direction
#define AWS_DIRECTION_UNKNOW			-1
#define AWS_DIRECTION_LEFT2RIGHT		0
#define AWS_DIRECTION_RIGHT2LEFT		1

// View Angle
#define AWS_VIEW_ANGLE_HORIZ			0
#define AWS_VIEW_ANGLE_VERT				1
#define AWS_VIEW_ANGLE_DIAG				2

// User Command
#define AWS_COMMAND_SELECT				0
#define AWS_COMMAND_STITCH				1
#define AWS_COMMAND_STOP				(1<<1)

// ***************** Data structures ***************** //
// INIT_PARAM
typedef struct tag_AWS_INIT_PARAMETER
{	
	MUInt8	*pu8Buffer;
	MInt32  i32BufferSize;

	MInt32	i32Mode;

	MFloat	fCameraViewAngleForWidth;
	MFloat	fCameraViewAngleForHeight;
	
	MUInt32	u32SrcFormat;
	MInt32  i32FullImageWidth;
	MInt32  i32FullImageHeight;
	
	//input thumbnail info
	MUInt32 u32ThumbnailFormat;
	MInt32  i32ThumbnailWidth;
	MInt32  i32ThumbnailHeight;
		
	MInt32	i32MaxResultWidth;

	MInt32	i32ProgressBarThumbHeight;
	MInt32	i32GuideStopBarThumbHeight;
	MInt32	i32GuideStableBarThumbHeight;
	MFloat	fProgressBarThumbHeightCropRatio;
	MInt32	i32ChangeDirectionThumbThreshold;

	MInt32	i32DeviceOrientation;
} AWS_INIT_PARAMETER, *LPAWS_INIT_PARAMETER;

// PROJECT_GUIDE
typedef struct tag_AWS_PROJECT_GUIDE
{	
	MPOINT	ptLeftTopOffset;
	MPOINT	ptRightTopOffset;
	MPOINT	ptLeftBottomOffset;
	MPOINT	ptRightBottomOffset;
} AWS_PROJECT_GUIDE, *LPAWS_PROJECT_GUIDE;

// VERSION INFO
typedef struct tag_AWS_VERSION_INFO
{
	MInt32 i32Codebase;	
	MInt32 i32Major;		
	MInt32 i32Minor;
	MInt32 i32Build;		
	MTChar Version[50];  
	MTChar BuildDate[50]; 
	MTChar CopyRight[50];
} AWS_VERSION_INFO, *LPAWS_VERSION_INFO;
// *************************************************** //

// ****************** API Functions ****************** //

AWS_API MInt32 AWS_CalcResultLength(
		IN	MInt32				i32SrcWidth,
		IN	MInt32				i32SrcHeight,
		IN	MFloat				fSrcViewAngle,
		IN	MInt32				i32SrcViewAngleType,
		IN	MFloat				fResultViewAngle,
		IN	MInt32				i32ResultViewAngleType,
		IN	MInt32				i32DeviceOrientation
);

AWS_API MRESULT AWS_Init(
		IN	LPAWS_INIT_PARAMETER	pInitParam,
		OUT MHandle					*phEngine
		
);

AWS_API MRESULT AWS_Process(
		IN	MHandle				*phEngine,
		IN	LPASVLOFFSCREEN		pFullSrcImage,
		IN	LPASVLOFFSCREEN		pThumbnailSrcImage,
		IN  MInt32				i32UserCommand,
		OUT LPASVLOFFSCREEN		*ppProgressBarThumbImage,	
		OUT	MRECT				*pProgressBarThumbRect,
		OUT	MPOINT				*pptProgressBarThumbOffset,
		OUT	MPOINT				*pptProgressBarThumbOffsetCapture,
		OUT LPAWS_PROJECT_GUIDE	pProjectGuide,
		OUT	MBool				*pbCapture,
		OUT LPASVLOFFSCREEN		*ppResultImage,
		OUT	MInt32				*pi32Direction,
		OUT	MInt32				*pi32Progress
);

AWS_API MRESULT AWS_Capture(
		IN	MHandle				*phEngine,
		IN	LPASVLOFFSCREEN		pFullSrcImage
);

AWS_API MRESULT AWS_Uninit(
		OUT MHandle				*phEngine
);

AWS_API MRESULT AWS_GetVersionInfo(
		OUT   LPAWS_VERSION_INFO	pVersionInfo
);


#ifdef __cplusplus
}
#endif

#endif // ARCSOFT_WIDE_SELFIE_H

