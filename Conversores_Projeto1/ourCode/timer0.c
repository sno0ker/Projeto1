#include"timer0.h"

extern char flag_ADC,flag_Timer0,flag_atualiza_Duty;


void Config_Timer0(void){

    EALLOW;
    CpuTimer0Regs.PRD.all = Timer0_PRD; //periodo
    CpuTimer0Regs.TCR.bit.TSS = 1;      //stop timer
    CpuTimer0Regs.TPR.all = 0x0;        // init prescale counter to divide by 1(sysclockout)

    CpuTimer0Regs.TCR.bit.TRB = 1;       //reload all counter registers with period value
    CpuTimer0Regs.TCR.bit.FREE = 0;     // STOP AFTER THE timH: tim decrements to 0
    CpuTimer0Regs.TCR.bit.SOFT = 1;
    CpuTimer0Regs.TCR.bit.TIE = 1;      // enable interrupt
    EDIS;
}

interrupt void Timer0_isr(void){
    AdcRegs.ADCSOCFRC1.all = 0x03; // forces the start of the SOC0 and SOC1 ADC conversation

    flag_ADC=1;
    flag_Timer0=1;
    flag_atualiza_Duty=1;
    //GpioDataRegs.GPATOGGLE.bit.GPIO0=1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
