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

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdbool.h>
#include <stdint.h>

void joystick_pins_init(void);
void joystick_read_pin(uint32_t *axis_state);
void joystick_adc_init(void);
void joystick_init(void);
bool joystick_scan(void);
void joystick_func(bool *arrows);

#endif
