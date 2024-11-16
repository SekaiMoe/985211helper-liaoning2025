AC_PREREQ([2.71])
AC_INIT([helper],[0.0.1],[example@example.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([src/helper.cpp])
AC_CONFIG_HEADERS([config.h])

# 检查编译器和工具
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL

AC_LANG_PUSH([C++])
AC_MSG_CHECKING([for C++17 support])
AX_CXX_COMPILE_STDCXX([17])
AC_LANG_POP([C++])

AC_DEFINE([HAVE_CONFIG_H], [1], [Define if you have config.h])

# 获取当前的日期和时间
AC_ARG_VAR([BUILD_DATE], [Build Date])
AC_ARG_VAR([BUILD_TIME], [Build Time])
DATE=$(date +%Y-%m-%d)
TIME=$(date +%H:%M:%S)

# 将当前日期和时间设置为宏
AC_DEFINE_UNQUOTED([BUILD_DATE], ["$DATE"], [Build date])
AC_DEFINE_UNQUOTED([BUILD_TIME], ["$TIME"], [Build time])

# 输出日期和时间供调试查看
AC_MSG_NOTICE([Build date: $DATE])
AC_MSG_NOTICE([Build time: $TIME])

# 启用可选的静态链接
AC_ARG_ENABLE([static-build],
    [AS_HELP_STRING([--enable-static-build], [Enable static linking])],
    [STATIC_BUILD=$enableval], [STATIC_BUILD=no])
AM_CONDITIONAL([STATIC_BUILD], [test "x$STATIC_BUILD" = "xyes"])

# 启用调试模式
AC_ARG_ENABLE([debug-mode],
    [AS_HELP_STRING([--enable-debug-mode], [Enable debug mode])],
    [DEBUG_MODE=$enableval], [DEBUG_MODE=no])
AM_CONDITIONAL([DEBUG_MODE], [test "x$DEBUG_MODE" = "xyes"])

# 启用 WEBUI
AC_ARG_ENABLE([webui],
    [AS_HELP_STRING([--enable-webui], [Enable WEBUI interface])],
    [APPUI_WEBUI=$enableval], [APPUI_WEBUI=no])
AM_CONDITIONAL([APPUI_WEBUI], [test "x$APPUI_WEBUI" = "xyes"])

# 检查必要工具
AC_CHECK_PROG([CCACHE_FOUND], [ccache], [yes], [no])
if test "$CCACHE_FOUND" = "yes"; then
    AC_MSG_NOTICE([Using ccache])
    CC="ccache gcc"
    CXX="ccache g++"
else
    AC_MSG_NOTICE([Ccache not found])
fi

# 检查 Python
AC_PATH_PROG([PYTHON], [python3])
if test -z "$PYTHON"; then
    AC_MSG_ERROR([Python is required but not found])
fi

# 包含检查是否启用 ccache
AC_PATH_PROG([CCACHE], [ccache])
if test -n "$CCACHE"; then
    AC_MSG_NOTICE([Using ccache: $CCACHE])
fi

AC_CONFIG_FILES([
Makefile
])
AC_OUTPUT

