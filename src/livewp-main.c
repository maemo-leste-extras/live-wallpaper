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
#include "livewp-scene.h"
#include "livewp-settings.h"
#include "livewp-home-widget.h"
/*******************************************************************************/

void set_live_bg (Display *display, Window xwindow, int mode);
void quit_from_program (Animation_WallpaperPrivate *priv);
void view_state_changed (Animation_WallpaperPrivate *priv);
static gboolean short_timeout (AWallpaperPlugin *desktop_plugin);
void run_long_timeout(AWallpaperPlugin *desktop_plugin);
gboolean long_timeout (AWallpaperPlugin *desktop_plugin);
void conky_visible(AWallpaperPlugin *desktop_plugin);
void flash_visible(AWallpaperPlugin *desktop_plugin);

void
set_live_bg (Display *display, Window xwindow, int mode){
   Atom atom;
   atom = XInternAtom (display, "_HILDON_LIVE_DESKTOP_BACKGROUND", False);
   fprintf (stderr, "XID: 0x%x\n", (unsigned)xwindow);

   XChangeProperty (display,
                 xwindow,
                  atom,
                  XA_INTEGER, 32, PropModeReplace,
                  (unsigned char *) &mode, 1);
}
/*******************************************************************************/
gboolean
delayed_pause_video (AWallpaperPlugin *desktop_plugin){

    if (!desktop_plugin->priv->visible){
        gst_element_set_state (desktop_plugin->priv->pipeline, GST_STATE_PAUSED);
    }
    return FALSE;
}
/*******************************************************************************/
void
view_state_changed (Animation_WallpaperPrivate *priv)
{
   fprintf(stderr," VIew %i State %i\n", priv->view, priv->visible);
   if (priv->visible){
       /* ToDo  make undependent function */
       /* For Video */
       if (!strcmp(priv->theme,"Video")){
           if (priv->pipeline){
               gst_element_set_state (priv->pipeline, GST_STATE_PLAYING);
           }else{
               init_scene_Video(priv->desktop_plugin);
           }
       }
       /* For Conky */
       if (!strcmp(priv->theme,"Conky")&& priv->running){
           conky_visible(priv->desktop_plugin);
           return;
       }
       /* For Flash */
       if (!strcmp(priv->theme,"Flash")&& priv->running){
           flash_visible(priv->desktop_plugin);
           return;
       }

       if (!priv->running){
            if (priv->hash_theme && g_hash_table_lookup(priv->hash_theme, "exec_path")){
                init_scene_External(priv->desktop_plugin);
            }
       }
       if (priv->long_timer == 0 ){
            priv->long_timer = g_timeout_add(LONG_TIMER, (GtkFunction)long_timeout, priv->desktop_plugin);
            run_long_timeout(priv->desktop_plugin);
       }else {
            run_long_timeout(priv->desktop_plugin);
       }
   }else{
        /* For Video */
        if (priv->pipeline){
           if (priv->one_in_all_view)
                g_timeout_add(200, (GtkFunction)delayed_pause_video, priv->desktop_plugin);
           else
                gst_element_set_state (priv->pipeline, GST_STATE_PAUSED);
           return;
        }
        /* For Conky */
        if (!strcmp(priv->theme,"Conky")){
           conky_visible(priv->desktop_plugin);
           return;
        }
        /* For Flash */
        if (!strcmp(priv->theme,"Flash")){
           flash_visible(priv->desktop_plugin);
           return;
        }

        if (priv->long_timer != 0 ){
            g_source_remove(priv->long_timer);
            priv->long_timer = 0;
        }
   }
}
/*******************************************************************************/
gboolean
check_alive_event(Animation_WallpaperPrivate *priv)
{
    if ( time(NULL) - priv->last_alive_event > 10*60) /* If last event later 10 minute */
        quit_from_program(priv);
    return TRUE;
}

