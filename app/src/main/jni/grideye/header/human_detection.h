/*******************************************************************************
 * Copyright(C) by 2014 Panasonic Corporation.
 ******************************************************************************/
#ifndef	__HUMAN_DETECTION_H
#define	__HUMAN_DETECTION_H


/*******************************************************************************
	include file
*******************************************************************************/
#include	"common.h"
//#include "human_detect_alarm.h"

/*******************************************************************************
	macro definition
*******************************************************************************/

/* short */
#define		TempStoF(x)			( (float)(x)/256 )

/* float */
#define		TempFtoS(x)			( ( (x) > 0 )							\
									? ( (short)( (x) * 256 + 0.5 ) )	\
									: ( (short)( (x) * 256 - 0.5 ) )	\
								)

#define		OT_TRACKING_MAX		(5)
#define		OT_LOGSIZE_D		(4)
#define		OT_LOGSIZE_G		(10)

/*  */
#define		OUT_SZ_X			(2)							/* ����f?		*/
#define		OUT_SZ_Y			(2)							/* �c��f?		*/
#define		OUT_SZ				(OUT_SZ_X * OUT_SZ_Y)		/* �S��f?		*/

#define		BED_MODE			(0)
#define		BATH_MODE			(1)
#define		DIFF_MODE			(2)

/*----- Grid-EYE  -----*/

/* Grid-EYE Address */
#define		GRIDEYE_ADR			(0xD0)	/* AD_SELECT�[�q GND�ڑ�?			*/
#define		GRIDEYE_REG_TMP00	(0x80)	/* ���x?�W�X�^�擪�A�h?�X			*/

/* Grid-EYE  */
#define		SNR_SZ_X			(8)							/* ����f?		*/
#define		SNR_SZ_Y			(8)							/* �c��f?		*/
#define		SNR_SZ				(SNR_SZ_X * SNR_SZ_Y)		/* �S��f?		*/

/*-----  -----*/

/*  */
#define		IMG_SZ_X			(SNR_SZ_X * 2 - 1)			/* ����f?		*/
#define		IMG_SZ_Y			(SNR_SZ_Y * 2 - 1)			/* �c��f?		*/
#define		IMG_SZ				(IMG_SZ_X * IMG_SZ_Y)		/* �S��f?		*/


/*  */
#define		TEMP_FRAME_NUM		(8)		/* �t?�[??						*/
#define		BACK_FRAME_NUM		(8)		/* ?���w�i�擾�t?�[??			*/
#define		TEMP_MEDIAN_FILTER	(2)		/* ���x��� Median Filter			*/
#define		TEMP_SMOOTH_COEFF	(0.1f)	/* ���x�X�V�W?						*/

/*  */
#define		DETECT_MAX			(10)	/* ?�o�ő�?						*/
#define		DETECTLIST_SIZE		(DETECT_MAX + 1)
/* 	*/
#define		DIFFTEMP_THRESH		(0.3f)	/* ?�l���x							*/
#define		LABELING_THRESH		(3)		/* ?�x??�O?�̕���??�ʐσT�C�Y */
#define		OUTPUT_THRESH		(6)		/* �o��      ?�̕���??�ʐσT�C�Y */

/*  */
#define		BKUPDT_ALLUP_THRESH	(22)	/* �ꗥ�X�V?�̕��̕s?�o��f??�l	*/
#define		BKUPDT_ALLUP_TEMP	(0.1f)	/* �ꗥ�X�V?�̍X�V���x				*/
/*  */
#define		BKUPDT_COEFF_BASE	(0.1f)	/* ?���l							*/
#define		BKUPDT_COEFF_AVE_T	(0.0f)	/* ���ύ����ɂ��ύX����?�l		*/
#define		BKUPDT_COEFF_AVE_S	(0.0f)	/* ���ύ������Z?�̃}�C�i�X�␳�l	*/
#define		BKUPDT_COEFF_STDV_T	(0.25f)	/* �W?�΍��ɂ��ύX����?�l		*/
#define		BKUPDT_COEFF_STDV_S	(0.2f)	/* �W?�΍����Z?�̃}�C�i�X�␳�l	*/
#define		BKUPDT_COEFF_MAX	(0.5f)	/* �ő�l							*/

/*  */
#define		SensorX(i)			( (i) % SNR_SZ_X )
#define		SensorY(i)			( (i) / SNR_SZ_X )
#define		SensorIndex(x,y)	( (x) + (y) * SNR_SZ_X )

#define		ImageX(i)			( (i) % IMG_SZ_X )
#define		ImageY(i)			( (i) / IMG_SZ_X )
#define		ImageIndex(x,y)		( (x) + (y) * IMG_SZ_X )

