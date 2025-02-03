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

#include <string.h>
#include <umatter/net/interface.h>

/****************************************
 * umatter_net_interface_new
 ****************************************/

uMatterNetworkInterface* umatter_net_interface_new(void)
{
  uMatterNetworkInterface* net_if;

  net_if = (uMatterNetworkInterface*)malloc(sizeof(uMatterNetworkInterface));

  if (!net_if)
    return NULL;

  umatter_list_node_init((uMatterList*)net_if);
  net_if->name = umatter_string_new();
  net_if->ipaddr = umatter_string_new();
  net_if->netmask = umatter_string_new();
  umatter_net_interface_setindex(net_if, 0);
  memset(net_if->macaddr, 0, (size_t)UMATTER_NET_MACADDR_SIZE);

  return net_if;
}

/****************************************
 * umatter_net_interface_delete
 ****************************************/

void umatter_net_interface_delete(uMatterNetworkInterface* net_if)
{
  if (!net_if)
    return;

  umatter_list_remove((uMatterList*)net_if);
  umatter_string_delete(net_if->name);
  umatter_string_delete(net_if->ipaddr);
  umatter_string_delete(net_if->netmask);
  free(net_if);
}

/****************************************
 * umatter_net_interface_getany
 ****************************************/

uMatterNetworkInterface* umatter_net_interface_getany(void)
{
  uMatterNetworkInterface* net_if;

  net_if = umatter_net_interface_new();
  umatter_net_interface_setname(net_if, "INADDR_ANY");
  umatter_net_interface_setaddress(net_if, "0.0.0.0");

  return net_if;
}

/****************************************
 * umatter_net_interface_setname
 ****************************************/

void umatter_net_interface_setname(uMatterNetworkInterface* net_if, char* name)
{
  if (!net_if)
    return;

  umatter_string_setvalue(net_if->name, name);
}

/****************************************
 * umatter_net_interface_getname
 ****************************************/

char* umatter_net_interface_getname(uMatterNetworkInterface* net_if)
{
  if (!net_if)
    return NULL;

  return umatter_string_getvalue(net_if->name);
}

/****************************************
 * umatter_net_interface_setaddress
 ****************************************/

void umatter_net_interface_setaddress(uMatterNetworkInterface* net_if,
    char* value)
{
  if (!net_if)
    return;

  umatter_string_setvalue(net_if->ipaddr, value);
}

/****************************************
 * umatter_net_interface_getaddress
 ****************************************/

char* umatter_net_interface_getaddress(uMatterNetworkInterface* net_if)
{
  if (!net_if)
    return NULL;

  return umatter_string_getvalue(net_if->ipaddr);
}

/****************************************
 * umatter_net_interface_setnetmask
 ****************************************/

void umatter_net_interface_setnetmask(uMatterNetworkInterface* net_if,
    char* value)
{
  if (!net_if)
    return;

  umatter_string_setvalue(net_if->netmask, value);
}

/****************************************
 * umatter_net_interface_getnetmask
 ****************************************/

char* umatter_net_interface_getnetmask(uMatterNetworkInterface* net_if)
{
  if (!net_if)
    return NULL;

  return umatter_string_getvalue(net_if->netmask);
}
