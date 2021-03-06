SUBDIRS = bookmarks . search-provider profile-migrator

pkglib_LTLIBRARIES = libephymain.la

bin_PROGRAMS = epiphany

dist_noinst_DATA = \
	epiphany.gresource.xml \
	$(RESOURCE_FILES)

TYPES_H_FILES = \
	ephy-link.h				\
	ephy-session.h				\
	ephy-shell.h				\
	ephy-window.h

libephymain_la_SOURCES = \
	clear-data-dialog.c			\
	clear-data-dialog.h			\
	cookies-dialog.c			\
	cookies-dialog.h			\
	ephy-action-helper.c			\
	ephy-action-helper.h			\
	ephy-completion-model.c			\
	ephy-completion-model.h			\
	ephy-encoding-dialog.c			\
	ephy-encoding-dialog.h			\
	ephy-encoding-row.c			\
	ephy-encoding-row.h			\
	ephy-header-bar.c			\
	ephy-header-bar.h			\
	ephy-history-window.c			\
	ephy-history-window.h			\
	ephy-link.c				\
	ephy-link.h				\
	ephy-location-controller.c		\
	ephy-location-controller.h		\
	ephy-lockdown.c				\
	ephy-lockdown.h				\
	ephy-notebook.c				\
	ephy-notebook.h				\
	ephy-private.h				\
	ephy-session.c				\
	ephy-session.h				\
	ephy-shell.c				\
	ephy-shell.h				\
	ephy-window.c				\
	ephy-window.h				\
	languages.h				\
	passwords-dialog.c			\
	passwords-dialog.h			\
	popup-commands.c			\
	popup-commands.h			\
	prefs-dialog.c				\
	prefs-dialog.h				\
	window-commands.c			\
	window-commands.h

nodist_libephymain_la_SOURCES = \
	$(TYPES_SOURCE)

libephymain_la_CPPFLAGS = \
	-I$(top_builddir)/embed		\
	-I$(top_builddir)/lib		\
	-I$(top_builddir)/lib/widgets	\
	-I$(top_srcdir)/src/bookmarks	\
	-I$(top_srcdir)/embed 		\
	-I$(top_srcdir)/lib   		\
	-I$(top_srcdir)/lib/egg		\
	-I$(top_srcdir)/lib/history	\
	-I$(top_srcdir)/lib/widgets   	\
	-I$(top_srcdir)/gvdb		\
	-DLOCALEDIR=\"$(localedir)\"	\
	-DPKGDATADIR=\"$(pkgdatadir)\"	\
	$(GDK_CFLAGS)			\
	$(GIO_CFLAGS)			\
	$(GLIB_CFLAGS)			\
	$(GTK_CFLAGS)			\
	$(LIBNOTIFY_CFLAGS)		\
	$(LIBSECRET_CFLAGS)		\
	$(LIBSOUP_CFLAGS)		\
	$(LIBXML_CFLAGS)		\
	$(WEBKIT2GTK_CFLAGS)

libephymain_la_CFLAGS = \
	$(WARN_CFLAGS)

libephymain_la_LDFLAGS = \
	$(WARN_LDFLAGS) -avoid-version -no-undefined

libephymain_la_LIBADD = \
	$(top_builddir)/src/bookmarks/libephybookmarks.la	\
	$(top_builddir)/embed/libephyembed.la			\
	$(top_builddir)/lib/libephymisc.la			\
	$(top_builddir)/lib/egg/libegg.la			\
	$(GDK_LIBS)		\
	$(GIO_LIBS)		\
	$(GLIB_LIBS)		\
	$(GTK_LIBS)		\
	$(LIBNOTIFY_LIBS)	\
	$(LIBSECRET_LIBS)	\
	$(LIBSOUP_LIBS)		\
	$(LIBXML_LIBS)		\
	$(WEBKIT2GTK_LIBS)

RESOURCE_FILES = \
	resources/about.css			  					\
	resources/channel-insecure-symbolic.png						\
	resources/computer-fail-symbolic.png						\
	resources/clear-data-dialog.ui							\
	resources/cookies-dialog.ui							\
	resources/encoding-dialog.ui							\
	resources/encoding-row.ui							\
	resources/epiphany-application-menu.ui						\
	resources/epiphany.css								\
	resources/error.html								\
	resources/gtk/menus.ui								\
	resources/gtk/page-menu-popover.ui						\
	resources/history-dialog.ui							\
	resources/incognito.png								\
	resources/missing-thumbnail.png							\
	resources/passwords-dialog.ui							\
	resources/prefs-dialog.ui							\
	resources/prefs-lang-dialog.ui							\
	resources/prism.css								\
	resources/prism.js								\
	resources/shortcuts-dialog.ui							\
	resources/ephy-bookmark-tag-symbolic.svg					\
	resources/ephy-bookmarks-symbolic.svg						\
	resources/gtk/bookmark-properties-grid.ui					\
	resources/gtk/bookmark-row.ui							\
	resources/gtk/bookmarks-popover.ui						\
	resources/gtk/menus.ui								\
	$(NULL)

