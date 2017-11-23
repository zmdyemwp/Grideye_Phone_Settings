/*******************************************************************************
 * Copyright(C) by 2016 FIH - HowardJHLi.
 ******************************************************************************/
//#include	"stdafx.h"
/*******************************************************************************
	include file
*******************************************************************************/
#include "../header/common.h"
#include "../header/human_detect_alarm.h"
#include "../header/alarm_mode.h"

/*******************************************************************************
	declare function
*******************************************************************************/

/*******************************************************************************
	init function
*******************************************************************************/
BOOLEAN HumanDetectAlarm_Initialize(AlarmOutInfo* sAlarmOutInfo){
	sAlarmOutInfo->bAlarm = 0;
	sAlarmOutInfo->bIsHuman = 0;
	sAlarmOutInfo->usAlertStatus = 0;
	sAlarmOutInfo->usHIndex = 0;
	sAlarmOutInfo->usPreHIndex = 0;
	sAlarmOutInfo->usHumanFrame = 0;
	/* 20161118 - howardjhli - add more info for detect alarm */
	Coordinate sG;
	sG.fx = 0;
	sG.fy = 0;
	ClearArray(sAlarmOutInfo->sLogG, LOGSIZE_G, sG);
	/* 20161118 - howardjhli - add more info for detect alarm */
	ClearArray(sAlarmOutInfo->usLogArea, LOGSIZE_G, 0);

	initialBedAlarm2();
}

void HumanDetectAlarm_setParameter(ConfigParameters psConfigParameters){
	QUILT_MOV_THRESH = psConfigParameters.sAlarmParameters.fQuiltMovThresh;
	LOGSIZE_G = psConfigParameters.sAlarmParameters.ucLogSizeG;
	HUMAN_THRESH = psConfigParameters.sAlarmParameters.ucHumanThresh;
	FNMV_FRAMEMAX = psConfigParameters.sAlarmParameters.ucFNMV_FrameMax;
	QUILT_AREA_THRESH = psConfigParameters.sAlarmParameters.fQuiltAreaThresh;
	BED_LEFT = psConfigParameters.sAlarmParameters.ucBedLeftBoardValue;
	BED_RIGHT = psConfigParameters.sAlarmParameters.ucBedRightBoardValue;

	LEFT_X_LINE = psConfigParameters.sTrackingParameters.usLeftXLine;
	RIGHT_X_LINE = psConfigParameters.sTrackingParameters.usRightXLine;
	TOP_Y_LINE = psConfigParameters.sTrackingParameters.usTopYLine;
	BOTTOM_Y_LINE = psConfigParameters.sTrackingParameters.usBottomYLine;
		
}
/*******************************************************************************
	main function
*******************************************************************************/

void GetOutputBath(OutInfo* psHumanOutInfo, OutInfo* psObjectOutInfo, AlarmOutInfo* psAlarmOutInfo){
	/* Check human is in bath */
	UpdateIsHumanInBath(psHumanOutInfo, psObjectOutInfo, psAlarmOutInfo);
	/* Check human is display or been filtered */
	UpdateIsHumanInBathStatus(psHumanOutInfo, psObjectOutInfo, psAlarmOutInfo);
	/* Update human detect alarm status */
	UpdateHumanStatusInBath(psHumanOutInfo, psObjectOutInfo, psAlarmOutInfo);
	
	/* Check human is not in bath */
	UpdateIsHumanNotInBath(psHumanOutInfo, psObjectOutInfo, psAlarmOutInfo);
}

void GetOutputBed(OutInfo* psHumanOutInfo, OutInfo* psObjectOutInfo, AlarmOutInfo* psAlarmOutInfo, BOOLEAN pbIsHumanInBed){
	
	/* Check human is in bed */
	//UpdateIsHumanInBed(psHumanOutInfo, psObjectOutInfo, psAlarmOutInfo);
	
	/* Check human is display or been filtered */
	//UpdateHumanInBedFilterStatus(psHumanOutInfo, psObjectOutInfo, psAlarmOutInfo);
	
	/* Update human detect alarm status */
	//UpdateHumanQuiltStatus(psHumanOutInfo, psObjectOutInfo, psAlarmOutInfo);
	
	/* Update human detect alarm status */
	//UpdateHumanLeaveBedStatus(psHumanOutInfo, psObjectOutInfo, psAlarmOutInfo);
	
	
	/* Check human is not in bed */
	//UpdateIsHumanNotInBed(psHumanOutInfo, psObjectOutInfo, psAlarmOutInfo);

	//CheckHumanApart(psHumanOutInfo, psObjectOutInfo, psAlarmOutInfo);
	
	//UpdateIsPatientIn(psHumanOutInfo, psObjectOutInfo, psAlarmOutInfo);




	UpdateEffectiveCount(psHumanOutInfo, psObjectOutInfo, psAlarmOutInfo, pbIsHumanInBed);

	UpdateEffectiveNum(psAlarmOutInfo);
}


