SUBDIRS = src examples doc
if DEBUG
AM_CFLAGS = -Wall -W -Werror -DCFU_VERSION="\"@VERSION@\"" -DCFU_DEBUG=1
else
AM_CFLAGS = -Wall -W -Werror -DCFU_VERSION="\"@VERSION@\""
endif

LIBCFU_TYPE_u_int=@LIBCFU_TYPE_u_int@

INCLUDES = -I$(top_srcdir)/include

EXTRA_DIST = include/cfutypes.h.in include/cfu.h.in
