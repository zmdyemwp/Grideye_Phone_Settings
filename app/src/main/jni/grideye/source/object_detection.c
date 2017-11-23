/*******************************************************************************
 * Copyright(C) by 2014 Panasonic Corporation.
 ******************************************************************************/

/*******************************************************************************
	�C?�N?�[�h�t�@�C?
*******************************************************************************/
//#include	"stdafx.h"
#include <string.h>
#include "../header/common.h"
#include "../header/human_detection.h"

/*******************************************************************************
	�}�N?��`
*******************************************************************************/

#define		OD_IMG_SZ_MAX		(15*15)

#define		OD_NORMALLIST_SIZE	(10   )
#define		OD_NORMALLIST_BASE	(20.0f)
#define		OD_NORMALLIST_RANGE	( 1.0f)
#define		OD_DETECTLIST_SIZE	(10   )
#define		OD_DETECTLIST_BASE	(20.0f)
#define		OD_DETECTLIST_RANGE	( 1.0f)

#define		OD_IVD_INITIAL		(0)
#define		OD_IVD_MINIMUM		(0)
#define		OD_IVD_MAXIMUM		(8)
#define		OD_IVD_UP			(1)
#define		OD_IVD_DOWN			(1)

#define		OD_IVD2_INITIAL		(0)
#define		OD_IVD2_MINIMUM		(0)
#define		OD_IVD2_MAXIMUM		(9)
#define		OD_IVD2_UP			(1)
#define		OD_IVD2_DOWN		(1)

#define		OD_NOR_IVD_TH		(6)

#define		OD_LowDetectMargin	(0.22f)
#define		OD_LOW_IVD2_TH		(6)


#define		DETECT_FRAME		(6)

/*******************************************************************************
	��?�錾
*******************************************************************************/

const float c_fNormalAreaMargin[]

				= { 0.40, 0.40, 0.40, 0.40, 0.40, 0.40, 0.40, 0.40, 0.40, 0.40 };

const float c_fDetectAreaMargin[]

				= { 0.30, 0.30, 0.30, 0.30, 0.30, 0.30, 0.30, 0.30, 0.30, 0.30 };

/*******************************************************************************
	�\���̐錾
*******************************************************************************/

typedef struct _tagObjectDetectInfo {
	struct {
		UCHAR	ucImgX;
		UCHAR	ucImgY;
		USHORT	usImg;
	} Sz;
	UCHAR	ucIVD [OD_IMG_SZ_MAX];
	UCHAR	ucIVD2[OD_IMG_SZ_MAX];
	float	fBackTempAve;
	float	fBackTempAve_Prev;
} ObjectDetectInfo;

/*******************************************************************************
	��?�錾
*******************************************************************************/
ObjectDetectInfo	sODI;

//UCHAR ucDetectImgArray[DETECT_FRAME][IMG_SZ];
short sDiffTemp2Array[DETECT_FRAME][IMG_SZ];
/*******************************************************************************
	�v?�g�^�C�v�錾
*******************************************************************************/
void OD_CalcBackTempAve( short* );
void OD_MakeDiffTempImage( short*, short*, short* );
void OD_MakeDiffTempImage2(short*, short*, short*, short*);
void OD_MakeDetectImage( short*, UCHAR*, UCHAR* );
void OD_MakeDetectImage2(short*, UCHAR*, UCHAR*, float);
void OD_UpdateIVD( UCHAR* );
void OD_FilterBedDetectImg(UCHAR*, ConfigParameters*);
void OD_FilterOutBedDetectImg(UCHAR*, ConfigParameters*);
/*******************************************************************************
 * public?�\�b�h��`
 ******************************************************************************/

/*------------------------------------------------------------------------------
	����?�o ?����
------------------------------------------------------------------------------*/
BOOLEAN ObjectDetection_Initialize( UCHAR ucImgSzX, UCHAR ucImgSzY )
{
	USHORT	uImg;

	if( ucImgSzX * ucImgSzY > OD_IMG_SZ_MAX ){
		return FALSE;
	}

	sODI.Sz.ucImgX = ucImgSzX;
	sODI.Sz.ucImgY = ucImgSzY;
	sODI.Sz.usImg  = (USHORT)( ucImgSzX * ucImgSzY );
	 
	for( uImg = 0; uImg < OD_IMG_SZ_MAX; uImg++ ){
		sODI.ucIVD [uImg] = OD_IVD_INITIAL;
		sODI.ucIVD2[uImg] = OD_IVD2_INITIAL;
	}

	/* 20170216 - For track diff temp */
	for (int i = 0; i < DETECT_FRAME; i++){
		//ClearArray(ucDetectImgArray[i], IMG_SZ, '0');
		for (uImg = 0; uImg < IMG_SZ; uImg++){
			//ucDetectImgArray[i][uImg] = 0;
			sDiffTemp2Array[i][uImg] = 0;
		}
	}

	return TRUE;
}

