##################################################################
#
# uMatter for C
#
# Copyright (C) The uMatter for C Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADERS([config.h])

AC_INIT([umatter],[1.0.0])
AC_CONFIG_SRCDIR([include/umatter/typedef.h])
AM_INIT_AUTOMAKE([subdir-objects])

##############################
# Checks for programs.
##############################

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_AWK
#AC_PROG_LIBTOOL
AC_PROG_RANLIB

##############################
# Checks for header files.
##############################

AC_CHECK_HEADERS([stdbool.h])

##############################
# Checks for pthread
##############################

AC_CHECK_HEADERS([pthread.h],,[AC_MSG_ERROR(uMatter needs POSIX thread library)])
AC_CHECK_LIB([pthread],[main])

##############################
# Checks for Network functions
##############################

AC_CHECK_FUNCS([socket])
AC_CHECK_FUNCS([inet_ntoa])
AC_CHECK_HEADERS([ifaddrs.h])
AC_CHECK_FUNCS([getifaddrs])

##### socklen_t ####
AC_MSG_CHECKING(for socklen_t)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <sys/socket.h>
socklen_t foosocket;
]], [[]])],[AC_MSG_RESULT(yes)
AC_DEFINE([HAVE_SOCKLEN_T],1,[SOCKLEN_T])],[AC_MSG_RESULT(no)
])

##### SIOCGIFHWADDR ####
AC_MSG_CHECKING(for SIOCGIFHWADDR)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
void func()
{
  int sock;
  struct ifreq ifr;
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
  ifr.ifr_addr.sa_family = AF_INET;
  ioctl(sock, SIOCGIFHWADDR, &ifr);
}
]], [[]])],[AC_MSG_RESULT(yes)
AC_DEFINE([HAVE_SIOCGIFHWADDR],1,[SIOCGIFHWADDR])],[AC_MSG_RESULT(no)
])

##### sockaddr_dl ####
AC_MSG_CHECKING(for sockaddr_dl)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <sys/socket.h>
void func()
{
  struct ifaddrs *ifaddr;
  struct ifaddrs *i;
  struct sockaddr_dl *dl;
  unsigned char *macaddr;
  getifaddrs(&ifaddr);
  dl = (struct sockaddr_dl *)(ifaddr->ifa_addr);
  LLADDR(dl);
  freeifaddrs(ifaddr);
}
]], [[]])],[AC_MSG_RESULT(yes)
AC_DEFINE([HAVE_SOCKADDR_DL],1,[SOCKADDR_DL])],[AC_MSG_RESULT(no)
])

##### IP_PKTINFO ####
AC_MSG_CHECKING(for IP_PKTINFO)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <sys/socket.h>
void func()
{
  int on=1;
  setsockopt(0, IPPROTO_IP, IP_PKTINFO,  &on, sizeof(on));
}
]], [[]])],[AC_MSG_RESULT(yes)
AC_DEFINE([HAVE_IP_PKTINFO],1,[IP_PKTINFO])],[AC_MSG_RESULT(no)
])

##### SO_NOSIGPIPE ####
AC_MSG_CHECKING(for SO_NOSIGPIPE)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <sys/socket.h>
void func()
{
  int on=1;
  setsockopt(0, SOL_SOCKET, SO_NOSIGPIPE,  &on, sizeof(on));
}
]], [[]])],[AC_MSG_RESULT(yes)
AC_DEFINE([HAVE_SO_NOSIGPIPE],1,[SO_NOSIGPIPES])],[AC_MSG_RESULT(no)
])

##############################
# Debug
##############################

AC_ARG_ENABLE(
 	[debug],
	AS_HELP_STRING([--enable-debug],[ enable debugging (default = no) ]),
	[case "${enableval}" in
    	yes | no ) enable_debug="${enableval}" ;;
	esac],
	[]
)

if [ test "$enable_debug" = yes ]; then
	AC_DEFINE([DEBUG],1,[Define to 1 if you want to debug])
fi

##############################
# Testing
##############################

AC_ARG_ENABLE(
 	[test],
	AS_HELP_STRING([--enable-test],[ build tests (default = no) ]),
	[case "${enableval}" in
    	yes | no ) build_test="${enableval}" ;;
	esac],
	[]
)

AM_CONDITIONAL(UMATTER_ENABLE_TEST,test "$build_test" = yes)
if [ test "$build_test" = yes ]; then
	AC_CHECK_LIB([boost_system-mt],[main],,
		[AC_CHECK_LIB([boost_system],[main],,[AC_MSG_ERROR(uMatter needs boost::system)])])
	AC_CHECK_LIB([boost_unit_test_framework-mt],[main],,
		[AC_CHECK_LIB([boost_unit_test_framework],[main],,[AC_MSG_ERROR(uMatter needs boost::unit_test_framework)])])
fi

##############################
# Examples
##############################

AC_ARG_ENABLE(
	[examples],
	AS_HELP_STRING([--enable-examples],[ build examples (default = yes) ]),
	[case "${enableval}" in
	yes | no ) build_examples="${enableval}" ;;
	esac],
	[build_examples="yes"]
)
AM_CONDITIONAL(UMATTER_ENABLE_EXAMPLES,test "$build_examples" = yes)

##############################
# Makefiles
##############################

AC_CONFIG_FILES([
Makefile
include/Makefile
lib/Makefile
lib/unix/Makefile
test/Makefile
test/unix/Makefile
])
AC_OUTPUT
