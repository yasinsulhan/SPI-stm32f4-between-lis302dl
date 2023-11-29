#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

int8_t x, y, z;
uint8_t x_address = 0x29, y_address = 0x2B, z_address = 0x2D;

int8_t n_y;

void RCC_Config()
{
	RCC->CR |= 0x00010000;			// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;			// CSS Enable
	RCC->CR |= 0x01000000;			//pll enable
	RCC->PLLCFGR |= 0x00400000;		// PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;		// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;		// Pll N = 168
	RCC->PLLCFGR |= 0x00000000;		// PLL p = 2
	RCC->CFGR |= 0x00000000;		// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;		// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;		// APB1 Prescaler = 4
	RCC->CIR |= 0x00800000;			// CSS Flag clear
}

//A,E aktif olmalý.
void GPIO_Config()
{

	RCC->AHB1ENR |= (1 << 0);								// AHB1 A pin enabled
	GPIOA->MODER |= (2 << 10) | (2 << 12) | (2 << 14);		// A5-A6-A7 Alternate function ?
	//GPIOA->OTYPER |= (0 << 5) | (0 << 6) | (0 << 7);		// A3-A4-A5 Output push-pull
	GPIOA->OSPEEDR |= (2 << 10) | (2 << 12) | (2 << 14);	// A5-A6-A7 High speed
	//GPIOA->PUPDR |= (2 << 10) | (2 << 12) | (2 << 14);	// A5-A6-A7 Pull Down ?
	GPIOA->AFR[0] |= (5 << 20) | (5 << 24) | (5 << 28);		// A5-A6-A7 Alternate function bit set

	RCC->AHB1ENR |= (1 << 4);								// AHB1 E pin enabled
	GPIOE->MODER |= (1 << 6);								// E3 Output
	//GPIOE->OTYPER |= (0 << 3) | (0 << 4) | (0 <<5);		// A3-A4-A5 Output push-pull ?
	GPIOE->OSPEEDR |= (2 << 6);								// A3 High Speed
	//GPIOE->PUPDR |= (2 << 6) | (2 << 8) | (2 << 10);		// A3-A4-A5 Pull Down ?


}

void SPI_Config()
{
	RCC->APB2ENR |= (1 << 12);								// SPI1 Clock Enable
	SPI1->CR1 |= (0 << 3);									// fPCLK/2 -> 84/2 = 42MHz
	SPI1->CR1 |= (1 << 0);									// Second clock transition first data capture edge.
	//SPI1->CR1 |= (1 << 0);
	SPI1->CR1 |= (0 << 11);									// 8 bit data
	SPI1->CR1 |= (0 << 10);									// Full duplex (Transmit and receive)
	SPI1->CR1 |= (0 << 7);									// MSB priority
	SPI1->CR1 |= (1 << 2);									// Master mode
	SPI1->CR1 |= (1 << 9) | (1 << 8);						// Software slave management enabled														// It ýs set when SSM is set
	SPI1->CR1 |= (1 << 6);									// SPI Enable

	GPIOE->ODR |= (1 << 3);
	//GPIOE->ODR &= ~(1 << 3); //3. pin reset

}

void  SPI_Write(uint8_t address, uint8_t data)
{

	GPIOE->ODR &= ~(1 << 3);
	while (!((SPI1->SR)&(1<<1)));  							// wait for TXE bit to set -> This will indicate that the buffer is empty
	SPI1->DR = address;
	while (!((SPI1->SR) &(1<<0)));							// Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	SPI1->DR;

	while (!((SPI1->SR)&(1<<1)));  							// wait for TXE bit to set -> This will indicate that the buffer is empty
	SPI1->DR = data;
	while (!((SPI1->SR) &(1<<0)));							// // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	SPI1->DR;
	GPIOE->ODR |= (1 << 3);



}

uint8_t SPI_Read(uint8_t address, uint8_t data)
{
	GPIOE->ODR &= ~(1 << 3);

	address = address | 0x80;	// okuma islemi yapilacak

	while (!((SPI1->SR)&(1<<1)));  							// wait for TXE bit to set -> This will indicate that the buffer is empty
	SPI1->DR = address;
	while (!((SPI1->SR) &(1<<0)));							// // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	SPI1->DR;
	while (!((SPI1->SR)&(1<<1)));  							// wait for TXE bit to set -> This will indicate that the buffer is empty
	SPI1->DR = data;
	while (!((SPI1->SR) &(1<<0)));							// // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	SPI1->DR;
	GPIOE->ODR |= (1 << 3);
	return 	SPI1->DR;
}



int main(void)
{
  GPIO_Config();
  SPI_Config();

  SPI_Write(0x20, 0x47);		// CTRL_REG1 100Hz +-8g
  while (1)
  {
	  x = SPI_Read(x_address, 0x00);
	  y = SPI_Read(y_address, 0x00);
	  z = SPI_Read(z_address, 0x00);

	  n_y = Convert_To_Val(y);



  }
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){

  return;
}


uint16_t EVAL_AUDIO_GetSampleCallBack(void){

  return -1;
}