/*------------------------------------------------------------------------------
	����?�o ?�C???
------------------------------------------------------------------------------*/
void ObjectDetection_forDLL(short* pshAveTemp, short* pshBackTemp, short* pshDiffTemp, short* pshDiffTemp2, 
	UCHAR* pucDetectImg, UCHAR* pucDetectImg2, ConfigParameters* psCp)
{

	OD_CalcBackTempAve( pshBackTemp );

	// for diff temp img
	//OD_MakeDiffTempImage2( pshAveTemp, pshBackTemp, pshDiffTemp, pshDiffTemp2 );

	

	OD_MakeDiffTempImage( pshAveTemp, pshBackTemp, pshDiffTemp );


	OD_MakeDetectImage(pshDiffTemp, pucDetectImg, pucDetectImg2);


	//OD_FilterBedDetectImg(pucDetectImg, psCp);
	//OD_FilterOutBedDetectImg(pucDetectImg2, psCp);

	// for diff temp detect img
	//OD_MakeDetectImage2(pshDiffTemp2, pucDetectImg, pucDetectImg2, psCp->sDetectParameters.fDiffTempThresh);

}


void ObjectDetection(short* pshAveTemp, short* pshBackTemp, short* pshDiffTemp, UCHAR* pucDetectImg)
{

	OD_CalcBackTempAve(pshBackTemp);

	OD_MakeDiffTempImage(pshAveTemp, pshBackTemp, pshDiffTemp);

	//OD_MakeDetectImage(pshDiffTemp, pucDetectImg);

}

/*******************************************************************************
 * private?�\�b�h��`
 ******************************************************************************/
void OD_FilterBedDetectImg(UCHAR* pucDetectImg, ConfigParameters* psCp){
	USHORT uImg;
	for (uImg = 0; uImg < sODI.Sz.usImg; uImg++){
		if (((uImg % sODI.Sz.ucImgX) < psCp->sAlarmParameters.ucBedLeftBoardValue) &&
			((uImg % sODI.Sz.ucImgX) > psCp->sAlarmParameters.ucBedRightBoardValue)){
			pucDetectImg[uImg] = 0;
		}
	}
}
void OD_FilterOutBedDetectImg(UCHAR* pucDetectImg, ConfigParameters* psCp){
	USHORT uImg;
	for (uImg = 0; uImg < sODI.Sz.usImg; uImg++){
		if (((uImg % sODI.Sz.ucImgX) > psCp->sAlarmParameters.ucBedLeftBoardValue) ||
			((uImg % sODI.Sz.ucImgX) < psCp->sAlarmParameters.ucBedRightBoardValue)){
			pucDetectImg[uImg] = 0;
		}
	}
}

/*------------------------------------------------------------------------------
	�ʏ�G?�A�̕���?�o?�l�擾
------------------------------------------------------------------------------*/
float OD_GetMarginOfNormalArea( float fTemp )
{
	int nIndex = (int)( (fTemp - OD_NORMALLIST_BASE) / OD_NORMALLIST_RANGE );

	if( nIndex < 0 ){
		nIndex = 0;
	}

	if( nIndex > OD_NORMALLIST_SIZE - 1 ){
		nIndex = OD_NORMALLIST_SIZE - 1;
	}

	return( c_fNormalAreaMargin[nIndex] );
}

/*------------------------------------------------------------------------------
	�O?�o�G?�A�̕���?�o?�l�擾
------------------------------------------------------------------------------*/
float OD_GetMarginOfPreDetectArea( float fTemp )
{
	int nIndex = (int)( (fTemp - OD_DETECTLIST_BASE) / OD_DETECTLIST_RANGE );

	if( nIndex < 0 ){
		nIndex = 0;
	}

	if( nIndex > OD_DETECTLIST_SIZE - 1 ){
		nIndex = OD_DETECTLIST_SIZE - 1;
	}

	return( c_fDetectAreaMargin[nIndex] );
}

