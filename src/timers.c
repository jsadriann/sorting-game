#include "timers.h"


#define DELAY_USE_INTERRUPT			1
static bool flag_timer;

/**
 * \brief   This macro will check for write POSTED status

 *
 **/
void DMTimerWaitForWrite(unsigned int reg,Timer timer){
    switch (timer){
    case TIMER2:
        if(HWREG(SOC_DMTIMER_2_REGS+DMTIMER_TSICR) & 0x4);
        while((reg & HWREG(SOC_DMTIMER_2_REGS+DMTIMER_TWPS)));
    break;
    
    case TIMER3:
        if(HWREG(SOC_DMTIMER_3_REGS+DMTIMER_TSICR) & 0x4);
        while((reg & HWREG(SOC_DMTIMER_3_REGS+DMTIMER_TWPS)));
    break;
    
    case TIMER4:
        if(HWREG(SOC_DMTIMER_4_REGS+DMTIMER_TSICR) & 0x4);
        while((reg & HWREG(SOC_DMTIMER_4_REGS+DMTIMER_TWPS)));
    break;

    case TIMER5:
        if(HWREG(SOC_DMTIMER_5_REGS+DMTIMER_TSICR) & 0x4);
        while((reg & HWREG(SOC_DMTIMER_5_REGS+DMTIMER_TWPS)));
    break;

    case TIMER6:
        if(HWREG(SOC_DMTIMER_6_REGS+DMTIMER_TSICR) & 0x4);
        while((reg & HWREG(SOC_DMTIMER_6_REGS+DMTIMER_TWPS)));
    break;

    case TIMER7:
        if(HWREG(SOC_DMTIMER_7_REGS+DMTIMER_TSICR) & 0x4);
        while((reg & HWREG(SOC_DMTIMER_7_REGS+DMTIMER_TWPS)));
    break;
    
    default:
        break;
    }

}

void timerEnable(Timer timer){
    switch (timer){
    case TIMER2:
        /* Wait for previous write to complete in TCLR */
	    DMTimerWaitForWrite(0x1,TIMER2);
        /* Start the timer */
        HWREG(SOC_DMTIMER_2_REGS+DMTIMER_TCLR) |= 0x1;
    break;
    
    case TIMER3:
        /* Wait for previous write to complete in TCLR */
	    DMTimerWaitForWrite(0x1,TIMER3);
        /* Start the timer */
        HWREG(SOC_DMTIMER_3_REGS+DMTIMER_TCLR) |= 0x1;
    break;
    
    case TIMER4:
        /* Wait for previous write to complete in TCLR */
	    DMTimerWaitForWrite(0x1,TIMER4);
        /* Start the timer */
        HWREG(SOC_DMTIMER_4_REGS+DMTIMER_TCLR) |= 0x1;
    break;

    case TIMER5:
        /* Wait for previous write to complete in TCLR */
	    DMTimerWaitForWrite(0x1,TIMER5);
        /* Start the timer */
        HWREG(SOC_DMTIMER_5_REGS+DMTIMER_TCLR) |= 0x1;
    break;

    case TIMER6:
        /* Wait for previous write to complete in TCLR */
	    DMTimerWaitForWrite(0x1,TIMER6);
        /* Start the timer */
        HWREG(SOC_DMTIMER_6_REGS+DMTIMER_TCLR) |= 0x1;
    break;

    case TIMER7:
        /* Wait for previous write to complete in TCLR */
	    DMTimerWaitForWrite(0x1,TIMER7);
        /* Start the timer */
        HWREG(SOC_DMTIMER_7_REGS+DMTIMER_TCLR) |= 0x1;
    break;
    
    default:
        break;
    }
}/* -----  end of function timerEnable  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  timerDisable
 *  Description:  
 * =====================================================================================
 */
void timerDisable(Timer timer){
    switch (timer){
        case TIMER2:
            /* Wait for previous write to complete in TCLR */
            DMTimerWaitForWrite(0x1,TIMER2);
            /* Start the timer */
            HWREG(SOC_DMTIMER_2_REGS+DMTIMER_TCLR) &= ~(0x1);
        break;
        
        case TIMER3:
            /* Wait for previous write to complete in TCLR */
            DMTimerWaitForWrite(0x1,TIMER3);
            /* Start the timer */
            HWREG(SOC_DMTIMER_3_REGS+DMTIMER_TCLR) &= ~(0x1);
        break;
        
        case TIMER4:
            /* Wait for previous write to complete in TCLR */
            DMTimerWaitForWrite(0x1,TIMER4);
            /* Start the timer */
            HWREG(SOC_DMTIMER_4_REGS+DMTIMER_TCLR) &= ~(0x1);
        break;

        case TIMER5:
            /* Wait for previous write to complete in TCLR */
            DMTimerWaitForWrite(0x1,TIMER5);
            /* Start the timer */
            HWREG(SOC_DMTIMER_5_REGS+DMTIMER_TCLR) &= ~(0x1);
        break;

        case TIMER6:
            /* Wait for previous write to complete in TCLR */
            DMTimerWaitForWrite(0x1,TIMER6);
            /* Start the timer */
            HWREG(SOC_DMTIMER_6_REGS+DMTIMER_TCLR) &= ~(0x1);
        break;

        case TIMER7:
            /* Wait for previous write to complete in TCLR */
            DMTimerWaitForWrite(0x1,TIMER7);
            /* Start the timer */
            HWREG(SOC_DMTIMER_7_REGS+DMTIMER_TCLR) &= ~(0x1);
        break;
        
        default:
            break;
    }
}/* -----  end of function timerEnable  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  delay
 *  Description:  
 * =====================================================================================
 */
