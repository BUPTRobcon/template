#ifndef __MAIN_H
#define __MAIN_H

#include "spi.h"
#include "cmd.h"
#include "configuration.h"
#include "param.h"
#include "can.h"
#include "TIM.h"
#include "math.h"
#include "vega.h"
#include "math.h"
#include "global.h"

float pur_pitch,pur_roll;
bool roll_flag=false,pitch_flag=false;

struct {
	float d,dd,d_last,now;
	int cnt;
}pitch,roll;

#endif