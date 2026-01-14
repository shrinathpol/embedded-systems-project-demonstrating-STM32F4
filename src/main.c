#include "stm32f4xx.h"
#include <stdio.h>

volatile uint16_t adc_raw_value = 0;
volatile uint8_t dma_transfer_complete = 0;

void GPIO_Init(void);
void DMA_Init(void);
void ADC_Init(void);
void TIM2_Init(void);
void UART1_Init(void);
void UART1_SendString(char* str);

int main(void) {
    GPIO_Init();
    DMA_Init();
    ADC_Init();
    UART1_Init();
    TIM2_Init();

    __enable_irq();
    TIM2->CR1 |= TIM_CR1_CEN;

    char msg_buffer[64];

    while (1) {
        if (dma_transfer_complete) {
            dma_transfer_complete = 0;

            uint32_t voltage_mv = (adc_raw_value * 3300) / 4095;
            uint32_t val_whole = voltage_mv / 1000;
            uint32_t val_dec = voltage_mv % 1000;

            sprintf(msg_buffer, "ADC: %4d | Volts: %lu.%03lu V\r\n", 
                    adc_raw_value, val_whole, val_dec);

            UART1_SendString(msg_buffer);
            GPIOC->ODR ^= (1 << 13);
        }
    }
}

void DMA2_Stream0_IRQHandler(void) {
    if (DMA2->LISR & DMA_LISR_TCIF0) {
        DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
        dma_transfer_complete = 1;
    }
}

void GPIO_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

    GPIOA->MODER |= (3U << (0 * 2));
    GPIOA->MODER |= (2U << (9 * 2));
    GPIOA->AFR[1] |= (7U << 4);

    GPIOC->MODER &= ~(3U << (13 * 2));
    GPIOC->MODER |= (1U << (13 * 2));
}

void DMA_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    DMA2_Stream0->CR = 0;
    while(DMA2_Stream0->CR & DMA_SxCR_EN);

    DMA2_Stream0->CR |= (1U << 8) | (1U << 13) | (1U << 11) | (1U << 4);
    DMA2_Stream0->NDTR = 1;
    DMA2_Stream0->PAR = (uint32_t)&(ADC1->DR);
    DMA2_Stream0->M0AR = (uint32_t)&adc_raw_value;

    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
    DMA2_Stream0->CR |= DMA_SxCR_EN;
}

void ADC_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    ADC1->CR2 |= (1U << 28);
    ADC1->CR2 |= (6U << 24);
    ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_DDS;

    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;

    ADC1->CR2 |= ADC_CR2_ADON;
}

void TIM2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = 1600 - 1;
    TIM2->ARR = 100 - 1;

    TIM2->CR2 &= ~TIM_CR2_MMS;
    TIM2->CR2 |= (2U << 4);
}

void UART1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    USART1->BRR = 0x8B;
    USART1->CR1 |= USART_CR1_TE | USART_CR1_UE;
}

void UART1_SendString(char* str) {
    while(*str) {
        while(!(USART1->SR & USART_SR_TXE));
        USART1->DR = *str++;
    }
}