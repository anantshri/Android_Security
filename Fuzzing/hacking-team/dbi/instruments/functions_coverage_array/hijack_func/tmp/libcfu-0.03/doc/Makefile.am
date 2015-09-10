MAKEINFO = case $@ in			\
	*.info) echo "  INFO    $@";; 	\
	*.html) echo "  HTML    $@";; 	\
	esac && @MAKEINFO@
TEXI2DVI = && case $@ in		\
	*.dvi) echo "  DVI      $@";;	\
	*.pdf) echo "  PDF      $@";;	\
	esac && texi2dvi --quiet
DVIPS = echo "  PS      $@" && dvips -q

info_TEXINFOS = libcfu.texi

.SILENT:
