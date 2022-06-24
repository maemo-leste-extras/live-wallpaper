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
#include "livewp-common.h"

#include <hildon-fm-2/hildon/hildon-file-chooser-dialog.h>
#include <unistd.h>
void fill_theme_button (Animation_WallpaperPrivate *priv, GtkWidget *button, gchar *category);
void lw_main_settings(Animation_WallpaperPrivate *priv, gpointer data);
void save_config(Animation_WallpaperPrivate *priv);
gboolean check_applet_state(gint number);
void reload_scene(AWallpaperPlugin *desktop_plugin);
GtkWidget* create_settings(void);
void show_settings(GtkWidget *widget, Animation_WallpaperPrivate *priv);
void start_applet(gint number);
void stop_applet(gint number);
void send_dbus_signal (Animation_WallpaperPrivate *priv, const gchar *interface,
                  const gchar *path, const gchar *member);
gint read_config(Animation_WallpaperPrivate *priv);
void lw_theme_settings(GtkWidget *button, Animation_WallpaperPrivate *priv); 
void theme_button_clicked(GtkWidget *button, GdkEventButton *event, Animation_WallpaperPrivate *priv);
void rich_animation_additional_parametr(GtkWidget *vbox, Animation_WallpaperPrivate *priv);
void additional_parametr_for_theme_video(GtkWidget *vbox, Animation_WallpaperPrivate *priv);
GSList * get_list_exthemes(void);
void show_problem_package (GtkWidget *widget, gchar *package_name);
void show_duplicate_theme (GtkWidget *widget, gchar *theme_name);
void changed_value_one_in_all_cb (GtkWidget *toggle, Animation_WallpaperPrivate *priv);
void save_one_in_all_views_to_config(gboolean one_in_all_views);
gboolean cb_timeout_settings(GtkWidget *window);
void create_themes_buttons_hbox(Animation_WallpaperPrivate *priv);

