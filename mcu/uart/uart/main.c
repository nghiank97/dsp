/*
 * main.c
 *
 *  Created on: 05-Sep-2024
 *  Author: hong xiem
 */

#include "DSP28x_Project.h"

// Prototype statements for functions found within this file.
void scic_init(void);
void scic_fifo_init(void);
void scic_xmit(int a);
void scic_msg(char *msg);

// Global counts used in this example
Uint16 LoopCount;
Uint16 ErrorCount;

void main(void) {
    InitSysCtrl();
    DINT;
    InitPieCtrl();
    InitPieVectTable();

    scic_fifo_init();
    scic_init();

    EALLOW;
    GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;
    GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;
    EDIS;

    for (;;) {
        scic_msg("Hello World!\r\n");
        DELAY_US(100000);
        GpioDataRegs.GPBTOGGLE.bit.GPIO48=1;
    }
}

void scic_init(void) {

    /* SCI setup GPIO */
    EALLOW;
    // Enable pull-up for GPIO62 (SCIRXDC)
    GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;
    // Enable pull-up for GPIO63 (SCITXDC)
    GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;
    // Asynch input GPIO62 (SCIRXDC)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;
    // Configure GPIO62 to SCIRXDC
    GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;
    // Configure GPIO63 to SCITXDC
    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;
    EDIS;
    /* SCI initialize */
    // data 8 bit
    ScicRegs.SCICCR.bit.SCICHAR = 0x07;
    // parity: no ~ disable
    ScicRegs.SCICCR.bit.PARITYENA = 0;
    // 1 stop bit
    ScicRegs.SCICCR.bit.STOPBITS = 0;

    // tx enable, rx enable
    ScicRegs.SCICTL1.bit.RXENA = 1;
    ScicRegs.SCICTL1.bit.TXENA = 1;

    /* SCI Asynchronous Baud = LSPCLK / ((BRR + 1) *8)
     * -> BRR = (LSPCLK/8)/Baud - 1
     */

    /* LSPCLK = 37.5MHz, baud = 115200
     * -> BRR = (37.5M/8)/115200 - 1 ~= 40;
     */
#if (CPU_FRQ_150MHZ)
    ScicRegs.SCIHBAUD = 0;
    ScicRegs.SCILBAUD = 40;
#endif
    ScicRegs.SCICTL1.bit.SWRESET = 1;
}

// Transmit a character from the SCI
void scic_xmit(int a) {
    while (ScicRegs.SCIFFTX.bit.TXFFST != 0) {
    }
    ScicRegs.SCITXBUF = a;
}

void scic_msg(char * msg) {
    int i;
    i = 0;
    while (msg[i] != '\0') {
        scic_xmit(msg[i]);
        i++;
    }
}

// Initialize the SCI FIFO
void scic_fifo_init() {
    ScicRegs.SCIFFTX.all = 0xE040;
    ScicRegs.SCIFFRX.all = 0x204f;
    ScicRegs.SCIFFCT.all = 0x0;
}
