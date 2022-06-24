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
#include "livewp-common.h" 
#include "livewp-control-widget.h"

osso_return_t 
execute(osso_context_t *osso, gpointer data, gboolean user_activated){

    Animation_WallpaperPrivate *priv = g_new0 (Animation_WallpaperPrivate, 1);
    priv->theme_string_parametr1 = NULL;
    priv->osso = osso_initialize("org.maemo.livewp", VERSION, TRUE, NULL);
    /* Load config */
    read_config(priv);
    /* Initialize DBUS */
    livewp_initialize_dbus(priv);


    lw_main_settings(priv, data);
    return OSSO_OK;

}

osso_return_t 
save_state(osso_context_t *osso, gpointer data)
{
		/* ... save state ... */
		fprintf(stderr,"SAVE state\n");
		return OSSO_OK;
}

