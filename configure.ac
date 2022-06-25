AC_PREREQ(2.61)
AC_INIT([livewp], [0.4])
AC_CONFIG_SRCDIR([src/livewp-home-widget.c])
AC_CONFIG_HEADER([src/config.h:src/config.h.in])
AM_INIT_AUTOMAKE(livewp, 1.0.3)
AC_PROG_LIBTOOL
AC_PROG_INTLTOOL([0.23])


AC_PROG_CC
AC_HEADER_STDC
AC_PROG_INSTALL
AM_PROG_LIBTOOL

PKG_CHECK_MODULES(MAEMO, x11 libxml-2.0 hildon-1 libhildondesktop-1 hildon-fm-2 gtk+-2.0 gstreamer-1.0 gstreamer-plugins-base-1.0 gstreamer-video-1.0)
AC_SUBST(MAEMO_CFLAGS)
AC_SUBST(MAEMO_LIBS)
MAEMO_LIBS="$MAEMO_LIBS -lm"
PKG_CHECK_MODULES(OSSO, [libosso >= 1])
AC_SUBST(OSSO_LIBS)
OSSO_LIBS="$OSSO_LIBS -lm"
AC_SUBST(OSSO_CFLAGS)

PKG_CHECK_MODULES(CONTROLPANEL, hildon-control-panel)
AC_SUBST(CONTROLPANEL_CFLAGS)
AC_SUBST(CONTROLPANEL_LIBS)
CONTROLPANEL_LIBS="$CONTROLPANEL_LIBS"

# Discover where to install the .desktop file:
HILDON_HOME_DESKTOP_DIR=`pkg-config libhildondesktop-1 --variable=hildonhomedesktopentrydir`
AC_SUBST(HILDON_HOME_DESKTOP_DIR)

# Discover where to install the status widget's lib file:
HILDON_DESKTOP_LIB_DIR=`pkg-config libhildondesktop-1 --variable=hildondesktoplibdir`
AC_SUBST(HILDON_DESKTOP_LIB_DIR) 

PKG_CHECK_MODULES(SQLITE, [sqlite3])
AC_SUBST(SQLITE_LIBS)
AC_SUBST(SQLITE_CFLAGS)


# Discover where to install the panel's lib file:
HILDON_PANEL_LIB_DIR=`pkg-config hildon-control-panel --variable=pluginlibdir`
AC_SUBST(HILDON_PANEL_LIB_DIR) 

dnl Localisation
localedir=`pkg-config osso-af-settings --variable=localedir`
if test "$localedir" == ""; then
#Hack for non Maemo Platform
#    localedir="${datarootdir}/locale"
    localedir="/usr/share/locale"
    #${datadir}
fi
GETTEXT_PACKAGE=$PACKAGE
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], "${GETTEXT_PACKAGE}", [Name of gettext package])
AC_DEFINE_UNQUOTED([LOCALEDIR], "${localedir}", [Path to store localization files])
ALL_LINGUAS="ru_RU fi_FI es_ES es_MX"
AM_GLIB_GNU_GETTEXT


dnl Config files
AC_CONFIG_FILES([Makefile
		livewp-home-widget.desktop
		po/Makefile.in
		po/Makefile
		data/Makefile
		src/Makefile])
dnl Produce output files
AC_OUTPUT

