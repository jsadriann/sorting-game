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
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include "gpio.h"
#define mask_init_module					(1<<18) | (0x2<<0)
#define mask_init_pin_function_gpio			(0x7<<0)
#define mask_received_enabled				(1<<5)

/*-----------------------------------------------------------------------------
 * insert [pin, mod], get CM_conf adress
 * Datasheet 4.3.2
 *-----------------------------------------------------------------------------*/
static const CONTROL_MODULE GPIO_CTRL_MODULE_ARRAY[32][4] = 
{
    //p0                          //p1                          //p2                          //p3
   {CM_conf_mdio                 ,CM_conf_gpmc_ad0             ,CM_conf_gpmc_csn3            ,CM_conf_mii1_col       },//.0
   {CM_conf_mdc                  ,CM_conf_gpmc_ad1             ,CM_conf_gpmc_clk             ,CM_conf_mii1_crs       },//.1
   {CM_conf_spi0_sclk            ,CM_conf_gpmc_ad2             ,CM_conf_gpmc_advn_ale        ,CM_conf_mii1_rx_er     },//.2
   {CM_conf_spi0_d0              ,CM_conf_gpmc_ad3             ,CM_conf_gpmc_oen_ren         ,CM_conf_mii1_tx_en     },//.3
   {CM_conf_spi0_d1              ,CM_conf_gpmc_ad4             ,CM_conf_gpmc_wen             ,CM_conf_mii1_rx_dv     },//.4
   {CM_conf_spi0_cs0             ,CM_conf_gpmc_ad5             ,CM_conf_gpmc_ben0_cle        ,CM_conf_i2c0_sda       },//.5
   {CM_conf_spi0_cs1             ,CM_conf_gpmc_ad6             ,CM_conf_lcd_data0            ,CM_conf_i2c0_scl       },//.6
   {CM_conf_ecap0_in_pwm0_out    ,CM_conf_gpmc_ad7             ,CM_conf_lcd_data1            ,CM_conf_emu0           },//.7
   {CM_conf_lcd_data12           ,CM_conf_uart0_ctsn           ,CM_conf_lcd_data2            ,CM_conf_emu1           },//.8
   {CM_conf_lcd_data13           ,CM_conf_uart0_rtsn           ,CM_conf_lcd_data3            ,CM_conf_mii1_tx_clk    },//.9
   {CM_conf_lcd_data14           ,CM_conf_uart0_rxd            ,CM_conf_lcd_data4            ,CM_conf_mii1_rx_clk    },//.10
   {CM_conf_lcd_data15           ,CM_conf_uart0_txd            ,CM_conf_lcd_data5            ,-1                     },//.11
   {CM_conf_uart1_ctsn           ,CM_conf_gpmc_ad12            ,CM_conf_lcd_data6            ,-1                     },//.12
   {CM_conf_uart1_rtsn           ,CM_conf_gpmc_ad13            ,CM_conf_lcd_data7            ,CM_conf_usb1_drvvbus   },//.13
   {CM_conf_uart1_rxd            ,CM_conf_gpmc_ad14            ,CM_conf_lcd_data8            ,CM_conf_mcasp0_aclkx   },//.14
   {CM_conf_uart1_txd            ,CM_conf_gpmc_ad15            ,CM_conf_lcd_data9            ,CM_conf_mcasp0_fsx     },//.15
   {CM_conf_mii1_txd3            ,CM_conf_gpmc_a0              ,CM_conf_lcd_data10           ,CM_conf_mcasp0_axr0    },//.16
   {CM_conf_mii1_txd2            ,CM_conf_gpmc_a1              ,CM_conf_lcd_data11           ,CM_conf_mcasp0_ahclkr  },//.17
   {CM_conf_usb0_drvvbus         ,CM_conf_gpmc_a2              ,CM_conf_mii1_rxd3            ,CM_conf_mcasp0_aclkr   },//.18
   {CM_conf_xdma_event_intr0     ,CM_conf_gpmc_a3              ,CM_conf_mii1_rxd2            ,CM_conf_mcasp0_fsr     },//.19
   {CM_conf_xdma_event_intr1     ,CM_conf_gpmc_a4              ,CM_conf_mii1_rxd1            ,CM_conf_mcasp0_axr1    },//.20
   {CM_conf_mii1_txd1            ,CM_conf_gpmc_a5              ,CM_conf_mii1_rxd0            ,CM_conf_mcasp0_ahclkx  },//.21
   {CM_conf_gpmc_ad8             ,CM_conf_gpmc_a6              ,CM_conf_lcd_vsync            ,-1                     },//.22
   {CM_conf_gpmc_ad9             ,CM_conf_gpmc_a7              ,CM_conf_lcd_hsync            ,-1                     },//.23
   {-1                           ,CM_conf_gpmc_a8              ,CM_conf_lcd_pclk             ,-1                     },//.24
   {-1                           ,CM_conf_gpmc_a9              ,CM_conf_lcd_ac_bias_en       ,-1                     },//.25
   {CM_conf_gpmc_ad10            ,CM_conf_gpmc_a10             ,CM_conf_mmc0_dat3            ,-1                     },//.26
   {CM_conf_gpmc_ad11            ,CM_conf_gpmc_a11             ,CM_conf_mmc0_dat2            ,-1                     },//.27
   {CM_conf_mii1_txd0            ,CM_conf_gpmc_ben1            ,CM_conf_mmc0_dat1            ,-1                     },//.28
   {CM_conf_rmii1_ref_clk        ,CM_conf_gpmc_csn0            ,CM_conf_mmc0_dat0            ,-1                     },//.29
   {CM_conf_gpmc_wait0           ,CM_conf_gpmc_csn1            ,CM_conf_mmc0_clk             ,-1                     },//.30
   {CM_conf_gpmc_wpn             ,CM_conf_gpmc_csn2            ,CM_conf_mmc0_cmd             ,-1                     },//.31
};
static bool check_module_gpio(gpioMod GPIO){
	if(GPIO >= GPIO0 && GPIO <= GPIO3) // verification if GPIO be range the GPIO0:GPIO3
		return true;
	return false;
}

