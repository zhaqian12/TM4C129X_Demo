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

#include "color.h"

/**
 * @brief HSV转换为RGB
 * 
 * @param hsv 
 * @return RGB 
 */
RGB hsv_to_rgb(HSV hsv)
{
    RGB rgb;
    uint8_t hi, f, p, q, t;
    uint16_t h, s, v;
    if (hsv.s == 0) // gray
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }
    h = hsv.h;
    s = hsv.s;
    v = hsv.v;
    /** 
     *  hue 0-360  -> 0-255
     *  sat 0-1    -> 0-255
     *  val 0-1    -> 0-255 
     *  计算放大255倍 
     */
    hi = (h * 6 / 255) % 6;
    f = h * 6 - hi * 255;
    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * f) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - f)) >> 8))) >> 8;
    switch (hi) {
        case 0:
            rgb.r = v;
            rgb.g = t;
            rgb.b = p;
            break;
        case 1:
            rgb.r = q;
            rgb.g = v;
            rgb.b = p;
            break;
        case 2:
            rgb.r = p;
            rgb.g = v;
            rgb.b = t;
            break;
        case 3:
            rgb.r = p;
            rgb.g = q;
            rgb.b = v;
            break;
        case 4:
            rgb.r = t;
            rgb.g = p;
            rgb.b = v;
            break;
        case 5:
            rgb.r = v;
            rgb.g = p;
            rgb.b = q;
            break;
        default:
            break;
    }
    return rgb;
}

