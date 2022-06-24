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
#include "livewp-settings.h"
/*******************************************************************************/
void lw_about(void){

    gchar *about_string;
    GtkWidget *window = NULL,
    *vbox = NULL,
    *label_about = NULL;
    window = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(window), _("About"));
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);
    vbox = gtk_vbox_new (FALSE, 5);
    about_string = g_strdup_printf(_("Live Wallpaper Version %s \n Copyright(c) 2010\n \
Tanya Makova\n Vlad Vasiliev\n \
Copyright(c) 2010 for design themes Berlin, Modern and Accel Vasya Bobrikov\n \
Copyright(c) 2010 for design themes Matrix, Fifteen \nand for icons Andrew Zhilin\n \
Translators:\n \
Finnish - Marko Vertainen\n \
Spain  - Alejandro López\n \
Italian  - Emanuele Cassioli\n \
Dutch - Roland van Tilburg (aka ROLAN900D) \n \
Russian - Tanya Makova \n \
          Vlad Vasiliev\n"), VERSION);

    label_about = gtk_label_new (about_string);
    gtk_box_pack_start (GTK_BOX (vbox), label_about, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                                   vbox, TRUE, TRUE, 0);
    gtk_widget_show (label_about);
    gtk_widget_show (vbox);
    gtk_widget_show (window);
    gtk_dialog_run(GTK_DIALOG(window));

}
/*******************************************************************************/
GtkWidget *
create_category_selector (Animation_WallpaperPrivate *priv){
    GtkWidget *selector;
    GSList *store = priv->extheme_list;
    GHashTable *result_table = NULL;

    selector = hildon_touch_selector_new_text();


    result_table = g_hash_table_new(g_str_hash, g_str_equal);
    while (store){
        if (!g_hash_table_lookup(result_table, g_hash_table_lookup(store->data, "category"))){
            g_hash_table_insert(result_table, g_hash_table_lookup(store->data, "category"), (gint *)1);
            hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), g_hash_table_lookup(store->data, "category"));
        }
        store = g_slist_next(store);
    }

    /* Add Xscreensaver for install message */
    if (!g_hash_table_lookup(result_table,"Xscreensaver")){
            g_hash_table_insert(result_table, "Xscreensaver", (gint *)1);
            hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), "Xscreensaver");
    }
    /* Add LiveWallpaper to selector */
    if (!g_hash_table_lookup(result_table,"LiveWallpaper")){
        hildon_touch_selector_prepend_text (HILDON_TOUCH_SELECTOR (selector), "LiveWallpaper" );
    }

    return selector;
}

