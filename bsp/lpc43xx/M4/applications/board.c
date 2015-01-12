/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2014 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      first implementation
 * 2014-06-20     xiaonong     ported to LPC43xx
 */

#include <rthw.h>
#include <rtthread.h>

#include "board.h"
#include "drv_uart.h"


/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

extern void SystemCoreClockUpdate(void);
/**
 * This function will initial LPC43xx board.
 */
void rt_hw_board_init()
{
#ifdef CORE_M4
    /* NVIC Configuration */
#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x10000000 */
    SCB->VTOR  = 0x10000000;
#else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x00000000 */
    SCB->VTOR  = 0x1A000000;
#endif
#endif
    /* update the core clock */
    SystemCoreClockUpdate();

    /* init systick */
    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    /* set pend exception priority */
    NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

    /* init uart device */
    rt_hw_uart_init();

#ifdef RT_USING_COMPONENTS_INIT
    /* initialization board with RT-Thread Components */
    rt_components_board_init();
#endif

    /* setup the console device */
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
}

#ifdef RT_USING_RTGUI
#include <rtgui/driver.h>
#include "drv_lcd.h"

/* initialize for gui driver */
int rtgui_lcd_init(void)
{
    rt_device_t device;

    rt_hw_lcd_init();

    device = rt_device_find("lcd");
    /* set graphic device */
    rtgui_graphic_set_device(device);

    return 0;
}
INIT_DEVICE_EXPORT(rtgui_lcd_init);
#endif

