/*
 * Minimal register map for STM32F407VG (peripherals used by this project only).
 * Hand-written on purpose so the project builds with nothing but arm-none-eabi-gcc
 * and doesn't depend on CMSIS/HAL being installed yet.
 * Addresses/bit positions are from RM0090 (STM32F405/415, F407/417, F427/437, F429/439).
 */
#ifndef STM32F407_REGS_H
#define STM32F407_REGS_H

#include <stdint.h>

#define __IO volatile

/* ---- Core peripherals (Cortex-M4, same on every Cortex-M4 part) ---- */
typedef struct {
    __IO uint32_t CTRL;
    __IO uint32_t LOAD;
    __IO uint32_t VAL;
    __IO uint32_t CALIB;
} SysTick_TypeDef;
#define SysTick ((SysTick_TypeDef *)0xE000E010UL)

#define SysTick_CTRL_ENABLE_Msk    (1UL << 0)
#define SysTick_CTRL_CLKSOURCE_Msk (1UL << 2)
#define SysTick_CTRL_COUNTFLAG_Msk (1UL << 16)

/* ---- RCC ---- */
typedef struct {
    __IO uint32_t CR;
    __IO uint32_t PLLCFGR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t AHB1RSTR;
    __IO uint32_t AHB2RSTR;
    __IO uint32_t AHB3RSTR;
    uint32_t      RESERVED0;
    __IO uint32_t APB1RSTR;
    __IO uint32_t APB2RSTR;
    uint32_t      RESERVED1[2];
    __IO uint32_t AHB1ENR;
    __IO uint32_t AHB2ENR;
    __IO uint32_t AHB3ENR;
    uint32_t      RESERVED2;
    __IO uint32_t APB1ENR;
    __IO uint32_t APB2ENR;
} RCC_TypeDef;
#define RCC ((RCC_TypeDef *)0x40023800UL)

#define RCC_AHB1ENR_GPIOAEN (1UL << 0)
#define RCC_AHB1ENR_GPIODEN (1UL << 3)
#define RCC_APB1ENR_USART2EN (1UL << 17)

/* ---- GPIO ---- */
typedef struct {
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;
    __IO uint32_t PUPDR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t LCKR;
    __IO uint32_t AFR[2]; /* AFR[0] = AFRL (pins 0-7), AFR[1] = AFRH (pins 8-15) */
} GPIO_TypeDef;
#define GPIOA ((GPIO_TypeDef *)0x40020000UL)
#define GPIOD ((GPIO_TypeDef *)0x40020C00UL)

/* ---- USART ---- */
typedef struct {
    __IO uint32_t SR;
    __IO uint32_t DR;
    __IO uint32_t BRR;
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t CR3;
    __IO uint32_t GTPR;
} USART_TypeDef;
#define USART2 ((USART_TypeDef *)0x40004400UL)

#define USART_SR_TXE  (1UL << 7)
#define USART_SR_TC   (1UL << 6)
#define USART_CR1_RE  (1UL << 2)
#define USART_CR1_TE  (1UL << 3)
#define USART_CR1_UE  (1UL << 13)

#endif /* STM32F407_REGS_H */