#define		OutImgX(i)			( (i) % OUT_SZ_X )
#define		OutImgY(i)			( (i) / OUT_SZ_X )
#define		OutImgIndex(x,y)	( (x) + (y) * OUT_SZ_X )


#define OBJ_TRACKING	(0)
#define DIFF_TRACKING	(1)

#define MOVE_NA		(0)
#define MOVE_IN		(1)
#define MOVE_OUT	(2)

#define RANGE_OF_HUMAN_IN_BED	(1)

/*******************************************************************************
	type definition
*******************************************************************************/

/*  */
typedef struct _tagCoordinate {
	float	fx;							/* x ���W							*/
	float	fy;							/* y ���W							*/
} Coordinate;

/*  */
typedef struct _tagRect {
	UCHAR	ucMinX;						/* ��`�̍��W ����					*/
	UCHAR	ucMaxX;						/* ��`�̍��W �E��					*/
	UCHAR	ucMinY;						/* ��`�̍��W �㑤					*/
	UCHAR	ucMaxY;						/* ��`�̍��W ����					*/
} Rect;

/*  */
typedef struct _tagFeature {
	UCHAR		ucLabelNum;				/* ?�x?��?						*/
	USHORT		usArea;					/* �ʐ�								*/
	Rect	sRectangle;				/* �O�ڋ�`							*/
	Coordinate	sG;						/* �d�S���W							*/
	short		shMaxDiffTemp;			/* �w�i�������x�ő�l				*/
} Feature;

/*  */
typedef struct _tagOutInfo {
	UCHAR	ucHumanNum;					/* �l?�o?							*/
	UCHAR	ucMap[OUT_SZ];				/* �l���݃}�b�v						*/
	UCHAR   ucLabelNum[OT_TRACKING_MAX];
	Coordinate   sCenter[OT_TRACKING_MAX];
	/* 20161118 - howardjhli - add more info for detect alarm */
	USHORT	usArea[OT_TRACKING_MAX];
	Rect	sRectangle[OT_TRACKING_MAX];
	//Rect	sRect_Prev[OT_TRACKING_MAX];
	//UCHAR   ucNotHumanFrame[OT_TRACKING_MAX];
	//UCHAR   ucBlinkCount[OT_TRACKING_MAX];
	CHAR    cLogDir[OT_TRACKING_MAX][OT_LOGSIZE_D];
	USHORT  usFNMV[OT_TRACKING_MAX];
	USHORT  usFrame[OT_TRACKING_MAX];

	/* for Diff temp tracking */
	//Coordinate  sDiffLogG[OT_LOGSIZE_G];
	//CHAR	cDiffLogDir[OT_LOGSIZE_D];


	BOOLEAN bOutterLine[OT_TRACKING_MAX];
	BOOLEAN bInnerLine[OT_TRACKING_MAX];
	UCHAR ucMoveStatus[OT_TRACKING_MAX];


} OutInfo;



/* setting detect parameters */
typedef struct _tagDetectParameters {
	USHORT ucTempFrameNum;
	USHORT ucBackFrameNum;
	USHORT ucTempMedianFilter;
	float fTempSmoothCoeff;
	float fDiffTempThresh;
	USHORT usLabelingThresh;
} DetectParameters;

/* setting detect parameters */
typedef struct _tagTrackingParameters {
	USHORT	ucFNMV_FrameMax;	//(10) related human_detect_alarm.h OT_FNMV_FRAMEMAX
	USHORT  ucEdgeThresh;		//	(10)
	float	fBA_CheckSumMove;	//(0.01f)
	float	fBA_CheckDiffArea;	//(0.3f)
	USHORT	usLeftXLine;
	USHORT  usRightXLine;
	USHORT  usTopYLine;
	USHORT  usBottomYLine;
} TrackingParameters;


/* setting alarm parameters */
typedef struct _tagAlarmParameters {
	float fQuiltMovThresh; //(0.5f)
	USHORT	ucLogSizeG;		//(20)
	USHORT   ucHumanThresh;		//(20)
	USHORT	ucFNMV_FrameMax;	//(1)	// related objectTracking.c OT_FNMV_FRAMEMAX
	float fQuiltAreaThresh; //(1.5f)
	USHORT	ucBedLeftBoardValue;
	USHORT	ucBedRightBoardValue;
} AlarmParameters;

/* setting all parameters */
typedef struct _tagConfigParameters {
	UCHAR   ucMode;	

	DetectParameters sDetectParameters;
	TrackingParameters sTrackingParameters;
	AlarmParameters sAlarmParameters;
} ConfigParameters;

typedef UCHAR					IMGSZ;

