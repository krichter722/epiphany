noinst_LTLIBRARIES = libephywebkitembed.la

libephywebkitembed_la_SOURCES = 	\
	AutoJSContextStack.cpp		\
	AutoJSContextStack.h		\
	AutoWindowModalState.cpp	\
	AutoWindowModalState.h		\
	ContentHandler.cpp		\
	ContentHandler.h		\
	EphyAboutModule.cpp		\
	EphyAboutModule.h		\
	EphyBadCertRejector.cpp		\
	EphyBadCertRejector.h		\
	EphyContentPolicy.cpp		\
	EphyContentPolicy.h		\
	EphyDirectoryProvider.cpp       \
	EphyDirectoryProvider.h		\
	EphyHeaderSniffer.cpp		\
	EphyHeaderSniffer.h		\
	EphyBrowser.cpp                 \
	EphyBrowser.h	                \
	EphyFind.cpp			\
	EphyFind.h			\
	EphyHistoryListener.cpp		\
	EphyHistoryListener.h		\
	EphyPromptService.cpp		\
	EphyPromptService.h		\
	EphyRedirectChannel.cpp		\
	EphyRedirectChannel.h		\
	EphySidebar.cpp			\
	EphySidebar.h			\
	EphySingle.cpp			\
	EphySingle.h			\
	EphyUtils.cpp			\
	EphyUtils.h			\
	EventContext.cpp                \
	EventContext.h                  \
	GeckoCookiePromptService.cpp	\
	GeckoCookiePromptService.h	\
	GeckoFormSigningDialog.cpp	\
	GeckoFormSigningDialog.h	\
	GeckoPrintService.cpp		\
	GeckoPrintService.h		\
	GeckoPrintSession.cpp		\
	GeckoPrintSession.h		\
	GlobalHistory.cpp               \
	GlobalHistory.h                 \
	MozDownload.cpp			\
	MozDownload.h			\
	MozRegisterComponents.cpp       \
	MozRegisterComponents.h         \
	mozilla-download.cpp		\
	mozilla-download.h		\
	webkit-embed.cpp	        \
	webkit-embed.h			\
	mozilla-embed-event.cpp		\
	mozilla-embed-event.h		\
	mozilla-embed-find.cpp		\
	mozilla-embed-find.h		\
	webkit-embed-persist.cpp       \
	webkit-embed-persist.h		\
	webkit-embed-single.cpp	\
	webkit-embed-single.h	        \
	mozilla-notifiers.cpp		\
	mozilla-notifiers.h

if ENABLE_FILEPICKER
libephywebkitembed_la_SOURCES += 	\
	FilePicker.cpp			\
	FilePicker.h
endif

if HAVE_MOZILLA_PSM
libephywebkitembed_la_SOURCES += 	\
	GtkNSSClientAuthDialogs.cpp	\
	GtkNSSClientAuthDialogs.h	\
	GtkNSSDialogs.cpp		\
	GtkNSSDialogs.h			\
	GtkNSSKeyPairDialogs.cpp	\
	GtkNSSKeyPairDialogs.h		\
	GtkNSSSecurityWarningDialogs.cpp\
	GtkNSSSecurityWarningDialogs.h
endif

if ENABLE_CERTIFICATE_MANAGER
libephywebkitembed_la_SOURCES += \
	mozilla-x509-cert.cpp		\
	mozilla-x509-cert.h
endif

if ENABLE_SPELLCHECKER
libephywebkitembed_la_SOURCES += \
	GeckoSpellCheckEngine.cpp	\
	GeckoSpellCheckEngine.h
endif

webkit_include_subdirs = \
	.			\
	caps			\
	chardet			\
	chrome			\
	commandhandler		\
	content			\
	cookie			\
	docshell		\
	dom			\
	exthandler		\
	fastfind		\
	helperAppDlg		\
	find			\
	gfx			\
	gtkembedmoz		\
	history			\
	js			\
	layout			\
	locale			\
	mimetype		\
	necko			\
	nkcache			\
	passwordmgr		\
	pipboot			\
	pipnss			\
	pref			\
	shistory		\
	sidebar			\
	spellchecker		\
	uriloader		\
	uconv			\
	wallet			\
	webbrowserpersist	\
	webbrwsr		\
	widget			\
	windowwatcher		\
	xmlextras		\
	xpcom			\
	xpconnect

libephywebkitembed_la_CPPFLAGS = \
	-I$(top_srcdir)/lib	    		\
	-I$(top_srcdir)/embed	    		\
	-I$(top_srcdir)		    		\
	$(addprefix -I$(MOZILLA_INCLUDE_ROOT)/,$(webkit_include_subdirs))	\
	-DSHARE_DIR=\"$(pkgdatadir)\"   	\
	-DPLUGINDIR=\"$(libdir)/epiphany/$(EPIPHANY_MAJOR)/plugins\"		\
	-DMOZILLA_HOME=\"$(MOZILLA_HOME)\"	\
	-DMOZILLA_PREFIX=\"$(MOZILLA_PREFIX)\"	\
	-DMOZILLA_NATIVE_PLUGINSDIR=\"$(libdir)/mozilla/plugins\"		\
	-DUA_VERSION=\"$(EPIPHANY_UA_VERSION)\"	\
	-DALLOW_PRIVATE_API			\
	$(AM_CPPFLAGS)

libephywebkitembed_la_CXXFLAGS = \
	$(GECKO_CFLAGS)		\
	$(DEPENDENCIES_CFLAGS) 	\
	$(AM_CXXFLAGS)
