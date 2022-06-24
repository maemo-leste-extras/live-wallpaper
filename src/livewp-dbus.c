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
#include "livewp-dbus.h"
/*******************************************************************************/
void
livewp_initialize_dbus(Animation_WallpaperPrivate *priv){
    gchar       *filter_string;
    DBusError   error;

    dbus_error_init (&error);
    /* Add D-BUS signal handler for 'status_changed' */
    priv->dbus_conn = (DBusConnection *) osso_get_sys_dbus_connection(priv->osso);
    priv->dbus_conn_session = (DBusConnection *) osso_get_dbus_connection(priv->osso);

    if (priv->dbus_conn_session){
        filter_string =
            g_strdup_printf("type='signal', interface='%s'", LIVEWP_SIGNAL_INTERFACE);
        dbus_bus_add_match(priv->dbus_conn_session, filter_string, &error);
        if (dbus_error_is_set(&error)){
             fprintf(stderr,"dbus_bus_add_match failed: %s", error.message);
             dbus_error_free(&error);
        }
        g_free(filter_string);
        /* add the callback */
        dbus_connection_add_filter(priv->dbus_conn_session,
                                   (DBusHandleMessageFunction)get_livewp_signal_cb,
                                   priv, NULL);
#if 0
       filter_string =
            g_strdup_printf("type='signal', interface='%s'", NOTIFY_SIGNAL_INTERFACE);
        dbus_bus_add_match(priv->dbus_conn_session, filter_string, &error);
        if (dbus_error_is_set(&error)){
             fprintf(stderr,"dbus_bus_add_match failed: %s", error.message);
             dbus_error_free(&error);
        }
        g_free(filter_string);
        /* add the callback */
        dbus_connection_add_filter(priv->dbus_conn_session,
                                   (DBusHandleMessageFunction)get_livewp_signal_cb,
                                   priv, NULL);
#endif

    }
}
/*******************************************************************************/
void
livewp_deinitialize_dbus(Animation_WallpaperPrivate *priv){

    gchar       *filter_string;
    DBusError   error;

    if (priv->dbus_conn_session){
        filter_string =
                g_strdup_printf("type='signal', interface='%s'", LIVEWP_SIGNAL_INTERFACE);

        dbus_error_init (&error);
        dbus_bus_remove_match(priv->dbus_conn_session, filter_string, &error);
        if (!dbus_error_is_set(&error)){
            dbus_connection_remove_filter(priv->dbus_conn_session,
                                          (DBusHandleMessageFunction)get_livewp_signal_cb, 
                                          NULL);
        }else{
      
            fprintf(stderr,"dbus_bus_add_match failed: %s", error.message);
            dbus_error_free(&error);
        }

         g_free(filter_string);
         /*
         filter_string =
                g_strdup_printf("type='signal', interface='%s'", NOTIFY_SIGNAL_INTERFACE);

        dbus_error_init (&error);
        dbus_bus_remove_match(priv->dbus_conn_session, filter_string, &error);
        if (!dbus_error_is_set(&error)){
            dbus_connection_remove_filter(priv->dbus_conn_session,
                                          (DBusHandleMessageFunction)get_livewp_signal_cb, 
                                          NULL);
        }else{
      
            fprintf(stderr,"dbus_bus_add_match failed: %s", error.message);
            dbus_error_free(&error);
        }

         g_free(filter_string);
*/
    }

}
/*******************************************************************************/
void
send_dbus_signal (Animation_WallpaperPrivate *priv,
                  const gchar *interface,
                  const gchar *path,
                  const gchar *member)
{
  gboolean       success;
/*    
  gint param = 0;
  dbus_uint32_t serial = 0;
  DBusMessageIter args;
*/
  
  DBusMessage *message = dbus_message_new (DBUS_MESSAGE_TYPE_SIGNAL);
  dbus_message_set_interface (message, interface);
  dbus_message_set_path (message, path);
  dbus_message_set_member (message, member);
  /*
  if (member == LIVEWP_PLAY_LIVEBG_ON_VIEW ||
      member == LIVEWP_PAUSE_LIVEBG_ON_VIEW){
      param = priv->view + 1;

      fprintf(stderr, "dbus send try send %s %i\n", member, param);
      dbus_message_iter_init_append(message, &args);
      if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &param)){
          fprintf(stderr, "error append param\n");
      }
  }
  if (!dbus_connection_send(priv->dbus_conn_session, message, &serial)){
      fprintf(stderr, "error send\n");
  }
  dbus_connection_flush(priv->dbus_conn_session);
  */
  success = dbus_connection_send (priv->dbus_conn_session, message, NULL);
  dbus_connection_flush(priv->dbus_conn_session);
  dbus_message_unref (message);
  
#if 0
  fprintf (stderr, "%s '%s' message.\n",
                                 success ? "Sent" : "Failed to send",
                                 member);
#endif

}