/* �l?�o��� */
typedef struct _tagHumanDetectInfo {
	UCHAR	ucFrameNum;					/* ??Frame?						*/
	UCHAR	ucRawTempIndex;				/* ���xIndex						*/
	UCHAR	ucBackFrameNum;				/* �w�iFrame?						*/
	UCHAR	ucDetectNum;				/* ����?�o?						*/
	UCHAR	ucDetectImg[IMG_SZ];		/* ����?�o��?						*/
	UCHAR	ucDetectImg2[IMG_SZ];		/* ����?�o��?						*/
	short	shRawTemp[TEMP_FRAME_NUM][SNR_SZ]; /* ��?���O���x				*/
	short	shAveTemp[IMG_SZ];			/* ��?���㉷�x						*/
	short	shBackTemp[IMG_SZ];			/* �w�i���x							*/
	short	shDiffTemp[IMG_SZ];			/* �w�i�������x						*/
	short	shDiffTemp2[IMG_SZ];			/* 					*/
	Feature	sFeature[DETECTLIST_SIZE];	/* �����ʏ��						*/
	OutInfo	sOutInfo;					/* �o�͏��							*/

	short   shHumanTemp[IMG_SZ];
	// 20170221
	UCHAR	ucDetectNum2;				/* ����?�o?						*/
	Feature sDiffFeature[DETECTLIST_SIZE];
	OutInfo sOutDiffInfo;

} HumanDetectInfo;

/*  */
typedef union _tagHumanDetectWork {
	union {
		UCHAR	ucWork[SNR_SZ];
		UCHAR	ucWork2[2][SNR_SZ];
		short	shWork[SNR_SZ];
	} Sensor;
	union {
		UCHAR	ucWork[IMG_SZ];
		IMGSZ	uImg[IMG_SZ];
	} Image;
	struct {
		UCHAR	ucWork[IMG_SZ];
		IMGSZ	uImg[IMG_SZ];
	} Image2;
} HumanDetectWork;


/*******************************************************************************
	public method definition
*******************************************************************************/
/* declare -- Do Human Detection for Dll */
void	HumanDetect_Initialize(void);
void	HumanDetect_Measure(short*);
void	HumanDetect_GetOutputHumanDetectInfo(HumanDetectInfo* hdi);
void	HumanDetect_GetOutputFeature(UCHAR*);
void	setConfigParameter_InputUShort(UCHAR, USHORT);
void	setConfigParameter_InputFloat(UCHAR, float);

void	HumanDetect_Initialize( void );
void	HumanDetect_Measure( short* );

UCHAR*	HumanDetect_GetOutput( UCHAR*, UCHAR* );
UCHAR*	HumanDetect_GetRawTemp( UCHAR*, UCHAR* );

short*  HumanDetect_GetOutputBackTemp();
UCHAR*  HumanDetect_GetOutputHumanNum();
UCHAR*	HumanDetect_GetOutputLabelNum();
Coordinate* HumanDetect_GetOutputCenter();
/* 20161118 - howardjhli - add more info for detect alarm */
USHORT* HumanDetect_GetOutputArea();
//UCHAR* HumanDetect_GetOutputNoHumanFrame();
//UCHAR* HumanDetect_GetOutputBlinkCount();

short*	HumanDetect_GetOutputHumanTemp();

BOOLEAN*	HumanDetectAlarm_GetOutputInfo();
USHORT* HumanDetect_GetOutputFNMV();
USHORT* HumanDetect_GetOutputObjFNMV();

void	setConfigParameter(UCHAR);


/*******************************************************************************
	private method definition
*******************************************************************************/
BOOLEAN	GetTemp( short*, UCHAR* );
BOOLEAN	ReadTempFromGridEYE( short* );
UCHAR	UpdateTemp( short*, short* );
short	CalcAverage( short*, UCHAR, UCHAR, UCHAR );
BOOLEAN	MakeInitialTemp( UCHAR, short*, short* );
BOOLEAN	MakeInitialTemp2( UCHAR, short*, short* );
void	Calc_BckRevTempImage( short*, short*, short*, UCHAR* );
void	LinearInterpolationSQ15( short* );
UCHAR	DataLabeling8( UCHAR* );
void	Calc_Features( short*, UCHAR*, UCHAR, Feature* );
void	Calc_AreaFeatures( short*, UCHAR*, UCHAR, Feature* );
void	Sort_AreaFeatures( UCHAR, Feature* );
void	Updata_BackTemp( UCHAR*, short*, short* );
void	ClearOutInfo( OutInfo* );
//void	UpdateOutInfo( UCHAR, Feature*, OutInfo* );
void	UpdateOutInfo(UCHAR);
BOOLEAN CheckHumanInBed(Feature* );
float Calc_DiffTempThresh(short*);



#endif	/* __HUMAN_DETECTION_H */
/*******************************************************************************
 * Copyright(C) by 2014 Panasonic Corporation.
 ******************************************************************************/
