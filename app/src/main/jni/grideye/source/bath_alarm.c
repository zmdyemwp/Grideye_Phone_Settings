/*******************************************************************************
 * Copyright(C) by 2016 FIH - HowardJHLi.
 ******************************************************************************/
//#include	"stdafx.h"
/*******************************************************************************
	include file
*******************************************************************************/
#include "../header/common.h"
#include "../header/human_detect_alarm.h"


/*******************************************************************************
	declare function
*******************************************************************************/

/*******************************************************************************
	tool function
*******************************************************************************/
void UpdateHIndexInBath(OutInfo* psOutInfo, AlarmOutInfo* psAlarmOutInfo){
	for (int i=0; i<OT_TRACKING_MAX; i++){
		if (psOutInfo->ucLabelNum[i] > 0){
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

/*******************************************************************************
	public function
*******************************************************************************/
void UpdateIsHumanInBath(OutInfo* psHumanOutInfo, OutInfo* psObjectOutInfo, AlarmOutInfo* psAlarmOutInfo){
	if (psAlarmOutInfo->bIsHuman == 2){
		return;
	}
	
	/* is human in bath */
	if (psHumanOutInfo->ucHumanNum > 0){
		
		/* 20170206 - howardjhli - for bath detect */
		//if (psHumanOutInfo->ucHumanNum > 1){
			//return;
		//}
		
		UpdateHIndexInBath(psHumanOutInfo, psAlarmOutInfo);
		
		if (psAlarmOutInfo->usHumanFrame > HUMAN_THRESH){
			psAlarmOutInfo->bIsHuman = 1;
			addCenter(psHumanOutInfo->sCenter[psAlarmOutInfo->usHIndex], psAlarmOutInfo);
			/* 20161118 - howardjhli - add more info for detect alarm */
			addArea(psHumanOutInfo->usArea[psAlarmOutInfo->usHIndex], psAlarmOutInfo);
		} else {
			/* 20170206 - howardjhli - for bath detect */
			//psAlarmOutInfo->bIsHuman = 0;
			
			Coordinate sG;
			sG.fx = 0;
			sG.fy = 0;
			ClearArray(psAlarmOutInfo->sLogG, LOGSIZE_G, sG);
			/* 20161118 - howardjhli - add more info for detect alarm */
			ClearArray(psAlarmOutInfo->usLogArea, LOGSIZE_G, 0);
		}
	}
}

void UpdateIsHumanNotInBath(OutInfo* psHumanOutInfo, OutInfo* psObjectOutInfo, AlarmOutInfo* psAlarmOutInfo){
	if (psAlarmOutInfo->bIsHuman == 0){
		//psAlarmOutInfo->usHumanFrame = 0;
		psAlarmOutInfo->bAlarm = 0;
			
		return;
	}
	
	/* human not in bath */
	if (psObjectOutInfo->ucHumanNum == 0){
		psAlarmOutInfo->bIsHuman = 0;
		
	}
}

void UpdateIsHumanInBathStatus(OutInfo* psHumanOutInfo, OutInfo* psObjectOutInfo, AlarmOutInfo* psAlarmOutInfo){
	/* 20170206 - howardjhli - for bath detect */
	/*
	// if not human in bath, then return
	if (psAlarmOutInfo->bIsHuman == 0){
		return;
	}
	UpdateHIndexInBath(psObjectOutInfo, psAlarmOutInfo);
	if (psObjectOutInfo->ucHumanNum > 0 && psHumanOutInfo->ucHumanNum == 0){
		psAlarmOutInfo->bIsHuman = 2;
		if ( (psObjectOutInfo->usFNMV[psAlarmOutInfo->usHIndex] >= FNMV_FRAMEMAX)){
			psAlarmOutInfo->bIsHuman = 3;
			//psAlarmOutInfo->bIsHuman = 2;
			//psAlarmOutInfo->usHumanFrame = 0;
			//Coordinate sG;
			//ClearArray(psAlarmOutInfo->sLogG, OT_LOGSIZE_G, sG);
		} 		
	} else {
		psAlarmOutInfo->bIsHuman = 1;
		
	}
	*/

	/* 20170206 - howardjhli - for bath detect */
	// 20161206
	// if not human in bath, then return
	if (psAlarmOutInfo->bIsHuman == 0){
		return;
	}

	UpdateHIndexInBath(psObjectOutInfo, psAlarmOutInfo);

	if (psObjectOutInfo->ucHumanNum > 0){
		for (int i = 0; i<OT_TRACKING_MAX; i++){
			if (psHumanOutInfo->usFrame[i] >= FNMV_FRAMEMAX){
				psAlarmOutInfo->bIsHuman = 3;
			}
		}


		if ((psObjectOutInfo->usFrame[psAlarmOutInfo->usHIndex] >= FNMV_FRAMEMAX)){
			psAlarmOutInfo->bIsHuman = 3;
		}

	}
	else {
		//psAlarmOutInfo->bIsHuman = 1;

	}
	
}


void UpdateHumanStatusInBath(OutInfo* psHumanOutInfo, OutInfo* psObjectOutInfo, AlarmOutInfo* psAlarmOutInfo){
	// if not human in bath, then return
	if (psAlarmOutInfo->bIsHuman == 0){
		return;
	}
	int i;
	
	switch (psAlarmOutInfo->bIsHuman){
		/* CASE1: human temperature show in bath */
		case 1:
			psAlarmOutInfo->bAlarm = 0;
			break;
		
		/* CASE2: human temperature has been filter to object&background */
		case 2:	

			if (psAlarmOutInfo->bAlarm != 1) psAlarmOutInfo->bAlarm = 2;
			
			break;
		/* CASE2: human temperature has been filter to object&background (frame < OT_FNMV_FRAMEMAX)*/
		case 3:

 			psAlarmOutInfo->bAlarm = 1;

		break;	
		default:
			break;
	}
	
	//psAlarmOutInfo->bAlarm = sAlarmOutInfo->bAlarm;
}


/*******************************************************************************
 * Copyright(C) by 2016 FIH - HowardJHLi.
 ******************************************************************************/
