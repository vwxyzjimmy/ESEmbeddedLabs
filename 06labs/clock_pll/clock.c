#include <stdint.h>
#include "reg.h"

/**
 * 
 * output sysclk (PC9)
 * 
 */
void op_sysclk(unsigned int div)
{
	//RCC
	CLEAR_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2_1_BIT);
	CLEAR_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2_0_BIT);

	if (div == 1)
		CLEAR_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2PRE_2_BIT);

	else if (div >= 2 && div <= 5)
	{
		SET_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2PRE_2_BIT);
		REG(RCC_BASE + RCC_CFGR_OFFSET) = (REG(RCC_BASE + RCC_CFGR_OFFSET) & ~(((uint32_t)0b11) << (MCO2PRE_0_BIT))) | ((uint32_t)(div - 2) << (MCO2PRE_0_BIT));
	}
	else
		while (1)
			;

	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTC));

	//GPIO
	SET_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_MODER_OFFSET, MODERy_1_BIT(9));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_MODER_OFFSET, MODERy_0_BIT(9));

	//Output push-pull
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_OTYPER_OFFSET, OTy_BIT(9));

	//OSPEEDR15 = 11 => Very high speed
	SET_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(9));
	SET_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(9));

	//PUPDR15 = 00 => No pull-up, pull-down
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(9));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(9));
}

/**
 * 
 * set sysclk pll (168 MHz)
 * 
 */
void set_sysclk_pll(void)
{
	//enable HSE
	SET_BIT(RCC_BASE + RCC_CR_OFFSET, HSEON_BIT );

	//wait
	while (READ_BIT(RCC_BASE + RCC_CR_OFFSET, HSERDY_BIT) != 1)
		;

	//set pll
  //use HSE for PLL source
	SET_BIT(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLSRC_BIT);
	//f_HSE = 8 MHz
	//
	//N = 168
	//M = 4
	//
	//f_VCO = 8 * 168 / 4 = 168 * 2
	//
	//P = 2
	//
	//f_PLL_out = 168
	//
	WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLP_1_BIT, PLLP_0_BIT, 0b00);
	WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLN_8_BIT, PLLN_0_BIT, 168);
	WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLM_5_BIT, PLLM_0_BIT, 4);

	//enable pll
	SET_BIT(RCC_BASE + RCC_CR_OFFSET, PLLON_BIT );

	//wait
	while(!(READ_BIT(RCC_BASE + RCC_CR_OFFSET, PLLRDY_BIT)));

	//enable flash prefetch buffer
	SET_BIT(FLASH_BASE + FLASH_ACR_OFFSET, PRFTEN_BIT);

	//set flash wait state = 5
	WRITE_BITS(FLASH_BASE + FLASH_ACR_OFFSET, LATENCY_2_BIT, LATENCY_0_BIT, 5);
	

	//use pll
/*
	SET_BIT(RCC_BASE + RCC_CFGR_OFFSET, SW_1_BIT);
	CLEAR_BIT(RCC_BASE + RCC_CFGR_OFFSET, SW_0_BIT);
*/
	WRITE_BITS(RCC_BASE + RCC_CFGR_OFFSET,SW_1_BIT, SW_0_BIT,2 );
	//wait
	while(!((READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_1_BIT) == 1) && (READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_0_BIT) == 0)));
}
