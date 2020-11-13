#include "stdlib.h"
#include "DSP28x_Project.h"
#include "F2802x_Device.h"

#define PWM_Period_StepUp 1200
#define DC_MAX 1150
#define DC_MIN 50

uint16_t srs_limites(int valor);
void Config_EPwm_StepUp(void);
void Atualiza_PWM(int new_DC);
void Config_gpio(void);
