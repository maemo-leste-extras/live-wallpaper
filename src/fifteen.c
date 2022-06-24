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
#include <hildon/hildon.h>
#include <time.h>
#include <unistd.h>
#include "livewp-common.h"
#include "livewp-actor.h"
#include "livewp-dbus.h"

const gint width = 110;
const gint height = 110;
const gint timeout = 50; 
const gint time_bone = 15; 
const gint time_reload = 20; 

typedef struct _Scene1 Scene1;
struct _Scene1 
{
    gint *pg;
    //Actor *actors;
    gint empty;
    gint bone;
    gint timer_num;
};
Scene1 *scene;

Actor *actors[15];

void init_pg(gint *pg)
{
    srand(time(NULL));
    gint i, j, t;
    for (i=0; i<16; i++){
        pg[i] = i;
    }
    for (i=0; i<15; i++){
        j = rand()%15;
        t = pg[i];
        pg[i] = pg[j];
        pg[j] = t;
    }
}
void reinit(AWallpaperPlugin *desktop_plugin)
{
    fprintf(stderr, "reinit\n");
    gint i;
    Actor *actor;
    init_pg(scene->pg);

    scene->empty = 15;
    for (i=0; i<15; i++){
        actor = actors[scene->pg[i]];
        actor->x = (i%4)*width;
        actor->y = (i/4)*height;
        //fprintf(stderr, "x=%d, y=%d\n", actor->x, actor->y);
        set_actor_position(actor, actor->x, actor->y, actor->z, desktop_plugin);
        //set_actor_position(actor, (i%4)*width, (i/4)*height, 2, desktop_plugin);
    }
}

void moving_actor(gint num, gint max, AWallpaperPlugin *desktop_plugin)
{
    Actor *actor = actors[scene->pg[scene->bone]];
    gint x0 = actor->x, y0 = actor->y,
         x1 = (scene->empty%4)*width, y1 = (scene->empty/4)*height,
         x,y;
    //x = x0 + (x1-x0)*(max - num)/max;
    //y = y0 + (y1-y0)*(max - num)/max;
    x = x0 + (x1-x0)*(max-num)*(max-num)/(max*max);
    y = y0 + (y1-y0)*(max-num)*(max-num)/(max*max);
    set_actor_position(actor, x, y, actor->z, desktop_plugin);
    if (num == 0){
        actor->x = x;
        actor->y = y;
        scene->pg[scene->empty] = scene->pg[scene->bone];
        scene->pg[scene->bone] = 15;
        scene->empty = scene->bone;
    }
}
void moving_all(gint num, gint max, AWallpaperPlugin *desktop_plugin)
{
    gint i, axis;
    double angle;
    angle = 360*(max - num)*(max-num)/(max*max);
    for (i=0; i<15; i++){
        if (i%2 == 0) axis = HILDON_AA_X_AXIS;
        else axis = HILDON_AA_Y_AXIS;
        set_actor_rotation(actors[i], axis, angle, width/2, height/2, 0);
    }
    //if (num == (int)max/2){
    if (num == 0){
        reinit(desktop_plugin);
    }
}
gboolean main_timer(AWallpaperPlugin *desktop_plugin)
{
    if (scene->timer_num > 0){
        scene->timer_num--;
        if (scene->bone>-1)
            moving_actor(scene->timer_num, time_bone, desktop_plugin);
        else 
            moving_all(scene->timer_num, time_reload, desktop_plugin);
        return TRUE;
    } 
    char * accel_filename = "/sys/class/i2c-adapter/i2c-3/3-001d/coord";
    //char * accel_filename = "/home/tanya/coord";

    gint bone;
    FILE *fd = NULL;
    int rs, ax, ay, az;
    fd = fopen(accel_filename, "r");
    if (fd == NULL){
        fprintf(stderr, "cannot open file\n");
        return TRUE;
    }
    rs = fscanf((FILE*)fd, "%i %i %i", &ax, &ay, &az);
    fclose(fd);
    if (rs != 3){
        fprintf(stderr, "cannot read information from file\n");
        return TRUE;
    }

    //fprintf(stderr, "change obj %i %i %i\n", ax, ay, az);
    if (az < -2000) {
        //reinit(desktop_plugin);
        //sleep(1);
        scene->timer_num = time_reload;
        scene->bone = -1;
        return TRUE;
    }
    if (abs(ax) - abs(ay) > 300){
        if (ax > 0) {
            // LEFT;
            bone = scene->empty + 1;
            if (bone % 4 == 0) return TRUE;
        }
        else {
            // RIGHT;
            bone = scene->empty - 1;
            if (scene->empty % 4 == 0) return TRUE;
        }
    }else
    if (abs(ay) - abs(ax) > 300){
        if (ay > 0){ 
            // UP;
            bone = scene->empty + 4;
            if (bone > 15) return TRUE;
        }
        else {
            // DOWN;
            bone = scene->empty - 4;
            if (bone < 0) return TRUE;
        }
    } else return TRUE;
    //fprintf(stderr, "move %d\n", bone);
    scene->bone = bone;
    scene->timer_num = time_bone;
        
    return TRUE;
}
void init_actors(AWallpaperPlugin *desktop_plugin)
{
    gint i;
    Actor *actor;
    actor = init_object(desktop_plugin, "background", g_strdup("background.jpg"),
                                -180, -15, 2, 800, 480,
                                TRUE, TRUE, 100, 255,
                                NULL, NULL, NULL);
    for (i=0; i<15; i++){
        actors[i] = init_object(desktop_plugin, "bone", g_strdup_printf("%d.png", i+1),
                                0, 0, 2, width, height,
                                TRUE, TRUE, 100, 255,
                                NULL, NULL, NULL);
    }
    //return actors;
}
void
quit_from_program (Animation_WallpaperPrivate *priv)
{
    gtk_main_quit();
}

