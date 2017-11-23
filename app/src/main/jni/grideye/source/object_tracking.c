/*******************************************************************************
 * Copyright(C) by 2014 Panasonic Corporation.
 ******************************************************************************/
//#include	"stdafx.h"
/*******************************************************************************
	�C?�N?�[�h�t�@�C?
*******************************************************************************/
#include "../header/common.h"
#include "../header/human_detection.h"
#include "math.h"
#include	<string.h>

#include "../header/object_tracking.h"


/*******************************************************************************
 define
*******************************************************************************/


/*******************************************************************************
	��?�錾
*******************************************************************************/
ObjectTrackingInfo		sOTI;
TrackingControlInfo		sTCI;
TrackingControlInfo*	psTCI;
TrackingOutputInfo	sOutInfoA;


/* for object tracking */
ObjectTrackingInfo		sOTI_Obj;
TrackingControlInfo		sTCI_Obj;
TrackingControlInfo*	psTCI_Obj;
TrackingOutputInfo	sOutInfoA_Obj;

/* for diff temp tracking */
ObjectTrackingInfo		sOTI_Diff;
TrackingControlInfo		sTCI_Diff;
TrackingControlInfo*	psTCI_Diff;
TrackingOutputInfo	sOutInfoA_Diff;



/*******************************************************************************
	�O?�Q�ƃv?�g�^�C�v�錾
*******************************************************************************/
extern float sqrtf( float );
extern ConfigParameters g_sConfigParameters;
/*******************************************************************************
 * public?�\�b�h��`
 ******************************************************************************/


/*------------------------------------------------------------------------------
	���̒ǐ� ?����
------------------------------------------------------------------------------*/
BOOLEAN ObjectTracking_Initialize( UCHAR ucImgSzX, UCHAR ucImgSzY, UCHAR ucOutSzX, UCHAR ucOutSzY )
{
	UCHAR	ucTN, ucSN;

	if( ucImgSzX * ucImgSzY > OT_IMG_SZ_MAX ){
		return FALSE;
	}
	if( ucOutSzX * ucOutSzY > OT_OUT_SZ_MAX ){
		return FALSE;
	}

	sOTI.Sz.ucImgX = ucImgSzX;
	sOTI.Sz.ucImgY = ucImgSzY;
	sOTI.Sz.usImg  = (SHORT)( ucImgSzX * ucImgSzY );

	sOTI.Sz.ucOutX = ucOutSzX;
	sOTI.Sz.ucOutY = ucOutSzY;
	sOTI.Sz.ucOut  = ucOutSzX * ucOutSzY;

	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){
		ClearTrackingFeature( &sOTI.sTF[ucTN] );
	}

	for( ucSN = 0; ucSN < OT_STORED_MAX; ucSN++ ){
		ClearStoredFeature( &sOTI.sSF[ucSN] );
	}

	ClearTrackingOutputInfo( &sOTI.sOutInfo );

	ClearTrackingOutputInfo( &sOutInfoA );

	psTCI = &sTCI;

	ClearTrackingControlInfo( psTCI );


	/* clear sOTI_Obj */
	sOTI_Obj.Sz.ucImgX = ucImgSzX;
	sOTI_Obj.Sz.ucImgY = ucImgSzY;
	sOTI_Obj.Sz.usImg = (SHORT)(ucImgSzX * ucImgSzY);

	sOTI_Obj.Sz.ucOutX = ucOutSzX;
	sOTI_Obj.Sz.ucOutY = ucOutSzY;
	sOTI_Obj.Sz.ucOut = ucOutSzX * ucOutSzY;

	for (ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++){
		ClearTrackingFeature(&sOTI_Obj.sTF[ucTN]);
	}

	for (ucSN = 0; ucSN < OT_STORED_MAX; ucSN++){
		ClearStoredFeature(&sOTI_Obj.sSF[ucSN]);
	}

	ClearTrackingOutputInfo(&sOTI_Obj.sOutInfo);

	ClearTrackingOutputInfo(&sOutInfoA_Obj);

	/* clear sOTI_Diff */
	sOTI_Diff.Sz.ucImgX = ucImgSzX;
	sOTI_Diff.Sz.ucImgY = ucImgSzY;
	sOTI_Diff.Sz.usImg = (SHORT)(ucImgSzX * ucImgSzY);

	sOTI_Diff.Sz.ucOutX = ucOutSzX;
	sOTI_Diff.Sz.ucOutY = ucOutSzY;
	sOTI_Diff.Sz.ucOut = ucOutSzX * ucOutSzY;

	for (ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++){
		ClearTrackingFeature(&sOTI_Diff.sTF[ucTN]);
	}

	for (ucSN = 0; ucSN < OT_STORED_MAX; ucSN++){
		ClearStoredFeature(&sOTI_Diff.sSF[ucSN]);
	}

	ClearTrackingOutputInfo(&sOTI_Diff.sOutInfo);

	ClearTrackingOutputInfo(&sOutInfoA_Diff);




	return TRUE;
}


void RestoreObjectTrackingIfo(UCHAR trackingType){
	if (trackingType == OBJ_TRACKING){
		sOTI = sOTI_Obj;
		sTCI = sTCI_Obj;
		//*psTCI = *psTCI_Obj;
		sOutInfoA = sOutInfoA_Obj;
	}
	if (trackingType == DIFF_TRACKING){
		sOTI = sOTI_Diff;
		sTCI = sTCI_Diff;
		//*psTCI = *psTCI_Diff;
		sOutInfoA = sOutInfoA_Diff;
	}
}
void SaveObjectTrackingIfo(UCHAR trackingType){
	if (trackingType == OBJ_TRACKING) {
		sOTI_Obj = sOTI;
		sTCI_Obj = sTCI;
		//*psTCI_Obj = *psTCI;
		sOutInfoA_Obj = sOutInfoA;
	}
	if (trackingType == DIFF_TRACKING) {
		sOTI_Diff = sOTI;
		sTCI_Diff = sTCI;
		//*psTCI_Diff = *psTCI;
		sOutInfoA_Diff = sOutInfoA;
	}
}

/*------------------------------------------------------------------------------
	���̒ǐ� ?�C???
------------------------------------------------------------------------------*/
void ObjectTracking( UCHAR ucDetectNum, Feature* psFeature)
{

	ClearTrackingControlInfo( psTCI );

	psTCI->ucDetectNum	= MIN( ucDetectNum, OT_DETECT_MAX );    

	psTCI->psDF			= psFeature;

    /********** ��꒣ frame �i�� ************/
	if( TRUE == LinkForNoTrackingObject() ){

		UpdateTrackingOutInfo( &sOTI.sOutInfo );
		return;
	}
    /******************************************/
    

    // ��񒣔V��Iframes
	DetectApart();

	CheckStoredObject();

	DetectUnion();

	CalculateDistance();

	LinkObject();

	DeleteNotUsedObjectNearAnothorObject();

	UpdateTrackingOutInfo( &sOTI.sOutInfo );
	
	UpdateHumanDetectAlarmInfo(&sOutInfoA);
}



/*------------------------------------------------------------------------------
	�w�i��??
------------------------------------------------------------------------------*/
void ConvertNotHumanAreaToBackground( UCHAR* pucDetectImg, short* pshAveTemp, short* pshBackTemp, UCHAR* pucWork, short* pshHumanTemp )
{
	USHORT	uImg;
	UCHAR	ucImgX	= sOTI.Sz.ucImgX;
	UCHAR	ucImgY	= sOTI.Sz.ucImgY;
	USHORT	usImg	= sOTI.Sz.usImg;
	UCHAR	ucX, ucY;
	UCHAR	ucTN;
	float	fBackAveTemp = 0;

    // �w�i?�xshort to float �����`
	for( uImg = 0; uImg < usImg; uImg++ ){
		fBackAveTemp += TempStoF( pshBackTemp[uImg] );
	}
	fBackAveTemp /= usImg;  // �Z�o����?�x
	
	//for( uImg = 0; uImg < usImg; uImg++ ){
		//pshHumanTemp[uImg] = TempFtoS(26.0f);
	//}

	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){

		TrackingFeature* psTF = &sOTI.sTF[ucTN];

		if( psTF->ucLabelNum == 0 ){    // Label��0 �\��?�LDetect?objectt 
			continue;
		}

		if( FALSE == JudgeNotHuman( psTF, fBackAveTemp ) ){ // �@�ʐ�Human���s��
			/*
			if( psTF->bHumanValid == TRUE){
				for( uImg = 0; uImg < usImg; uImg++ ){
					if ( pucDetectImg[uImg] == psTF->ucLabelNum ) {
						pshHumanTemp[uImg] = TempFtoS(28.0f);
					}
				}
			}
			*/
			continue;
		}
		
        // �c��?�M���I����W 1 
		for( uImg = 0; uImg < usImg; uImg++ ){
			pucWork[uImg] = ( pucDetectImg[uImg] == psTF->ucLabelNum ) ? 1 : 0;
		}

		for( uImg = 0; uImg < usImg; uImg++ ){
            // �s�� 1 �I?���s��?
			if( pucWork[uImg] & 0x01 == 0 ){
				continue;
			}
            
            // �c�L�M���I?����܎��ȓI�㉺���E�l�����Ipixel�s| 2
			ucX = uImg % ucImgX;
			ucY = uImg / ucImgX;
			pucWork[uImg] |= 0x02;
			if( ucX != 0        ){ pucWork[uImg-1     ] |= 0x02; }
			if( ucX != ucImgX-1 ){ pucWork[uImg+1     ] |= 0x02; }
			if( ucY != 0        ){ pucWork[uImg-ucImgX] |= 0x02; }
			if( ucY != ucImgY-1 ){ pucWork[uImg+ucImgX] |= 0x02; }
		}

        
		for( uImg = 0; uImg < usImg; uImg++ ){
            // �s�� 2 �I�n���s��?
			if( pucWork[uImg] & 0x02 == 0 ){
				continue;
			}
            // �c�L�M���I?����܎��ȓI�㉺���E�l�����Ipixel�s |4
			ucX = uImg % ucImgX;
			ucY = uImg / ucImgX;
			pucWork[uImg] |= 0x04;
			if( ucX != 0        ){ pucWork[uImg-1     ] |= 0x04; }
			if( ucX != ucImgX-1 ){ pucWork[uImg+1     ] |= 0x04; }
			if( ucY != 0        ){ pucWork[uImg-ucImgX] |= 0x04; }
			if( ucY != ucImgY-1 ){ pucWork[uImg+ucImgX] |= 0x04; }
		}

		for( uImg = 0; uImg < usImg; uImg++ ){
			if( pucWork[uImg] != 0 ){
				pshBackTemp[uImg] = pshAveTemp[uImg];
			}
		}
	}
	 
}

