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
#ifndef Animation_Wallpaper_PLUGIN_H
#define Animation_Wallpaper_PLUGIN_H

#include <glib-object.h>
#include "livewp-actor.h"
#include "livewp-scene.h"

G_BEGIN_DECLS


#define Animation_Wallpaper_TYPE_HOME_PLUGIN (animation_wallpaper_plugin_get_type ())

#define Animation_Wallpaper_HOME_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                        Animation_Wallpaper_TYPE_HOME_PLUGIN, AWallpaperPlugin))

#define Animation_Wallpaper_HOME_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), \
                        Animation_Wallpaper_TYPE_HOME_PLUGIN, AWallpaperPluginClass))

#define Animation_Wallpaper_IS_HOME_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                        Animation_Wallpaper_TYPE_HOME_PLUGIN))

#define Animation_Wallpaper_IS_HOME_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                        Animation_Wallpaper_TYPE_HOME_PLUGIN))

#define Animation_Wallpaper_HOME_PLUGIN_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                        Animation_Wallpaper_TYPE_HOME_PLUGIN, AWallpaperPluginClass))


/*
typedef struct {
    time_t timestart;
    gint timeall;
    gint count;
    GtkWidget *actor;
    void (*func_change)(GtkWidget *, double);
    gint (*func_time)(gint);
} Animation;
*/

GType animation_wallpaper_plugin_get_type(void);

Animation_WallpaperPrivate* animation_wallpaper_plugin_new (void);
G_END_DECLS
void lw_settings(gpointer user_data, gpointer data);
//void run_long_timeout(AWallpaperPlugin *desktop_plugin);
void get_sun_screen_pos(double alt, double azm, gint * x, gint * y);
void show_settings(GtkWidget *widget, Animation_WallpaperPrivate *priv);
void livewp_initialize_dbus(Animation_WallpaperPrivate *priv);
void livewp_deinitialize_dbus(Animation_WallpaperPrivate *priv);
gint current_active_view(void);
//gboolean long_timeout (AWallpaperPlugin *desktop_plugin);
void desktop_plugin_visible_notify (GObject *object, GParamSpec *spec, AWallpaperPlugin *desktop_plugin);
void send_dbus_signal (Animation_WallpaperPrivate *priv, const gchar *interface, const gchar *path, const gchar *member);
gboolean get_one_in_all_views_from_config(void);
#endif
