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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <umatter/net/interface.h>

/****************************************
 * global variables
 ****************************************/

static bool umatter_net_use_only_ip_v4_addr = false;
static bool umatter_net_use_only_ip_v6_addr = false;

////////////////////////////////////////////////
// umatter_net_isuseaddress
////////////////////////////////////////////////

bool umatter_net_isuseaddress(char* addr)
{
  if (umatter_net_use_only_ip_v6_addr == true) {
    if (umatter_net_isipv6address(addr) == false)
      return false;
  }
  if (umatter_net_use_only_ip_v4_addr == true) {
    if (umatter_net_isipv6address(addr) == true)
      return false;
  }
  return true;
}

////////////////////////////////////////////////
// umatter_net_getipv6address
////////////////////////////////////////////////

bool umatter_net_isipv6address(const char* addr)
{
  if (addr == NULL)
    return false;

  if (0 < umatter_strchr(addr, ":", 1))
    return true;

  return false;
}

////////////////////////////////////////////////
// umatter_net_getipv6address
////////////////////////////////////////////////

int umatter_net_getipv6scopeid(const char* addr)
{
  ssize_t per_idx;
  char scope_id_buf[8 + 1];

  if (umatter_net_isipv6address(addr) == false)
    return 0;

  per_idx = umatter_strchr(addr, "%", 1);
  if (per_idx < 0)
    return 0;

  umatter_strncpy(scope_id_buf, (addr + per_idx + 1), sizeof(scope_id_buf) - 1);
  scope_id_buf[sizeof(scope_id_buf) - 1] = '\0';

  return atoi(scope_id_buf);
}
