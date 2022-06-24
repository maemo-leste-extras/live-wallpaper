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
#include "livewp-config.h"
/*******************************************************************************/
gint
current_active_view(void){
    GConfClient *gconf_client = NULL;
    gint result = -1;

    gconf_client = gconf_client_get_default();
    if (!gconf_client) {
        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        return result;
    }
    result = gconf_client_get_int(gconf_client, "/apps/osso/hildon-desktop/views/current", NULL);

    return result;
}

/*******************************************************************************/
#ifdef APPLICATION
void
fill_priv(Animation_WallpaperPrivate *priv)
{
    /* Reset data */
    priv->hash_theme = NULL;
    /* Load config */
    read_config(priv);
    /* Set function */
    if (!strcmp(priv->theme, "Accel"))
        priv->scene_func = (gpointer)&init_scene_Accel;
    if (!strcmp(priv->theme, "Berlin"))
        priv->scene_func = (gpointer)&init_scene_Berlin;
    if (!strcmp(priv->theme, "Modern"))
        priv->scene_func = (gpointer)&init_scene_Modern;
    if (!strcmp(priv->theme, "Matrix"))
        priv->scene_func = (gpointer)&init_scene_Matrix;
    if (!strcmp(priv->theme, "Video"))
        priv->scene_func = (gpointer)&init_scene_Video;
    if (!strcmp(priv->theme, "Slideshow"))
    	priv->scene_func = (gpointer)&init_scene_Slideshow;

    priv->extheme_list = get_list_exthemes();
    GSList *store = priv->extheme_list;
    while (store){
        if (!strcmp(priv->theme, g_hash_table_lookup(store->data, "name"))){
            /* Default function for external themes init_scene_External */
            if (priv->scene_func)
                priv->scene_func = (gpointer)&init_scene_External;
            if (!strcmp(priv->theme, "Conky"))
                priv->scene_func = (gpointer)&init_scene_Conky;
            if (!strcmp(priv->theme, "Flash"))
                priv->scene_func = (gpointer)&init_scene_Flash;
            priv->hash_theme = store->data;
            break;
        }
        store = g_slist_next(store);
    }

}
#endif
/*******************************************************************************/
gint
read_config(Animation_WallpaperPrivate *priv) {

    GConfClient *gconf_client = NULL;
    gchar *tmp = NULL;
    GConfValue *value = NULL;
    gint id = priv->view;
    gchar * str = NULL;

    gconf_client = gconf_client_get_default();
    if (!gconf_client) {
        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        return -1;
    }
    /* get Theme default Modern */
    str = g_strdup_printf("%s%i", GCONF_KEY_THEME, id);
    tmp = gconf_client_get_string(gconf_client,
                                  str, NULL);
    if (str){
        g_free(str);
        str = NULL;
    }
    if (tmp){
        priv->theme = tmp;
    }else
        priv->theme = g_strdup("Modern");
    /* get Rich animation default TRUE */
    str = g_strdup_printf("%s%i", GCONF_KEY_RANIMATION, id);
    value = gconf_client_get(gconf_client, str, NULL);
    if (str){
        g_free(str);
        str = NULL;
    }
    if (value) {
        priv->rich_animation = gconf_value_get_bool(value);
        gconf_value_free(value);
    } else
        priv->rich_animation = TRUE;
    /* get theme additional bool aparametr 1 default  TRUE */
    str = g_strdup_printf("%s%i", GCONF_KEY_ADDITIONAL_BOOL_1, id);
    value = gconf_client_get(gconf_client, str, NULL);
    if (str){
        g_free(str);
        str = NULL;
    }
    if (value) {
        priv->theme_bool_parametr1 = gconf_value_get_bool(value);
        gconf_value_free(value);
    } else
       priv->theme_bool_parametr1= TRUE;

    /* get theme additional parameter 1  */
    str = g_strdup_printf("%s%i", GCONF_KEY_ADDITIONAL_STRING_1 , id);
    value = gconf_client_get(gconf_client, str, NULL);
    if (str){
        g_free(str);
        str = NULL;
    }
    if (value) {
        priv->theme_string_parametr1 = g_strdup(gconf_value_get_string(value));
        gconf_value_free(value);
    }
    /* get theme additional int parameter 1  */
    str = g_strdup_printf("%s%i", GCONF_KEY_ADDITIONAL_INT_1 , id);
    value = gconf_client_get(gconf_client, str, NULL);
    if (str){
        g_free(str);
        str = NULL;
    }
    if (value) {
        priv->theme_int_parametr1 = gconf_value_get_int(value);
        gconf_value_free(value);
    }

    /* get parameter one theme in all view */
    priv->one_in_all_view = get_one_in_all_views_from_config();
    return 0;
}
/*******************************************************************************/
gboolean
get_one_in_all_views_from_config(void){
    GConfClient *gconf_client;
    GConfValue *value = NULL;
    gboolean result;

    gconf_client = gconf_client_get_default();
    if (!gconf_client) {
        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        return FALSE;
    }
    /* get parameter one theme in all view */
    value = gconf_client_get(gconf_client, GCONF_KEY_ONE_IN_ALL_VIEW, NULL);
    if (value) {
        result = gconf_value_get_bool(value);
        gconf_value_free(value);
    } else
        result = FALSE;
    return result;
}
/*******************************************************************************/
void
save_one_in_all_views_to_config(gboolean one_in_all_views){
    GConfClient *gconf_client;

    gconf_client = gconf_client_get_default();
    if (!gconf_client) {
        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        return;
    }

    if (one_in_all_views)
        gconf_client_set_bool(gconf_client,
                              GCONF_KEY_ONE_IN_ALL_VIEW, TRUE, NULL);
    else
        gconf_client_set_bool(gconf_client,
                              GCONF_KEY_ONE_IN_ALL_VIEW, FALSE, NULL);

}
/*******************************************************************************/
void
save_config(Animation_WallpaperPrivate *priv) {

    GConfClient *gconf_client;
    gchar * str = NULL;
    gint id = priv->view;

    gconf_client = gconf_client_get_default();
    if (!gconf_client) {
        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        return;
    }

    if (priv->theme){
        str = g_strdup_printf("%s%i", GCONF_KEY_THEME, id);
        gconf_client_set_string(gconf_client,
                  str,
                  priv->theme, NULL);
        if (str){
            g_free(str);
            str = NULL;
        }
    }

    str = g_strdup_printf("%s%i", GCONF_KEY_RANIMATION, id);
    if (priv->rich_animation)
        gconf_client_set_bool(gconf_client,
                              str, TRUE, NULL);
    else
        gconf_client_set_bool(gconf_client,
                              str, FALSE, NULL);
    if (str){
        g_free(str);
        str = NULL;
    }
    str = g_strdup_printf("%s%i", GCONF_KEY_ADDITIONAL_BOOL_1, id);
    if (priv->theme_bool_parametr1)
        gconf_client_set_bool(gconf_client,
                              str, TRUE, NULL);
    else
        gconf_client_set_bool(gconf_client,
                              str, FALSE, NULL);
    if (str){
        g_free(str);
        str = NULL;
    }

    str = g_strdup_printf("%s%i",GCONF_KEY_ADDITIONAL_STRING_1, id);
    if (priv->theme_string_parametr1){
        gconf_client_set_string(gconf_client,
                  str,
                  priv->theme_string_parametr1, NULL);
    }else
        gconf_client_unset(gconf_client, str, NULL);
    if (str){
            g_free(str);
            str = NULL;
    }
    str = g_strdup_printf("%s%i",GCONF_KEY_ADDITIONAL_INT_1, id);
    if (priv->theme_int_parametr1){
        gconf_client_set_int(gconf_client,
                  str,
                  priv->theme_int_parametr1, NULL);
    }else
        gconf_client_unset(gconf_client, str, NULL);
    if (str){
            g_free(str);
            str = NULL;
    }

}
/*******************************************************************************/
gint
get_count_themes_from_config(gchar *theme_name){
    GConfClient *gconf_client;
    gint i, count;
    gchar *str, *value;

    gconf_client = gconf_client_get_default();
    if (!gconf_client) {
        fprintf(stderr, _("Failed to initialize GConf. Quitting.\n"));
        return FALSE;
    }
    /* get count such themes */
    count = 0;
    for (i=1; i<=9; i++){
        str = g_strdup_printf("%s%i", GCONF_KEY_THEME, i);
        value = gconf_client_get_string(gconf_client, str, NULL);
        if (str){
            g_free(str);
            str = NULL;
        }
        if (value && !strcmp(value, theme_name)){
            count++;
        }

    }
    //fprintf(stderr, "get count_theme from config = %d\n", count);
    return count;
}

