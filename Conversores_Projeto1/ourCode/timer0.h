#include "stdlib.h"
#include "DSP28x_Project.h"
#include "F2802x_Device.h"

#define Timer0_PRD 1200

void Config_Timer0(void);
interrupt void Timer0_isr(void);