static bool check_pin_gpio(unsigned char pin){
	if(pin >= 0 && pin <= 31) // verification if GPIO be range the 0:31
		return true;
	return false;


}

int Init_module_gpio(gpioMod GPIO){
	if(!check_module_gpio(GPIO)) return 0;

	switch (GPIO)
	{
	case GPIO0:
		// GPIO0 haven't control clock
	break;
	
	case GPIO1:
		while(HWREG(CKM_PER + CKM_PER_GPIO1_CLKCTRL) & (0x3<<16));// if module is desabled
		
		HWREG(CKM_PER + CKM_PER_GPIO1_CLKCTRL) |= (1<<18) | (0x2<<0); // set (1<<18) | (0x2<<0)
	break;

	case GPIO2:
		while(HWREG(CKM_PER + CKM_PER_GPIO2_CLKCTRL) & (0x3<<16)); // if module is desabled
		
		HWREG(CKM_PER + CKM_PER_GPIO1_CLKCTRL) |= (1<<18) | (0x2<<0); // set (1<<18) | (0x2<<0)
	break;

	case GPIO3:
		while(HWREG(CKM_PER + CKM_PER_GPIO3_CLKCTRL) & (0x3<<16)); // if module is desabled
		
		HWREG(CKM_PER + CKM_PER_GPIO1_CLKCTRL) |= mask_init_module; // set (1<<18) | (0x2<<0)
	break;
	}
	
	return 1;

}

int Init_pin_gpio(gpioMod gpio, unsigned char pin, Direction dir){
	if(!(check_module_gpio(gpio) && check_pin_gpio(pin))) return -1;

	unsigned int pin_r = GPIO_CTRL_MODULE_ARRAY[pin][gpio];
	HWREG(SOC_CONTROL_REGS + pin_r) |= mask_init_pin_function_gpio;
	if(dir == INPUT){
		HWREG(SOC_CONTROL_REGS + pin_r) |= mask_received_enabled;
	}
	Set_direction_pin_gpio(gpio,dir,pin);
	return 1;

}

static void Set_direction_pin_gpio(gpioMod gpio, Direction dir, unsigned char pin){
	if(dir == INPUT || dir == OUTPUT){
		switch (gpio)
		{
		case GPIO0:
			HWREG(SOC_GPIO_0_REGS + GPIO_OE) &= ~(1<<pin);
			HWREG(SOC_GPIO_0_REGS + GPIO_OE) |= (dir<<pin);
		break;
		
		case GPIO1:
			HWREG(SOC_GPIO_1_REGS + GPIO_OE) &= ~(1<<pin);
			HWREG(SOC_GPIO_1_REGS + GPIO_OE) |= (dir<<pin);
		break;

		case GPIO2:
			HWREG(SOC_GPIO_2_REGS + GPIO_OE) &= ~(1<<pin);
			HWREG(SOC_GPIO_2_REGS + GPIO_OE) |= (dir<<pin);
		break;
		
		case GPIO3:
			HWREG(SOC_GPIO_3_REGS + GPIO_OE) &= ~(1<<pin);
			HWREG(SOC_GPIO_3_REGS + GPIO_OE) |= (dir<<pin);
		break;
		}
	}
}

