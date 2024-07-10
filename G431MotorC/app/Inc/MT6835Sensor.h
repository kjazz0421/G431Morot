#ifndef _MT6835SENSOR_H_
#define _MT6835SENSOR_H_

#include "spi.h"
#include "gpio.h"
#include "stdio.h"
#include "main.h"
#include "stdint.h"

#define Read			0x3000
#define Write 			0x6000
#define WriteEEPROM		0xC000
#define SetZeroPoint	0x5000
#define ContinuousRead	0xA000

void Read_AngleReg(void);

#endif // _MT6835SENSOR_H_
