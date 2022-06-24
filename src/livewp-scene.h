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

#ifndef _livewp_scene_h
#define _livewp_scene_h 1

#include "livewp-common.h"
#include "livewp-actor.h"
#include <sys/time.h>
#include <sys/wait.h>
#include "livewp-rules.h"
#include <glib-object.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <dirent.h>
#include <linux/fs.h>
typedef struct dirent Dirent;

void fill_priv(Animation_WallpaperPrivate *priv);
void destroy_scene(AWallpaperPlugin *desktop_plugin);
void reload_scene(AWallpaperPlugin *desktop_plugin);
void run_long_timeout(AWallpaperPlugin *desktop_plugin);
void reload_scene(AWallpaperPlugin *desktop_plugin);
void init_scene_Modern(AWallpaperPlugin *desktop_plugin);
void init_scene_Berlin(AWallpaperPlugin *desktop_plugin);
void init_scene_Matrix(AWallpaperPlugin *desktop_plugin);
void init_scene_Accel(AWallpaperPlugin *desktop_plugin);
void init_scene_Video(AWallpaperPlugin *desktop_plugin);
void init_scene_Slideshow(AWallpaperPlugin *desktop_plugin);
void init_scene_External(AWallpaperPlugin *desktop_plugin);
void init_scene_theme(AWallpaperPlugin *desktop_plugin);
void create_hildon_actor_text(Actor *actor, AWallpaperPlugin *desktop_plugin);
void set_live_bg (Display *display, Window xwindow, int mode);
void create_xwindow (Animation_WallpaperPrivate *priv);
#endif
