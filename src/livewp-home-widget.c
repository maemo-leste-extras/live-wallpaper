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
#include "livewp-home-widget.h"
#include <gconf/gconf-client.h>
#include "livewp-rules.h"
#include "livewp-scene.h"
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define GCONF_KEY_POSITION "/apps/osso/hildon-desktop/applets/livewp-home-widget.desktop-%i/position"
#define GCONF_KEY_MODIFIED "/apps/osso/hildon-desktop/applets/livewp-home-widget.desktop-%i/modified"
#define GCONF_KEY_VIEW     "/apps/osso/hildon-desktop/applets/livewp-home-widget.desktop-%i/view"

HD_DEFINE_PLUGIN_MODULE (AWallpaperPlugin, animation_wallpaper_plugin, HD_TYPE_HOME_PLUGIN_ITEM)
#define Animation_Wallpaper_HOME_PLUGIN_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE (obj,\
                                                          Animation_Wallpaper_TYPE_HOME_PLUGIN,\
                                                          Animation_WallpaperPrivate))
static void
lw_applet_realize (GtkWidget *widget)
{
    GdkScreen *screen;
    gchar * strid; 
    gint id; 
    AWallpaperPlugin *desktop_plugin = (AWallpaperPlugin *)widget;
    char* child_argv[] =  {"/usr/bin/livewp", "0", (char *) 0 };
    GSList *stlist = NULL;
    GSList *tmp = NULL;
    GConfClient *gconf_client = NULL;
    GConfValue *value = NULL;
    gboolean result = TRUE;


    strid = g_strdup(hd_plugin_item_get_plugin_id (HD_PLUGIN_ITEM (widget)));

    id = strid[strlen(strid)-1] - '0';
    desktop_plugin->priv->view = id;
    g_free(strid); 

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
                  if ((id + 1) == GPOINTER_TO_INT(tmp->data))
                      result = TRUE;
                  tmp = g_slist_next(tmp);
            }
            g_slist_free(stlist);
        }
        g_object_unref(gconf_client);
    }

    switch (id){
	    case 0: child_argv[1] = "1";
		break;
	    case 1: child_argv[1] = "2";
		break;
	    case 2: child_argv[1] = "3";
		break;
	    case 3: child_argv[1] = "4";
		break;
	    case 4: child_argv[1] = "5";
		break;
	    case 5: child_argv[1] = "6";
		break;
	    case 6: child_argv[1] = "7";
		break;
	    case 7: child_argv[1] = "8";
		break;
	    case 8: child_argv[1] = "9";
		break;

    }
    desktop_plugin->priv->one_in_all_view = get_one_in_all_views_from_config();
    /* Check one theme in all view mode */
    if (result && 
	(!desktop_plugin->priv->one_in_all_view || 
        (desktop_plugin->priv->one_in_all_view && id == 0))){
	    /* Start a new process */
	    desktop_plugin->priv->pid = fork();
	    if (desktop_plugin->priv->pid == 0){
		execvp("/usr/bin/livewp", child_argv);
		fprintf(stderr,"Problem with new process");
	    }
    }

    desktop_plugin_visible_notify((GObject *)widget, NULL, desktop_plugin);

    screen = gtk_widget_get_screen (widget);
    gtk_widget_set_colormap (widget,
                                gdk_screen_get_rgba_colormap (screen));
    gtk_widget_set_app_paintable (widget,
                                TRUE);
    GTK_WIDGET_CLASS (animation_wallpaper_plugin_parent_class)->realize (widget);
}

/* Set position of widget on desktop */
static void
init_applet_position(gint *xapplet, gint *yapplet)
{
  GSList *position = NULL;
  gchar *position_key;
  gchar *modified_key;
  gchar *modified;
  GError *error = NULL;
  gint i;
  GConfClient   *gconf_client = gconf_client_get_default ();
  for (i=0;i<9;i++){
      position_key = g_strdup_printf (GCONF_KEY_POSITION, i);
      position = gconf_client_get_list (gconf_client,
                                        position_key,
                                        GCONF_VALUE_INT,
                                        NULL);
      if (position && position->data && position->next->data){
        *xapplet = GPOINTER_TO_INT (position->data);
        *yapplet = GPOINTER_TO_INT (position->next->data);
      }else{
        position = g_slist_prepend (g_slist_prepend (NULL,
                                          GINT_TO_POINTER (Ystartposition)),
                              GINT_TO_POINTER (Xstartposition));
        gconf_client_set_list (gconf_client,
                                   position_key,
                                   GCONF_VALUE_INT,
                               position,
                       &error);
        *xapplet = Xstartposition;
        *yapplet = Ystartposition;
      }
      g_free (position_key);
      modified = g_strdup_printf ("%i", 0);
      modified_key = g_strdup_printf (GCONF_KEY_MODIFIED, i);
      gconf_client_set_string (gconf_client,
                               modified_key,
                               modified,
                               &error);
      g_free(modified);
      g_free(modified_key);
      modified_key = g_strdup_printf (GCONF_KEY_VIEW, i);
      gconf_client_set_int (gconf_client,
                               modified_key,
                               (i+1),
                               &error);
      g_free(modified_key);

  }
  gconf_client_clear_cache(gconf_client);
  g_object_unref(gconf_client);
}

