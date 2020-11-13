#include "sci.h"


extern volatile char charUSART[16];
extern char receive_sci;
extern int i;



interrupt void sciaRx_isr(void){
   // i=0;
    charUSART[i] = SciaRegs.SCIRXBUF.all;
        if(charUSART[i++] == 0x0A){
            charUSART[i] = '\0';
            receive_sci = 1;
            i = 0;
        }
    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1; // Clear Overflow flag
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1; // Clear Interrupt flag
    PieCtrlRegs.PIEACK.all |= 0x100; // Issue PIE ack
}

void scia_xmit(char *string){
int index = 0;
    while(string[index] != '\0'){
        while( ( (SciaRegs.SCIFFTX.bit.TXFFST!=1) & (SciaRegs.SCICTL2.bit.TXEMPTY!=0) ) == 0 );
            SciaRegs.SCITXBUF = string[index];
        index++;
    }
}

void Config_sci(void){
    SciaRegs.SCICCR.all = 0x0007; // 1 stop bit, No loopback
    // No parity, 8 char bits,
    // async mode, idle-line protocol
    SciaRegs.SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK,
    // Disable RX ERRO, SLEEP, TXWAKE
    SciaRegs.SCICTL2.bit.TXINTENA = 1; // Enable TXRDY interrupt
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1; // Enable RXRDY/BRKDT interrupt
    SciaRegs.SCIHBAUD = 0x00; // Ideal Baud=x ##
    SciaRegs.SCILBAUD = 0xC2;
    SciaRegs.SCIFFTX.all = 0xC001;
    SciaRegs.SCIFFRX.all = 0x0021;
    SciaRegs.SCIFFCT.all = 0x00;
    SciaRegs.SCICTL1.all = 0x0023; // Relinquish SCI from Reset
    SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;
}

void reverse(char s[]){

    int a, b;
    char c;
    for (a = 0, b = strlen(s)-1; a<b; a++, b--) {
        c = s[a];
        s[a] = s[b];
        s[b] = c;
    }
}

void my_itoa(int n, char s[]){
    int aux_i, sign;
    if ((sign = n) < 0) // record sign
        n = -n; // make n positive
        aux_i = 0;
        do { // generate digits in reverse order
              s[aux_i++] = n % 10 + '0'; // get next digit
        } while ((n /= 10) > 0); // delete it
            if (sign < 0)
                s[aux_i++] = '-';
            s[aux_i] = '\0';
            reverse(s);
 }
