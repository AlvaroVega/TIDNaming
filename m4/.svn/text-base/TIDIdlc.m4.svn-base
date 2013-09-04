AC_DEFUN([TID_PROG_TIDIDLC],
  [

    dnl Check for TIDIDLC_HOME variable
    AC_MSG_CHECKING([for TIDIDLC_HOME variable])
    if test -z ${TIDIDLC_HOME} ; then
      AC_MSG_FAILURE([not found])
    else
      AC_MSG_RESULT([found])
      idlc_bin=${TIDIDLC_HOME}/bin/idl2cpp
      AC_MSG_CHECKING([for ${idlc_bin} to execute])
      if test -x ${idlc_bin}; then
        AC_MSG_RESULT([yes])
        AC_SUBST([IDLC], ${idlc_bin})
      else
        AC_MSG_FAILURE([cannot be executed])
      fi
    fi

    dnl Check for IDL compiler correct version
    AC_MSG_CHECKING([IDL compiler version])

    idlc_version=`${IDLC} --version | grep version | cut -d" " -f5`
    idlc_major_version=`echo $idlc_version | cut -d"." -f1,2`
    idlc_minor_version=`echo $idlc_version | cut -d"." -f3`

    idlc_needed_version=$1
    idlc_needed_major_version=`echo $idlc_needed_version | cut -d"." -f1,2`
    idlc_needed_minor_version=`echo $idlc_needed_version | cut -d"." -f3`
    idlc_invalid_version_msg="IDL compiler version must be at least ${idlc_needed_version}"

    if test x"`echo "$idlc_major_version < $idlc_needed_major_version" | bc`" = x"1"; then
      AC_MSG_FAILURE([${idlc_invalid_version_msg}])
    elif test x"$idlc_major_version" = x"$idlc_needed_major_version"; then
      if test x"`echo "$idlc_minor_version < $idlc_needed_minor_version" | bc`" = x"1"; then
        AC_MSG_FAILURE([${idlc_invalid_version_msg}])
      fi
    fi

    AC_MSG_RESULT([${idlc_version}])

    AC_MSG_CHECKING([if IDL compiler works])
    foo_idl=".foo_idl"
    foo_idl_body="module foo_module{interface foo_interface{};};"
    echo "${foo_idl_body}" > ${foo_idl}.idl
    ${IDLC} ${foo_idl}.idl&>/dev/null
    result="$?"
    rm ${foo_idl}.idl ${foo_idl}.h ${foo_idl}.C POA_${foo_idl}.h POA_${foo_idl}.C
    if test x"$result" = x"0"; then 
      AC_MSG_RESULT([yes])
    else
      AC_MSG_FAILURE([IDL compiler fails])
    fi

  ]
)
