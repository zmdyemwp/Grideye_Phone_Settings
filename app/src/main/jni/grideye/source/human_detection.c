/*******************************************************************************
 * Copyright(C) by 2017 FIH.
 ******************************************************************************/
//#include	"stdafx.h"

/*******************************************************************************
	include file
*******************************************************************************/
#include "../header/common.h"
#include "../header/human_detection.h"
#include "../header/human_detect_alarm.h"
//#include	"human_detection_output.h"
#include "math.h"

#include	<string.h>
#include "../header/object_tracking.h"
#include "../header/tuning_info.h"
/*******************************************************************************
	extern method definition
*******************************************************************************/
extern UCHAR I2C_Read( UCHAR, UCHAR, UCHAR*, UCHAR );
extern UCHAR bAMG_PUB_I2C_Read( UCHAR, UCHAR, UCHAR, UCHAR* );
extern float sqrtf( float );								/* math.h		*/

/* ����?�o */
extern BOOLEAN ObjectDetection_Initialize( UCHAR, UCHAR );
extern void ObjectDetection_forDLL( short*, short*, short*, short*, UCHAR*, UCHAR*, ConfigParameters* );
extern void ObjectDetection(short*, short*, short*, UCHAR*);

/* extern */
extern BOOLEAN ObjectTracking_Initialize( UCHAR, UCHAR, UCHAR, UCHAR );
extern void ObjectTracking( UCHAR, Feature* );
extern void ConvertNotHumanAreaToBackground( UCHAR*, short*, short*, UCHAR*, short* );
/* 20161118 - howardjhli - add more info for detect alarm */
extern void ObjectTracking_GetOutput( UCHAR*, UCHAR*, UCHAR*, Coordinate*, CHAR*, USHORT*, USHORT*, Rect*, USHORT*,
									  BOOLEAN*, BOOLEAN*, UCHAR*);
extern void ObjectTracking_GetALarmInfoOutput( UCHAR*, UCHAR*, UCHAR*, Coordinate*, CHAR*, USHORT*, USHORT*, Rect*, USHORT*,
											   BOOLEAN*, BOOLEAN*, UCHAR*);
extern void UpdateHumanTempOutInfo( UCHAR*, short*, short*, short*);
extern void HumanDetectAlarm_GetOutput(OutInfo*, OutInfo*, AlarmOutInfo*, UCHAR, BOOLEAN);
extern BOOLEAN HumanDetectAlarm_Initialize(AlarmOutInfo*);
extern void HumanDetectAlarm_setParameter(ConfigParameters);

extern void DiffTempTracking( UCHAR, Feature* );
extern void SaveObjectTrackingIfo( UCHAR );
extern void RestoreObjectTrackingIfo(UCHAR);

extern void UpdateTFFromAlarmInfo(OutInfo*);

extern void OD_FilterBedDetectImg(UCHAR*, ConfigParameters*);


extern void initTuningInfo();
extern void updateTuningInfo(HumanDetectInfo*);

extern void call_back_to_java(int, int);
extern void call_back_to_java_humanOutputImg(short[]);

/*******************************************************************************
	variable value definition
*******************************************************************************/
HumanDetectInfo	sHDI;					/* �l?�o���						*/
HumanDetectWork	sWork;					/* ?�[�N�̈�						*/
HumanDetectAlarmInfo sHDAI;				/* for human detect alarm		*/
ConfigParameters g_sConfigParameters;	/* for get config parameters	*/

extern ObjectTrackingInfo	sOTI;
extern ObjectTrackingInfo		sOTI_Obj;
extern ObjectTrackingInfo		sOTI_Diff;

extern TuningInfo sTuningInfo;

/****************************************************************************
*************   Functions -- Do Human Detection for Dll  *******************
****************************************************************************/
/*------------------------------------------------------------------------------
	ReadTempFromGridEYE for Dll
------------------------------------------------------------------------------*/
BOOLEAN ReadTempFromGridEYE_forDll(short* pshInTemp)
{
	UCHAR	i;
	UCHAR*	pucBuf = (UCHAR*)pshInTemp;

	for (i = 0; i < SNR_SZ; i++){
		short shTemp = (short)(((pucBuf[2 * i + 1] & 0x07) << 8) | pucBuf[2 * i]);
		if (0 != (0x08 & pucBuf[2 * i + 1])){
			shTemp -= 2048;
		}
		/* �ȉ����ɕϊ�   */
		/* bit15   : ??   */
		/* bit14-8 : ��?? */
		/* bit7-0  : ��?? */
		shTemp *= 64;

		pshInTemp[i] = shTemp;
	}

	return TRUE;
}
/*------------------------------------------------------------------------------
	GetTemp for Dll
------------------------------------------------------------------------------*/
BOOLEAN GetTemp_forDll(short* pshAveTemp, UCHAR* pucRawTempIndex, short* pshInTemp)
{
	//short*	pshInTemp = sWork.Sensor.shWork;	/* ���͉��x ��?�ۑ��p		*/

	{

		/* ���x�擾 */
		if (FALSE == ReadTempFromGridEYE_forDll(pshInTemp)){
			return FALSE;
		}

		LOGD("howard - HumanDetect_Measure1.1");

		/* ���x�X�V */
		*pucRawTempIndex = UpdateTemp(pshInTemp, pshAveTemp);
	}

	return TRUE;
}


/*------------------------------------------------------------------------------
	HumanDetect_Initialize
------------------------------------------------------------------------------*/
void HumanDetect_Initialize( void )
{
	IMGSZ	uImg;						/* ??��?�p �J�E?�^				*/
	UCHAR	i, j;

	sHDI.ucFrameNum = 0;
	sHDI.ucRawTempIndex = 0;
	sHDI.ucBackFrameNum = 0;
	for( i = 0; i < SNR_SZ; i++ ){
		for( j = 0; j < TEMP_FRAME_NUM; j++ ){
			sHDI.shRawTemp[j][i] = 0;
		}
	}
	for( uImg = 0; uImg < IMG_SZ; uImg++ ){
		sHDI.shAveTemp  [uImg] = 0;
		sHDI.shBackTemp [uImg] = 0;
		sHDI.shDiffTemp [uImg] = 0;
		sHDI.shDiffTemp2[uImg] = 0;
		sHDI.ucDetectImg[uImg] = 0;
		sHDI.ucDetectImg2[uImg] = 0;
	}
	sHDI.ucDetectNum = 0;
	sHDI.ucDetectNum2 = 0;
	for( i = 0; i < DETECTLIST_SIZE; i++ ){
		sHDI.sFeature[i].usArea = 0;
		sHDI.sFeature[i].sG.fx = 0;
		sHDI.sFeature[i].sG.fy = 0;

		sHDI.sDiffFeature[i].usArea = 0;
		sHDI.sDiffFeature[i].sG.fx = 0;
		sHDI.sDiffFeature[i].sG.fy = 0;
	}
	ClearOutInfo( &sHDI.sOutInfo );
	ClearOutInfo( &sHDI.sOutDiffInfo);

	ClearOutInfo( &sHDAI.sObjectOutInfo );


	ObjectDetection_Initialize( IMG_SZ_X, IMG_SZ_Y );


	ObjectTracking_Initialize(IMG_SZ_X, IMG_SZ_Y, OUT_SZ_X, OUT_SZ_Y);


	HumanDetectAlarm_Initialize(&sHDAI.sAlarmOutInfo);

	initTuningInfo();
}



