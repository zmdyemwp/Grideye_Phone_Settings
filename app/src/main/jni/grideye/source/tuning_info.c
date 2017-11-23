/*******************************************************************************
* Copyright(C) by 2016 Howard JH Li
******************************************************************************/
//#include	"stdafx.h"
/*******************************************************************************
�C?�N?�[�h�t�@�C?
*******************************************************************************/
#include <jni.h>
#include "../header/common.h"
#include "../header/human_detection.h"
#include "../header/tuning_info.h"


TuningInfo sTuningInfo;
extern ConfigParameters g_sConfigParameters;	/* for get config parameters	*/

UCHAR frame;
BOOLEAN isInitDiffTemp;

void initFeature(TuningFeature* psTuningFeature){
	psTuningFeature->fAveDiffTemp = 0.0f;
	psTuningFeature->fMaxDiffTemp = 0.0f;
	psTuningFeature->fMinDiffTemp = 0.0f;
	psTuningFeature->fAveBackTemp = 0.0f;
	psTuningFeature->usHumanArea = 0;
	psTuningFeature->usHumanHeadArea = 0;

	Coordinate sG;
	sG.fx = 0.0f;
	sG.fy = 0.0f;
	psTuningFeature->sCenter = sG;

	Rect sRec;
	sRec.ucMaxX = 0;
	sRec.ucMaxY = 0;
	sRec.ucMinX = 0;
	sRec.ucMinY = 0;
	psTuningFeature->sRectangle = sRec;
}

void initTuningInfo()
{
	sTuningInfo.bAuto = FALSE;
	sTuningInfo.sConfig = g_sConfigParameters;

	initFeature(&sTuningInfo.sTuningFeature);

	frame = 0;
	isInitDiffTemp = 0;
}


void updateTuningInfo(HumanDetectInfo* psHDI){
	
	updateTuningFeature(&psHDI);

	// diff temp thresh
    

	// labeling 


	// range
	

}


void updateTuningFeature(HumanDetectInfo* psHDI)
{
	TuningFeature* sTF;
	sTF = &sTuningInfo.sTuningFeature;

	//if (psHDI->ucFrameNum > (TUNING_FRAME + BACK_FRAME_NUM)) return;
	//if (psHDI->ucFrameNum < BACK_FRAME_NUM) return;
	if (psHDI->ucFrameNum == BACK_FRAME_NUM) {
		frame = psHDI->ucFrameNum;
		return;
	}
	

	UCHAR ucAveDiffFrame = psHDI->ucFrameNum - BACK_FRAME_NUM;
	UCHAR ucAveBgFrame = psHDI->ucFrameNum - BACK_FRAME_NUM;



	// calc fAveDiffTemp
	sTF->fAveDiffTemp = (ucAveDiffFrame - 1)*sTF->fAveDiffTemp + getAveDiffTemp(psHDI->shDiffTemp);
	if (sTF->fAveDiffTemp != 0.0f)
	{
		sTF->fAveDiffTemp = calcTemp(sTF->fAveDiffTemp, ucAveDiffFrame);
	}

	// calc fMaxDiffTemp
	sTF->fMaxDiffTemp = calMaxDiffTemp(sTF->fMaxDiffTemp, getMaxDiffTemp(psHDI->shDiffTemp));

	// calc fMinDiffTemp
	sTF->fMinDiffTemp = calMinDiffTemp(sTF->fMinDiffTemp, getMinDiffTemp(psHDI->shDiffTemp));

	// calc fAveBackTemp
	sTF->fAveBackTemp = (ucAveBgFrame - 1) * sTF->fAveBackTemp + getBgDiffTemp(psHDI->shBackTemp);
	if (sTF->fAveBackTemp != 0.0f)
	{
		sTF->fAveBackTemp = calcTemp(sTF->fAveBackTemp, ucAveBgFrame);
	}

	if (isInitDiffTemp != 1) {
		initialDiffTemp(sTF->fAveBackTemp, &sTuningInfo.sConfig);
	} else {
		tunningDiffTemp(psHDI->sFeature->usArea, &sTuningInfo.sConfig);
	}
}

