/*
 * =====================================================================================
 *
 *       Filename:  gpio.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/02/2017 10:50:03
 *       Revision:  none
 *       Compiler:  arm-none-eabi-gcc
 *
 *         Author: José Adrian 
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */


#include "clock_module.h"
#include "hw_types.h"
#include "soc_AM335x.h"


static bool ckmCheckValidModule(CKM_MODULE_REG module){
	if((module > CKM_PER_CLK_24MHZ_CLKSTCTRL)){
      		return(false);
   	}
   	return(true);
}

void ckmSetCLKModuleRegister(CKM_MODULE_REG module, unsigned int value){
   	if(ckmCheckValidModule(module)){
      		unsigned int addr_temp = SOC_CM_PER_REGS + module;    // clock module base + module offset, TRM 2.1 & 8.1.12.1
		HWREG(addr_temp) |= value;
   	}
}

unsigned int ckmGetCLKModuleRegister(CKM_MODULE_REG module){
   	if(ckmCheckValidModule(module)){
      		unsigned int addr_temp = SOC_CM_PER_REGS + module;    // clock module base + module offset, TRM 2.1 & 8.1.12.1
      		return(HWREG(addr_temp));
   	}
   	return(0);
}

