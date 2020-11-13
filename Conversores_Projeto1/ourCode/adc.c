#include "adc.h"


extern uint16_t sensorSampleV, sensorSampleI;
extern float Vadc, Iadc, Vo, Io;

void Config_Adc(void){
    // Sequencia de PowerUp
    EALLOW;
    // Sequencia de PowerUp
    AdcRegs.ADCCTL1.bit.ADCBGPWD = 1; // Power ADC BG
    AdcRegs.ADCCTL1.bit.ADCREFPWD = 1; // Power reference
    AdcRegs.ADCCTL1.bit.ADCPWDN = 1; // Power ADC
    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1; // ADCINT1 trips after AdcResults latch
    DELAY_US(1000);
    // ADCA0
    AdcRegs.ADCSOC0CTL.bit.CHSEL = 0; // SOC0 will convert pin A0 -> ADCINA0 || j5.6
    AdcRegs.ADCSOC0CTL.bit.ACQPS = 6; // ADCSOC0CTL = 0x0886 -> sample window 7 cycles
    AdcRegs.ADCSOC0CTL.bit.TRIGSEL = 1; // TRIGSEL -> timer0
    // ADCA1
    AdcRegs.ADCSOC1CTL.bit.CHSEL = 1; // ADCSOC0CTL = 0x0886 -> sample window 7 cycles
    AdcRegs.ADCSOC1CTL.bit.ACQPS = 6; // ADCSOC0CTL = 0x0886 -> sample window 7 cycles
    AdcRegs.ADCSOC1CTL.bit.TRIGSEL = 1; // TRIGSEL -> timer0
    AdcRegs.INTSEL1N2.bit.INT1CONT = 0; // disable ADCINT1 Continuous mode
    AdcRegs.INTSEL1N2.bit.INT1SEL = 1; // INTSEL1N2 = 0x0001, EOC1 is trigger for ADCINT1
    AdcRegs.INTSEL1N2.bit.INT1E = 1; // enable ADCINT1
    EDIS;
}

void Atualiza_ADC(){
    while(AdcRegs.ADCINTFLG.bit.ADCINT1 == 0){} // Wait for ADCINT1
        AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; // Clear ADCINT1
        sensorSampleV = AdcResult.ADCRESULT0; // Get adc sample result
        sensorSampleI = AdcResult.ADCRESULT1;
}

void Calcular_Valores_ADC(void){
    Vadc=sensorSampleV*3.3/4096;
    Iadc=sensorSampleI*3.3/4096;

    Vo=40*Vadc-60;
    if(Vadc<1){
        Vo=0;
    }
    Io=10*Iadc-15;
}