/*******************************************************************************/
void
theme_button_clicked(GtkWidget *button, GdkEventButton *event, Animation_WallpaperPrivate *priv){
    lw_theme_settings(GTK_WIDGET(button), priv);
}
/********************************************************************************/
void
set_button_image(GtkWidget * button, Animation_WallpaperPrivate *priv, gboolean enable){
    GtkWidget * image = NULL;
    GdkPixbuf * pixbuf = NULL;
    gchar *str = NULL;
    gchar *icon_on = NULL;
    gchar *icon_off = NULL;
    GSList *store = priv->extheme_list;

    while (store){
        if (!strcmp(priv->theme, g_hash_table_lookup(store->data, "name"))){
            icon_on = g_strdup(g_hash_table_lookup(store->data, "icon_on"));
            icon_off = g_strdup(g_hash_table_lookup(store->data, "icon_off"));
            break;
        }
        store = (GSList *)g_list_next(store);
    }
    if (enable){
        if (icon_on)
            str = g_strdup_printf("%s", icon_on);
        else
            str = g_strdup_printf( "%s/%s/%s", THEME_PATH,
                            priv->theme, "icon.png");
            if (access(str, F_OK) != 0){
                g_free(str);
                str = g_strdup_printf( "%s/%s/%s", THEME_PATH,
                            "Video", "icon.png");
            }

    }else {
        if (icon_off)
            str = g_strdup_printf("%s", icon_off);
        else
            str = g_strdup_printf( "%s/%s/%s", THEME_PATH,
                            priv->theme, "icond.png");
            if (access(str, F_OK) != 0){
                g_free(str);
                str = g_strdup_printf( "%s/%s/%s", THEME_PATH,
                            "Video", "icond.png");
            }
    }
    pixbuf = gdk_pixbuf_new_from_file_at_size (str,
                                                 124,
                                                 79,
                                                 NULL);
    if (str)
        g_free(str);
    if (icon_on)
        g_free(icon_on);
    if (icon_off)
        g_free(icon_off);
    if (pixbuf){
        image = gtk_image_new_from_pixbuf (pixbuf);
        g_object_unref(G_OBJECT(pixbuf));
    }

    //hildon_button_set_image (HILDON_BUTTON (button), image);
    GList *list = gtk_container_get_children(GTK_CONTAINER (button));
    if (list){
        gtk_container_remove(GTK_CONTAINER (button), list->data);
    }
    gtk_container_add(GTK_CONTAINER (button), image);
    gtk_widget_show(image);
    gtk_widget_show(button);

}
/********************************************************************************/
GtkWidget *
create_image_button (gint view, DBusConnection *conn_sess){
    GtkWidget *event_box;

    Animation_WallpaperPrivate *priv = g_new0(Animation_WallpaperPrivate, 1);
    /* Add external themes to priv */
    priv->extheme_list = get_list_exthemes();

    priv->view = view;
    priv->theme_string_parametr1 = NULL;
    priv->dbus_conn_session = conn_sess;
    read_config(priv);

    event_box = gtk_event_box_new();
    g_object_set_data(G_OBJECT(event_box), "view", GINT_TO_POINTER(view));
    g_object_set_data(G_OBJECT(event_box), "priv", priv);
    set_button_image(event_box, priv, check_applet_state(view));
    g_signal_connect(G_OBJECT (event_box), "button_release_event", G_CALLBACK(theme_button_clicked), priv);
    return event_box;
#if 0
    button = hildon_button_new(HILDON_SIZE_AUTO_WIDTH | HILDON_SIZE_FINGER_HEIGHT,
                                    HILDON_BUTTON_ARRANGEMENT_VERTICAL);
    g_object_set_data(G_OBJECT(button), "view", GINT_TO_POINTER(view));
    g_object_set_data(G_OBJECT(button), "priv", priv);
    set_button_image(button, priv, check_applet_state(view));
    g_signal_connect(button, "clicked", G_CALLBACK(theme_button_clicked), priv);
    hildon_button_set_image_position (HILDON_BUTTON (button), GTK_POS_RIGHT);
    return button;
#endif
}
/********************************************************************************/
void
changed_value_one_in_all_cb (GtkWidget *toggle, Animation_WallpaperPrivate *priv)
{
    priv->one_in_all_view = hildon_check_button_get_active((HildonCheckButton *)toggle);
    create_themes_buttons_hbox(priv);
    fprintf(stderr, "changed_value_one_in_all_cb\n");
}
/********************************************************************************/
void
changed_value_theme_cb (HildonPickerButton *picker, Animation_WallpaperPrivate *priv)
{
    const gchar *choice = hildon_button_get_value(HILDON_BUTTON (picker));
    GtkWidget *vbox = NULL;
    GtkWidget *area_vbox = NULL;

    area_vbox = g_object_get_data(G_OBJECT(priv->window), "area_vbox");
    if (!area_vbox)
        return;
    vbox = g_object_get_data(G_OBJECT(priv->window), "custom_vbox");
    if (vbox){
        gtk_widget_destroy(vbox);
    }
    vbox = gtk_vbox_new (TRUE, 5);
    g_object_set_data(G_OBJECT(priv->window), "custom_vbox", vbox);
    gtk_box_pack_start(GTK_BOX(area_vbox),
                                   vbox, FALSE, FALSE, 5);
    if (choice) {
        if (!strcmp(choice, _("Berlin"))){
            rich_animation_additional_parametr(vbox,priv);
        }
        if (!strcmp(choice, _("Modern"))){
            rich_animation_additional_parametr(vbox,priv);
        }
        if (!strcmp(choice, _("Matrix"))){
            rich_animation_additional_parametr(vbox,priv);
        }

        if (!strcmp(choice, _("Accel"))){
            rich_animation_additional_parametr(vbox,priv);
        }
        if (!strcmp(choice, _("Video"))){
            additional_parametr_for_theme_video(vbox, priv);
        }
        if (!strcmp(choice, _("Flash"))){
            additional_parametr_for_theme_flash(vbox, priv);
        }
        if (!strcmp(choice, _("Slideshow"))){
            additional_parametr_for_theme_slideshow(vbox, priv);
        }

    }
    gtk_widget_show(vbox);
}
/********************************************************************************/
void
changed_value_category_cb (HildonPickerButton *picker, Animation_WallpaperPrivate *priv)
{
    GtkWidget *theme_button = NULL;
    GtkWidget *confirm;
    gchar *text;


    const gchar *choice = hildon_button_get_value(HILDON_BUTTON (picker));

    theme_button = g_object_get_data(G_OBJECT(priv->window), "theme_button");
    if (!theme_button)
        return;
    HildonTouchSelector * selector =  hildon_picker_button_get_selector((HildonPickerButton *) theme_button);
    hildon_touch_selector_remove_column(selector, 0);
    hildon_touch_selector_append_text_column(selector, (GtkTreeModel*)gtk_list_store_new (1, G_TYPE_STRING), TRUE);
    fill_theme_button(priv, theme_button, (gchar *)hildon_button_get_value(HILDON_BUTTON (picker)));
    /* check xscreensaver category */
    if (choice){
        if (!strcmp(choice,"Xscreensaver"))
            if (access("/usr/bin/xscreensaver", F_OK) != 0){
                text = g_strdup_printf(_("You haven't got the installed package %s. Do you want to install it via using Application Manager?"),  "Xscreensaver");
                confirm = hildon_note_new_confirmation(GTK_WINDOW(priv->window), text);
                if(GTK_RESPONSE_OK == gtk_dialog_run(GTK_DIALOG(confirm))){
                     gchar * cmd = g_strdup_printf("dbus-send --print-reply --dest=com.nokia.osso_browser /com/nokia/osso_browser/service com.nokia.osso_browser.open_new_window string:%s", "http://maemo.org/downloads/product/raw/Maemo5/xscreensaver/?get_installfile");
                     fprintf(stderr, "system %s\n", cmd);
                     system(cmd);
                     g_free(cmd);
                 }
                 g_free(text);
                 gtk_widget_destroy(confirm);
            }
    }
}
/********************************************************************************/
void
fill_theme_button (Animation_WallpaperPrivate *priv, GtkWidget *button, gchar *category){

    gchar *theme = priv->theme;
    gboolean flag = False;
    gint num=0;
        hildon_button_set_value(HILDON_BUTTON (button), NULL);
    if (!category){
        changed_value_theme_cb(HILDON_PICKER_BUTTON (button), priv);
        return;
    }
    GSList *store = priv->extheme_list;
    HildonTouchSelector * selector =  hildon_picker_button_get_selector((HildonPickerButton *) button);
    if (!selector)
        selector = (HildonTouchSelector *)hildon_touch_selector_new_text ();

    hildon_button_set_value(HILDON_BUTTON(button), NULL);
    if (!strcmp(category, "LiveWallpaper")){
        hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), _("Berlin"));
        hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), _("Modern"));
        hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), _("Matrix"));
        hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), _("Accel"));
        hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), _("Video"));
        hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), _("Slideshow"));
        num = 6;
        if (theme){
            if (!strcmp(theme, "Berlin")){
                hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, 0);
                hildon_button_set_value(HILDON_BUTTON(button), _("Berlin"));
            }
            if (!strcmp(theme, "Modern")){
                hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, 1);
                hildon_button_set_value(HILDON_BUTTON(button), _("Modern"));
            }
            if (!strcmp(theme, "Matrix")){
                hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, 2);
                hildon_button_set_value(HILDON_BUTTON(button), _("Matrix"));
            }
            if (!strcmp(theme, "Accel")){
                hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, 3);
                hildon_button_set_value(HILDON_BUTTON(button), _("Accel"));
            }
            if (!strcmp(theme, "Video")){
                hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, 4);
                hildon_button_set_value(HILDON_BUTTON(button), _("Video"));
            }
            if (!strcmp(theme, "Slideshow")){
                hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, 5);
                hildon_button_set_value(HILDON_BUTTON(button), _("Slideshow"));
            }
       }
    }
    while (store){
        if (!g_hash_table_lookup(store->data, "category"))
            continue;
        if (!strcmp(g_hash_table_lookup(store->data, "category"), category)){
            hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), _(g_hash_table_lookup(store->data, "name")));
            if (!strcmp(theme, g_hash_table_lookup(store->data, "name"))){
                hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, num);
                hildon_button_set_value(HILDON_BUTTON(button), _(g_hash_table_lookup(store->data, "name")));
            }
            num++;
        }
        store = g_slist_next(store);
    }
    /* Added Shreman's Aquarium for message */
    if (!strcmp(category, "Unknown")){
        store = priv->extheme_list;
        while (store){
            if (!strcmp("Shermans Aquarium" , g_hash_table_lookup(store->data, "name"))){
                flag = TRUE;
                break;
            }
            store = g_slist_next(store);
        }
        if (!flag)
            hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), "Shermans Aquarium");
    }

        hildon_picker_button_set_selector (HILDON_PICKER_BUTTON (button),
                                                       HILDON_TOUCH_SELECTOR (selector));

    changed_value_theme_cb(HILDON_PICKER_BUTTON (button), priv);
}
/********************************************************************************/
GtkWidget *
create_themes_button (Animation_WallpaperPrivate *priv, gchar *category){

    GtkWidget *button;
    button = hildon_picker_button_new (HILDON_SIZE_FINGER_HEIGHT, HILDON_BUTTON_ARRANGEMENT_VERTICAL);
    hildon_button_set_title (HILDON_BUTTON (button), _("Theme"));


    return button;
}
/********************************************************************************/
GtkWidget *
create_categories_button (Animation_WallpaperPrivate *priv){

    GtkWidget *button;
    GtkWidget *selector;
    gchar *theme = priv->theme;
    gint num=0;
    GHashTable *result_table = NULL;

    result_table = g_hash_table_new(g_str_hash, g_str_equal);
    selector = create_category_selector(priv);
    button = hildon_picker_button_new (HILDON_SIZE_FINGER_HEIGHT, HILDON_BUTTON_ARRANGEMENT_VERTICAL);
    hildon_button_set_title (HILDON_BUTTON (button), _("Category"));
    hildon_picker_button_set_selector (HILDON_PICKER_BUTTON (button),
                                                       HILDON_TOUCH_SELECTOR (selector));
    if (theme) {
        if (!strcmp(theme, "Berlin")){
            hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, 0);
            hildon_button_set_value(HILDON_BUTTON(button), "LiveWallpaper");
        }
        if (!strcmp(theme, "Modern")){
            hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, 0);
            hildon_button_set_value(HILDON_BUTTON(button), "LiveWallpaper");
        }
        if (!strcmp(theme, "Matrix")){
            hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, 0);
            hildon_button_set_value(HILDON_BUTTON(button), "LiveWallpaper");
        }
        if (!strcmp(theme, "Accel")){
            hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, 0);
            hildon_button_set_value(HILDON_BUTTON(button), "LiveWallpaper");
        }
        if (!strcmp(theme, "Video")){
            hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, 0);
            hildon_button_set_value(HILDON_BUTTON(button), "LiveWallpaper");
        }
        if (!strcmp(theme, "Slideshow")){
        	hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, 0);
            hildon_button_set_value(HILDON_BUTTON(button), "LiveWallpaper");
        }
        GSList *store = priv->extheme_list;
        num = 1;
        while (store){
            if (g_hash_table_lookup(result_table, g_hash_table_lookup(store->data, "category")) == NULL){
                g_hash_table_insert(result_table, g_hash_table_lookup(store->data, "category"), (gint *)num);
                num++;
            }
            if (!strcmp(theme, g_hash_table_lookup(store->data, "name"))){
                if (g_hash_table_lookup(store->data, "category")){
                    hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0,
                    (gint)g_hash_table_lookup(result_table, g_hash_table_lookup(store->data, "category")));
                    hildon_button_set_value(HILDON_BUTTON(button), g_hash_table_lookup(store->data, "category"));
                }
                break;
            }

            store = g_slist_next(store);
        }
    }
    return button;
}
/*******************************************************************************/
GtkWidget *
create_time_button (guint *active, gchar *name)
{
	GtkWidget *button;
	GtkWidget *selector;
	guint values[5] = {20, 40, 60, 120, 300};
	int i, num = 0;
	gchar *str;

	button = hildon_picker_button_new (HILDON_SIZE_FINGER_HEIGHT, HILDON_BUTTON_ARRANGEMENT_VERTICAL);
	hildon_button_set_title (HILDON_BUTTON (button), name);

	selector = hildon_touch_selector_new_text();
	for (i=0; i<5; i++){
		str = g_strdup_printf("%d", values[i]);
		hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), str);
		if (values[i] == active) num = i;
	}
	hildon_picker_button_set_selector (HILDON_PICKER_BUTTON (button),
			HILDON_TOUCH_SELECTOR (selector));
	hildon_touch_selector_set_active (HILDON_TOUCH_SELECTOR (selector), 0, num);
	if (active){
        str = g_strdup_printf("%d", active);
		hildon_button_set_value(HILDON_BUTTON(button), str);
    }else
		hildon_button_set_value(HILDON_BUTTON(button), "");
	return button;
}
/*******************************************************************************/
GtkWidget *
create_bool_button (gboolean active, gchar *name)
{
    GtkWidget *button;
    button = hildon_check_button_new (HILDON_SIZE_FINGER_HEIGHT);
    gtk_button_set_label (GTK_BUTTON (button), name);
    hildon_check_button_set_active(HILDON_CHECK_BUTTON(button), active);
    return button;
}
/*******************************************************************************/
GtkWidget *
create_enable_button (gboolean active)
{
    GtkWidget *button;
    button = hildon_check_button_new (HILDON_SIZE_FINGER_HEIGHT);
    gtk_button_set_label (GTK_BUTTON (button), _("Enable"));
    hildon_check_button_set_active(HILDON_CHECK_BUTTON(button), active);
    return button;
}

