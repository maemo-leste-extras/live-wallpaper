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
#include "livewp-flash.h"
void 
init_scene_Flash(AWallpaperPlugin *desktop_plugin){
    init_scene_External(desktop_plugin);
}
void
flash_visible(AWallpaperPlugin *desktop_plugin){
    fprintf(stderr,"Flash visible\n");
    if (desktop_plugin->priv->visible)
        kill (desktop_plugin->priv->podpid, SIGCONT);
    else
        kill (desktop_plugin->priv->podpid, SIGSTOP);
}