/*------------------------------------------------------------------------------
	�w�i���x�̑S��f���ϒl�v�Z
------------------------------------------------------------------------------*/
void OD_CalcBackTempAve( short* pshBackTemp )
{
	USHORT	uImg;
	float	fBackTempAve;

	fBackTempAve = 0;
	for( uImg = 0; uImg < sODI.Sz.usImg; uImg++ ){
		fBackTempAve += TempStoF( pshBackTemp[uImg] );
	}
	fBackTempAve /= sODI.Sz.usImg;

	sODI.fBackTempAve_Prev = sODI.fBackTempAve;
	sODI.fBackTempAve      = fBackTempAve;
}

/*------------------------------------------------------------------------------
	�w�i�������x��?����
------------------------------------------------------------------------------*/
void OD_MakeDiffTempImage( short* pshAveTemp, short* pshBackTemp, short* pshDiffTemp )
{
	USHORT	uImg;

	for( uImg = 0; uImg < sODI.Sz.usImg; uImg++ ){
		pshDiffTemp[uImg] = (short)( pshAveTemp[uImg] - pshBackTemp[uImg] );
	}
}

/* 20170216 - For track diff temp */
/*------------------------------------------------------------------------------
OD_MakeDiffTempImage2
------------------------------------------------------------------------------*/
void OD_MakeDiffTempImage2(short* pshAveTemp, short* pshBackTemp, short* pshDiffTemp, short* pshDiffTemp2)
{
	USHORT	uImg;
	//short[] shDiffTemp;
	//short[] shPreDiffTemp;
	//ClearArray(shDiffTemp, 225, 0);
	//ClearArray(shPreDiffTemp, 225, 0);


	for (int i = DETECT_FRAME - 1; i > 0; i--){
		memcpy(sDiffTemp2Array[i], sDiffTemp2Array[i - 1], sODI.Sz.usImg*sizeof(short));
	}

	for (uImg = 0; uImg < sODI.Sz.usImg; uImg++){
		pshDiffTemp2[uImg] = 0;
		sDiffTemp2Array[0][uImg] = (short)(((pshAveTemp[uImg] - pshBackTemp[uImg]) - pshDiffTemp[uImg]) * 3);
	}


	for (int i = 0; i < DETECT_FRAME; i++){
		for (uImg = 0; uImg < sODI.Sz.usImg; uImg++){

			if (sDiffTemp2Array[i][uImg] > pshDiffTemp2[uImg])
				pshDiffTemp2[uImg] = sDiffTemp2Array[i][uImg];

		}
	}



	/*
	for (uImg = 0; uImg < sODI.Sz.usImg; uImg++){
	//shPreDiffTemp[uImg] = pshDiffTemp[uImg];
	//shDiffTemp[uImg] = (short)(pshAveTemp[uImg] - pshBackTemp[uImg]);

	//pshDiffTemp2[uImg] = (short)(shDiffTemp[uImg] - shPreDiffTemp[uImg]);

	pshDiffTemp2[uImg] = (short)(((pshAveTemp[uImg] - pshBackTemp[uImg]) - pshDiffTemp[uImg])*3);
	}
	*/
}

/* 20170216 - For track diff temp */
/*------------------------------------------------------------------------------
OD_MakeDiffTempImage2
------------------------------------------------------------------------------*/
void OD_MakeDetectImage2(short* pshDiffTemp2, UCHAR* pucDetectImg, UCHAR* pucDetectImg2, float pfDiffThresh)
{
	USHORT	uImg;
	UCHAR   ucDetectImg[IMG_SZ];

	/*
	for (uImg = 0; uImg < sODI.Sz.usImg; uImg++){
		ucDetectImg[uImg] = 0;
	}
	

	
	float	fBackTempAve = sODI.fBackTempAve;
	short	shNormalThresh = TempFtoS(OD_GetMarginOfNormalArea(fBackTempAve));
	short	shDetectThresh = TempFtoS(OD_GetMarginOfPreDetectArea(fBackTempAve));


	for (uImg = 0; uImg < sODI.Sz.usImg; uImg++){
		short	shThreshold;

		//shThreshold = shNormalThresh;
		shThreshold = TempFtoS(pfDiffThresh);
		//shThreshold = TempFtoS(pfDiffThresh) < shNormalThresh ? shNormalThresh : TempFtoS(pfDiffThresh);
		ucDetectImg[uImg] = (pshDiffTemp2[uImg] > shThreshold) ? 0x01 : 0;
	}
	




	for (int i = DETECT_FRAME - 1; i > 0; i--){
		memcpy(ucDetectImgArray[i], ucDetectImgArray[i - 1], sODI.Sz.usImg*sizeof(UCHAR));
	}

	for (uImg = 0; uImg < sODI.Sz.usImg; uImg++){
		pucDetectImg2[uImg] = 0;
		ucDetectImgArray[0][uImg] = ucDetectImg[uImg];
	}


	for (int i = 0; i < DETECT_FRAME; i++){
		for (uImg = 0; uImg < sODI.Sz.usImg; uImg++){

			if (ucDetectImgArray[i][uImg] != 0)
				pucDetectImg2[uImg] = ucDetectImgArray[i][uImg];

		}
	}

	for (uImg = 0; uImg < sODI.Sz.usImg; uImg++){
		pucDetectImg2[uImg] = pucDetectImg2[uImg] & pucDetectImg[uImg];
	}
	*/

	/* 20170221 */
	short	shThreshold;
	shThreshold = TempFtoS(pfDiffThresh);
	for (uImg = 0; uImg < sODI.Sz.usImg; uImg++){
		pucDetectImg2[uImg] = (pshDiffTemp2[uImg] > shThreshold) ? 0xFF : 0;
	}
}

