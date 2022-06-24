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
#ifndef _livewp_multiactor_h
#define _livewp_multiactor_h 1
/*******************************************************************************/
#include <glib-object.h>

#include <libhildondesktop/libhildondesktop.h>
#include "livewp-common.h"
/*******************************************************************************/
MultiActor * multiactor_init(gchar * name, GSList * child, gint x, gint y, gint z, double scale, gboolean visible);
void multiactor_set_visible(MultiActor *ma, gboolean visible);
void multiactor_set_scale(MultiActor *ma, double scale);
void multiactor_set_position(MultiActor *ma, gint dx, gint dy, gint dz);
void actor_set_position_full(GtkWidget *actor, gint x, gint y, gint z);
#endif
