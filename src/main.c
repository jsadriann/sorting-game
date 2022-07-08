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
 *         Author:  José Adriann, jadriannassilva@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include	"gpio.h"
#include 	"interruption.h"
#include 	"timers.h"
#include 	"uart.h"
#include	"memory.h"
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
    putString("Frequence : ",17);
    for(int i = cont-1; i >= 0; i--)
        putCh(str[i]);
	
	putString(" ms\n\r",3);
}

void ISR_Handler(void){

	/* Verify active IRQ number */
	unsigned int irq_number = HWREG(INTC_BASE+INTC_SIR_IRQ) & 0x7f;
	switch(irq_number){
		case 62:
			gpioIsrHandler(GPIO3,type0,21);
			up();
			putString("aqui\n\r",6);
		break;

		case 95:
			timerIrqHandler(TIMER7);
			//putString("TIMER INTERRUPT!\n\r",18);
		break;
		
		case 63:
			gpioIsrHandler(GPIO3,type1,19);
			down();
			putString("aqui\n\r",6);
		break;

		case 98:
			gpioIsrHandler(GPIO1,type0,16);
			left();
			putString("aqui\n\r",6);
		break;

		case 99:
			gpioIsrHandler(GPIO1,type1,17);
			right();
			putString("aqui\n\r",6);
		break;
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
	//putString("button press!\n\r",15);
	int count = 0;
	ucPinNumber up = 21, down = 19, left = 16, right = 17;
	Direction dir_led = OUTPUT, dir_button = INPUT;

	disableWdt();
	
	/*-----------------------------------------------------------------------------
	 *  initialize GPIO modules
	 *-----------------------------------------------------------------------------*/
	Init_module_gpio(GPIO1);
	Init_module_gpio(GPIO3);
	
	timerSetup(TIMER7);
	/*-----------------------------------------------------------------------------
	 *  Initialize interruption gpio_1_Int_A and gpio_1_int_B
	 *-----------------------------------------------------------------------------*/
	Interrupt_Setup(98); //gpio1_A interruption
	Interrupt_Setup(99); //gpio1_B interruption
	Interrupt_Setup(62); //gpio3_A interruption
	Interrupt_Setup(63); //gpio3_B interruption

#ifdef DELAY_USE_INTERRUPT
	Interrupt_Setup(95); // timers interruption on DMTIMER7
#endif

	

	/*-----------------------------------------------------------------------------
	 *  Set pin for interruption
	 *-----------------------------------------------------------------------------*/
	Pin_Interrup_Config(GPIO3,up,0); // interruption 62
	Pin_Interrup_Config(GPIO3,down,1); // interruption 63
	Pin_Interrup_Config(GPIO1,left,0); // interruption 98
	Pin_Interrup_Config(GPIO1,right,1); // interruption 99

	Init_pin_gpio(GPIO3,up,INPUT);
	Init_pin_gpio(GPIO3,down,INPUT);
	Init_pin_gpio(GPIO1,left,INPUT);
	Init_pin_gpio(GPIO1,right,INPUT);

	while (tente){
		start_game();
		tente = false;
	}

	return 0;

} // end of function main

