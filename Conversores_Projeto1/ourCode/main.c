

/**
 * main.c
 */

/* INCLUDES */
#include "DSP28x_Project.h"
#include "F2802x_Device.h"
#include "string.h"
#include "Math.h"
#include "sci.h"
#include "timer0.h"
#include "adc.h"
#include "pwm.h"
#include "stdlib.h"
#include "stdio.h"


#pragma CODE_SECTION(sciaRx_isr, "ramfuncs");
#pragma CODE_SECTION(Timer0_isr, "ramfuncs");
#pragma CODE_SECTION(Atualiza_ADC, "ramfuncs");
#pragma CODE_SECTION(Atualiza_PWM, "ramfuncs");
#pragma CODE_SECTION(scia_xmit, "ramfuncs");
#pragma CODE_SECTION(my_itoa, "ramfuncs");
#pragma CODE_SECTION(reverse, "ramfuncs");
// These are defined by the linker
extern uint16_t RamfuncsLoadStart;
extern uint16_t RamfuncsLoadSize;
extern uint16_t RamfuncsRunStart;
extern uint16_t RamfuncsLoadEnd;


//CHAR
char receive_sci=0, i=0, stop=1, controlo=0;
volatile char charUSART[16];
char flag_Timer0=0, flag_ADC=0,s[4];
char flag_atualiza_Duty=0, flag_periodic_sample_low = 0, duty_c[5];

//UINT_16
uint16_t my_DC, sensorSampleI=0, sensorSampleV=0;
volatile uint16_t CompA_StepUp= 750;

//INT
int valor_Atoi = 750;
int operation_mode = 0; //0- step up, 1- cc-ca pi, 2- cc-ca ps

//FLOAT
float erro=0,integral=0,pwm_ctrl=0;
float Vo=0, Io=0;
float Iadc=0,Vadc=0;

float ki=0.05;
float kp=1.0;
float Vref=3;




void pi(){
    erro=Vref-Vo;
    integral+=erro;
    pwm_ctrl=erro*kp + integral*ki;
    valor_Atoi=pwm_ctrl;
}

/***************************************************************************************/

int main(void){
// …..
    InitSysCtrl();
    // Step 2. Initialize Funcs
    InitGpio();
    //InitSciGpio();
    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    DINT;
    // Initialize PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags are cleared.
    InitPieCtrl();
    // Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;
    // Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).
    InitPieVectTable();

    #ifdef _FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
    #endif

    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
    InitFlash();

    EALLOW; // This is needed to write to EALLOWow protected registers

    PieVectTable.SCIRXINTA = &sciaRx_isr; // Group 9 PIE Peripheral Vectors:
    PieVectTable.TINT0 = &Timer0_isr;
    EDIS; // This is needed to disable write to EALLOW protected registers
                    //Step 5. User Code
    Config_sci();
    Config_gpio();
    Config_Timer0();
    Config_Adc();

    Config_EPwm_StepUp();

    CpuTimer0Regs.TCR.all = 0x4001; // Start timer
            //...
            //...
            // Enable interrupts required for this example
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1; // enable pie block
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1; // timer0
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1; // PIE Group 9, SCI-A
    IER = 0x101; // Enable CPU INT
    EINT;
    ERTM; // Enable Global realtime interrupt DBGM
    scia_xmit("ProjectoI");
    s[0]=0x0A;
    s[1]='\0';
    scia_xmit(s);

    while(1){
        if(flag_ADC){ // Refresh ADC values
        flag_ADC = 0;
        Atualiza_ADC();
        Calcular_Valores_ADC();
        pi();
        }
        if(receive_sci){
        receive_sci=0;
        switch(charUSART[0]){
            // teste sci
            case 't':
            case 'T':
                scia_xmit("Teste sci");
                s[0]=0x0A;
                s[1]='\0';
                scia_xmit(s);
                break;
            case 'j':
            case 'J':
                GpioDataRegs.GPASET.bit.GPIO0=1; // desligar porto
                break;
            case 'k':
            case 'K':
                GpioDataRegs.GPACLEAR.bit.GPIO0=1; // ligar porto
                break;
            case 'l':
            case 'L':
                GpioDataRegs.GPATOGGLE.bit.GPIO0=1; // toggle porto
                break;
            case 'd':
            case 'D':
                duty_c[0] = charUSART[1];
                duty_c[1] = charUSART[2];
                duty_c[2] = charUSART[3];
                duty_c[3] = charUSART[4];
                duty_c[4] = '\0';
                valor_Atoi = atoi(duty_c);
                scia_xmit(duty_c);
                break;
            case 'o':
            case 'O':
                EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; // Enable the complementary
                EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
                EPwm2Regs.AQCSFRC.bit.CSFA = 0; // Forces a continuous low on output A
                EPwm2Regs.AQCSFRC.bit.CSFB = 0; // Forces a continuous low on output B
                EPwm3Regs.AQCSFRC.bit.CSFA = 0; // Forces a continuous low on output A
                EPwm3Regs.AQCSFRC.bit.CSFB = 0; // Forces a continuous low on output B
                controlo = 1;
                stop=0;
                break;

            case 's':
            case 'S':
                stop=1;
                controlo=0;
                break;
            default:
                //
                break;
            } // end switch

        }
        if(stop){ // Disable the PWM
            EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HI; //
            EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HI;
            EPwm2Regs.AQCSFRC.bit.CSFA = 1; // Forces a continuous low on output A
            EPwm2Regs.AQCSFRC.bit.CSFB = 1; // Forces a continuous low on output B
            EPwm3Regs.AQCSFRC.bit.CSFA = 1; // Forces a continuous low on output A
            EPwm3Regs.AQCSFRC.bit.CSFB = 1; // Forces a continuous low on output B
            integral=0;
            }

        if(flag_atualiza_Duty && controlo && !stop){ // Refresh the DC value
            flag_atualiza_Duty = 0;
            Atualiza_PWM(valor_Atoi);
            }
    }
}

