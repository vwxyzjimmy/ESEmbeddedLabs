#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "blink.h"
#include "reg.h"

#define HEAP_MAX (64 * 1024) //64 KB
void init_usart1(void)
{
	//PB6 PB7

	//RCC EN GPIOB
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

	//Baud
	const unsigned int BAUD = 115200;
	const unsigned int SYSCLK_MHZ = 168;
	const double USARTDIV = SYSCLK_MHZ * 1.0e6 / 16 / BAUD;

	const uint32_t DIV_MANTISSA = (int)(USARTDIV);
	const uint32_t DIV_FRACTION = (int)(((USARTDIV)-DIV_MANTISSA) *16);

	//USART  Configurations
	SET_BIT(USART1_BASE + USART_CR1_OFFSET, UE_BIT);
	WRITE_BITS(USART1_BASE + USART_BRR_OFFSET, DIV_MANTISSA_11_BIT, DIV_MANTISSA_0_BIT, DIV_MANTISSA);
	WRITE_BITS(USART1_BASE + USART_BRR_OFFSET, DIV_FRACTION_3_BIT, DIV_FRACTION_0_BIT, DIV_FRACTION);
	SET_BIT(USART1_BASE + USART_CR1_OFFSET, TE_BIT);
	SET_BIT(USART1_BASE + USART_CR1_OFFSET, RE_BIT);

}

void usart1_send_char(const char ch)
{
	while (!READ_BIT(USART1_BASE + USART_SR_OFFSET,TXE_BIT));
	REG(USART1_BASE + USART_DR_OFFSET) = ch;
}

char usart1_receive_char(void)
{
	while (!READ_BIT(USART1_BASE + USART_SR_OFFSET,RXNE_BIT));
	return(REG(USART1_BASE + USART_DR_OFFSET));
}

int main(void)
{
	init_usart1();
	int i = 75;
	printf("Decimal: %d  Hexadecimal: 0x%x \r\n", i, i);
	printf("Character: %c\r\n", i);
	char keyword[] = "Howdoyouturnthison";
	char input[255];
	while(1){
		scanf("%s", input);
		printf("%s\r\n", input);
		if ( strcmp(input, keyword) == 0){
			printf("Hello World\r\n");
		}
	}
	blink(LED_BLUE);
}

void *_sbrk(int incr)
{
	extern uint8_t _mybss_vma_end; //Defined by the linker script
	static uint8_t *heap_end = NULL;
	uint8_t *prev_heap_end;

	if (heap_end == NULL)
		heap_end = &_mybss_vma_end;

	prev_heap_end = heap_end;
	if (heap_end + incr > &_mybss_vma_end + HEAP_MAX)
		return (void *)-1;

	heap_end += incr;
	return (void *)prev_heap_end;
}

int _write(int file, char *ptr, int len)
{
	for (unsigned int i = 0; i < len; i++)
		usart1_send_char(*ptr++);
	return len;
}

int _close(int file)
{
	return -1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _read(int file, char *ptr, int len)
{
	size_t input_len = 0;
	while(*(ptr-1) != 0x0d){
		*ptr++ = usart1_receive_char();
		input_len++;
		if (*(ptr-1) == 0x08){
			usart1_send_char(8);
			usart1_send_char(32);
			usart1_send_char(8);
			if(input_len > 1){
				input_len = input_len - 2;
			}
			else if (input_len == 1){
				input_len = 0;
			}
		}
		else{
			usart1_send_char(*(ptr-1));
		}
	}
	usart1_send_char('\n');
	return input_len;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}
