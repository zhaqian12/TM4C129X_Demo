/* Copyright 2021 ZhaQian
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ws2812.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"

// 定义系统主频时钟
#ifndef SYSCLK
#define SYSCLK (120000000) // 120M
#endif

// 定义ws2812 strip 灯数,默认为测试板16颗
#ifndef DRIVER_LED_TOTAL
#define DRIVER_LED_TOTAL 16
#endif

// 定义ws2812pwm频率,手册为800k,但实际测试80k才能正常工作
#define WS2812_PWM_FREQUENCY 80000 // 80k

// 定义ws2812pwm load值,如使用PWM模块无需减1,库函数进行处理
#define WS2812_PWM_PERIOD (SYSCLK / WS2812_PWM_FREQUENCY) // 1250ns

// 定义0码与1码的高低电平时间
#define T1H 800     // T1H:800ns
#define T1L 450     // T1L:450ns
#define T0H 350     // T0H:350ns
#define T0L 900     // T0L:900ns

// 定义RST时间
#define RST 100000  // RESET:100us(>50us)

// 定义所需RST的位数
#define WS2812_RESET_BIT_NUM (RST / 1250)

// 定义所需颜色的位数，一个灯为 3 X 8 bits
#define WS2812_COLOR_BIT_NUM (DRIVER_LED_TOTAL * 24) 

// 定义所需总位数
#define WS2812_BIT_NUM (WS2812_COLOR_BIT_NUM + WS2812_RESET_BIT_NUM)

// 定义0码占空比
#define WS2812_DUTYCYCLE_0 (SYSCLK / (1000000000 / T0H))

// 定义1码占空比
#define WS2812_DUTYCYCLE_1 (SYSCLK / (1000000000 / T1H))  

// 定义RST码占空比
#define WS2812_DUTYCYCLE_RST 0

// 计算位所在位置,编码方式为GRB
#define WS2812_BIT(led, byte, bit)  (24 * (led) + 8 * (byte) + (7 - (bit)))

// 定义位数据缓冲区
static uint16_t ws2812_frame_buffer[WS2812_BIT_NUM];

// 定义PWM中断计数器
static uint16_t timer = 0;

// 定义PWM使能标志
static uint8_t pwm_enable = 0;

/**
 * @brief PWM中断
 * 
 */
void PWM_GEN_3_INT(void)
{
    unsigned long Status;
    Status = MAP_PWMGenIntStatus(PWM0_BASE, PWM_GEN_3, true);
    if (Status == PWM_INT_CNT_ZERO)
    {
        if (ws2812_frame_buffer[timer] == WS2812_DUTYCYCLE_RST)
        {
            MAP_PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, false);
            pwm_enable = 1;
        }
        else
        {
            if (pwm_enable == 1)
            {
                MAP_PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
                pwm_enable ^= 1;
            }
            MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, ws2812_frame_buffer[timer]);
        }
        timer ++;
        if (timer >= WS2812_BIT_NUM)
            timer = 0;
    }
    MAP_PWMGenIntClear(PWM0_BASE, PWM_GEN_3, PWM_INT_CNT_ZERO);
}

/**
 * @brief 初始化GPIO引脚
 * 
 */
void ws2812_pin_init(void) 
{ 
    GPIOPinTypePWM(GPIO_PORTK_BASE, GPIO_PIN_4);
}

/**
 * @brief 初始化PWM模块及中断
 * 
 */
void ws2812_pwm_init(void) 
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    MAP_GPIOPinConfigure(GPIO_PK4_M0PWM6);
    MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    MAP_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, WS2812_PWM_PERIOD);
    MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, WS2812_DUTYCYCLE_0);
    MAP_PWMGenIntTrigEnable(PWM0_BASE, PWM_GEN_3, PWM_INT_CNT_ZERO);
    PWMGenIntRegister(PWM0_BASE, PWM_GEN_3, PWM_GEN_3_INT);
    MAP_PWMIntEnable(PWM0_BASE, PWM_INT_GEN_3);
    MAP_IntEnable(INT_PWM0_3);
    MAP_IntPrioritySet(INT_PWM0_3, 0x09);
    MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_3);
    MAP_PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
}

/**
 * @brief 初始化ws2812数据缓冲区及相关外设
 * 
 */
void ws2812_init(void) 
{
    uint32_t i;
    for (i = 0; i < WS2812_COLOR_BIT_NUM; i++) 
        ws2812_frame_buffer[i] = WS2812_DUTYCYCLE_0;    
    for (i = 0; i < WS2812_RESET_BIT_NUM; i++) 
        ws2812_frame_buffer[i + WS2812_COLOR_BIT_NUM] = 0; 

    ws2812_pin_init();
    ws2812_pwm_init();
}

/**
 * @brief 单个修改led数据
 * 
 * @param led 修改的led位置
 * @param r   red
 * @param g   green
 * @param b   blue
 */
void ws2812_write_led(uint16_t led, uint8_t r, uint8_t g, uint8_t b) 
{
    static bool s_init = false;
    uint8_t bit;
    if (!s_init) {
        ws2812_init();
        s_init = true;
    }
    for (bit = 0; bit < 8; bit++) {
        ws2812_frame_buffer[WS2812_BIT(led, 1, bit)] = ((r >> bit) & 0x01) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
        ws2812_frame_buffer[WS2812_BIT(led, 0, bit)] = ((g >> bit) & 0x01) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
        ws2812_frame_buffer[WS2812_BIT(led, 2, bit)] = ((b >> bit) & 0x01) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
    }
}

/**
 * @brief 批量设置led数据
 * 
 * @param ledarray 修改的led颜色数据
 * @param num      修改的led数量
 */
void ws2812_set_leds(ws2812_byte_t* ledarray, uint16_t num) 
{
    static bool s_init = false;
    uint16_t i;
    if (!s_init) {
        ws2812_init();
        s_init = true;
    }
    for (i = 0; i < num; i++) {
        ws2812_write_led(i, ledarray[i].r, ledarray[i].g, ledarray[i].b);
    }
}
