AC_INIT([gstreamer-media-engine], [1.0], [email@address.ext])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_CONFIG_MACRO_DIR(m4)
GTK_DOC_CHECK([1.14], [--flavour no-tmpl])

dnl Check for GLib
PKG_CHECK_MODULES(GLIB, [glib-2.0, gobject-2.0])

AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