/*------------------------------------------------------------------------------
	�o��
------------------------------------------------------------------------------*/
void ObjectTracking_GetOutput( UCHAR* pucHumanNum, UCHAR* pucMap, UCHAR* puclabelNum, Coordinate* psCenter, 
	CHAR* pcLogDir, USHORT* pusFNMV, USHORT* pusArea, Rect* psRectangle, USHORT* pusFrame,
	BOOLEAN* pbInnerLine, BOOLEAN* pbOutterLine, UCHAR* pucMoveStatus)
{
	*pucHumanNum =                    sOTI.sOutInfo.sOut.ucHumanNum;
	CopyArray( pucMap, sOTI.Sz.ucOut, sOTI.sOutInfo.sOut.ucMap );
	CopyArray( puclabelNum, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.ucLabelNum );
	CopyArray( psCenter, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.sCenter );
		
	//CopyArray( pucNotHumanFrame, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.ucNotHumanFrame );	
	//CopyArray( pucBlinkCount, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.ucBlinkCount );	
	
	CopyArray( pusFNMV, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.usFNMV );	
	//CopyArray( pcLogDir, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.cLogDir );
	
	memcpy(pcLogDir, sOTI.sOutInfo.sOut.cLogDir, OT_TRACKING_MAX*OT_LOGSIZE_D*sizeof(CHAR));
	
	/* 20161118 - howardjhli - add more info for detect alarm */
	//memcpy(pusLogArea, sOTI.sOutInfo.sOut.usLogArea, OT_TRACKING_MAX*OT_LOGSIZE_G*sizeof(USHORT));
	CopyArray( pusArea, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.usArea );	
	CopyArray( psRectangle, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.sRectangle );	
	//CopyArray( psRectangle_Prev, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.sRectangle_Prev );

	CopyArray(pusFrame, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.usFrame);


	CopyArray(pbInnerLine, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.bInnerLine);
	CopyArray(pbOutterLine, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.bOutterLine);
	CopyArray(pucMoveStatus, OT_TRACKING_MAX, sOTI.sOutInfo.sOut.ucMoveStatus);
}
/*------------------------------------------------------------------------------
	�o��
------------------------------------------------------------------------------*/
void ObjectTracking_GetALarmInfoOutput( UCHAR* pucHumanNum, UCHAR* pucMap, UCHAR* puclabelNum, Coordinate* psCenter, 
	CHAR* pcLogDir, USHORT* pusFNMV, USHORT* pusArea, Rect* psRectangle, USHORT* pusFrame,
	BOOLEAN* pbInnerLine, BOOLEAN* pbOutterLine, UCHAR* pucMoveStatus)
{
	*pucHumanNum =                    sOutInfoA.sOut.ucHumanNum;
	CopyArray( pucMap, sOTI.Sz.ucOut, sOutInfoA.sOut.ucMap );
	CopyArray( puclabelNum, OT_TRACKING_MAX, sOutInfoA.sOut.ucLabelNum );
	CopyArray( psCenter, OT_TRACKING_MAX, sOutInfoA.sOut.sCenter );

	//CopyArray( pucNotHumanFrame, OT_TRACKING_MAX, sOutInfoA.sOut.ucNotHumanFrame );	
	//CopyArray( pucBlinkCount, OT_TRACKING_MAX, sOutInfoA.sOut.ucBlinkCount );	
	
	CopyArray( pusFNMV, OT_TRACKING_MAX, sOutInfoA.sOut.usFNMV );	
	//CopyArray( pcLogDir, OT_TRACKING_MAX, sOutInfoA.sOut.cLogDir );
	
	memcpy(pcLogDir, sOutInfoA.sOut.cLogDir, OT_TRACKING_MAX*OT_LOGSIZE_D*sizeof(CHAR));


	/* 20161118 - howardjhli - add more info for detect alarm */
	CopyArray( pusArea, OT_TRACKING_MAX, sOutInfoA.sOut.usArea );	
	//memcpy(pusLogArea, sOutInfoA.sOut.usLogArea, OT_TRACKING_MAX*OT_LOGSIZE_G*sizeof(USHORT));
	CopyArray( psRectangle, OT_TRACKING_MAX, sOutInfoA.sOut.sRectangle );
	//CopyArray( psRectangle_Prev, OT_TRACKING_MAX, sOutInfoA.sOut.sRectangle_Prev );
	
	//for (int i=0; i<OT_TRACKING_MAX; i++ ){
		//memcpy(pcLogDir[i], sOutInfoA.sOut.cLogDir[i], sizeof(sOutInfoA.sOut.cLogDir[0]));
	//}
	
	//for(int i = 0; i < OT_TRACKING_MAX; i++ ){
		//for(int j = 0; j < OT_LOGSIZE_D; j++ ){
			//pcLogDir[i][j] = sOutInfoA.sOut.cLogDir[i][j];
		//}
	//}

	CopyArray(pusFrame, OT_TRACKING_MAX, sOutInfoA.sOut.usFrame);


	CopyArray(pbInnerLine, OT_TRACKING_MAX, sOutInfoA.sOut.bInnerLine);
	CopyArray(pbOutterLine, OT_TRACKING_MAX, sOutInfoA.sOut.bOutterLine);
	CopyArray(pucMoveStatus, OT_TRACKING_MAX, sOutInfoA.sOut.ucMoveStatus);
}
/*******************************************************************************
 * private?�\�b�h��`
 ******************************************************************************/

/*------------------------------------------------------------------------------
	���W ?����
------------------------------------------------------------------------------*/
void ClearCoordinate( Coordinate* psC )
{
	psC->fx = OT_INVALID_C;
	psC->fy = OT_INVALID_C;
}

/*------------------------------------------------------------------------------
	?���v�Z
------------------------------------------------------------------------------*/
float CalcDistance( Coordinate sC1, Coordinate sC2 )
{
	return sqrtf( POW( sC1.fx - sC2.fx ) + POW( sC1.fy - sC2.fy ) );
}

/*------------------------------------------------------------------------------
	�ړ������v�Z
------------------------------------------------------------------------------*/
CHAR CalcDirection( Coordinate sC1, Coordinate sC2 )
{
	float fdx = sC1.fx - sC2.fx;
	float fdy = sC1.fy - sC2.fy;

	CHAR cDir = OT_INVALID_DIR;

	if( MAX( ABS(fdx), ABS(fdy) ) < 0.01 ){ // �ӎw�ړ�?�����Z �c�쐥"��@�I�ړ�" 
		cDir = OT_INVALID_DIR;
	}else if( fdx > 0 ){
		if( fdy > 0 ){
			cDir = (  fdx >  fdy ) ? 0 : 1;
		}else{
			cDir = (  fdx > -fdy ) ? 7 : 6;
		}
	}else{
		if( fdy > 0 ){
			cDir = ( -fdx >  fdy ) ? 3 : 2;
		}else{
			cDir = ( -fdx > -fdy ) ? 4 : 5;
		}
	}

	return( cDir );
}

/*------------------------------------------------------------------------------
	�ړ����������v�Z
------------------------------------------------------------------------------*/
UCHAR DiffDirection( CHAR cDir1, CHAR cDir2 )
{
	UCHAR ucDiff = ABS( cDir1 - cDir2 );

	if( ( 4 < ucDiff ) && ( ucDiff < 8 ) ){
		ucDiff = 8 - ucDiff;
	}

	return( ucDiff );
}

/*------------------------------------------------------------------------------
	��` ?����
------------------------------------------------------------------------------*/
void ClearRectangle( Rect* psR )
{
	psR->ucMinX = 0;
	psR->ucMaxX = 0;
	psR->ucMinY = 0;
	psR->ucMaxY = 0;
}

/*------------------------------------------------------------------------------
	��` �����v�Z
------------------------------------------------------------------------------*/
void DiffRectangle( Rect sR1, Rect sR2, Rect* psDiff )
{
	psDiff->ucMinX = ABS( sR1.ucMinX - sR2.ucMinX );
	psDiff->ucMinY = ABS( sR1.ucMinY - sR2.ucMinY );
	psDiff->ucMaxX = ABS( sR1.ucMaxX - sR2.ucMaxX );
	psDiff->ucMaxY = ABS( sR1.ucMaxY - sR2.ucMaxY );
}

/*------------------------------------------------------------------------------
	��` ?�v�l�v�Z
------------------------------------------------------------------------------*/
int SumRectangle( Rect sR )
{
	return ( sR.ucMinX + sR.ucMinY + sR.ucMaxX + sR.ucMaxY );
}

/*------------------------------------------------------------------------------
	��` �����
------------------------------------------------------------------------------*/
BOOLEAN IsConnotationRectangle( Rect sR1, Rect sR2, UCHAR ucThresh )
{

	if( sR1.ucMinX + ucThresh < sR2.ucMinX ){
		return FALSE;
	}
	if( sR1.ucMinY + ucThresh < sR2.ucMinY ){
		return FALSE;
	}
	if( sR2.ucMaxX + ucThresh < sR1.ucMaxX ){
		return FALSE;
	}
	if( sR2.ucMaxY + ucThresh < sR1.ucMaxY ){
		return FALSE;
	}

	return TRUE;
}

/*------------------------------------------------------------------------------
	�O�ڋ�`���߂����̂��ǂ����̔���
------------------------------------------------------------------------------*/
BOOLEAN IsNearRectangle( Rect sR1, Rect sR2, UCHAR ucThresh )
{
	UCHAR ucDifferencePoint = 0;

	Rect sDiff;
	DiffRectangle( sR1, sR2, &sDiff );

	ucDifferencePoint += (sDiff.ucMinX > 3) ? 5 : (sDiff.ucMinX == 3) ? 2 : (sDiff.ucMinX == 2) ? 1 : 0;
	ucDifferencePoint += (sDiff.ucMinY > 3) ? 5 : (sDiff.ucMinY == 3) ? 2 : (sDiff.ucMinY == 2) ? 1 : 0;
	ucDifferencePoint += (sDiff.ucMaxX > 3) ? 5 : (sDiff.ucMaxX == 3) ? 2 : (sDiff.ucMaxX == 2) ? 1 : 0;
	ucDifferencePoint += (sDiff.ucMaxY > 3) ? 5 : (sDiff.ucMaxY == 3) ? 2 : (sDiff.ucMaxY == 2) ? 1 : 0;

	if( ucDifferencePoint <= ucThresh ){
		return TRUE;
	}

	return FALSE;
}

/*------------------------------------------------------------------------------
	���W�ʒu���߂����ǂ����̔���
------------------------------------------------------------------------------*/
BOOLEAN IsNearCoordinate( Coordinate sC1, Coordinate sC2, float fThresh )
{
	float fdx = ABS( sC1.fx - sC2.fx );
	float fdy = ABS( sC1.fy - sC2.fy );

	if( MAX( fdx, fdy ) <= fThresh ){
		return TRUE;
	}

	return FALSE;
}