epiphany-resources.c: epiphany.gresource.xml $(RESOURCE_FILES)
	$(AM_V_GEN) glib-compile-resources --target=$@ --sourcedir=$(srcdir)/resources --generate-source --c-name epiphany $(srcdir)/epiphany.gresource.xml

epiphany-resources.h: epiphany.gresource.xml $(RESOURCE_FILES)
	$(AM_V_GEN) glib-compile-resources --target=$@ --sourcedir=$(srcdir)/resources --generate-header --c-name epiphany $(srcdir)/epiphany.gresource.xml

EPIPHANY_RESOURCES = \
	epiphany-resources.c \
	epiphany-resources.h

BUILT_SOURCES = \
	$(EPIPHANY_RESOURCES) \
	$(TYPES_SOURCE)

epiphany_SOURCES = ephy-main.c \
	$(EPIPHANY_RESOURCES)

epiphany_CFLAGS = \
	$(WARN_CFLAGS)

epiphany_CPPFLAGS = \
	-I$(top_builddir)/lib		\
	-I$(top_srcdir)/embed 		\
	-I$(top_srcdir)/lib   		\
	-I$(top_srcdir)/lib/egg		\
	-I$(top_srcdir)/lib/history	\
	-I$(top_srcdir)/lib/widgets	\
	-I$(top_builddir)/lib/widgets	\
	-I$(top_srcdir)/src/bookmarks	\
	-DGETTEXT_PACKAGE=\"$(GETTEXT_PACKAGE)\" \
	-DLOCALEDIR=\"$(localedir)\"	\
	$(GLIB_CFLAGS)			\
	$(GTK_CFLAGS)			\
	$(LIBNOTIFY_CFLAGS)		\
	$(LIBXML_CFLAGS)		\
	$(WEBKIT2GTK_CFLAGS)

epiphany_LDADD = \
	$(top_builddir)/lib/libephymisc.la	\
	libephymain.la		\
	$(GLIB_LIBS)		\
	$(GTK_LIBS)		\
	$(LIBNOTIFY_LIBS)	\
	$(LIBXML_LIBS)		\
	$(WEBKIT2GTK_LIBS)

epiphany_LDFLAGS = \
	-dlopen self	\
	$(WARN_LDFLAGS)

TYPES_SOURCE = \
	ephy-type-builtins.c	\
	ephy-type-builtins.h

stamp_files = \
	stamp-ephy-type-builtins.c	\
	stamp-ephy-type-builtins.h

ephy-type-builtins.c: stamp-ephy-type-builtins.c Makefile
	@true
stamp-ephy-type-builtins.c: Makefile $(TYPES_H_FILES)
	$(AM_V_GEN) $(GLIB_MKENUMS) \
		--fhead "#include <config.h>\n\n" \
		--fhead "#include \"ephy-type-builtins.h\"\n\n" \
		--fprod "\n/* enumerations from \"@filename@\" */" \
		--fprod "\n#include \"@filename@\"" \
		--vhead "GType\n@enum_name@_get_type (void)\n{\n" \
		--vhead "  static GType type = 0;\n\n" \
		--vhead "  if (G_UNLIKELY (type == 0))\n  {\n" \
		--vhead "    static const G@Type@Value _@enum_name@_values[] = {" \
		--vprod "      { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
		--vtail "      { 0, NULL, NULL }\n    };\n\n" \
		--vtail "    type = g_@type@_register_static (\"@EnumName@\", _@enum_name@_values);\n  }\n\n" \
		--vtail "  return type;\n}\n\n" \
		$(filter-out $<,$^) > xgen-$(@F) \
	&& ( cmp -s xgen-$(@F) $(@F:stamp-%=%) || cp xgen-$(@F) $(@F:stamp-%=%) ) \
	&& rm -f xgen-$(@F) \
	&& echo timestamp > $(@F)

ephy-type-builtins.h: stamp-ephy-type-builtins.h Makefile
	@true
stamp-ephy-type-builtins.h: Makefile $(TYPES_H_FILES)
	$(AM_V_GEN) $(GLIB_MKENUMS) \
		--fhead "#ifndef EPHY_TYPE_BUILTINS_H\n" \
		--fhead "#define EPHY_TYPE_BUILTINS_H 1\n\n" \
		--fhead "#include <glib-object.h>\n\n" \
		--fhead "G_BEGIN_DECLS\n\n" \
		--ftail "G_END_DECLS\n\n" \
		--ftail "#endif /* EPHY_TYPE_BUILTINS_H */\n" \
		--fprod "\n/* --- @filename@ --- */" \
		--eprod "#define EPHY_TYPE_@ENUMSHORT@ @enum_name@_get_type()\n" \
		--eprod "GType @enum_name@_get_type (void);\n" \
		$(filter-out $<,$^) > xgen-$(@F) \
	&& ( cmp -s xgen-$(@F) $(@F:stamp-%=%) || cp xgen-$(@F) $(@F:stamp-%=%) ) \
	&& rm -f xgen-$(@F) \
	&& echo timestamp > $(@F)

CLEANFILES = \
	$(stamp_files) \
	$(BUILT_SOURCES) \
	resources/.sass-cache

-include $(top_srcdir)/git.mk
