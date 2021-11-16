/*
 * LED74H595.h
 * Author: Vladimir Loskutnikov
 *
 * stm32 lib for "4-Bit LED Digital Tube Module" based on TIMER3 & SPI
 *
 *********************************************************************
 * ----------EXAMPLE-------------------
#define  STM32f030
#include "stm32f0xx.h"
#include "LED74H595.h"

int main(int argc, char* argv[])
{
  init74H595Display();
  while(1){
		for (int i=0;i<2000;i++){
			clear_display();
			printInt(i,0);
			if (i>1000)
				set_dp(3);
			else
				if (i>=100){
					set_dp(2);
				}
				else {
					if (i>=10) 	set_dp(1);
					else		set_dp(0);
				}
			for (long t=0;t<1000000;t++);
			write_bytes(_H,_E,_L,_P, 0);
			for (long t=0;t<1000000;t++);
		}
	}
}
 */
#ifndef mLED74H595_H_
#define mLED74H595_H_

//===========settings=======================
#define REFRESH_FREQUENCY 500UL
//========= pins definition (for stm32030f)
// PA6 RCLK,
#define RCLK_PIN 						 GPIO_Pin_6
// SPI1 used
#define SPIx                             SPI1
#define SPI_DATASIZE                     SPI_DataSize_16b
#define SPIx_CLK                         RCC_APB2Periph_SPI1
#define SPIx_IRQn                        SPI1_IRQn
#define SPIx_IRQHandler                  SPI1_IRQHandler

// PA5 SPI1_SCK,
#define SPIx_SCK_PIN                     GPIO_Pin_5
#define SPIx_SCK_GPIO_PORT               GPIOA
#define SPIx_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOA
#define SPIx_SCK_SOURCE                  GPIO_PinSource5
#define SPIx_SCK_AF                      GPIO_AF_0

// PA7 SPI1_MOSI
#define SPIx_MOSI_PIN                    GPIO_Pin_7
#define SPIx_MOSI_GPIO_PORT              GPIOA
#define SPIx_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define SPIx_MOSI_SOURCE                 GPIO_PinSource7
#define SPIx_MOSI_AF                     GPIO_AF_0

//======LED-segments bit definition=======
#define sDP 0x01  // dp (decimal point)
#define sG  0x02  // G middle
#define sF  0x04  // F top left
#define sE  0x08  // E bottom left
#define sD  0x10  // D bottom
#define sC  0x20  // C bottom right
#define sB  0x40  // B top right
#define sA  0x80  // A top

#define _0		  (sA|sB|sC|sD|sE|sF|0)
#define _1		  (0 |sB|sC)
#define _2		  (sA|sB|0 |sD|sE| 0|sG) //2
#define _3		  (sA|sB|sC|sD| 0| 0|sG) //3
#define _4		  (0 |sB|sC| 0| 0|sF|sG) //4
#define _5		  (sA| 0|sC|sD| 0|sF|sG) //5
#define _6		  (sA| 0|sC|sD|sE|sF|sG) //6
#define _7		  (sA|sB|sC| 0| 0| 0| 0) //7
#define _8		  (sA|sB|sC|sD|sE|sF|sG) //8
#define _9		  (sA|sB|sC|sD| 0|sF|sG) //9
#define _A		  (sA|sB|sC| 0|sE|sF|sG) //A
#define _b		  (0 | 0|sC|sD|sE|sF|sG) //b
#define _C		  (sA| 0| 0|sD|sE|sF|0 ) //C
#define _d		  (0 |sB|sC|sD|sE| 0|sG) //d
#define _E		  (sA| 0| 0|sD|sE|sF|sG) //E
#define _F		  (sA| 0| 0| 0|sE|sF|sG) //F
#define _G 		  (sA| 0|sC|sD|sE|sF|sG)
#define _H 		  ( 0|sB|sC| 0|sE|sF|sG)
#define _L 		  ( 0| 0| 0|sD|sE|sF| 0)
#define _P 		  (sA|sB| 0| 0|sE|sF|sG)
#define _o 		  ( 0| 0|sC|sD|sE| 0|sG)
#define _U 		  ( 0|sB|sC|sD|sE|sF|sG)
#define _SPACE	  (0)
#define _MINUS	  (sG)

#ifdef __cplusplus
 extern "C" {
#endif
	extern void init74H595Display(void);
	extern void clear_display();//turn off all LEDs
	extern void printInt(int n, uint8_t showZero);
	extern void set_dp(uint8_t pos); //0-3
	extern void set_num(uint8_t num,uint8_t pos); 	//set number into pos(0-3)
	extern void set_byte(uint8_t b,uint8_t pos);    //set byte into pos
	extern void write_bytes(uint8_t b0,uint8_t b1,uint8_t b2,uint8_t b3,uint8_t pointPos);
#ifdef __cplusplus
}
#endif
#endif /* mLED74H595_H_ */
