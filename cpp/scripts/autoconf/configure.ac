AC_PREREQ([2.69])
AC_INIT([helper], [0.0.1], [support@example.com])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([src/helper.cpp])
AC_CONFIG_HEADERS([config.h])

# 检查编译器
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# 设置C++标准
AX_CXX_COMPILE_STDCXX([17], [mandatory])

# 检查需要的头文件
AC_CHECK_HEADERS([iostream])

# 定义编译器标志
CXXFLAGS="-Wall -Wno-unused-result -fstack-clash-protection -fstack-protector-all -std=c++17 -pipe"
LDFLAGS="-ffunction-sections -fdata-sections -z now -z noexecstack -fPIE -flto"

# 检查是否有ccache
AC_PATH_PROG([CCACHE], [ccache], [no])
if test "x$CCACHE" != "xno"; then
    CC="$CCACHE $CC"
    CXX="$CCACHE $CXX"
    AC_MSG_NOTICE([Using ccache: $CCACHE])
else
    AC_MSG_NOTICE([Ccache not found. Compiling with cache will be disabled.])
fi

# 定义调试模式选项
AC_ARG_ENABLE([debug-mode],
    [AS_HELP_STRING([--enable-debug-mode], [Enable debugging mode])],
    [if test "x$enableval" = "xyes"; then
        DEBUG_MODE=true
        CXXFLAGS="$CXXFLAGS -g -O0 -Wextra -Wpedantic"
    else
        DEBUG_MODE=false
        CXXFLAGS="$CXXFLAGS -O2"
    fi],
    [DEBUG_MODE=false]
)
AM_CONDITIONAL([DEBUG_MODE], [test "$DEBUG_MODE" = "true"])

# 定义WebUI选项
AC_ARG_ENABLE([webui],
    [AS_HELP_STRING([--enable-webui], [Enable WebUI (Web User Interface)])],
    [if test "x$enableval" = "xyes"; then
        AC_DEFINE([APPUI], [WEBUI], [Define APPUI as WEBUI])
    else
        AC_DEFINE([APPUI], [TUI], [Define APPUI as TUI])
    fi]
)

# 检查Python
AM_PATH_PYTHON([2.7], [], [AC_MSG_ERROR([Cannot build test suites without Python])])

# 启用测试
AM_CONDITIONAL([BUILD_TEST], [test "x$enable_tests" = "xyes"])

# 输出配置文件
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

