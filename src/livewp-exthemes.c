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
#include "livewp-exthemes.h"
/*******************************************************************************/

GHashTable  *
parse_theme(gchar *file){
    GHashTable  *hash;
    xmlDoc *doc = NULL;
    xmlNode *root = NULL, *first_child, *node;
    doc = xmlReadFile(file, NULL, 0);
    if (!doc)
        return NULL;
    root = xmlDocGetRootElement(doc);
    if (!root)
        return NULL;
    first_child = root->children;
    hash = g_hash_table_new(g_str_hash, g_str_equal);
    for (node = first_child; node; node = node->next){
        //if (node-type == XML_ELEMENT_NODE && xmlStrcmp(node->name, "param")){
          //  child =
        //}
        if (node->type == XML_ELEMENT_NODE){
            //fprintf(stderr, "%s => %s\n", node->name, xmlNodeGetContent(node));
            g_hash_table_insert(hash, g_strdup((gchar*)node->name), g_strdup((gchar*)xmlNodeGetContent(node)));
        }
    }
    if (!(g_hash_table_lookup(hash, "category")))
        g_hash_table_insert(hash, g_strdup("category"), g_strdup("Unknown"));
    if (!(g_hash_table_lookup(hash, "name")))
        g_hash_table_insert(hash, g_strdup("name"), g_strdup("Unknown"));

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return hash;
}

/*******************************************************************************/
gint compar (gpointer a, gpointer b){
    return strcmp( (g_hash_table_lookup(a, "name")), (g_hash_table_lookup(b, "name")) );
}


GSList *
get_list_exthemes(void){
    Dirent  *dp;
    DIR     *dir_fd;
    GSList *store = NULL;
    GHashTable *hash = NULL;
    gchar *ext;
    int i;

    dir_fd = opendir(EXTHEME_PATH);
    if(dir_fd){

        while((dp = readdir(dir_fd))){

            if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
                continue;

            ext = strrchr(dp->d_name, '.') + 1; /* get file extension */
            if (!ext) continue;
            /* lowercase string */
            for(i=0; ext[i]; i++)
            	ext[i] = tolower(ext[i]);
            /* read only xml files */
            if (strcmp(ext, "xml"))
            	continue;

            if(dp->d_type == DT_REG || dp->d_type == DT_LNK){
                hash = parse_theme(g_strdup_printf("%s%s", EXTHEME_PATH, dp->d_name));
                if (hash)
                    store = g_slist_append(store, hash);
            }
        }
        closedir(dir_fd);
    }
    store = g_slist_sort(store, (GCompareFunc)compar);
    return store;
}
