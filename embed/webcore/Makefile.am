INCLUDES = \
	-I$(top_srcdir)/embed       \
	$(WEBCORE_CFLAGS)	    \
	$(GALEON_DEPENDENCY_CFLAGS) \
	-DSHARE_DIR=\"$(pkgdatadir)\"

libwebcoreembed_la_LIBADD = \
	$(WEBCORE_LIBS)

noinst_LTLIBRARIES = libwebcoreembed.la

libwebcoreembed_la_SOURCES = \
	webcore-embed.c		\
	webcore-embed.h		\
	webcore-embed-single.c	\
	webcore-embed-single.h   \
	webcore-embed-persist.c \
	webcore-embed-persist.h
