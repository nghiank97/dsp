//
//
///**
// * gpio.h
// * Create on    : Aug 30,2024
// * Author       : nghiank97
// */
//#include "binary.h"
//
//
//
///*
// * CONTROL REGISTERS (EALLOW PROTECTED)
// */
//
//volatile u16 *GPACTRL   = (volatile u16*)0x6F80; // GPIO A Control Register (GPIO0-GPIO31) Figure 1-53
//volatile u16 *GPAQSEL1  = (volatile u16*)0x6F82; // GPIO A Qualifier Select 1 Register (GPIO0-GPIO15) Figure 1-55
//volatile u16 *GPAQSEL2  = (volatile u16*)0x6F84; // GPIO A Qualifier Select 2 Register (GPIO16-GPIO31) Figure 1-56
//volatile u16 *GPAMUX1   = (volatile u16*)0x6F86; // GPIO A MUX 1 Register (GPIO0-GPIO15) Figure 1-47
//volatile u16 *GPAMUX2   = (volatile u16*)0x6F88; // GPIO A MUX 2 Register (GPIO16-GPIO31) Figure 1-48
//volatile u16 *GPADIR    = (volatile u16*)0x6F8A; // GPIO A Direction Register (GPIO0-GPIO31) Figure 1-59
//volatile u16 *GPAPUD    = (volatile u16*)0x6F8C; // GPIO A Pull Up Disable Register (GPIO0-GPIO31) Figure 1-62
//
//volatile u16 *GPBCTRL   = (volatile u16*)0x6F90; // GPIO B Control Register (GPIO32-GPIO63) Figure 1-54
//volatile u16 *GPBQSEL1  = (volatile u16*)0x6F92; // GPIO B Qualifier Select 1 Register (GPIO32-GPIO47) Figure 1-57
//volatile u16 *GPBQSEL2  = (volatile u16*)0x6F94; // GPIO B Qualifier Select 2 Register (GPIO48 - GPIO63) Figure 1-58
//volatile u16 *GPBMUX1   = (volatile u16*)0x6F96; // GPIO B MUX 1 Register (GPIO32-GPIO47) Figure 1-49
//volatile u16 *GPBMUX2   = (volatile u16*)0x6F98; // GPIO B MUX 2 Register (GPIO48-GPIO63) Figure 1-50
//volatile u16 *GPBDIR    = (volatile u16*)0x6F9A; // GPIO B Direction Register (GPIO32-GPIO63) Figure 1-60
//volatile u16 *GPBPUD    = (volatile u16*)0x6F9C; // GPIO B Pull Up Disable Register (GPIO32-GPIO63) Figure 1-63
//
//volatile u16 *GPCMUX1   = (volatile u16*)0x6FA6; // GPIO C MUX 1 Register (GPIO64-GPIO79) Figure 1-51
//volatile u16 *GPCMUX2   = (volatile u16*)0x6FA8; // GPIO C MUX 2 Register (GPIO80-GPIO87) Figure 1-52
//volatile u16 *GPCDIR    = (volatile u16*)0x6FAA; // GPIO C Direction Register (GPIO64-GPIO87) Figure 1-61
//volatile u16 *GPCPUD    = (volatile u16*)0x6FAC; // GPIO C Pull Up Disable Register (GPIO64-GPIO87) Figure 1-64
//
//volatile u16 *GPADAT    = (volatile u16*)0x6FC0; // GPIO A Data Register (GPIO0 to 31)
//volatile u16 *GPASET    = (volatile u16*)0x6FC2; // GPIO A Data Set Register (GPIO0 to 31)
//volatile u16 *GPACLEAR  = (volatile u16*)0x6FC4; // GPIO A Data Clear Register (GPIO0 to 31)
//volatile u16 *GPATOGGLE = (volatile u16*)0x6FC6; // GPIO A Data Toggle Register (GPIO0 to 31)
//volatile u16 *GPBDAT    = (volatile u16*)0x6FC8; // GPIO B Data Register (GPIO32 to 63)
//volatile u16 *GPBSET    = (volatile u16*)0x6FCA; // GPIO B Data Set Register (GPIO32 to 63)
//volatile u16 *GPBCLEAR  = (volatile u16*)0x6FCC; // GPIO B Data Clear Register (GPIO32 to 63)
//volatile u16 *GPBTOGGLE = (volatile u16*)0x6FCE; // GPIOB Data Toggle Register (GPIO32 to 63)
//volatile u16 *GPCDAT    = (volatile u16*)0x6FD0; // GPIO C Data Register (GPIO64 to 87)
//volatile u16 *GPCSET    = (volatile u16*)0x6FD2; // GPIO C Data Set Register (GPIO64 to 87)
//volatile u16 *GPCCLEAR  = (volatile u16*)0x6FD4; // GPIO C Data Clear Register (GPIO64 to 87)
//volatile u16 *GPCTOGGLE = (volatile u16*)0x6FD6; // GPIO C Data Toggle Register (GPIO64 to 87)
