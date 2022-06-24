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
#include <stdio.h>
#include "livewp-astro.h"
    
double ut(int hour, int min, int zone)
{
    return (hour - zone + (double)min/60);
}

double jd(int year, int month, int day, double uu)
{
    return (367*year)-floor((floor((month+9)/12)+year)*7/4)+floor(275*month/9)+day-730531.5+(uu/24);
}

double altitude(double lon, double lat, int year, int month, int day, int hour, int min, int zone)
{
    double uu, jj, T, k, M, Lo, DL, L, eps, delta, RA, GMST, LMST, H, eqt, alt;
    uu = ut(hour, min, zone);
    jj = jd(year, month, day, uu);
    T = jj / 36525;
    k = M_PI / 180;
    M = 357.5291 + 35999.0503 * T - 0.0001559 * T * T - 0.00000045 * T * T * T;
    M = fmod(M, 360);
    Lo = 280.46645 + 36000.76983 * T + 0.0003032 * T * T;
    Lo = fmod(Lo, 360);
    DL = (1.9146-0.004817*T-0.000014*T*T)*sin(k*M)+(0.019993-0.000101*T)*sin(k*2*M)+0.00029*sin(k*3*M);
    L=Lo+DL;
    eps=23.43999-0.013*T;
    delta=(1/k)*asin(sin(L*k)*sin(eps*k));
    RA=(1/k)*atan2(cos(eps*k)*sin(L*k),cos(L*k));
    RA=fmod((RA+360), 360);
    GMST=280.46061837+360.98564736629*jj+0.000387933*T*T-T*T*T/38710000;
    GMST=fmod((GMST+360), 360);
    LMST=GMST+lon;
    H=LMST-RA;
    eqt=(Lo-RA)*4;
    alt=(1/k)*asin(sin(lat*k)*sin(delta*k)+cos(lat*k)*cos(delta*k)*cos(H*k));
    return alt;
}

