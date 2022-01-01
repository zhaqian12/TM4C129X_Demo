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

#ifndef WS2812_H
#define WS2812_H

#include "color.h"

typedef struct {
    uint8_t g;     
    uint8_t r;
    uint8_t b;
} ws2812_byte_t;

void ws2812_init(void);
void ws2812_write_led(uint16_t led, uint8_t r, uint8_t g, uint8_t b);
void ws2812_set_leds(ws2812_byte_t* ledarray, uint16_t num);

#endif 
