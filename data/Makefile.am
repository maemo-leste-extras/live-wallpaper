#
# This file is part of live wallpaper 
#
# Copyright (C) 2010 Vlad Vasiliev
# Copyright (C) 2010 Tanya Makova
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
 
berlinthemedir = $(datadir)/livewp/theme/Berlin
berlintheme_DATA = theme/Berlin/* 

modernthemedir = $(datadir)/livewp/theme/Modern
moderntheme_DATA = theme/Modern/*  

matrixthemedir = $(datadir)/livewp/theme/Matrix
matrixtheme_DATA = theme/Matrix/* 

accelthemedir = $(datadir)/livewp/theme/Accel
acceltheme_DATA = theme/Accel/* 

fifteenthemedir = $(datadir)/livewp/theme/Fifteen
fifteentheme_DATA = theme/Fifteen/* 

liqflowthemedir = $(datadir)/livewp/theme/Liqflow
liqflowtheme_DATA = theme/Liqflow/* 

crazythemedir = $(datadir)/livewp/theme/CrazyChickens
crazytheme_DATA = theme/CrazyChickens/* 

#orrerythemedir = $(datadir)/livewp/theme/Orrery
#orrerytheme_DATA = theme/Orrery/* 

videothemedir = $(datadir)/livewp/theme/Video
videotheme_DATA = theme/Video/* 

flashthemedir = $(datadir)/livewp/theme/Flash
flashtheme_DATA = theme/Flash/* 

slideshowthemedir = $(datadir)/livewp/theme/Slideshow
slideshowtheme_DATA = theme/Slideshow/* 

xsnowthemedir = $(datadir)/livewp/theme/Xsnow
xsnowtheme_DATA = theme/Xsnow/* 

colorfloodthemedir = $(datadir)/livewp/theme/Colorflood
colorfloodtheme_DATA = theme/Colorflood/* 

conkythemedir = $(datadir)/livewp/theme/Conky
conkytheme_DATA = theme/Conky/* 

externalthemedir = $(datadir)/livewp/external_themes
externaltheme_DATA = external_themes/*.xml 

dbus_servicedir = $(datadir)/dbus-1/services
dbus_service_DATA = org.maemo.livewp.service


desktoppaneldir    = $(datadir)/applications/hildon-control-panel
desktoppanel_files = livewp.desktop
desktoppanel_DATA  = $(desktoppanel_files)

# The icons

icondir = $(datadir)/icons/hicolor

install-data-local:
	i=livewp; \
	$(mkinstalldirs) $(DESTDIR)$(icondir)/26x26/hildon; \
	$(mkinstalldirs) $(DESTDIR)$(icondir)/40x40/hildon; \
	$(mkinstalldirs) $(DESTDIR)$(icondir)/scalable/hildon; \
	$(INSTALL_DATA) $(srcdir)/icons/$$i.26.png $(DESTDIR)$(icondir)/26x26/hildon/$$i.png; \
	$(INSTALL_DATA) $(srcdir)/icons/$$i.40.png $(DESTDIR)$(icondir)/40x40/hildon/$$i.png; \
	$(INSTALL_DATA) $(srcdir)/icons/$$i.40.png $(DESTDIR)$(icondir)/scalable/hildon/$$i.png;

uninstall-local:
	i=livewp; \
	rm $(DESTDIR)$(icondir)/26x26/hildon/$$i.png; \
	rm $(DESTDIR)$(icondir)/40x40/hildon/$$i.png; \
	$(DESTDIR)$(icondir)/scalable/hildon/$$i.png;


EXTRA_DIST = $(berlintheme_DATA) \
	     $(moderntheme_DATA) \
	     $(matrixtheme_DATA) \
	     $(acceltheme_DATA) \
	     $(fifteentheme_DATA) \
	     $(crazytheme_DATA) \
	     $(liqflowtheme_DATA) \
	     $(videotheme_DATA) \
	     $(flashtheme_DATA) \
	     $(slideshowtheme_DATA) \
	     $(xshowtheme_DATA) \
	     $(conkytheme_DATA) \
	     $(colorfloodtheme_DATA) \
	     $(externaltheme_DATA) \
	     $(dbus_service_DATA)


livewp.desktop: livewp.desktop.in
	sed "s/__VERSION__/$(VERSION)/g" $< >$@.tmp && \
	sed "s/__PACKAGE__/$(PACKAGE)/g" $@.tmp >$@ && \
	$(RM) *.tmp