double azimuth(double lon, double lat, int year, int month, int day, int hour, int min, int zone)
{
    double uu, jj, T, k, M, Lo, DL, L, eps, delta, RA, GMST, LMST, H, eqt, azm;
    uu = ut(hour, min, zone);
    jj = jd(year, month, day, uu);
    T = jj / 36525;
    k = M_PI / 180;
    M = 357.5291 + 35999.0503 * T - 0.0001559 * T * T - 0.00000045 * T * T * T;
    M = fmod(M, 360);
    Lo = 280.46645 + 36000.76983 * T + 0.0003032 * T * T;
    Lo = fmod(Lo, 360);
    DL = (1.9146-0.004817*T-0.000014*T*T)*sin(k*M)+(0.019993-0.000101*T)*sin(k*2*M)+0.00029*sin(k*3*M);
    L=Lo+DL;
    eps=23.43999-0.013*T;
    delta=(1/k)*asin(sin(L*k)*sin(eps*k));
    RA=(1/k)*atan2(cos(eps*k)*sin(L*k),cos(L*k));
    RA=fmod((RA+360), 360);
    GMST=280.46061837+360.98564736629*jj+0.000387933*T*T-T*T*T/38710000;
    GMST=fmod((GMST+360), 360);
    LMST=GMST+lon;
    H=LMST-RA;
    eqt=(Lo-RA)*4;
    azm=(1/k)*atan2(-sin(H*k),cos(lat*k)*tan(delta*k)-sin(lat*k)*cos(H*k));
    azm=fmod((azm+360), 360);
    return azm;
}
int day_of_year(int year, int month, int day)
{
    int n1, n2, n3, n;
    n1 = floor(275 * month / 9);
    n2 = floor((month + 9) / 12);
    n3 = (1 + floor((year - 4 * floor(year / 4) + 2) / 3));
    n = n1 - (n2 * n3) + day - 30;
    return n;
}
double deg2rad(double deg)
{
    return M_PI * deg / 180;
}
double truel(double m)
{
    double l;
    l = m + (1.916 * sin(deg2rad(m))) + (0.020 * sin(deg2rad(2*m))) + 282.634;
    if (l >= 360)
        l -= 360;
    else if (l < 0)
        l += 360;
    return l;
}
double rightasc(double l)
{
    double ra, lq, rq;
    ra = 180 * atan(0.91764 * tan(l * M_PI/180)) / M_PI;
    lq = 90 * floor(l/90);
    rq = 90 * floor(ra / 90);
    ra = ra + (lq - rq);
    return ra / 15;
}
double locangle(double lat, double sindec, double cosdec)
{
    double cosh = (-0.01454 - sindec * sin(deg2rad(lat))) / (cosdec * cos(deg2rad(lat)));
    return cosh;
}
double tout(double t, double lng_hour)
{
    double ut = t - lng_hour;
    if (ut < 0)
        ut += 24;
    else if (ut >= 24)
        ut -= 24;
    return ut;
}
void 
sun_rise_set(double lon, double lat, 
             int year, int month, int day, 
             int zone,
             int * hourrise, int *minrise, int *hourset, int *minset)
{
    int n;
    double lng_hour, trise, tset, mrise, mset, lrise, lset, 
           rarise, raset, sindecrise, cosdecrise, sindecset, cosdecset, 
           coshrise, coshset, 
           hrise, hset, ttrise, ttset, utrise, utset;
    int utrisehour, utrisemin, utsethour, utsetmin;
    n = day_of_year(year, month, day);
    lng_hour = lon / 15;
    trise = (double)n + (double)((6 - lng_hour) / 24);
    tset = (double)n + (double)((18 - lng_hour) / 24);
    mrise = (0.9856 * trise) - 3.289;
    mset = (0.9856 * tset) - 3.289;
    lrise = truel(mrise);
    lset = truel(mset);
    rarise = rightasc(lrise);
    raset = rightasc(lset);
    sindecrise = 0.39782 * sin(deg2rad(lrise));
    cosdecrise = cos(asin(sindecrise));
    sindecset = 0.39782 * sin(deg2rad(lset));
    cosdecset = cos(asin(sindecset));
    coshrise = locangle(lat, sindecrise, cosdecrise);
    /* if (coshrise > 1) sun never rises
     * if (coshrise < -1) sun never sets
    */
    coshset = locangle(lat, sindecset, cosdecset);
    hrise = (360 - 180 * acos(coshrise) / M_PI) / 15;
    hset = (180 * acos(coshrise) / M_PI) / 15;
    ttrise = hrise + rarise - (0.06571 * trise) - 6.622;
    ttset = hset + raset - (0.06571 * tset) - 6.622;
    utrise = tout(ttrise, lng_hour);
    utset = tout(ttset, lng_hour);

    utrisehour = floor(utrise);
    utrisemin = floor((utrise - utrisehour) * 60);
    utsethour = floor(utset);
    utsetmin = floor((utset - utsethour) * 60);

    *hourrise = utrisehour + zone;
    if (*hourrise >= 24)
        *hourrise -= 24;
    else if (*hourrise < 0)
        *hourrise += 24;
    *minrise = utrisemin;
    *hourset = utsethour + zone;
    if (*hourset >= 24)
        *hourset -= 24;
    else if (*hourset < 0)
        *hourset += 24;
    *minset = utsetmin;

}


double moon_phase(int year, int month, int day)
{
    int lg;
    double agepart, ge, jd, moonday;
    ge = 1721425.5;
    moonday = 29.530588853;
    if (month <= 2) lg = 0;
    else {
        if ((fmod(year, 4) == 0) && (!(fmod(year, 100) == 0) && (fmod(year, 400) != 0)))
            lg = -1;
        else lg = -2;
    }
    jd = (ge - 1) + 
        (365 * (year - 1)) + 
        floor((year - 1) / 4) + 
        (-floor((year - 1) / 100)) +
        floor((year - 1) / 400) + 
        floor((((367 * month) - 362) / 12) + lg + day);
    
    agepart = (jd - 2451550.1) / moonday;
    agepart = agepart - floor(agepart);
    if (agepart < 0) agepart ++;
/*
    ageday = agepart * moonday;

    moonfill = 1.0 - ((moonday/2 - ageday) / (moonday/2));
    fprintf(stderr, "agepart = %f, ageday = %f, moonfill = %f\n", agepart, ageday, moonfill);
    */
    return agepart;
}

/*
int main(int argc, char * argv)
{   
    int year, month, day, hour, min, zone;
    double alt, azm, lon, lat, moonpart;
    lon = 30.133333;
    lat = 55.166666;
    year = 2010;
    month = 4;
    day = 13;
    hour = 18;
    min = 10;
    zone = 2;
    
    //moonpart = moon_phase1(year, month, day);
    //fprintf(stderr, "moon path = %f\n", moonpart);
    alt = altitude(lon, lat, year, month, day, hour, min, zone);
    azm = azimuth(lon, lat, year, month, day, hour, min, zone);
    fprintf(stderr, "alt=%5.6f azm=%5.6f\n", alt, azm);

    return 1;
}
*/
