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
#ifndef _livewp_common_h
#define _livewp_common_h 1
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include <hildon/hildon.h>
#include <locale.h>
#include <libhildondesktop/libhildondesktop.h>
#include <libosso.h>
#include <gconf/gconf-client.h>
#include <time.h> 
#include <X11/extensions/Xrender.h>
#include <gdk/gdkx.h>

#include <X11/Xatom.h>
/* Gstreamer */
#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>

#define GETTEXT_PACKAGE "livewp"
#include <glib/gi18n-lib.h>

/* Position of plugin on desktop */
#define Xstartposition 800
#define Ystartposition 480 

#define THEME_PATH "/usr/share/livewp/theme"
#define EXTHEME_PATH "/usr/share/livewp/external_themes/"
#define SHORT_TIMER 75 /* 100 milisecond */
//#define LONG_TIMER 5*60*1000 /* 10 minutes */
#define LONG_TIMER  20*1000 /* 20 seconds */
enum { SHORT_TIMER_TYPE, LONG_TIMER_TYPE };
#define SKY_LEVEL 0 
#define SUN_LEVEL 0 
#define TOWN_LEVEL 0 
#define TRAM_LEVEL 20 

typedef struct _Scene Scene;
struct _Scene 
{
    GSList * actors;
    gint daytime;
    gint timer_type;
    gint wind_orientation; // +/- 1
    double wind_angle; // [-1 .. 1]
    guint seed; /* init value for random  */
    guint notification;
};

typedef struct _AWallpaperPlugin AWallpaperPlugin;
typedef struct _AWallpaperPluginClass AWallpaperPluginClass;
typedef struct _Animation_WallpaperPrivate Animation_WallpaperPrivate;



struct _Animation_WallpaperPrivate
{
    gint long_timer;
    gint short_timer;
    gchar *theme;
    gboolean visible;
    gboolean rich_animation;
    gboolean one_in_all_view;
    GtkWidget *main_widget;
    GtkWidget *window;
    GtkWidget *right_corner;
    AWallpaperPlugin *desktop_plugin;
    DBusConnection      *dbus_conn;
    DBusConnection      *dbus_conn_session;
    osso_context_t      *osso;
    gint xapplet;
    gint yapplet;
    Scene *scene;
    //GHashTable * hash_scene_func;
    void (*scene_func)(gpointer);
    GKeyFile *config_file;
    gint view;
    pid_t  pid;
    pid_t  podpid;
    guint view_notify;
    guint alive_timer;
    time_t last_alive_event;
    gchar *theme_string_parametr1;
    guint theme_int_parametr1;
    gboolean theme_bool_parametr1;
    GstElement *pipeline;
    gboolean running;
    GSList *extheme_list;
    GHashTable *hash_theme;
    GtkWidget *video_socket;
};

struct _AWallpaperPlugin
{
    HDHomePluginItem parent;
    Animation_WallpaperPrivate *priv;
};

struct _AWallpaperPluginClass
{
  HDHomePluginItemClass parent_class;
};