/*------------------------------------------------------------------------------
	�ʐϓ������߂����̂��ǂ����̔���
------------------------------------------------------------------------------*/
BOOLEAN IsNearArea( USHORT usArea1, USHORT usArea2, float fDiffThresh, float fRateThresh )
{
	float fDiff = ABS( usArea1 - usArea2 );
	float fRate = (float)usArea1 / (float)usArea2;

	if( fDiff <= fDiffThresh ){
		return TRUE;
	}

	if( (fRate < fRateThresh) && (fRate > 1 / fRateThresh) ){
		return TRUE;
	}

	return FALSE;
}

/*------------------------------------------------------------------------------
	�O����f����
------------------------------------------------------------------------------*/
BOOLEAN IsOutside( Coordinate sC, int nWidthMask, int nHeightMask )
{
	if( sC.fx < nWidthMask || sC.fx > sOTI.Sz.ucImgX - nWidthMask - 1 ){
		return TRUE;
	}
	if( sC.fy < nHeightMask || sC.fy > sOTI.Sz.ucImgY - nHeightMask - 1 ){
		return TRUE;
	}

	return FALSE;
}

/*------------------------------------------------------------------------------
	�ǐՐ����� ?����
------------------------------------------------------------------------------*/
void ClearTrackingControlInfo( TrackingControlInfo* psTCI )
{
	UCHAR	ucTN, ucSN, ucDN;

	psTCI->ucDetectNum	= 0;
	psTCI->psDF			= 0;

	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){
		psTCI->bLinkedTF                  [ucTN] = FALSE;
		psTCI->fMinDistance_From_Tracked  [ucTN] = OT_INV_DISTANCE;
		psTCI->cMinDistanceNo_From_Tracked[ucTN] = -1;
	}

	for( ucSN = 0; ucSN < OT_STORED_MAX; ucSN++ ){
		psTCI->bAliveOfStored[ucSN] = TRUE;
	}

	for( ucDN = 0; ucDN < OT_DETECT_MAX; ucDN++ ){
		psTCI->bLinkedDF              [ucDN] = FALSE;
		psTCI->fMinDistance_From_Now  [ucDN] = OT_INV_DISTANCE;
		psTCI->cMinDistanceNo_From_Now[ucDN] = -1;
	}
}

/*------------------------------------------------------------------------------
	�ǐՓ����� ?����
------------------------------------------------------------------------------*/
void ClearTrackingFeature( TrackingFeature* psTF )
{
	Coordinate sG;
	ClearCoordinate( &sG );

	psTF->ucLabelNum		= 0;
	psTF->usFrame			= 0;
	psTF->usDetectFrame		= 0;
	psTF->ucNoDetectFrame	= 0;
	psTF->ucBlinkCount		= 0;
	ClearRectangle( &psTF->sRectangle );
	ClearRectangle( &psTF->sRectangle_Prev );
	psTF->fTMaxs			= 0;
	psTF->fTMaxl			= 0;
	psTF->sG_Half[0]		= sG;
	psTF->sG_Half[1]		= sG;
	psTF->sGl				= sG;
	psTF->fV				= 0;
	psTF->ucPM	 			= 0;
	psTF->usFNMV 			= 0;
	psTF->nHumanScore		= 0;
	psTF->bHumanValid		= FALSE;
	psTF->usNotHumanFrame	= 0;
	psTF->ucPartnerPM		= 0;
	psTF->usPartnerNum		= 0;

	psTF->bInnerLine = FALSE;
	psTF->bOutterLine = FALSE;
	psTF->ucMoveStatus = MOVE_NA;

	ClearArray( psTF->sLogG,     OT_LOGSIZE_G, sG );
	ClearArray( psTF->usLogArea, OT_LOGSIZE_G, 0 );
	ClearArray( psTF->cLogDir,   OT_LOGSIZE_D, OT_INVALID_DIR );
	ClearArray( psTF->fLogTMax,  OT_LOGSIZE_T, 0 );
	ClearArray( psTF->fLogTMaxl, OT_LOGSIZE_T_L, 0 );
}

/*------------------------------------------------------------------------------
	�ۑ��ǐՓ����� ?����
------------------------------------------------------------------------------*/
void ClearStoredFeature( StoredFeature* psSF )
{
	Coordinate sG;
	ClearCoordinate( &sG );

	psSF->usSaveFrame		= 0;
	psSF->usFrame			= 0;
	psSF->ucBlinkCount		= 0;
	ClearRectangle( &psSF->sRectangle );
	psSF->fTMaxs			= 0;
	psSF->fTMaxl			= 0;
	psSF->sG				= sG;
	psSF->ucPM	 			= 0;
	psSF->usFNMV 			= 0;
	psSF->nHumanScore		= 0;
	psSF->bHumanValid		= FALSE;
	psSF->usNotHumanFrame	= 0;

	psSF->bInnerLine = FALSE;
	psSF->bOutterLine = FALSE;
	psSF->ucMoveStatus = MOVE_NA;


	ClearArray( psSF->cLogDir,   OT_LOGSIZE_D, OT_INVALID_DIR );
	ClearArray( psSF->fLogTMax,  OT_LOGSIZE_T, 0 );
	ClearArray( psSF->fLogTMaxl, OT_LOGSIZE_T_L, 0 );
}

/*------------------------------------------------------------------------------
	�ǐՓ����ʂ̕ۑ�
------------------------------------------------------------------------------*/
void SaveStoredFeature( TrackingFeature* psTF )
{
	UCHAR	ucSN;
	StoredFeature* psSF;

	if( psTF->usFrame <= OT_UNION_STORED_F ){
		return;
	}

	if( psTF->usLogArea[0] <= OT_UNION_STORED_A ){
		return;
	}

	for( ucSN = 0; ucSN < OT_STORED_MAX; ucSN++ ){
		psSF = &sOTI.sSF[ucSN];
		if( psSF->usSaveFrame == 0 ){
			break;
		}
	}
	if( ucSN == OT_STORED_MAX ){
		return;
	}

	psSF->usSaveFrame		= 1;

	psSF->usFrame			= psTF->usFrame;
	psSF->ucBlinkCount		= psTF->ucBlinkCount;

	psSF->sRectangle		= psTF->sRectangle;

	psSF->fTMaxs			= psTF->fTMaxs;
	psSF->fTMaxl			= psTF->fTMaxl;

	psSF->sG				= psTF->sLogG[0];

	psSF->ucPM				= psTF->ucPM;
	psSF->usFNMV			= psTF->usFNMV;
	psSF->nHumanScore		= psTF->nHumanScore;
	psSF->bHumanValid		= psTF->bHumanValid;
	psSF->usNotHumanFrame	= psTF->usNotHumanFrame;

	psSF->bInnerLine = psTF->bInnerLine;
	psSF->bOutterLine = psTF->bOutterLine;
	psSF->ucMoveStatus = psTF->ucMoveStatus;

	CopyArray( psSF->cLogDir,   OT_LOGSIZE_D,   psTF->cLogDir   );
	CopyArray( psSF->fLogTMax,  OT_LOGSIZE_T,   psTF->fLogTMax  );
	CopyArray( psSF->fLogTMaxl, OT_LOGSIZE_T_L, psTF->fLogTMaxl );
}

/*------------------------------------------------------------------------------
	�ǐՓ����ʂ�?�A
------------------------------------------------------------------------------*/
void RestoreStoredFeature( StoredFeature* psSF, TrackingFeature* psTF )
{

	psTF->usFrame			= psSF->usFrame;
	psTF->ucBlinkCount		= psSF->ucBlinkCount;

	psTF->sRectangle		= psSF->sRectangle;

	psTF->fTMaxs			= psSF->fTMaxs;
	psTF->fTMaxl			= psSF->fTMaxl;

	psTF->ucPM				= psSF->ucPM;
	psTF->usFNMV			= psSF->usFNMV;
	psTF->nHumanScore		= psSF->nHumanScore;
	psTF->bHumanValid		= psSF->bHumanValid;
	psTF->usNotHumanFrame	= psSF->usNotHumanFrame;

	psTF->bInnerLine = psSF->bInnerLine;
	psTF->bOutterLine = psSF->bOutterLine;
	psTF->ucMoveStatus = psSF->ucMoveStatus;

	CopyArray( psTF->cLogDir,   OT_LOGSIZE_D,   psSF->cLogDir   );
	CopyArray( psTF->fLogTMax,  OT_LOGSIZE_T,   psSF->fLogTMax  );
	CopyArray( psTF->fLogTMaxl, OT_LOGSIZE_T_L, psSF->fLogTMaxl );
}

/*------------------------------------------------------------------------------
	����?�o�Ə��?��
    �ڑO�擾�I��?�o�ݒ��F?�I��?�i�s?��
    
    
    ***************** �K�{�_�㑪��?�ȏ�˘�??��? ApartProc

    
------------------------------------------------------------------------------*/
void DetectApart( void )
{
	UCHAR	ucTN, ucDN;

	UCHAR	ucIncNum;
	UCHAR	ucIncList[OT_INCLUDE_MAX];

	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){ // ���F�I��?

		TrackingFeature* psTF = &sOTI.sTF[ucTN];    // �擾��?1~5�ItrackingFeature?��

        // ��Q�o�s�׋�ITrackingFeature
        
		if( TRUE == IsLinkedTrackingFeature(ucTN) ){    // usFrame == 0 => TRUE, bBlink == TRUE => TRUE
			continue;
		}

		if( psTF->ucNoDetectFrame > 0 ){    // ?���F��?�ڑO����?�㑪?��?�Iframe
			continue;
		}

		ucIncNum = 0;
		ClearArray( ucIncList, OT_INCLUDE_MAX, 0 );

		for( ucDN = 0; ucDN < psTCI->ucDetectNum; ucDN++ ){  // �ڑO�㑪��?

			Feature* psDF = &psTCI->psDF[ucDN];

			UCHAR	ucTN2;

			if( TRUE == IsLinkedFeature(ucDN) ){    // ���ۗL�������F���C�߉�����continue
				continue;
			}

            // ��?�㑪��?�a���F��?�Ԑ��ې����`�I��?  ���I�b�\���׈��?
			if( FALSE == IsConnotationRectangle( psDF->sRectangle, psTF->sRectangle, OT_APART_TH_INC ) ){
				continue;
			}

            
			for( ucTN2 = 0; ucTN2 < OT_TRACKING_MAX; ucTN2++ ){

				if( TRUE == PreLinkObject( ucTN2, ucDN ) ){
					break;
				}
			}

            
			if( ucTN2 == OT_TRACKING_MAX ){ // �s?�LpreLink����
				if( ucIncNum < OT_INCLUDE_MAX ){
					ucIncList[ucIncNum] = ucDN;
					ucIncNum++;
				}
			}
		}

		if( TRUE == IsLinkedTrackingFeature(ucTN) ){
			continue;
		}

        // �L��?�_��?�C�s�a TrackingFeature[ucTN] Rect?��
		if( ucIncNum < 2 ){ 
			continue;
		}

		ApartProc( ucTN, ucIncNum, ucIncList );
	}
}

