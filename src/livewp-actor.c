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
#include "livewp-actor.h"

static void
realize (GtkWidget *widget)
{
    GdkScreen *screen;
    screen = gtk_widget_get_screen (widget);
    gtk_widget_set_colormap (widget, gdk_screen_get_rgba_colormap (screen));
}

static gboolean
expose_event (GtkWidget *widget,GdkEventExpose *event,
     gpointer data)
{
    cairo_t *cr;
    GdkPixbuf *pixbuf = (GdkPixbuf *) data;

    cr = gdk_cairo_create(widget->window);
    if (cr){
        gdk_cairo_region(cr, event->region);
        cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
        gdk_cairo_set_source_pixbuf(cr, pixbuf, 0.0, 0.0);
        cairo_paint(cr);
        cairo_destroy(cr);
    }
    return TRUE;
}

void
destroy_hildon_actor(Actor *actor)
{
    //fprintf(stderr, "destroy_hildon_actor %s\n",actor->name);
    gtk_widget_destroy(actor->widget);
    actor->widget = NULL;
}

void
create_hildon_actor_text(Actor *actor, AWallpaperPlugin *desktop_plugin)
{
  GtkWidget *ha = NULL;
  GtkWidget *label = NULL;

  ha = hildon_animation_actor_new();
  label = gtk_label_new(NULL);

  if (label){
    //g_signal_connect(G_OBJECT(label), "expose_event", G_CALLBACK(expose_event), NULL);

    gtk_container_add (GTK_CONTAINER (ha), label);
  }
  realize(ha);
  gtk_widget_show(label);
  gtk_widget_show_all(ha);

  /* TO DO check it */
  /*  gdk_flush (); */

  //g_object_set_data(G_OBJECT(ha), "image", image);
  actor->image = label;
  hildon_animation_actor_set_parent (HILDON_ANIMATION_ACTOR (ha), GTK_WINDOW(desktop_plugin->priv->window));
  actor->widget = ha;
  set_actor_position(actor, actor->x, actor->y, actor->z, desktop_plugin);
  set_actor_scale(actor, (double)actor->scale/100, (double)actor->scale/100);
  set_actor_visible(actor, actor->visible);
}

void
create_hildon_actor(Actor *actor, AWallpaperPlugin *desktop_plugin)
{
  GtkWidget *ha = NULL;
  GdkPixbuf *pixbuf = NULL;
  GtkWidget *image = NULL;
  gchar     *str = NULL;

  ha = hildon_animation_actor_new();
  if (!strcmp(actor->name, "original"))
      str = g_strdup(actor->filename);
  else
      str = g_strdup_printf( "%s/%s/%s", THEME_PATH,
                        desktop_plugin->priv->theme, actor->filename);
  /* fprintf(stderr, "create hildon actor file=%s\n", str); */
  pixbuf = gdk_pixbuf_new_from_file_at_size (str,
                                             actor->width,
                                             actor->height,
                                             NULL);
  if (str)
      g_free(str);
  if (pixbuf){
      image = gtk_image_new_from_pixbuf (pixbuf);
      g_object_unref(G_OBJECT(pixbuf));
  }
  if (image){
    g_signal_connect(G_OBJECT(image), "expose_event",
                           G_CALLBACK(expose_event), pixbuf);
    gtk_container_add (GTK_CONTAINER (ha), image);
  }
  realize(ha);
  gtk_widget_show_all(ha);

  /* TO DO check it */
  /*  gdk_flush (); */

  //g_object_set_data(G_OBJECT(ha), "image", image);
  actor->image = image;
  hildon_animation_actor_set_parent (HILDON_ANIMATION_ACTOR (ha), GTK_WINDOW(desktop_plugin->priv->window));

  actor->widget = ha;
  set_actor_position(actor, actor->x, actor->y, actor->z, desktop_plugin);
  set_actor_scale(actor, (double)actor->scale/100, (double)actor->scale/100);
  set_actor_visible(actor, actor->visible);
}

