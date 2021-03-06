noinst_LTLIBRARIES = libephywidgets.la

BUILT_SOURCES = \
	ephy-widgets-type-builtins.c	\
	ephy-widgets-type-builtins.h

TYPES_H_FILES = \
	ephy-location-entry.h

CLEANFILES = $(stamp_files) $(BUILT_SOURCES)

stamp_files = \
	stamp-ephy-widgets-type-builtins.c	\
	stamp-ephy-widgets-type-builtins.h

ephy-widgets-type-builtins.c: stamp-ephy-widgets-type-builtins.c Makefile
	@true

stamp-ephy-widgets-type-builtins.c: Makefile $(TYPES_H_FILES)
	$(AM_V_GEN) $(GLIB_MKENUMS) \
		--fhead "#include <config.h>\n\n" \
		--fhead "#include \"ephy-widgets-type-builtins.h\"\n\n" \
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

ephy-widgets-type-builtins.h: stamp-ephy-widgets-type-builtins.h Makefile
	@true

stamp-ephy-widgets-type-builtins.h: Makefile $(TYPES_H_FILES)
	$(AM_V_GEN) $(GLIB_MKENUMS) \
		--fhead "#ifndef EPHY_WIDGETS_TYPE_BUILTINS_H\n" \
		--fhead "#define EPHY_WIDGETS_TYPE_BUILTINS_H 1\n\n" \
		--fhead "#include <glib-object.h>\n\n" \
		--fhead "G_BEGIN_DECLS\n\n" \
		--ftail "G_END_DECLS\n\n" \
		--ftail "#endif /* EPHY_WIDGETS_TYPE_BUILTINS_H */\n" \
		--fprod "\n/* --- @filename@ --- */" \
		--eprod "#define EPHY_TYPE_@ENUMSHORT@ @enum_name@_get_type()\n" \
		--eprod "GType @enum_name@_get_type (void);\n" \
		$(filter-out $<,$^) > xgen-$(@F) \
	&& ( cmp -s xgen-$(@F) $(@F:stamp-%=%) || cp xgen-$(@F) $(@F:stamp-%=%) ) \
	&& rm -f xgen-$(@F) \
	&& echo timestamp > $(@F)

nodist_libephywidgets_la_SOURCES = \
	$(BUILT_SOURCES)

libephywidgets_la_SOURCES = \
	ephy-certificate-dialog.c		\
	ephy-certificate-dialog.h		\
	ephy-downloads-popover.h		\
	ephy-downloads-popover.c		\
	ephy-downloads-progress-icon.h		\
	ephy-downloads-progress-icon.c		\
	ephy-download-widget.c			\
	ephy-download-widget.h			\
	ephy-file-chooser.c			\
	ephy-file-chooser.h			\
	ephy-location-entry.c			\
	ephy-location-entry.h			\
	ephy-middle-clickable-button.c		\
	ephy-middle-clickable-button.h		\
	ephy-node-view.c			\
	ephy-node-view.h			\
	ephy-security-popover.c			\
	ephy-security-popover.h			\
	ephy-title-box.c			\
	ephy-title-box.h			\
	ephy-title-widget.c			\
	ephy-title-widget.h			\
	ephy-tree-model-node.c			\
	ephy-tree-model-node.h			\
	ephy-tree-model-sort.c			\
	ephy-tree-model-sort.h			\
	nautilus-floating-bar.c			\
	nautilus-floating-bar.h

libephywidgets_la_CFLAGS = \
	$(WARN_CFLAGS)

libephywidgets_la_CPPFLAGS = \
	-I$(top_builddir)/lib		\
	-I$(top_builddir)/lib/widgets	\
	-I$(top_srcdir)/embed		\
	-I$(top_srcdir)/lib		\
	-I$(top_srcdir)/lib/egg		\
	-I$(top_srcdir)/lib/history	\
	-I$(top_srcdir)/libgd		\
	$(GCR_CFLAGS)			\
	$(GDK_CFLAGS)			\
	$(GDK_PIXBUF_CFLAGS)		\
	$(GIO_CFLAGS)			\
	$(GLIB_CFLAGS)			\
	$(GTK_CFLAGS)			\
	$(LIBSOUP_CFLAGS)		\
	$(WEBKIT2GTK_CFLAGS)

libephywidgets_la_LDFLAGS = \
	$(WARN_LDFLAGS)

libephywidgets_la_LIBADD = \
	$(top_builddir)/libgd/libgd.la	\
	$(GCR_LIBS)		\
	$(GDK_LIBS)		\
	$(GDK_PIXBUF_LIBS)	\
	$(GIO_LIBS)		\
	$(GLIB_LIBS)		\
	$(GTK_LIBS)		\
	$(LIBSOUP_LIBS)		\
	$(WEBKIT2GTK_LIBS)

-include $(top_srcdir)/git.mk
