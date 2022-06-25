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
#include "livewp-scene.h"

/* This code form X11-utils */
Window Window_With_Name( Display *dpy, Window top, char *name)
{
	Window *children, dummy;
	unsigned int nchildren;
	int i;
	Window w=0;
	char *window_name;

    XClassHint *class_hint;
    class_hint = XAllocClassHint();
    XGetClassHint(dpy, top, class_hint);
    if (class_hint->res_name && name && !strcmp(class_hint->res_name, name)){
      XFree(class_hint->res_class);
      XFree(class_hint->res_name);
      return(top);
    }
    XFree(class_hint->res_class);
    XFree(class_hint->res_name);

	if (XFetchName(dpy, top, &window_name) && !strcmp(window_name, name))
	  return(top);

	if (!XQueryTree(dpy, top, &dummy, &dummy, &children, &nchildren))
	  return(0);

	for (i=0; i<nchildren; i++) {
		w = Window_With_Name(dpy, children[i], name);
		if (w)
		  break;
	}
	if (children) XFree ((char *)children);
	return(w);
}

/*******************************************************************************/

void
destroy_scene(AWallpaperPlugin *desktop_plugin)
{
    int status = 0;

    if (desktop_plugin->priv->scene){
	    GSList * tmp = desktop_plugin->priv->scene->actors;
	    while (tmp != NULL){
		    destroy_actor(tmp->data);
		    tmp = g_slist_next(tmp);
	    }
	    if (tmp)
		    g_slist_free(tmp);
	    desktop_plugin->priv->scene->actors = NULL;
	    if (desktop_plugin->priv->scene){
		    g_free(desktop_plugin->priv->scene);
		    desktop_plugin->priv->scene = NULL;
	    }
    }
    if (desktop_plugin->priv->pipeline){
        gst_element_set_state (desktop_plugin->priv->pipeline, GST_STATE_NULL);
        gst_object_unref (GST_OBJECT (desktop_plugin->priv->pipeline));
        desktop_plugin->priv->pipeline = NULL;
    }
    if (desktop_plugin->priv->podpid > 1){
        kill (desktop_plugin->priv->podpid, SIGTERM);
        /* Hack for Flash */
        if (!strcmp(desktop_plugin->priv->theme,"Flash")){
            sleep(1);
            kill (desktop_plugin->priv->podpid, SIGKILL);
        }
        while (TRUE){
            if (wait(&status) == desktop_plugin->priv->podpid)
                break;
        }
        desktop_plugin->priv->podpid = -1;
        desktop_plugin->priv->running = FALSE;
    }
}
/*******************************************************************************/
void
reload_scene(AWallpaperPlugin *desktop_plugin)
{
    fprintf(stderr,"Reload scene %s\n", desktop_plugin->priv->theme);
    destroy_scene(desktop_plugin);
    fill_priv(desktop_plugin->priv);
    gtk_widget_destroy(desktop_plugin->priv->window);
    if (desktop_plugin->priv->one_in_all_view && desktop_plugin->priv->view >1)
        exit(-1);
    create_xwindow(desktop_plugin->priv);
    init_scene_theme(desktop_plugin);
}
/*******************************************************************************/
void
init_scene_Accel(AWallpaperPlugin *desktop_plugin)
{
    Actor *actor;
    Scene *scene;
    GPtrArray *child;
    gint now = time(NULL);
    gchar *str;
    gchar *bgfile = NULL;
    gint sizes1[4] = {57, 76, 43, 50},
         n, j;

    /* fprintf(stderr, "init scene accel\n"); */
    scene = g_new0(Scene, 1);
    scene->actors = NULL;
    desktop_plugin->priv->scene = scene;

    bgfile = g_strdup_printf("/home/user/.backgrounds/background-%i.png", desktop_plugin->priv->view);
    actor = init_object(desktop_plugin, "original", bgfile,
                      0, 0, 0, 800, 480,
                      TRUE, TRUE, 100, 255,
                      NULL, NULL, NULL);
    scene->actors = g_slist_append(scene->actors, actor);

    child = g_ptr_array_sized_new(16);

    for (j= 0; j<4; j++){
        for (n=0; n<4; n++){
            str = g_strdup_printf("tape%i.png", n+1);
            actor = init_object(desktop_plugin, "tape", str,
                                fast_rnd(800), fast_rnd(480), 2+fast_rnd(6), 800, sizes1[n],
                                TRUE, TRUE, 100, 255,
                                NULL, NULL, NULL);
            scene->actors = g_slist_append(scene->actors, actor);
            g_ptr_array_add(child, actor);
            g_free(str);
        }
    }
    actor = init_object(desktop_plugin, "tape", "",
                      0, 800, 5, 800, 170,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_tape, NULL, child);
    actor->time_start_animation = now;
    actor->duration_animation = G_MAXINT;
    scene->actors = g_slist_append(scene->actors, actor);

    run_long_timeout(desktop_plugin);
}
/*******************************************************************************/
void
parsestring(char *line, char **argv)
{
   while (*line != '\0') {
          while (*line == ' ' || *line == '\n')
                *line++ = '\0';     /* replace white spaces with 0    */
                 *argv++ = line;          /* save the argument position     */
                 while (*line != '\0' && *line != ' ' &&
                        *line != '\n')
                     line++;             /* skip the argument until ...    */
   }
	          *argv = '\0';                 /* mark the end of argument list  */
}

