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
#include <umatter/net/socket.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>

#if defined(WIN32)
#define UECNO_USE_WIN32_GETADAPTERSINFO 1
#endif

#if defined(__APPLE_CC__) && !defined(HAVE_IFADDRS_H)
#define HAVE_IFADDRS_H 1
#endif

#if defined(WIN32)
#include <Iphlpapi.h>
#include <Iptypes.h>
#else
#if defined(HAVE_IFADDRS_H)
#include <ifaddrs.h>
#if defined(HAVE_SIOCGIFHWADDR)
#include <net/if.h>
#include <sys/ioctl.h>
#endif
#else
#include <sys/ioctl.h>
#endif
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

/****************************************
 * umatter_net_gethostinterfaces (WIN32)
 * (WINCE follows below)
 ****************************************/

#if defined(WIN32)

size_t umatter_net_gethostinterfaces(uMatterNetworkInterfaceList* netIfList)
{
#if !defined(UECNO_USE_WIN32_GETHOSTADDRESSES) && !defined(UECNO_USE_WIN32_GETADAPTERSINFO)
  uMatterNetworkInterface* netIf;
  SOCKET sd;
  int nNumInterfaces;
  INTERFACE_INFO InterfaceList[20];
  unsigned long nBytesReturned, *pnBytesReturned = &nBytesReturned;
  struct sockaddr_in* pAddress;
  struct sockaddr_in* pNetmask;
  char* host;
  char* netmask;
  u_long nFlags;
  int i;

  umatter_socket_startup();
  umatter_net_interfacelist_clear(netIfList);

  sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
  if (sd == INVALID_SOCKET) {
    return 0;
  }

  if (WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, &InterfaceList, sizeof(InterfaceList), &nBytesReturned, 0, 0) == SOCKET_ERROR)
    return 0;

  nNumInterfaces = nBytesReturned / sizeof(INTERFACE_INFO);
  for (i = 0; i < nNumInterfaces; ++i) {
    nFlags = InterfaceList[i].iiFlags;
    if (nFlags & IFF_LOOPBACK)
      continue;
    if (!(nFlags & IFF_UP))
      continue;

    netIf = umatter_net_interface_new();

    pAddress = (struct sockaddr_in*)&(InterfaceList[i].iiAddress);
    host = inet_ntoa(pAddress->sin_addr);
    umatter_net_interface_setaddress(netIf, host);

    pNetmask = (struct sockaddr_in*)&(InterfaceList[i].iiNetmask);
    netmask = inet_ntoa(pNetmask->sin_addr);
    umatter_net_interface_setnetmask(netIf, netmask);

    umatter_net_interfacelist_add(netIfList, netIf);
  }

#elif defined(UECNO_USE_WIN32_GETADAPTERSINFO)
#pragma comment(lib, "Iphlpapi.lib")

  uMatterNetworkInterface* netIf;
  PIP_ADAPTER_INFO pAdapterInfo = NULL, pAdapter = NULL;
  ULONG ulOutBufLen;
  DWORD dwRetVal;
  DWORD nOfInterfaces;

  umatter_socket_startup();
  umatter_net_interfacelist_clear(netIfList);

  ulOutBufLen = sizeof(IP_ADAPTER_INFO);
  pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
  if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
    free(pAdapterInfo);
    pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
  }

  if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
    for (pAdapter = pAdapterInfo, nOfInterfaces = 0; pAdapter;
        pAdapter = pAdapter->Next, ++nOfInterfaces) {
      if (pAdapter->Type == MIB_IF_TYPE_LOOPBACK)
        continue;
      if (umatter_streq(pAdapter->IpAddressList.IpAddress.String, "0.0.0.0"))
        continue;
      netIf = umatter_net_interface_new();
      umatter_net_interface_setaddress(
          netIf, pAdapter->IpAddressList.IpAddress.String);
      umatter_net_interface_setnetmask(netIf,
          pAdapter->IpAddressList.IpMask.String);
      if (pAdapter->AddressLength == umatter_NET_MACADDR_SIZE)
        umatter_net_interface_setmacaddress(netIf, pAdapter->Address);
      umatter_net_interfacelist_add(netIfList, netIf);
    }
  }
  free(pAdapterInfo);

