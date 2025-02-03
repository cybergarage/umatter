/******************************************************************
 *
 * Copyright (C) 2025 The uMatter for C Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef _UMATTER_NET_CSOCKET_OPTION_H_
#define _UMATTER_NET_CSOCKET_OPTION_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct UEchoSocketOption {
  bool reuse;
  bool bind;
  bool loop;
} uMatterSocketOption;

/****************************************
 * Function (Socket)
 ****************************************/

uMatterSocketOption* umatter_socket_option_new(void);
bool umatter_socket_option_delete(uMatterSocketOption* opt);

#define umatter_socket_option_setreuseaddress(opt, flag) ((opt)->reuse = flag)
#define umatter_socket_option_isreuseaddress(opt) ((opt)->reuse)

#define umatter_socket_option_setbindinterface(opt, flag) ((opt)->bind = flag)
#define umatter_socket_option_isbindinterface(opt) ((opt)->bind)

#define umatter_socket_option_setmulticastloop(opt, flag) ((opt)->loop = flag)
#define umatter_socket_option_ismulticastloop(opt) ((opt)->loop)

#ifdef __cplusplus
}
#endif

#endif