#if 0
GstBusSyncReply
sync_handler(GstBus *bus, GstMessage *message, AWallpaperPlugin *desktop_plugin){

    if (!desktop_plugin->priv->visible)
        gst_element_set_state (desktop_plugin->priv->pipeline, GST_STATE_PAUSED);
    if (GST_MESSAGE_TYPE(message) != GST_MESSAGE_ELEMENT){
		return (GST_BUS_PASS);
	}
    if (!gst_structure_has_name(message->structure, "prepare-xwindow-id")){
        return (GST_BUS_PASS);
    }
    return (GST_BUS_DROP);

}
#endif
/*******************************************************************************/
static gboolean
bus_call (GstBus *bus, GstMessage *msg, AWallpaperPlugin *desktop_plugin)
{
    switch (GST_MESSAGE_TYPE (msg))
    {
       case GST_MESSAGE_EOS:
           if (desktop_plugin->priv->rich_animation){

                GstClockTime nach   = (GstClockTime)(0 * GST_MSECOND);
                if (!gst_element_seek(desktop_plugin->priv->pipeline, 1.0, GST_FORMAT_TIME,
                   (GstSeekFlags) (GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), GST_SEEK_TYPE_SET,
                   nach, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE))
                    fprintf(stderr,"ERROR in seek\n");

                gst_element_set_state (desktop_plugin->priv->pipeline, GST_STATE_PLAYING);

           }else{
                if (desktop_plugin->priv->pipeline){
                    gst_element_set_state (desktop_plugin->priv->pipeline, GST_STATE_NULL);
                    gst_object_unref (GST_OBJECT (desktop_plugin->priv->pipeline));
                }
           }
           break;
       case GST_MESSAGE_ERROR: break;
       default: break;
     }
       return TRUE;
}

