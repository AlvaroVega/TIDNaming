AC_DEFUN([TID_DOXYGEN],
  [

    dnl Check for Java Runtime Machine binary
    AC_MSG_CHECKING([for doxygen binary])
    if test -x doxygen; then
      AM_CONDITIONAL(GENERATE_DOXYGEN_DOC, true)
      AC_SUBST([DOXYGEN_BIN], doxygen)
      AC_MSG_RESULT([${JAVA}])
    else
      AM_CONDITIONAL(GENERATE_DOXYGEN_DOC, false)
      AC_MSG_RESULT([not found, doxygen documentation will not be generated])
    fi

  ]
)
