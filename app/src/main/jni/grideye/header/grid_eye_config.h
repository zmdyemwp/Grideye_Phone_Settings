/*
 * grid_eye_cinfig.h
 *
 * Created: 2015/5/21 11:23:53
 *  Author: Sam
 */


#ifndef GRID_EYE_CINFIG_H_
#define GRID_EYE_CINFIG_H_

#include "common.h"
//*****************************************************************************
//
//! \addtogroup GRID_EYE_Component GRID_EYE
//! \brief Component ILI9325
//! @{
//
//*****************************************************************************
//*****************************************************************************
//
//! \addtogroup GRID_EYE_CONFIGURE Configuration
//! \brief The configure macro for grid eye sensor
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup GRID_EYE_Data_Type Data Type
//! \brief The data type for grid eye sensor
//! @{
//
//*****************************************************************************
//typedef unsigned char			BOOLEAN;
//typedef unsigned char			UCHAR;
//typedef unsigned short			USHORT;
//typedef unsigned long			ULONG;
//typedef signed char				CHAR;

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup GRID_EYE_System_Config Feature Setting
//! \brief The configure macro for grid eye sensor
//! @{
//
//*****************************************************************************
/* Grid-EYE's number of pixels */
#define	TRUE					(1)
#define	FALSE					(0)
#define	SHORT_MAX_VAL	   	    ( 32767)				/* 0x7FFF		*/
#define	SHORT_MIN_VAL		    (-32768)					/* 0x8000		*/
#define	ULONG_MAX_VAL		    ( 4294967295)		    /* 0xFFFFFFFF	*/

/* Grid-EYE's number of image pixels */
#define imageWidth				8
#define imageHeight				8

/* Grid-EYE's I2C Sampling period */
#define GRID_EYE_SAMP_PERI      600   ///< Sampling period (ms) ;
#define GRID_EYE_IIC_SPEED      I2C_MASTER_BAUD_RATE_400KHZ ///< IIC interface speed ;

/* Grid-EYE's I2C slave address */
#define GRID_EYE_ADDRESS        0x68 ///< AD_SELECT == GND ? 0x68 : 0x69 ;
#define	GRIDEYE_ADR			    GRID_EYE_ADDRESS

/* Grid-EYE's register address */
#define	GRIDEYE_REG_THS00	   (GE_TTHL_REG)	/* head address of thermistor  resister	*/
#define	GRIDEYE_REG_TMP00	   (GE_PIXEL_BASE)	/* head address of temperature resister	*/

/* Grid-EYE's register size */
#define	GRIDEYE_REGSZ_THS  	   (0x02)	/* size of thermistor  resister		*/
#define	GRIDEYE_REGSZ_TMP	   (0x80)	/* size of temperature resister		*/

/* Grid-EYE's number of pixels */
#define	SNR_SZ_X			   (imageWidth)
#define	SNR_SZ_Y			   (imageHeight)
#define	SNR_SZ				   (SNR_SZ_X * SNR_SZ_Y)
#define HDS_SZ				   (11)		/* human detection data size */
#define DM_SZ				   (1)		/* DEBUG MODE size */
#define DETECT_SZ			   (225)    /* detect img */
#define TRACKING_SZ			   (55)    /* tracking data size */
#define ALARM_SZ			   (1)    /* alarm data size */
#define	SNR_HDS_SZ			   (SNR_SZ_X * SNR_SZ_Y + DM_SZ  + TRACKING_SZ + ALARM_SZ + DETECT_SZ)
#define RAW_DATA_SZ			   (64)


/* Setting size of human detection */
#define	IMG_SZ_X			   (SNR_SZ_X * 2 - 1)
#define	IMG_SZ_Y			   (SNR_SZ_Y * 2 - 1)
#define	IMG_SZ				   (IMG_SZ_X * IMG_SZ_Y)
#define	OUT_SZ_X			   (2)
#define	OUT_SZ_Y			   (2)
#define	OUT_SZ				   (OUT_SZ_X * OUT_SZ_Y)

/* Parameters of human detection */
#define TEMP_FILTER_FRAME_NUM (5)
#define TEMP_FILTER_FRAME_NUM2 (3)