void
view_state_changed (Animation_WallpaperPrivate *priv)
{
    if (priv->visible && priv->long_timer == 0){
        priv->long_timer = g_timeout_add(timeout, (GtkFunction)main_timer, priv->desktop_plugin);
        //fprintf(stderr, "visible = 1 timeout_add %d\n", priv->long_timer);
    }else {
        //fprintf(stderr, "visible = 0 timer remove %d\n", priv->long_timer);
        g_source_remove(priv->long_timer);
        priv->long_timer = 0;
    }
    
}
gint
read_config (Animation_WallpaperPrivate *priv)
{
    return 0;
}
void
reload_scene(AWallpaperPlugin *desktop_plugin){}

int main( int   argc, char *argv[] )
{
    GtkWidget *window;
    AWallpaperPlugin *desktop_plugin = g_new0 (AWallpaperPlugin, 1);
    Animation_WallpaperPrivate *priv = g_new0 (Animation_WallpaperPrivate, 1);
    scene = g_new0(Scene1, 1);
    gint c, window_id=0, view=0;
    gint pg[16];

    while ((c = getopt(argc, argv, ":v:w:")) != -1){
        switch (c){
            case 'v':
                view = atoi(optarg);
                break;
            case 'w':
                window_id = atoi(optarg);
        }
    }

    //fprintf(stderr, "view=%d window_id=%d\n", view, window_id);

    hildon_gtk_init (&argc, &argv);
    g_set_application_name ("Fifteen");
    window = hildon_window_new ();
    //gtk_window_fullscreen (GTK_WINDOW(window));
    gtk_window_set_title(GTK_WINDOW(window),"Fifteen");
    gtk_window_set_wmclass(GTK_WINDOW(window),"Fifteen_exec","Fifteen_exec");
    //window->window = window_id;
    g_signal_connect (G_OBJECT (window), "delete_event",
                        G_CALLBACK (gtk_main_quit), NULL);
    priv->window = window;
    priv->theme = g_strdup("Fifteen"); 
    priv->xapplet = -180;
    priv->yapplet = -15;
    priv->desktop_plugin = desktop_plugin;
    priv->view = view;
    priv->long_timer = 0;
    desktop_plugin->priv = priv;

    priv->osso = osso_initialize("org.maemo.livewp", VERSION, TRUE, NULL);
    livewp_initialize_dbus(priv);
    

    //init_pg(pg);
    scene->pg = pg;
    //scene->actors = init_actors(desktop_plugin);
    init_actors(desktop_plugin);
    scene->timer_num = 0;
    reinit(desktop_plugin);
    gtk_widget_show  (window);
    //priv->long_timer = g_timeout_add(100, main_timer, desktop_plugin);
    //fprintf(stderr, "create timer %d\n", priv->long_timer);
    gtk_main ();
    return 0;
}