void
change_hildon_actor(Actor *actor, AWallpaperPlugin *desktop_plugin)
{
    GtkWidget *image = NULL;
    GdkPixbuf *pixbuf = NULL;
    gchar     *str = NULL;

    str = g_strdup_printf( "%s/%s/%s", THEME_PATH,
                            desktop_plugin->priv->theme, actor->filename);

    pixbuf = gdk_pixbuf_new_from_file_at_size (str,
                                               actor->width,
                                               actor->height,
                                               NULL);
    if(str)
        g_free(str);
    if (pixbuf){
        image = gtk_image_new_from_pixbuf (pixbuf);
        g_object_unref(G_OBJECT(pixbuf));
    }
    if (image){
        g_signal_connect(G_OBJECT(image), "expose_event",
                                       G_CALLBACK(expose_event), pixbuf);
        //if (g_object_get_data(G_OBJECT(actor->widget), "image")){
        if (actor->image){
            gtk_container_remove(GTK_CONTAINER(actor->widget), actor->image);
        }
        //g_object_set_data(G_OBJECT(actor->widget), "image", image);
        actor->image = image;
        gtk_container_add (GTK_CONTAINER (actor->widget), image);
        realize(actor->widget);
        gtk_widget_show_all(actor->widget);
        /* TO DO check it */
       /*  gdk_flush (); */


    }
}

Actor*
init_object(AWallpaperPlugin *desktop_plugin,
            gchar * name,
            gchar * filename,
            gint x,
            gint y,
            gint z,
            gint width,
            gint height,
            gboolean visible,
            gboolean load_image,
            gint scale,
            gint opacity,
            void (*pfunc_change)(Actor*),
            void (*pfunc_probability)(Actor*),
            GPtrArray *child
           )
{
    Actor *actor = NULL;
    actor = g_new0(Actor, 1);
    actor->x = x;
    actor->y = y;
    actor->z = z;
    actor->width = width;
    actor->height = height;
    actor->visible = visible;
    actor->scale = scale;
    actor->opacity = opacity;
    actor->filename = g_strdup(filename);
    actor->name = g_strdup(name);
    actor->func_change = (gpointer)pfunc_change;
    actor->func_probability = (gpointer)pfunc_probability;
    actor->child = child;
    if (load_image){
        create_hildon_actor(actor, desktop_plugin);
    }
    else
         actor->widget = NULL;
    actor->time_start_animation = 0;
    actor->duration_animation = 0;
    return actor;
}

void
destroy_actor(Actor *actor)
{
    if (actor){
        if (actor->child){
            g_ptr_array_free(actor->child, TRUE);
        }
        if (actor->filename)
            g_free(actor->filename);
        if (actor->name)
            g_free(actor->name);
        gtk_widget_destroy(actor->widget);
        //actor->widget = NULL;
        g_free(actor);
    }
}
static gint
path_line(gint x0, gint x1, double t)
{
    // уравниение прямой
    return ((x1 - x0) * t + x0);
}
void
set_actor_scale(Actor *actor, double scalex, double scaley)
{
    hildon_animation_actor_set_scale(
            HILDON_ANIMATION_ACTOR(actor->widget),
            scalex,
            scaley
    );

}
void
set_actor_opacity(Actor *actor, gint opacity)
{
    hildon_animation_actor_set_opacity(
            HILDON_ANIMATION_ACTOR(actor->widget),
            opacity
    );

}

void
set_actor_rotation(Actor *actor, gint axis, double degrees, gint x, gint y, gint z)
{
    hildon_animation_actor_set_rotation(
            HILDON_ANIMATION_ACTOR(actor->widget),
            axis,
            degrees,
            x,
            y,
            z
    );
}
void
set_actor_visible(Actor *actor, gboolean visible)
{
    hildon_animation_actor_set_show(HILDON_ANIMATION_ACTOR(actor->widget), visible);
}

void
set_actor_position(Actor *actor, gint x, gint y, gint z, AWallpaperPlugin *desktop_plugin)
{
    //fprintf(stderr, "set actor position %d, %d, %d\n", x, y, z);
    hildon_animation_actor_set_position_full(HILDON_ANIMATION_ACTOR (actor->widget),
                                             x-desktop_plugin->priv->xapplet,
                                             y-desktop_plugin->priv->yapplet,
                                             z);
}