/*******************************************************************************/
void
show_settings(GtkWidget *widget, Animation_WallpaperPrivate *priv){
    lw_main_settings(priv, NULL);
}
/*******************************************************************************/
void
create_themes_buttons_hbox(Animation_WallpaperPrivate *priv){
    GtkWidget *hbox = NULL;
    GtkWidget *area_hbox = NULL;
    GtkWidget *theme_button;
    GSList *stlist = NULL;
    GConfClient *gconf_client = NULL;
    GConfValue *value = NULL;
    guint count_of_view = 1;
    guint i;


    area_hbox = g_object_get_data(G_OBJECT(priv->window), "area_hbox");
    if (!area_hbox)
        return;
    hbox = g_object_get_data(G_OBJECT(priv->window), "custom_hbox");
    if (hbox){
        gtk_widget_destroy(hbox);
    }

    hbox = gtk_hbox_new(FALSE, 0);
    g_object_set_data(G_OBJECT(priv->window), "custom_hbox", hbox);
    gtk_box_pack_start(GTK_BOX(area_hbox),
                                   hbox, FALSE, FALSE, 5);
    /* Create Theme buttons */
    theme_button = create_image_button(1, priv->dbus_conn_session);
    gtk_box_pack_start(GTK_BOX(hbox),
                                   theme_button, TRUE, TRUE, 10);
    gtk_widget_show (theme_button);
    gtk_widget_show (hbox);
    if (priv->one_in_all_view)
        return;

    gconf_client = gconf_client_get_default();
    if (!gconf_client)
        return;

    stlist = gconf_client_get_list(gconf_client,
                                       "/apps/osso/hildon-desktop/views/active",
                                       GCONF_VALUE_INT, NULL);
    if (stlist){
        count_of_view = g_slist_length(stlist);
        g_slist_free(stlist);
    }else
        count_of_view = 4;
    g_object_unref(gconf_client);
    for (i = 2; i < count_of_view + 1; i++){
        theme_button = create_image_button(i, priv->dbus_conn_session);
        gtk_box_pack_start(GTK_BOX(hbox),
                                   theme_button, TRUE, TRUE, 10);
        gtk_widget_show (theme_button);
    }
}
/*******************************************************************************/
void
lw_main_settings(Animation_WallpaperPrivate *priv, gpointer data){
    gint result;
    GtkWidget *window = NULL;
    GtkWidget *banner = NULL;
    GtkWidget *area_hbox;
    GtkWidget *scrolled_window;
    GtkWidget *one_in_all_view_button;
    gboolean one_in_all_view;
    gint i;

    window = gtk_dialog_new();
    priv->window = window;

    one_in_all_view = priv->one_in_all_view;
    gtk_window_set_title(GTK_WINDOW(window), _("Live Wallpaper Settings"));
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);
    one_in_all_view_button = create_bool_button(priv->one_in_all_view, _("One theme in all views"));
    g_signal_connect (G_OBJECT (one_in_all_view_button), "toggled",  G_CALLBACK (changed_value_one_in_all_cb), priv);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox), one_in_all_view_button, TRUE, TRUE, 5);
    area_hbox = gtk_vbox_new(FALSE, 2);
    g_object_set_data(G_OBJECT(window), "area_hbox", area_hbox);

    create_themes_buttons_hbox(priv);
    scrolled_window = hildon_pannable_area_new ();
    g_object_set (G_OBJECT (scrolled_window), "mov-mode", HILDON_MOVEMENT_MODE_HORIZ, NULL);
    g_object_set (G_OBJECT (scrolled_window), "hscrollbar-policy", GTK_POLICY_ALWAYS, NULL);
    g_object_set (G_OBJECT (scrolled_window), "scrollbar-fade-delay", 27000, NULL);
    g_object_set (G_OBJECT (scrolled_window), "indicator-width", 28, NULL);
    gtk_widget_set_size_request(scrolled_window, -1, 120);
    hildon_pannable_area_add_with_viewport(HILDON_PANNABLE_AREA (scrolled_window), GTK_WIDGET (area_hbox));

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                                   scrolled_window, TRUE, TRUE, 0);

    gtk_widget_show(scrolled_window);
    gtk_widget_show(one_in_all_view_button);
    gtk_widget_show_all(area_hbox);
    gtk_widget_show(window);
    gtk_dialog_add_button(GTK_DIALOG(window), _("About"), GTK_RESPONSE_NO);

    result = gtk_dialog_run(GTK_DIALOG(window));

    switch(result){
        case GTK_RESPONSE_NO:
            gtk_widget_destroy(window);
            window = NULL;
            lw_about();
        break;
    }

    if (hildon_check_button_get_active (HILDON_CHECK_BUTTON(one_in_all_view_button)) != one_in_all_view){
        save_one_in_all_views_to_config(hildon_check_button_get_active (HILDON_CHECK_BUTTON(one_in_all_view_button)));
        //fprintf(stderr,"CHECK!!!!!!!!!!!!!!\n");
        banner = hildon_banner_show_information (window, NULL, _("Livewallpaper is reloading..."));
        hildon_banner_set_timeout(HILDON_BANNER(banner), 3000);
        for (i=1;i<10;i++)
            stop_applet(i);
        g_timeout_add(3000, (GSourceFunc)cb_timeout_settings, window);
    }else{
        if (window)
            gtk_widget_destroy(window);
    }
}
/*******************************************************************************/
gboolean
cb_timeout_settings(GtkWidget *window){

    gint i;
    for (i=1;i<10;i++)
        start_applet(i);

    if (window)
        gtk_widget_destroy(window);
    return FALSE;
}
/*******************************************************************************/
void
file_button_clicked(GtkButton *button, Animation_WallpaperPrivate *priv){

    GtkWidget *dialog = hildon_file_chooser_dialog_new(GTK_WINDOW (priv->window), GTK_FILE_CHOOSER_ACTION_OPEN);

    if (priv->theme_string_parametr1)
        gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog), priv->theme_string_parametr1);

    gtk_widget_show_all (GTK_WIDGET (dialog));

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
      hildon_button_set_value (HILDON_BUTTON(button), gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    }
    gtk_widget_destroy (dialog);

}
/*******************************************************************************/
void
folder_button_clicked(GtkButton *button, Animation_WallpaperPrivate *priv){

    GtkWidget *dialog = hildon_file_chooser_dialog_new(GTK_WINDOW (priv->window), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);

    if (priv->theme_string_parametr1)
        gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog), priv->theme_string_parametr1);

    gtk_widget_show_all (GTK_WIDGET (dialog));

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
    	hildon_button_set_value (HILDON_BUTTON(button), gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    }
    gtk_widget_destroy (dialog);

}
/*******************************************************************************/
void
rich_animation_additional_parametr(GtkWidget *vbox, Animation_WallpaperPrivate *priv){
    GtkWidget *rich_animation_button;

    /* Create rich animation button */
    rich_animation_button = create_bool_button(priv->rich_animation, _("Rich Animation"));
    gtk_box_pack_start(GTK_BOX(vbox),
                                   rich_animation_button, TRUE, TRUE, 5);
    g_object_set_data(G_OBJECT(priv->window), "rich_animation_button", rich_animation_button);
    gtk_widget_show (rich_animation_button);
}
/*******************************************************************************/
void
link_button_clicked(GtkButton *button, gchar *url){

  gchar * cmd = g_strdup_printf("dbus-send --print-reply --dest=com.nokia.osso_browser \
         /com/nokia/osso_browser/service com.nokia.osso_browser.open_new_window string:%s", url);
  fprintf(stderr, "system %s\n", cmd);
  system(cmd);
  g_free(cmd);
}
/*******************************************************************************/
void
additional_parametr_for_theme_video(GtkWidget *vbox, Animation_WallpaperPrivate *priv){

    GtkWidget *file_button;
    GtkWidget *link_button;
    GtkWidget *smoothing_button;
    GtkWidget *rich_animation_button;

    if (priv->theme_string_parametr1)
        file_button = hildon_button_new_with_text (HILDON_SIZE_FINGER_HEIGHT, HILDON_BUTTON_ARRANGEMENT_VERTICAL,
                                                   _("Play file"), priv->theme_string_parametr1);
    else
        file_button = hildon_button_new_with_text (HILDON_SIZE_FINGER_HEIGHT,HILDON_BUTTON_ARRANGEMENT_VERTICAL,
                                                   _("Play file")," ");

    g_signal_connect (file_button, "clicked", G_CALLBACK (file_button_clicked), priv);

    gtk_box_pack_start(GTK_BOX(vbox),
                                   file_button, TRUE, TRUE, 5);
    g_object_set_data(G_OBJECT(priv->window), "filename_button", file_button);
    /* Create rich animation button */
    rich_animation_button = create_bool_button(priv->rich_animation, _("Loop"));
    gtk_box_pack_start(GTK_BOX(vbox),
                                   rich_animation_button, TRUE, TRUE, 5);
    g_object_set_data(G_OBJECT(priv->window), "rich_animation_button", rich_animation_button);
    /* Create Smoothing button */

    smoothing_button = create_bool_button(priv->theme_bool_parametr1, _("Smoothing (Need more memory)"));
    gtk_box_pack_start(GTK_BOX(vbox),
                                   smoothing_button, TRUE, TRUE, 5);
    g_object_set_data(G_OBJECT(priv->window), "smoothing_button", smoothing_button);

    link_button = hildon_button_new_with_text (HILDON_SIZE_FINGER_HEIGHT,HILDON_BUTTON_ARRANGEMENT_VERTICAL,
                                                   _("Press me for"), _("download more videos"));

    g_signal_connect (link_button, "clicked", G_CALLBACK (link_button_clicked), "http://talk.maemo.org/showthread.php?t=60185");
    gtk_box_pack_start(GTK_BOX(vbox),
                                   link_button, TRUE, TRUE, 5);

    gtk_widget_show (smoothing_button);
    gtk_widget_show (file_button);
    gtk_widget_show (rich_animation_button);
    gtk_widget_show (link_button);

}
/*******************************************************************************/
void
additional_parametr_for_theme_flash(GtkWidget *vbox, Animation_WallpaperPrivate *priv){

    GtkWidget *file_button;
    GtkWidget *link_button;
    GtkWidget *smoothing_button;
    GtkWidget *rich_animation_button;

    if (priv->theme_string_parametr1)
        file_button = hildon_button_new_with_text (HILDON_SIZE_FINGER_HEIGHT, HILDON_BUTTON_ARRANGEMENT_VERTICAL,
                                                   _("Play file"), priv->theme_string_parametr1);
    else
        file_button = hildon_button_new_with_text (HILDON_SIZE_FINGER_HEIGHT,HILDON_BUTTON_ARRANGEMENT_VERTICAL,
                                                   _("Play file")," ");

    g_signal_connect (file_button, "clicked", G_CALLBACK (file_button_clicked), priv);

    gtk_box_pack_start(GTK_BOX(vbox),
                                   file_button, TRUE, TRUE, 5);
    g_object_set_data(G_OBJECT(priv->window), "filename_button", file_button);
    gtk_widget_show (file_button);
}
/*******************************************************************************/
void
additional_parametr_for_theme_slideshow(GtkWidget *vbox, Animation_WallpaperPrivate *priv){

    GtkWidget *file_button;
    GtkWidget *time_button;

    if (priv->theme_string_parametr1)
        file_button = hildon_button_new_with_text (HILDON_SIZE_FINGER_HEIGHT, HILDON_BUTTON_ARRANGEMENT_VERTICAL,
                                                   _("Folder with images"), priv->theme_string_parametr1);
    else
        file_button = hildon_button_new_with_text (HILDON_SIZE_FINGER_HEIGHT,HILDON_BUTTON_ARRANGEMENT_VERTICAL,
                                                   _("Folder with images")," ");

    g_signal_connect (file_button, "clicked", G_CALLBACK (folder_button_clicked), priv);

    gtk_box_pack_start(GTK_BOX(vbox), file_button, TRUE, TRUE, 5);
    g_object_set_data(G_OBJECT(priv->window), "filename_button", file_button);

    if (priv->theme_int_parametr1)
    	time_button = create_time_button(priv->theme_int_parametr1, _("Time to change image"));
    else
    	time_button = create_time_button(0, _("Time to change image"));
    gtk_box_pack_start(GTK_BOX(vbox), time_button, TRUE, TRUE, 5);
    g_object_set_data(G_OBJECT(priv->window), "time_button", time_button);

    gtk_widget_show (time_button);
    gtk_widget_show (file_button);
}
/*******************************************************************************/