void delay(unsigned int mSec,Timer timer){
#ifdef DELAY_USE_INTERRUPT
    unsigned int countVal = TIMER_OVERFLOW - (mSec * TIMER_1MS_COUNT);

    switch (timer){
        case TIMER2:
            /* Wait for previous write to complete */
            DMTimerWaitForWrite(0x2,TIMER2);

            /* Load the register with the re-load value */
            HWREG(SOC_DMTIMER_2_REGS+DMTIMER_TCRR) = countVal;
            
            flag_timer = false;

            /* Enable the DMTimer interrupts */
            HWREG(SOC_DMTIMER_2_REGS+DMTIMER_IRQENABLE_SET) = 0x2; 

            /* Start the DMTimer */
            timerEnable(TIMER2);

            while(flag_timer == false);

            /* Disable the DMTimer interrupts */
            HWREG(SOC_DMTIMER_2_REGS+DMTIMER_IRQENABLE_CLR) = 0x2; 
        break;
        
        case TIMER3:
            /* Wait for previous write to complete */
            DMTimerWaitForWrite(0x2,TIMER3);

            /* Load the register with the re-load value */
            HWREG(SOC_DMTIMER_3_REGS+DMTIMER_TCRR) = countVal;
            
            flag_timer = false;

            /* Enable the DMTimer interrupts */
            HWREG(SOC_DMTIMER_3_REGS+DMTIMER_IRQENABLE_SET) = 0x2; 

            /* Start the DMTimer */
            timerEnable(TIMER3);

            while(flag_timer == false);

            /* Disable the DMTimer interrupts */
            HWREG(SOC_DMTIMER_3_REGS+DMTIMER_IRQENABLE_CLR) = 0x2; 
        break;
        
        case TIMER4:
            /* Wait for previous write to complete */
            DMTimerWaitForWrite(0x2,TIMER4);

            /* Load the register with the re-load value */
            HWREG(SOC_DMTIMER_4_REGS+DMTIMER_TCRR) = countVal;
            
            flag_timer = false;

            /* Enable the DMTimer interrupts */
            HWREG(SOC_DMTIMER_4_REGS+DMTIMER_IRQENABLE_SET) = 0x2; 

            /* Start the DMTimer */
            timerEnable(TIMER4);

            while(flag_timer == false);

            /* Disable the DMTimer interrupts */
            HWREG(SOC_DMTIMER_4_REGS+DMTIMER_IRQENABLE_CLR) = 0x2; 
        break;

        case TIMER5:
            /* Wait for previous write to complete */
            DMTimerWaitForWrite(0x2,TIMER5);

            /* Load the register with the re-load value */
            HWREG(SOC_DMTIMER_5_REGS+DMTIMER_TCRR) = countVal;
            
            flag_timer = false;

            /* Enable the DMTimer interrupts */
            HWREG(SOC_DMTIMER_5_REGS+DMTIMER_IRQENABLE_SET) = 0x2; 

            /* Start the DMTimer */
            timerEnable(TIMER5);

            while(flag_timer == false);

            /* Disable the DMTimer interrupts */
            HWREG(SOC_DMTIMER_5_REGS+DMTIMER_IRQENABLE_CLR) = 0x2; 
        break;

        case TIMER6:
            /* Wait for previous write to complete */
            DMTimerWaitForWrite(0x2,TIMER6);

            /* Load the register with the re-load value */
            HWREG(SOC_DMTIMER_6_REGS+DMTIMER_TCRR) = countVal;
            
            flag_timer = false;

            /* Enable the DMTimer interrupts */
            HWREG(SOC_DMTIMER_6_REGS+DMTIMER_IRQENABLE_SET) = 0x2; 

            /* Start the DMTimer */
            timerEnable(TIMER6);

            while(flag_timer == false);

            /* Disable the DMTimer interrupts */
            HWREG(SOC_DMTIMER_6_REGS+DMTIMER_IRQENABLE_CLR) = 0x2; 
        break;

        case TIMER7:
            /* Wait for previous write to complete */
            DMTimerWaitForWrite(0x2,TIMER7);

            /* Load the register with the re-load value */
            HWREG(SOC_DMTIMER_7_REGS+DMTIMER_TCRR) = countVal;
            
            flag_timer = false;

            /* Enable the DMTimer interrupts */
            HWREG(SOC_DMTIMER_7_REGS+DMTIMER_IRQENABLE_SET) = 0x2; 

            /* Start the DMTimer */
            timerEnable(TIMER7);

            while(flag_timer == false);

            /* Disable the DMTimer interrupts */
            HWREG(SOC_DMTIMER_7_REGS+DMTIMER_IRQENABLE_CLR) = 0x2; 
        break;
        
        default:
            break;
    }
#else
    switch (timer){
        case TIMER2:
            while(mSec != 0){
                /* Wait for previous write to complete */
                DMTimerWaitForWrite(0x2,TIMER2);

                /* Set the counter value. */
                HWREG(SOC_DMTIMER_2_REGS+DMTIMER_TCRR) = 0x0;

                timerEnable(TIMER2);

                while(HWREG(SOC_DMTIMER_2_REGS+DMTIMER_TCRR) < TIMER_1MS_COUNT);

                /* Stop the timer */
                HWREG(SOC_DMTIMER_2_REGS+DMTIMER_TCLR) &= ~(0x00000001u);

                mSec--;
            }
        break;
        
        case TIMER3:
            while(mSec != 0){
                /* Wait for previous write to complete */
                DMTimerWaitForWrite(0x2,TIMER3);

                /* Set the counter value. */
                HWREG(SOC_DMTIMER_3_REGS+DMTIMER_TCRR) = 0x0;

                timerEnable(TIMER3);

                while(HWREG(SOC_DMTIMER_3_REGS+DMTIMER_TCRR) < TIMER_1MS_COUNT);

                /* Stop the timer */
                HWREG(SOC_DMTIMER_3_REGS+DMTIMER_TCLR) &= ~(0x00000001u);

                mSec--;
            }
        break;
        
        case TIMER4:
            while(mSec != 0){
                /* Wait for previous write to complete */
                DMTimerWaitForWrite(0x2,TIMER4);

                /* Set the counter value. */
                HWREG(SOC_DMTIMER_4_REGS+DMTIMER_TCRR) = 0x0;

                timerEnable(TIMER4);

                while(HWREG(SOC_DMTIMER_4_REGS+DMTIMER_TCRR) < TIMER_1MS_COUNT);

                /* Stop the timer */
                HWREG(SOC_DMTIMER_4_REGS+DMTIMER_TCLR) &= ~(0x00000001u);

                mSec--;
            }
        break;

        case TIMER5:
            while(mSec != 0){
                /* Wait for previous write to complete */
                DMTimerWaitForWrite(0x2,TIMER5);

                /* Set the counter value. */
                HWREG(SOC_DMTIMER_5_REGS+DMTIMER_TCRR) = 0x0;

                timerEnable(TIMER5);

                while(HWREG(SOC_DMTIMER_5_REGS+DMTIMER_TCRR) < TIMER_1MS_COUNT);

                /* Stop the timer */
                HWREG(SOC_DMTIMER_TIMER5_REGS+DMTIMER_TCLR) &= ~(0x00000001u);

                mSec--;
            }
        break;

        case TIMER6:
            while(mSec != 0){
                /* Wait for previous write to complete */
                DMTimerWaitForWrite(0x2,TIMER6);

                /* Set the counter value. */
                HWREG(SOC_DMTIMER_6_REGS+DMTIMER_TCRR) = 0x0;

                timerEnable(TIMER6);

                while(HWREG(SOC_DMTIMER_6_REGS+DMTIMER_TCRR) < TIMER_1MS_COUNT);

                /* Stop the timer */
                HWREG(SOC_DMTIMER_6_REGS+DMTIMER_TCLR) &= ~(0x00000001u);

                mSec--;
            }
        break;

        case TIMER7:
            while(mSec != 0){
                /* Wait for previous write to complete */
                DMTimerWaitForWrite(0x2,TIMER7);

                /* Set the counter value. */
                HWREG(SOC_DMTIMER_7_REGS+DMTIMER_TCRR) = 0x0;

                timerEnable(TIMER7);

                while(HWREG(SOC_DMTIMER_7_REGS+DMTIMER_TCRR) < TIMER_1MS_COUNT);

                /* Stop the timer */
                HWREG(SOC_DMTIMER_7_REGS+DMTIMER_TCLR) &= ~(0x00000001u);

                mSec--;
            }
        break;
        
        default:
            break;
    }
    
#endif
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  timerSetup
 *  Description:  
 * =====================================================================================
 */
void timerSetup(Timer timer){
    switch (timer){
        case TIMER2:
            /*  Clock enable for DMTIMER2 TRM 8.1.12.1.25 */
            HWREG(SOC_CM_PER_REGS+TIMER2_CLKCTRL) |= 0x2;

            /*  Check clock enable for DMTIMER2 TRM 8.1.12.1.25 */    
            while((HWREG(SOC_CM_PER_REGS+TIMER2_CLKCTRL) & 0x3) != 0x2);
        break;
        
        case TIMER3:
            /*  Clock enable for DMTIMER3 TRM 8.1.12.1.25 */
            HWREG(SOC_CM_PER_REGS+TIMER3_CLKCTRL) |= 0x2;

            /*  Check clock enable for DMTIMER3 TRM 8.1.12.1.25 */    
            while((HWREG(SOC_CM_PER_REGS+TIMER3_CLKCTRL) & 0x3) != 0x2);

        break;
        
        case TIMER4:
            /*  Clock enable for DMTIMER4 TRM 8.1.12.1.25 */
            HWREG(SOC_CM_PER_REGS+TIMER4_CLKCTRL) |= 0x2;

            /*  Check clock enable for DMTIMER4 TRM 8.1.12.1.25 */    
            while((HWREG(SOC_CM_PER_REGS+TIMER4_CLKCTRL) & 0x3) != 0x2);

        break;

        case TIMER5:
            /*  Clock enable for DMTIMER5 TRM 8.1.12.1.25 */
            HWREG(SOC_CM_PER_REGS+TIMER5_CLKCTRL) |= 0x2;

            /*  Check clock enable for DMTIMER5 TRM 8.1.12.1.25 */    
            while((HWREG(SOC_CM_PER_REGS+TIMER5_CLKCTRL) & 0x3) != 0x2);

        break;

        case TIMER6:
            /*  Clock enable for DMTIMER6 TRM 8.1.12.1.25 */
            HWREG(SOC_CM_PER_REGS+TIMER6_CLKCTRL) |= 0x2;

            /*  Check clock enable for DMTIMER6 TRM 8.1.12.1.25 */    
            while((HWREG(SOC_CM_PER_REGS+TIMER6_CLKCTRL) & 0x3) != 0x2);

        break;

        case TIMER7:
            /*  Clock enable for DMTIMER7 TRM 8.1.12.1.25 */
            HWREG(SOC_CM_PER_REGS+TIMER7_CLKCTRL) |= 0x2;

            /*  Check clock enable for DMTIMER7 TRM 8.1.12.1.25 */    
            while((HWREG(SOC_CM_PER_REGS+TIMER7_CLKCTRL) & 0x3) != 0x2);

        break;
        
        default:
            break;
    }

}

void timerIrqHandler(Timer timer){
    switch (timer){
        case TIMER2:
            /* Clear the status of the interrupt flags */
            HWREG(SOC_DMTIMER_2_REGS+DMTIMER_IRQSTATUS) = 0x2; 

            flag_timer = true;

            /* Stop the DMTimer */
            timerDisable(TIMER2);
        break;
        
        case TIMER3:
            /* Clear the status of the interrupt flags */
            HWREG(SOC_DMTIMER_3_REGS+DMTIMER_IRQSTATUS) = 0x2; 

            flag_timer = true;

            /* Stop the DMTimer */
            timerDisable(TIMER3);
        break;
        
        case TIMER4:
            /* Clear the status of the interrupt flags */
            HWREG(SOC_DMTIMER_4_REGS+DMTIMER_IRQSTATUS) = 0x2; 

            flag_timer = true;

            /* Stop the DMTimer */
            timerDisable(TIMER4);
        break;

        case TIMER5:
            /* Clear the status of the interrupt flags */
            HWREG(SOC_DMTIMER_5_REGS+DMTIMER_IRQSTATUS) = 0x2; 

            flag_timer = true;

            /* Stop the DMTimer */
            timerDisable(TIMER5);
        break;

        case TIMER6:
            /* Clear the status of the interrupt flags */
            HWREG(SOC_DMTIMER_6_REGS+DMTIMER_IRQSTATUS) = 0x2; 

            flag_timer = true;

            /* Stop the DMTimer */
            timerDisable(TIMER6);
        break;

        case TIMER7:
            /* Clear the status of the interrupt flags */
            HWREG(SOC_DMTIMER_7_REGS+DMTIMER_IRQSTATUS) = 0x2; 

            flag_timer = true;

            /* Stop the DMTimer */
            timerDisable(TIMER2);
        break;
        
        default:
            break;
    }

}