int get_notify_count(gchar *notify_type)
{
    sqlite3 *db = NULL;
    sqlite3_stmt *res = NULL;
    gint rc = 0, result = 0;
    gchar sql[1024];

    rc = sqlite3_open("/home/user/.config/hildon-desktop/notifications.db", &db);
    if (rc){
        fprintf(stderr, "error open db %d %s\n", rc, sqlite3_errmsg(db));
    }else {
        snprintf(sql, sizeof(sql)-1, "select count(id) from notifications where icon_name='%s'", notify_type);
        rc = sqlite3_prepare(db, sql, sizeof(sql)-1, &res, NULL);
        if (rc != SQLITE_OK){
            fprintf(stderr, "error prepare %d %s\n", rc, sql);
        }
        if (sqlite3_step(res) != SQLITE_ROW){
            fprintf(stderr, "not sqlite_row\n");
        }
        result = sqlite3_column_int(res, 0);
        //fprintf(stderr, "count missing calls = %d\n", call_count);
        sqlite3_finalize(res);

        sqlite3_close(db);
    }
    return result;
}
gchar * read_notification()
{
    gchar *message = "";
    gint count = 0;

    fprintf(stderr, "read notification \n");
    count = get_notify_count("general_missed");
    if (count > 0){
        message = g_strdup_printf("%s: %d", _("Missed calls"), count);
    }
    count = get_notify_count("general_sms");
    if (count > 0){
        if (message){
            message = g_strdup_printf("%s \n%s: %d", message, _("Missed sms"), count);
        }else {
            message = g_strdup_printf("%s: %d", _("Missed sms"), count);
        }
    }
    count = get_notify_count("general_chat");
    if (count > 0){
        if (message){
            message = g_strdup_printf("%s \n%s: %d", message, _("Missed chat"), count);
        }else {
            message = g_strdup_printf("%s: %d", _("Missed chat"), count);
        }
    }
    count = get_notify_count("qgn_list_messagin");
    if (count > 0){
        if (message){
            message = g_strdup_printf("%s \n%s: %d", message, _("Missed mail"), count);
        }else {
            message = g_strdup_printf("%s: %d", _("Missed mail"), count);
        }
    }
    fprintf(stderr, "notify=%s\n", message);
    return message;
}

void
change_obj(Actor *actor, AWallpaperPlugin *desktop_plugin)
{
    char * accel_filename = "/sys/class/i2c-adapter/i2c-3/3-001d/coord";
    //char * accel_filename = "/home/tanya/coord";

    FILE *fd = NULL;
    int rs, ax, ay, az, dx, dy;
    fd = fopen(accel_filename, "r");
    if (fd == NULL){
        fprintf(stderr, "cannot open file\n");
        return;
    }
    rs = fscanf((FILE*)fd, "%i %i %i", &ax, &ay, &az);
    fclose(fd);
    if (rs != 3){
        fprintf(stderr, "cannot read information from file\n");
        return;
    }

    fprintf(stderr, "change obj %i %i %i\n", ax, ay, az);
    dx = -ax / 100;
    dy = -ay / 100;

    actor->x = actor->x + dx;
    actor->y = actor->y + dy;

    if (actor->x > 800) actor->x = 0;
    if (actor->x < 0) actor->x = 800;

    if (actor->y > 480) actor->y = 0;
    if (actor->y < 0) actor->y = 480;

    set_actor_position(actor, actor->x, actor->y, actor->z, desktop_plugin);


}

void
change_billboard(Actor * actor, AWallpaperPlugin *desktop_plugin)
{
    gint count = 0;
    Actor *a = NULL;

    //fprintf(stderr, "change_billboard\n");

    if (desktop_plugin->priv->scene->notification < time(NULL)){
        count = get_notify_count("general_missed");
        a = g_ptr_array_index(actor->child, 0);
        if (count > 0){
            set_actor_visible(a, TRUE);
        }else {
            set_actor_visible(a, FALSE);
        }
        count = get_notify_count("general_sms");
        a = g_ptr_array_index(actor->child, 3);
        if (count > 0){
            set_actor_visible(a, TRUE);
        }else {
            set_actor_visible(a, FALSE);
        }
        count = get_notify_count("general_chat");
        a = g_ptr_array_index(actor->child, 1);
        if (count > 0){
            set_actor_visible(a, TRUE);
        }else {
            set_actor_visible(a, FALSE);
        }
        count = get_notify_count("qgn_list_messagin");
        a = g_ptr_array_index(actor->child, 2);
        if (count > 0){
            set_actor_visible(a, TRUE);
        }else {
            set_actor_visible(a, FALSE);
        }

        desktop_plugin->priv->scene->notification = FALSE;
    }
    actor->time_start_animation = time(NULL) + 20;
}

