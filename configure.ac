AC_PREREQ(2.59)
AC_INIT([capataz], [0.3.1])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.11 -Wall -Wno-portability foreign tar-ustar dist-xz no-dist-gzip])
AM_MAINTAINER_MODE([enable])
AM_SILENT_RULES([yes])

#####################################################################
# Program checks
#####################################################################

AC_PROG_CC
AC_PROG_CC_STDC
AC_PATH_PROG([XSLTPROC], [xsltproc], [no])

#####################################################################
# --with-
#####################################################################

# Insert calls to AC_ARG_WITH here

#####################################################################
# --enable-
#####################################################################

AC_ARG_ENABLE([manpages],
              [AS_HELP_STRING([--disable-manpages],
                              [disable MAN pages generation @<:@default=enabled@:>@])],
              [enable_manpages="$enableval"],
              [enable_manpages="yes"])

AS_IF([test "x$enable_manpages" != "xno"],
      AS_IF([test "x$XSLTPROC" = "xno"],
            [AC_MSG_ERROR([MAN pages generation enabled but xsltproc not found])])
)

AM_CONDITIONAL([BUILD_MANPAGES], [test "x$enable_manpages" != "xno"])

AC_ARG_WITH([zshcompletiondir],
            AS_HELP_STRING([--with-zshcompletiondir=DIR],
                           [Zsh completions directory]),
            [],
            [with_zshcompletiondir=${datadir}/zsh/site-functions])

AM_CONDITIONAL(ENABLE_ZSH_COMPLETION, [test "x$with_zshcompletiondir" != "xno"])
AC_SUBST([zshcompletiondir], [$with_zshcompletiondir])

#####################################################################
# Packages checks
#####################################################################

GLIB_REQUIRED=2.32.0
PKG_CHECK_MODULES([GLIB],
                  [glib-2.0 >= $GLIB_REQUIRED
                   gobject-2.0 >= $GLIB_REQUIRED
                   gio-2.0 >= $GLIB_REQUIRED])
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

CAPATAZ_CFLAGS="$GLIB_CFLAGS"
CAPATAZ_LIBS="$GLIB_LIBS"

#####################################################################
# Function and structure checks
#####################################################################

AC_SUBST([CAPATAZ_LIBS])
AC_SUBST([CAPATAZ_CFLAGS])

#####################################################################
# Generate files from *.in
#####################################################################

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
    man/Makefile
    shell-completion/Makefile
    shell-completion/zsh/Makefile
    src/Makefile
])

#####################################################################

AC_OUTPUT
AC_MSG_RESULT([
    >>> $PACKAGE $VERSION <<<

    prefix:                 ${prefix}
    sysconfdir:             ${sysconfdir}
    bindir:                 ${bindir}

    man:                    ${enable_manpages}
    Zsh completions dir:    ${with_zshcompletiondir}
])
