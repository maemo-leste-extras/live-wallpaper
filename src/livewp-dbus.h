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
#ifndef _livewp_dbus_h
#define _livewp_dbus_h 1
/*******************************************************************************/
#include "livewp-common.h"
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
/*******************************************************************************/
void livewp_initialize_dbus(Animation_WallpaperPrivate *priv);
void livewp_deinitialize_dbus(Animation_WallpaperPrivate *priv);
DBusHandlerResult get_livewp_signal_cb(DBusConnection *conn, DBusMessage *msg, Animation_WallpaperPrivate *priv);
void send_dbus_signal (Animation_WallpaperPrivate *priv, const gchar *interface, const gchar *path, const gchar *member);
gint read_config(Animation_WallpaperPrivate *priv);
void reload_scene(AWallpaperPlugin *desktop_plugin);
void quit_from_program (Animation_WallpaperPrivate *priv);
void view_state_changed (Animation_WallpaperPrivate *priv);
/*******************************************************************************/
#endif
