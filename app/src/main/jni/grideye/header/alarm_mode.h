/*
 * IncFile1.h
 *
 * Created: 2016/11/25 上午 10:37:54
 *  Author: HowardJHLi
 */ 


#ifndef ALARM_MODE_H_
#define ALARM_MODE_H_


/*******************************************************************************
	function define
*******************************************************************************/
void UpdateIsHumanInBed(OutInfo*, OutInfo*, AlarmOutInfo*);
void UpdateIsHumanNotInBed(OutInfo*, OutInfo*, AlarmOutInfo*);
void UpdateIsHumanInBedStatus(OutInfo*, OutInfo*, AlarmOutInfo*);
void UpdateHumanStatusInBed(OutInfo*, OutInfo*, AlarmOutInfo*);
void UpdateHumanLeaveBedStatus(OutInfo*, OutInfo*, AlarmOutInfo*);

void CheckHumanApart(OutInfo*, OutInfo*, AlarmOutInfo*);
void GetHumanAlarmStatus(OutInfo*, OutInfo*, AlarmOutInfo*);

void UpdateIsHumanInBath(OutInfo*, OutInfo*, AlarmOutInfo*);
void UpdateIsHumanNotInBath(OutInfo*, OutInfo*, AlarmOutInfo*);
void UpdateIsHumanInBathStatus(OutInfo*, OutInfo*, AlarmOutInfo*);
void UpdateHumanStatusInBath(OutInfo*, OutInfo*, AlarmOutInfo*);


void UpdateEffectiveCount(OutInfo*, OutInfo*, AlarmOutInfo*, UCHAR);
void UpdateEffectiveNum(AlarmOutInfo*);
/*******************************************************************************
 * Copyright(C) by 2016 HowardJHLi.
 ******************************************************************************/


#endif /* ALARM_MODE_H_ */