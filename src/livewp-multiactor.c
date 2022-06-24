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
#include "livewp-multiactor.h"

MultiActor * multiactor_init(gchar * name, GSList * child, gint x, gint y, gint z, double scale, gboolean visible)
{
    MultiActor *ma = NULL;
    ma = g_new0(MultiActor, 1);
    ma->name = name;
    ma->child = child;
    ma->x = x;
    ma->y = y;
    ma->z = z;
    ma->scale = scale;
    ma->visible = visible;
    fprintf(stderr, "multiator init name=%s\n", ma->name);
    return ma;
}

void multiactor_set_visible(MultiActor *ma, gboolean visible)
{
    GSList * tmp = ma->child;
    gboolean cur_visible;
    while (tmp != NULL){
        if (visible == FALSE){
            // hide all
            hildon_animation_actor_set_show(tmp->data, visible);
        }else {
            // show only needed
            cur_visible = g_object_get_data(G_OBJECT(tmp->data), "visible");
            if (cur_visible) hildon_animation_actor_set_show(tmp->data, visible);
        }
        tmp = g_slist_next(tmp);
    }
    ma->visible = visible;
}
void multiactor_set_scale(MultiActor *ma, double scale)
{
    GSList * tmp = ma->child;
    while (tmp != NULL){
        hildon_animation_actor_set_scale(tmp->data, scale, scale);
        tmp = g_slist_next(tmp);
    }
    ma->scale = scale;
}
void multiactor_set_position(MultiActor *ma, gint dx, gint dy, gint dz)
{
    GSList *tmp = ma->child;
    gint x, y, z;
    while (tmp != NULL){
        x = g_object_get_data(G_OBJECT(tmp->data), "x");
        y = g_object_get_data(G_OBJECT(tmp->data), "y");
        z = g_object_get_data(G_OBJECT(tmp->data), "z");
        actor_set_position_full(tmp->data, x + dx, y + dy, z + dz);        
        tmp = g_slist_next(tmp);
    }
    ma->x = dx;
    ma->y = dy;
    ma->z = dz;
}
