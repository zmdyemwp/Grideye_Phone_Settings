/*
* object_tracking.h
*
* Created: 2016/10/28 �W�� 11:11:13
*  Author: HowardJHLi
*/
/*******************************************************************************
* Copyright(C) by 2016 FIH - HowardJHLi.
******************************************************************************/

#ifndef OBJECT_TRACKING_H_
#define OBJECT_TRACKING_H_


/*******************************************************************************
include file
*******************************************************************************/
#include	"common.h"
#include	"human_detection.h"

/*******************************************************************************
macro definition
*******************************************************************************/

#define		OT_IMG_SZ_MAX		(15*15)
#define		OT_OUT_SZ_MAX		(2*2)

#define		OT_DETECT_MAX		(5)
#define		OT_TRACKING_MAX		(5)
#define		OT_STORED_MAX		(5)
#define		OT_INCLUDE_MAX		(3)

#define		OT_LOGSIZE_T		(5)
#define		OT_LOGSIZE_T_L		(6)
#define		OT_LOGSPAN_T_L		(100)
#define		OT_LOGSIZE_G		(10)

// HowardJHLI 20161102
#define		OT_LOGSIZE_D		(4)
#define		OT_LOGSIZE_D		(6)

#define		OT_APART_TH_INC		(3)
#define		OT_APART_PM_THRESH	(1.5f)
#define		OT_APART_PM_MAX		(2)

#define		OT_RESTORE_THRESH_R	(3)
#define		OT_RESTORE_THRESH_G	(2.0f)

#define		OT_STORED_THRESH_R	(2)
#define		OT_STORED_THRESH_F	(3000)

#define		OT_UNION_TH_INC		(3)
#define		OT_UNION_THRESH_G_L	(1.0f)
#define		OT_UNION_PM_RATE	(100)
#define		OT_UNION_HS_RATE	(0.01f)
#define		OT_UNION_V_RATE		(100)
#define		OT_UNION_THRESH_V	(1.5f)
#define		OT_UNION_THRESH_F	(5)
#define		OT_UNION_STORED_F	(10)
#define		OT_UNION_STORED_A	(14)

#define		OT_CALC_DISTANCE_D	(0.5f)
#define		OT_CALC_DISTANCE_R	(2.0f)

#define		OT_PRELINK_TH_G		(1.0f)
#define		OT_PRELINK_TH_A_D	(10)
#define		OT_PRELINK_TH_A_R	(1.3f)

#define		OT_LINK_THRESH_D	(4.0f)

#define		OT_MAXLINKFRAME		(20)

#define		OT_NEAR_ANOTHER_D	(2)

#define		OT_BLINK_COUNT		(30)
#define		OT_BLINK_RESET		(200)
#define		OT_FRAMECOUNT_MAX	(30000)
#define		OT_FNMV_FRAMEMAX	(1800)	// related human_detect_alarm.h OT_FNMV_FRAMEMA
#define     EDGE_THRESH			(600)

#define		OT_NOTHUMAN_FRAME	(1800)
#define		OT_NOTHUMAN_BLINK	(10)
#define		OT_NOTHUMAN_TEMP	(25)
#define		OT_NOTHUMAN_ISU		(1.0f)

#define		OT_PRINTER_TEMP		(0.5f)

#define		OT_CONMOV_EDGE_W	(7)
#define		OT_CONMOV_EDGE_H	(7)

#define		OT_CONMOV_MV_R0		(0.5f)
#define		OT_CONMOV_MV_R1		(0.75f)
#define		OT_CONMOV_MV_MAX	(1.0f)
#define		OT_CONMOV_MV_EDGE	(2.0f)
#define		OT_CONMOV_SMV_NEDGE	(0.5f)
#define		OT_CONMOV_SMV_EDGE	(1.1f)

#define		OT_CONMOV_FRAME		(3)
#define		OT_G10PLUS			(1.5f)

#define		OT_V_MOVE			(3.5f)
#define		OT_V_SUBMOVE1		(2.5f)
#define		OT_V_SUBMOVE2		(1.5f)

#define		OT_PM_MOVE			(4)
#define		OT_PM_SUBMOVE		(2)

#define		OT_G_HALF			(0.5f)
#define		OT_G_ALPHA			(0.1f)

#define		OT_T_MAXS			(0.5f)
#define		OT_T_MAXL			(0.02f)

#define		OT_INV_DISTANCE		(10000)
#define		OT_INVALID_DIR		(-10)

