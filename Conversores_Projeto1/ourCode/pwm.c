#include"pwm.h"

extern int operation_mode;

uint16_t srs_limites(int valor){
    if(valor > DC_MAX)
    {
        valor=DC_MAX;
    }
    else if(valor < DC_MIN)
    {
        valor=DC_MIN;
    }
    return valor;
}

void Config_EPwm_StepUp(void){
// PWM_Period_StepUp = 60 MHz/40 kHz = 1500
        EPwm2Regs.TBPRD = PWM_Period_StepUp; // Period = 1501 TBCLK counts
        EPwm2Regs.TBPHS.half.TBPHS = 0; // Set Phase register to zero
        EPwm2Regs.TBCTR = 0; // clear TB counter
        EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count mode up
        EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE; // disable phase loading
        EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1; // TBCLK = SYSCLK
        EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;
        EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW; // period autoreload
        EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE; // sync down-stream module
        // Setup shadow register load on ZERO
        EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
        EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR = Zero
        EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR = Zero
        //Set compare values
        EPwm2Regs.CMPA.half.CMPA = 0;//CompA_StepUp; // Compare A = 350 TBCLK counts
        EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR; // clear PWM2A on event a, up count
        EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET; // set PWM2A on event ZRO, up count
}

void Atualiza_PWM(int new_DC){
    //new_DC=limites(new_DC,DC_MAX,DC_MIN);
    new_DC=srs_limites(new_DC);
   // my_itoa(new_DC,printer);
     //    scia_xmit(printer);
       //  scia_xmit(s);
    if(!operation_mode)
            EPwm2Regs.CMPA.half.CMPA = new_DC; // Compare A = 350 TBCLK counts
    /*    else if(operation_mode == 1){
            EPwm2Regs.CMPA.half.CMPA = new_DC;
            EPwm3Regs.CMPA.half.CMPA = EPwm2Regs.TBPRD - EPwm2Regs.CMPA.half.CMPA;
            }
        else if(operation_mode == 2){
            if(flag_periodic_sample_low){
               EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET; // clear PWM2A on event a, up count
               EPwm3Regs.AQCTLA.bit.ZRO = AQ_CLEAR; // set PWM2A on event ZRO, up count
                }
                else{
                    EPwm2Regs.AQCTLA.bit.ZRO = AQ_CLEAR; // clear PWM2A on event a, up count
                    EPwm3Regs.AQCTLA.bit.ZRO = AQ_SET; // set PWM2A on event ZRO, up count
                    }
            }*/
}

void Config_gpio(void){

    EALLOW;


    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;
    GpioDataRegs.GPASET.bit.GPIO0 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO0=1;        //output
    GpioDataRegs.GPACLEAR.bit.GPIO0=1;      // turn off port

    InitSciGpio();

    InitEPwm2Gpio();
    InitEPwm3Gpio();
}