unsigned int Get_direction_pin_gpio(gpioMod gpio, unsigned char pin){
	if(!(check_module_gpio(gpio) && check_pin_gpio(pin))) return -1;
	unsigned int ret;
	switch (gpio)
	{
	case GPIO0:
		ret = HWREG(SOC_GPIO_0_REGS + GPIO_OE) & (1<<pin);
	break;

	case GPIO1:
		ret = HWREG(SOC_GPIO_1_REGS + GPIO_OE) & (1<<pin);
	break;
	
	case GPIO2:
		ret = HWREG(SOC_GPIO_2_REGS + GPIO_OE) & (1<<pin);
	break;

	case GPIO3:
		ret = HWREG(SOC_GPIO_3_REGS + GPIO_OE) & (1<<pin);
	break;
	default:
		break;
	}
	if(ret) 
		return INPUT;
	
	return OUTPUT;
}

void GpioSetPinValue(gpioMod mod, ucPinNumber pin, Level value){
	unsigned int addr_temp, val_temp;

	if(value == HIGH)
	{
		if(check_module_gpio(mod) && check_pin_gpio(pin))
		{
			switch (mod) 
			{
				case GPIO0:	
					// GPIOx base + set data out offset, TRM 2.1 & 25.4.1.26
      				addr_temp = SOC_GPIO_0_REGS + GPIO_SETDATAOUT;
					val_temp = 1<<pin;
				break;

				case GPIO1:
					// GPIOx base + set data out offset, TRM 2.1 & 25.4.1.26
      				addr_temp = SOC_GPIO_1_REGS + GPIO_SETDATAOUT;
					val_temp = 1<<pin;
				break;

				case GPIO2:	
					// GPIOx base + set data out offset, TRM 2.1 & 25.4.1.26
      				addr_temp = SOC_GPIO_2_REGS + GPIO_SETDATAOUT;
					val_temp = 1<<pin;	
				break;

				case GPIO3:	
					// GPIOx base + set data out offset, TRM 2.1 & 25.4.1.26
      				addr_temp = SOC_GPIO_3_REGS + GPIO_SETDATAOUT;
					val_temp = 1<<pin;	
				break;
				break;

				default:	
				break;
			}/* -----  end switch  ----- */
      		HWREG(addr_temp) |= val_temp;
   		}
	}else
	{
		if(check_module_gpio(mod) && check_pin_gpio(pin))
		{
			switch (mod) 
			{
				case GPIO0:
					// GPIOx base+clear data out offset, TRM 2.1 & 25.4.1.25
      				addr_temp = SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT;        			
					val_temp = 1<<pin;
				break;

				case GPIO1:
					// GPIOx base+clear data out offset, TRM 2.1 & 25.4.1.25
      				addr_temp = SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT;        			
					val_temp = 1<<pin;
				break;

				case GPIO2:
					// GPIOx base+clear data out offset, TRM 2.1 & 25.4.1.25
      				addr_temp = SOC_GPIO_2_REGS + GPIO_CLEARDATAOUT;        			
					val_temp = 1<<pin;		
				break;

				case GPIO3:	
					// GPIOx base+clear data out offset, TRM 2.1 & 25.4.1.25
      				addr_temp = SOC_GPIO_3_REGS + GPIO_CLEARDATAOUT;        			
					val_temp = 1<<pin;
				break;

				default:	
				break;

			}/* -----  end switch  ----- */
      		HWREG(addr_temp) &= val_temp;
   		}
	}
}/* -----  end of function gpioGetPinValue  ----- */