#if 0
void
change_billboard1(Actor * actor, AWallpaperPlugin *desktop_plugin)
{
    GtkWidget *label;
    sqlite3 *db = NULL;
    sqlite3_stmt *res = NULL;
    gchar *errMsg = NULL, *message;
    gchar sql[1024];
    gint call_count=0, sms_count=0, rc=0;
    GtkListStore *list = NULL;
    PangoFontDescription *pfd = NULL;

    rc = sqlite3_open("/home/user/.rtcom-eventlogger/el.db", &db);
    if (rc){
        fprintf(stderr, "error open db %d %s\n", rc, sqlite3_errmsg(db));
    }else {
        snprintf(sql, sizeof(sql)-1, "select count(id) from Events where event_type_id=%d", 3);

        rc = sqlite3_prepare(db, sql, sizeof(sql)-1, &res, NULL);
        if (rc != SQLITE_OK){
            fprintf(stderr, "error prepare %d %s\n", rc, sql);
        }
        if (sqlite3_step(res) != SQLITE_ROW){
            fprintf(stderr, "not sqlite_row\n");
        }
        call_count = sqlite3_column_int(res, 0);
        //fprintf(stderr, "count missing calls = %d\n", call_count);
        sqlite3_finalize(res);

        snprintf(sql, sizeof(sql)-1, "select count(id) from Events where event_type_id=%d and is_read=%d", 7, 0);
        rc = sqlite3_prepare(db, sql, sizeof(sql)-1, &res, NULL);
        if (rc != SQLITE_OK){
            fprintf(stderr, "error prepare %d %s\n", rc, sql);
        }
        if (sqlite3_step(res) != SQLITE_ROW){
            fprintf(stderr, "not sqlite_row\n");
        }
        sms_count = sqlite3_column_int(res, 0);
        //fprintf(stderr, "count sms = %d\n", sms_count);
        sqlite3_finalize(res);


        sqlite3_close(db);
    }
    label = actor->image;
    message = g_markup_printf_escaped("<span bgcolor=\"%s\" foreground=\"%s\">Missed calls: %d Unread sms: %d</span>", "#FFFFFF", "#000000", call_count, sms_count);
    gtk_label_set_markup(GTK_LABEL(label), message);
    g_free(message);
    pfd = pango_font_description_from_string("Sans 14");
    gtk_widget_modify_font(GTK_WIDGET(label), NULL);
    gtk_widget_modify_font(GTK_WIDGET(label), pfd);
    pango_font_description_free(pfd);
    actor->time_start_animation = time(NULL) + 20;
}
#endif

void
change_moon(Actor * actor, AWallpaperPlugin *desktop_plugin)
{
    gint phase;
    char *newfile;
    gint x0 = 150,
         x1 = 650,
         x, y;
    struct timeval tvb;
    suseconds_t ms;
    long sec;
    double t;
#if 0
    gint y0, y1, x2, y2;
    double a, b, c;
    a = (double)(y2 - (double)(x2*(y1-y0) + x1*y0 - x0*y1)/(x1-x0))/(x2*(x2-x0-x1)+x0*x1);
    b = (double)(y1-y0)/(x1-x0) - (double)a*(x0+x1);
    c = (double)(x1*y0 - x0*y1)/(x1-x0) + (double)a*x0*x1;
    fprintf(stderr, "a=%f, b=%f, c=%f\n", a, b, c);
#endif
    gettimeofday(&tvb, NULL);

    ms = tvb.tv_usec;
    sec = tvb.tv_sec;

    if (actor){
        if (desktop_plugin->priv->scene->daytime == TIME_NIGHT){
            if (!actor->visible){
                actor->visible = TRUE;
                phase = get_moon_phase();
                newfile = g_strdup_printf( "%s%d.png", actor->name, phase);
                if (actor->filename)
                    g_free(actor->filename);
                actor->filename = newfile;
                actor->time_start_animation = sec - fast_rnd(60 * 60);
                actor->duration_animation = 1 * 60 * 60;
                create_hildon_actor(actor, desktop_plugin);

            }
            t = (double)((double)sec+(double)ms/1000000 - actor->time_start_animation) / actor->duration_animation;
            if (t <= 1)
                x = path_line(x0, x1, t);
            else
                x = path_line(x1, x0, t-1);
            y = 0.001920*x*x - 1.536*x + 337.2;
            //y = a*x*x + b*x + c;

            set_actor_position(actor, x, y, actor->z, desktop_plugin);

            if (t>=2){
                actor->time_start_animation = sec;
            }

         }else if (actor->visible){
            actor->visible = FALSE;
            fprintf(stderr, "destroy moon \n");
            destroy_hildon_actor(actor);
            actor->time_start_animation = 0;
        }
    }

}

