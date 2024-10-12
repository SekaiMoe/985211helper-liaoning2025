AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = helper
helper_SOURCES = \
    src/helper.cpp \
    src/index.cpp \
    src/json.cpp \
    src/search.cpp \
    src/signal.cpp \
    src/usage.cpp \
    src/version.cpp \
    src/data.cpp \
    src/web/web.cpp

AM_CPPFLAGS = -I$(top_srcdir)/include

AM_CXXFLAGS = -Wall -Wno-unused-result -fstack-clash-protection -fstack-protector-all -std=c++17 -pipe
if DEBUG_MODE
    AM_CXXFLAGS += -Wextra -Wpedantic -Winit-self -Wlogical-op
endif

helper_LDFLAGS = -ffunction-sections -fdata-sections -z now -z noexecstack -fPIE -flto

install-exec-hook:
    $(INSTALL) helper $(DESTDIR)/usr/local/bin/