#define LIVEWP_SIGNAL_INTERFACE "org.maemo.livewp"
#define LIVEWP_SIGNAL_PATH "/org/maemo/livewp"
#define LIVEWP_SIGNAL_RELOAD_CONFIG_INTERFACE "org.maemo.livewp.reload_config"
#define LIVEWP_SIGNAL_RELOAD_CONFIG_PATH "/org/maemo/livewp/reload_config"
#define LIVEWP_RELOAD_CONFIG "reload_config"
#define LIVEWP_RELOAD_PLUGIN "reload_plugin"
#define LIVEWP_STOP_LIVEBG_ON_VIEW1 "stop_livebg_on_view1"
#define LIVEWP_STOP_LIVEBG_ON_VIEW2 "stop_livebg_on_view2"
#define LIVEWP_STOP_LIVEBG_ON_VIEW3 "stop_livebg_on_view3"
#define LIVEWP_STOP_LIVEBG_ON_VIEW4 "stop_livebg_on_view4"
#define LIVEWP_STOP_LIVEBG_ON_VIEW5 "stop_livebg_on_view5"
#define LIVEWP_STOP_LIVEBG_ON_VIEW6 "stop_livebg_on_view6"
#define LIVEWP_STOP_LIVEBG_ON_VIEW7 "stop_livebg_on_view7"
#define LIVEWP_STOP_LIVEBG_ON_VIEW8 "stop_livebg_on_view8"
#define LIVEWP_STOP_LIVEBG_ON_VIEW9 "stop_livebg_on_view9"
#define LIVEWP_PAUSE_LIVEBG_ON_VIEW1 "pause_livebg_on_view1"
#define LIVEWP_PAUSE_LIVEBG_ON_VIEW2 "pause_livebg_on_view2"
#define LIVEWP_PAUSE_LIVEBG_ON_VIEW3 "pause_livebg_on_view3"
#define LIVEWP_PAUSE_LIVEBG_ON_VIEW4 "pause_livebg_on_view4"
#define LIVEWP_PAUSE_LIVEBG_ON_VIEW5 "pause_livebg_on_view5"
#define LIVEWP_PAUSE_LIVEBG_ON_VIEW6 "pause_livebg_on_view6"
#define LIVEWP_PAUSE_LIVEBG_ON_VIEW7 "pause_livebg_on_view7"
#define LIVEWP_PAUSE_LIVEBG_ON_VIEW8 "pause_livebg_on_view8"
#define LIVEWP_PAUSE_LIVEBG_ON_VIEW9 "pause_livebg_on_view9"
#define LIVEWP_PAUSE_LIVEBG_ON_VIEW "pause_livebg_on_view"
#define LIVEWP_PLAY_LIVEBG_ON_VIEW1 "play_livebg_on_view1"
#define LIVEWP_PLAY_LIVEBG_ON_VIEW2 "play_livebg_on_view2"
#define LIVEWP_PLAY_LIVEBG_ON_VIEW3 "play_livebg_on_view3"
#define LIVEWP_PLAY_LIVEBG_ON_VIEW4 "play_livebg_on_view4"
#define LIVEWP_PLAY_LIVEBG_ON_VIEW5 "play_livebg_on_view5"
#define LIVEWP_PLAY_LIVEBG_ON_VIEW6 "play_livebg_on_view6"
#define LIVEWP_PLAY_LIVEBG_ON_VIEW7 "play_livebg_on_view7"
#define LIVEWP_PLAY_LIVEBG_ON_VIEW8 "play_livebg_on_view8"
#define LIVEWP_PLAY_LIVEBG_ON_VIEW9 "play_livebg_on_view9"
#define LIVEWP_PLAY_LIVEBG_ON_VIEW "play_livebg_on_view"
#define LIVEWP_ALIVE_LIVEBG_ON_VIEW1 "alive_livebg_on_view1"
#define LIVEWP_ALIVE_LIVEBG_ON_VIEW2 "alive_livebg_on_view2"
#define LIVEWP_ALIVE_LIVEBG_ON_VIEW3 "alive_livebg_on_view3"
#define LIVEWP_ALIVE_LIVEBG_ON_VIEW4 "alive_livebg_on_view4"
#define LIVEWP_ALIVE_LIVEBG_ON_VIEW5 "alive_livebg_on_view5"
#define LIVEWP_ALIVE_LIVEBG_ON_VIEW6 "alive_livebg_on_view6"
#define LIVEWP_ALIVE_LIVEBG_ON_VIEW7 "alive_livebg_on_view7"
#define LIVEWP_ALIVE_LIVEBG_ON_VIEW8 "alive_livebg_on_view8"
#define LIVEWP_ALIVE_LIVEBG_ON_VIEW9 "alive_livebg_on_view9"




/* For DBUS notification */
#define NOTIFY_SIGNAL_INTERFACE "org.freedesktop.Notifications"
#define NOTIFY_SIGNAL_PATH "/org/freedesktop/Notifications"
#define NOTIFY_MEMBER "Notify"
#define CLOSENOTIFY_MEMBER "NotificationClosed"

#endif
