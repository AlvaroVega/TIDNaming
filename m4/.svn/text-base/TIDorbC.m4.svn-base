AC_DEFUN([TID_LIB_TIDORBC],
  [

    dnl Backup compilation flags
    tidorbc_save_LDFLAGS=${LDFLAGS}
    tidorbc_save_LIBS=${LIBS}

    dnl Check for TIDorbC header files
    AC_MSG_CHECKING([for TIDorbC header files])
    if test $TIDORBC_HOME; then
      tidorbc_include_dir=["${TIDORBC_HOME}/include"];
    else
      case ${host_os} in
        linux*) tidorbc_include_dir=["/usr/include/TIDorbC/"];;
        *) tidorbc_include_dir=["/usr/include/TIDorbC/"];;
      esac
    fi
    AC_MSG_RESULT([${tidorbc_include_dir}])

    dnl Check for TIDorbC library files
    AC_MSG_CHECKING([for TIDorbC library files])
    if test $TIDORBC_HOME; then
      tidorbc_lib_dir=["${TIDORBC_HOME}/lib"];
    else
      case ${host_os} in
        linux*) tidorbc_lib_dir=["/usr/lib/"];;
        *) tidorbc_include_dir=["/usr/lib/"];;
      esac
    fi
    AC_MSG_RESULT([${tidorbc_lib_dir}])

    LIBS=["-I${tidorbc_include_dir}"]
    if test $tidorbc_lib_dir; then
       LIBS=["${LIBS} -L${tidorbc_lib_dir}"]
    fi

    dnl This ld flags are the same in all systems
    LDFLAGS=["${LDFLAGS} -lTIDorb -lTIDorbIdl -ltidsocket -ltidthr"]

    dnl OS-specific ld flags
    AC_MSG_CHECKING([for OS-specified ld flags for TIDorbC]) 
    case ${host_os} in
      solaris*)
         AC_MSG_RESULT([${host_os} uses -lsocket and -lrt])
         LDFLAGS=["${LDFLAGS} -lsocket -lrt"]
         ;;
      hpux*)
         AC_MSG_RESULT([${host_os} uses -lxnet])
         LDFLAGS=["${LDFLAGS} -lxnet"]
         ;;
      *)
         AC_MSG_RESULT([${host_os} needs none])
         ;;
    esac

    dnl Try to link a test program
    AC_MSG_CHECKING([TIDorbC linking])
    AC_LANG([C++])
    AC_LINK_IFELSE(
      AC_LANG_PROGRAM(
        [[
          #include <CORBA.h>
          CORBA::ORB_ptr orb;
          int argc;
          char ** argv;
        ]],
        [[
          orb = CORBA::ORB_init(argc,argv);
        ]]),
        [AC_MSG_RESULT([yes])],
        [AC_MSG_FAILURE([cannot link against TIDorbC libraries])]
    )
    AC_SUBST([TIDORBC_LDFLAGS], ${LDFLAGS})
    AC_SUBST([TIDORBC_LIBS], ${LIBS})
    LDFLAGS=${tidorbc_save_LDFLAGS}
    LIBS=${tidorbc_save_LIBS}]
)
