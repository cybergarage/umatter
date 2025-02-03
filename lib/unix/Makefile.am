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

#if HAVE_LIBTOOL
#lib_LTLIBRARIES = libumatter.la
#else
lib_LIBRARIES = libumatter.a
#endif

AM_CFLAGS = -I$(top_builddir)/include -I$(top_builddir)/src

umatter_sources = \
	../../src/umatter/net/interface_function.c \
	../../src/umatter/net/interface.c \
	../../src/umatter/net/datagram_packet.c \
	../../src/umatter/net/interface_list.c \
	../../src/umatter/net/net_function.c \
	../../src/umatter/net/socket.c \
	../../src/umatter/net/socket_opt.c \
	../../src/umatter/util/log.c \
	../../src/umatter/util/thread.c \
	../../src/umatter/util/list.c \
	../../src/umatter/util/timer.c \
	../../src/umatter/util/strings.c \
	../../src/umatter/util/bytes.c \
	../../src/umatter/util/thread_list.c \
	../../src/umatter/util/mutex.c \
	../../src/umatter/util/cond.c \
	../../src/umatter/util/strings_function.c \
	../../src/umatter/util/strings_tokenizer.c \
	../../src/umatter/util/logs.c

libumatterincludedir = $(includedir)/umatter
nobase_libumatterinclude_HEADERS =  \
        $(umatter_headers)

#if HAVE_LIBTOOL
#libumatter_la_SOURCES = \
#        $(umatter_source)
#libumatter_la_CFLAGS = $(AM_CFLAGS)
#else
libumatter_a_SOURCES = \
        $(umatter_sources)
libumatter_a_CFLAGS = $(AM_CFLAGS)
#endif