unsigned int GpioGetPinValue(gpioMod mod, ucPinNumber pin){
  	unsigned int addr_temp;
 
	if(check_module_gpio(mod) && check_pin_gpio(pin))
	{
		switch (mod) 
		{
			case GPIO0:
				addr_temp = SOC_GPIO_0_REGS + GPIO_DATAIN;        			
			break;

			case GPIO1:	
				addr_temp = SOC_GPIO_1_REGS + GPIO_DATAIN;        			
			break;

			case GPIO2:	
				addr_temp = SOC_GPIO_2_REGS + GPIO_DATAIN;        			
			break;

			case GPIO3:	
				addr_temp = SOC_GPIO_3_REGS + GPIO_DATAIN;        			
			break;

			default:	
			break;
		}	
		return (HWREG(addr_temp) & (1<<pin));
   	}else
    	return -1;  // isnt a valid mod/pin combination or doesnt exist
}/* -----  end of function gpiogetPinValue  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  disableWdt
 *  Description:  
 * =====================================================================================
 */

int gpioIsrHandler(gpioMod gpio, GroupInterrup type,ucPinNumber pin){
    //if((gpio < GPIO0 || gpio > GPIO3 ) || (type != 0 && type != 1)) return -1;

    switch (gpio)
    {
    case GPIO0:
        if(type == 0){ // group A
            HWREG(SOC_GPIO_0_REGS+GPIO_IRQSTATUS_0) = 1<<pin;
        }else // group B
            HWREG(SOC_GPIO_0_REGS+GPIO_IRQSTATUS_1) = 1<<pin;
        break;
    
    case GPIO1:
        if(type == 0){
            HWREG(SOC_GPIO_1_REGS+GPIO_IRQSTATUS_0) = 1<<pin;
        }else
            HWREG(SOC_GPIO_1_REGS+GPIO_IRQSTATUS_1) = 1<<pin;
        break;

    case GPIO2:
        if(type == 0){
            HWREG(SOC_GPIO_2_REGS+GPIO_IRQSTATUS_0) = 1<<pin;
        }else
            HWREG(SOC_GPIO_2_REGS+GPIO_IRQSTATUS_1) = 1<<pin;
        break;

    case GPIO3:
        if(type == 0){
            HWREG(SOC_GPIO_3_REGS+GPIO_IRQSTATUS_0) = 1<<pin;
        }else
            HWREG(SOC_GPIO_3_REGS+GPIO_IRQSTATUS_1) = 1<<pin;
        break;
    default:
        break;
    }
    return 0;
}

int Pin_Interrup_Config(gpioMod gpio, ucPinNumber pin, GroupInterrup type){
    //if((gpio < GPIO0 || gpio > GPIO3 ) || (pin < 0 || pin > 31 ) || (type != 0 && type != 1)) return -1;

    switch (gpio)
    {
    case GPIO0:
        if(type == 0){
            HWREG(SOC_GPIO_0_REGS + GPIO_IRQSTATUS_SET_0) |= (1<<pin);
            HWREG(SOC_GPIO_0_REGS + GPIO_RISINGDETECT) |= (1<<pin);
        }else{
            HWREG(SOC_GPIO_0_REGS + GPIO_IRQSTATUS_SET_1) |= (1<<pin);
            HWREG(SOC_GPIO_0_REGS + GPIO_RISINGDETECT) |= (1<<pin);
        }
        break;
    case GPIO1:
        if(type == 0){
            HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_SET_0) |= (1<<pin);
            HWREG(SOC_GPIO_1_REGS + GPIO_RISINGDETECT) |= (1<<pin);
        }else{
            HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_SET_1) |= (1<<pin);
            HWREG(SOC_GPIO_1_REGS + GPIO_RISINGDETECT) |= (1<<pin);
        }
        break;
    case GPIO2:
        if(type == 0){
            HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET_0) |= (1<<pin);
            HWREG(SOC_GPIO_2_REGS + GPIO_RISINGDETECT) |= (1<<pin);
        }else{
            HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET_1) |= (1<<pin);
            HWREG(SOC_GPIO_2_REGS + GPIO_RISINGDETECT) |= (1<<pin);
        }
        break;
    case GPIO3:
        if(type == 0){
            HWREG(SOC_GPIO_3_REGS + GPIO_IRQSTATUS_SET_0) |= (1<<pin);
            HWREG(SOC_GPIO_3_REGS + GPIO_RISINGDETECT) |= (1<<pin);
        }else{
            HWREG(SOC_GPIO_3_REGS + GPIO_IRQSTATUS_SET_1) |= (1<<pin);
            HWREG(SOC_GPIO_3_REGS + GPIO_RISINGDETECT) |= (1<<pin);
        }
        break;
    default:
        break;
    }

    return 1;
}