/*------------------------------------------------------------------------------
	HumanDetect_Measure for JNI
------------------------------------------------------------------------------*/
void HumanDetect_Measure(short* pshInTemp)
{

	UCHAR	ucRawTempIndex;				/* ��?���O���x�ւ�Index			*/
	BOOLEAN bIsHumanInBed;

	HumanDetectAlarm_setParameter(g_sConfigParameters);

	/* To calculate ave temperature and interpolation */
	if (FALSE == GetTemp_forDll(sHDI.shAveTemp, &ucRawTempIndex, pshInTemp)){
		return;
	}


	/* count the temp image frame smaller than UCHAR_MAX_VAL (255) */
	if (sHDI.ucFrameNum < UCHAR_MAX_VAL){
		sHDI.ucFrameNum++;
	}


	/* 20170619 - howard -  calculate first init background temperature */
	if (FALSE == MakeInitialTemp2(ucRawTempIndex, sHDI.shBackTemp, sHDI.shAveTemp)){
		return;
	}


	/* Do Object detection - get
	 *  1. object in outter bed temperature
	 *  2. object for all temperature
	 */
	ObjectDetection_forDLL(
		sHDI.shAveTemp,					/* Input : ��?���㉷�x				*/
		sHDI.shBackTemp,				/* Input : �w�i���x					*/
		sHDI.shDiffTemp,				/* Output: �w�i�������x				*/
		sHDI.shDiffTemp2,				/* Output: �w�i�������x				*/
		sHDI.ucDetectImg,				/* Output: ����?�o��?				*/
		sHDI.ucDetectImg2,				/* Output: ����?�o��?				*/
		&g_sConfigParameters
		);

	Calc_BckRevTempImage(sHDI.shAveTemp, sHDI.shBackTemp, sHDI.shDiffTemp, sHDI.ucDetectImg);
	Calc_BckRevTempImage(sHDI.shAveTemp, sHDI.shBackTemp, sHDI.shDiffTemp, sHDI.ucDetectImg2);

	OD_FilterBedDetectImg(sHDI.ucDetectImg, &g_sConfigParameters);

	/* detect number of image for outter bed area */
	sHDI.ucDetectNum = DataLabeling8(sHDI.ucDetectImg);

	/* Get features */
	Calc_Features(sHDI.shDiffTemp, sHDI.ucDetectImg, sHDI.ucDetectNum, sHDI.sFeature);


	/* ���̒ǐ� */
	RestoreObjectTrackingIfo(OBJ_TRACKING);
	ObjectTracking(
		sHDI.ucDetectNum,				/* Input : ����?�o?				*/
		&sHDI.sFeature[1]				/* Input : �����ʏ��				*/
		);

	/* �l?�o?�ʂɂ��w�i��?? */
	//20170117
	//ConvertNotHumanAreaToBackground(
		//sHDI.ucDetectImg,				/* Input : ����?�o��?				*/
		//sHDI.shAveTemp,					/* Input : ��?���㉷�x				*/
		//sHDI.shBackTemp,				/* Update: �w�i���x					*/
		//sWork.Image.ucWork,				/* Update: ?�[�N�̈�				*/
		//sHDI.shHumanTemp
		//);


	/* out info - human tracking temperature */
	UpdateHumanTempOutInfo(
		sHDI.ucDetectImg,
		sHDI.shHumanTemp,
		sHDI.shBackTemp,
		sHDI.shAveTemp
		);

	/* �w�i�X�V */
	if (sHDI.ucFrameNum > 15) /* 20170619 - howard - initial backTemp */
		Updata_BackTemp(sHDI.ucDetectImg2, sHDI.shDiffTemp, sHDI.shBackTemp);

	/* �o�͍X�V */
	//UpdateOutInfo(sHDI.ucDetectNum, sHDI.sFeature, &sHDI.sOutInfo);
	UpdateOutInfo(OBJ_TRACKING);


    /*********************************************************/
	/*******    To check human is in bed or not     **********/
    /*********************************************************/

	/* 20170313 - To Tracking full temperature */
	/* 20170307 - detect number of image for bed area */
	sHDI.ucDetectNum2 = DataLabeling8(sHDI.ucDetectImg2);
	Calc_Features(sHDI.shDiffTemp, sHDI.ucDetectImg2, sHDI.ucDetectNum2, sHDI.sDiffFeature);
	bIsHumanInBed = CheckHumanInBed(&sHDI.sDiffFeature);
	if (sHDI.ucDetectNum2 == 0)bIsHumanInBed = FALSE;
	/* 20170313 - For tracking full temp */

    /*********************************************************/



    // 20170425 - for tuning info
    //updateTuningInfo(&sHDI);



	/* output human detect alarm result */
	HumanDetectAlarm_GetOutput(&sHDI.sOutInfo, &sHDAI.sObjectOutInfo, &sHDAI.sAlarmOutInfo,
		g_sConfigParameters.ucMode, bIsHumanInBed);


	UpdateTFFromAlarmInfo(&sHDI.sOutInfo);

	//for (int i = 0; i < IMG_SZ; i++){
	//	pshOutTemp[i] = sHDI.shHumanTemp[i];
	//}

	// call back to java HumanOutputImg
	call_back_to_java_humanOutputImg(sHDI.shHumanTemp);

	SaveObjectTrackingIfo(OBJ_TRACKING);



	/* 20170223 - For tracking diff temp */
	/*
	sHDI.ucDetectNum2 = DataLabeling8(sHDI.ucDetectImg2);
	Calc_Features(sHDI.shDiffTemp2, sHDI.ucDetectImg2, sHDI.ucDetectNum2, sHDI.sDiffFeature);

	RestoreObjectTrackingIfo(DIFF_TRACKING);
	ObjectTracking(
		sHDI.ucDetectNum2,				
		&sHDI.sDiffFeature[1]				
		);
	
	UpdateOutInfo(DIFF_TRACKING);
	HumanDetectAlarm_GetOutput(&sHDI.sOutDiffInfo, &sHDAI.sObjectOutInfo, &sHDAI.sAlarmOutInfo,
		g_sConfigParameters.ucMode);
	SaveObjectTrackingIfo(DIFF_TRACKING);
	*/

    USHORT area = 0;
    float x = 0.0f;
    float y = 0.0f;
    UCHAR index = 0;

    for (UCHAR i = 0; i < 11; i++){
        if (sHDI.sFeature[i].ucLabelNum > 0){
            index = i;
            break;
        }
    }

	/*********************************************************************/
	/***                   call back value by JNI                      ***/
	/*********************************************************************/
    USHORT usArea = 0;
    Coordinate sCenter;
    Rect sRect;
    sCenter.fx = 0.0f;
    sCenter.fy = 0.0f;
    sRect.ucMaxX = 0;
    sRect.ucMaxY = 0;
    sRect.ucMinX = 0;
    sRect.ucMinY = 0;

    if (sHDI.ucDetectNum2 > 0) {
        sRect.ucMinY = sHDI.sDiffFeature[index].sRectangle.ucMinY;
        sRect.ucMinX = sHDI.sDiffFeature[index].sRectangle.ucMinX;
        sRect.ucMaxY = sHDI.sDiffFeature[index].sRectangle.ucMaxY;
        sRect.ucMaxX = sHDI.sDiffFeature[index].sRectangle.ucMaxX;
        sCenter.fx = sHDI.sDiffFeature[index].sG.fx;
        sCenter.fy = sHDI.sDiffFeature[index].sG.fy;
        usArea = sHDI.sDiffFeature[index].usArea;
    }

	call_back_to_java(sHDI.ucDetectNum2, DETECT_NUM);
	call_back_to_java(usArea, HUMAN_AREA);
	call_back_to_java(sHDAI.sAlarmOutInfo.bIsHuman, HUMAN_IN_BED);
	call_back_to_java(sHDAI.sAlarmOutInfo.usAlertStatus, ALERT_STATUS);
	call_back_to_java(sHDAI.sAlarmOutInfo.bAlarm, DETAIL_STATUS);
    call_back_to_java(sCenter.fx, CENTER_X);
    call_back_to_java(sCenter.fy, CENTER_Y);
    call_back_to_java(sRect.ucMinY, MIN_Y);
    call_back_to_java(sRect.ucMinX, MIN_X);
    call_back_to_java(sRect.ucMaxY, MAX_Y);
    call_back_to_java(sRect.ucMaxX, MAX_X);

	LOGD("[howard] grideye_jni DETECT_NUM = %d", sHDI.ucDetectNum);
	LOGD("[howard] grideye_jni HUMAN_AREA = %d", usArea);
	LOGD("[howard] grideye_jni HUMAN_IN_BED = %d", sHDAI.sAlarmOutInfo.bIsHuman);
	LOGD("[howard] grideye_jni ALERT_STATUS = %d", sHDAI.sAlarmOutInfo.usAlertStatus);
	LOGD("[howard] grideye_jni centerX = %d", sCenter.fx);
	LOGD("[howard] grideye_jni centerY = %d", sCenter.fy);

	/*********************************************************************/
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void HumanDetect_GetOutputHumanDetectInfo(HumanDetectInfo* psHDI)
{
	*psHDI = sHDI;
}

void HumanDetect_GetOutputObjTrackingInfo(ObjectTrackingInfo* objTInfo)
{
	*objTInfo = sOTI_Obj;
}

void HumanDetect_GetOutputDiffTrackingInfo(ObjectTrackingInfo* objTInfo)
{
	*objTInfo = sOTI_Diff;
}

void HumanDetect_GetOutputHumanAlarmInfo(HumanDetectAlarmInfo* psHDAI)
{
	*psHDAI = sHDAI;
}

void HumanDetect_GetConfigParametersInfo(ConfigParameters* psCP)
{
	*psCP = g_sConfigParameters;
}

void HumanDetect_GetTuningInfo(TuningInfo* psTI)
{
	*psTI = sTuningInfo;
}


/******************************  END  **************************************
*************   Functions -- Do Human Detection for Dll  *******************
****************************************************************************/

void setConfigParameter_InputUShort(UCHAR key, USHORT value){

    if (key == KEY_LABELING_THRESH){
        g_sConfigParameters.sDetectParameters.usLabelingThresh = value;
    }
    else if (key == KEY_FNMV_THRESH){
        g_sConfigParameters.sTrackingParameters.ucFNMV_FrameMax = value;
    }
    else if (key == KEY_EDGE_THRESH){
        g_sConfigParameters.sTrackingParameters.ucEdgeThresh = value;
    }
    else if (key == KEY_HUMAN_FRAME_THRESH){
        g_sConfigParameters.sAlarmParameters.ucHumanThresh = value;
    }
    else if (key == KEY_BED_LEFT){
        g_sConfigParameters.sAlarmParameters.ucBedLeftBoardValue = value;
    }
    else if (key == KEY_BED_RIGHT){
        g_sConfigParameters.sAlarmParameters.ucBedRightBoardValue = value;
    }
    else if (key == KEY_LEFT_RANGE){
        g_sConfigParameters.sTrackingParameters.usLeftXLine = value;
    }
    else if (key == KEY_RIGHT_RANGE){
        g_sConfigParameters.sTrackingParameters.usRightXLine = value;
    }
    else if (key == KEY_TOP_RANGE){
        g_sConfigParameters.sTrackingParameters.usTopYLine = value;
    }
    else if (key == KEY_BOTTOM_RANGE){
        g_sConfigParameters.sTrackingParameters.usBottomYLine = value;
    }

}


void setConfigParameter_InputFloat(UCHAR key, float value){

    if (key == KEY_DIFF_TEMP_THRESH){
        g_sConfigParameters.sDetectParameters.fDiffTempThresh = value;
    }

}


void setConfigParameter(UCHAR mode){
	
	g_sConfigParameters.sDetectParameters.ucTempFrameNum = 8;
	g_sConfigParameters.sDetectParameters.ucBackFrameNum = 8;
	g_sConfigParameters.sDetectParameters.ucTempMedianFilter = 2;
	g_sConfigParameters.sDetectParameters.fTempSmoothCoeff = 0.1f;
	g_sConfigParameters.sDetectParameters.fDiffTempThresh = 0.3f;
	g_sConfigParameters.sDetectParameters.usLabelingThresh = 5;

	if (mode == BED_MODE){
		g_sConfigParameters.ucMode = BED_MODE;
		g_sConfigParameters.sTrackingParameters.fBA_CheckDiffArea = 0.3f;
		g_sConfigParameters.sTrackingParameters.fBA_CheckSumMove = 0.01f;
		g_sConfigParameters.sTrackingParameters.ucEdgeThresh = 600;
		g_sConfigParameters.sTrackingParameters.ucFNMV_FrameMax = 1800;
		g_sConfigParameters.sTrackingParameters.usLeftXLine = 11;
		g_sConfigParameters.sTrackingParameters.usRightXLine = 3;
		g_sConfigParameters.sTrackingParameters.usTopYLine = 12;
		g_sConfigParameters.sTrackingParameters.usBottomYLine = 2;

		g_sConfigParameters.sAlarmParameters.fQuiltAreaThresh = 1.5f;
		g_sConfigParameters.sAlarmParameters.fQuiltMovThresh = 0.5f;
		g_sConfigParameters.sAlarmParameters.ucFNMV_FrameMax = 9;
		g_sConfigParameters.sAlarmParameters.ucHumanThresh = 20;
		g_sConfigParameters.sAlarmParameters.ucLogSizeG = 10;
		g_sConfigParameters.sAlarmParameters.ucBedLeftBoardValue = 11;
		g_sConfigParameters.sAlarmParameters.ucBedRightBoardValue = 3;
	}
	else if (mode == BATH_MODE){
		g_sConfigParameters.ucMode = BATH_MODE;
		g_sConfigParameters.sTrackingParameters.fBA_CheckDiffArea = 0.3f;
		g_sConfigParameters.sTrackingParameters.fBA_CheckSumMove = 0.01f;
		g_sConfigParameters.sTrackingParameters.ucEdgeThresh = 10;
		g_sConfigParameters.sTrackingParameters.ucFNMV_FrameMax = 10;
		g_sConfigParameters.sTrackingParameters.usLeftXLine = 11;
		g_sConfigParameters.sTrackingParameters.usRightXLine = 3;
		g_sConfigParameters.sTrackingParameters.usTopYLine = 12;
		g_sConfigParameters.sTrackingParameters.usBottomYLine = 2;

		g_sConfigParameters.sAlarmParameters.fQuiltAreaThresh = 1.5f;
		g_sConfigParameters.sAlarmParameters.fQuiltMovThresh = 0.5f;
		g_sConfigParameters.sAlarmParameters.ucFNMV_FrameMax = 9;
		g_sConfigParameters.sAlarmParameters.ucHumanThresh = 10;
		g_sConfigParameters.sAlarmParameters.ucLogSizeG = 10;
		g_sConfigParameters.sAlarmParameters.ucBedLeftBoardValue = 0;
		g_sConfigParameters.sAlarmParameters.ucBedRightBoardValue = 0;
	}

}

/*------------------------------------------------------------------------------
	�o��
------------------------------------------------------------------------------*/
BOOLEAN* HumanDetectAlarm_GetOutputInfo()
{
	return sHDAI.sAlarmOutInfo.bAlarm;
}
/*------------------------------------------------------------------------------
	�o��
------------------------------------------------------------------------------*/
UCHAR* HumanDetect_GetOutput( UCHAR* pucSizeX, UCHAR*  pucSizeY )
{
	*pucSizeX = OUT_SZ_X;
	*pucSizeY = OUT_SZ_Y;

	return sHDI.sOutInfo.ucMap;
}
/*------------------------------------------------------------------------------
	�o��
------------------------------------------------------------------------------*/
UCHAR* HumanDetect_GetOutputHumanNum()
{
	return sHDI.sOutInfo.ucHumanNum;
}
/*------------------------------------------------------------------------------
	�o��
------------------------------------------------------------------------------*/
USHORT* HumanDetect_GetOutputFNMV()
{
	return sHDI.sOutInfo.usFNMV;
}
/*------------------------------------------------------------------------------
	�o��
------------------------------------------------------------------------------*/
USHORT* HumanDetect_GetOutputObjFNMV()
{
	return sHDAI.sObjectOutInfo.usFNMV;
}
/*------------------------------------------------------------------------------
	�o��
------------------------------------------------------------------------------*/
UCHAR* HumanDetect_GetOutputLabelNum()
{
	return sHDI.sOutInfo.ucLabelNum;
}
/*------------------------------------------------------------------------------
	�o��
------------------------------------------------------------------------------*/
Coordinate* HumanDetect_GetOutputCenter()
{
	return sHDI.sOutInfo.sCenter;
}
/* 20161118 - howardjhli - add more info for detect alarm */
/*------------------------------------------------------------------------------
	�o��
------------------------------------------------------------------------------*/
USHORT* HumanDetect_GetOutputArea()
{
	return sHDI.sOutInfo.usArea;
}

/*------------------------------------------------------------------------------
	�o��
------------------------------------------------------------------------------*/
short* HumanDetect_GetOutputBackTemp()
{
	return sHDI.shBackTemp;
}
/*------------------------------------------------------------------------------
	�o��
------------------------------------------------------------------------------*/
short* HumanDetect_GetOutputHumanTemp()
{
	return sHDI.shHumanTemp;
}

/*------------------------------------------------------------------------------
	��?���O���x�o��
------------------------------------------------------------------------------*/
UCHAR* HumanDetect_GetRawTemp( UCHAR* pucSizeX, UCHAR*  pucSizeY )
{
	UCHAR	i;
	UCHAR*	pucWork = sWork.Image.ucWork;

	/* Index��?�̊i�[��ɍX�V */
	UCHAR	nIndex = ( sHDI.ucRawTempIndex + (TEMP_FRAME_NUM-1) ) % TEMP_FRAME_NUM;

	*pucSizeX = SNR_SZ_X;
	*pucSizeY = SNR_SZ_Y;

	for( i = 0; i < SNR_SZ; i++ ){
		short shWork = sHDI.shRawTemp[nIndex][i];
		if( shWork < 0 ){
			pucWork[i] = 0;
		}else{
			pucWork[i] = (UCHAR)( ( shWork >> 6 ) & 0x00FF );
		}
	}

	return pucWork;
}


/*******************************************************************************
 * private?�\�b�h��`
 ******************************************************************************/

/*------------------------------------------------------------------------------
	���x�擾
------------------------------------------------------------------------------*/
BOOLEAN GetTemp( short* pshAveTemp, UCHAR* pucRawTempIndex )
{
	short*	pshInTemp = sWork.Sensor.shWork;	/* ���͉��x ��?�ۑ��p		*/

	{

		/* ���x�擾 */
		if( FALSE == ReadTempFromGridEYE( pshInTemp ) ){
			return FALSE;
		}

		/* ���x�X�V */
		*pucRawTempIndex = UpdateTemp( pshInTemp, pshAveTemp );
	}

	return TRUE;
}

/*------------------------------------------------------------------------------
	Grid-EYE����̉��x�f�[�^�ǂݏo��
------------------------------------------------------------------------------*/
BOOLEAN ReadTempFromGridEYE( short* pshInTemp )
{
	UCHAR	i;
	UCHAR*	pucBuf = (UCHAR*)pshInTemp;

	//if( FALSE == I2C_Read( GRIDEYE_ADR, GRIDEYE_REG_TMP00, pucBuf, (UCHAR)SNR_SZ * 2 ) ){
	//	return FALSE;
	//}
	
	/* Get temperature register value. */
	//if (FALSE ==
	//bAMG_PUB_I2C_Read(GRIDEYE_ADR, GRIDEYE_REG_TMP00, (UCHAR)SNR_SZ * 2, pucBuf)) {
		//return FALSE;   
	//}

	for( i = 0; i < SNR_SZ; i++ ){
		short shTemp = (short)( ((pucBuf[2*i+1] & 0x07) << 8) | pucBuf[2*i] );
		if( 0 != (0x08 & pucBuf[2*i+1]) ){
			shTemp -= 2048;
		}
		/* �ȉ����ɕϊ�   */
		/* bit15   : ??   */
		/* bit14-8 : ��?? */
		/* bit7-0  : ��?? */
		shTemp *= 64;

		pshInTemp[i] = shTemp;
	}

	return TRUE;
}

/*------------------------------------------------------------------------------
	���x�f�[�^�̍X�V
------------------------------------------------------------------------------*/
UCHAR UpdateTemp( short* pshInTemp, short* pshAveTemp )
{
	IMGSZ	uImg;						/* ??��?�p �J�E?�^				*/
	UCHAR	i;
	UCHAR	ucIndex			= sHDI.ucRawTempIndex;

	short*	pshTopRawTemp	= sHDI.shRawTemp[0];
	short*	pshCurRawTemp	= sHDI.shRawTemp[ucIndex];


	for( i = 0; i < SNR_SZ; i++ ){

		uImg = ImageIndex( SensorX(i)*2, SensorY(i)*2 );

		/* ���x�X�V */
		pshCurRawTemp[i] = pshInTemp[i];
		/* ���ϒl�v�Z */
		if( sHDI.ucFrameNum >= TEMP_FRAME_NUM ){
			short	shAveTemp;
			shAveTemp = CalcAverage(&pshTopRawTemp[i], SNR_SZ, TEMP_FRAME_NUM, g_sConfigParameters.sDetectParameters.ucTempMedianFilter);

			//pshAveTemp[uImg] = (short)( pshAveTemp[uImg] * (1-g_sConfigParameters.sDetectParameters.fTempSmoothCoeff) + shAveTemp * g_sConfigParameters.sDetectParameters.fTempSmoothCoeff );
			pshAveTemp[uImg] = (short)(shAveTemp);

		}
	}
	
	{
		/* Index��?�̊i�[��ɍX�V */
		sHDI.ucRawTempIndex = ( ucIndex + 1 ) % TEMP_FRAME_NUM;

		/* ���`��� */
		LinearInterpolationSQ15( pshAveTemp );
	}

	LOGD("howard - HumanDetect_Measure1.2");

	/* ���݂̊i�[��|�C?�^��ԋp */
	return ucIndex;
}

/*------------------------------------------------------------------------------
	���ϒl���v�Z

	pshArray : TEMP_FRAME_NUM rawData array
	ucSkip : 8*8 = 64
	ucSize : TEMP_FRAME_NUM
	ucMedian : Median Filter
------------------------------------------------------------------------------*/
short CalcAverage( short* pshArray, UCHAR ucSkip, UCHAR ucSize, UCHAR ucMedian )
{
	UCHAR	i, j;

	BOOLEAN	bList[TEMP_FRAME_NUM];

	//BOOLEAN*	bList = malloc(TEMP_FRAME_NUM*sizeof(USHORT));

	short	shRet = 0;
	
	if( (ucSize <= 1) || (TEMP_FRAME_NUM < ucSize) ){
		return( *pshArray );
	}

	ClearArray( bList, ucSize, TRUE );

	ucSkip = MAX( ucSkip, 1 );

	ucMedian = MIN( ucMedian, (ucSize - 1) / 2 );

	for( j = 0; j < ucMedian; j++ ){
		short	shMaxData = SHORT_MIN_VAL;
		short	shMinData = SHORT_MAX_VAL;
		UCHAR	ucIndex;
		for( i = 0; i < ucSize; i++ ){
			short	shCurData = pshArray[i*ucSkip];
			if( bList[i] == FALSE ){
				continue;
			}
			if( shMaxData < shCurData ){
				shMaxData = shCurData;
				ucIndex = i;
			}
		}
		bList[ucIndex] = FALSE;

		for( i = 0; i < ucSize; i++ ){
			short	shCurData = pshArray[i*ucSkip];
			if( bList[i] == FALSE ){
				continue;
			}
			if( shMinData > shCurData ){
				shMinData = shCurData;
				ucIndex = i;
			}
		}
		bList[ucIndex] = FALSE;
	}

	for( i = 0; i < ucSize; i++ ){
		short	shCurData = pshArray[i*ucSkip];	// Get the first pixel of each temp frame
		if( bList[i] == FALSE ){
			continue;
		}
		shRet += shCurData / ( ucSize - ucMedian * 2 );
	}

	return( shRet );
}

/*------------------------------------------------------------------------------
	?�����x����
------------------------------------------------------------------------------*/
BOOLEAN MakeInitialTemp( UCHAR ucRawTempIndex, short* pshBackTemp, short* pshAveTemp )
{
	IMGSZ	uImg;						/* ??��?�p �J�E?�^				*/
	UCHAR	i;

	/* ?�����x�����ς����� */
	if( sHDI.ucBackFrameNum >= g_sConfigParameters.sDetectParameters.ucBackFrameNum ){
		return TRUE;					/* ?�����x������ */
	}

	/* ?�����x�v�Z */
	{
		short*	pshInTemp		= sHDI.shRawTemp[ucRawTempIndex];

		for( i = 0; i < SNR_SZ; i++ ){
			uImg = ImageIndex( SensorX(i)*2, SensorY(i)*2 );
			/* 1�t?�[?�̏d�ݕ������Z */
			pshBackTemp[uImg] += pshInTemp[i] / g_sConfigParameters.sDetectParameters.ucBackFrameNum;

		}
	}

	/* �t?�[??�X�V */
	sHDI.ucBackFrameNum++;

	/* �w��t?�[??�X�V������Ō�ɉ��x�g�� */
	if( sHDI.ucBackFrameNum == g_sConfigParameters.sDetectParameters.ucBackFrameNum ){

		/*--------------------------------------------------*/
		/* �w�i���x ?�����x�ݒ�							*/
		/*--------------------------------------------------*/
		/* ���`��� */
		LinearInterpolationSQ15( pshBackTemp );

		/*--------------------------------------------------*/
		/* ��?���㉷�x ?�����x�ݒ�						*/
		/*--------------------------------------------------*/
		for( uImg = 0; uImg < IMG_SZ; uImg++ ){
			pshAveTemp[uImg] = pshBackTemp[uImg];
		}
	}

	return FALSE;
}

/*------------------------------------------------------------------------------
MakeInitialTemp2 - backTemp filter part of high temp and low temp
------------------------------------------------------------------------------*/
BOOLEAN MakeInitialTemp2(UCHAR ucRawTempIndex, short* pshBackTemp, short* pshAveTemp)
{
	IMGSZ    uImg;                           /* ??? ?              */
	UCHAR    i;
	short temp;

	/* ? */
	if (sHDI.ucBackFrameNum >= g_sConfigParameters.sDetectParameters.ucBackFrameNum){
		return TRUE;                    /* ? */
	}

	/* ? */
	{
		short*   pshInTemp = sHDI.shRawTemp[ucRawTempIndex];

		for (i = 0; i < SNR_SZ; i++){
			uImg = ImageIndex(SensorX(i) * 2, SensorY(i) * 2);
			/* 1?? */
			pshBackTemp[uImg] += pshInTemp[i] / g_sConfigParameters.sDetectParameters.ucBackFrameNum;

		}
	}

	/* ??? */
	sHDI.ucBackFrameNum++;

	/* ??? */
	if (sHDI.ucBackFrameNum == g_sConfigParameters.sDetectParameters.ucBackFrameNum){



		/*--------------------------------------------------*/
		/*  ?                                */
		/*--------------------------------------------------*/
		/*  */
		LinearInterpolationSQ15(pshBackTemp);

		/*--------------------------------------------------*/
		/* ? ?                         */
		/*--------------------------------------------------*/
		for (uImg = 0; uImg < IMG_SZ; uImg++){
			pshAveTemp[uImg] = pshBackTemp[uImg];
		}



		/*--------------------------------------------------*/
		/*            filter high and low temp              */
		/*--------------------------------------------------*/
		short minTemp = 0;
		int aveTemp = 0;
		// 1. get minimun value
		minTemp = pshBackTemp[0];
		for (i = 0; i < SNR_SZ; i++){
			uImg = ImageIndex(SensorX(i) * 2, SensorY(i) * 2);
			if (pshBackTemp[uImg] < minTemp){
				minTemp = pshBackTemp[uImg];
			}
		}

		// 2. get ave value
		short count = 0;

		for (i = 0; i < SNR_SZ; i++){
			uImg = ImageIndex(SensorX(i) * 2, SensorY(i) * 2);
			if (pshBackTemp[uImg] <= (minTemp + TempFtoS(1.0f))){
				aveTemp += pshBackTemp[uImg];
				count++;
			}
		}

		if (count > 0) aveTemp = aveTemp / count;


		// 3. replace high temp
		for (i = 0; i < SNR_SZ; i++){
			uImg = ImageIndex(SensorX(i) * 2, SensorY(i) * 2);
			/*
            if (pshBackTemp[uImg] > (minTemp + TempFtoS(1.0f)) && pshBackTemp[uImg] <= (minTemp + TempFtoS(1.5f))){
                pshBackTemp[uImg] = (aveTemp*0.3) + (pshBackTemp[uImg]*0.7);
            }

            if (pshBackTemp[uImg] > (minTemp + TempFtoS(1.5f)) && pshBackTemp[uImg] <= (minTemp + TempFtoS(2.0f))){
                pshBackTemp[uImg] = (aveTemp*0.5) + (pshBackTemp[uImg] * 0.5);
            }
            */
			if (pshBackTemp[uImg] > (minTemp + TempFtoS(2.0f))){
				pshBackTemp[uImg] = (aveTemp*0.7) + (pshBackTemp[uImg] * 0.3);
			}
		}

		/*--------------------------------------------------*/
		/*  ?                                */
		/*--------------------------------------------------*/
		/*  */
		LinearInterpolationSQ15(pshBackTemp);

	}

	return FALSE;
}

/*------------------------------------------------------------------------------
	Linear Interpolation
------------------------------------------------------------------------------*/
void LinearInterpolationSQ15( short* pshTemp )
{
	IMGSZ	uImg;						/* ??��?�p �J�E?�^				*/
	UCHAR	ucX, ucY;

	for( ucY = 0; ucY < IMG_SZ_Y; ucY += 2 ){
		for( ucX = 1; ucX < IMG_SZ_X; ucX += 2 )
		{
			uImg = ImageIndex( ucX, ucY );
			pshTemp[uImg] = ( pshTemp[uImg-1] + pshTemp[uImg+1] ) / 2;
		}
	}
	for( ucY = 1; ucY < IMG_SZ_Y; ucY += 2 ){
		for( ucX = 0; ucX < IMG_SZ_X; ucX++ )
		{
			uImg = ImageIndex( ucX, ucY );
			pshTemp[uImg] = ( pshTemp[uImg-IMG_SZ_X] + pshTemp[uImg+IMG_SZ_X] ) / 2;
		}
	}
}

/*------------------------------------------------------------------------------
	����?�o(�w�i????��?����)
------------------------------------------------------------------------------*/
void Calc_BckRevTempImage( short* pshAveTemp, short* pshBackTemp, short* pshDiffTemp, UCHAR* pucDetectImg )
{
	IMGSZ	uImg;						/* ??��?�p �J�E?�^				*/
	float   fDiffTempThresh;
	/*
	for( uImg = 0; uImg < IMG_SZ; uImg++ ){
		pshDiffTemp [uImg] = (short)( pshAveTemp[uImg] - pshBackTemp[uImg] );
		pucDetectImg[uImg] = ( pshDiffTemp[uImg] >= TempFtoS(g_sConfigParameters.sDetectParameters.fDiffTempThresh) ) ? 0xFF : 0;
	}
	*/
	fDiffTempThresh = TempFtoS(g_sConfigParameters.sDetectParameters.fDiffTempThresh);


	//for (uImg = 0; uImg < IMG_SZ; uImg++){
		//pshDiffTemp[uImg] = (short)(pshAveTemp[uImg] - pshBackTemp[uImg]);
	//}

	//if (fDiffTempThresh <= 0.1f)
	//	fDiffTempThresh = Calc_DiffTempThresh(pshDiffTemp);

		
	for (uImg = 0; uImg < IMG_SZ; uImg++){
		//pshDiffTemp[uImg] = (short)(pshAveTemp[uImg] - pshBackTemp[uImg]);
		if (pucDetectImg[uImg] != 0){
			pucDetectImg[uImg] = (pshDiffTemp[uImg] >= (fDiffTempThresh)) ? 0xFF : 0;
		}
	}
}

float Calc_DiffTempThresh(short* pshDiffTemp){
	IMGSZ	uImg;
	SHORT  sum = 0;
	SHORT  count = 0;

	for (uImg = 0; uImg < IMG_SZ; uImg++){
		if (pshDiffTemp[uImg] >= TempFtoS(DIFFTEMP_THRESH)){
			sum += pshDiffTemp[uImg];
			count++;
		}
	}


	if ((sum / count) > TempFtoS(2.5f))
		return 1.0f;
	else if ((sum / count) > TempFtoS(1.8f))
		return 0.7f;
	else if ((sum / count) > TempFtoS(1.2f))
		return 0.5f;
	else 
		return 0.3f;

	//return ((sum / count));
}


/*------------------------------------------------------------------------------
	�����ʎZ�o�F?�x??�O
------------------------------------------------------------------------------*/
UCHAR DataLabeling8( UCHAR* pucDetectImg )
{
	IMGSZ	CheckPixel( UCHAR, UCHAR, UCHAR*, IMGSZ*, IMGSZ );

	IMGSZ	uImg, sImg2;				/* ??��?�p �J�E?�^				*/
	UCHAR	ucDetectNum = 0;			/* 									*/
	IMGSZ*	psSearchList = sWork.Image.uImg;	/* �T��Index?�X�g(?�����s�v)	*/

	/* �S��f�ɂ��� */
	for( uImg = 0; uImg < IMG_SZ; uImg++ ){
		UCHAR	ucLabelNum	= 0;		/* ?�x?��?						*/
		IMGSZ	sIndex		= 0;		/* 									*/
		IMGSZ	sIndexAdd	= CheckPixel( ImageX(uImg), ImageY(uImg), pucDetectImg, psSearchList, sIndex );

		/* ���͓_�̒T�� */
		while( sIndex < sIndexAdd ){
			UCHAR	ucX = ImageX( psSearchList[sIndex] );
			UCHAR	ucY = ImageY( psSearchList[sIndex] );
			/* �㉺��2��f */
			{
				if( ucY-1 >= 0 ){							/* �㑤�̉�f	*/
					sIndexAdd += CheckPixel( ucX,   ucY-1, pucDetectImg, psSearchList, sIndexAdd );
				}
				if( ucY+1 < IMG_SZ_Y ){						/* �����̉�f	*/
					sIndexAdd += CheckPixel( ucX,   ucY+1, pucDetectImg, psSearchList, sIndexAdd );
				}
			}
			/* ������3��f */
			if( ucX-1 >= 0 ){
				{											/* �����̉�f	*/
					sIndexAdd += CheckPixel( ucX-1, ucY,   pucDetectImg, psSearchList, sIndexAdd );
				}
				if( ucY-1 >= 0 ){							/* ����̉�f	*/
					sIndexAdd += CheckPixel( ucX-1, ucY-1, pucDetectImg, psSearchList, sIndexAdd );
				}
				if( ucY+1 < IMG_SZ_Y ){						/* �����̉�f	*/
					sIndexAdd += CheckPixel( ucX-1, ucY+1, pucDetectImg, psSearchList, sIndexAdd );
				}
			}
			/* �E����3��f */
			if( ucX+1 < IMG_SZ_X ){
				{											/* �E���̉�f	*/
					sIndexAdd += CheckPixel( ucX+1, ucY,   pucDetectImg, psSearchList, sIndexAdd );
				}
				if( ucY-1 >= 0 ){							/* �E��̉�f	*/
					sIndexAdd += CheckPixel( ucX+1, ucY-1, pucDetectImg, psSearchList, sIndexAdd );
				}
				if( ucY+1 < IMG_SZ_Y ){						/* �E���̉�f	*/
					sIndexAdd += CheckPixel( ucX+1, ucY+1, pucDetectImg, psSearchList, sIndexAdd );
				}
			}
			sIndex++;
		}
		
		/* �ʐς������l?? */
		if( sIndex <= g_sConfigParameters.sDetectParameters.usLabelingThresh ){
			ucLabelNum = 0;
		}else{
			ucDetectNum++;
			ucLabelNum = ucDetectNum;
		}

		/* ?�x??�O */
		for( sImg2 = 0; sImg2 < sIndex; sImg2++ ){
			pucDetectImg[ psSearchList[sImg2] ] = ucLabelNum;
		}
	}

	return MIN( ucDetectNum, DETECT_MAX );
}

/*------------------------------------------------------------------------------
	�����ʎZ�o�F1��f�`�F�b�N
------------------------------------------------------------------------------*/
IMGSZ CheckPixel( UCHAR ucX, UCHAR ucY, UCHAR* pucDetectImg, IMGSZ* psSearchList, IMGSZ sAdd )
{
	IMGSZ	sCheckIndex = ImageIndex( ucX, ucY );

	if( pucDetectImg[sCheckIndex] != 0xFF ){
		return 0;
	}

	pucDetectImg[sCheckIndex] = 0;
	psSearchList[sAdd] = sCheckIndex;
	return 1;
}

/*------------------------------------------------------------------------------
	�����ʎZ�o�F�����ʂ��v�Z
------------------------------------------------------------------------------*/
void Calc_Features( short* pshDiffTemp, UCHAR* pucDetectImg, UCHAR ucDetectNum, Feature* psFeature )
{
	UCHAR	i;

	for( i = 1; i <= ucDetectNum; i++ ){
		Calc_AreaFeatures( pshDiffTemp, pucDetectImg, i, &(psFeature[i]) );
	}

	Sort_AreaFeatures( ucDetectNum, psFeature );
}

/*------------------------------------------------------------------------------
	�����ʎZ�o�F?�x?��?�̃G?�A�̓����ʂ��v�Z
------------------------------------------------------------------------------*/
void Calc_AreaFeatures( short* pshDiffTemp, UCHAR* pucLabelData, UCHAR ucLabelNumber, Feature* psFeature )
{
	IMGSZ	uImg;						/* ??��?�p �J�E?�^				*/
	USHORT	usArea	= 0;				/* �ʐ�								*/
	UCHAR	ucMinX	= IMG_SZ_X -1;		/* �O�ڋ�`�̍��W ��				*/
	UCHAR	ucMaxX	= 0;				/* �O�ڋ�`�̍��W �E				*/
	UCHAR	ucMinY	= IMG_SZ_Y -1;		/* �O�ڋ�`�̍��W ��				*/
	UCHAR	ucMaxY	= 0;				/* �O�ڋ�`�̍��W ��				*/
	short	shMaxDiffTemp = 0;			/* �w�i�������x�ő�l				*/
	float	fGx		= 0;				/* �d�S���Wx�̏d�ݕt���a			*/
	float	fGy		= 0;				/* �d�S���Wy�̏d�ݕt���a			*/
	float	fGw		= 0;				/* �ʐς̏d�ݕt���a					*/

	/* ?�x?��?��ۑ� */
	psFeature->ucLabelNum = ucLabelNumber;

	/* �ʐρA�ڋ�`�̍��W�A�w�i�������x�ő�l���v�Z */
	for( uImg = 0; uImg < IMG_SZ; uImg++ ){
		if( pucLabelData[uImg] == ucLabelNumber ){
			usArea++;
			ucMinX = MIN( ucMinX, ImageX(uImg) );
			ucMaxX = MAX( ucMaxX, ImageX(uImg) );
			ucMinY = MIN( ucMinY, ImageY(uImg) );
			ucMaxY = MAX( ucMaxY, ImageY(uImg) );
			shMaxDiffTemp = MAX( shMaxDiffTemp, pshDiffTemp[uImg] );
		}
	}
	psFeature->usArea				= usArea;
	psFeature->sRectangle.ucMinX	= ucMinX;
	psFeature->sRectangle.ucMaxX	= ucMaxX;
	psFeature->sRectangle.ucMinY	= ucMinY;
	psFeature->sRectangle.ucMaxY	= ucMaxY;
	psFeature->shMaxDiffTemp		= shMaxDiffTemp;

	/* �d�S���v�Z */
	if( usArea > 0 ){
		/* �ő剷�x�����v�Z */
		short	shDiffMax = 1;
		float	fDiffMax;
		for( uImg = 0; uImg < IMG_SZ; uImg++ ){
			if( pucLabelData[uImg] == ucLabelNumber ){
				shDiffMax = MAX( shDiffMax, pshDiffTemp[uImg] );
			}
		}
		fDiffMax = TempStoF( shDiffMax );

		/* �d�ݕt���a�v�Z */
		for( uImg = 0; uImg < IMG_SZ; uImg++ ){
			UCHAR ucX = ImageX( uImg );
			UCHAR ucY = ImageY( uImg );

			if( pucLabelData[uImg] == ucLabelNumber ){
				float fWeight = TempStoF( pshDiffTemp[uImg] ) / fDiffMax;
				fGx += fWeight * (ucX + 1);
				fGy += fWeight * (ucY + 1);
				fGw += fWeight;
			}
		}
	}
	if( fGw > 0 ){
		psFeature->sG.fx = fGx / fGw - 1;
		psFeature->sG.fy = fGy / fGw - 1;
	}else{
		psFeature->sG.fx = 0;
		psFeature->sG.fy = 0;
	}
}

/*------------------------------------------------------------------------------
	�����ʎZ�o�F�ʐς��傫��?�ɕ��בւ�
------------------------------------------------------------------------------*/
void Sort_AreaFeatures( UCHAR ucDetectNum, Feature* psFeature )
{
	UCHAR	i, j;

	for( i = 1; i < ucDetectNum; i++ ){
		UCHAR	ucMaxIndex = i;
		USHORT	usMaxArea  = psFeature[i].usArea;
		for( j = i+1; j <= ucDetectNum; j++ ){
			USHORT usArea = psFeature[j].usArea;
			if( usMaxArea < usArea ){
				usMaxArea = usArea;
				ucMaxIndex = j;
			}
		}
		if( i != ucMaxIndex ){
			SWAP( Feature, psFeature[i], psFeature[ucMaxIndex] );
		}
	}
}

/*------------------------------------------------------------------------------
	��f�c��
------------------------------------------------------------------------------*/
void ImageDilation( UCHAR* pucInImg, UCHAR* pucOutImg )
{
	IMGSZ	uImg;						/* ??��?�p �J�E?�^				*/

	for( uImg = 0; uImg < IMG_SZ; uImg++ ){
		pucOutImg[uImg] = 0;
	}
	for( uImg = 0; uImg < IMG_SZ; uImg++ ){
		UCHAR ucX = ImageX( uImg );
		UCHAR ucY = ImageY( uImg );

		if( pucInImg[uImg] == 0 ){
			continue;
		}

		pucOutImg[uImg] = 1;
		if( ucX != 0          ){ pucOutImg[uImg-1       ] = 1; }
		if( ucX != IMG_SZ_X-1 ){ pucOutImg[uImg+1       ] = 1; }
		if( ucY != 0          ){ pucOutImg[uImg-IMG_SZ_X] = 1; }
		if( ucY != IMG_SZ_Y-1 ){ pucOutImg[uImg-IMG_SZ_X] = 1; }
	}
}

/*------------------------------------------------------------------------------
	�w�i�X�V
------------------------------------------------------------------------------*/
void Updata_BackTemp( UCHAR* pucDetectImg, short* pshDiffTemp, short* pshBackTemp )
{
	IMGSZ	uImg;						/* ??��?�p �J�E?�^				*/
	IMGSZ	sSumNoDetected = 0;			/* ���̕s?�o?						*/
	IMGSZ*	pucList = sWork.Image2.uImg;
	UCHAR*	pucIDX  = sWork.Image2.ucWork;
	float	fAveDiff;					/* ���ϒl							*/
	float	fS = 0.0;					/* �c�����a						*/
	float	fStdDiff;					/* �W?�΍�							*/
	float	fCoff;						/* �w�i�X�V�W?						*/

	//short sSortDiffTempList[IMG_SZ];
	//ClearArray(sSortDiffTempList, IMG_SZ, 0);


	ImageDilation( pucDetectImg, pucIDX );

	/* ���̕s?�o?�v�Z */
	for( uImg = 0; uImg < IMG_SZ; uImg++ ){
		if( pucIDX[uImg] == 0 )
		{
			pucList[sSumNoDetected] = uImg;
			sSumNoDetected++;
		}
	}


	/* ���̕s?�o��f��?�l�ȉ��̂Ƃ��͑S��f�̔w�i���x��+0.1?������I�� */
	if( sSumNoDetected <= BKUPDT_ALLUP_THRESH ){
		for( uImg = 0; uImg < IMG_SZ; uImg++ ){
			pshBackTemp[uImg] += TempFtoS( BKUPDT_ALLUP_TEMP );
		}
		return;
	}

	/*
	sSortDiffTempList[0] = TempStoF(pshDiffTemp[pucList[0]]);
	for (uImg = 1; uImg < sSumNoDetected - 1; uImg++){
		if (sSortDiffTempList[uImg - 1] < TempStoF(pshDiffTemp[pucList[uImg]])){
			sSortDiffTempList[uImg] = sSortDiffTempList[uImg - 1];
			sSortDiffTempList[uImg - 1] = TempStoF(pshDiffTemp[pucList[uImg]]);
		}
		else {
			sSortDiffTempList[uImg] = TempStoF(pshDiffTemp[pucList[uImg]]);
		}
	}

	fAveDiff = 0;
	for (uImg = 15; uImg < sSumNoDetected - 5; uImg++){
		fAveDiff += sSortDiffTempList[uImg];
	}
	fAveDiff /= (sSumNoDetected - 20);
	*/


	/* ���̕s?�o��?�̉��x�����v�Z */
	fAveDiff = 0;
	for( uImg = 0; uImg < sSumNoDetected; uImg++ ){
		fAveDiff += TempStoF(pshDiffTemp[pucList[uImg]]);
	}
	fAveDiff /= sSumNoDetected;


	/* �W?�΍��v�Z */
	for( uImg = 0; uImg < sSumNoDetected; uImg++ ){
		fS += POW( TempStoF( pshDiffTemp[pucList[uImg]] ) - fAveDiff );
	}
	fStdDiff = sqrtf( fS / sSumNoDetected );

	/* �w�i�X�V�W?�ݒ� */
	fCoff = BKUPDT_COEFF_BASE;
	/* ���̕s?�o��?�̉��x������?�l�𒴂����?�͕␳ */
	fCoff += (ABS(fAveDiff) > BKUPDT_COEFF_AVE_T ) ? (ABS(fAveDiff) - BKUPDT_COEFF_AVE_T ) : 0;
	/* ���̕s?�o��?�̉��x�����̕W?�΍���?�l�𒴂����?�͕␳ */
	fCoff += (fStdDiff > BKUPDT_COEFF_STDV_T) ? (fStdDiff - BKUPDT_COEFF_STDV_S) : 0;
	fCoff = MIN( fCoff, BKUPDT_COEFF_MAX );

	/* �w�i�X�V */
	for( uImg = 0; uImg < IMG_SZ; uImg++ ){
		short shTemp;
		if( pucIDX[uImg] == 0 )
		{
			shTemp = TempFtoS( fCoff * TempStoF( pshDiffTemp[uImg] ) );			
		}else{
			shTemp = TempFtoS( fCoff * fAveDiff );
			shTemp = MAX( shTemp, TempFtoS(-0.05f) );
		}
		pshBackTemp[uImg] += shTemp;
		
	}

}

/*------------------------------------------------------------------------------
	�o�͏��N?�A
------------------------------------------------------------------------------*/
void ClearOutInfo( OutInfo* psOutInfo )
{
	UCHAR	i;
	psOutInfo->ucHumanNum = 0;
	for( i = 0; i < OUT_SZ; i++ ){
		psOutInfo->ucMap[i] = 0;
	}
	for( i = 0; i < OT_TRACKING_MAX; i++ ){
		psOutInfo->ucLabelNum[i] = 0;
	}
	for( i = 0; i < OT_TRACKING_MAX; i++ ){
		psOutInfo->sCenter[i].fx = 0;
		psOutInfo->sCenter[i].fy = 0;
	}
	/* 20161118 - howardjhli - add more info for detect alarm */
	for( i = 0; i < OT_TRACKING_MAX; i++ ){
		psOutInfo->usArea[i] = 0;
	}
	for( i = 0; i < OT_TRACKING_MAX; i++ ){
		psOutInfo->sRectangle[i].ucMaxX = 0;
		psOutInfo->sRectangle[i].ucMaxY = 0;
		psOutInfo->sRectangle[i].ucMinX = 0;
		psOutInfo->sRectangle[i].ucMinY = 0;
	}
	//for( i = 0; i < OT_TRACKING_MAX; i++ ){
	//psOutInfo->sRect_Prev[i].ucMaxX = 0;
	//psOutInfo->sRect_Prev[i].ucMaxY = 0;
	//psOutInfo->sRect_Prev[i].ucMinX = 0;
	//psOutInfo->sRect_Prev[i].ucMinY = 0;
	//}
	//for( i = 0; i < OT_TRACKING_MAX; i++ ){
		//psOutInfo->ucNotHumanFrame[i] = 0;
	//}
	//for( i = 0; i < OT_TRACKING_MAX; i++ ){
		//psOutInfo->ucBlinkCount[i] = 0;
	//}
	for( i = 0; i < OT_TRACKING_MAX; i++ ){
		for (int j = 0; j < OT_LOGSIZE_D; j++){
			psOutInfo->cLogDir[i][j] = 0;
		}
	}
	for( i = 0; i < OT_TRACKING_MAX; i++ ){
		psOutInfo->usFNMV[i] = 0;
		psOutInfo->bInnerLine[i] = FALSE;
		psOutInfo->bOutterLine[i] = FALSE;
		psOutInfo->ucMoveStatus[i] = MOVE_NA;
	}

}

/*------------------------------------------------------------------------------
	�o�͏��X�V
------------------------------------------------------------------------------*/
/*
void UpdateOutInfo( UCHAR ucDetectNum, Feature* psFeature, OutInfo* psOutInfo )
{
	UCHAR	i, ucX, ucY;

	ObjectTracking_GetOutput(
		&sHDI.sOutInfo.ucHumanNum,	
		sHDI.sOutInfo.ucMap,				
		sHDI.sOutInfo.ucLabelNum,
		sHDI.sOutInfo.sCenter,
		//sHDI.sOutInfo.ucNotHumanFrame,
		//sHDI.sOutInfo.ucBlinkCount,
		sHDI.sOutInfo.cLogDir,
		sHDI.sOutInfo.usFNMV,
		// 20161118 - howardjhli - add more info for detect alarm 
		sHDI.sOutInfo.usArea,
		sHDI.sOutInfo.sRectangle,
		//sHDI.sOutInfo.sRect_Prev
		sHDI.sOutInfo.usFrame
	);

	ObjectTracking_GetALarmInfoOutput(
		&sHDAI.sObjectOutInfo.ucHumanNum,
		sHDAI.sObjectOutInfo.ucMap,
		sHDAI.sObjectOutInfo.ucLabelNum,
		sHDAI.sObjectOutInfo.sCenter,
		//sHDAI.sObjectOutInfo.ucNotHumanFrame,
		//sHDAI.sObjectOutInfo.ucBlinkCount,
		sHDAI.sObjectOutInfo.cLogDir,
		sHDAI.sObjectOutInfo.usFNMV,
		// 20161118 - howardjhli - add more info for detect alarm 
		sHDAI.sObjectOutInfo.usArea,
		sHDAI.sObjectOutInfo.sRectangle,
		//sHDAI.sObjectOutInfo.sRect_Prev
		sHDAI.sObjectOutInfo.usFrame
	);
	

	
	//for( ucX = 0; ucX < OUT_SZ_X; ucX++ ){
		//for( ucY = 0; ucY < OUT_SZ_Y; ucY++ ){
			//if( psOutInfo->ucMap[ OutImgIndex(ucX,ucY)] ){
				//break;
			//}
		//}
		//for( ; ucY < OUT_SZ_Y; ucY++ ){
			//psOutInfo->ucMap[ OutImgIndex(ucX,ucY) ] = 1;
		//}
	//}
	
}
*/
/*------------------------------------------------------------------------------
�o�͏��X�V
------------------------------------------------------------------------------*/
void UpdateOutInfo(UCHAR trackingType)
{
	UCHAR	i, ucX, ucY;
	if (trackingType == OBJ_TRACKING){
		ObjectTracking_GetOutput(
			&sHDI.sOutInfo.ucHumanNum,		/* Output: �l?�o?					*/
			sHDI.sOutInfo.ucMap,				/* Output: �l���݃}�b�v				*/
			sHDI.sOutInfo.ucLabelNum,
			sHDI.sOutInfo.sCenter,
			//sHDI.sOutInfo.ucNotHumanFrame,
			//sHDI.sOutInfo.ucBlinkCount,
			sHDI.sOutInfo.cLogDir,
			sHDI.sOutInfo.usFNMV,
			/* 20161118 - howardjhli - add more info for detect alarm */
			sHDI.sOutInfo.usArea,
			sHDI.sOutInfo.sRectangle,
			//sHDI.sOutInfo.sRect_Prev
			sHDI.sOutInfo.usFrame,
			sHDI.sOutInfo.bInnerLine,
			sHDI.sOutInfo.bOutterLine,
			sHDI.sOutInfo.ucMoveStatus
			);

		ObjectTracking_GetALarmInfoOutput(
			&sHDAI.sObjectOutInfo.ucHumanNum,
			sHDAI.sObjectOutInfo.ucMap,
			sHDAI.sObjectOutInfo.ucLabelNum,
			sHDAI.sObjectOutInfo.sCenter,
			//sHDAI.sObjectOutInfo.ucNotHumanFrame,
			//sHDAI.sObjectOutInfo.ucBlinkCount,
			sHDAI.sObjectOutInfo.cLogDir,
			sHDAI.sObjectOutInfo.usFNMV,
			/* 20161118 - howardjhli - add more info for detect alarm */
			sHDAI.sObjectOutInfo.usArea,
			sHDAI.sObjectOutInfo.sRectangle,
			//sHDAI.sObjectOutInfo.sRect_Prev
			sHDAI.sObjectOutInfo.usFrame,
			sHDAI.sObjectOutInfo.bInnerLine,
			sHDAI.sObjectOutInfo.bOutterLine,
			sHDAI.sObjectOutInfo.ucMoveStatus
			);
	}
	if (trackingType == DIFF_TRACKING){
		ObjectTracking_GetOutput(
			&sHDI.sOutDiffInfo.ucHumanNum,		/* Output: �l?�o?					*/
			sHDI.sOutDiffInfo.ucMap,				/* Output: �l���݃}�b�v				*/
			sHDI.sOutDiffInfo.ucLabelNum,
			sHDI.sOutDiffInfo.sCenter,
			//sHDI.sOutInfo.ucNotHumanFrame,
			//sHDI.sOutInfo.ucBlinkCount,
			sHDI.sOutDiffInfo.cLogDir,
			sHDI.sOutDiffInfo.usFNMV,
			/* 20161118 - howardjhli - add more info for detect alarm */
			sHDI.sOutDiffInfo.usArea,
			sHDI.sOutDiffInfo.sRectangle,
			//sHDI.sOutInfo.sRect_Prev
			sHDI.sOutDiffInfo.usFrame,
			sHDI.sOutDiffInfo.bInnerLine,
			sHDI.sOutDiffInfo.bOutterLine,
			sHDI.sOutDiffInfo.ucMoveStatus
			);

	}
}

BOOLEAN CheckHumanInBed(Feature* psFeature){
	USHORT usLeftBoardValue = g_sConfigParameters.sAlarmParameters.ucBedLeftBoardValue - 1;
	USHORT usRightBoardValue = g_sConfigParameters.sAlarmParameters.ucBedRightBoardValue + 1;

	for (int i = 0; i < OT_TRACKING_MAX; i++){
		if (psFeature[i].ucLabelNum == 0)
			continue;


		if ((psFeature[i].sG.fx < (float)(usLeftBoardValue)) &&
			(psFeature[i].sG.fx >(float)(usRightBoardValue)))
		{
			return TRUE;
		}

	}

	return FALSE;
}
/*******************************************************************************
 * Copyright(C) by 2014 Panasonic Corporation.
 ******************************************************************************/
