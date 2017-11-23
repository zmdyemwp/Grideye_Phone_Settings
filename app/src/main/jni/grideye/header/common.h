/*******************************************************************************
 * Copyright(C) by 2014 Panasonic Corporation.
 ******************************************************************************/
#ifndef	__COMMON_H
#define	__COMMON_H

#include "../include/fih_common.h"
/*******************************************************************************
	�^��`
*******************************************************************************/
typedef unsigned char			BOOLEAN;
typedef unsigned char			UCHAR;
typedef unsigned short			USHORT;
typedef unsigned long			ULONG;
typedef signed char				CHAR;
typedef signed short			SHORT;
typedef signed long				LONG;


/*******************************************************************************
	�}�N?��`
*******************************************************************************/
#define		TRUE				1
#define		FALSE				0


/*----- �ėp�}�N? -----*/

#define		UCHAR_MAX_VAL		( 255)						/* 0xFF			*/
#define		CHAR_MAX_VAL		( 127)						/* 0x7F			*/
#define		CHAR_MIN_VAL		(-128)						/* 0x80			*/

#define		USHORT_MAX_VAL		( 65535)					/* 0xFFFF		*/
#define		SHORT_MAX_VAL		( 32767)					/* 0x7FFF		*/
#define		SHORT_MIN_VAL		(-32768)					/* 0x8000		*/


/* �v�Z */
#define		ABS(a)				( ((a)>0) ? (a) : ((a)*(-1)) )	/* ��Βl		*/
#define		MIN(a,b)			( ((a)<(b)) ? (a) : (b) )	/* �ŏ��l		*/
#define		MAX(a,b)			( ((a)>(b)) ? (a) : (b) )	/* �ő�l		*/
#define		POW(a)				( (a) * (a) )				/* 2��			*/

#define		SWAP(type,a,b)		\
{								\
	type c = a;					\
	a = b;						\
	b = c;						\
}

/* �\�t�g�E�F�A�^�C�}�[ */
#define		WAIT(cnt)					\
{										\
	volatile int i;						\
	for( i = 0; i < cnt; i++ );			\
}

/* �z��?���� */
#define		ClearArray(dst,size,data)	\
{										\
	int i;								\
	for( i = 0; i < size; i++ ){		\
		dst[i] = data;					\
	}									\
}

/* �z��R�s�[ */
#define		CopyArray(dst,size,src)		\
{										\
	int i;								\
	for( i = 0; i < size; i++ ){		\
		dst[i] = src[i];				\
	}									\
}




#define HUMAN_IN_BED  (0)
#define HUMAN_AREA  (1)
#define HUMAN_NUM   (2)
#define DETECT_NUM  (3)
#define BACK_TEMP_STATE (4)
#define CENTER_X  (5)
#define CENTER_Y (6)
#define ALERT_STATUS (7)
#define DETAIL_STATUS (8)
#define DETAIL_STATUS_CODE (10)
#define MAX_X	(11)
#define MAX_Y	(12)
#define MIN_X	(13)
#define MIN_Y	(14)


#define		KEY_DIFF_TEMP_THRESH	(1)
#define		KEY_LABELING_THRESH		(2)
#define		KEY_FNMV_THRESH		    (3)
#define		KEY_EDGE_THRESH		    (4)
#define		KEY_HUMAN_FRAME_THRESH	(5)
#define		KEY_BED_LEFT		    (6)
#define		KEY_BED_RIGHT		    (7)
#define		KEY_LEFT_RANGE		    (8)
#define		KEY_RIGHT_RANGE		    (9)
#define		KEY_TOP_RANGE		    (10)
#define		KEY_BOTTOM_RANGE	    (11)

#endif	/* __COMMON_H */
/*******************************************************************************
 * Copyright(C) by 2014 Panasonic Corporation.
 ******************************************************************************/