/*******************************************************************************/
void
view_changed (GConfClient *client, guint cnxn_id,
                 GConfEntry *entry, Animation_WallpaperPrivate *priv)
{
   if (entry->value->type == GCONF_VALUE_INT) {
        if (priv->view == gconf_value_get_int(entry->value)){
            if (priv->visible != TRUE){
                priv->visible = TRUE;
                view_state_changed(priv);
            }
        }else{
            if (priv->visible != FALSE){
                priv->visible = FALSE;
                view_state_changed(priv);
            }
        }
    }
}
/*******************************************************************************/
void
quit_from_program (Animation_WallpaperPrivate *priv)
{
     fprintf(stderr,"quit_from_program\n");
     if (priv){
	     livewp_deinitialize_dbus(priv);
         if (priv->view_notify != 0) {
            gconf_client_notify_remove (gconf_client_get_default (), priv->view_notify);
            priv->view_notify = 0;
         }
	     if (priv->long_timer){
		    g_source_remove(priv->long_timer);
	    	priv->long_timer = 0;
	     }
	     if (priv->short_timer){
		    g_source_remove(priv->short_timer);
		    priv->short_timer = 0;
	     }
	     destroy_scene(priv->desktop_plugin);
         osso_deinitialize(priv->osso);
         //g_hash_table_destroy(priv->hash_scene_func);
         g_free(priv->desktop_plugin);
         priv->desktop_plugin = NULL;
         g_free(priv);
         priv = NULL;
     }
     fprintf(stderr,"end of quit_from_program\n");
     gtk_main_quit();;
}
/*******************************************************************************/
static gboolean
short_timeout (AWallpaperPlugin *desktop_plugin)
{
    //gint daytime = get_daytime();
    GSList * tmp;
    void (*pfunc)(gpointer, gpointer);
    time_t now;
    Actor *actor;
    gboolean stop_flag = TRUE;

    if (!desktop_plugin->priv->visible || !desktop_plugin->priv->rich_animation){
        desktop_plugin->priv->short_timer = 0;
        return FALSE;
    }

    now = time(NULL);
    //scene.daytime = daytime;
    /* fprintf(stderr, "Short timer %d\n", now); */
    tmp = desktop_plugin->priv->scene->actors;
    while (tmp != NULL){
           actor = tmp->data;
           if (now >= actor->time_start_animation
               && actor->time_start_animation > 0
               /* && now - actor->time_start_animation <= actor->duration_animation*/){
                pfunc = actor->func_change;
                if (pfunc){
                    (*pfunc)(actor, desktop_plugin);
                    stop_flag = FALSE;
                }
            }
            tmp = g_slist_next(tmp);
    }
    if (stop_flag){
         desktop_plugin->priv->short_timer = 0;
         return FALSE;
    }else
         return TRUE; /* keep running this event */
}
/*******************************************************************************/
void
run_long_timeout(AWallpaperPlugin *desktop_plugin)
{
	gint daytime = get_daytime();
    GSList * tmp;
    void (*pfunc)(gpointer, gpointer);
    time_t now;
    Actor *actor;

    if (!desktop_plugin->priv->scene)
	return;
    /*ifprintf(stderr, "!!!run long timeout short_timer=%d\n", desktop_plugin->priv->short_timer); */
    if (desktop_plugin->priv->scene->daytime != daytime){
        desktop_plugin->priv->scene->daytime = daytime;
        tmp = desktop_plugin->priv->scene->actors;
        while (tmp != NULL){
            //change_actor(tmp->data);
            pfunc =((Actor*)tmp->data)->func_change;
            if (pfunc){
                (*pfunc)(tmp->data, desktop_plugin);
            }
            tmp = g_slist_next(tmp);
        }
    }

    now = time(NULL);
    //fprintf(stderr, "Now  %d\n", now);
    tmp = desktop_plugin->priv->scene->actors;
    while (tmp != NULL){
        actor = tmp->data;
        if (now >= actor->time_start_animation
            && actor->time_start_animation > 0
            && desktop_plugin->priv->short_timer == 0){
            /* fprintf(stderr, "start short timer act = %s %d\n", actor->name, now); */
            if (desktop_plugin->priv->rich_animation){
                actor->time_start_animation = now;
                desktop_plugin->priv->short_timer = g_timeout_add(SHORT_TIMER, (GtkFunction)short_timeout, desktop_plugin);
            }
            else {
                (*actor->func_change)(actor, desktop_plugin);
            }
        }
        tmp = g_slist_next(tmp);
    }

}
/*******************************************************************************/
gboolean
long_timeout (AWallpaperPlugin *desktop_plugin)
{
    GstFormat fmt = GST_FORMAT_TIME;
    gint64 p;

    /*fprintf(stderr, "long_timeout %i\n", desktop_plugin->priv->long_timer); */
    if (desktop_plugin->priv->pipeline){
	if (!desktop_plugin->priv->visible && !desktop_plugin->priv->theme_bool_parametr1){
		gst_element_query_position(desktop_plugin->priv->pipeline, &fmt, &p);
		desktop_plugin->priv->theme_int_parametr1 = p / GST_SECOND;
		gst_element_set_state (desktop_plugin->priv->pipeline, GST_STATE_NULL);
		gst_object_unref (GST_OBJECT (desktop_plugin->priv->pipeline));
		desktop_plugin->priv->pipeline = NULL;
	}
	return TRUE;
    }

    if (desktop_plugin->priv->long_timer == 0 )
        return FALSE;
    if (!desktop_plugin->priv->visible){
        if(desktop_plugin->priv->short_timer != 0){
            g_source_remove(desktop_plugin->priv->short_timer);
            desktop_plugin->priv->short_timer = 0;
        }
        desktop_plugin->priv->long_timer = 0;
        return FALSE;
    }

    run_long_timeout(desktop_plugin);
    return TRUE; /* keep running this event */
}
/*******************************************************************************/
void
create_xwindow (Animation_WallpaperPrivate *priv){
    GtkWidget *main_widget = NULL;
    GtkWidget *label = NULL;

    /* Create Main GUI */
    main_widget = hildon_window_new ();
    priv->window = main_widget;
    gtk_window_set_title(GTK_WINDOW(main_widget), PACKAGE);
    gtk_window_fullscreen(GTK_WINDOW(main_widget));

    /* Check parametr 'gtk_socket_id'. It use in flash player for example */
    if (g_hash_table_lookup(priv->hash_theme, "gtk_socket_id")){
        priv->video_socket = gtk_socket_new ();
        gtk_container_add (GTK_CONTAINER (priv->window), priv->video_socket);
    }else{
        label = gtk_label_new(_("Loading Livewallpaper scene ..."));
        gtk_container_add (GTK_CONTAINER (main_widget), label);
    }

    gtk_widget_show_all(GTK_WIDGET(main_widget));

    /* Set window to background mode */
    if (priv->one_in_all_view)
        set_live_bg(GDK_WINDOW_XDISPLAY (main_widget->window), GDK_WINDOW_XID (main_widget->window), -1);
    else
        set_live_bg(GDK_WINDOW_XDISPLAY (main_widget->window), GDK_WINDOW_XID (main_widget->window), priv->view);
}
/*******************************************************************************/
int
main(int argc, char *argv[])
{
    HildonProgram       *app;
    int view = 1;
    GSList *stlist = NULL;
    GSList *tmp = NULL;
    GConfClient *gconf_client = NULL;
    GConfValue *value = NULL;
    gboolean result = TRUE;

    /* Ininitializing */
    hildon_gtk_init (&argc, &argv);
    app = HILDON_PROGRAM (hildon_program_get_instance());
    g_set_application_name (PACKAGE);

    /* Set view from argument string */
    if (argc == 2)
        view = atoi(argv[1]);
    if (view < 1 || view > 9)
        view = 1;

    /* Checking of running view */ 
    gconf_client = gconf_client_get_default();
    if (gconf_client){
        stlist = gconf_client_get_list(gconf_client,
                                           "/apps/osso/hildon-desktop/views/active",
                                           GCONF_VALUE_INT, NULL);
        if (stlist){
            tmp = stlist;
            result = FALSE;
            while (tmp != NULL){
                  if (view == GPOINTER_TO_INT(tmp->data))
                      result = TRUE;
                  tmp = g_slist_next(tmp);
            }
            g_slist_free(stlist);
        }
        g_object_unref(gconf_client);
    }
    if (!result)
        exit (-3);

    AWallpaperPlugin *desktop_plugin = g_new0 (AWallpaperPlugin, 1);
    Animation_WallpaperPrivate *priv = g_new0 (Animation_WallpaperPrivate, 1);
    desktop_plugin->priv = priv;


    priv->osso = osso_initialize("org.maemo.livewp", VERSION, TRUE, NULL);
    if(!priv->osso){
        fprintf(stderr,"osso_initialize failed\n");
        return 1;
    }

#ifdef ENABLE_NLS
    setlocale(LC_ALL, "");
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif

    priv->scene = NULL;
    priv->pipeline = NULL;
    priv->podpid = -1;
    priv->running = FALSE;
    /* fprintf(stderr,"XWINDOW %i\n",GDK_WINDOW_XID (main_widget->window)); */
    priv->desktop_plugin = desktop_plugin;


    priv->view = view;
    fill_priv(priv);
    fprintf(stderr,"THEME %s\n", priv->theme);

    /* Check activity of opition "One theme in all views" */
    if (priv->one_in_all_view && view >1)
        return -1;


    if (current_active_view() == view && !priv->one_in_all_view)
        priv->visible = TRUE;
    else
        priv->visible = FALSE;


    create_xwindow(priv);
    /* Initialize DBUS */
    livewp_initialize_dbus(priv);

    priv->view_notify = 0;
    /* fprintf(stderr,"VISIBLE %i %i\n",priv->visible, current_active_view()); */
    gconf_client_add_dir(gconf_client_get_default (), "/apps/osso/hildon-desktop/views", GCONF_CLIENT_PRELOAD_NONE, NULL);
    priv->view_notify = gconf_client_notify_add(gconf_client_get_default (),"/apps/osso/hildon-desktop/views/current",
                                                (GConfClientNotifyFunc)view_changed, priv, NULL, NULL);

    /* Set start position of Video to zero */
    if (!strcmp(priv->theme,"Video")){
        gst_init (NULL, NULL);
        priv->theme_int_parametr1 = 0;
    }

    init_scene_theme(desktop_plugin);

    priv->long_timer = g_timeout_add(LONG_TIMER, (GtkFunction)long_timeout, desktop_plugin);

    priv->alive_timer = g_timeout_add(60000*10, (GtkFunction)check_alive_event, priv);  /* One per 10 minute */
    gtk_main();
    return 0;
}