void
change_sun(Actor * actor, AWallpaperPlugin *desktop_plugin)
{
    double alt, azm;
    gint x, y;

    //fprintf(stderr, "change sun\n");
    if (actor){
        if (desktop_plugin->priv->scene->daytime != TIME_NIGHT){
            if (!actor->visible){
                actor->visible = TRUE;
                create_hildon_actor(actor, desktop_plugin);
            }
            get_sun_pos(&alt, &azm);
            get_sun_screen_pos(alt, azm, &x, &y);
            actor->x = x;
            actor->y = y;
            set_actor_position(actor, x, y, actor->z, desktop_plugin);
            actor->time_start_animation = time(NULL) + 60;
         }else if (actor->visible){
            actor->visible = FALSE;
            destroy_hildon_actor(actor);
            actor->time_start_animation = 0;
        }
    }

}

void
change_tram(Actor * actor, AWallpaperPlugin *desktop_plugin)
{
    gint x0 = -300, y0 = 225, scale0 = 100,
         x1 = 800, y1 = 162, scale1 = 130,
         x, y, scale;
    struct timeval tvb;
    suseconds_t ms;
    long sec;
    double t;

    //fprintf(stderr, "change tram\n");
    gettimeofday(&tvb, NULL);

    ms = tvb.tv_usec;
    sec = tvb.tv_sec;

    if (!actor->visible){
        actor->visible = TRUE;
        if (desktop_plugin->priv->scene->daytime == TIME_NIGHT){
            if (actor->filename)
                g_free(actor->filename);
            actor->filename = g_strdup("tram_dark.png");
        } else{
            if (actor->filename)
                g_free(actor->filename);
            actor->filename = g_strdup("tram.png");
        }
        create_hildon_actor(actor, desktop_plugin);
    }
    t = (double)((double)sec+(double)ms/1000000 - actor->time_start_animation) / actor->duration_animation;
    x = path_line(x0, x1, t);
    y = path_line(y0, y1, t);
    scale = path_line(scale0, scale1, t);
    set_actor_position(actor, x, y, actor->z, desktop_plugin);
    set_actor_scale(actor, (double)scale/100, (double)scale/100);
    if (t >= 1){
        /* stop animation */
        actor->visible = FALSE;
        destroy_hildon_actor(actor);
        actor->time_start_animation = sec + fast_rnd(60);
    }
}

void
change_plane1(Actor *actor, AWallpaperPlugin *desktop_plugin)
{
    gint x0 = 620, y0 = 233,
         x1 = 79, y1 = -146,
         x, y;
    struct timeval tvb;
    suseconds_t ms;
    long sec;
    double t;

    gettimeofday(&tvb, NULL);

    ms = tvb.tv_usec;
    sec = tvb.tv_sec;
//    fprintf(stderr, "1 %f - %d\n", sec+(double)ms/100000, now);

    if (desktop_plugin->priv->scene->daytime != TIME_NIGHT){
        if (actor->time_start_animation == 0){
            actor->time_start_animation = sec + fast_rnd(180);
            return;
        }
    }
    if (!actor->visible){
        actor->visible = TRUE;
        create_hildon_actor(actor, desktop_plugin);
    }
    t = (double)((double)sec+(double)ms/1000000 - actor->time_start_animation) / actor->duration_animation;
    x = path_line(x0, x1, t);
    y = path_line(y0, y1, t);
    //scale = path_line(scale0, scale1, t);
    set_actor_position(actor, x, y, actor->z, desktop_plugin);
    if (t >= 1){
        /* stop animation */
        actor->visible = FALSE;
        destroy_hildon_actor(actor);
        if (desktop_plugin->priv->scene->daytime == TIME_NIGHT)
            actor->time_start_animation = 0;
        else
            actor->time_start_animation = sec + fast_rnd(180);
    }

}