/*******************************************************************************/
void
init_scene_External(AWallpaperPlugin *desktop_plugin){

    char* child_argv[2048];
    char *run_string = NULL;
    gchar *exec_path = NULL,
        *window_id = NULL,
        *window_name = NULL,
        *view = NULL,
        *gtk_socket_id = NULL,
        *strwin = NULL,
        *strsocket = NULL,
        *addedstring = NULL,
        *theme_string_parametr1 = NULL,
        *straddedstring = NULL,
        *strview = NULL;
    gint i;
    Window  id_xwindow;
    if (!desktop_plugin->priv->visible)
        return;

    exec_path = g_strdup(g_hash_table_lookup(desktop_plugin->priv->hash_theme, "exec_path"));
    window_id = g_strdup(g_hash_table_lookup(desktop_plugin->priv->hash_theme, "window_id"));
    view = g_strdup(g_hash_table_lookup(desktop_plugin->priv->hash_theme, "view"));
    window_name = g_strdup(g_hash_table_lookup(desktop_plugin->priv->hash_theme, "window_name"));
    gtk_socket_id =  g_strdup(g_hash_table_lookup(desktop_plugin->priv->hash_theme, "gtk_socket_id"));
    addedstring =  g_strdup(g_hash_table_lookup(desktop_plugin->priv->hash_theme, "added_string"));
    if (!exec_path)
        return;
    if (window_id){
        strwin = g_strdup_printf(" %s %i", window_id, (gint)GDK_WINDOW_XID(desktop_plugin->priv->window->window));
    }else
        strwin = "";
    if (gtk_socket_id){
        strsocket = g_strdup_printf(" %s %i", gtk_socket_id, (int)gtk_socket_get_id (GTK_SOCKET (desktop_plugin->priv->video_socket)));
    }else
        strsocket = "";

    if (view){
        strview = g_strdup_printf(" %s %i", view, desktop_plugin->priv->view);
    }else
        strview = "";

    if (addedstring){
        straddedstring = g_strdup_printf(" %s", addedstring);
    }else
        straddedstring = "";

    if (desktop_plugin->priv->theme_string_parametr1){
        theme_string_parametr1 = g_strdup_printf(" %s", desktop_plugin->priv->theme_string_parametr1);
    }else
        theme_string_parametr1 = "";

    run_string = g_strdup_printf("%s%s%s%s%s%s", exec_path, strwin, strview, strsocket, straddedstring, theme_string_parametr1);
    fprintf(stderr, "runs string = %s\n", run_string);
    parsestring(run_string, child_argv);

    desktop_plugin->priv->running = TRUE;
    desktop_plugin->priv->podpid = fork();
    if (desktop_plugin->priv->podpid == 0){
        execvp(child_argv[0], child_argv);
    	fprintf(stderr,"Problem with new podprocess");
    }
    g_free(run_string);

    fprintf(stderr, "window name = %s\n", window_name);
    if (window_name){
        /* Do 10 trying to search of window */
        for (i=0; i<10; i++){
            sleep(1);
            id_xwindow = Window_With_Name(GDK_WINDOW_XDISPLAY (desktop_plugin->priv->window->window),
                RootWindow( GDK_WINDOW_XDISPLAY (desktop_plugin->priv->window->window), XDefaultScreen( GDK_WINDOW_XDISPLAY (desktop_plugin->priv->window->window))),
                window_name);
            fprintf(stderr,"name %s %i %i\n", window_name, id_xwindow, i);
            if (id_xwindow>0){
                if (desktop_plugin->priv->one_in_all_view)
                    set_live_bg (GDK_WINDOW_XDISPLAY (desktop_plugin->priv->window->window),
                                                      id_xwindow, -1);
                else
                    set_live_bg (GDK_WINDOW_XDISPLAY (desktop_plugin->priv->window->window),
                                                      id_xwindow, desktop_plugin->priv->view);
                /* gtk_widget_destroy(desktop_plugin->priv->window); */
                break;
            }
        }
    }
}
/*******************************************************************************/
gboolean
cb_timeout0(AWallpaperPlugin *desktop_plugin) {

    if (!desktop_plugin || !desktop_plugin->priv->pipeline)
        return FALSE;
    if (desktop_plugin->priv->theme_int_parametr1 == 0){
        if (!gst_element_seek((GstElement *)GST_PIPELINE (desktop_plugin->priv->pipeline), 1.0, GST_FORMAT_TIME,
                                          GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET,
                                          desktop_plugin->priv->theme_int_parametr1 * GST_SECOND,
                                          GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE))
            fprintf(stderr,"Error in seek:\n");
        return FALSE;
    }

    if (gst_element_get_state (desktop_plugin->priv->pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE)
        return TRUE;
    else{
         if (!gst_element_seek((GstElement *)GST_PIPELINE (desktop_plugin->priv->pipeline), 1.0, GST_FORMAT_TIME,
                                          GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET,
                                          desktop_plugin->priv->theme_int_parametr1 * GST_SECOND,
                                          GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE))
            fprintf(stderr,"Error in seek:\n");
         return FALSE;
    }
}
/*******************************************************************************/
void
init_scene_Video(AWallpaperPlugin *desktop_plugin)
{
    GstElement *bin;
    GstElement *videosink;
    gchar *file_plus_uri;


    /* fprintf(stderr, "init scene Video \n"); */
    desktop_plugin->priv->pipeline = gst_pipeline_new("gst-player");
    bin = gst_element_factory_make ("playbin2", "bin");
    videosink = gst_element_factory_make ("ximagesink", "videosink");
    g_object_set (G_OBJECT (bin), "video-sink", videosink, NULL);
    gst_bin_add (GST_BIN (desktop_plugin->priv->pipeline), bin);

    {
    	GstBus *bus;
     	bus = gst_pipeline_get_bus (GST_PIPELINE (desktop_plugin->priv->pipeline));
       	gst_bus_add_watch(bus, (GstBusFunc)bus_call, desktop_plugin);
    	gst_object_unref (bus);
    }
    file_plus_uri = g_strdup_printf("file://%s",desktop_plugin->priv->theme_string_parametr1);
    g_object_set (G_OBJECT (bin), "uri", file_plus_uri, NULL );
#if 0
    /* Doesn't work in real device. Hardware volume buttons can to change volume for mutted track */
    /* Set Mute */
    if (!desktop_plugin->priv->theme_bool_parametr1)
        g_object_set (G_OBJECT (bin), "mute", TRUE, NULL );
#endif
    g_object_set (G_OBJECT (videosink), "force-aspect-ratio", TRUE, NULL  );

    if (GST_IS_X_OVERLAY (videosink))
	    //gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), GDK_DRAWABLE_XID(desktop_plugin->priv->window->window));
	    gst_x_overlay_set_window_handle (GST_X_OVERLAY (videosink), GDK_DRAWABLE_XID(desktop_plugin->priv->window->window));

    if (desktop_plugin->priv->visible){
        g_timeout_add(50, (GSourceFunc)cb_timeout0, desktop_plugin);
        gst_element_set_state (desktop_plugin->priv->pipeline, GST_STATE_PLAYING);
    }

    //gst_element_get_state(deskddtop_plugin->priv->pipeline, NULL, NULL, 100 * GST_MSECOND);

}
/*******************************************************************************/
void
init_scene_Slideshow(AWallpaperPlugin *desktop_plugin)
{
	gchar *ext, *file, *str, *bgfile;
	Dirent  *dp;
	DIR     *dir_fd;
	int i;
	Actor *actor;
	Scene *scene;
	GPtrArray *child;
	gint now = time(NULL);
	GSList *store = NULL;
	gint num = 0;

	fprintf(stderr, "init scene Slideshow %d %s\n", desktop_plugin->priv->theme_int_parametr1,
                desktop_plugin->priv->theme_string_parametr1);

	scene = g_new0(Scene, 1);
	scene->actors = NULL;
	desktop_plugin->priv->scene = scene;

	dir_fd = opendir(desktop_plugin->priv->theme_string_parametr1);
	if(dir_fd){
		while((dp = readdir(dir_fd))){
			if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
				continue;

			ext = strrchr(dp->d_name, '.') + 1; /* get file extension */
			if (!ext) continue;
			/* lowercase string */
			for(i=0; ext[i]; i++)
				ext[i] = tolower(ext[i]);
			if (strcmp(ext, "jpg") && strcmp(ext, "png"))
				continue;
			if(dp->d_type == DT_REG || dp->d_type == DT_LNK){
				str = g_strdup_printf("%s/%s", desktop_plugin->priv->theme_string_parametr1, dp->d_name);
				store = g_slist_append(store, str);
				num++;
			}
		}
		closedir(dir_fd);
	}

    bgfile = g_strdup_printf("/home/user/.backgrounds/background-%i.png", desktop_plugin->priv->view);
    actor = init_object(desktop_plugin, "original", bgfile,
                      0, 0, 0, 800, 480,
                      TRUE, TRUE, 100, 255,
                      NULL, NULL, NULL);
    scene->actors = g_slist_append(scene->actors, actor);

	child = g_ptr_array_sized_new(num);
	while (store){
		actor = init_object(desktop_plugin, "original", store->data,
				0, 0, 5, 800, 480,
				FALSE, FALSE, 100, 0,
				NULL, NULL, NULL);
		scene->actors = g_slist_append(scene->actors, actor);
		g_ptr_array_add(child, actor);
		store = g_slist_next(store);
	}
	actor = init_object(desktop_plugin, "images", "",
			0, 0, -1, 800, 480,
			FALSE, FALSE, num, -1,
			(gpointer)&change_slideshow, NULL, child);
	actor->time_start_animation = now;
	actor->duration_animation = 5;
	scene->actors = g_slist_append(scene->actors, actor);
	desktop_plugin->priv->rich_animation = TRUE;
	run_long_timeout(desktop_plugin);

}
/*******************************************************************************/
void
init_scene_Matrix(AWallpaperPlugin *desktop_plugin)
{
	Actor *actor;
	Scene *scene;
	GPtrArray *child;
	gint now = time(NULL);
	gint y1, y2;

	if (desktop_plugin->priv->rich_animation){
		y1 = -480;
		y2 = -480-480;
	}else {
		y1 = 0;
		y2 = -480;
	}

	/* fprintf(stderr, "init scene matrix \n"); */
	scene = g_new0(Scene, 1);
	//scene.daytime = get_daytime();
	scene->actors = NULL;
	desktop_plugin->priv->scene = scene;

	actor = init_object(desktop_plugin, "background", "bg.png",
			0, 0, 5, 800, 480,
			TRUE, TRUE, 100, 255,
			NULL, NULL, NULL);
	scene->actors = g_slist_append(scene->actors, actor);

	actor = init_object(desktop_plugin, "symbols", "symbols.png",
			0, 0, 10, 800, 480,
			TRUE, TRUE, 100, 255,
			NULL, NULL, NULL);
	scene->actors = g_slist_append(scene->actors, actor);

	child = g_ptr_array_sized_new(4);
	actor = init_object(desktop_plugin, "layer1", "layer1_2.png",
			0, y1, 6, 800, 960,
			TRUE, TRUE, 100, 255,
			NULL, NULL, NULL);
	//actor->time_start_animation = now;
	//actor->duration_animation = G_MAXINT;
	scene->actors = g_slist_append(scene->actors, actor);
	g_ptr_array_add(child, actor);

	actor = init_object(desktop_plugin, "layer1", "layer1_1.png",
			0, y2, 7, 800, 960,
			TRUE, TRUE, 100, 255,
			NULL, NULL, NULL);
	//actor->time_start_animation = now;
	//actor->duration_animation = G_MAXINT;
	scene->actors = g_slist_append(scene->actors, actor);
	g_ptr_array_add(child, actor);

	actor = init_object(desktop_plugin, "layer2", "layer2_2.png",
			0, y1, 8, 800, 960,
			TRUE, TRUE, 100, 255,
			NULL, NULL, NULL);
	//actor->time_start_animation = now;
	//actor->duration_animation = G_MAXINT;
	scene->actors = g_slist_append(scene->actors, actor);
	g_ptr_array_add(child, actor);

	actor = init_object(desktop_plugin, "layer2", "layer2_1.png",
			0, y2, 9, 800, 960,
			TRUE, TRUE, 100, 255,
			NULL, NULL, NULL);
	//actor->time_start_animation = now;
	//actor->duration_animation = G_MAXINT;
	scene->actors = g_slist_append(scene->actors, actor);
	g_ptr_array_add(child, actor);

	actor = init_object(desktop_plugin, "layers", "",
			0, y2, 9, 800, 960,
			FALSE, FALSE, 100, 255,
			(gpointer)&change_layer, NULL, child);
	actor->time_start_animation = now;
	actor->duration_animation = G_MAXINT;
	scene->actors = g_slist_append(scene->actors, actor);

	run_long_timeout(desktop_plugin);

}
/*******************************************************************************/
/* Init Modern Scene */
void
init_scene_Modern(AWallpaperPlugin *desktop_plugin)
{
  Actor *actor;
  Scene *scene;
  gint now = time(NULL);
  gint i;
  gint winds[13][2];
  GPtrArray *child;

  /* fprintf(stderr, "init scene \n"); */
  scene = g_new0(Scene, 1);
  scene->daytime = get_daytime();
  scene->actors = NULL;
  scene->wind_orientation = -1;
  scene->wind_angle = 0.3;
  /* init value for random */
  scene->seed = time(NULL);
  scene->notification = TRUE;
  desktop_plugin->priv->scene = scene;
  actor = init_object(desktop_plugin, "sky", "sky0.png",
                      0, 0, 5, 800, 480,
                      TRUE , TRUE, 100, 255,
                      (gpointer)&change_static_actor, NULL, NULL);
  scene->actors = g_slist_append(scene->actors, actor);
  change_static_actor(actor, desktop_plugin);

  actor = init_object(desktop_plugin, "sun", "sun.png",
                      0, 0, 6, 88, 88,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_sun, NULL, NULL);
  actor->time_start_animation = now;
  actor->duration_animation = G_MAXINT;
  change_sun(actor, desktop_plugin);
  scene->actors = g_slist_append(scene->actors, actor);

  //actor = init_object(desktop_plugin, "dot", "dot1.png", 0, 0, 11, 50, 50,
    //                  TRUE, 100, 255, NULL, NULL);
  //scene.actors = g_slist_append(scene.actors, actor);

  actor = init_object(desktop_plugin, "cloud1", "cloud1.png",
                      0, fast_rnd(300)-97, 7, 150, 97,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_cloud, NULL, NULL);
  actor->time_start_animation = now + fast_rnd(20);
  actor->duration_animation = 3*60;
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "cloud2", "cloud2.png",
                      0, fast_rnd(300)-75, 7, 188, 75,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_cloud, NULL, NULL);
  actor->time_start_animation = now + fast_rnd(40)+10;
  actor->duration_animation = 3*60;
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "cloud4", "cloud4.png",
                      0, fast_rnd(300)-75, 7, 150, 75,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_cloud, NULL, NULL);
  actor->time_start_animation = now + fast_rnd(60) + 20;
  actor->duration_animation = 5*60;
  scene->actors = g_slist_append(scene->actors, actor);


  actor = init_object(desktop_plugin, "town", "town0.png",
                      0, 0, 8, 800, 480,
                      TRUE, TRUE, 100, 255,
                      (gpointer)&change_static_actor, NULL, NULL);
  change_static_actor(actor, desktop_plugin);
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "stend", "stend0.png",
                      452, 166, 9, 300, 305,
                      TRUE, TRUE, 100, 255,
                      (gpointer)&change_static_actor, NULL, NULL);
  change_static_actor(actor, desktop_plugin);
  scene->actors = g_slist_append(scene->actors, actor);


  child = g_ptr_array_sized_new(4);
  actor = init_object(desktop_plugin, "call", "call.png",
                      480, 190, 9, 50, 58,
                      FALSE, TRUE, 100, 255,
                      NULL, NULL, NULL);
  scene->actors = g_slist_append(scene->actors, actor);
  g_ptr_array_add(child, actor);

  actor = init_object(desktop_plugin, "chat", "chat.png",
                      540, 190, 9, 50, 58,
                      FALSE, TRUE, 100, 255,
                      NULL, NULL, NULL);
  scene->actors = g_slist_append(scene->actors, actor);
  g_ptr_array_add(child, actor);

  actor = init_object(desktop_plugin, "mail", "mail.png",
                      600, 190, 9, 50, 58,
                      FALSE, TRUE, 100, 255,
                      NULL, NULL, NULL);
  scene->actors = g_slist_append(scene->actors, actor);
  g_ptr_array_add(child, actor);

  actor = init_object(desktop_plugin, "sms", "sms.png",
                      660, 190, 9, 50, 58,
                      FALSE, TRUE, 100, 255,
                      NULL, NULL, NULL);
  scene->actors = g_slist_append(scene->actors, actor);
  g_ptr_array_add(child, actor);

  actor = init_object(desktop_plugin, "billboard_text", "",
                      470, 174, 9, 300, 108,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_billboard, NULL, child);
  create_hildon_actor_text(actor, desktop_plugin);
  //actor->time_start_animation = time(NULL) + 20;
  change_billboard(actor, desktop_plugin);
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "tram", "tram.png",
                      -300, 225, 10, 350, 210,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_tram, NULL, NULL);
  actor->time_start_animation = time(NULL) + fast_rnd(10);
  actor->duration_animation = 60;
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "border", "border0.png",
                      0, 480-79, 11, 800, 79,
                      TRUE, TRUE, 100, 255,
                      (gpointer)&change_static_actor_with_corner, NULL, NULL);
  change_static_actor_with_corner(actor, desktop_plugin);
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "moon", "moon1.png",
                      400, 20, 6, 60, 60,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_moon, NULL, NULL);
  change_moon(actor, desktop_plugin);
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "wind", "",
                      0, 0, 5, 0, 0,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_wind, NULL, NULL);
  change_wind(actor, desktop_plugin);
  scene->actors = g_slist_append(scene->actors, actor);

    /* windows in 4-th house  */

    winds[0][0] = 482;
    winds[0][1] = 180;

    winds[1][0] = 495;
    winds[1][1] = 179;

    winds[2][0] = 482;
    winds[2][1] = 191;

    winds[3][0] = 495;
    winds[3][1] = 190;

    winds[4][0] = 482;
    winds[4][1] = 201;

    winds[5][0] = 495;
    winds[5][1] = 210;

    winds[6][0] = 482;
    winds[6][1] = 222;

    winds[7][0] = 495;
    winds[7][1] = 221;

    winds[8][0] = 459;
    winds[8][1] = 203;

    winds[9][0] = 495;
    winds[9][1] = 241;

    winds[10][0] = 495;
    winds[10][1] = 252;

    winds[11][0] = 482;
    winds[11][1] = 273;

    winds[12][0] = 495;
    winds[12][1] = 303;
    for (i=0; i<13; i++){
        actor = init_object(desktop_plugin, "window1", "window1.png",
                            winds[i][0], winds[i][1], 8, 8, 10,
                            FALSE, FALSE, 100, 255,
                            (gpointer)&change_window1, NULL, NULL);
        //change_window1(actor, desktop_plugin);
        actor->time_start_animation = now + fast_rnd(30);
        scene->actors = g_slist_append(scene->actors, actor);

    }

    /* windows in 1-th house  */

    winds[0][0] = 86;
    winds[0][1] = 321;

    winds[1][0] = 86;
    winds[1][1] = 363;

    winds[2][0] = 86;
    winds[2][1] = 385;

    winds[3][0] = 86;
    winds[3][1] = 286;

    winds[4][0] = 94;
    winds[4][1] = 232;

    winds[5][0] = 94;
    winds[5][1] = 243;

    winds[6][0] = 94;
    winds[6][1] = 265;

    winds[7][0] = 94;
    winds[7][1] = 331;
    for (i=0; i<8; i++){
        actor = init_object(desktop_plugin, "window2", "window2.png",
                            winds[i][0], winds[i][1], 8, 8, 10,
                            FALSE, FALSE, 100, 255,
                            (gpointer)&change_window1, NULL, NULL);
        //change_window1(actor, desktop_plugin);
        actor->time_start_animation = now + fast_rnd(30);
        scene->actors = g_slist_append(scene->actors, actor);

    }

    /* windows in 3-th house  */

    winds[0][0] = 251;
    winds[0][1] = 162;

    winds[1][0] = 251;
    winds[1][1] = 196;

    winds[2][0] = 251;
    winds[2][1] = 278;

    winds[3][0] = 251;
    winds[3][1] = 289;

    winds[4][0] = 313;
    winds[4][1] = 173;

    winds[5][0] = 322;
    winds[5][1] = 160;

    winds[6][0] = 303;
    winds[6][1] = 217;

    winds[7][0] = 322;
    winds[7][1] = 224;

    winds[8][0] = 323;
    winds[8][1] = 217;

    winds[9][0] = 322;
    winds[9][1] = 288;

    for (i=0; i<10; i++){
        actor = init_object(desktop_plugin, "window3", "window3.png",
                            winds[i][0], winds[i][1], 8, 8, 10,
                            FALSE, FALSE, 100, 255,
                            (gpointer)&change_window1, NULL, NULL);
        //change_window1(actor, desktop_plugin);
        actor->time_start_animation = now + fast_rnd(30);
        scene->actors = g_slist_append(scene->actors, actor);

    }

    /* windows in 5-th house  */

    winds[0][0] = 610;
    winds[0][1] = 224;

    winds[1][0] = 602;
    winds[1][1] = 245;

    winds[2][0] = 602;
    winds[2][1] = 264;

    winds[3][0] = 610;
    winds[3][1] = 301;

    winds[4][0] = 610;
    winds[4][1] = 320;

    winds[5][0] = 593;
    winds[5][1] = 352;

    winds[6][0] = 610;
    winds[6][1] = 368;

    for (i=0; i<7; i++){
        actor = init_object(desktop_plugin, "window4", "window4.png",
                            winds[i][0], winds[i][1], 8, 8, 10,
                            FALSE, FALSE, 100, 255,
                            (gpointer)&change_window1, NULL, NULL);
        //change_window1(actor, desktop_plugin);
        actor->time_start_animation = now + fast_rnd(30);
        scene->actors = g_slist_append(scene->actors, actor);

    }

    /* windows in 6-th house  */

    winds[0][0] = 717;
    winds[0][1] = 283;

    winds[1][0] = 698;
    winds[1][1] = 293;

    winds[2][0] = 717;
    winds[2][1] = 315;

    winds[3][0] = 717;
    winds[3][1] = 323;

    winds[4][0] = 698;
    winds[4][1] = 362;

    winds[5][0] = 698;
    winds[5][1] = 400;

    for (i=0; i<6; i++){
        actor = init_object(desktop_plugin, "window5", "window5.png",
                            winds[i][0], winds[i][1], 8, 8, 10,
                            FALSE, FALSE, 100, 255,
                            (gpointer)&change_window1, NULL, NULL);
        //change_window1(actor, desktop_plugin);
        actor->time_start_animation = now + fast_rnd(30);
        scene->actors = g_slist_append(scene->actors, actor);

    }
    run_long_timeout(desktop_plugin);

