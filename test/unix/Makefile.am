######################################################################
# 
# Copyright (C) 2025 The uMatter for C Authors
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#  http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# 
######################################################################

TESTS = umattertest

check_PROGRAMS = umattertest

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src -I../


umattertest_SOURCES = \
	..//BytesTest.cpp \
	..//TestMain.cpp \
	..//InterfaceTest.cpp

#if HAVE_LIBTOOL
#umattertest_LDADD = ../../lib/unix/libumatter.la
#else
umattertest_LDADD = ../../lib/unix/libumatter.a
#endif
