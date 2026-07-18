/*
 * Minimal startup file for STM32F407VG (Cortex-M4).
 * Only the 16 core Cortex-M exception vectors are filled in - this project
 * never enables a peripheral (NVIC) interrupt, so the STM32-specific IRQ
 * table entries after them are never reached and are intentionally omitted.
 */
    .syntax unified
    .cpu cortex-m4
    .fpu fpv4-sp-d16
    .thumb

    .global Reset_Handler
    .global _estack

    .section .isr_vector, "a", %progbits
    .type g_pfnVectorTable, %object
g_pfnVectorTable:
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word DebugMon_Handler
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler
    .size g_pfnVectorTable, . - g_pfnVectorTable

    .section .text.Reset_Handler
    .weak Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata
copy_data:
    cmp r1, r2
    bge copy_data_done
    ldr r3, [r0]
    str r3, [r1]
    adds r0, r0, #4
    adds r1, r1, #4
    b copy_data
copy_data_done:

    ldr r0, =_sbss
    ldr r1, =_ebss
    movs r2, #0
zero_bss:
    cmp r0, r1
    bge zero_bss_done
    str r2, [r0]
    adds r0, r0, #4
    b zero_bss
zero_bss_done:

    bl main
    b .
    .size Reset_Handler, . - Reset_Handler

    .section .text.Default_Handler, "ax", %progbits
Default_Handler:
    b .
    .size Default_Handler, . - Default_Handler

    .macro def_irq_handler handler_name
    .weak \handler_name
    .set \handler_name, Default_Handler
    .endm

    def_irq_handler NMI_Handler
    def_irq_handler HardFault_Handler
    def_irq_handler MemManage_Handler
    def_irq_handler BusFault_Handler
    def_irq_handler UsageFault_Handler
    def_irq_handler SVC_Handler
    def_irq_handler DebugMon_Handler
    def_irq_handler PendSV_Handler
    def_irq_handler SysTick_Handler
