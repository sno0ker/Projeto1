#include "stdlib.h"
#include "string.h"
#include "DSP28x_Project.h"
#include "F2802x_Device.h"

interrupt void sciaRx_isr(void);
void scia_xmit(char *string);
void Config_sci(void);
void reverse(char s[]);
void my_itoa(int n, char s[]);
