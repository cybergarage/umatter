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

#include <stdlib.h>
#include <umatter/net/socket_opt.h>

/****************************************
 * umatter_socket_option_new
 ****************************************/

uMatterSocketOption* umatter_socket_option_new()
{
  uMatterSocketOption* opt;

  opt = (uMatterSocketOption*)malloc(sizeof(uMatterSocketOption));
  if (!opt)
    return NULL;

  umatter_socket_option_setreuseaddress(opt, false);
  umatter_socket_option_setbindinterface(opt, false);
  umatter_socket_option_setmulticastloop(opt, false);

  return opt;
}

/****************************************
 * umatter_socket_option_delete
 ****************************************/

bool umatter_socket_option_delete(uMatterSocketOption* opt)
{
  if (!opt)
    return true;

  free(opt);

  return true;
}
