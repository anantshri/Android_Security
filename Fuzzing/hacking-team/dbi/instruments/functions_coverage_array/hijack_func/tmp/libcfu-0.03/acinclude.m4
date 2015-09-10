AC_DEFUN([LIBCFU_CHECK_TYPE], [
AC_CHECK_TYPE($1, LIBCFU_TYPE_$1="/* $1 is defined */",
        LIBCFU_TYPE_$1="typedef $2 $1;")
])
