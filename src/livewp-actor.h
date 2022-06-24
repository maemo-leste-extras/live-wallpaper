/*vim: set sw=4 ts=4 et: */
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
#ifndef _livewp_actor_h
#define _livewp_actor_h 1

#include "livewp-common.h"
#include <sys/time.h>
#include "livewp-rules.h"
#include <sqlite3.h>

typedef struct _Actor Actor;
struct _Actor
{
    GtkWidget * widget;
    GtkWidget * image;
    gchar  *name;
    gchar *filename;
    gint x;
    gint y;
    gint z;
    gint width;
    gint height;
    gint scale;
    gint opacity;
    gboolean visible;
    void (*func_change)(gpointer, gpointer);
    time_t time_start_animation;
    gint duration_animation;
    void (*func_probability)(gpointer);
    GPtrArray *child;

};

/*******************************************************************************/
Actor* init_object(AWallpaperPlugin *desktop_plugin,
            gchar * name,
            gchar * filename,
            gint x,
            gint y,
            gint z,
            gint width,
            gint height,
            gboolean visible,
            gboolean load_image,
            gint scale,
            gint opacity,
            void (*pfunc_change)(Actor*),
            void (*pfunc_probability)(Actor*),
            GPtrArray *child
           );
void destroy_hildon_actor(Actor *actor);
void create_hildon_actor_text(Actor *actor, AWallpaperPlugin *desktop_plugin);
void create_hildon_actor(Actor *actor, AWallpaperPlugin *desktop_plugin);
void change_hildon_actor(Actor *actor, AWallpaperPlugin *desktop_plugin);
void destroy_actor(Actor *actor);
void set_actor_position(Actor *actor, gint x, gint y, gint z, AWallpaperPlugin *desktop_plugin);
void set_actor_scale(Actor *actor, double scalex, double scaley);
void set_actor_opacity(Actor *actor, gint opacity);
void set_actor_rotation(Actor *actor, gint axis, double degrees, gint x, gint y, gint z);
void set_actor_visible(Actor *actor, gboolean visible);
void change_obj(Actor * actor, AWallpaperPlugin *desktop_plugin);
void change_tape(Actor * actor, AWallpaperPlugin *desktop_plugin);
void change_slideshow(Actor * actor, AWallpaperPlugin *desktop_plugin);
void change_billboard(Actor * actor, AWallpaperPlugin *desktop_plugin);
void change_moon(Actor * actor, AWallpaperPlugin *desktop_plugin);
void change_sun(Actor * actor, AWallpaperPlugin *desktop_plugin);
void change_tram(Actor * actor, AWallpaperPlugin *desktop_plugin);
void change_plane1(Actor *actor, AWallpaperPlugin *desktop_plugin);
void change_plane2(Actor *actor, AWallpaperPlugin *desktop_plugin);
void change_cloud(Actor *actor, AWallpaperPlugin *desktop_plugin);
void change_wind(Actor *actor, AWallpaperPlugin *desktop_plugin);
void change_window1(Actor * actor, AWallpaperPlugin *desktop_plugin);
void change_signal(Actor * actor, AWallpaperPlugin *desktop_plugin);
void change_layer(Actor * actor, AWallpaperPlugin *desktop_plugin);
void change_static_actor(Actor * actor, AWallpaperPlugin *desktop_plugin);
void change_static_actor_with_corner(Actor * actor, AWallpaperPlugin *desktop_plugin);
void create_hildon_actor(Actor *actor, AWallpaperPlugin *desktop_plugin);
void change_hildon_actor(Actor *actor, AWallpaperPlugin *desktop_plugin);
void destroy_hildon_actor(Actor *actor);
#endif
