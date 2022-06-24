SUBDIRS = data src po

# .desktop file
desktopdir = $(HILDON_HOME_DESKTOP_DIR)
desktop_DATA = livewp-home-widget.desktop

DISTCLEANFILES = $(desktop_DATA)
deb:
	./make-package $(PACKAGE) $(VERSION)
