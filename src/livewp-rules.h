/*
 * This file is part of Live Wallpaper (livewp)
 * 
 * Copyright (C) 2010 Vlad Vasiliev
 * Copyright (C) 2010 Tanya Makova
 *       for the code
 * 
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#ifndef _livewp_rules_h
#define _livewp_rules_h 1
/*******************************************************************************/
#include <time.h>
#include "livewp-astro.h"
#include "livewp-common.h" 
/*******************************************************************************/
#define TIME_DAY        0
#define TIME_NIGHT      3 
#define TIME_SUNRISE    1
#define TIME_SUNSET     2

#define MOON_NONE               0
#define MOON_GROWS              5
#define MOON_GROWSHALF          2
#define MOON_FULL               1 
#define MOON_DECREASHALF        3
#define MOON_DECREAS            4

/*******************************************************************************/
void get_localtime(int * year, int * month, int * day, int * hour, int * min, int * zone);
void get_coord(double * lat, double * lon);
void get_sun_pos(double * alt, double * azm);
int get_daytime();
int get_moon_phase();
time_t get_next_sunrise();
time_t get_next_sunset();
gint fast_rnd(gint max);
void get_sun_screen_pos(double alt, double azm, gint * x, gint * y);

/*******************************************************************************/
#endif