void
change_plane2(Actor *actor, AWallpaperPlugin *desktop_plugin)
{
    gint x0 = -actor->width, y0 = 45,
         x1 = 800, y1 = 20,
         x, y;
    struct timeval tvb;
    suseconds_t ms;
    long sec;
    double t;

    gettimeofday(&tvb, NULL);

    ms = tvb.tv_usec;
    sec = tvb.tv_sec;
//    fprintf(stderr, "1 %f - %d\n", sec+(double)ms/100000, now);
    if (desktop_plugin->priv->scene->daytime != TIME_NIGHT){
        if (actor->time_start_animation == 0){
            actor->time_start_animation = sec + fast_rnd(180);
            return;
        }
    }
    if (!actor->visible){
        actor->visible = TRUE;
        create_hildon_actor(actor, desktop_plugin);
    }

    t = (double)((double)sec+(double)ms/1000000 - actor->time_start_animation) / actor->duration_animation;
    x = path_line(x0, x1, t);
    y = path_line(y0, y1, t);
    //scale = path_line(scale0, scale1, t);
    set_actor_position(actor, x, y, actor->z, desktop_plugin);
    if (t >= 1){
        /* stop animation */
        actor->visible = FALSE;
        destroy_hildon_actor(actor);
        if (desktop_plugin->priv->scene->daytime == TIME_NIGHT)
            actor->time_start_animation = 0;
        else
            actor->time_start_animation = sec + fast_rnd(180);
    }

}

void
change_cloud(Actor *actor, AWallpaperPlugin *desktop_plugin)
{
    gint x0, y0 = 300, scale0 = 100,
         x1, y1 = -actor->height, scale1 = 150,
         x, y, scale;
    struct timeval tvb;
    suseconds_t ms;
    long sec;
    double t;
    gchar *newfile;

    //fprintf(stderr, "change cloud\n");
    gettimeofday(&tvb, NULL);

    ms = tvb.tv_usec;
    sec = tvb.tv_sec;

    if (!actor->visible){
        actor->visible = TRUE;
        if (desktop_plugin->priv->scene->daytime == TIME_NIGHT){
            newfile = g_strdup_printf("%s_dark.png", actor->name);
        }else{
            newfile = g_strdup_printf("%s.png", actor->name);
        }
        if (actor->filename)
            g_free(actor->filename);
        actor->filename = newfile;

        create_hildon_actor(actor, desktop_plugin);
    }
    t = (double)((double)sec+(double)ms/1000000 - actor->time_start_animation) / actor->duration_animation;

    if (desktop_plugin->priv->scene->wind_orientation == 1){
        x0 = -actor->width;
        x1 = 800;
    }
    else {
        x0 = 800;
        x1 = -actor->width;
    }

    x = path_line(x0, x1, t);
    y = -desktop_plugin->priv->scene->wind_angle * (x - x0) + actor->y;
    scale = path_line(scale0, scale1, (double)(y - y0)/(y1 - y0));

    set_actor_position(actor, x, y, actor->z, desktop_plugin);
    set_actor_scale(actor, (double)scale/100, (double)scale/100);
    if ((y < y1 || y > y0) || t >= 1){
        /* stop animation */
        actor->visible = FALSE;
        destroy_hildon_actor(actor);
        actor->time_start_animation = sec + fast_rnd(300);
        actor->y = fast_rnd(300);
    }

}

void
change_wind(Actor *actor, AWallpaperPlugin *desktop_plugin)
{
    desktop_plugin->priv->scene->wind_orientation = fast_rnd(2);
    if (desktop_plugin->priv->scene->wind_orientation == 0) desktop_plugin->priv->scene->wind_orientation = -1;
    desktop_plugin->priv->scene->wind_angle = (double)(fast_rnd(200) - 100) / 100;
    actor->time_start_animation = time(NULL) + (fast_rnd(10) + 10) * 60;
    //fprintf(stderr, "change wind orient = %d angle = %f after = %d\n", scene.wind_orientation, scene.wind_angle, actor->time_start_animation-time(NULL));
}

void
change_window1(Actor * actor, AWallpaperPlugin *desktop_plugin)
{
    gint now = time(NULL);
    if (desktop_plugin->priv->scene->daytime == TIME_DAY){
        if (actor->widget){
            actor->visible = FALSE;
            destroy_hildon_actor(actor);
        }
        actor->time_start_animation = 0;
        return;
    }else {
        if (!actor->widget)
            create_hildon_actor(actor, desktop_plugin);
        if (actor->time_start_animation == 0){
            actor->time_start_animation = now + fast_rnd(30);
            return;
        }
    }

    if (!actor->visible)
        actor->visible = TRUE;
    else
        actor->visible = FALSE;
    set_actor_visible(actor, actor->visible);
    actor->time_start_animation = now + fast_rnd(60) + 10;

}