void
desktop_plugin_visible_notify (GObject    *object,
                                          GParamSpec *spec,
                                          AWallpaperPlugin *desktop_plugin)
{
    gboolean visible;
    Animation_WallpaperPrivate *priv = desktop_plugin->priv;
    g_object_get (object, "is-on-current-desktop", &visible, NULL);
    fprintf (stderr, "LWP is-on-current-desktop changed. visible: %u", visible); 
    if (priv){
             if (priv->one_in_all_view){
		     if (visible)
			send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PLAY_LIVEBG_ON_VIEW1);
		     else{
			/* start any application on current view */
			if ((priv->view + 1) == current_active_view())
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PAUSE_LIVEBG_ON_VIEW1);
		     }
		     return;
             }
	     switch (priv->view + 1){
		case 1: if (visible)
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PLAY_LIVEBG_ON_VIEW1);
			else
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PAUSE_LIVEBG_ON_VIEW1);
			break;
		case 2: if (visible)
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PLAY_LIVEBG_ON_VIEW2);
			else
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PAUSE_LIVEBG_ON_VIEW2);
			break;
		case 3: if (visible)
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PLAY_LIVEBG_ON_VIEW3);
			else
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PAUSE_LIVEBG_ON_VIEW3);
			break;
		case 4: if (visible)
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PLAY_LIVEBG_ON_VIEW4);
			else
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PAUSE_LIVEBG_ON_VIEW4);
			break;
	       case 5: if (visible)
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PLAY_LIVEBG_ON_VIEW5);
			else
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PAUSE_LIVEBG_ON_VIEW5);
			break;
	       case 6: if (visible)
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PLAY_LIVEBG_ON_VIEW6);
			else
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PAUSE_LIVEBG_ON_VIEW6);
			break;
	       case 7: if (visible)
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PLAY_LIVEBG_ON_VIEW7);
			else
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PAUSE_LIVEBG_ON_VIEW7);
			break;
	       case 8: if (visible)
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PLAY_LIVEBG_ON_VIEW8);
			else
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PAUSE_LIVEBG_ON_VIEW8);
			break;
	       case 9: if (visible)
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PLAY_LIVEBG_ON_VIEW9);
			else
				send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_PAUSE_LIVEBG_ON_VIEW9);
			break;

	     }

     }
    
}

gboolean
send_alive_event(Animation_WallpaperPrivate *priv) 
{
    switch (priv->view + 1){
	case 1: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
				  LIVEWP_SIGNAL_PATH, LIVEWP_ALIVE_LIVEBG_ON_VIEW1);
		break;
	case 2: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
				  LIVEWP_SIGNAL_PATH, LIVEWP_ALIVE_LIVEBG_ON_VIEW2);
		break;
	case 3: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
				  LIVEWP_SIGNAL_PATH, LIVEWP_ALIVE_LIVEBG_ON_VIEW3);
		break;
	case 4: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
				  LIVEWP_SIGNAL_PATH, LIVEWP_ALIVE_LIVEBG_ON_VIEW4);
		break;
	case 5: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
				  LIVEWP_SIGNAL_PATH, LIVEWP_ALIVE_LIVEBG_ON_VIEW5);
		break;
	case 6: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
				  LIVEWP_SIGNAL_PATH, LIVEWP_ALIVE_LIVEBG_ON_VIEW6);
		break;
	case 7: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
				  LIVEWP_SIGNAL_PATH, LIVEWP_ALIVE_LIVEBG_ON_VIEW7);
		break;
	case 8: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
				  LIVEWP_SIGNAL_PATH, LIVEWP_ALIVE_LIVEBG_ON_VIEW8);
		break;
	case 9: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
				  LIVEWP_SIGNAL_PATH, LIVEWP_ALIVE_LIVEBG_ON_VIEW9);
		break;


    }

    return TRUE;
}

