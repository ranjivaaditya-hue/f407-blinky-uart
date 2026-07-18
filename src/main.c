/*
 * Blinky + UART status print for the STM32F407G-DISC1 (STM32F4-Discovery) board.
 *
 * On-board LEDs (all on GPIOD):
 *   PD12 green (LD4), PD13 orange (LD3), PD14 red (LD5), PD15 blue (LD6)
 *
 * UART: USART2, PA2 = TX, PA3 = RX, 115200 8N1.
 * The Discovery board's on-board ST-Link does NOT bridge this UART to a USB
 * virtual COM port (that's a Nucleo-only feature) - you need an external
 * USB-to-TTL serial adapter wired to PA2/GND to see this in a serial monitor.
 * See README.md for wiring and the VS Code Serial Monitor setup.
 *
 * Clock: runs on the default 16 MHz internal HSI oscillator (no PLL setup),
 * which is plenty for a blink demo and keeps this file self-contained.
 */
#include "stm32f407_regs.h"

#define HSI_HZ   16000000UL
#define BAUD     115200UL

static void delay_ms(uint32_t ms)
{
    SysTick->LOAD = (HSI_HZ / 1000) - 1;
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    for (uint32_t i = 0; i < ms; i++) {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {
        }
    }
    SysTick->CTRL = 0;
}

static void leds_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    /* PD12..PD15 as general purpose output (MODER = 01), push-pull, no pull */
    GPIOD->MODER &= ~((3UL << (12 * 2)) | (3UL << (13 * 2)) | (3UL << (14 * 2)) | (3UL << (15 * 2)));
    GPIOD->MODER |=  ((1UL << (12 * 2)) | (1UL << (13 * 2)) | (1UL << (14 * 2)) | (1UL << (15 * 2)));
}

static void leds_set(uint8_t on)
{
    if (on) {
        GPIOD->BSRR = (1UL << 12) | (1UL << 13) | (1UL << 14) | (1UL << 15);
    } else {
        GPIOD->BSRR = (1UL << (12 + 16)) | (1UL << (13 + 16)) | (1UL << (14 + 16)) | (1UL << (15 + 16));
    }
}

static void usart2_init(void)
{
    RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR  |= RCC_APB1ENR_USART2EN;

    /* PA2/PA3 -> alternate function mode (MODER = 10) */
    GPIOA->MODER &= ~((3UL << (2 * 2)) | (3UL << (3 * 2)));
    GPIOA->MODER |=  ((2UL << (2 * 2)) | (2UL << (3 * 2)));

    /* AF7 = USART2 on PA2/PA3 */
    GPIOA->AFR[0] &= ~((0xFUL << (2 * 4)) | (0xFUL << (3 * 4)));
    GPIOA->AFR[0] |=  ((7UL   << (2 * 4)) | (7UL   << (3 * 4)));

    /* high speed for the TX pin */
    GPIOA->OSPEEDR |= (3UL << (2 * 2)) | (3UL << (3 * 2));

    /* BRR = round(PCLK1 / baud); PCLK1 = HSI = 16 MHz (no prescaling at reset) */
    USART2->BRR = (uint16_t)((HSI_HZ + (BAUD / 2)) / BAUD);
    USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

static void usart2_write_char(char c)
{
    while (!(USART2->SR & USART_SR_TXE)) {
    }
    USART2->DR = c;
}

static void usart2_write_string(const char *s)
{
    while (*s) {
        usart2_write_char(*s++);
    }
}

int main(void)
{
    leds_init();
    usart2_init();

    usart2_write_string("\r\n--- STM32F407 Discovery blinky ---\r\n");

    uint8_t led_on = 0;
    for (;;) {
        led_on = !led_on;
        leds_set(led_on);
        usart2_write_string(led_on ? "LED state: ON\r\n" : "LED state: OFF\r\n");
        delay_ms(500);
    }
}
