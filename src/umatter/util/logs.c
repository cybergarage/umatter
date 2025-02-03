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

#include <stdio.h>
#include <string.h>
#include <umatter/util/_log.h>
#include <umatter/util/logs.h>

/****************************************
 * umatter_net_socket_log
 ****************************************/

void umatter_net_socket_log(int severity, const char* prefix, const char* from_addr, const char* to_addr, const byte* msg_bytes, size_t msg_len)
{
  char buf[256];
  size_t offset;
  size_t n;

  if (msg_len <= 0)
    return;

  offset = 0;
  if (prefix && (0 < strlen(prefix))) {
    snprintf(buf, sizeof(buf), "%s ", prefix);
    offset = strlen(buf);
  }
  snprintf((buf + offset), (sizeof(buf) - offset), "%-15s -> %-15s ", from_addr, to_addr);
  offset = strlen(buf);
  for (n = 0; n < msg_len; n++) {
    snprintf((buf + offset), (sizeof(buf) - offset), "%02X", msg_bytes[n]);
    offset += 2;
  }

  umatter_log_debug("%s", buf);
}

/****************************************
 * umatter_net_socket_*
 ****************************************/

void umatter_net_socket_debug(const char* prefix, const char* from_addr, const char* to_addr, const byte* pkt_bytes, size_t pkt_len)
{
  return umatter_net_socket_log(UMATTER_LOG_DEBUG, prefix, from_addr, to_addr, pkt_bytes, pkt_len);
}

void umatter_net_socket_error(const char* prefix, const char* from_addr, const char* to_addr, const byte* pkt_bytes, size_t pkt_len)
{
  return umatter_net_socket_log(UMATTER_LOG_ERROR, prefix, from_addr, to_addr, pkt_bytes, pkt_len);
}

/****************************************
 * umatter_net_datagram_packet_*
 ****************************************/

void umatter_net_datagram_packet_error(const char* prefix,
    uMatterDatagramPacket* dpkt)
{
  if (!dpkt)
    return;

  return umatter_net_socket_error(
      prefix, umatter_socket_datagram_packet_getremoteaddress(dpkt), umatter_socket_datagram_packet_getlocaladdress(dpkt), umatter_socket_datagram_packet_getdata(dpkt), umatter_socket_datagram_packet_getlength(dpkt));
}

void umatter_net_datagram_packet_debug(const char* prefix,
    uMatterDatagramPacket* dpkt)
{
  if (!dpkt)
    return;

  return umatter_net_socket_debug(
      prefix, umatter_socket_datagram_packet_getremoteaddress(dpkt), umatter_socket_datagram_packet_getlocaladdress(dpkt), umatter_socket_datagram_packet_getdata(dpkt), umatter_socket_datagram_packet_getlength(dpkt));
}