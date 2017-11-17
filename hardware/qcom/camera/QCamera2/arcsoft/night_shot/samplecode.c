/*******************************************************************************
Copyright(c) ArcSoft, All right reserved.

This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary 
and confidential information. 

The information and code contained in this file is only for authorized ArcSoft 
employees to design, create, modify, or review.

DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER 
AUTHORIZATION.

If you are not an intended recipient of this file, you must not copy, 
distribute, modify, or take any action in reliance on it. 

If you have received this file in error, please immediately notify ArcSoft and 
permanently delete the original and any copy of any file and any printout 
thereof.
*******************************************************************************/
#include "arcsoft_night_shot.h"
#include "ammem.h"

// Please replace this function by your code
MVoid LoadImage(MLong *plWidth, MLong *plHeight, MByte **ppImgData)
{
	// Load image information and data here
}

int main()
{
	MRESULT res = MOK;
	MLong nMemSize = 10 * 1024 * 1024;
	MVoid *pMem = MNull;
	MHandle hMemMgr = MNull, hEnhancer = MNull;
	ANS_INPUTINFO SrcInput;
	ASVLOFFSCREEN SrcImgs[MAX_INPUT_IMAGES];
	ASVLOFFSCREEN DstImg;
	ANS_PARAM param;

	MLong i, lWidth, lHeight;
	MByte *pImgData;

	// Fill source images off-screen
	for(i = 0; i < MAX_INPUT_IMAGES; i++)
	{
		LoadImage(&lWidth, &lHeight, &pImgData);

		SrcImgs[i].i32Width = lWidth;
		SrcImgs[i].i32Height = lHeight;
		SrcImgs[i].u32PixelArrayFormat = ASVL_PAF_I420;
		SrcImgs[i].pi32Pitch[0] = (lWidth + 3) & 0xfffffffc;
		SrcImgs[i].pi32Pitch[1] = (lWidth/2 + 3) & 0xfffffffc;
		SrcImgs[i].pi32Pitch[2] = SrcImgs[i].pi32Pitch[1];
		SrcImgs[i].ppu8Plane[0] = pImgData;
		SrcImgs[i].ppu8Plane[1] = SrcImgs[i].ppu8Plane[0] + lHeight*SrcImgs[i].pi32Pitch[0];
		SrcImgs[i].ppu8Plane[2] = SrcImgs[i].ppu8Plane[1] + (lHeight/2)*SrcImgs[i].pi32Pitch[1];
	}
	// Fill source images off-screen
	DstImg.i32Width = lWidth;
	DstImg.i32Height = lHeight;
	DstImg.u32PixelArrayFormat = ASVL_PAF_I420;
	DstImg.pi32Pitch[0] = (lWidth + 3) & 0xfffffffc;
	DstImg.pi32Pitch[1] = (lWidth/2 + 3) & 0xfffffffc;
	DstImg.pi32Pitch[2] = DstImg.pi32Pitch[1];
	DstImg.ppu8Plane[0] =(MByte*)MMemAlloc(MNull, 
		lHeight * (DstImg.pi32Pitch[0] + DstImg.pi32Pitch[1]));
	DstImg.ppu8Plane[1] = DstImg.ppu8Plane[0] + lHeight*DstImg.pi32Pitch[0];
	DstImg.ppu8Plane[2] = DstImg.ppu8Plane[1] + (lHeight/2)*DstImg.pi32Pitch[1];

	pMem = MMemAlloc(MNull, nMemSize);
	hMemMgr = MMemMgrCreate(pMem, nMemSize);

	res = ANS_Init(hMemMgr, &hEnhancer);
	if(MOK != res)
		goto exit;

	SrcInput.lImgNum = 0;
	for(i = 0; i < MAX_INPUT_IMAGES; i++)
	{
		SrcInput.lImgNum++;
		SrcInput.pImages[i] = &SrcImgs[i];
	}

	ANS_GetDefaultParam(&param);

	res = ANS_Enhancement(hEnhancer, &SrcInput, &DstImg, &param, MNull, MNull);
	if(MOK != res)
		goto exit;

	// Save DstImg to file or other operations

exit:
	ANS_Uninit(&hEnhancer);
	if(hMemMgr) MMemMgrDestroy(hMemMgr);
	if(pMem) MMemFree(MNull, pMem);
	if(DstImg.ppu8Plane[0])
		MMemFree(MNull, DstImg.ppu8Plane[0]);
	for(i = 0; i < MAX_INPUT_IMAGES; i++) {
		if(SrcImgs[i].ppu8Plane[0])
			MMemFree(MNull, SrcImgs[i].ppu8Plane[0]);
	}
	return res;
}
