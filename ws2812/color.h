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
 
#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <stdbool.h>

// HSV
typedef struct 
{
    uint8_t h;    // 色相:0-255表示0-360°
    uint8_t s;    // 饱和:0-255表示0-360°
    uint8_t v;    // 亮度:0-255表示0-360°
} HSV;

// RGB
typedef struct
{
    uint8_t r;      // red:0-255
    uint8_t g;      // green:0-255
    uint8_t b;      // blue:0-255
} RGB;

RGB hsv_to_rgb(HSV hsv);

#endif