void GetOutputDiff(OutInfo* psOutInfo, AlarmOutInfo* psAlarmOutInfo){
	//UpdateDiffTempLeaveStatus(psOutInfo, psAlarmOutInfo);
}

void HumanDetectAlarm_GetOutput(OutInfo* psOutInfo, OutInfo* psObjectOutInfo, 
								AlarmOutInfo* psAlarmOutInfo, UCHAR pucMode, BOOLEAN pbIsHumanInBed){
	if (pucMode == BATH_MODE){
		GetOutputBath(psOutInfo, psObjectOutInfo, psAlarmOutInfo);
	}
	else if (pucMode == BED_MODE){
		GetOutputBed(psOutInfo, psObjectOutInfo, psAlarmOutInfo, pbIsHumanInBed);
	}
	else if (pucMode == DIFF_MODE){
		GetOutputDiff(psOutInfo, psAlarmOutInfo);
	} 
	else {
		GetOutputBed(psOutInfo, psObjectOutInfo, psAlarmOutInfo, pbIsHumanInBed);
	}
}

/*******************************************************************************
	tool function
*******************************************************************************/
void addCenter(Coordinate psG, AlarmOutInfo* psAlarmOutInfo){
	for (int i=LOGSIZE_G-1; i>0; i--){
		psAlarmOutInfo->sLogG[i].fx = psAlarmOutInfo->sLogG[i-1].fx;
		psAlarmOutInfo->sLogG[i].fy = psAlarmOutInfo->sLogG[i-1].fy;
	}
	
	psAlarmOutInfo->sLogG[0].fx = psG.fx;
	psAlarmOutInfo->sLogG[0].fy = psG.fy;
}
/* 20161118 - howardjhli - add more info for detect alarm */
void addArea(USHORT pusArea, AlarmOutInfo* psAlarmOutInfo){
	for (int i=LOGSIZE_G-1; i>0; i--){
		psAlarmOutInfo->usLogArea[i] = psAlarmOutInfo->usLogArea[i-1];
	}
	
	psAlarmOutInfo->usLogArea[0] = pusArea;
}
void UpdateHIndexAndFrame(OutInfo* psHumanOutInfo, AlarmOutInfo* psAlarmOutInfo, SHORT psIndex){
	if (psIndex == -1) return;
	for (int i=0; i<OT_TRACKING_MAX; i++){
		if ((psHumanOutInfo->ucLabelNum[i] > 0) && 
			(i == psIndex)){
			psAlarmOutInfo->usHIndex = i;
		} else {
			//psAlarmOutInfo->usHIndex = 0;
			continue;
		}
		
		if (psAlarmOutInfo->usPreHIndex == psAlarmOutInfo->usHIndex){
			psAlarmOutInfo->usHumanFrame++;
		} else {
			psAlarmOutInfo->usHumanFrame = 0;
		}
		
		psAlarmOutInfo->usPreHIndex = psAlarmOutInfo->usHIndex;
	}
}
void UpdateHIndex(OutInfo* psHumanOutInfo, AlarmOutInfo* psAlarmOutInfo, SHORT psIndex){
	if (psIndex == -1) return;
	for (int i=0; i<OT_TRACKING_MAX; i++){
		if ((psHumanOutInfo->ucLabelNum[i] > 0) &&
		(i == psIndex)){
			psAlarmOutInfo->usHIndex = i;
		} else {
			//psAlarmOutInfo->usHIndex = 0;
			continue;
		}
		
		psAlarmOutInfo->usPreHIndex = psAlarmOutInfo->usHIndex;
	}
}

/* 20161118 - howardjhli - add more info for detect alarm */
// check area for quilt status //
SHORT checkAreaStatus(AlarmOutInfo* psAlarmOutInfo){
	SHORT maxArea = 0;
	SHORT minArea = 999;
	USHORT maxIndex = 0;
	USHORT minIndex = 0;
	SHORT diffArea = 0;
	
	for (int i=0; i<LOGSIZE_G; i++){
		if (psAlarmOutInfo->usLogArea[i] < minArea){
			minArea = psAlarmOutInfo->usLogArea[i];
			minIndex = i;
		}
		if (psAlarmOutInfo->usLogArea[i] > maxArea){
			maxArea = psAlarmOutInfo->usLogArea[i];
			maxIndex = i;
		}
	}
	
	if (minIndex == maxIndex){
		return INVALID_RESULT;
	}
	
	diffArea = maxArea - minArea;
	if (diffArea <= 0){
		return INVALID_RESULT;
	} else {
		if (diffArea < minArea*QUILT_AREA_THRESH){
			return INVALID_RESULT;
		}
	}
	
	// the way to open the quilt
	if (minIndex > maxIndex){
		return OPEN_QUILT;
	} 
	//the way to cover the quilt
	else if (minIndex < maxIndex){
		return COVER_QUILT;
	} else {
		return INVALID_RESULT;
	}
	
}


/*******************************************************************************
 * Copyright(C) by 2016 FIH - HowardJHLi.
 ******************************************************************************/
