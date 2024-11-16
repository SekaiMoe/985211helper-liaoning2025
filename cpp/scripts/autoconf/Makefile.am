bin_PROGRAMS = helper

AUTOMAKE_OPTIONS = foreign subdir-objects

# 可执行文件的源文件
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

# 头文件包含路径
AM_CPPFLAGS = -I$(top_srcdir)/include

# 链接器标志
AM_LDFLAGS = $(LDFLAGS)

# 根据选项启用编译器标志
if STATIC_BUILD
AM_LDFLAGS += -static
endif

if APPUI_WEBUI
AM_CPPFLAGS += -DAPPUI=\"WEBUI\" -DWEBUI
else
AM_CPPFLAGS += -DAPPUI=\"TUI\" -DTUI
endif

# 如果找到 ccache，则启用它
#if USE_CCACHE
#CXX = $(CCACHE) $(CXX)
#endif

# 添加编译选项
AM_CXXFLAGS = -std=c++17 -Wall -Wextra -fstack-clash-protection -fstack-protector-all

# 安装目标
install-exec-hook:
	@echo "Installing helper to /usr/local/bin"

