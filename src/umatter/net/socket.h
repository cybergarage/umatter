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

#ifndef _UMATTER_NET_CSOCKET_H_
#define _UMATTER_NET_CSOCKET_H_

#if defined(UMATTER_USE_OPENSSL)
#include <openssl/ssl.h>
#endif

#include <umatter/net/socket_opt.h>
#include <umatter/typedef.h>
#include <umatter/util/strings.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

#define UMATTER_NET_SOCKET_NONE 0

#define UMATTER_NET_SOCKET_STREAM 0x01
#define UMATTER_NET_SOCKET_DGRAM 0x02

#define UMATTER_NET_SOCKET_CLIENT 1
#define UMATTER_NET_SOCKET_SERVER 2

#define UMATTER_NET_SOCKET_MAXHOST 32
#define UMATTER_NET_SOCKET_MAXSERV 32

#if !defined(WIN32)
typedef int SOCKET;
#endif

#define UMATTER_SOCKET_LF '\n'

#define UMATTER_NET_SOCKET_DGRAM_RECV_BUFSIZE 512
#define UMATTER_NET_SOCKET_DGRAM_ANCILLARY_BUFSIZE 512
#define UMATTER_NET_SOCKET_MULTICAST_DEFAULT_TTL 4
#define UMATTER_NET_SOCKET_AUTO_IP_NET 0xa9fe0000
#define UMATTER_NET_SOCKET_AUTO_IP_MASK 0xffff0000

/****************************************
 * Data Type
 ****************************************/

#include <umatter/util/list.h>

typedef struct UEchoSocket {
  SOCKET id;
  int type;
  int direction;
  uMatterString* ipaddr;
  int port;
#if defined(UMATTER_USE_OPENSSL)
  SSL_CTX* ctx;
  SSL* ssl;
#endif
} uMatterSocket;

typedef struct UEchoDatagramPacket {
  byte* data;
  size_t data_len;

  uMatterString* local_address;
  int local_port;

  uMatterString* remote_address;
  int remote_port;
} uMatterDatagramPacket;

/****************************************
 * Function (Socket)
 ****************************************/

void umatter_socket_startup(void);
void umatter_socket_cleanup(void);

uMatterSocket* umatter_socket_new(int type);
#define umatter_socket_stream_new() \
  umatter_socket_new(UMATTER_NET_SOCKET_STREAM)
#define umatter_socket_dgram_new() umatter_socket_new(UMATTER_NET_SOCKET_DGRAM)
bool umatter_socket_delete(uMatterSocket* socket);

void umatter_socket_setid(uMatterSocket* socket, SOCKET value);
#define umatter_socket_getid(socket) (socket->id)

#define umatter_socket_settype(socket, value) (socket->type = value)
#define umatter_socket_gettype(socket) (socket->type)
#define umatter_socket_issocketstream(socket) \
  ((socket->type & UMATTER_NET_SOCKET_STREAM) ? true : false)
#define umatter_socket_isdatagramstream(socket) \
  ((socket->type & UMATTER_NET_SOCKET_DGRAM) ? true : false)

#define umatter_socket_setdirection(socket, value) (socket->direction = value)
#define umatter_socket_getdirection(socket) (socket->direction)
#define umatter_socket_isclient(socket) \
  ((socket->direction == UMATTER_NET_SOCKET_CLIENT) ? true : false)
#define umatter_socket_isserver(socket) \
  ((socket->direction == UMATTER_NET_SOCKET_SERVER) ? true : false)

#define umatter_socket_setaddress(socket, value) \
  umatter_string_setvalue(socket->ipaddr, value)
#define umatter_socket_getaddress(socket) \
  umatter_string_getvalue(socket->ipaddr)
bool umatter_socket_isboundaddress(uMatterSocket* socket, const char* addr);

#define umatter_socket_setport(socket, value) (socket->port = value)
#define umatter_socket_getport(socket) (socket->port)

bool umatter_socket_isbound(uMatterSocket* socket);
bool umatter_socket_close(uMatterSocket* socket);

