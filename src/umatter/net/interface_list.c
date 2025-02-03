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

#include <umatter/net/interface.h>
#include <umatter/util/list.h>

/****************************************
 * umatter_net_interfacelist_new
 ****************************************/

uMatterNetworkInterfaceList* umatter_net_interfacelist_new(void)
{
  uMatterNetworkInterfaceList* net_if_list;

  net_if_list = (uMatterNetworkInterfaceList*)malloc(sizeof(uMatterNetworkInterfaceList));
  if (!net_if_list)
    return NULL;

  umatter_list_header_init((uMatterList*)net_if_list);
  net_if_list->name = NULL;
  net_if_list->ipaddr = NULL;

  return net_if_list;
}

/****************************************
 * umatter_net_interfacelist_delete
 ****************************************/

void umatter_net_interfacelist_delete(uMatterNetworkInterfaceList* net_if_list)
{
  umatter_net_interfacelist_clear(net_if_list);
  free(net_if_list);
}
