/*
 * LED74H595.cpp
 *
 *  Created on: 11 окт. 2018 г.
 *  Author: Vladimir Loskutnikov
 */
#include "stm32f0xx.h"
#include "stm32f0xx_syscfg.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_tim.h"
#include <LED74H595.h>

//======== LEDs bits
const uint16_t DIGIT[4]={0x1000,0x2000,0x4000,0x8000};
//======== chars table
const unsigned char LED[]={
		  _0,
		  _1,
		  _2,
		  _3,
		  _4,
		  _5,
		  _6,
		  _7,
		  _8,
		  _9,
		  _A,
		  _b,
		  _C,
		  _d,
		  _E,
		  _F
};

volatile uint8_t d_number;
volatile static uint8_t point_pos;
volatile uint8_t d[4];

void timer3_init();

void init74H595Display(void){
      GPIO_InitTypeDef GPIO_InitStructure;
      SPI_InitTypeDef  SPI_InitStructure;
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

      GPIO_StructInit(&GPIO_InitStructure);
      // PIN RCLK
	  GPIO_InitStructure.GPIO_Pin = RCLK_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  GPIO_ResetBits(GPIOA,RCLK_PIN);

	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN|SPIx_MOSI_PIN;
	  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

	 /* Enable the SPI peripheral */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	 /* SPI pin mappings */
	  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF); //
	  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);

	 /* SPI configuration -------------------------------------------------------*/
	  SPI_I2S_DeInit(SPIx);
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	  SPI_InitStructure.SPI_DataSize = SPI_DATASIZE;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
	  SPI_Cmd(SPIx, ENABLE);
	  SPI_Init(SPIx, &SPI_InitStructure);
	  timer3_init();
	  set_dp(0);
}

void printInt(int n, uint8_t showZero){
	  int n1, n2, n3, n4;
	  n1 = (int)  n % 10;
	  n2 = (int) ((n % 100)-n1)/10;
	  n3 = (int) ((n % 1000) - n2 - n1) / 100;
	  n4 = (int) ((n % 10000) - n3 - n2 - n1) / 1000;
	  set_num(n1, 3);
	  if(showZero || n>9)   set_num(n2, 2);
	  if(showZero || n>99)  set_num(n3, 1);
	  if(showZero || n>999) set_num(n4, 0);
}

void clear_display(void){
	d[0]=0;
	d[1]=0;
	d[2]=0;
	d[3]=0;
}

void timer3_init(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef  timerStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructInit(&timerStructure);
    timerStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock  / 1000000UL) - 1u;
    timerStructure.TIM_Period = (1000000UL + (REFRESH_FREQUENCY/2))/REFRESH_FREQUENCY;
    TIM_TimeBaseInit(TIM3, &timerStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	NVIC_EnableIRQ(TIM3_IRQn);
}

void RCLK_impulse(){
	GPIO_SetBits(GPIOA,RCLK_PIN);
	GPIO_ResetBits(GPIOA,RCLK_PIN);
}

void TIM3_IRQHandler()
{
	if (d_number%2){
	  RCLK_impulse();
	}
	else {
		SPI_I2S_SendData16(SPIx, DIGIT[(d_number>>1)]|(0x00FFu & (~(d[(d_number>>1)]|((point_pos==(d_number>>1))?1:0)))));
	}
	d_number = (d_number+1) & 7;
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void set_dp(uint8_t pos){ //0-3
	if (pos==0) point_pos=8;
	else point_pos =pos^3;
}

inline void set_byte(uint8_t b,uint8_t pos){
	d[pos]=b;
}

void set_num(uint8_t num,uint8_t pos){
	set_byte(LED[num],pos);
}

void write_bytes(uint8_t b0,uint8_t b1,uint8_t b2,uint8_t b3,uint8_t pointPos){
	set_byte(b0,0);
	set_byte(b1,1);
	set_byte(b2,2);
	set_byte(b3,3);
	set_dp(pointPos);
}
