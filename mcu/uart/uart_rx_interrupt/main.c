/*
 * main.c
 *
 *  Created on: 05-Sep-2024
 *  Author: hong xiem
 */
#include "binary.h"
#include "DSP28x_Project.h"


#define LED_TOGGLE()        (GpioDataRegs.GPBTOGGLE.bit.GPIO48=1)


// Prototype statements for functions found within this file.
void scic_init(void);
void scic_fifo_init(void);
void scic_xmit(int a);
void scic_msg(char *msg);
char scic_recv(void);
interrupt void scic_rx_isr(void);

void main(void) {
    InitSysCtrl();
    DINT;
    InitPieCtrl();
    // Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    // For this example, only initialize the Cpu Timers
    InitCpuTimers();
    #if (CPU_FRQ_150MHZ)
        // Configure CPU-Timer 0, 1, and 2 to interrupt every second:
        // 150MHz CPU Freq, 1 second Period (in uSeconds)
        ConfigCpuTimer(&CpuTimer0, 150, 10000);
        ConfigCpuTimer(&CpuTimer1, 150, 1000);
        ConfigCpuTimer(&CpuTimer2, 150, 1000000);
    #endif

    EALLOW;
    GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;
    GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;
    EDIS;

    scic_fifo_init();
    scic_init();

    for (;;) {
        DELAY_US(1000000);
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
    EALLOW;
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

    /* SCI setup interrupt */
    ScicRegs.SCICTL2.bit.RXBKINTENA = 1;
    ScicRegs.SCICTL2.bit.TXINTENA = 1;

    /* Relinquish SCI from Reset */
    ScicRegs.SCICTL1.bit.SWRESET = 1;
    EDIS;

    EALLOW;

    // Enable CPU int8 and int9 which are connected to SCIs
    IER |= M_INT8;
    // Enable SCIRXINTC in the PIE: Group 8 interrupt 5
    PieCtrlRegs.PIEIER8.bit.INTx5 = 1;
    // Enable SCITXINTC in the PIE: Group 8 interrupt 6
    PieCtrlRegs.PIEIER8.bit.INTx6 = 1;
    //Set my ISR to be run rather than the default.
    PieVectTable.SCIRXINTC = &scic_rx_isr;
    // Enable global Interrupts and higher priority real-time debug events:
    EDIS;
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM


    scic_msg("hello \r\n");
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

char scic_recv(void){
    char c;
    while (!ScicRegs.SCIRXST.bit.RXRDY);
    c = (char)ScicRegs.SCIRXBUF.all;
    return c;
}


interrupt void scic_rx_isr(void){
    LED_TOGGLE();
    if (ScicRegs.SCIRXST.bit.RXRDY){
        scic_msg("hello world \r\n");
        ScicRegs.SCIRXST.bit.RXRDY = 0;
        // Acknowledge this interrupt to receive more interrupts from group 8
        PieCtrlRegs.PIEACK.bit.ACK8 = 1;
    }

//    unsigned char t;
//
//    while (ScicRegs.SCIRXST.bit.RXRDY)
//    {
//    t = ScicRegs.SCIRXBUF.bit.RXDT;
//    if (IsDirectGPS)
//    {
//    scia_xmit_ex(t);
//    }
//    else
//    {
//    if (SCIC_RX_COUNT < SCI_RX_BUF_MAX)
//    {
//    SCIC_RX_BUF[SCIC_RX_COUNT++] = t;
//    SCIC_RX_NEW = TRUE;
//    }
//    }
//    }
//
//    // Acknowledge this interrupt to receive more interrupts from group 8
//    PieCtrlRegs.PIEACK.bit.ACK8 = 1;
}

// Initialize the SCI FIFO
void scic_fifo_init() {
    ScicRegs.SCIFFTX.all = 0xE040;
    ScicRegs.SCIFFRX.all = 0x204f;
    ScicRegs.SCIFFCT.all = 0x0;
}