#elif defined(UECNO_USE_WIN32_GETHOSTADDRESSES)
#pragma comment(lib, "Iphlpapi.lib")

  IP_ADAPTER_ADDRESSES *pAdapterAddresses, *ai;
  DWORD ifFlags;
  ULONG outBufLen;
  IP_ADAPTER_UNICAST_ADDRESS* uai;
  SOCKET_ADDRESS sockaddr;
  SOCKADDR* saddr;
  INT saddrlen;
  char addr[NI_MAXHOST];
  char port[NI_MAXSERV];
  int namInfoRet;
  int ifIdx;
  uMatterNetworkInterface* netIf;

  umatter_socket_startup();
  umatter_net_interfacelist_clear(netIfList);

  outBufLen = 0;
  ifFlags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_FRIENDLY_NAME | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER;

  GetAdaptersAddresses(AF_UNSPEC, ifFlags, NULL, NULL, &outBufLen);
  pAdapterAddresses = (IP_ADAPTER_ADDRESSES*)LocalAlloc(LMEM_ZEROINIT, outBufLen);
  GetAdaptersAddresses(AF_UNSPEC, ifFlags, NULL, pAdapterAddresses, &outBufLen);
  ai = pAdapterAddresses;
  while (ai != NULL) {
    if (ai->OperStatus != IfOperStatusUp) {
      ai = ai->Next;
      continue;
    }
    if (ai->IfType == IF_TYPE_SOFTWARE_LOOPBACK) {
      ai = ai->Next;
      continue;
    }
    if (ai->IfType == IF_TYPE_TUNNEL) {
      ai = ai->Next;
      continue;
    }
    uai = ai->FirstUnicastAddress;
    while (uai != NULL) {
      sockaddr = uai->Address;
      saddr = sockaddr.lpSockaddr;
      saddrlen = sockaddr.iSockaddrLength;
      namInfoRet = getnameinfo(saddr, saddrlen, addr, sizeof(addr), port, sizeof(port), NI_NUMERICHOST | NI_NUMERICSERV);
      if (namInfoRet == 0) {
        // if (IsUseAddress(addr) == true) {
        ifIdx = 0;
        if (umatter_net_isipv6address(addr) == TRUE)
          ifIdx = umatter_net_getipv6scopeid(addr);
        netIf = umatter_net_interface_new();
        umatter_net_interface_setaddress(netIf, addr);
        if (ai->PhysicalAddressLength == umatter_NET_MACADDR_SIZE)
          umatter_net_interface_setmacaddress(netIf, ai->PhysicalAddress);
        umatter_net_interface_setindex(netIf, ifIdx);
        umatter_net_interfacelist_add(netIfList, netIf);
        //}
      }
      else {
        int err = WSAGetLastError();
      }
      uai = uai->Next;
    }
    ai = ai->Next;
  }
  LocalFree(pAdapterAddresses);

#endif

  return umatter_net_interfacelist_size(netIfList);
}

#else

/****************************************
 * umatter_net_gethostinterfaces (UNIX)
 ****************************************/

#if defined(HAVE_IFADDRS_H)