bool umatter_socket_listen(uMatterSocket* socket);

bool umatter_socket_bind(uMatterSocket* sock, int bind_port, const char* bind_addr, uMatterSocketOption* opt);
bool umatter_socket_accept(uMatterSocket* sock, uMatterSocket* client_sock);
bool umatter_socket_connect(uMatterSocket* sock, const char* addr, int port);
ssize_t umatter_socket_read(uMatterSocket* sock, char* buffer, size_t buffer_len);
size_t umatter_socket_write(uMatterSocket* sock, const char* buffer, size_t buffer_len);
ssize_t umatter_socket_readline(uMatterSocket* sock, char* buffer, size_t buffer_len);
size_t umatter_socket_skip(uMatterSocket* sock, size_t skip_len);

size_t umatter_socket_sendto(uMatterSocket* sock, const char* addr, int port, const byte* data, size_t datae_len);
ssize_t umatter_socket_recv(uMatterSocket* sock, uMatterDatagramPacket* dgm_pkt);

/****************************************
 * Function (Multicast)
 ****************************************/

bool umatter_socket_joingroup(uMatterSocket* sock, const char* mcast_addr, const char* if_addr);

/****************************************
 * Function (Option)
 ****************************************/

bool umatter_socket_setreuseaddress(uMatterSocket* socket, bool flag);
bool umatter_socket_setmulticastloop(uMatterSocket* sock, bool flag);
bool umatter_socket_setmulticastttl(uMatterSocket* sock, int ttl);
bool umatter_socket_settimeout(uMatterSocket* sock, int sec);

/****************************************
 * Function (DatagramPacket)
 ****************************************/

uMatterDatagramPacket* umatter_socket_datagram_packet_new(void);
void umatter_socket_datagram_packet_delete(uMatterDatagramPacket* dgm_pkt);
bool umatter_socket_datagram_packet_setdata(uMatterDatagramPacket* dgm_pkt,
    const byte* data,
    size_t data_len);
bool umatter_socket_datagram_packet_clear(uMatterDatagramPacket* dgm_pkt);

#define umatter_socket_datagram_packet_getdata(dgmPkt) (dgmPkt->data)
#define umatter_socket_datagram_packet_getlength(dgmPkt) (dgmPkt->data_len)

#define umatter_socket_datagram_packet_setlocaladdress(dgmPkt, addr) \
  umatter_string_setvalue(dgmPkt->local_address, addr)
#define umatter_socket_datagram_packet_getlocaladdress(dgmPkt) \
  umatter_string_getvalue(dgmPkt->local_address)
#define umatter_socket_datagram_packet_setlocalport(dgmPkt, port) \
  (dgmPkt->local_port = port)
#define umatter_socket_datagram_packet_getlocalport(dgmPkt) (dgmPkt->local_port)
#define umatter_socket_datagram_packet_setremoteaddress(dgmPkt, addr) \
  umatter_string_setvalue(dgmPkt->remote_address, addr)
#define umatter_socket_datagram_packet_getremoteaddress(dgmPkt) \
  umatter_string_getvalue(dgmPkt->remote_address)
#define umatter_socket_datagram_packet_setremoteport(dgmPkt, port) \
  (dgmPkt->remote_port = port)
#define umatter_socket_datagram_packet_getremoteport(dgmPkt) \
  (dgmPkt->remote_port)

bool umatter_socket_datagram_packet_copy(uMatterDatagramPacket* dst_dgm_pkt,
    uMatterDatagramPacket* src_dgm_pkt);

/****************************************
 * Function (SSLSocket)
 ****************************************/

#if defined(UMATTER_USE_OPENSSL)
#define UMATTER_NET_SOCKET_SSL 0x0100
#define umatter_socket_ssl_new() \
  umatter_socket_new(UMATTER_NET_SOCKET_STREAM | UMATTER_NET_SOCKET_SSL)
#define umatter_socket_isssl(socket) \
  ((socket->type & UMATTER_NET_SOCKET_SSL) ? true : false)
#endif

#ifdef __cplusplus
}
#endif

#endif
