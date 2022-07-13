/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/02/2017 20:05:55
 *       Revision:  none
 *       Compiler:  arm-none-eabi-gcc
 *
 *         Author:  José Adrian, jadriannassilva@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include	"gpio.h"
#include 	"interruption.h"
#include 	"timers.h"
#include 	"uart.h"
#include	"memory.h"
#include 	"menus.h"
#define DELAY_USE_INTERRUPT			1


/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
void ledON(gpioMod, ucPinNumber, ucPinNumber);
void ledOFF(gpioMod,  ucPinNumber, ucPinNumber );
int button_press(gpioMod, ucPinNumber);
void mode_led(mode);
bool tente = true;

/****************************************************************************
**				GLOBAL VARIABLES
****************************************************************************/
unsigned int opc_button1 = MODE0 , opc_button2 = MODE4, opc = MODE0, TIME = 100;


void disableWdt(void){
	HWREG(WDT_WSPR) = 0xAAAA;
	while((HWREG(WDT_WWPS) & (1<<4)));
	
	HWREG(WDT_WSPR) = 0x5555;
	while((HWREG(WDT_WWPS) & (1<<4)));
}

void print_frequence(unsigned int frequence){
	int num;
    int cont = 0;
    char str[5];
    while(frequence > 0){
        num = frequence%10;
        frequence = frequence/10;
        str[cont] = num+'0';
        cont++;
    }
    putString(UART0,"Frequence : ",17);
    for(int i = cont-1; i >= 0; i--)
        putCh(UART0,str[i]);
	
	putString(UART0," ms\n\r",3);
}

void ISR_Handler(void){

	/* Verify active IRQ number */
	unsigned int irq_number = HWREG(INTC_BASE+INTC_SIR_IRQ) & 0x7f;
	if(irq_number == 98){
		if(HWREG(SOC_GPIO_1_REGS+GPIO_IRQSTATUS_RAW_0)&(1<<13)){
			up();
			gpioIsrHandler(GPIO1,0,13);
		}else if(HWREG(SOC_GPIO_1_REGS+GPIO_IRQSTATUS_RAW_0)&(1<<12)){
			down();
			gpioIsrHandler(GPIO1,0,12);
		}else if (HWREG(SOC_GPIO_1_REGS+GPIO_IRQSTATUS_RAW_0)&(1<<15)){
			left();
			gpioIsrHandler(GPIO1,0,15);
		}else{
			right();
			gpioIsrHandler(GPIO1,0,14);
		}
		
	}
    
	/* acknowledge IRQ */
	HWREG(INTC_BASE+INTC_CONTROL) = 0x1;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */

int main(void)
{
	int count = 0;
	ucPinNumber up = 13, down = 12, left = 15, right = 14;
	Direction dir_led = OUTPUT, dir_button = INPUT;
	char r = '0';

	disableWdt();
	
	/*-----------------------------------------------------------------------------
	 *  initialize GPIO modules
	 *-----------------------------------------------------------------------------*/
	Init_module_gpio(GPIO1);
	
	timerSetup(TIMER7);
	/*-----------------------------------------------------------------------------
	 *  Initialize interruption gpio_1_Int_A and gpio_1_int_B
	 *-----------------------------------------------------------------------------*/
	Interrupt_Setup(98); //gpio1_A interruption

#ifdef DELAY_USE_INTERRUPT
	Interrupt_Setup(95); // timers interruption on DMTIMER7
#endif
		
	/*-----------------------------------------------------------------------------
	 *  Set pin for interruption
	 *-----------------------------------------------------------------------------*/
	Pin_Interrup_Config(GPIO1,up,0); // interruption 98
	Pin_Interrup_Config(GPIO1,down,0); // interruption 98
	Pin_Interrup_Config(GPIO1,left,0); // interruption 98
	Pin_Interrup_Config(GPIO1,right,0); // interruption 98

	Init_pin_gpio(GPIO1,up,INPUT);
	Init_pin_gpio(GPIO1,down,INPUT);
	Init_pin_gpio(GPIO1,left,INPUT);
	Init_pin_gpio(GPIO1,right,INPUT);

	while(tente){
		menu_select_game();
		while((r != '1') && (r != '2') && (r != '3')){
			r = getCh(UART0);
		}
		start_game(r);
		menu_continua();
		while(r != '1' && r != '2'){
			r = getCh(UART0);
		}
		if(r == '2') tente = false;
	}

	return 0;

} // end of function main