/*------------------------------------------------------------------------------
	����??
    ucIncTN : 
    ucIncList  
    
        ��v���ח������_�ȏ㕨???
        
------------------------------------------------------------------------------*/
void ApartProc( UCHAR ucIncTN, UCHAR ucIncNum, UCHAR* ucIncList )
{
	TrackingFeature*	psTF = &sOTI.sTF[0];
	Feature*			psDF = psTCI->psDF;

	UCHAR	i;
	UCHAR	ucTN, ucDN, ucSN;

	BOOLEAN	bRestored;
	UCHAR	ucRestCount;
	UCHAR	ucRestDN;
	UCHAR	ucRestTN;

	ucRestCount = ucIncNum;

	for( i = 0; i < ucIncNum; i++ ){

		ucDN = ucIncList[i];

		ucTN = SearchNewIndex();    // new index of TrackingFeature
		if( ucTN == OT_TRACKING_MAX ){
			return;
		}

		UpdateFeatureAndScore( ucTN, ucDN );

		bRestored = FALSE;

		/* check stored objectTracking */
		for( ucSN = 0; ucSN < OT_STORED_MAX; ucSN++ ){
			/* 1. Is this saved objectFeature has frame? */
			if( sOTI.sSF[ucSN].usSaveFrame == 0 ){
				continue;
			}

			/* 2. if new apart object(human) near saved objectFeature */
			if( FALSE == IsNearRectangle( psDF[ucDN].sRectangle, sOTI.sSF[ucSN].sRectangle, OT_RESTORE_THRESH_R ) ){
				continue;
			}
			if( FALSE == IsNearCoordinate( sOTI.sSF[ucSN].sG, psDF[ucDN].sG, OT_RESTORE_THRESH_G ) ){
				continue;
			}

			/* 3. restore saved objectFeature to aparted object */
			RestoreStoredFeature( &sOTI.sSF[ucSN], &psTF[ucTN] );

			ClearArray( psTF[ucTN].usLogArea, OT_LOGSIZE_G, psDF[ucDN].usArea );
			ClearArray( psTF[ucTN].sLogG,     OT_LOGSIZE_G, psDF[ucDN].sG     );

			bRestored = TRUE;
			psTCI->bAliveOfStored[ucSN] = FALSE;
		}
		if( bRestored == TRUE ){
			ucRestCount--;
			continue;
		}


		psTF[ucTN].usPartnerNum = (psTF[ucIncTN].usFrame % 10) * 100 + ucIncTN + 1;

		psTF[ucTN].ucPartnerPM = psTF[ucIncTN].ucPM;

		if( TRUE == IsNearCoordinate( psTF[ucIncTN].sLogG[0], psDF[ucDN].sG, OT_APART_PM_THRESH ) ){
			psTF[ucTN].ucPM = psTF[ucIncTN].ucPM;
		}else{
			psTF[ucTN].ucPM = MIN( psTF[ucIncTN].ucPM, OT_APART_PM_MAX );
		}

		psTF[ucTN].nHumanScore = psTF[ucIncTN].nHumanScore;
		psTF[ucTN].bHumanValid = psTF[ucIncTN].bHumanValid;


		/* 20170307 - for move status */
		psTF[ucTN].bInnerLine = psTF[ucIncTN].bInnerLine;
		psTF[ucTN].bOutterLine = psTF[ucIncTN].bOutterLine;
		psTF[ucTN].ucMoveStatus = psTF[ucIncTN].ucMoveStatus;


		ucRestDN = ucDN;
		ucRestTN = ucTN;

	}

	if( ucRestCount == 1 ){

		UpdateFeatureAndScore( ucIncTN, ucRestDN );

		ClearTrackingFeature( &psTF[ucRestTN] );
	}else{

		ClearTrackingFeature( &psTF[ucIncTN] );
	}
}

/*------------------------------------------------------------------------------
	�ۑ��ǐՕ��̂̊�?
------------------------------------------------------------------------------*/
void CheckStoredObject( void )
{
	UCHAR	ucDN, ucSN;

	for( ucSN = 0; ucSN < OT_STORED_MAX; ucSN++ ){

		StoredFeature* psSF = &sOTI.sSF[ucSN];

		if( psSF->usSaveFrame == 0 ){
			continue;
		}

		if( psTCI->bAliveOfStored[ucSN] == FALSE ){
			ClearStoredFeature( psSF );
			continue;
		}

		if( psSF->usSaveFrame > OT_STORED_THRESH_F ){
			ClearStoredFeature( psSF );
			continue;
		}

		for( ucDN = 0; ucDN < psTCI->ucDetectNum; ucDN++ ){

			Feature* psDF = &psTCI->psDF[ucDN];

			if( TRUE == IsConnotationRectangle( psSF->sRectangle, psDF->sRectangle, OT_STORED_THRESH_R ) ){
				break;
			}
		}

		if( ucDN == psTCI->ucDetectNum ){
			ClearStoredFeature( psSF );
			continue;
		}

		psSF->usSaveFrame++;
	}
}

/*------------------------------------------------------------------------------
	?��?�o
------------------------------------------------------------------------------*/
void DetectUnion( void )
{
	UCHAR	ucTN, ucDN;

	UCHAR	ucIncNum;
	UCHAR	ucIncList[OT_INCLUDE_MAX];

	for( ucDN = 0; ucDN < psTCI->ucDetectNum; ucDN++ ){

		Feature* psDF = &psTCI->psDF[ucDN];

		if( TRUE == IsLinkedFeature(ucDN) ){
			continue;
		}

		ucIncNum = 0;
		ClearArray( ucIncList, OT_INCLUDE_MAX, 0 );

		for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){

			TrackingFeature* psTF = &sOTI.sTF[ucTN];

			UCHAR	ucDN2;

			if( TRUE == IsLinkedTrackingFeature(ucTN) ){
				continue;
			}

			if( psTF->ucNoDetectFrame > 0 ){
				continue;
			}

			if( FALSE == IsConnotationRectangle( psTF->sRectangle, psDF->sRectangle, OT_UNION_TH_INC ) ){
				continue;
			}

			for( ucDN2 = 0; ucDN2 < psTCI->ucDetectNum; ucDN2++ ){

				if( TRUE == PreLinkObject( ucTN, ucDN2 ) ){
					break;
				}
			}

			if( ucDN2 == psTCI->ucDetectNum ){
				if( ucIncNum < OT_INCLUDE_MAX ){
					ucIncList[ucIncNum] = ucTN;
					ucIncNum++;
				}
			}
		}

		if( TRUE == IsLinkedFeature(ucDN) ){
			continue;
		}

		if( ucIncNum < 2 ){
			continue;
		}

		UnionProc( ucDN, ucIncNum, ucIncList );
	}
}

/*------------------------------------------------------------------------------
	?��??
------------------------------------------------------------------------------*/
void UnionProc( UCHAR ucDN, UCHAR ucIncNum, UCHAR* ucIncList )
{
	TrackingFeature*	psLinkTF;
	Feature*			psDF = &psTCI->psDF[ucDN];

	UCHAR	i;

	UCHAR	ucLinkTN;

	{
		float	fUnionPoint;
		float	fUnionMaxValue = -1;

		// find the TreakingFeature which have biggest fUnionPoint(ucPM + humanscore + fv)
		for( i = 0; i < ucIncNum; i++ ){

			UCHAR				ucTN = ucIncList[i];
			TrackingFeature*	psTF = &sOTI.sTF[ucTN];

			fUnionPoint  = psTF->ucPM        * OT_UNION_PM_RATE;
			fUnionPoint += psTF->nHumanScore * OT_UNION_HS_RATE;

			if( psTF[ucTN].fV >= OT_UNION_THRESH_V ){
				fUnionPoint += psTF->fV * OT_UNION_V_RATE;
			}

			if( fUnionMaxValue < fUnionPoint ){
				fUnionMaxValue = fUnionPoint;
				ucLinkTN = ucTN;
			}
		}
	}
	psLinkTF = &sOTI.sTF[ucLinkTN];

	SaveStoredFeature( psLinkTF );

	if( FALSE == IsNearCoordinate( psLinkTF->sLogG[1], psDF->sG, OT_UNION_THRESH_G_L ) ){
		ClearArray( psLinkTF->sLogG, OT_LOGSIZE_G, psDF->sG );
	}else{
		psLinkTF->sLogG[0]	= psDF->sG;
		psLinkTF->sLogG[1]	= psDF->sG;
	}

	psLinkTF->sG_Half[0]	= psDF->sG;
	psLinkTF->sG_Half[1]	= psDF->sG;
	psLinkTF->sGl			= psDF->sG;

	ClearArray( psLinkTF->usLogArea, OT_LOGSIZE_G, psDF->usArea );

	UpdateFeatureAndScore( ucLinkTN, ucDN );

	for( i = 0; i < ucIncNum; i++ ){

		UCHAR				ucTN = ucIncList[i];
		TrackingFeature*	psTF = &sOTI.sTF[ucTN];

		if( ucTN == ucLinkTN ){
			continue;
		}

		SaveStoredFeature( psTF );

		if( psTF->usFrame <= OT_UNION_THRESH_F ){
			if( psLinkTF->usPartnerNum == psTF->usPartnerNum ){
				psLinkTF->ucPM = MAX( psLinkTF->ucPM, psLinkTF->ucPartnerPM );
			}
		}

		ClearTrackingFeature( psTF );
	}

	psLinkTF->usPartnerNum = 0;
	psLinkTF->ucPartnerPM  = 0;
}

/*------------------------------------------------------------------------------
	�d�S��?���v�Z
------------------------------------------------------------------------------*/
void CalculateDistance( void )
{
	UCHAR	ucTN, ucDN;

	Coordinate	sGT;	// current sG[0] + offset
	Coordinate	sGD;	// 
	float		fD;

	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){

		TrackingFeature* psTF = &sOTI.sTF[ucTN];

		if( TRUE == IsLinkedTrackingFeature(ucTN) ){
			continue;
		}

		sGT = psTF->sLogG[0];

		if( sOTI.sTF[ucTN].usFrame > 1 ){
			float fOffsetX = psTF->sLogG[0].fx - psTF->sLogG[1].fx;
			float fOffsetY = psTF->sLogG[0].fy - psTF->sLogG[1].fy;
			fOffsetX = ( ABS(fOffsetX) < OT_CALC_DISTANCE_D ) ? 0 : ( fOffsetX * OT_CALC_DISTANCE_R );
			fOffsetY = ( ABS(fOffsetY) < OT_CALC_DISTANCE_D ) ? 0 : ( fOffsetY * OT_CALC_DISTANCE_R );
			sGT.fx += fOffsetX;
			sGT.fy += fOffsetY;
		}

		for( ucDN = 0; ucDN < psTCI->ucDetectNum; ucDN++ ){

			Feature* psDF = &psTCI->psDF[ucDN];

			if( TRUE == IsLinkedFeature(ucDN) ){
				continue;
			}

			sGD = psDF->sG;

			//fD =  ( sGD, sGT );
			fD = sqrtf(POW(sGD.fx - sGT.fx) + POW(sGD.fy - sGT.fy));
			
			

			if( psTCI->fMinDistance_From_Now  [ucDN] > fD ){
				psTCI->fMinDistance_From_Now  [ucDN] = fD;
				psTCI->cMinDistanceNo_From_Now[ucDN] = ucTN;
			}

			if( psTCI->fMinDistance_From_Tracked  [ucTN] > fD ){
				psTCI->fMinDistance_From_Tracked  [ucTN] = fD;
				psTCI->cMinDistanceNo_From_Tracked[ucTN] = ucDN;
			}
		}
	}
}

