#include <stdint.h>
#include "reg.h"

void init_usart1(void)
{
	//PB6 PB7

	//RCC EN GPIOB
<<<<<<< HEAD
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTB));

	//GPIO Configurations
	SET_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_MODER_OFFSET, MODERy_1_BIT(6));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_MODER_OFFSET, MODERy_0_BIT(6));

	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OTYPER_OFFSET, OTy_BIT(6));

	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(6));
	SET_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(6));

	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(6));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(6));

	SET_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_MODER_OFFSET, MODERy_1_BIT(7));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_MODER_OFFSET, MODERy_0_BIT(7));

	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OTYPER_OFFSET, OTy_BIT(7));

	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(7));
	SET_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(7));

	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(7));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(7));

	WRITE_BITS(GPIO_BASE(GPIO_PORTB) + GPIOx_AFRL_OFFSET, AFRLy_3_BIT(7), AFRLy_0_BIT(7), 7);
	WRITE_BITS(GPIO_BASE(GPIO_PORTB) + GPIOx_AFRL_OFFSET, AFRLy_3_BIT(6), AFRLy_0_BIT(6), 7);

	//RCC EN USART1
	SET_BIT(RCC_BASE + RCC_APB2ENR_OFFSET, USART1EN);
=======
	??????

	//GPIO Configurations
	??????

	//RCC EN USART1
	??????
>>>>>>> fc4395a573416d2589bb8d96291d3d6a142e7e8f

	//Baud
	const unsigned int BAUD = 115200;
	const unsigned int SYSCLK_MHZ = 168;
	const double USARTDIV = SYSCLK_MHZ * 1.0e6 / 16 / BAUD;

<<<<<<< HEAD
	const uint32_t DIV_MANTISSA = (int)(USARTDIV);
	const uint32_t DIV_FRACTION = (int)(((USARTDIV)-DIV_MANTISSA) *16);

	//USART  Configurations
	SET_BIT(USART1_BASE + USART_CR1_OFFSET, UE_BIT);
	WRITE_BITS(USART1_BASE + USART_BRR_OFFSET, DIV_MANTISSA_11_BIT, DIV_MANTISSA_0_BIT, DIV_MANTISSA);
	WRITE_BITS(USART1_BASE + USART_BRR_OFFSET, DIV_FRACTION_3_BIT, DIV_FRACTION_0_BIT, DIV_FRACTION);
	SET_BIT(USART1_BASE + USART_CR1_OFFSET, TE_BIT);
	SET_BIT(USART1_BASE + USART_CR1_OFFSET, RE_BIT);
=======
	const uint32_t DIV_MANTISSA = ??????
	const uint32_t DIV_FRACTION = ??????

	//USART  Configurations
	??????
>>>>>>> fc4395a573416d2589bb8d96291d3d6a142e7e8f

}

void usart1_send_char(const char ch)
{
<<<<<<< HEAD
	while (!READ_BIT(USART1_BASE + USART_SR_OFFSET,TXE_BIT));
	REG(USART1_BASE + USART_DR_OFFSET) = ch;
=======
	??????
>>>>>>> fc4395a573416d2589bb8d96291d3d6a142e7e8f
}

char usart1_receive_char(void)
{
<<<<<<< HEAD
	while (!READ_BIT(USART1_BASE + USART_SR_OFFSET,RXNE_BIT));
	return(REG(USART1_BASE + USART_DR_OFFSET));
=======
	??????
>>>>>>> fc4395a573416d2589bb8d96291d3d6a142e7e8f
}

int main(void)
{
	init_usart1();

	char *hello = "Hello world!\r\n";

	//send Hello world
	while (*hello != '\0')
		usart1_send_char(*hello++);


	//receive char and resend it
	char ch;
	while (1)
	{
		ch = usart1_receive_char();

		if (ch == '\r')
			usart1_send_char('\n');

		usart1_send_char(ch);
	}
}
