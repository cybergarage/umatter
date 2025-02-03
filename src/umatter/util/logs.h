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

#ifndef _UMATTER_LOG_FUNC_H_
#define _UMATTER_LOG_FUNC_H_

#include <umatter/net/socket.h>
#include <umatter/typedef.h>

#define UMATTER_LOG_NET_PREFIX_SEND "S"
#define UMATTER_LOG_NET_PREFIX_RECV "R"

void umatter_net_socket_error(const char* prefix, const char* from_addr, const char* to_addr, const byte* pkt_bytes, size_t pkt_len);
void umatter_net_socket_debug(const char* prefix, const char* from_addr, const char* to_addr, const byte* pkt_bytes, size_t pkt_len);

void umatter_net_datagram_packet_error(const char* prefix,
    uMatterDatagramPacket* dpkt);
void umatter_net_datagram_packet_debug(const char* prefix,
    uMatterDatagramPacket* dpkt);

#endif /* _UMATTER_LOG_FUNC_H_ */
