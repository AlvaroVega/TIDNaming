AC_DEFUN([TID_JRE],
  [

    dnl Check for JAVA_HOME variable
    AC_MSG_CHECKING([for JAVA_HOME variable])
    if test -z ${JAVA_HOME} ; then
			if test ${JDK_HOME}; then
				JAVA_HOME=${JDK_HOME}
				AC_MSG_RESULT([using JDK_HOME])
			else
      	AC_MSG_FAILURE([environment variable JAVA_HOME is not set])
			fi
    fi
    AC_MSG_RESULT([yes])

    dnl Check for Java Runtime Machine binary
		AC_MSG_CHECKING([for java runtime machine binary])
		if test -x ${JAVA_HOME}/bin/java; then
			JAVA="${JAVA_HOME}/bin/java"
	    AC_MSG_RESULT([${JAVA}])
		else
			AC_MSG_FAILURE([not found])
		fi

    dnl Check for Java Runtime Machine version
    AC_MSG_CHECKING([java runtime environment version])

		java_version=`${JAVA} -version 2>&1 | grep version | cut -d' ' -f3 | \
			sed 's/\"//g' | sed 's/_.*//g'`
		java_major_version=`echo $java_version | cut -d"." -f1,2`
		java_minor_version=`echo $java_version | cut -d"." -f3`

		java_needed_version=$1
		java_needed_major_version=`echo $java_needed_version | cut -d"." -f1,2`
		java_needed_minor_version=`echo $java_needed_version | cut -d"." -f3`
		java_invalid_version_msg="java runtime machine version must be at least ${java_needed_version}"

		if test x"`echo "${java_major_version} < ${java_needed_major_version}" | bc`" = x"1"; then
			AC_MSG_FAILURE([${java_invalid_version_msg}])
		elif test x"$java_major_version" = x"$java_needed_major_version"; then
			if test x"`echo "${java_minor_version} < ${java_needed_minor_version}" | bc`" = x"1"; then
				AC_MSG_FAILURE([${java_invalid_version_msg}])
  		fi
		fi

    AC_MSG_RESULT([${java_version}])

  ]
)