#define	TEMP_FRAME_NUM		   (8)
#define	TEMP_MEDIAN_FILTER	   (2)
#define	TEMP_SMOOTH_COEFF	   (0.1f)
#define	DIFFTEMP_THRESH		   (0.3f)
#define	DETECT_MARK			   ((UCHAR)0xFF)
#define	LABELING_THRESH		   (3)
#define	OUTPUT_THRESH		   (6)
#define	BKUPDT_COEFF		   (0.1f)
#define	HUMAN_JUDGE_AREA	   (4)
#define	HUMAN_JUDGE_FRAME	   (1)

/* Parameters of background update */
#define	BKUPDT_ALLUP_THRESH	   (0.9f)	/* The ratio of the number of pixels that detected the object	*/
#define	BKUPDT_ALLUP_TEMP	   (0.1f)	/* Uniform update temperature			*/


/* memory size of object detection */
#define	SIZE_OF_ODT			   (IMG_SZ+0x20)

/* memory size of object tracking */
#define	TRACKING_MAX		   (5)
#define	SIZE_OF_OTR			   ((TRACKING_MAX+1) * 0x138 )


/* Parameters of human detection */
#define		NORMAL		(0)
#define		PREALARM	(1)
#define		ALARM	    (2)
#define     SIT_THRESH  (2)

/* detect status type*/
#define     EMPTY		(0)
#define		NORMAL		(1)
#define		PREALARM	(2)
#define		ALARM	    (3)


#define no_Human			(0)
#define human_in			(1)
#define human_in_bed_board  (2)
#define human_in_bed		(3)
#define human_out_bed_board (4)
#define human_out_bed		(5)
#define human_in_care		(6)
#define human_out_care		(7)
#define human_head			(8)
#define human_no_head		(9)
#define human_out			(10)


/* number of frames for reconfirm status detection */
#define     DETECT_STATUS_CONTINUOUS_THREESH (3)


#define BED_OUTTER	(0)
#define BED_BOARD	(1)
#define BED_INNER	(2)
#define BED_IS_HUMAN	(3)


#define IS_HUMAN = (0)
#define HUMAN_AREA = (1)
#define HUMAN_NUM = (2)
#define DETECT_NUM = (3)
#define BACK_TEMP_STATE = (4)
#define CENTER_X = (5)
#define CENTER_Y = (6)
#define DETECT_STATUS = (7)
#define DETAIL_STATUS = (8)
#define FILTER_DATA = (9)
#define DETAIL_STATUS_CODE = (10)


//*****************************************************************************
//
//! @}
//
//*****************************************************************************
//*****************************************************************************
//
//! @}
//
//*****************************************************************************
//*****************************************************************************
//
//! @}
//
//*****************************************************************************
#define MIN_X (0)
#define MIN_Y (1)
#define MAX_X (2)
#define MAX_Y (3)
struct ge_cb_packet {
	/** detected human center  */
	short center [2] ;
	/** number of detected human */
	UCHAR detectHumanNum;
	/** Maximum area of human */
	UCHAR human_maximum_area;
	/** the human status code **/
	UCHAR statusValue;
	/** alarm status **/
	UCHAR alarmStatus;
	/** detail status **/
	UCHAR detailStatus;
	/** rectangle for max min x y **/
	UCHAR rectangle [4];
	/** Detect Labeling Img array **/
	UCHAR aucDetectImg [IMG_SZ];
};

struct ge_config_packet {
	/** The thresh of detect different temperature **/
	float difftemp_thresh;
	/** The thresh of to be identified a object **/
	int labeling_thresh;
	/** The thresh of to be identified a human **/
	int human_thresh;
	/** Boundary of Bed boarder **/
	int boundary_thresh;
	/** Is Filter enable **/
	BOOLEAN isFilterEnable;

	//setValue(float d, int l, int h, int b, BOOLEAN i){
		//this->difftemp_thresh = d;
		//this->labeling_thresh = l;
		//this->human_thresh = h;
		//this->boundary_thresh = b;
		//this->isFilterEnable = i;
	//}
};

#endif /* GRID_EYE_CINFIG_H_ */


