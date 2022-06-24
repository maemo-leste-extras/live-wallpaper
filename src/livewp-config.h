/* vim: set sw=4 ts=4 et: */
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
#ifndef _livewp_config_h
#define _livewp_config_h 1

#include "livewp-common.h"

#define GCONF_KEY_PREFIX                            "/apps/maemo/livewp"
#define GCONF_KEY_THEME                             GCONF_KEY_PREFIX"/theme"
#define GCONF_KEY_ONE_IN_ALL_VIEW                   GCONF_KEY_PREFIX"/one_in_all_view"
#define GCONF_KEY_RANIMATION                        GCONF_KEY_PREFIX"/rich_animation"
#define GCONF_KEY_ADDITIONAL_STRING_1               GCONF_KEY_PREFIX"/additional_string_1"
#define GCONF_KEY_ADDITIONAL_BOOL_1                 GCONF_KEY_PREFIX"/additional_bool_1"
#define GCONF_KEY_ADDITIONAL_INT_1                  GCONF_KEY_PREFIX"/additional_int_1"
void init_scene_Modern(AWallpaperPlugin *desktop_plugin);
void init_scene_Berlin(AWallpaperPlugin *desktop_plugin);
void init_scene_Matrix(AWallpaperPlugin *desktop_plugin);
void init_scene_Accel(AWallpaperPlugin *desktop_plugin);
void init_scene_Video(AWallpaperPlugin *desktop_plugin);
void init_scene_External(AWallpaperPlugin *desktop_plugin);
void init_scene_theme(AWallpaperPlugin *desktop_plugin);
void init_scene_Conky(AWallpaperPlugin *desktop_plugin);
void init_scene_Flash(AWallpaperPlugin *desktop_plugin);
void init_scene_Slideshow(AWallpaperPlugin *desktop_plugin);
gint read_config(Animation_WallpaperPrivate *priv);
void save_config(Animation_WallpaperPrivate *priv);
void fill_priv(Animation_WallpaperPrivate *priv);
gint current_active_view(void);
GSList * get_list_exthemes(void);
void save_one_in_all_views_to_config(gboolean one_in_all_views);
gboolean get_one_in_all_views_from_config(void);

#endif