/*******************************************************************************/
DBusHandlerResult
get_livewp_signal_cb(DBusConnection *conn, DBusMessage *msg, Animation_WallpaperPrivate *priv)
{

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

#if 0
#if  defined APPLICATION 
    fprintf (stderr, "APPLICATION PATH11111111111111111111 %s %s %s\n",   dbus_message_get_path(msg),   dbus_message_get_interface (msg), dbus_message_get_member (msg)); 
#endif

#if  defined CONTROLPANEL
    fprintf (stderr, "APPLICATION PATH11111111111111111111 %s %s %s\n",   dbus_message_get_path(msg),   dbus_message_get_interface (msg), dbus_message_get_member (msg)); 
#endif

#if !(defined CONTROLPANEL || defined APPLICATION) 
    fprintf (stderr, "APPLET PATH11111111111111111111 %s %s %s\n",   dbus_message_get_path(msg),   dbus_message_get_interface (msg), dbus_message_get_member (msg)); 
#endif
#endif
    if (!priv)
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

    if (dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_RELOAD_CONFIG)){
        if(read_config(priv)){
                fprintf(stderr, "\nCan not read config file.\n");
        }else{
            read_config(priv);
#if  defined APPLICATION 
            reload_scene(priv->desktop_plugin);
#endif
        }
    }
#if  defined APPLICATION
    /* QUIT FROM APPLICATION */
    if ((priv->view == 1 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_STOP_LIVEBG_ON_VIEW1))||
        (priv->view == 2 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_STOP_LIVEBG_ON_VIEW2))||   
        (priv->view == 3 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_STOP_LIVEBG_ON_VIEW3))||   
        (priv->view == 4 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_STOP_LIVEBG_ON_VIEW4))||   
        (priv->view == 5 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_STOP_LIVEBG_ON_VIEW5))||   
        (priv->view == 6 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_STOP_LIVEBG_ON_VIEW6))||   
        (priv->view == 7 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_STOP_LIVEBG_ON_VIEW7))||   
        (priv->view == 8 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_STOP_LIVEBG_ON_VIEW8))||   
        (priv->view == 9 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_STOP_LIVEBG_ON_VIEW9))){

        quit_from_program(priv);
    }
    /* PLAY SCENE */
    if ((priv->view == 1 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PLAY_LIVEBG_ON_VIEW1))||
        (priv->view == 2 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PLAY_LIVEBG_ON_VIEW2))||   
        (priv->view == 3 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PLAY_LIVEBG_ON_VIEW3))||   
        (priv->view == 4 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PLAY_LIVEBG_ON_VIEW4))||   
        (priv->view == 5 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PLAY_LIVEBG_ON_VIEW5))||   
        (priv->view == 6 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PLAY_LIVEBG_ON_VIEW6))||   
        (priv->view == 7 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PLAY_LIVEBG_ON_VIEW7))||   
        (priv->view == 8 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PLAY_LIVEBG_ON_VIEW8))||   
        (priv->view == 9 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PLAY_LIVEBG_ON_VIEW9))){
        /* fprintf(stderr, "Play scene visible %i\n", priv->visible); */
        if (priv->visible != TRUE){
            priv->visible = TRUE;
            view_state_changed(priv);
        }
    }
    /* PAUSE SCENE */
    if ((priv->view == 1 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PAUSE_LIVEBG_ON_VIEW1))||
        (priv->view == 2 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PAUSE_LIVEBG_ON_VIEW2))||   
        (priv->view == 3 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PAUSE_LIVEBG_ON_VIEW3))||   
        (priv->view == 4 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PAUSE_LIVEBG_ON_VIEW4))||   
        (priv->view == 5 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PAUSE_LIVEBG_ON_VIEW5))||   
        (priv->view == 6 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PAUSE_LIVEBG_ON_VIEW6))||   
        (priv->view == 7 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PAUSE_LIVEBG_ON_VIEW7))||   
        (priv->view == 8 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PAUSE_LIVEBG_ON_VIEW8))||   
        (priv->view == 9 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_PAUSE_LIVEBG_ON_VIEW9))){
         /* fprintf(stderr, "Pause scene visible %i\n", priv->visible); */
         if (priv->visible != FALSE){
            priv->visible = FALSE;
            view_state_changed(priv);
         }
    }
    /* ALIVE APPLICATION */
    if ((priv->view == 1 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_ALIVE_LIVEBG_ON_VIEW1))||
        (priv->view == 2 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_ALIVE_LIVEBG_ON_VIEW2))||   
        (priv->view == 3 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_ALIVE_LIVEBG_ON_VIEW3))||   
        (priv->view == 4 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_ALIVE_LIVEBG_ON_VIEW4))||   
        (priv->view == 5 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_ALIVE_LIVEBG_ON_VIEW5))||   
        (priv->view == 6 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_ALIVE_LIVEBG_ON_VIEW6))||   
        (priv->view == 7 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_ALIVE_LIVEBG_ON_VIEW7))||   
        (priv->view == 8 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_ALIVE_LIVEBG_ON_VIEW8))||   
        (priv->view == 9 && dbus_message_is_signal(msg, LIVEWP_SIGNAL_INTERFACE, LIVEWP_ALIVE_LIVEBG_ON_VIEW9))){

        priv->last_alive_event = time(NULL);
    }

    /* Check notification of mail, sms, call */
  //fprintf(stderr, "Type %i\n", dbus_message_get_type (msg));
    if (dbus_message_is_method_call(msg, NOTIFY_SIGNAL_INTERFACE, NOTIFY_MEMBER)){
        //fprintf(stderr,"read notifications.db notify\n");    
        //read_notification(priv->desktop_plugin);
        priv->scene->notification = time(NULL) + 5;
    }
    if (dbus_message_is_signal(msg, NOTIFY_SIGNAL_INTERFACE, CLOSENOTIFY_MEMBER)){
        //fprintf(stderr,"read notifications.db close notify\n");    
        //read_notification(priv->desktop_plugin);
        priv->scene->notification = time(NULL) + 5;
    }
#endif

    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