void
change_signal(Actor * actor, AWallpaperPlugin *desktop_plugin)
{
    gint now = time(NULL);
    Actor *a;
    a = g_ptr_array_index(actor->child, 0);
    if (a->visible)
        a->visible = FALSE;
    else
        a->visible = TRUE;
    set_actor_visible(a, a->visible);

    a = g_ptr_array_index(actor->child, 1);
    if (a->visible)
        a->visible = FALSE;
    else
        a->visible = TRUE;
    set_actor_visible(a, a->visible);

    actor->time_start_animation = now + fast_rnd(30) + 10;
}

void
change_tape(Actor *actor, AWallpaperPlugin *desktop_plugin)
{
    gint x, y, i;
    Actor *a;

    if (!desktop_plugin->priv->rich_animation) return;

    char * accel_filename = "/sys/class/i2c-adapter/i2c-3/3-001d/coord";
    //char * accel_filename = "/home/tanya/coord";

    FILE *fd = NULL;
    int rs, ax, ay, az;
    fd = fopen(accel_filename, "r");
    if (fd == NULL){
        //fprintf(stderr, "cannot open file\n");
        fd = fopen("/home/user/coord", "r");
    }
    rs = fscanf((FILE*)fd, "%i %i %i", &ax, &ay, &az);
    fclose(fd);
    if (rs != 3){
        fprintf(stderr, "cannot read information from file\n");
        return;

    }

    //fprintf(stderr, "change obj %i %i %i angle rad=%f, deg=%f\n", ax, ay, az, atan2(ax, -ay), atan2(ax, -ay)*180/M_PI);
    int ang = (int)floor(atan2(ay, ax)*180/M_PI);
    if (ang < 0) ang = 360+ang;

    if (!desktop_plugin->priv->rich_animation) return;

    for (i=0; i<16; i++){
        a = g_ptr_array_index(actor->child, i);
        if (a->scale == 100) a->scale = ang;
        if (abs(a->scale - ang) > 10){
            if (a->scale > ang){
                if ((a->scale - ang) < (ang + (360-a->scale))) a->scale--;
                else a->scale++;
            }
            if (a->scale < ang) {
                if (ang - a->scale < (a->scale+(360-ang))) a->scale++;
                else a->scale--;
            }
            if (a->scale > 360) a->scale = 0;
            if (a->scale < 0) a->scale = 360;
        }

        x = a->x - (float)cos(a->scale*M_PI/180)*a->z;
        y = a->y - (float)sin(a->scale*M_PI/180)*a->z;
        //x = round(a->x - (float)cos(a->scale*M_PI/180)*a->z);
        //y = round(a->y - (float)sin(a->scale*M_PI/180)*a->z);
        //x = a->x - cos(angle)*a->z;
        //y = a->y - sin(angle)*a->z;
        if ((a->scale > 270 || a->scale < 90) && x < -a->width*cos(a->scale*M_PI/180)){
            x = 800;
            y = fast_rnd(480);
        }
        if ((a->scale > 90 && a->scale < 270) && x > 800 - a->width*cos(a->scale*M_PI/180)){
            x = 0;
            y = fast_rnd(480);
        }
        if (a->scale > 0 && a->scale < 180 && y < -a->width*sin(a->scale*M_PI/180)){
            y = 480;
            x = fast_rnd(800);
        }
        if (a->scale < 360 && a->scale > 180 && y > 480 - a->width*sin(a->scale*M_PI/180)){
            y = 0;
            x = fast_rnd(800);
        }
        //if (i ==0) fprintf(stderr, "x=%d y=%d ang=%d speed=%d\n", x, y, a->scale, a->z);
        set_actor_rotation(a, HILDON_AA_Z_AXIS, a->scale, 0, 0, 0);
        set_actor_position(a, x, y, a->z, desktop_plugin);
        a->x = x;
        a->y = y;
    }

}

