AM_V_XSLT = $(AM_V_XSLT_$(V))
AM_V_XSLT_ = $(AM_V_XSLT_$(AM_DEFAULT_VERBOSITY))
AM_V_XSLT_0 = @echo "  XSLT    " $@;

MAN1 = capataz.1

dist_man_MANS = $(MAN1)

EXTRA_DIST = $(MAN1:%.1=%.xml)

XSLTPROC_FLAGS = \
	--nonet \
	--stringparam man.output.quietly 1 \
	--param funcsynopsis.style "'ansi'"

CLEANFILES = $(dist_man_MANS)

%.1: %.xml
	$(AM_V_XSLT)$(XSLTPROC) $(XSLTPROC_FLAGS) \
		http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl $<
