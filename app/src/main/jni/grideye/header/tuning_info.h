/*
* tuning_info.h
*
* Created: 2017/04/24 
*  Author: HowardJHLi
*/
/*******************************************************************************
* Copyright(C) by 2016 FIH - HowardJHLi.
******************************************************************************/

#ifndef TUNING_INFO_H_
#define TUNING_INFO_H_


/*******************************************************************************
include file
*******************************************************************************/
#include	"common.h"
#include	"human_detection.h"


#define	TUNING_FRAME	(1000)

#define TEMP_RANGE_1	(21)
#define TEMP_RANGE_2	(24)
#define TEMP_RANGE_3	(27)
#define TEMP_RANGE_4	(30)

#define FIT_HUMAN_AREA	(10)
#define MIN_DIFF_TEMP	(0.3f)
#define MAX_DIFF_TEMP	(0.7f)

typedef struct tagTuningFeature {
	float fAveDiffTemp;
	float fMaxDiffTemp;
	float fMinDiffTemp;
	float fAveBackTemp;
	USHORT usHumanArea;
	USHORT usHumanHeadArea;
	Rect sRectangle;
	Coordinate sCenter;
} TuningFeature;

typedef struct tagTuningInfo {
	BOOLEAN bAuto;
	ConfigParameters sConfig;
	TuningFeature sTuningFeature;
} TuningInfo;




void initTuningInfo();
void updateTuningInfo(HumanDetectInfo*);
void updateTuningFeature(HumanDetectInfo*);
float getAveDiffTemp(short*);
float getMaxDiffTemp(short*);
float getMinDiffTemp(short*);
float getBgDiffTemp(short*);
float calcTemp(float, UCHAR);
float calMaxDiffTemp(float, float);
float calMinDiffTemp(float, float);

#endif /* TUNING_INFO_H_ */