void
change_slideshow(Actor *actor, AWallpaperPlugin *desktop_plugin)
{
	gint num, next, prev, now, w, h;
	Actor *a1 = NULL, *a2 = NULL;
    GtkImage *image;
    GdkPixbuf *pixbuf;
    struct timeval tvb;
    suseconds_t ms;
    long sec;
    double t;

    now = time(NULL);
	num = actor->scale; // total number of images
	prev = actor->opacity; // number of current image
    next = actor->z; // number of new image
    if (next < 0){
        // new image not selected, first step of animation
	    srand(now);
	    next = rand()%num;
	    while (num > 1 && next == prev)
		    next = rand()%num;
        a2 = g_ptr_array_index(actor->child, next);
	    a2->visible = TRUE;
	    create_hildon_actor(a2, desktop_plugin);
        set_actor_opacity(a2, 0);
        pixbuf = gtk_image_get_pixbuf(a2->image);
        w = gdk_pixbuf_get_width(pixbuf);
        h = gdk_pixbuf_get_height(pixbuf);
        if (w < 800) {
            set_actor_position(a2, (800-w)/2, 0, 5, desktop_plugin);
        }
        if (h < 480){
            set_actor_position(a2, 0, (480-h)/2, 5, desktop_plugin);
        }
        actor->z = next;
    }else {
        a2 = g_ptr_array_index(actor->child, next);
    }
	    
    gettimeofday(&tvb, NULL);

    ms = tvb.tv_usec;
    sec = tvb.tv_sec;

    t = (double)((double)sec+(double)ms/1000000 - actor->time_start_animation) / actor->duration_animation;
    
    set_actor_opacity(a2, path_line(0, 255, t));

	if (prev > -1){
		a1 = g_ptr_array_index(actor->child, prev);
        set_actor_opacity(a1, path_line(255, 0, t));
	}
    if (t >= 1){
        // last step of animation
		actor->opacity = next;
        actor->z = -1;
        if (prev > -1){
		    a1->visible = FALSE;
		    destroy_hildon_actor(a1);
        }
	    actor->time_start_animation = now + desktop_plugin->priv->theme_int_parametr1;
    }
}

void
change_layer(Actor * actor, AWallpaperPlugin *desktop_plugin)
{
    gint y, speed1 = 8, speed2 = 16;
    Actor *a;

    if (!desktop_plugin->priv->rich_animation) return;

    a = g_ptr_array_index(actor->child, 0);
    y = a->y + speed1;
    if (y > 480) y = -480;
    set_actor_position(a, a->x, y, a->z, desktop_plugin);
    a->y = y;

    a = g_ptr_array_index(actor->child, 1);
    y = a->y + speed1;
    if (y > 480) y = -480;
    set_actor_position(a, a->x, y, a->z, desktop_plugin);
    a->y = y;

    a = g_ptr_array_index(actor->child, 2);
    y = a->y + speed2;
    if (y > 480) y = -480;
    set_actor_position(a, a->x, y, a->z, desktop_plugin);
    a->y = y;

    a = g_ptr_array_index(actor->child, 3);
    y = a->y + speed2;
    if (y > 480) y = -480;
    set_actor_position(a, a->x, y, a->z, desktop_plugin);
    a->y = y;
}

void
change_static_actor(Actor * actor, AWallpaperPlugin *desktop_plugin)
{
    gchar *newfile;
    newfile = g_strdup_printf("%s%d.png", actor->name, desktop_plugin->priv->scene->daytime);
    if (actor->filename)
            g_free(actor->filename);
    actor->filename = newfile;
    change_hildon_actor(actor, desktop_plugin);
}

void
change_static_actor_with_corner(Actor * actor, AWallpaperPlugin *desktop_plugin)
{
    gchar buffer[2048];

    if (desktop_plugin->priv->right_corner)
        gtk_widget_destroy(desktop_plugin->priv->right_corner);
    snprintf(buffer, sizeof(buffer) - 1, "%s/%s/town%i_right_corner.png", \
                                  THEME_PATH, desktop_plugin->priv->theme, desktop_plugin->priv->scene->daytime);
    desktop_plugin->priv->right_corner = gtk_image_new_from_file (buffer);
    if (desktop_plugin->priv->right_corner){
        gtk_fixed_put(GTK_FIXED(desktop_plugin->priv->main_widget), desktop_plugin->priv->right_corner, 0, 0);
        gtk_widget_show (desktop_plugin->priv->right_corner);
    }
    change_static_actor(actor, desktop_plugin);

}