/*------------------------------------------------------------------------------
	�ǐՕ��̂��Ȃ���?��??�N
    ���ۗL����Tracking�I��?
------------------------------------------------------------------------------*/
BOOLEAN LinkForNoTrackingObject( void )
{
	UCHAR	ucTN, ucDN;

	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){
		if( sOTI.sTF[ucTN].usFrame != 0 ){
			break;
		}
	}
    
    // �L usFrame �s??0 �� ucTN != OT_TRACKING_MAX => return FALSE
    // �S? usFrame ??0 �� ucTN == OT_TRACKING_MAX => ??���ʓI���� => return TRUE
	if( ucTN != OT_TRACKING_MAX ){ 
		return FALSE;
	}

	for( ucDN = 0; ucDN < psTCI->ucDetectNum; ucDN++ ){
		if( ucDN == OT_TRACKING_MAX ){
			break;
		}

		UpdateFeatureAndScore( ucDN, ucDN );
	}
	return TRUE;
}

/*------------------------------------------------------------------------------
	?�o���̂ƒǐՕ��̂�?�O??�N
    ucTN : Tracking Feature number
    ucDN : Feature number
    
    ��LINK TrackingFeature�oFeature
    
------------------------------------------------------------------------------*/
BOOLEAN PreLinkObject( UCHAR ucTN, UCHAR ucDN )
{
	TrackingFeature*	psTF = &sOTI.sTF[ucTN];
	Feature*			psDF = &psTCI->psDF[ucDN];

	if( TRUE == IsLinkedTrackingFeature(ucTN) ){
		return FALSE;
	}

	if( TRUE == IsLinkedFeature(ucDN) ){
		return FALSE;
	}

	if( FALSE == IsNearCoordinate( psTF->sLogG[0], psDF->sG, OT_PRELINK_TH_G ) ){
		return FALSE;
	}

	if( FALSE == IsNearArea( psTF->usLogArea[0], psDF->usArea, OT_PRELINK_TH_A_D, OT_PRELINK_TH_A_R ) ){
		return FALSE;
	}

	UpdateFeatureAndScore( ucTN, ucDN );

	return TRUE;
}

/*------------------------------------------------------------------------------
	?�o���̂ƒǐՕ��̂�??�N
------------------------------------------------------------------------------*/
void LinkObject( void )
{
	UCHAR	ucTN, ucDN;

	// first find TF
	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){

		BOOLEAN bLink = FALSE;

		if( TRUE == IsLinkedTrackingFeature(ucTN) ){
			continue;
		}

		if( psTCI->fMinDistance_From_Tracked[ucTN] <= OT_LINK_THRESH_D ){   // OT_LINK_THRESH_D (4.0f)

			ucDN = psTCI->cMinDistanceNo_From_Tracked[ucTN];

			if( FALSE == IsLinkedFeature(ucDN) ){

				if( ucTN == psTCI->cMinDistanceNo_From_Now[ucDN] ){

					bLink = TRUE;
				}
			}
		}

		if( bLink == TRUE ){

			UpdateFeatureAndScore( ucTN, ucDN );
		}else{

			IncrementFrameNum( &sOTI.sTF[ucTN] );
		}
	}

	// second find Object
	for( ucDN = 0; ucDN < psTCI->ucDetectNum; ucDN++ ){

		if( TRUE == IsLinkedFeature(ucDN) ){    // �㑪�I��?�Җ�Update Feature�� (��?����F) => FALSE
			continue;
		}

		ucTN = SearchNewIndex();
		if( ucTN == OT_TRACKING_MAX ){
			return;
		}

		UpdateFeatureAndScore( ucTN, ucDN );
	}
}

/*------------------------------------------------------------------------------
	�O��??�N����̃t?�[??�̃C?�N???�g
	�i���݂̃t?�[?��??�N���ꂽ�I�u�W�F�N�g��������Ȃ������Ƃ��Ɏ��s�j
------------------------------------------------------------------------------*/
void IncrementFrameNum( TrackingFeature* psTF )
{

	if( psTF->usDetectFrame <= OT_BLINK_COUNT ){   // OT_BLINK_COUNT (30)
		psTF->ucBlinkCount++;
	}

	psTF->ucNoDetectFrame++;
	psTF->usDetectFrame	= 0;
	psTF->ucLabelNum	= 0;
	psTF->fV			= 0;
	psTF->cLogDir[0]	= OT_INVALID_DIR;

	if( psTF->ucNoDetectFrame >= MIN( psTF->usFrame, OT_MAXLINKFRAME ) ){   // OT_MAXLINKFRAME (20)
		ClearTrackingFeature( psTF );
	}
}

/*------------------------------------------------------------------------------
	����?�o����Ă��Ȃ����̂̂����A�ǐՑΏۂɂȂ��Ă��镨�̂�?�����߂����̂���?����
    
    �Q�o ucNoDetectFrame != 0 �I TrackingFeature psTF
    ��?���a�������F?�ITrackingFeature���ۏd�S?����? OT_NEAR_ANOTHER_D
    ��:����??��TrackingFeature psTF
------------------------------------------------------------------------------*/
void DeleteNotUsedObjectNearAnothorObject( void )
{
	UCHAR	ucTN, ucTN2;

	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){

		TrackingFeature* psTF = &sOTI.sTF[ucTN];

		if( psTF->ucNoDetectFrame == 0 ){
			continue;
		}

		for( ucTN2 = 0; ucTN2 < OT_TRACKING_MAX; ucTN2++ ){

			TrackingFeature* psTF2 = &sOTI.sTF[ucTN2];

			float fDistance;
			if( psTF2->ucNoDetectFrame != 0 ){
				continue;
			}
			if( psTF2->usFrame == 0 ){
				continue;
			}

			fDistance = CalcDistance( psTF->sLogG[0], psTF2->sLogG[0] );
			if( fDistance < OT_NEAR_ANOTHER_D ){
				ClearTrackingFeature( psTF );
				break;
			}
		}
	}
}

/*------------------------------------------------------------------------------
	�ǐՕ��̂Ƃ��Ė��g�p��Index�̃T�[�`
------------------------------------------------------------------------------*/
UCHAR SearchNewIndex( void )
{
	UCHAR	ucTN;

	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){
		if( sOTI.sTF[ucTN].usFrame == 0 ){
			break;
		}
	}

	return ucTN;
}

/*------------------------------------------------------------------------------
	�ǐՕ��̂ɂ���??�N�ς݂�����
------------------------------------------------------------------------------*/
BOOLEAN IsLinkedTrackingFeature( UCHAR ucTN )
{
	if( sOTI.sTF[ucTN].usFrame == 0 ){
		return TRUE;
	}
	if( psTCI->bLinkedTF[ucTN] == TRUE ){
		return TRUE;
	}

	return FALSE;
}

/*------------------------------------------------------------------------------
	?�o���̂ɂ���??�N�ς݂�����
------------------------------------------------------------------------------*/
BOOLEAN IsLinkedFeature( UCHAR ucDN )
{
	if( psTCI->bLinkedDF[ucDN] == TRUE ){
		return TRUE;
	}

	return FALSE;
}

/*------------------------------------------------------------------------------
	�ǐՓ����ʋy�ѐl���蓾�_�̍X�V
------------------------------------------------------------------------------*/
void UpdateFeatureAndScore( UCHAR ucTN, UCHAR ucDN )
{
	TrackingFeature*	psTF = &sOTI.sTF[ucTN];
	Feature*			psDF = &psTCI->psDF[ucDN];

	UpdateTrackingFeature( psTF, psDF );

	UpdateMoveDirection( psTF );

	UpdateConMove( psTF );
	UpdateMovePoint( psTF );

	UpdateHumanScore( psTF );

	psTCI->bLinkedTF[ucTN] = TRUE;
	psTCI->bLinkedDF[ucDN] = TRUE;

	UpdateMoveLineStatus(psTF);
}

/*------------------------------------------------------------------------------
	�ǐՓ����ʂ̍X�V
------------------------------------------------------------------------------*/
void UpdateTrackingFeature( TrackingFeature* psTF, Feature* psDF )
{
	float	fMaxTempDiff;

	psTF->usFrame			= MIN( psTF->usFrame       + 1, OT_FRAMECOUNT_MAX );  // MAX=30000
	psTF->usDetectFrame		= MIN( psTF->usDetectFrame + 1, OT_FRAMECOUNT_MAX );  
	psTF->ucNoDetectFrame	= 0;

	if( psTF->usDetectFrame == OT_BLINK_RESET ){    // OT_BLINK_RESET = 200
		psTF->ucBlinkCount = 0;
	}

	psTF->ucLabelNum = psDF->ucLabelNum;

    
    // ��`���F��??
    // �O����F��??
	if( psTF->usFrame > 1 ){    // ��@���F��??�I��?�ߗL����frame (�s��?�i�����F)
		psTF->sRectangle_Prev = psTF->sRectangle;
	}else{
		psTF->sRectangle_Prev = psDF->sRectangle;
	}
	psTF->sRectangle = psDF->sRectangle;

	UpdateLog( psTF->usLogArea, OT_LOGSIZE_G, psDF->usArea );
    
	UpdateLog( psTF->sLogG,     OT_LOGSIZE_G, psDF->sG     );

    
    // �d�S���F��??
	if( psTF->usFrame > 1 ){
		psTF->sG_Half[1]	= psTF->sG_Half[0];
		psTF->sG_Half[0].fx	= psTF->sG_Half[0].fx * (1-OT_G_HALF ) + psDF->sG.fx * OT_G_HALF;
		psTF->sG_Half[0].fy	= psTF->sG_Half[0].fy * (1-OT_G_HALF ) + psDF->sG.fy * OT_G_HALF;
		psTF->sGl.fx		= psTF->sGl.fx        * (1-OT_G_ALPHA) + psDF->sG.fx * OT_G_ALPHA;
		psTF->sGl.fy		= psTF->sGl.fy        * (1-OT_G_ALPHA) + psDF->sG.fy * OT_G_ALPHA;
	}else{  
		psTF->sG_Half[1]	= psDF->sG;
		psTF->sG_Half[0]	= psDF->sG;
		psTF->sGl			= psDF->sG;
	}

	fMaxTempDiff = TempStoF( psDF->shMaxDiffTemp );
	if( psTF->usFrame > 1 ){
		psTF->fTMaxs = psTF->fTMaxs * (1- OT_T_MAXS) + fMaxTempDiff * OT_T_MAXS;
		psTF->fTMaxl = psTF->fTMaxl * (1- OT_T_MAXL) + fMaxTempDiff * OT_T_MAXL;
	}else{
		psTF->fTMaxs = fMaxTempDiff;
		psTF->fTMaxl = fMaxTempDiff;
	}

	UpdateLog( psTF->fLogTMax, OT_LOGSIZE_T, fMaxTempDiff );

	if( psTF->usFrame % OT_LOGSPAN_T_L == 1 ){
		UpdateLog( psTF->fLogTMaxl, OT_LOGSIZE_T_L, psTF->fTMaxl );
	}
}

