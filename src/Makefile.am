 # This file is part of Live Wallpaper (livewp)
 # 
 # Copyright (C) 2010 Vlad Vasiliev
 # Copyright (C) 2010 Tanya Makova
 #       for the code
 # 
 # This software is free software; you can redistribute it and/or
 # modify it under the terms of the GNU General Public License
 # as published by the Free Software Foundation; either version 2 of
 # the License, or (at your option) any later version.
 # 
 # This software is distributed in the hope that it will be useful, but
 # WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 # Lesser General Public License for more details.
 # 
 # You should have received a copy of the GNU Lesser General Public
 # License along with this software; if not, write to the Free Software
 # Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 # 02110-1301 USA

SOURCE_FILES = livewp-main.c \
	       livewp-home-widget.c livewp-home-widget.h \
	       livewp-astro.c livewp-astro.h \
	       livewp-rules.c livewp-rules.h \
	       livewp-settings.c livewp-settings.h livewp-common.h \
	       livewp-config.c livewp-config.h \
	       livewp-dbus.c livewp-dbus.h \
	       livewp-actor.c livewp-actor.h \
	       livewp-exthemes.c livewp-exthemes.h \
	       livewp-scene.c livewp-scene.h \
	       livewp-conky.c livewp-conky.h \
	       livewp-flash.c livewp-flash.h



#bin_PROGRAMS = livewp
livewp_SOURCES = $(SOURCE_FILES)
#
livewp_CFLAGS = -Wall $(MAEMO_CFLAGS) $(OSSO_CFLAGS) $(SQLITE_CFLAGS) -DAPPLICATION
#
livewp_LDADD = $(MAEMO_LIBS) $(OSSO_LIBS) $(SQLITE_LIBS)

# We invent desktoplib_LTLIBRARIES instead of the normal lib_LTLIBRARIES,
# so we can specify the non-standard installation directory.
desktoplib_LTLIBRARIES =  liblivewp-home-widget.la
desktoplibdir = $(HILDON_DESKTOP_LIB_DIR)

liblivewp_home_widget_la_SOURCES = livewp-home-widget.c livewp-home-widget.h \
				   livewp-common.h \
				   livewp-config.c livewp-config.h \
	       			   livewp-dbus.c livewp-dbus.h 
liblivewp_home_widget_la_LIBADD = $(MAEMO_LIBS) $(OSSO_LIBS) 

AM_CFLAGS = -Wall $(MAEMO_CFLAGS) $(OSSO_CFLAGS)  

panellib_LTLIBRARIES =  liblivewp-panel-widget.la
panellibdir = $(HILDON_PANEL_LIB_DIR)

liblivewp_panel_widget_la_SOURCES = livewp-settings.c livewp-settings.h livewp-common.h \
				       livewp-config.c livewp-config.h \
				       livewp-exthemes.c livewp-exthemes.h \
	       			   livewp-control-widget.c livewp-control-widget.h \
	       			   livewp-dbus.c livewp-dbus.h
liblivewp_panel_widget_la_LIBADD = $(MAEMO_LIBS) $(OSSO_LIBS) $(CONTROLPANEL_LIBS)   

liblivewp_panel_widget_la_CFLAGS = -Wall $(MAEMO_CFLAGS) $(OSSO_CFLAGS)  $(CONTROLPANEL_CFLAGS) -DCONTROLPANEL 

FIFTEEN_SOURCE_FILES = fifteen.c livewp-common.h \
					livewp-actor.c livewp-actor.h \
					livewp-astro.c livewp-astro.h \
					livewp-dbus.c livewp-dbus.h \
					livewp-rules.c livewp-rules.h 


bin_PROGRAMS = fifteen livewp
fifteen_SOURCES = $(FIFTEEN_SOURCE_FILES)
#
fifteen_CFLAGS = -Wall $(MAEMO_CFLAGS) $(OSSO_CFLAGS) $(SQLITE_CFLAGS) -DAPPLICATION
#
fifteen_LDADD = $(MAEMO_LIBS) $(OSSO_LIBS) $(SQLITE_LIBS)

