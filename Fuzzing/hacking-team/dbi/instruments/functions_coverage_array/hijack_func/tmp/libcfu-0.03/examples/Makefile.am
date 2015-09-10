noinst_PROGRAMS = hash_usage large_hash strings conf_example conf_example2 opt_example
LDADD = -L$(top_srcdir)/src -lcfu

if DEBUG
AM_CFLAGS = -Wall -W -Werror -I$(top_srcdir)/include -DCFU_DEBUG=1
else
AM_CFLAGS = -Wall -W -Werror -I$(top_srcdir)/include
endif

AM_LDFLAGS =  -Xlinker -rpath -Xlinker $(top_src_dir)/src