/*------------------------------------------------------------------------------
	�ړ������̍X�V
------------------------------------------------------------------------------*/
void UpdateMoveDirection( TrackingFeature* psTF )
{
	CHAR	cDir;

	if( psTF->usFrame <= 1 ){
		return;
	}

	cDir = CalcDirection( psTF->sG_Half[0], psTF->sG_Half[1] );

	UpdateLog( psTF->cLogDir, OT_LOGSIZE_D, cDir );
}
/*------------------------------------------------------------------------------
	Summary of LogG distance
------------------------------------------------------------------------------*/
float CalcLogGDistance( TrackingFeature* psTF )
{
	float fDeltaConMoveSum =  0.0f;
	if (psTF->usFrame < OT_LOGSIZE_G){
		return fDeltaConMoveSum;
	}

	for (int i=0; i<OT_LOGSIZE_G-1; i++){
		fDeltaConMoveSum += CalcDistance( psTF->sLogG[i], psTF->sLogG[i+1] );
	}
	
	return fDeltaConMoveSum;
}
/*------------------------------------------------------------------------------
	Average of LogArea
------------------------------------------------------------------------------*/
USHORT CalcLogAreaAve( TrackingFeature* psTF )
{
	USHORT usLogAreaSum = 0;
	
	if (psTF->usFrame < OT_LOGSIZE_G){
		return usLogAreaSum;
	}

	for (int i=0; i<OT_LOGSIZE_G; i++){
		usLogAreaSum += psTF->usLogArea[i];
	}
	
	return ( usLogAreaSum/OT_LOGSIZE_G );
}
/*------------------------------------------------------------------------------
	diff of max and min Area
------------------------------------------------------------------------------*/
USHORT CalcDiffArea( TrackingFeature* psTF )
{
	USHORT usMaxArea = 0;
	USHORT usMinArea = sOTI.Sz.ucImgX*sOTI.Sz.ucImgY;
	USHORT usDiffArea = 0;
	
	if (psTF->usFrame < OT_LOGSIZE_G){
		return usDiffArea;
	}
	
	for (int i=0; i<OT_LOGSIZE_G; i++){
		if (psTF->usLogArea[i] < usMinArea){
			usMinArea = psTF->usLogArea[i];
		}
		if (psTF->usLogArea[i] > usMaxArea){
			usMaxArea = psTF->usLogArea[i];
		}
	}
	
	if (usMaxArea > usMinArea)
		usDiffArea = usMaxArea - usMinArea;
	
	return usDiffArea;
}


/*------------------------------------------------------------------------------
	�A���ړ��ʂ̍X�V
------------------------------------------------------------------------------*/
void UpdateConMove( TrackingFeature* psTF )
{

	BOOLEAN bEdgeArea = FALSE;
	if( psTF->usFrame <= g_sConfigParameters.sTrackingParameters.ucEdgeThresh ){
		bEdgeArea = TRUE;
	}

	switch( JudgeMoveContinuity( psTF, bEdgeArea ) ){
		case MV_Continuous:
			{
				float fDeltaConMove = CalcDistance( psTF->sLogG[0], psTF->sLogG[1] );
				float fDeltaConMoveSum = CalcLogGDistance( psTF );
				USHORT usArea = CalcLogAreaAve( psTF );
				USHORT usDiffArea = CalcDiffArea( psTF );
				
				if( bEdgeArea == FALSE ){
					if (fDeltaConMoveSum > usArea*0.003){
						fDeltaConMove = 15.0f;
						psTF->fV += fDeltaConMove;
						return;
					}
					if (fDeltaConMove < 0.02f){
						if (usDiffArea > usArea*g_sConfigParameters.sTrackingParameters.fBA_CheckDiffArea){
							fDeltaConMove = 15.0f;
							} else {
							psTF->fV *= OT_CONMOV_SMV_NEDGE; // (*0.5f)  if object frame > EDGE_THRESH && object keep not move
							return;
						}
					} else {
						fDeltaConMove *= 5.0f;
						psTF->fV += fDeltaConMove;
						return;
					}
					/******************************************************/
					
					Rect sDiff;
					int nOuterBorderMove;
					DiffRectangle( psTF->sRectangle, psTF->sRectangle_Prev, &sDiff );
					nOuterBorderMove = SumRectangle( sDiff );
					if( nOuterBorderMove == 0 ){
						fDeltaConMove *= OT_CONMOV_MV_R0; // (*0.5f)
					}
					else if( nOuterBorderMove == 1 ){
						fDeltaConMove *= OT_CONMOV_MV_R1; // (*0.75f)
					}
				}

				//fDeltaConMove = MIN( fDeltaConMove, OT_CONMOV_MV_MAX ); // min(fDeltaConMove, 1.0f)
				fDeltaConMove = MIN( fDeltaConMove, 15.0f ); 
				if( bEdgeArea == TRUE ){

					//fDeltaConMove *= OT_CONMOV_MV_EDGE; // (*2.0f)
					fDeltaConMove *= 14.0f; 
				}
				psTF->fV += fDeltaConMove;
			}
			break;
		case MV_SubContinuous:
			if( bEdgeArea == FALSE ){
				psTF->fV *= OT_CONMOV_SMV_NEDGE; // (*0.5f)
			}
			else{
				//psTF->fV *= OT_CONMOV_SMV_EDGE; // (*1.1f)   if object frame < EDGE_THRESH && object keep not move
				psTF->fV *= 1.5f;
			}
			break;
		case MV_NotContinuous:
			//20170117
			psTF->fV = 0;
			break;
	}

	if( psTF->usFrame >= OT_LOGSIZE_G ){

		float	fAreaNew  = (float)psTF->usLogArea[0];
		float	fAreaOld  = (float)psTF->usLogArea[OT_LOGSIZE_G-1];
		float	fAreaRate = (fAreaNew < fAreaOld) ? (fAreaNew / fAreaOld) : (fAreaOld / fAreaNew);
		float	fGDiff10  = CalcDistance( psTF->sLogG[0], psTF->sLogG[OT_LOGSIZE_G-1] ) * fAreaRate;

		if( fGDiff10 > OT_G10PLUS ){
			psTF->fV += fGDiff10 - OT_G10PLUS;
		}
	}
}

/*------------------------------------------------------------------------------
	�ړ��A��������
------------------------------------------------------------------------------*/
ConMove JudgeMoveContinuity( TrackingFeature* psTF, BOOLEAN bEdgeArea )
{
	CHAR	cDir0	= psTF->cLogDir[0];
	CHAR	cDir1	= psTF->cLogDir[1];
	CHAR	cDirN	= psTF->cLogDir[OT_LOGSIZE_D-1];

	UCHAR	ucDiff1	= DiffDirection( cDir0, cDir1 ); // ��3���I�ړ������V����
	UCHAR	ucDiffN	= DiffDirection( cDir0, cDirN ); // ��OT_LOGSIZE_D-1(3)��frame�V�ԓI�ړ������V����

	if( psTF->usFrame < OT_CONMOV_FRAME ){ // frames�ɏ�?(3)���ɉߏ����@�p�Ҕ��ЁC���d��?NotContinuous�s�A?�ړ�
		return MV_NotContinuous;
	}

    // ��@�ߙ_���ړ������I���ُ�?2 
	if( ucDiff1 < 2 ){  
        // ��@�ߌܒ��ړ��������ُ�???2 �� edgeArea��TRUE �� ��ܒ�frame�ה�@
		if( (bEdgeArea == TRUE) || ( cDirN == OT_INVALID_DIR ) || (ucDiffN <= 2) ){
			return MV_Continuous;
		}else{
			return MV_SubContinuous;
		}
    // ��@�ߙ_���ړ���������??2 �� ��ꓮ�ה�@ �� ��񓮈ה�@
	}else if( (ucDiff1 == 2) || cDir0 == OT_INVALID_DIR || cDir1 == OT_INVALID_DIR ){
		return MV_SubContinuous;
    // �ߙ_���ړ��������ّ�?2�ȏ�
	}else{
		return MV_NotContinuous;
	}
}

/*------------------------------------------------------------------------------
	�A���ړ��ʂ̍X�V
------------------------------------------------------------------------------*/
void UpdateMovePoint( TrackingFeature* psTF )
{
	if( psTF->fV > OT_V_MOVE ){ // (3.5f)
		psTF->usFNMV      = 0;
		psTF->ucPM        = OT_PM_MOVE;
		psTF->bHumanValid = TRUE;

	}else if( psTF->fV > OT_V_SUBMOVE1 ){ // (2.5f)
		psTF->usFNMV      = 0;
		psTF->ucPM        = MAX( psTF->ucPM, OT_PM_SUBMOVE );

	}else if( psTF->fV > OT_V_SUBMOVE2 ){ // (1.5f)
		psTF->usFNMV      = 0;

	}else{
		psTF->usFNMV++;

		if (psTF->usFNMV > g_sConfigParameters.sTrackingParameters.ucFNMV_FrameMax){
			psTF->usFNMV = 0;
			psTF->ucPM   = 0;

			psTF->bHumanValid = FALSE;
		}
	}

	if( psTF->fV > OT_V_SUBMOVE1 ){

		psTF->ucBlinkCount = 0;
	}
}

/*------------------------------------------------------------------------------
	�l���蓾�_�̍X�V
------------------------------------------------------------------------------*/
void UpdateHumanScore( TrackingFeature* psTF )
{
	int nHS = psTF->nHumanScore;

	float fBase;

	if( psTF->usFrame == 1 ){   // ��J�nframe 1 ���I?��C��?���humanScroe??

		psTF->nHumanScore = OT_HS_INITIAL;  // OT_HS_INITIAL = 12000
		return;
	}

	if( psTF->ucPM == OT_PM_SUBMOVE ){  // OT_PM_SUBMOVE(2)
		nHS -= OT_HS_DEC_SUSPICIOUS; // OT_HS_DEC_SUSPICIOUS (3)
	}
	else if( psTF->ucPM == 0 ){
		nHS -= OT_HS_DECREACE; // OT_HS_DECREACE(15)
	}

	fBase = OT_HS_BASE_MOVE / psTF->fTMaxs;

	if( psTF->fV > fBase ){
		nHS += (int)((psTF->fV - fBase) * OT_HS_INC_BY_MOVE) + OT_HS_DECREACE * 2;
	}

	nHS = MIN( nHS, OT_HS_MAXIMUM );
	nHS = MAX( nHS, OT_HS_MINIMUM );
	//20170117
	nHS = 15000;
	psTF->nHumanScore = nHS;

	if( nHS <= OT_HS_HUMAN ){   // <= OT_HS_HUMAN (3000)
		psTF->ucPM = 0;
		psTF->usNotHumanFrame++;
		psTF->bHumanValid = FALSE;
	}else{
		psTF->usNotHumanFrame = 0;
	}
}