size_t umatter_net_gethostinterfaces(uMatterNetworkInterfaceList* netIfList)
{
  uMatterNetworkInterface* netIf;
  struct ifaddrs* ifaddr;
  char addr[NI_MAXHOST + 1];
  char netmask[NI_MAXHOST + 1];
  char* ifname;
  struct ifaddrs* i;
#if defined(HAVE_SOCKADDR_DL)
  struct sockaddr_dl* dladdr;
#elif defined(HAVE_SIOCGIFHWADDR)
  int sock;
  struct ifreq ifr;
#endif

  umatter_net_interfacelist_clear(netIfList);

  if (getifaddrs(&ifaddr) != 0)
    return 0;

  for (i = ifaddr; i != NULL; i = i->ifa_next) {
    // Thanks for Ricardo Rivldo (04/10/12)
    //  - for some reason, vmware and virtualbox \"virtual\" interfaces does not
    //  return ifa_addr
    if (i->ifa_addr == NULL || i->ifa_netmask == NULL)
      continue;

    // Thanks for Tobias.Gansen (01/15/06)
    if (i->ifa_addr->sa_family != AF_INET)
      continue;
    if (!(i->ifa_flags & IFF_UP))
      continue;
    if (i->ifa_flags & IFF_LOOPBACK)
      continue;

    if (getnameinfo(i->ifa_addr, sizeof(struct sockaddr), addr, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0)
      continue;

    if (getnameinfo(i->ifa_netmask, sizeof(struct sockaddr), netmask, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0)
      continue;

    ifname = i->ifa_name;
    netIf = umatter_net_interface_new();
    umatter_net_interface_setname(netIf, ifname);
    umatter_net_interface_setaddress(netIf, addr);
    umatter_net_interface_setnetmask(netIf, netmask);
#if defined(HAVE_SOCKADDR_DL)
    dladdr = (struct sockaddr_dl*)(i->ifa_addr);
    umatter_net_interface_setmacaddress(netIf, LLADDR(dladdr));
#elif defined(HAVE_SIOCGIFHWADDR)
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
    ifr.ifr_addr.sa_family = AF_INET;
    ioctl(sock, SIOCGIFHWADDR, &ifr);
    umatter_net_interface_setmacaddress(netIf, ifr.ifr_hwaddr.sa_data);
    close(sock);
#endif
    umatter_net_interfacelist_add(netIfList, netIf);
  }
  freeifaddrs(ifaddr);

  return umatter_net_interfacelist_size(netIfList);
}

#else

static const char* path_proc_net_dev = "/proc/net/dev";

size_t umatter_net_gethostinterfaces(uMatterNetworkInterfaceList* net_if_list)
{
  uMatterNetworkInterface* net_if;
  FILE* fd;
  int s;
  char buffer[256 + 1];
  char ifaddr[20 + 1];
  char* ifname;
  char* sep;

  umatter_net_interfacelist_clear(net_if_list);

  s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0)
    return 0;
  fd = fopen(path_proc_net_dev, "r");
  fgets(buffer, sizeof(buffer) - 1, fd);
  fgets(buffer, sizeof(buffer) - 1, fd);
  while (!feof(fd)) {
    ifname = buffer;
    sep;
    if (fgets(buffer, sizeof(buffer) - 1, fd) == NULL)
      break;
    sep = strrchr(buffer, ':');
    if (sep)
      *sep = 0;
    while (*ifname == ' ')
      ifname++;
    struct ifreq req;
    strcpy(req.ifr_name, ifname);
    if (ioctl(s, SIOCGIFFLAGS, &req) < 0)
      continue;
    if (!(req.ifr_flags & IFF_UP))
      continue;
    if (req.ifr_flags & IFF_LOOPBACK)
      continue;
    if (ioctl(s, SIOCGIFADDR, &req) < 0)
      continue;
    strncpy(ifaddr, inet_ntoa(((struct sockaddr_in*)&req.ifr_addr)->sin_addr), sizeof(ifaddr) - 1);
    net_if = umatter_net_interface_new();
    umatter_net_interface_setname(net_if, ifname);
    umatter_net_interface_setaddress(net_if, ifaddr);
    umatter_net_interfacelist_add(net_if_list, net_if);
  }
  fclose(fd);
  close(s);

  return umatter_net_interfacelist_size(net_if_list);
}

#endif

#endif

/****************************************
 * umatter_net_selectaddr
 ****************************************/

#if !defined(HAVE_IFADDRS_H) || defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)

char* umatter_net_selectaddr(struct sockaddr* remoteaddr)
{
  uMatterNetworkInterfaceList* net_if_list;
  uMatterNetworkInterface* net_if;
  uMatterNetworkInterface* select_net_if;
  char* select_net_if_addr;
  u_long laddr, lmask, raddr;
  struct addrinfo hints;
  struct addrinfo* net_if_addr_info;
  struct addrinfo* net_mask_addr_info;

  net_if_list = umatter_net_interfacelist_new();
  if (!net_if_list)
    return umatter_strdup("127.0.0.1");

  if (umatter_net_gethostinterfaces(net_if_list) <= 0) {
    umatter_net_interfacelist_delete(net_if_list);
    return umatter_strdup("127.0.0.1");
  }

  raddr = ntohl(((struct sockaddr_in*)remoteaddr)->sin_addr.s_addr);

  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;

  select_net_if = NULL;
  if (1 <= umatter_net_gethostinterfaces(net_if_list)) {
    for (net_if = umatter_net_interfacelist_gets(net_if_list); net_if;
        net_if = umatter_net_interface_next(net_if)) {
      if (getaddrinfo(umatter_net_interface_getaddress(net_if), NULL, &hints, &net_if_addr_info) != 0)
        continue;
      if (getaddrinfo(umatter_net_interface_getnetmask(net_if), NULL, &hints, &net_mask_addr_info) != 0) {
        freeaddrinfo(net_if_addr_info);
        continue;
      }
      laddr = ntohl(
          ((struct sockaddr_in*)net_if_addr_info->ai_addr)->sin_addr.s_addr);
      lmask = ntohl(
          ((struct sockaddr_in*)net_mask_addr_info->ai_addr)->sin_addr.s_addr);
      if ((laddr & lmask) == (raddr & lmask))
        select_net_if = net_if;
      freeaddrinfo(net_if_addr_info);
      freeaddrinfo(net_mask_addr_info);
      if (select_net_if)
        break;
    }
  }

  if (!select_net_if)
    select_net_if = umatter_net_interfacelist_gets(net_if_list);

  select_net_if_addr = umatter_strdup(umatter_net_interface_getaddress(select_net_if));

  umatter_net_interfacelist_delete(net_if_list);

  return select_net_if_addr;
}
#else

char* umatter_net_selectaddr(struct sockaddr* remoteaddr)
{
  struct ifaddrs *ifaddrs, *ifaddr;
  uint32_t laddr, lmask, raddr;
  char *address_candidate = NULL, *auto_ip_address_candidate = NULL;

  raddr = ntohl(((struct sockaddr_in*)remoteaddr)->sin_addr.s_addr);

  if (0 != getifaddrs(&ifaddrs)) {
    return NULL;
  }

  for (ifaddr = ifaddrs; NULL != ifaddr; ifaddr = ifaddr->ifa_next) {
    if (ifaddr->ifa_addr == NULL)
      continue;
    if (!(ifaddr->ifa_flags & IFF_UP))
      continue;
    if (ifaddr->ifa_flags & IFF_LOOPBACK)
      continue;
    if (ifaddr->ifa_flags & IFF_POINTOPOINT)
      continue;

    laddr = ntohl(((struct sockaddr_in*)ifaddr->ifa_addr)->sin_addr.s_addr);
    if (NULL != (struct sockaddr_in*)ifaddr->ifa_netmask)
      lmask = ntohl(((struct sockaddr_in*)ifaddr->ifa_netmask)->sin_addr.s_addr);
    else {
      continue;
    }

    /* Checking if we have an exact subnet match */
    if ((laddr & lmask) == (raddr & lmask)) {
      if (NULL != address_candidate)
        free(address_candidate);
      address_candidate = umatter_strdup(inet_ntoa(
          (struct in_addr)((struct sockaddr_in*)ifaddr->ifa_addr)->sin_addr));
      break;
    }

    /* Checking if we have and auto ip address */
    if ((laddr & lmask) == UMATTER_NET_SOCKET_AUTO_IP_NET) {
      if (NULL != auto_ip_address_candidate)
        free(auto_ip_address_candidate);
      auto_ip_address_candidate = umatter_strdup(inet_ntoa(
          (struct in_addr)((struct sockaddr_in*)ifaddr->ifa_addr)->sin_addr));
    }
    /* Good. We have others than auto ips present. */
    else {
      if (NULL != address_candidate)
        free(address_candidate);
      address_candidate = umatter_strdup(inet_ntoa(
          (struct in_addr)((struct sockaddr_in*)ifaddr->ifa_addr)->sin_addr));
    }
  }

  freeifaddrs(ifaddrs);

  if (NULL != address_candidate) {
    if (NULL != auto_ip_address_candidate)
      free(auto_ip_address_candidate);
    return address_candidate;
  }

  if (NULL != auto_ip_address_candidate) {
    if (NULL != address_candidate)
      free(address_candidate);
    return auto_ip_address_candidate;
  }

  return umatter_strdup("127.0.0.1");
}

#endif
