/*
 * File     : main.c
 * Author   : hong xiem
 * Date     : 09-Sep, 2024
 */
#include "binary.h"
#include "DSP28x_Project.h"

void Init_Gpio(void);
void Init_PWM(void);
void Put_PWM(u16 percent);

void main(void)
{
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

    Init_Gpio();
    Init_PWM();
    while(1)
    {
        Put_PWM(25);
    }
}

void Init_Gpio(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO0   = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO0   = 1;
    EDIS;

}

void Init_PWM(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

#if (CPU_FRQ_150MHZ)
    EPwm1Regs.TBPRD             = 7500;             // Set timer period 801 TBCLKs
#endif
    EPwm1Regs.TBPHS.half.TBPHS  = 0x0000;           // Phase is 0
    EPwm1Regs.TBCTR             = 0x0000;           // Clear counter

    EPwm1Regs.CMPA.half.CMPA    = 2500;             // Set compare A value

    EPwm1Regs.TBCTL.bit.CTRMODE = 0x2;              // Count up-down
    EPwm1Regs.TBCTL.bit.PHSEN   = 0;                // Disable phase loading
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;              // Clock ratio to SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV  = 0;


    EPwm1Regs.AQCTLA.bit.CAU    = 0x2;              // Set PWM1A on event A, up count
    EPwm1Regs.AQCTLA.bit.CAD    = 0x1;              //Clear PWM1A on event A, down count

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}

void Put_PWM(u16 percent)
{
    EPwm1Regs.CMPA.half.CMPA    = 7500-(u16)percent*75;             // Set compare A value
}
