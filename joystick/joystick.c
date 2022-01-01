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

#include "joystick.h"

// 定义adc采样分辨率
#ifndef JOYSTICK_RESOLUTION
#define JOYSTICK_RESOLUTION 4096
#endif

// 定义摇杆阈值
#ifndef JOYSTICK_ACTUATION
#define JOYSTICK_ACTUATION 500
#endif

// 无动作时摇杆采样值
static const uint32_t resolution = JOYSTICK_RESOLUTION / 2;

// 摇杆两轴状态
static uint32_t axis_state[2];

// 摇杆方向状态
static bool arrows[4] = {0};

/**
 * @brief 初始化摇杆引脚
 * 
 */
void __attribute__((weak)) joystick_pins_init(void) { }

/**
 * @brief 初始化adc模块
 * 
 */
void __attribute__((weak)) joystick_adc_init(void) { }
 
 /**
  * @brief 采集摇杆引脚电压值
  * @param axis_state 摇杆两轴状态
  */
void __attribute__((weak)) joystick_read_pin(uint32_t *axis_state) { }

/**
 * @brief 初始化摇杆
 * 
 */
void joystick_init(void)
{
    joystick_pins_init();
    joystick_adc_init();
}

/**
 * @brief 更新摇杆状态
 * 
 * @return true 摇杆状态变更
 * @return false 摇杆状态未变更
 */
static bool joystick_update(void)
{
    bool changed = false;
    uint8_t index;
    int32_t tmp_axis;
    for (index = 0; index < 2; index++)
    {
        tmp_axis = axis_state[index] - resolution;
        if (!arrows[index * 2] && tmp_axis > JOYSTICK_ACTUATION)
        {
            arrows[index * 2] = true;
            changed = true;
        }
        else if (arrows[index * 2] && tmp_axis < JOYSTICK_ACTUATION)
        {
            arrows[index * 2] = false;
            changed = true;
        }
        if (!arrows[index * 2 + 1] && tmp_axis < -JOYSTICK_ACTUATION)
        {
            arrows[index * 2 + 1] = true;
            changed = true;
        }
        else if (arrows[index * 2 + 1] && tmp_axis > -JOYSTICK_ACTUATION)
        {
            arrows[index * 2 + 1] = false;
            changed = true;
        }
    }
    if (changed)
        joystick_func(arrows);
    return changed;
}

/**
 * @brief 扫描摇杆状态
 * 
 * @return true 摇杆状态变更
 * @return false 摇杆状态未变更
 */
bool joystick_scan(void)
{
    bool changed = false;
    joystick_read_pin(axis_state);
    changed |= joystick_update();
    return changed;
}

/**
 * @brief 摇杆功能实现
 * 
 * @param arrows 摇杆方向状态
 */
void __attribute__((weak)) joystick_func(bool *arrows) { }