static void
animation_wallpaper_plugin_init (AWallpaperPlugin *desktop_plugin)
{

    //fprintf(stderr, "!!!!!!!plugin init \n");
    Animation_WallpaperPrivate *priv =  Animation_Wallpaper_HOME_PLUGIN_GET_PRIVATE (desktop_plugin);
    desktop_plugin->priv =  Animation_Wallpaper_HOME_PLUGIN_GET_PRIVATE (desktop_plugin);
    priv->osso = osso_initialize(PACKAGE, VERSION, TRUE, NULL);

        /* Initialize DBUS */
    livewp_initialize_dbus(priv);

    priv->desktop_plugin = desktop_plugin;
    priv->window = (GtkWidget *)desktop_plugin;
    priv->visible = FALSE;
    priv->short_timer = 0;
    priv->xapplet = 0;
    priv->yapplet = 0;
    priv->scene = NULL;
    priv->pid = 0;
    priv->theme = g_strdup("Modern");
    //priv->theme = g_strdup("Accel");
    desktop_plugin->priv->main_widget = gtk_fixed_new();

    gtk_widget_set_size_request(desktop_plugin->priv->main_widget, 1, 1);
    gtk_widget_show (desktop_plugin->priv->main_widget);
    gtk_container_add (GTK_CONTAINER (desktop_plugin), desktop_plugin->priv->main_widget);
    init_applet_position(&(priv->xapplet), &(priv->yapplet));

    
    priv->scene = NULL;
    g_signal_connect (desktop_plugin, "notify::is-on-current-desktop",
                   G_CALLBACK (desktop_plugin_visible_notify), desktop_plugin);

    priv->alive_timer = g_timeout_add(60000*3, (GtkFunction)send_alive_event, priv);  /* One per 3 minute */ 
}

static void
lw_applet_finalize (GObject *object)
{
     int status = 0; int wpid; 
fprintf(stderr,"lw_applet_finalize\n");
     AWallpaperPlugin *desktop_plugin = Animation_Wallpaper_HOME_PLUGIN (object);
     Animation_WallpaperPrivate *priv = desktop_plugin->priv;
/*
     
     if (priv){
	     livewp_deinitialize_dbus(priv);
	     fprintf(stderr,"finalaze %i\n", priv->long_timer);
	     if (priv->long_timer){
		g_source_remove(priv->long_timer);
		priv->long_timer = 0;
	     }
	     if (priv->short_timer){
		g_source_remove(priv->short_timer);
		priv->short_timer = 0;
	     }

	     destroy_scene(desktop_plugin);
     }
*/    
     if (priv){
	     switch (priv->view + 1){
		case 1: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_STOP_LIVEBG_ON_VIEW1);
			break;
		case 2: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_STOP_LIVEBG_ON_VIEW2);
			break;
		case 3: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_STOP_LIVEBG_ON_VIEW3);
			break;
		case 4: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_STOP_LIVEBG_ON_VIEW4);
			break;
		case 5: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_STOP_LIVEBG_ON_VIEW5);
			break;
		case 6: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_STOP_LIVEBG_ON_VIEW6);
			break;
		case 7: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_STOP_LIVEBG_ON_VIEW7);
			break;
		case 8: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_STOP_LIVEBG_ON_VIEW8);
			break;
		case 9: send_dbus_signal (priv, LIVEWP_SIGNAL_INTERFACE, 
					  LIVEWP_SIGNAL_PATH, LIVEWP_STOP_LIVEBG_ON_VIEW9);
			break;
	     }
     }
     if(priv->alive_timer > 0) 
         g_source_remove(priv->alive_timer);

     livewp_deinitialize_dbus(priv);

     if (priv->pid >0){
	     while (TRUE){
		 wpid = wait(&status);
		 if (wpid == priv->pid)
		     break;
	     }
     }
}

static void
desktop_widget_finalize (GObject *object)
{
fprintf(stderr,"desktop lw_applet_finalize\n");
      /* Call the base class's implementation: */
     G_OBJECT_CLASS (animation_wallpaper_plugin_parent_class)->finalize (object);
}

static void
animation_wallpaper_plugin_class_init (AWallpaperPluginClass *klass) {

    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
    GtkObjectClass *gobject_class = GTK_OBJECT_CLASS (klass);
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    /* gobject */
    gobject_class->destroy = (gpointer)lw_applet_finalize;
    object_class->finalize = desktop_widget_finalize;
    widget_class->realize = lw_applet_realize;

    g_type_class_add_private (klass, sizeof (Animation_WallpaperPrivate));

}

static void
animation_wallpaper_plugin_class_finalize (AWallpaperPluginClass *class) {
}

Animation_WallpaperPrivate*
animation_wallpaper_plugin_new (void)
{
  return g_object_new (Animation_Wallpaper_TYPE_HOME_PLUGIN , NULL);
}

