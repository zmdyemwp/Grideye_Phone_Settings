/*
 * human_detect_alarm.h
 *
 * Created: 2016/10/28 上午 11:11:13
 *  Author: HowardJHLi
 */ 
/*******************************************************************************
 * Copyright(C) by 2016 FIH - HowardJHLi.
 ******************************************************************************/

#ifndef HUMAN_DETECT_ALARM_H_
#define HUMAN_DETECT_ALARM_H_


/*******************************************************************************
	include file
*******************************************************************************/
#include	"common.h"
#include	"human_detection.h"

/*******************************************************************************
	macro definition
*******************************************************************************/

#define BED_CENTER_X		(7)
#define BED_CENTER_X_DEV	(3)
#define BED_CENTER_Y		(7)
#define BED_CENTER_Y_DEV	(6)

#define NORMAL				(0)
#define ALARM				(1)
#define WARNING				(2)

#define INVALID_RESULT		(-1)
#define OPEN_QUILT			(1)
#define COVER_QUILT			(0)

#define DF_LOGSIZE			(6)


#define STANDBY				(0)
#define MONITOR				(1)
#define SAFE				(2)
#define ALERT				(3)

/*******************************************************************************
	struct
*******************************************************************************/

typedef struct _tagAlarmOutInfo {
	USHORT bAlarm;
	USHORT bIsHuman;
	USHORT usAlertStatus;
	USHORT usHIndex;
	USHORT usPreHIndex;
	Coordinate	sLogG[OT_LOGSIZE_G];
	/* 20161118 - howardjhli - add more info for detect alarm */
	USHORT usLogArea[OT_LOGSIZE_G];
	USHORT usHumanFrame;

} AlarmOutInfo;

/* for human detect alarm info*/
typedef struct _tagHumanDetectAlarmInfo {
	OutInfo sObjectOutInfo;
	AlarmOutInfo sAlarmOutInfo;
	
} HumanDetectAlarmInfo;



/*******************************************************************************
	parameter define
*******************************************************************************/

float QUILT_MOV_THRESH;
USHORT LOGSIZE_G;		
USHORT HUMAN_THRESH;
USHORT FNMV_FRAMEMAX;
float QUILT_AREA_THRESH; 
USHORT BED_LEFT;
USHORT BED_RIGHT;

USHORT LEFT_X_LINE;
USHORT RIGHT_X_LINE;
USHORT TOP_Y_LINE;
USHORT BOTTOM_Y_LINE;

/*******************************************************************************
	function define
*******************************************************************************/
void addCenter(Coordinate, AlarmOutInfo*);
void addArea(USHORT, AlarmOutInfo*);
void UpdateHIndexAndFrame(OutInfo*, AlarmOutInfo*, SHORT);
void UpdateHIndex(OutInfo*, AlarmOutInfo*, SHORT);
SHORT checkAreaStatus(AlarmOutInfo*);
void initialBedAlarm2();
#endif /* HUMAN_DETECT_ALARM_H_ */

/*******************************************************************************
 * Copyright(C) by 2016 FIH - HowardJHLi.
 ******************************************************************************/