#define		OT_INVALID_C		(-1)

#define		OT_HS_INITIAL		(12000)
#define		OT_HS_MINIMUM		(0)
#define		OT_HS_MAXIMUM		(15000)

#define		OT_HS_DEC_SUSPICIOUS	(3)
#define		OT_HS_DECREACE		(15)

#define		OT_HS_INC_BY_MOVE	(2000)
#define		OT_HS_BASE_MOVE		(1.5f)
#define		OT_HS_HUMAN			(3000)

#define		OT_OUT_LOGSIZE		(10)

#define		OT_OUT_G_SEL_V_TH	(2.5f)
#define		OT_OUT_AREA_TH		(4)
#define		OT_OUT_FRAME_TH		(1)

#define		OT_OUT_LR_TH_A		(30)
#define		OT_OUT_LR_TH_G		(1.5f)
#define		OT_OUT_LR_TH_R		(3)
#define		OT_OUT_UD_TH_A		(30)
#define		OT_OUT_UD_TH_G		(1.5f)
#define		OT_OUT_UD_TH_R		(3)

#define		OT_IVOUT_INITIAL	(0)
#define		OT_IVOUT_MINIMUM	(0)
#define		OT_IVOUT_MAXIMUM	(50)
#define		OT_IVOUT_UP			(1)
#define		OT_IVOUT_DOWN		(2)

#define		UpdateLog(array,size,data)	\
{										\
	int i;								\
	for( i = size; i > 1; i-- ){		\
		array[i-1] = array[i-2];		\
			}									\
	array[0] = data;					\
}



#define INNER_LINE_X	(3)
#define OUTTER_LINE_X	(1)
#define INNER_LINE_Y	(3)
#define OUTTER_LINE_Y	(1)
#define LINE_RANGE		(0.5f)


/*******************************************************************************
 struct
*******************************************************************************/

typedef enum {
	MV_Continuous,
	MV_SubContinuous,
	MV_NotContinuous
} ConMove;

typedef struct _tagTrackingFeature {

	UCHAR	ucLabelNum;

	USHORT	usFrame;
	USHORT	usDetectFrame;
	UCHAR	ucNoDetectFrame;
	UCHAR	ucBlinkCount;

	USHORT	usLogArea[OT_LOGSIZE_G];

	Rect	sRectangle;
	Rect	sRectangle_Prev;

	float	fLogTMax[OT_LOGSIZE_T];
	float	fTMaxs;
	float	fTMaxl;
	float	fLogTMaxl[OT_LOGSIZE_T_L];

	Coordinate	sG_Half[2];
	Coordinate	sGl;
	Coordinate	sLogG[OT_LOGSIZE_G];

	CHAR	cLogDir[OT_LOGSIZE_D];

	float	fV;
	UCHAR	ucPM;
	USHORT	usFNMV;

	int		nHumanScore;
	BOOLEAN	bHumanValid;
	USHORT	usNotHumanFrame;

	USHORT	usPartnerNum;
	UCHAR	ucPartnerPM;


	BOOLEAN bOutterLine;
	BOOLEAN bInnerLine;
	UCHAR ucMoveStatus;

} TrackingFeature;

typedef struct _tagStoredFeature {

	USHORT	usSaveFrame;

	USHORT	usFrame;
	UCHAR	ucPM;
	int		nHumanScore;
	BOOLEAN	bHumanValid;

	Rect	sRectangle;
	Coordinate	sG;

	UCHAR	ucBlinkCount;

	float	fLogTMax[OT_LOGSIZE_T];
	float	fTMaxs;
	float	fTMaxl;
	float	fLogTMaxl[OT_LOGSIZE_T_L];

	CHAR	cLogDir[OT_LOGSIZE_D];
	USHORT	usFNMV;
	USHORT	usNotHumanFrame;

	BOOLEAN bOutterLine;
	BOOLEAN bInnerLine;
	UCHAR ucMoveStatus;


} StoredFeature;

