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
#ifndef _livewp_astro_h
#define _livewp_astro_h 1
/*******************************************************************************/
#include <math.h>
/*******************************************************************************/
double ut(int hour, int min, int zone);
double jd(int year, int month, int day, double uu);
double altitude(double lon, double lat, int year, int month, int day, int hour, int min, int zone);
double azimuth(double lon, double lat, int year, int month, int day, int hour, int min, int zone);
double moon_phase(int year, int month, int day);
void sun_rise_set(double lon, double lat, 
             int year, int month, int day, 
             int zone,
             int * hourrise, int *minrise, int *hourset, int *minset);

/*******************************************************************************/
#endif