/*------------------------------------------------------------------------------
	����?�o��?����
------------------------------------------------------------------------------*/
void OD_MakeDetectImage(short* pshDiffTemp, UCHAR* pucDetectImg, UCHAR* pucDetectImg2)
{
	USHORT	uImg;

	float	fBackTempAve	= sODI.fBackTempAve;
	short	shNormalThresh	= TempFtoS( OD_GetMarginOfNormalArea   ( fBackTempAve ) );
	short	shDetectThresh	= TempFtoS( OD_GetMarginOfPreDetectArea( fBackTempAve ) );

	float	fLowThresh = OD_LowDetectMargin;
	short	shLowThresh;
	float	fBackTempAveDiff = sODI.fBackTempAve - sODI.fBackTempAve_Prev;

	if( fBackTempAveDiff < 0 ){

		fLowThresh -= fBackTempAveDiff;
	}
	shLowThresh = TempFtoS( fLowThresh );

	for( uImg = 0; uImg < sODI.Sz.usImg; uImg++ ){
		UCHAR	ucIVD;
		int		nIVD2;
		BOOLEAN	bPreDetect;
		short	shThreshold;
		int		nLowDetectFrame;

		ucIVD = sODI.ucIVD[uImg];

		bPreDetect = ( ucIVD >= OD_NOR_IVD_TH );

		shThreshold = (bPreDetect == TRUE) ? shDetectThresh : shNormalThresh;

		pucDetectImg[uImg] = (pshDiffTemp[uImg] >= shThreshold) ? 0xFF : 0;


		nIVD2 = sODI.ucIVD2[uImg];

		if( (pshDiffTemp[uImg] >= shThreshold) || (pshDiffTemp[uImg] >= shLowThresh) ){

			nIVD2 = MIN( nIVD2 + OD_IVD2_UP, OD_IVD2_MAXIMUM );
		}
		else {

			nIVD2 = MAX( nIVD2 - OD_IVD2_DOWN, OD_IVD2_MINIMUM );
			if( pshDiffTemp[uImg] <= 0 ){
				nIVD2 = MAX( nIVD2 - OD_IVD2_DOWN, OD_IVD2_MINIMUM );
			}
		}

		sODI.ucIVD2[uImg] = (UCHAR)nIVD2;

		nLowDetectFrame = OD_LOW_IVD2_TH;

		if( nIVD2 >= nLowDetectFrame ){

			pucDetectImg[uImg] = 0xFF;
			pucDetectImg2[uImg] = 0xFF;
		}

	}

	OD_UpdateIVD( pucDetectImg );
}



/*------------------------------------------------------------------------------
	����?�o?��?�[��?�X�V
------------------------------------------------------------------------------*/
void OD_UpdateIVD( UCHAR* pucDetectImg )
{
	USHORT	uImg;

	for( uImg = 0; uImg < sODI.Sz.usImg; uImg++ ){

		int nIVD = sODI.ucIVD[uImg];

		if( pucDetectImg[uImg] != 0 ){
			nIVD = MIN( nIVD + OD_IVD_UP,   OD_IVD_MAXIMUM );
		}else{
			nIVD = MAX( nIVD - OD_IVD_DOWN, OD_IVD_MINIMUM );
		}

		sODI.ucIVD[uImg] = (UCHAR)nIVD;
	}
}

/*******************************************************************************
 * Copyright(C) by 2014 Panasonic Corporation.
 ******************************************************************************/
