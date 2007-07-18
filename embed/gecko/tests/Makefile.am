required = \
	xpcom \
	string \
	dom

bin_PROGRAMS = testgeckoembed

testgeckoembed_SOURCES = \
	testgeckoembed.cpp

testgeckoembed_CPPFLAGS = \
	-I$(top_srcdir)				\
	-I$(top_srcdir)/src			\
	-I$(top_builddir)/src			\
	$(addprefix -I$(GECKO_INCLUDE_ROOT)/,$(required)) \
	-I$(GECKO_INCLUDE_ROOT)			\
	-DGECKO_HOME=\"$(GECKO_HOME)\"		\
	$(AM_CPPFLAGS)

testgeckoembed_CXXFLAGS = \
	$(GNOME_DEPENDENCY_CFLAGS)	\
	$(GECKO_CFLAGS)			\
	$(AM_CXXFLAGS)

testgeckoembed_LDADD = \
	$(top_builddir)/src/libgnomegeckoembed-0.0.la \
	$(GNOME_DEPENDENCY_LIBS)	\
	$(GECKO_LIBS)			\
	$(GECKO_EXTRA_LIBS)		\
	$(GECKO_GLUE_LIBS)		\
	-lxpcomglue

testgeckoembed_LDFLAGS = \
	-R$(GECKO_HOME)
#	`$(MOZILLA_CONFIG) --libs $(required)`
