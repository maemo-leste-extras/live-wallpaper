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

#include "livewp-rules.h"

/*******************************************************************************/

void get_localtime(int * year, int * month, int * day, int * hour, int * min, int * zone)
{
    time_t timet;
    struct tm nt;
    time(&timet);
    localtime_r(&timet, &nt);
    *year = nt.tm_year + 1900;
    *month = nt.tm_mon + 1;
    *day = nt.tm_mday;
    *hour = nt.tm_hour;
    *min = nt.tm_min;
    *zone = nt.tm_gmtoff/3600;
  /* 
 *year = 2010;
    *month = 3;
    *day = 24;
    *hour = 16;
    *min = 20;
    *zone = 2;*/
}

void get_coord(double * lat, double * lon)
{
    /* coordinates St.-Petersburg */
    *lat = 59.883333;
    *lon = 30.25;

    /* coordinates Vitebsk */
    //*lat = 55.166666;
    //*lon = 30.133333;


}

double
get_max_sun_alt(double lon, double lat, int year, int month, int day, int zone)
{
    int h1, m1, h2, m2, h, m;
    struct tm;
    double alt, hour1, hour2, hh, x;

    sun_rise_set(lon, lat,
                 year, month, day, 
                 zone,
                 &h1, &m1, &h2, &m2);
    //printf("h1=%d m1=%d h2=%d m2=%d\n", h1, m1, h2, m2);
    
    hour1 = (double)h1 + (double)m1/60;
    hour2 = (double)h2 + (double)m2/60;

    hh = (hour2 - hour1) / 2 + hour1;
    h = floor(hh);
    m = (modf(hh, &x) * 60);
    
    alt = altitude(lon, lat, year, month, day, h, m, zone);
    //printf("alt = %f \n", alt);
    return alt;
}

void get_sun_pos(double * alt, double * azm)
{
    int year, month, day, hour, min, zone;
    double lat, lon, alt_max;
    get_localtime(&year, &month, &day, &hour, &min, &zone);
    get_coord(&lat, &lon);
    alt_max = get_max_sun_alt(lon, lat, year, month, day, zone);
    *alt = altitude(lon, lat, year, month, day, hour, min, zone);
    //printf("max = %f alt = %f ",alt_max, *alt);
    *alt = *alt / (alt_max + 10); // sun height in percent
    //printf("alt1 = %f \n", *alt);

    *azm = azimuth(lon, lat, year, month, day, hour, min, zone);
    //printf("azm = %f ", *azm);
    //if south latitude
    if (lat < 0){
        *azm = 75 + (75 - *azm);
        *azm = fmod(*azm, 380);
    }
    *azm = (*azm - 75) / 210; // sun azimuth in percent
    //printf("azm1 = %f \n", *azm);
}
time_t get_next_sunrise()
{
    return time(NULL) + 60*60*8;
}
time_t get_next_sunset()
{
        return time(NULL) + 60*60*12;
}

int get_daytime()
{
    double alt, azm;
    get_sun_pos(&alt, &azm);
    alt = alt * 100;
    azm = azm * 100;
    //printf("alt = %f azm=%f\n", alt, azm);
    if (alt <= -7) return TIME_NIGHT;
    if (alt > -7 && alt < 7 && azm < 50) return TIME_SUNRISE;
    if (alt > -7 && alt < 7 && azm > 50) return TIME_SUNSET;
    if (alt >= 7) return TIME_DAY;
    return TIME_DAY;
}

int get_moon_phase()
{ 
    int year, month, day, hour, min, zone;
    double phase;
    get_localtime(&year, &month, &day, &hour, &min, &zone);
    phase = moon_phase(year, month, day) * 100;
    /* printf("ph = %f\n", phase); */
    if (phase <= 3 || phase >=97) return MOON_NONE;
    if (phase > 5 && phase <= 25) return MOON_GROWS;
    if (phase > 25 && phase <= 45) return MOON_GROWSHALF;
    if (phase > 45 && phase <= 55) return MOON_FULL;
    if (phase > 55 && phase <= 75) return MOON_DECREASHALF;
    if (phase > 75 && phase < 97) return MOON_DECREAS;
    return MOON_NONE;

}

gint 
fast_rnd(gint max)
{
    guint offset = 12923; 
    guint multiplier = 4079; 
    static guint seed = 123;

    seed = seed * multiplier + offset;
    return (gint)(seed % max);
}
void 
get_sun_screen_pos(double alt, double azm, gint * x, gint * y)
{
    gint y0 = 365;// - уровень горизонта
    *x = (int)(azm * 800) - 64;
    *y = (int)((1 - alt) * y0) - 64;
    //fprintf(stderr, "sun pos alt=%f azm=%f x=%d y=%d\n", alt, azm, *x, *y);
}

