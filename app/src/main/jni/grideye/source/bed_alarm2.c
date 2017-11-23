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
short ucEffective_count;
USHORT usBed_human_frame;
BOOLEAN bAlert_detecting;
void UpdateEffectiveCount(OutInfo* , OutInfo* , AlarmOutInfo*, BOOLEAN );
/*******************************************************************************
tool function
*******************************************************************************/
BOOLEAN IsHumanCoordinateInLine(OutInfo* psHumanOutInfo){
	for (int i = 0; i < OT_TRACKING_MAX; i++){
		if (psHumanOutInfo->ucLabelNum[i] == 0)
			continue;

		if ((psHumanOutInfo->sCenter[i].fx < LEFT_X_LINE && psHumanOutInfo->sCenter[i].fx > RIGHT_X_LINE) &&
			(psHumanOutInfo->sCenter[i].fy < TOP_Y_LINE && psHumanOutInfo->sCenter[i].fy > BOTTOM_Y_LINE))
		{
			return TRUE;
		}

	}
	return FALSE;
}

BOOLEAN IsHumanCoordinateOutBed(OutInfo* psHumanOutInfo){
	for (int i = 0; i < OT_TRACKING_MAX; i++){
		if (psHumanOutInfo->ucLabelNum[i] == 0)
			continue;

		if ((psHumanOutInfo->sCenter[i].fx > BED_LEFT) || (psHumanOutInfo->sCenter[i].fx < BED_RIGHT))
		{
			return TRUE;
		}

	}
	return FALSE;
}

BOOLEAN IsHumanCoordinateInBed(OutInfo* psHumanOutInfo){
	for (int i = 0; i < OT_TRACKING_MAX; i++){
		if (psHumanOutInfo->ucLabelNum[i] == 0)
			continue;

		if ((psHumanOutInfo->sCenter[i].fx < BED_LEFT) && (psHumanOutInfo->sCenter[i].fx > BED_RIGHT))
		{
			return TRUE;
		}
	
	}
	return FALSE;
}


USHORT HumanMoveOutCount (OutInfo* psHumanOutInfo){
	USHORT ushumanNum = 0;
	
	for (int i = 0; i < OT_TRACKING_MAX; i++){
		if (psHumanOutInfo->ucMoveStatus[i] == MOVE_OUT)
			ushumanNum++;
	}

	return ushumanNum;
}

USHORT HumanMoveInCount (OutInfo* psHumanOutInfo){
	USHORT ushumanNum = 0;
	
	for (int i = 0; i < OT_TRACKING_MAX; i++){
		if (psHumanOutInfo->ucMoveStatus[i] == MOVE_IN)
			ushumanNum++;
	}

	return ushumanNum;
}

void ClearHumanMoveStatus(AlarmOutInfo* psAlarmOutInfo, OutInfo* psHumanOutInfo, UCHAR moveStatus){
	for (int i = 0; i < OT_TRACKING_MAX; i++){
		if (psHumanOutInfo->ucMoveStatus[i] == moveStatus){
			psHumanOutInfo->ucMoveStatus[i] = MOVE_NA;
			if (moveStatus == MOVE_OUT) psHumanOutInfo->bInnerLine[i] = FALSE;
			if (moveStatus == MOVE_IN) psHumanOutInfo->bOutterLine[i] = FALSE;
		}
	}
}


/*******************************************************************************
main function
*******************************************************************************/
void initialBedAlarm2(){
	ucEffective_count = STANDBY;
	bAlert_detecting = FALSE;
	usBed_human_frame = 0;
}

BOOLEAN checkAlarm(AlarmOutInfo* psAlarmOutInfo){
	// ���@�� ���oĵ��
	if (ucEffective_count != SAFE && bAlert_detecting){
		psAlarmOutInfo->bAlarm = ALARM;

		ucEffective_count = ALERT;
		return TRUE;
	}
	return FALSE;
}

void UpdateEffectiveNum(AlarmOutInfo* psAlarmOutInfo){
	psAlarmOutInfo->usAlertStatus = (USHORT)ucEffective_count;
}

/*------------------------------------------------------------------------------
	UpdateEffectiveCount
		psHumanOutInfo : tracking human info
		psObjectOutInfo : tracking obj/human info
		psAlarmOutInfo : alarm info
		pbIsHumanInBed : human in bed or not

------------------------------------------------------------------------------*/
void UpdateEffectiveCount(OutInfo* psHumanOutInfo, OutInfo* psObjectOutInfo, AlarmOutInfo* psAlarmOutInfo, BOOLEAN pbIsHumanInBed){

	// define 
	USHORT moveOutHuman = 0;					// have human move out
	USHORT moveInHuman = 0;						// have human move in 
	BOOLEAN bIsHumanInBed = pbIsHumanInBed;		// Is human in bed or not 
	psAlarmOutInfo->bIsHuman = pbIsHumanInBed;	// Is human in bed or not 
	

	if (ucEffective_count == ALERT){
		return;
	}

	// get the move status
	moveOutHuman = HumanMoveOutCount(psHumanOutInfo);
	moveInHuman = HumanMoveInCount(psHumanOutInfo);


	/* humna in bed */
	if (bIsHumanInBed){
		if (ucEffective_count == STANDBY){
			usBed_human_frame++;
			if (usBed_human_frame < HUMAN_THRESH){
				return;
			}

			ucEffective_count = MONITOR;
			bAlert_detecting = TRUE;
		}

	}
	else {
		usBed_human_frame = 0;
		ucEffective_count = STANDBY;
	}


	// *******  have human MOVE OUT ********* //
	if (moveOutHuman > 0){

		if (checkAlarm(psAlarmOutInfo))
			return;

		/* exception handling -- check wheteher human in bed */
		if (!bIsHumanInBed){
			ucEffective_count = STANDBY;
			return;
		}

		// 20170315
		// �u���٦��H�b���I�~�|�o�� (��Ӭ��@�X�h�@�Ӫ����p)
		if (!IsHumanCoordinateInLine(psHumanOutInfo)){
			ucEffective_count = MONITOR;
		}
		ClearHumanMoveStatus(psAlarmOutInfo, psHumanOutInfo, MOVE_OUT);
	}

	// ********* have human MOVE IN ************ //
	if (moveInHuman > 0){
		if (ucEffective_count == STANDBY && bAlert_detecting){
			bAlert_detecting = FALSE;
			return;
		}

		/* exception handling -- check wheteher human in bed */
		if (!bIsHumanInBed){
			ucEffective_count = STANDBY;
			return;
		}	
		
		ucEffective_count = SAFE;
		ClearHumanMoveStatus(psAlarmOutInfo, psHumanOutInfo, MOVE_IN);
	} 
	

	// ���l�Ū����p�U�f�H�A�צ^�� SAFE --> MONITOR
	if (bIsHumanInBed){
		if (!IsHumanCoordinateInLine(psHumanOutInfo)){
			ucEffective_count = MONITOR;
		}
	}
}

/*******************************************************************************
* Copyright(C) by 2016 FIH - HowardJHLi.
******************************************************************************/
