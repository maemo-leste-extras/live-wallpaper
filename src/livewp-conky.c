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
#include "livewp-conky.h"
void create_config(gboolean sleep){
    FILE    *file_in;
    FILE    *file_out;
    gchar buffer[2048];

    file_in = fopen("/etc/conky/conky.conf","r");
    file_out = fopen("/tmp/conky.conf","w");
    if (file_in && file_out){
        while (!feof(file_in)) {
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, sizeof(buffer) - 1, file_in);
            if (!strcmp(buffer, "own_window yes\n")){
                fputs("own_window no\n", file_out);
		continue;
	    }
	    if (!strcmp(buffer, "$alignc $nodename - $sysname $kernel on $machine ${color yellow}Uptime:$color $uptime\n")){
                fputs("$alignr $nodename\n$alignr $sysname $kernel on $machine\n", file_out);
                fputs("$alignr ${color yellow}Uptime:$color $uptime\n", file_out);
		continue;
	    }
	    if (!strcmp(buffer, "gap_y 60\n")){
                fputs("gap_y 0\n", file_out);
		continue;
	    }
	    if (!strcmp(buffer, "gap_x 20\n")){
                fputs("gap_x 5\n", file_out);
		continue;
	    }
	    if (!strcmp(buffer, "update_interval 3.0\n") && sleep){
                fputs("update_interval 20.0\n", file_out);
		continue;
	    }
 	    if (!strcmp(buffer, "${color #24ff00} ${top name 4}    ${top pid 4} ${top cpu 4} ${top mem 4}")){
            	fputs(buffer, file_out);
                fputs("\n${color #20ff00} ${top name 5}    ${top pid 5} ${top cpu 5} ${top mem 5}\n", file_out);
                fputs("${color #16ff00} ${top name 6}    ${top pid 6} ${top cpu 6} ${top mem 6}\n", file_out);
		continue;
	    }
    
            fputs(buffer, file_out);
        }
        fclose(file_out);
        fclose(file_in);
    }

}
void 
init_scene_Conky(AWallpaperPlugin *desktop_plugin){
    create_config(False);
    init_scene_External(desktop_plugin);
}
void
conky_visible(AWallpaperPlugin *desktop_plugin){
    if (desktop_plugin->priv->visible)
    	create_config(FALSE);
    else
    	create_config(TRUE);
    kill (desktop_plugin->priv->podpid, SIGHUP);
}