void
show_problem_package (GtkWidget *widget, gchar *package_name){
    gchar *text;
    text = g_strdup_printf(_("You haven't got the installed package %s. Please install it via using Application Manager"), package_name);
    hildon_banner_show_information(GTK_WIDGET(widget), NULL, text);
    g_free(text);
}
/*******************************************************************************/
void
show_duplicate_theme (GtkWidget *widget, gchar *theme_name){
    gchar *text;
    text = g_strdup_printf(_("Theme %s has already been selected"), theme_name);
    hildon_banner_show_information(GTK_WIDGET(widget), NULL, text);
    g_free(text);
}

/*******************************************************************************/
void
lw_theme_settings(GtkWidget *button, Animation_WallpaperPrivate *priv) {
    gint result;
    GtkWidget *window = NULL;
    GtkWidget *scrolled_window = NULL;
    GtkWidget *save_button;
    GtkWidget *theme_button;
    GtkWidget *category_button;
    GtkWidget *enable_button;
    GtkWidget *vbox;
    GtkWidget *area_vbox;
    GtkWidget *temp_button;
    GtkWidget *button1 = NULL, *button2 = NULL;
    GtkWidget *rich_animation_button = NULL;
    GtkWidget *confirm;
    gint view = priv->view;
    gint count, int_param;
    gchar *text;

    window = gtk_dialog_new();
    priv->window = window;

    gtk_window_set_title(GTK_WINDOW(window), _("View Settings"));
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);

    /* Create panarea */
    area_vbox = gtk_vbox_new(FALSE, 2);
    g_object_set_data(G_OBJECT(window), "area_vbox", area_vbox);
    scrolled_window = hildon_pannable_area_new ();
    hildon_pannable_area_add_with_viewport(HILDON_PANNABLE_AREA (scrolled_window), GTK_WIDGET (area_vbox));
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                                   scrolled_window, FALSE, FALSE, 0);

    gtk_widget_set_size_request(scrolled_window, -1, 370);

    /* Create Enable button */
    enable_button = create_enable_button(check_applet_state(view));
    gtk_box_pack_start(GTK_BOX(area_vbox),
                                   enable_button, FALSE, FALSE, 5);
    /* Create Category button */
    category_button = create_categories_button(priv);
    g_object_set_data(G_OBJECT(window), "category_button", category_button);
    g_signal_connect (G_OBJECT (category_button), "value-changed",  G_CALLBACK (changed_value_category_cb), priv);
    gtk_box_pack_start(GTK_BOX(area_vbox),
                                   category_button, FALSE, FALSE, 5);
    /* Create Custom vbox */
    vbox = gtk_vbox_new (FALSE, 5);
    g_object_set_data(G_OBJECT(window), "custom_vbox", vbox);

    /* Create Theme button */
    theme_button = create_themes_button(priv, (gchar *)hildon_button_get_value(HILDON_BUTTON (category_button)));
    g_object_set_data(G_OBJECT(window), "theme_button", theme_button);
    g_signal_connect (G_OBJECT (theme_button), "value-changed",  G_CALLBACK (changed_value_theme_cb), priv);
    gtk_box_pack_start(GTK_BOX(area_vbox),
                                   theme_button, FALSE, FALSE, 5);
    fill_theme_button(priv, theme_button, (gchar *)hildon_button_get_value(HILDON_BUTTON (category_button)));

    /* Pack custom vbox. It must be last widget */
    gtk_box_pack_start(GTK_BOX(area_vbox),
                                   vbox, FALSE, FALSE, 5);

    gtk_widget_show (enable_button);
    gtk_widget_show (category_button);
    gtk_widget_show (theme_button);
    gtk_widget_show (vbox);
    gtk_widget_show (area_vbox);
    gtk_widget_show (scrolled_window);
    gtk_widget_show (window);

    save_button = gtk_dialog_add_button(GTK_DIALOG(window), _("Save"), GTK_RESPONSE_YES);

    result = gtk_dialog_run(GTK_DIALOG(window));

    switch(result){
        case GTK_RESPONSE_YES:
            /* Check theme */
            if (hildon_button_get_value(HILDON_BUTTON (theme_button)) &&
                strcmp(hildon_button_get_value(HILDON_BUTTON (theme_button)), "")){

                if (priv->theme_string_parametr1){
                    g_free(priv->theme_string_parametr1);
                    priv->theme_string_parametr1 = NULL;
                }

                if (priv->theme)
                    g_free(priv->theme);
                if (!strcmp(hildon_button_get_value(HILDON_BUTTON (theme_button)), _("Berlin")))
                    priv->theme = g_strdup("Berlin");
                if (!strcmp(hildon_button_get_value(HILDON_BUTTON (theme_button)), _("Modern")))
                    priv->theme = g_strdup("Modern");
                if (!strcmp(hildon_button_get_value(HILDON_BUTTON (theme_button)), _("Matrix")))
                    priv->theme = g_strdup("Matrix");
                if (!strcmp(hildon_button_get_value(HILDON_BUTTON (theme_button)), _("Accel")))
                    priv->theme = g_strdup("Accel");
                if (!strcmp(hildon_button_get_value(HILDON_BUTTON (theme_button)), _("Video"))){
                    priv->theme = g_strdup("Video");
                    button1 = g_object_get_data(G_OBJECT(priv->window), "filename_button");
                    if (button1){
                        priv->theme_string_parametr1 = g_strdup((gchar*)hildon_button_get_value (HILDON_BUTTON(button1)));
                    }
                    temp_button = g_object_get_data(G_OBJECT(priv->window), "smoothing_button");
                    if (temp_button){
                        if (hildon_check_button_get_active (HILDON_CHECK_BUTTON(temp_button)))
                            priv->theme_bool_parametr1 = TRUE;
                        else
                            priv->theme_bool_parametr1 = FALSE;
                    }
                }
                if (!strcmp(hildon_button_get_value(HILDON_BUTTON (theme_button)), _("Flash"))){
                    button1 = g_object_get_data(G_OBJECT(priv->window), "filename_button");
                    if (button1){
                        if (priv->theme_string_parametr1)
                            g_free(priv->theme_string_parametr1);
                        priv->theme_string_parametr1 = g_strdup((gchar*)hildon_button_get_value (HILDON_BUTTON(button1)));
                    }
                }
                if (!strcmp(hildon_button_get_value(HILDON_BUTTON (theme_button)), _("Slideshow"))){
                	priv->theme = g_strdup("Slideshow");
                	button1 = g_object_get_data(G_OBJECT(priv->window), "filename_button");
                	if (button1){
                		if (priv->theme_string_parametr1)
                			g_free(priv->theme_string_parametr1);
						priv->theme_string_parametr1 = g_strdup((gchar*)hildon_button_get_value (HILDON_BUTTON(button1)));
                	}
                	button2 = g_object_get_data(G_OBJECT(priv->window), "time_button");
                	if (button2){
                		//if (priv->theme_int_parametr1)
                		//	g_free(priv->theme_int_parametr1);
                        int_param = atoi(hildon_button_get_value (HILDON_BUTTON(button2)));
                		priv->theme_int_parametr1 = int_param;
                	}
                }
                /* Check external themes */
                GSList *store = priv->extheme_list;
                while (store){
                    if (!strcmp(hildon_button_get_value(HILDON_BUTTON (theme_button)), _(g_hash_table_lookup(store->data, "name")))){
                        //check if theme installed
                    	gchar *check_path = g_hash_table_lookup(store->data, "check_path");
                    	if (check_path){
                    		if (access(check_path, F_OK) != 0){
                    			gchar * install_file = g_hash_table_lookup(store->data, "install_file");
                    			if (install_file){
                                    if (g_hash_table_lookup(store->data, "associated_package"))
                                        text = g_strdup_printf(_("You haven't got the installed package %s. Do you want to install it via using Application Manager?"),
                                                (gchar *) g_hash_table_lookup(store->data, "associated_package"));
                                    else
                                        text = g_strdup_printf(_("You haven't got the installed package %s. Do you want to install it via using Application Manager?"),
                                                (gchar *)g_hash_table_lookup(store->data, "name"));
                                    confirm = hildon_note_new_confirmation(GTK_WINDOW(window), text);
                                    if(GTK_RESPONSE_OK == gtk_dialog_run(GTK_DIALOG(confirm))){
                                        gchar * cmd = g_strdup_printf("dbus-send --print-reply --dest=com.nokia.osso_browser /com/nokia/osso_browser/service com.nokia.osso_browser.open_new_window string:%s", install_file);
                                        fprintf(stderr, "system %s\n", cmd);
                                        system(cmd);
                                        g_free(cmd);
                                    }
                                    g_free(text);
                                    gtk_widget_destroy(confirm);
                    			}else
                                    if (g_hash_table_lookup(store->data, "associated_package"))
                    			        show_problem_package(button, g_hash_table_lookup(store->data, "associated_package"));
                                    else
                    			        show_problem_package(button, g_hash_table_lookup(store->data, "name"));
                    			/* if not success exit from without saving */
                    			break;
                    		}
                    	}


                    	//check if theme already selected
                    	gchar *copies = g_hash_table_lookup(store->data, "copies");
                        //fprintf(stderr, "copies = %s\n", copies);
                        if (copies){
                            count = atoi(copies);
                        }else count = 10;
                        if (count > get_count_themes_from_config(g_hash_table_lookup(store->data, "name")))
                            priv->theme = g_strdup(g_hash_table_lookup(store->data, "name"));
                        else
                            show_duplicate_theme(button, g_hash_table_lookup(store->data, "name"));
                        //priv->hash_theme = store->data;
                        break;
                    }
                    store = g_slist_next(store);
                }
				fprintf(stderr, "theme = %s\n", priv->theme);
            }

            rich_animation_button = g_object_get_data(G_OBJECT(priv->window), "rich_animation_button");
            if (rich_animation_button){
                /* Check rich animation */
                if (hildon_check_button_get_active (HILDON_CHECK_BUTTON(rich_animation_button)))
                    priv->rich_animation = TRUE;
                else
                    priv->rich_animation = FALSE;
            }
            /* Save config */
            save_config(priv);
            /* action with applet */
            if (hildon_check_button_get_active (HILDON_CHECK_BUTTON(enable_button))){
                    if (!check_applet_state(view)){
                        start_applet(view);
                    }else {
                        send_dbus_signal (priv,
                              LIVEWP_SIGNAL_INTERFACE,
                              LIVEWP_SIGNAL_PATH,
                              LIVEWP_RELOAD_CONFIG);
                    }
            }else
                    if (check_applet_state(view))
                        stop_applet(view);

            set_button_image(button, priv, check_applet_state(view));
            break;
        default:
        case GTK_RESPONSE_OK:
        break;
        case GTK_RESPONSE_NO:
            gtk_widget_destroy(window);
            window = NULL;
            lw_about();
        break;
    }
    if (window)
        gtk_widget_destroy(window);
}
/*******************************************************************************/
gboolean
check_applet_state(gint number){

    HDConfigFile *config_file = NULL;
    GKeyFile *gkey_file = NULL;
    gchar *str = NULL;
    gboolean result = FALSE;
    if (number > 9 || number < 1)
        return FALSE;

    str = g_strdup_printf("livewp-home-widget.desktop-%i", (number - 1));
    config_file = hd_config_file_new(NULL, "/home/user/.config/hildon-desktop/", "home.plugins");

    gkey_file = hd_config_file_load_file(config_file, FALSE);
    if (gkey_file && str){
        result = g_key_file_has_group(gkey_file, str);
        g_free(str);
    }
    return result;
}
/*******************************************************************************/
void
start_applet(gint number){

    HDConfigFile *config_file = NULL;
    GKeyFile *gkey_file = NULL;
    gchar *str = NULL;

    if (number > 9 || number < 1)
        return;
    str = g_strdup_printf("livewp-home-widget.desktop-%i",(number - 1));
    config_file = hd_config_file_new(NULL, "/home/user/.config/hildon-desktop/", "home.plugins");

    gkey_file = hd_config_file_load_file(config_file, FALSE);
    if (gkey_file){
        g_key_file_set_string (gkey_file, str, "X-Desktop-File", "/usr/share/applications/hildon-home/livewp-home-widget.desktop");
        hd_config_file_save_file( config_file, gkey_file);
        g_key_file_free(gkey_file);
    }else
        fprintf(stderr, "Problem with config file");
    if (str)
        g_free(str);
    g_object_unref(config_file);
}
/*******************************************************************************/
void
stop_applet(gint number){
    HDConfigFile *config_file = NULL;
    GKeyFile *gkey_file = NULL;
    gchar *str = NULL;

    if (number > 9 || number < 1)
        return;
    str = g_strdup_printf("livewp-home-widget.desktop-%i", (number - 1));
    config_file = hd_config_file_new(NULL, "/home/user/.config/hildon-desktop/", "home.plugins");

    gkey_file = hd_config_file_load_file(config_file, FALSE);
    if (gkey_file){
         g_key_file_remove_group(gkey_file, str, NULL);
        hd_config_file_save_file( config_file, gkey_file);
        g_key_file_free(gkey_file);
    }else
        fprintf(stderr, "Problem with config file");
    if (str)
        g_free(str);
    g_object_unref(config_file);

}