void initialDiffTemp(float pfAveBackTemp, ConfigParameters* psConfig){
	if ( pfAveBackTemp <= TEMP_RANGE_1 ){
		psConfig->sDetectParameters.fDiffTempThresh = MIN_DIFF_TEMP;
	}
	else if (pfAveBackTemp > TEMP_RANGE_1 && pfAveBackTemp <= TEMP_RANGE_2){
		psConfig->sDetectParameters.fDiffTempThresh = MIN_DIFF_TEMP + 0.1f;
	}
	else if (pfAveBackTemp > TEMP_RANGE_2 && pfAveBackTemp <= TEMP_RANGE_3){
		psConfig->sDetectParameters.fDiffTempThresh = MIN_DIFF_TEMP + 0.1f;
	}
	else if (pfAveBackTemp > TEMP_RANGE_3 && pfAveBackTemp <= TEMP_RANGE_4){
		psConfig->sDetectParameters.fDiffTempThresh = MIN_DIFF_TEMP + 0.1f;
	}
	else{
		psConfig->sDetectParameters.fDiffTempThresh = MAX_DIFF_TEMP;
	}
	isInitDiffTemp = 1;
}

void tunningDiffTemp(short pusArea, ConfigParameters* psConfig){
	if (pusArea > FIT_HUMAN_AREA + 2){
		if (psConfig->sDetectParameters.fDiffTempThresh == MAX_DIFF_TEMP) return;
		psConfig->sDetectParameters.fDiffTempThresh + 0.1;
		psConfig->sDetectParameters.fDiffTempThresh = MIN(psConfig->sDetectParameters.fDiffTempThresh, MAX_DIFF_TEMP);
	}
	else if (pusArea < FIT_HUMAN_AREA - 2){
		if (psConfig->sDetectParameters.fDiffTempThresh == MIN_DIFF_TEMP) return;
		psConfig->sDetectParameters.fDiffTempThresh - 0.1;
		psConfig->sDetectParameters.fDiffTempThresh = MAX(psConfig->sDetectParameters.fDiffTempThresh, MIN_DIFF_TEMP);
	}

}


float getAveDiffTemp(short* psDiffTemp)
{
	float result = 0.0f;


	for (int i = 0; i < IMG_SZ; i++)
	{
		result += psDiffTemp[i]/IMG_SZ;
	}

	//result = result / IMG_SZ;
	
	return result;
}

float getMaxDiffTemp(short* psDiffTemp)
{
	float result = 0.0f;

	for (int i = 0; i < IMG_SZ; i++)
	{
		if (psDiffTemp[i] > 200) continue;
		if (psDiffTemp[i] > result)
			result = psDiffTemp[i];
	}
	
	return result;
}

float getMinDiffTemp(short* psDiffTemp)
{
	float result = 0.0f;

	result = psDiffTemp[0];
	for (int i = 0; i < IMG_SZ; i++)
	{
		if (psDiffTemp[i] < result)
			result = psDiffTemp[i];
	}
	
	return result;
}

float getBgDiffTemp(short* sBgTemp)
{
	float result = 0.0f;

	for (int i = 0; i < IMG_SZ; i++)
	{
		result += sBgTemp[i];
	}

	result = result / IMG_SZ;
	
	return result;
}

float calcTemp(float fValue, UCHAR frame)
{
	return fValue / frame;
}
float calMaxDiffTemp(float fValue_new, float fValue_old)
{
	return fValue_new > fValue_old ? fValue_new : fValue_old;
}
float calMinDiffTemp(float fValue_new, float fValue_old)
{
	return fValue_new < fValue_old ? fValue_new : fValue_old;
}