typedef struct _tagOutInfoT {
	UCHAR	ucHumanNum;
	UCHAR	ucMap[OT_OUT_SZ_MAX];
	UCHAR   ucLabelNum[OT_TRACKING_MAX];
	Coordinate   sCenter[OT_TRACKING_MAX];
	//UCHAR   ucNotHumanFrame[OT_TRACKING_MAX];
	//UCHAR   ucBlinkCount[OT_TRACKING_MAX];
	/* 20161118 - howardjhli - add more info for detect alarm */
	USHORT	usArea[OT_TRACKING_MAX];
	Rect	sRectangle[OT_TRACKING_MAX];
	//Rectangle	sRectangle_Prev[OT_TRACKING_MAX];
	CHAR    cLogDir[OT_TRACKING_MAX][OT_LOGSIZE_D];
	USHORT  usFNMV[OT_TRACKING_MAX];
	USHORT  usFrame[OT_TRACKING_MAX];


	BOOLEAN bOutterLine[OT_TRACKING_MAX];
	BOOLEAN bInnerLine[OT_TRACKING_MAX];
	UCHAR ucMoveStatus[OT_TRACKING_MAX];

} OutInfoT;

typedef struct _tagTrackingOutputInfo {
	OutInfoT	sOut;
	OutInfoT	sOutLog[OT_OUT_LOGSIZE];
	UCHAR		ucIVOUT[OT_OUT_SZ_MAX];
	UCHAR		ucLogLength;
} TrackingOutputInfo;

typedef struct _tagObjectTrackingInfo {
	struct {
		UCHAR	ucImgX;
		UCHAR	ucImgY;
		USHORT	usImg;
		UCHAR	ucOutX;
		UCHAR	ucOutY;
		UCHAR	ucOut;
	} Sz;
	TrackingFeature	sTF[OT_TRACKING_MAX];
	StoredFeature	sSF[OT_STORED_MAX];
	TrackingOutputInfo	sOutInfo;
} ObjectTrackingInfo;

typedef struct tagTrackingControlInfo {

	BOOLEAN	bLinkedTF[OT_TRACKING_MAX];
	BOOLEAN	bLinkedDF[OT_DETECT_MAX];
	BOOLEAN	bAliveOfStored[OT_STORED_MAX];

	UCHAR	ucDetectNum;
	Feature*	psDF;

	float	fMinDistance_From_Tracked[OT_TRACKING_MAX];
	CHAR	cMinDistanceNo_From_Tracked[OT_TRACKING_MAX];
	float	fMinDistance_From_Now[OT_DETECT_MAX];
	CHAR	cMinDistanceNo_From_Now[OT_DETECT_MAX];

} TrackingControlInfo;



/*******************************************************************************
	�v?�g�^�C�v�錾
*******************************************************************************/
void	ClearTrackingControlInfo( TrackingControlInfo* );
void	ClearTrackingFeature    ( TrackingFeature* );
void	ClearStoredFeature  	( StoredFeature* );
void	ClearTrackingOutputInfo ( TrackingOutputInfo* );

void	DetectApart( void );
void	ApartProc( UCHAR, UCHAR, UCHAR* );
void	CheckStoredObject( void );
void	DetectUnion( void );
void	UnionProc( UCHAR, UCHAR, UCHAR* );

void	CalculateDistance( void );

BOOLEAN	LinkForNoTrackingObject( void );
BOOLEAN	PreLinkObject( UCHAR, UCHAR );
void	LinkObject( void );

void	IncrementFrameNum( TrackingFeature* );
void	DeleteNotUsedObjectNearAnothorObject( void );

UCHAR	SearchNewIndex( void );

BOOLEAN	IsLinkedTrackingFeature( UCHAR );
BOOLEAN	IsLinkedFeature( UCHAR );

void	UpdateFeatureAndScore( UCHAR, UCHAR );
void	UpdateTrackingFeature( TrackingFeature*, Feature* );
void	UpdateMoveDirection  ( TrackingFeature* );
void	UpdateConMove        ( TrackingFeature* );
ConMove JudgeMoveContinuity  ( TrackingFeature*, BOOLEAN );
void	UpdateMovePoint      ( TrackingFeature* );
void	UpdateHumanScore     ( TrackingFeature* );

BOOLEAN	JudgeNotHuman        ( TrackingFeature*, float );

void	UpdateTrackingOutInfo( TrackingOutputInfo* );
void    UpdateHumanTempOutInfo( UCHAR*, short*, short*, short* );
void    UpdateHumanDetectAlarmInfo( TrackingOutputInfo* );

void UpdateMoveLineStatus(TrackingFeature* );
void UpdateTFFromAlarmInfo(OutInfo* );


#endif /* OBJECT_TRACKING_H_ */

/*******************************************************************************
* Copyright(C) by 2016 FIH - HowardJHLi.
******************************************************************************/


/*******************************************************************************
�}�N?��`
*******************************************************************************/