#if 0
  anim = g_new0(Animation, 1);
  anim->count = 1;
  anim->actor = actor;
  anim->func_change = &change_tram;
  anim->func_time = NULL;
  anim->timestart = time(NULL);
  anim->timeall = 10;

  scene.dynamic_actors = g_slist_append(scene.dynamic_actors, anim);
#endif
}
/*******************************************************************************/
/* Init Berlin Scene */
void
init_scene_Berlin(AWallpaperPlugin *desktop_plugin)
{
  Actor *actor, *actor1, *actor2;
  Scene *scene;
  gint now = time(NULL);
  gint i;
  gint winds[13][2];
  GPtrArray *child = NULL;

  scene = g_new0(Scene, 1);
  scene->daytime = get_daytime();
  scene->actors = NULL;
  scene->wind_orientation = -1;
  scene->wind_angle = 0.3;
  /* init value for random */
  scene->seed = time(NULL);
  desktop_plugin->priv->scene = scene;

  actor = init_object(desktop_plugin, "sky", "sky.png", 0, 0, 5, 800, 480,
                      TRUE, TRUE, 100, 255,
                      (gpointer)&change_static_actor, NULL, NULL);
  change_static_actor(actor, desktop_plugin);
  scene->actors = g_slist_append(scene->actors, actor);


  actor = init_object(desktop_plugin, "sun", "sun.png", 0, 0, 6, 88, 88,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_sun, NULL, NULL);
  actor->time_start_animation = time(NULL);
  actor->duration_animation = G_MAXINT;
  change_sun(actor, desktop_plugin);
  scene->actors = g_slist_append(scene->actors, actor);

#if 0
  actor = init_object(desktop_plugin, "dot", "dot1.png", 0, 0, 11, 50, 50,
                      TRUE, 100, 255, NULL, NULL);
  scene.actors = g_slist_append(scene.actors, actor);
#endif

  actor = init_object(desktop_plugin, "moon", "moon1.png", 400, 15, 6, 60, 60,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_moon, NULL, NULL);
  change_moon(actor, desktop_plugin);
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "cloud1", "cloud1.png", 0, fast_rnd(300)-97, 7, 150, 97,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_cloud, NULL, NULL);
  actor->time_start_animation = now + fast_rnd(30) + 10;
  actor->duration_animation = 3*60;
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "cloud2", "cloud2.png", 0, fast_rnd(300)-75, 7, 188, 75,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_cloud, NULL, NULL);
  actor->time_start_animation = now + fast_rnd(10);
  actor->duration_animation = 3*60;
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "cloud4", "cloud4.png", 0, fast_rnd(300)-75, 7, 150, 75,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_cloud, NULL, NULL);
  actor->time_start_animation = now + fast_rnd(60) + 20;
  actor->duration_animation = 5*60;
  scene->actors = g_slist_append(scene->actors, actor);


  actor = init_object(desktop_plugin, "plane2", "plane3.png", 0, 45, 8, 160, 50,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_plane2, NULL, NULL);
  actor->time_start_animation = now + fast_rnd(40) + 20;
  actor->duration_animation = 60;
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "plane1", "tu154.png", 620, 233, 9, 300, 116,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_plane1, NULL, NULL);
  actor->time_start_animation = now + fast_rnd(20);
  actor->duration_animation = 30;
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "town", "town.png", 0, 0, 10, 800, 480,
                      TRUE, TRUE, 100, 255,
                      (gpointer)&change_static_actor_with_corner, NULL, NULL);
  change_static_actor_with_corner(actor, desktop_plugin);
  scene->actors = g_slist_append(scene->actors, actor);

  actor = init_object(desktop_plugin, "wind", "", 0, 0, 5, 0, 0,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_wind, NULL, NULL);
  change_wind(actor, desktop_plugin);
  scene->actors = g_slist_append(scene->actors, actor);

  actor1 = init_object(desktop_plugin, "signal_red", "red.png",
                      486, 425, 10, 18, 38,
                      FALSE, TRUE, 100, 255, NULL, NULL, NULL);
  //actor->time_start_animation = now + fast_rnd(30) + 10;
  scene->actors = g_slist_append(scene->actors, actor1);

  actor2 = init_object(desktop_plugin, "signal_green", "green.png",
                      486, 425, 10, 18, 38,
                      TRUE, TRUE, 100, 255, NULL, NULL, NULL);
  //actor->time_start_animation = now + fast_rnd(30) + 10;
  scene->actors = g_slist_append(scene->actors, actor2);
  child = g_ptr_array_sized_new(2);
  g_ptr_array_add(child, actor1);
  g_ptr_array_add(child, actor2);
  actor = init_object(desktop_plugin, "signal", "",
                      486, 425, 10, 18, 38,
                      FALSE, FALSE, 100, 255,
                      (gpointer)&change_signal, NULL, child);
  actor->time_start_animation = now + fast_rnd(30) + 10;
  scene->actors = g_slist_append(scene->actors, actor);

    winds[0][0] = 389;
    winds[0][1] = 305;

    winds[1][0] = 373;
    winds[1][1] = 306;

    winds[2][0] = 355;
    winds[2][1] = 306;

    winds[3][0] = 356;
    winds[3][1] = 288;

    winds[4][0] = 337;
    winds[4][1] = 269;

    winds[5][0] = 372;
    winds[5][1] = 268;

    winds[6][0] = 372;
    winds[6][1] = 249;

    winds[7][0] = 388;
    winds[7][1] = 249;

    winds[8][0] = 387;
    winds[8][1] = 230;

    winds[9][0] = 372;
    winds[9][1] = 211;

    winds[10][0] = 355;
    winds[10][1] = 159;

    winds[11][0] = 335;
    winds[11][1] = 158;

    winds[12][0] = 386;
    winds[12][1] = 119;

    for (i=0; i<13; i++){
        actor = init_object(desktop_plugin, "window", "window.png",
                            winds[i][0], winds[i][1], 10, 8, 9,
                            FALSE, TRUE, 100, 255,
                            (gpointer)&change_window1, NULL, NULL);
        //change_window1(actor, desktop_plugin);
        actor->time_start_animation = now + fast_rnd(30);
        scene->actors = g_slist_append(scene->actors, actor);

    }

    run_long_timeout(desktop_plugin);

}
/*******************************************************************************/
void
init_scene_theme(AWallpaperPlugin *desktop_plugin)
{
#if 0
    void (*func)(gpointer);
    func = g_hash_table_lookup(desktop_plugin->priv->hash_scene_func, desktop_plugin->priv->theme);
    if (func){
        (*func)(desktop_plugin);
    }
#endif
    fprintf(stderr, "Init_scene_theme\n");
    void (*func)(gpointer);
    func = desktop_plugin->priv->scene_func;
    if (func){
        fprintf(stderr, "Success init_scene_theme %s\n", desktop_plugin->priv->theme);
        (*func)(desktop_plugin);
    }
}