/*------------------------------------------------------------------------------
	�����?��?���������͔̂w�i�Ƃ���
------------------------------------------------------------------------------*/
BOOLEAN JudgeNotHuman( TrackingFeature* psTF, float fBackAveTemp )
{
	// 20170117
	//return FALSE;

	if( psTF->usNotHumanFrame >= OT_NOTHUMAN_FRAME ){
		return TRUE;
	}

	if( psTF->ucBlinkCount >= OT_NOTHUMAN_BLINK ){
		if( fBackAveTemp <= OT_NOTHUMAN_TEMP ){ // OT_NOTHUMAN_TEMP (25)
			return TRUE;
		}
	}

	if( psTF->usFrame % OT_LOGSPAN_T_L == 1 ){
		if( psTF->usFrame > (OT_LOGSPAN_T_L * OT_LOGSIZE_T_L) ){
			UCHAR	i;
			BOOLEAN	bDownTemp = TRUE;
			for( i = 1; i < OT_LOGSIZE_T_L; i++ ){
				if( psTF->fLogTMaxl[i-1] > psTF->fLogTMaxl[i] ){
					bDownTemp = FALSE;
				}
			}

			if( bDownTemp == TRUE ){
				float fTempNew = psTF->fLogTMaxl[0];
				float fTempOld = psTF->fLogTMaxl[OT_LOGSIZE_T_L-1];
				if( fTempOld - fTempNew >= OT_NOTHUMAN_ISU ){
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

/*------------------------------------------------------------------------------
	�o�͏��N?�A
------------------------------------------------------------------------------*/
void ClearOutInfoT( OutInfoT* psOutInfo )
{
	psOutInfo->ucHumanNum = 0;
	ClearArray( psOutInfo->ucMap, sOTI.Sz.ucOut, 0 );
	ClearArray( psOutInfo->ucLabelNum, OT_TRACKING_MAX, 0 );
	
	int i;
	for( i = 0; i < OT_TRACKING_MAX; i++ ){
		psOutInfo->sCenter[i].fx = 0;
		psOutInfo->sCenter[i].fy = 0;
	}

	/* 20161118 - howardjhli - add more info for detect alarm */
	ClearArray( psOutInfo->usArea, OT_TRACKING_MAX, 0 );
	for( i = 0; i < OT_TRACKING_MAX; i++ ){
		ClearRectangle(&psOutInfo->sRectangle[i]);
		//ClearRectangle(&psOutInfo->sRectangle_Prev[i]);
	}	
	//ClearArray( psOutInfo->ucNotHumanFrame, OT_TRACKING_MAX, 0 );
	//ClearArray( psOutInfo->ucBlinkCount, OT_TRACKING_MAX, 0 );
	ClearArray( psOutInfo->usFNMV, OT_TRACKING_MAX, 0);	

	ClearArray(psOutInfo->usFrame, OT_TRACKING_MAX, 0);


	ClearArray(psOutInfo->bInnerLine, OT_TRACKING_MAX, FALSE);
	ClearArray(psOutInfo->bOutterLine, OT_TRACKING_MAX, FALSE);
	ClearArray(psOutInfo->ucMoveStatus, OT_TRACKING_MAX, MOVE_NA);
}

/*------------------------------------------------------------------------------
	�o�͏��N?�A
------------------------------------------------------------------------------*/
void ClearTrackingOutputInfo( TrackingOutputInfo* psOutInfo )
{
	ClearOutInfoT( &psOutInfo->sOut );
	ClearArray( psOutInfo->sOutLog, OT_OUT_LOGSIZE, psOutInfo->sOut  );
	ClearArray( psOutInfo->ucIVOUT, OT_OUT_SZ_MAX,  OT_IVOUT_INITIAL );

	psOutInfo->ucLogLength = 0;
}

/*------------------------------------------------------------------------------
	�o�͏��p �d�S���W�擾
------------------------------------------------------------------------------*/
Coordinate GetCenterCoordinate( UCHAR ucTN )
{
	TrackingFeature*	psTF = &sOTI.sTF[ucTN];

	if( psTF->fV > OT_OUT_G_SEL_V_TH ){

		return psTF->sLogG[0];
	}else{

		return psTF->sGl;
	}
}

/*------------------------------------------------------------------------------
	�o�͏��p �l����?��
------------------------------------------------------------------------------*/
BOOLEAN GetHumanJudge( UCHAR ucTN )
{
	TrackingFeature*	psTF = &sOTI.sTF[ucTN];
	
	// 20170117
	//return TRUE;
	
	
	if( psTF->nHumanScore < OT_HS_HUMAN ){
		return FALSE;
	}
	return psTF->bHumanValid;
	
}

/*------------------------------------------------------------------------------
�o�̓}�b�v���W�ւ̕ϊ�
------------------------------------------------------------------------------*/
UCHAR ConvOutImgCoordinate( Coordinate sC )
{
	float	fxOut;
	float	fyOut;

	{

		fxOut = sC.fx / (sOTI.Sz.ucImgX/sOTI.Sz.ucOutX);
		fyOut = sC.fy / (sOTI.Sz.ucImgY/sOTI.Sz.ucOutY);
	}

	return( (UCHAR)fxOut + (UCHAR)fyOut * sOTI.Sz.ucOutX );
}

/*------------------------------------------------------------------------------
	���̒ǐՂɂ��o�͏��̍X�V
------------------------------------------------------------------------------*/
void UpdateTrackingOutInfo( TrackingOutputInfo* psOutInfo )
{
	void UpdateOutInfoFilterUsingLog( TrackingOutputInfo* );

	UCHAR	i;
	UCHAR	ucTN;
	UCHAR	ucSortNum = 0;
	UCHAR	ucSortList[OT_TRACKING_MAX];
	UCHAR	ucSortList_I[OT_TRACKING_MAX];
	UCHAR	ucFlag[OT_TRACKING_MAX];

	OutInfoT	sOut;

	UCHAR	ucAreaDetect[OT_OUT_SZ_MAX];
	ClearArray( ucAreaDetect, OT_OUT_SZ_MAX, 0 );

	ClearArray( ucFlag, OT_TRACKING_MAX, FALSE );

	while( 1 ){
		USHORT	usMaxArea	= 0;
		UCHAR	ucMaxTN		= 0;
		UCHAR	ucMaxOutImgIndex;

		for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){

			TrackingFeature*	psTF = &sOTI.sTF[ucTN];

			UCHAR ucOutImgIndex;
			Coordinate sG;
			USHORT usArea;

			if( ucFlag[ucTN] == TRUE ){
				continue;
			}

			if( psTF->ucLabelNum == 0 ){
				ucFlag[ucTN] = TRUE;
				continue;
			}

			sG = GetCenterCoordinate( ucTN );

			ucOutImgIndex = ConvOutImgCoordinate( sG );

			usArea  = psTF->usLogArea[0];
			usArea += psOutInfo->ucIVOUT[ ucOutImgIndex ];

			if( usMaxArea <= usArea ){
				usMaxArea			= usArea;
				ucMaxTN				= ucTN;
				ucMaxOutImgIndex	= ucOutImgIndex;
			}
		}

		if( usMaxArea == 0 ){
			break;
		}

		ucSortList  [ucSortNum] = ucMaxTN;
		ucSortList_I[ucSortNum] = ucMaxOutImgIndex;
		ucSortNum++;
		ucFlag[ucMaxTN] = TRUE;
	}

	ClearOutInfoT( &sOut );

	for( i = 0; i < ucSortNum; i++ ){
		UCHAR ucTN = ucSortList[i];
		TrackingFeature*	psTF = &sOTI.sTF[ucTN];
		UCHAR ucOutImgIndex = ucSortList_I[i];

		if( GetHumanJudge( ucTN ) == FALSE ){
			continue;
		}

		ucAreaDetect[ ucOutImgIndex ]++;

		if( psTF->usLogArea[0] < OT_OUT_AREA_TH ){
			continue;
		}
		if( psTF->usDetectFrame < OT_OUT_FRAME_TH ){
			continue;
		}


		sOut.ucLabelNum[i] = psTF->ucLabelNum;
		sOut.sCenter[i].fx = GetCenterCoordinate(ucTN).fx;
		sOut.sCenter[i].fy = GetCenterCoordinate(ucTN).fy;

		sOut.ucHumanNum++;
		sOut.ucMap[ucOutImgIndex]++;

		while( 1 ){
			if( sOTI.Sz.ucOut != 4 ){
				break;
			}
			if( psTF->usLogArea[0] < OT_OUT_UD_TH_A ){
				break;
			}
			if( (ucOutImgIndex & 0x02) == 0 ){
				break;
			}

			if( sOTI.Sz.ucImgY - 1 - psTF->sLogG[0].fy < OT_OUT_UD_TH_G ){
				break;
			}

			if( psTF->sRectangle.ucMinY > OT_OUT_UD_TH_R ){
				break;
			}

			sOut.ucMap[ucOutImgIndex-2]++;
			break;
		}

		if( sOTI.Sz.ucOut != 4 ){
			continue;
		}
		if( psTF->usLogArea[0] < OT_OUT_LR_TH_A ){
			continue;
		}

		if( (ucOutImgIndex & 0x01) == 0 ){

			if( psTF->sLogG[0].fx < OT_OUT_LR_TH_G ){
				continue;
			}

			if( sOTI.Sz.ucImgX - 1 - psTF->sRectangle.ucMaxX > OT_OUT_LR_TH_R ){
				continue;
			}

			ucOutImgIndex++;
		}else{

			if( sOTI.Sz.ucImgX - 1 - psTF->sLogG[0].fx < OT_OUT_LR_TH_G ){
				continue;
			}

			if( psTF->sRectangle.ucMinX > OT_OUT_LR_TH_R ){
				continue;
			}

			ucOutImgIndex--;
		}

		sOut.ucHumanNum++;
		sOut.ucMap[ucOutImgIndex]++;

		while( 1 ){
			if( sOTI.Sz.ucOut != 4 ){
				break;
			}
			if( psTF->usLogArea[0] < OT_OUT_UD_TH_A ){
				break;
			}
			if( (ucOutImgIndex & 0x02) == 0 ){
				break;
			}

			if( sOTI.Sz.ucImgY - 1 - psTF->sLogG[0].fy < OT_OUT_UD_TH_G ){
				break;
			}

			if( psTF->sRectangle.ucMinY > OT_OUT_UD_TH_R ){
				break;
			}

			sOut.ucMap[ucOutImgIndex-2]++;
			break;
		}
	}

	for( i = 0; i < sOTI.Sz.ucOut; i++ ){

		CHAR cIVOUT = psOutInfo->ucIVOUT[i];

		if( ucAreaDetect[i] > 0 ){

			cIVOUT  = MIN( cIVOUT + OT_IVOUT_UP * ucAreaDetect[i], OT_IVOUT_MAXIMUM );
		}else{

			cIVOUT  = MAX( cIVOUT - OT_IVOUT_DOWN, OT_IVOUT_MINIMUM );
		}

		psOutInfo->ucIVOUT[i] = (UCHAR)cIVOUT;
	}

	UpdateLog( psOutInfo->sOutLog, OT_OUT_LOGSIZE, sOut );


	if( psOutInfo->ucLogLength < OT_OUT_LOGSIZE ){
		psOutInfo->ucLogLength++;
		psOutInfo->sOut = sOut;
	}else{
		UpdateOutInfoFilterUsingLog( psOutInfo );
	}

}

/*------------------------------------------------------------------------------
	?�o?�ʂ�?�O��?�p�����o�͏��X�V
------------------------------------------------------------------------------*/
void UpdateOutInfoFilterUsingLog( TrackingOutputInfo* psOutInfo )
{
	UCHAR	i, j;
	UCHAR	ucTN;
	OutInfoT	sOut;

	ClearOutInfoT( &sOut );
	
	for( i = 0; i < sOTI.Sz.ucOut; i++ ){
		UCHAR	ucSumDetect;
		UCHAR	ucHist[OT_DETECT_MAX];
		UCHAR	ucMaxHist;
		UCHAR	ucHumanNum;
		
		ucSumDetect = 0;
		ClearArray( ucHist, psTCI->ucDetectNum, 0 );

		for( j = 0; j < OT_OUT_LOGSIZE; j++ ){

			if( psOutInfo->sOutLog[j].ucMap[i] == 0 ){
				continue;
			}

			ucSumDetect++;

			ucHist[ psOutInfo->sOutLog[j].ucMap[i]-1 ]++;
		}

		if( ucSumDetect <= OT_OUT_LOGSIZE / 2 ){
			continue;
		}

		ucMaxHist	= 0;
		ucHumanNum	= 0;
		
		for( j = 0; j < psTCI->ucDetectNum; j++ ){
			if( ucMaxHist < ucHist[j] ){
				ucMaxHist = ucHist[j] ;
				ucHumanNum = j+1;
			}
		}
		
		
		sOut.ucMap[i] = 1;
		sOut.ucHumanNum += ucHumanNum;

	}

	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){
		UCHAR ucIndex0;
		TrackingFeature* psTF = &sOTI.sTF[ucTN];

		if( psTF->usFrame < OT_LOGSIZE_G ){
			continue;
		}

		if( psTF->ucNoDetectFrame > 0 ){
			continue;
		}

		if( GetHumanJudge( ucTN ) == FALSE ){
			continue;
		}

		ucIndex0 = ConvOutImgCoordinate( psTF->sLogG[0] );

		for( i = 0; i < OT_LOGSIZE_G; i++ ){
			UCHAR ucIndex = ConvOutImgCoordinate( psTF->sLogG[i] );
			if( sOut.ucMap[ucIndex] == 1 ){
				break;
			}
		}

		if( i == OT_LOGSIZE_G ){
			sOut.ucMap[ucIndex0] = 1;
			sOut.ucHumanNum++;
		}

	}

	psOutInfo->sOut = sOut;

	ClearOutInfoT( &sOut );
	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){
		
		UCHAR ucIndex0;
		TrackingFeature* psTF = &sOTI.sTF[ucTN];
		if( psTF->usFrame > 1 ){
			if( psTF->ucNoDetectFrame == 0 ){
				if( GetHumanJudge( ucTN ) == TRUE ){
					ucIndex0 = ConvOutImgCoordinate( psTF->sLogG[0] );

					sOut.ucMap[ucIndex0] = 1;
					sOut.ucHumanNum++;
					
					sOut.ucLabelNum[ucTN] = psTF->ucLabelNum;
					sOut.sCenter[ucTN].fx = psTF->sLogG[0].fx;
					sOut.sCenter[ucTN].fy = psTF->sLogG[0].fy;
					/* 20161118 - howardjhli - add more info for detect alarm */
					sOut.sRectangle[ucTN] = psTF->sRectangle;
					//sOut.sRectangle_Prev[ucTN] = psTF->sRectangle_Prev;
					sOut.usArea[ucTN] = psTF->usLogArea[0];
					//sOut.ucNotHumanFrame[ucTN] = psTF->usNotHumanFrame;
					//sOut.ucBlinkCount[ucTN] = psTF->ucBlinkCount;
					
					for (int i=0; i<OT_LOGSIZE_D; i++){
						
						sOut.cLogDir[ucTN][i] = psTF->cLogDir[i];
					}
					sOut.usFNMV[ucTN] = psTF->usFNMV;
					sOut.usFrame[ucTN] = psTF->usFrame;

					sOut.bInnerLine[ucTN] = psTF->bInnerLine;
					sOut.bOutterLine[ucTN] = psTF->bOutterLine;
					sOut.ucMoveStatus[ucTN] = psTF->ucMoveStatus;
				}
			}
		}
	}
		
		
	psOutInfo->sOut = sOut;
}


/*------------------------------------------------------------------------------
	for check inner and outter line 
------------------------------------------------------------------------------*/
void UpdateMoveLineStatus(TrackingFeature* psTF){
	Coordinate sCenter = psTF->sLogG[0];
	
	if ((sCenter.fx < g_sConfigParameters.sTrackingParameters.usLeftXLine && sCenter.fx > g_sConfigParameters.sTrackingParameters.usRightXLine) &&
		(sCenter.fy < g_sConfigParameters.sTrackingParameters.usTopYLine && sCenter.fy > g_sConfigParameters.sTrackingParameters.usBottomYLine)){
		psTF->bInnerLine = TRUE;
		if (psTF->bOutterLine && psTF->bInnerLine){
			psTF->ucMoveStatus = MOVE_IN;
			psTF->bOutterLine = FALSE;
		}
	}
	else {
		psTF->bOutterLine = TRUE;
		if (psTF->bOutterLine && psTF->bInnerLine){
			psTF->ucMoveStatus = MOVE_OUT;
			psTF->bInnerLine = FALSE;
		}
	}

}


/*------------------------------------------------------------------------------
	for update human detect alarm info 
------------------------------------------------------------------------------*/
void UpdateHumanDetectAlarmInfo( TrackingOutputInfo* psOutInfo )
{
	UCHAR	i, j;
	UCHAR	ucTN;
	OutInfoT	sOut;

	ClearOutInfoT( &sOut );


	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){
		
		UCHAR ucIndex0;
		TrackingFeature* psTF = &sOTI.sTF[ucTN];
		//if( psTF->usFrame > OT_LOGSIZE_G ){
			if( psTF->ucNoDetectFrame == 0 ){

				ucIndex0 = ConvOutImgCoordinate( psTF->sLogG[0] );

				sOut.ucMap[ucIndex0] = 1;
				sOut.ucHumanNum++;
				
				sOut.ucLabelNum[ucTN] = psTF->ucLabelNum;
				sOut.sCenter[ucTN].fx = psTF->sLogG[0].fx;
				sOut.sCenter[ucTN].fy = psTF->sLogG[0].fy;
				/* 20161118 - howardjhli - add more info for detect alarm */
				sOut.sRectangle[ucTN] = psTF->sRectangle;
				//sOut.sRectangle_Prev[ucTN] = psTF->sRectangle_Prev;
				sOut.usArea[ucTN] = psTF->usLogArea[0];
				//sOut.ucNotHumanFrame[ucTN] = psTF->usNotHumanFrame;
				//sOut.ucBlinkCount[ucTN] = psTF->ucBlinkCount;
				
				for (int i=0; i<OT_LOGSIZE_D; i++){
					sOut.cLogDir[ucTN][i] = psTF->cLogDir[i];
				}
				sOut.usFNMV[ucTN] = psTF->usFNMV;
				sOut.usFrame[ucTN] = psTF->usFrame;

				sOut.bInnerLine[ucTN] = psTF->bInnerLine;
				sOut.bOutterLine[ucTN] = psTF->bOutterLine;
				sOut.ucMoveStatus[ucTN] = psTF->ucMoveStatus;
			}
		//}
	}
	psOutInfo->sOut = sOut;
}

/*******************************************************************************
 Update human temperature output
 ******************************************************************************/
void UpdateHumanTempOutInfo(UCHAR* pucDetectImg, short* pshHumanTemp, short* pshBackTemp, short* pshAveTemp ){
	USHORT	uImg;
	UCHAR	ucImgX	= sOTI.Sz.ucImgX;
	UCHAR	ucImgY	= sOTI.Sz.ucImgY;
	USHORT	usImg	= sOTI.Sz.usImg;
	UCHAR	ucX, ucY;
	UCHAR	ucTN;
	float	fBackAveTemp = 0;

	// �w�i?�xshort to float �����`
	for( UCHAR uImg = 0; uImg < usImg; uImg++ ){
		fBackAveTemp += TempStoF( pshBackTemp[uImg] );
	}
	fBackAveTemp /= usImg;  // �Z�o����?�x

	for( uImg = 0; uImg < usImg; uImg++ ){
		pshHumanTemp[uImg] = (pshBackTemp[uImg]);
	}

	for( ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++ ){
		UCHAR ucIndex0;
		TrackingFeature* psTF = &sOTI.sTF[ucTN];
		/*
		if( psTF->usFrame > OT_LOGSIZE_G ){
			if( psTF->ucNoDetectFrame == 0 ){
				if( GetHumanJudge( ucTN ) == TRUE ){

				
						for( uImg = 0; uImg < usImg; uImg++ ){
							if ((pucDetectImg[uImg] == psTF->ucLabelNum) && (psTF->ucLabelNum != 0)) {
								pshHumanTemp[uImg] = (pshAveTemp[uImg]);
							}
						}
					
				}
			}
		}
		*/
		for (uImg = 0; uImg < usImg; uImg++){
			if ((pucDetectImg[uImg] == psTF->ucLabelNum) && (psTF->ucLabelNum != 0)) {
				pshHumanTemp[uImg] = (pshAveTemp[uImg]);
			}
		}

	}
}


void UpdateTFFromAlarmInfo(OutInfo* psOutInfo){
	for (UCHAR ucTN = 0; ucTN < OT_TRACKING_MAX; ucTN++){ // ���F�I��?

		sOTI.sTF[ucTN].bInnerLine = psOutInfo->bInnerLine[ucTN];    
		sOTI.sTF[ucTN].bOutterLine = psOutInfo->bOutterLine[ucTN];
		sOTI.sTF[ucTN].ucMoveStatus = psOutInfo->ucMoveStatus[ucTN];

	}
}

/*******************************************************************************
 * Copyright(C) by 2014 Panasonic Corporation.
 ******************************************************************************/
