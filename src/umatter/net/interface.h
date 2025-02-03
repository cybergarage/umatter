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

#ifndef _UMATTER_NET_CINTERFACE_H_
#define _UMATTER_NET_CINTERFACE_H_

#include <umatter/typedef.h>
#include <umatter/util/list.h>
#include <umatter/util/strings.h>

#if !defined(WIN32)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

#define UMATTER_NET_IPV4_ADDRSTRING_MAXSIZE ((3 * 4) + (1 * 3) + 1)
#define UMATTER_NET_IPV6_ADDRSTRING_MAXSIZE (1 + (8 * 4) + (1 * 7) + 1 + 1)

#define UMATTER_NET_IPV4_LOOPBACK "127.0.0.1"
#define UMATTER_NET_IPV6_LOOPBACK "fixmelater"
#define UMATTER_NET_MACADDR_SIZE 6

/****************************************
 * Data Type
 ****************************************/

typedef struct {
  UMATTER_LIST_STRUCT_MEMBERS

  uMatterString* name;
  uMatterString* ipaddr;
  uMatterString* netmask;
  byte macaddr[UMATTER_NET_MACADDR_SIZE];
  int index;
} uMatterNetworkInterface, uMatterNetworkInterfaceList;

/****************************************
 * Function (NetworkInterface)
 ****************************************/

uMatterNetworkInterface* umatter_net_interface_new(void);
void umatter_net_interface_delete(uMatterNetworkInterface* net_if);
uMatterNetworkInterface* umatter_net_interface_getany(void);

#define umatter_net_interface_next(netIf) \
  (uMatterNetworkInterface*)umatter_list_next((uMatterList*)netIf)
#define umatter_net_interface_remove(netIf) \
  umatter_list_remove((uMatterList*)netIf)

void umatter_net_interface_setname(uMatterNetworkInterface* net_if, char* name);
char* umatter_net_interface_getname(uMatterNetworkInterface* net_if);
void umatter_net_interface_setaddress(uMatterNetworkInterface* net_if,
    char* ipaddr);
char* umatter_net_interface_getaddress(uMatterNetworkInterface* net_if);
void umatter_net_interface_setnetmask(uMatterNetworkInterface* net_if,
    char* ipaddr);
char* umatter_net_interface_getnetmask(uMatterNetworkInterface* net_if);
char* umatter_net_selectaddr(struct sockaddr* remoteaddr);

#define umatter_net_interface_setmacaddress(netIf, value) \
  memcpy(netIf->macaddr, value, UMATTER_NET_MACADDR_SIZE)
#define umatter_net_interface_getmacaddress(netIf, buf) \
  memcpy(buf, netIf->macaddr, UMATTER_NET_MACADDR_SIZE)

#define umatter_net_interface_setindex(netIf, value) (netIf->index = value)
#define umatter_net_interface_getindex(netIf, buf) (netIf->index)

/****************************************
 * Function (NetworkInterfaceList)
 ****************************************/

uMatterNetworkInterfaceList* umatter_net_interfacelist_new(void);
void umatter_net_interfacelist_delete(uMatterNetworkInterfaceList* net_if_list);

#define umatter_net_interfacelist_clear(netIfList) \
  umatter_list_clear(                              \
      (uMatterList*)netIfList,                     \
      (UMATTER_LIST_DESTRUCTORFUNC)umatter_net_interface_delete)
#define umatter_net_interfacelist_size(netIfList) \
  umatter_list_size((uMatterList*)netIfList)
#define umatter_net_interfacelist_gets(netIfList) \
  (uMatterNetworkInterface*)umatter_list_next((uMatterList*)netIfList)
#define umatter_net_interfacelist_add(netIfList, netIf) \
  umatter_list_add((uMatterList*)netIfList, (uMatterList*)netIf)

/****************************************
 * Function
 ****************************************/

size_t umatter_net_gethostinterfaces(uMatterNetworkInterfaceList* net_if_list);

bool umatter_net_isipv6address(const char* addr);
int umatter_net_getipv6scopeid(const char* addr);

#ifdef __cplusplus
}
#endif

#endif
