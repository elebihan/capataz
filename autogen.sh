#!/bin/bash
#
# Run this script to generate Makefiles.
#

report_missing() {
    echo "** Error: $1 is missing. Please install it **" 1>&2
    exit 2
}

olddir=$(pwd)
topdir=$(dirname $0)
test -n "$topdir" || topdir=.

conffile=configure.ac

(autoreconf --version) < /dev/null > /dev/null 2>&1 && {
    HAVE_AUTORECONF=yes
}

(autopoint --version) < /dev/null > /dev/null 2>&1 && {
    HAVE_AUTOPOINT=yes
}

(intltoolize --version) < /dev/null > /dev/null 2>&1 && {
    HAVE_INTLTOOLIZE=yes
}

(gtkdocize --version) < /dev/null > /dev/null 2>&1 && {
    HAVE_GTKDOCIZE=yes
}

cd $topdir

if grep "^IT_PROG_INTLTOOL" $conffile > /dev/null; then
    test -n "$HAVE_INTLTOOLIZE" || report_missing libtoolize
    test -n "$HAVE_AUTOPOINT" || report_missing autopoint
    echo "Running autopoint"
    autopoint --force
fi

if grep "^GTK_DOC_CHECK" $conffile > /dev/null; then
    test -n "$HAVE_GTKDOCIZE" || report_missing gtkdocize
    echo "Running gtkdocize"
    gtkdocize --copy
fi

test -n "$HAVE_AUTORECONF" || report_missing autoreconf
echo "Running autoreconf..."
if test -n "$HAVE_INTLTOOLIZE"; then
    AUTOPOINT='intltoolize --automake --copy' autoreconf --force --install --verbose
else
    autoreconf --force --install --verbose
fi

cd $olddir
test -n "$NOCONFIGURE" || "$topdir/configure" "$@"

# vim: ts=4 sts=4 sw=4 et ai
