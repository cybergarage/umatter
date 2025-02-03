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
#include <umatter/util/logs.h>
#include <umatter/util/timer.h>

#include <string.h>

#if defined(WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#if defined(UMATTER_USE_OPENSSL)
#pragma comment(lib, "libeay32MD.lib")
#pragma comment(lib, "ssleay32MD.lib")
#endif
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

/****************************************
 * static variable
 ****************************************/

static int socket_cnt = 0;

/****************************************
 * prototype
 ****************************************/

bool umatter_socket_tosockaddrin(const char* addr, int port, struct sockaddr_in* sockaddr, bool is_bind_addr);
bool umatter_socket_tosockaddrinfo(int sock_type, const char* addr, int port, struct addrinfo** addr_info, bool is_bind_addr);

#define umatter_socket_getrawtype(socket)                                    \
  (((socket->type & UMATTER_NET_SOCKET_STREAM) == UMATTER_NET_SOCKET_STREAM) \
          ? SOCK_STREAM                                                      \
          : SOCK_DGRAM)

/****************************************
 *
 * Socket
 *
 ****************************************/

/****************************************
 * umatter_socket_startup
 ****************************************/

void umatter_socket_startup(void)
{
  if (socket_cnt == 0) {
#if defined(WIN32)
    WSADATA wsaData;
    int err;

    err = WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

#if !defined(WIN32)
    // Thanks for Brent Hills (10/26/04)
    signal(SIGPIPE, SIG_IGN);
#endif

#if defined(UMATTER_USE_OPENSSL)
    SSL_library_init();
#endif
  }
  socket_cnt++;
}

/****************************************
 * umatter_socket_cleanup
 ****************************************/

void umatter_socket_cleanup(void)
{
  socket_cnt--;
  if (socket_cnt <= 0) {
#if defined(WIN32)
    WSACleanup();
#endif

#if !defined(WIN32)
    // Thanks for Brent Hills (10/26/04)
    signal(SIGPIPE, SIG_DFL);
#endif
  }
}

/****************************************
 * umatter_socket_new
 ****************************************/

uMatterSocket* umatter_socket_new(int type)
{
  uMatterSocket* sock;

  umatter_socket_startup();

  sock = (uMatterSocket*)malloc(sizeof(uMatterSocket));
  if (!sock)
    return NULL;

#if defined(WIN32)
  sock->id = INVALID_SOCKET;
#else
  sock->id = -1;
#endif

  umatter_socket_settype(sock, type);
  umatter_socket_setdirection(sock, UMATTER_NET_SOCKET_NONE);

  sock->ipaddr = umatter_string_new();

  umatter_socket_setaddress(sock, "");
  umatter_socket_setport(sock, -1);

#if defined(UMATTER_USE_OPENSSL)
  sock->ctx = NULL;
  sock->ssl = NULL;
#endif

  return sock;
}

/****************************************
 * umatter_socket_delete
 ****************************************/

bool umatter_socket_delete(uMatterSocket* sock)
{
  if (!sock)
    return true;

  umatter_socket_close(sock);
  umatter_string_delete(sock->ipaddr);
  free(sock);

  umatter_socket_cleanup();

  return true;
}

/****************************************
 * umatter_socket_isbound
 ****************************************/

bool umatter_socket_isbound(uMatterSocket* sock)
{
  if (!sock)
    return false;

#if defined(WIN32)
  return (sock->id != INVALID_SOCKET) ? true : false;
#else
  return (0 < sock->id) ? true : false;
#endif
}

/****************************************
 * umatter_socket_isboundaddress
 ****************************************/

bool umatter_socket_isboundaddress(uMatterSocket* sock, const char* addr)
{
  if (!sock)
    return false;

  return umatter_streq(umatter_socket_getaddress(sock), addr);
}

/****************************************
 * umatter_socket_setid
 ****************************************/

void umatter_socket_setid(uMatterSocket* sock, SOCKET value)
{
#if defined(WIN32) || defined(HAVE_IP_PKTINFO) || (!defined(WIN32) && defined(HAVE_SO_NOSIGPIPE))
  int on = 1;
#endif

  if (!sock)
    return;

  sock->id = value;

#if defined(WIN32) || defined(HAVE_IP_PKTINFO)
  if (UMATTER_NET_SOCKET_DGRAM == umatter_socket_gettype(socket))
    setsockopt(sock->id, IPPROTO_IP, IP_PKTINFO, &on, sizeof(on));
#endif

#if !defined(WIN32) && defined(HAVE_SO_NOSIGPIPE)
  setsockopt(sock->id, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on));
#endif
}

/****************************************
 * umatter_socket_close
 ****************************************/

bool umatter_socket_close(uMatterSocket* sock)
{
  if (!sock)
    return true;

  if (umatter_socket_isbound(sock) == false)
    return true;

#if defined(UMATTER_USE_OPENSSL)
  if (umatter_socket_isssl(sock) == true) {
    if (sock->ctx) {
      SSL_shutdown(sock->ssl);
      SSL_free(sock->ssl);
      sock->ssl = NULL;
    }
    if (sock->ctx) {
      SSL_CTX_free(sock->ctx);
      sock->ctx = NULL;
    }
  }
#endif

#if defined(WIN32)
  WSAAsyncSelect(sock->id, NULL, 0, FD_CLOSE);
  shutdown(sock->id, SD_BOTH);
  closesocket(sock->id);

  sock->id = INVALID_SOCKET;
#else
  int flag = fcntl(sock->id, F_GETFL, 0);
  if (0 <= flag)
    fcntl(sock->id, F_SETFL, flag | O_NONBLOCK);
  shutdown(sock->id, 2);
  close(sock->id);

  sock->id = -1;
#endif

  umatter_socket_setaddress(sock, "");
  umatter_socket_setport(sock, -1);

  return true;
}

/****************************************
 * umatter_socket_listen
 ****************************************/

bool umatter_socket_listen(uMatterSocket* sock)
{
  if (!sock)
    return false;

  int ret = listen(sock->id, SOMAXCONN);
  return (ret == 0) ? true : false;
}

/****************************************
 * umatter_socket_bind
 ****************************************/

bool umatter_socket_bind(uMatterSocket* sock, int bind_port, const char* bind_addr, uMatterSocketOption* opt)
{
  struct addrinfo* addr_info;
  int ret;

  if (!sock)
    return false;

  if (bind_port <= 0 /* || bindAddr == NULL*/)
    return false;

  if (umatter_socket_tosockaddrinfo(
          umatter_socket_getrawtype(sock), bind_addr, bind_port, &addr_info, umatter_socket_option_isbindinterface(opt))
      == false)
    return false;
  umatter_socket_setid(sock,
      socket(addr_info->ai_family, addr_info->ai_socktype, 0));
  if (sock->id == -1) {
    umatter_socket_close(sock);
    return false;
  }

  if (umatter_socket_option_isreuseaddress(opt)) {
    if (umatter_socket_setreuseaddress(sock, true) == false) {
      umatter_socket_close(sock);
      return false;
    }
  }

  if (umatter_socket_option_ismulticastloop(opt)) {
    if (umatter_socket_setmulticastloop(sock, true) == false) {
      umatter_socket_close(sock);
      return false;
    }
  }

  ret = bind(sock->id, addr_info->ai_addr, addr_info->ai_addrlen);
  freeaddrinfo(addr_info);

  if (ret != 0)
    return false;

  umatter_socket_setdirection(sock, UMATTER_NET_SOCKET_SERVER);
  umatter_socket_setaddress(sock, bind_addr);
  umatter_socket_setport(sock, bind_port);

  return true;
}

/****************************************
 * umatter_socket_accept
 ****************************************/

bool umatter_socket_accept(uMatterSocket* server_sock, uMatterSocket* client_sock)
{
  struct sockaddr_in sockaddr;
  socklen_t socklen;
  char local_addr[UMATTER_NET_SOCKET_MAXHOST];
  char local_port[UMATTER_NET_SOCKET_MAXSERV];
  struct sockaddr_storage sock_client_addr;
  socklen_t n_length = sizeof(sock_client_addr);

  umatter_socket_setid(
      client_sock,
      accept(server_sock->id, (struct sockaddr*)&sock_client_addr, &n_length));

#if defined(WIN32)
  if (clientSock->id == INVALID_SOCKET)
    return false;
#else
  if (client_sock->id < 0)
    return false;
#endif

  umatter_socket_setaddress(client_sock,
      umatter_socket_getaddress(server_sock));
  umatter_socket_setport(client_sock, umatter_socket_getport(server_sock));
  socklen = sizeof(struct sockaddr_in);

  if (getsockname(client_sock->id, (struct sockaddr*)&sockaddr, &socklen) == 0 && getnameinfo((struct sockaddr*)&sockaddr, socklen, local_addr, sizeof(local_addr), local_port, sizeof(local_port), NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
    /* Set address for the sockaddr to real addr */
    umatter_socket_setaddress(client_sock, local_addr);
  }

  return true;
}

/****************************************
 * umatter_socket_connect
 ****************************************/

bool umatter_socket_connect(uMatterSocket* sock, const char* addr, int port)
{
  struct addrinfo* toaddr_info;
  int ret;

  if (!sock)
    return false;

  if (umatter_socket_tosockaddrinfo(umatter_socket_getrawtype(sock), addr, port, &toaddr_info, true) == false)
    return false;

  if (umatter_socket_isbound(sock) == false) {
    umatter_socket_setid(
        sock, socket(toaddr_info->ai_family, toaddr_info->ai_socktype, 0));
  }

  ret = connect(sock->id, toaddr_info->ai_addr, toaddr_info->ai_addrlen);
  freeaddrinfo(toaddr_info);

  umatter_socket_setdirection(sock, UMATTER_NET_SOCKET_CLIENT);

#if defined(UMATTER_USE_OPENSSL)
  if (umatter_socket_isssl(sock) == true) {
    sock->ctx = SSL_CTX_new(SSLv23_client_method());
    sock->ssl = SSL_new(sock->ctx);
    if (SSL_set_fd(sock->ssl, umatter_socket_getid(sock)) == 0) {
      umatter_socket_close(sock);
      return false;
    }
    if (SSL_connect(sock->ssl) < 1) {
      umatter_socket_close(sock);
      return false;
    }
  }
#endif

  return (ret == 0) ? true : false;
}

/****************************************
 * umatter_socket_read
 ****************************************/

ssize_t umatter_socket_read(uMatterSocket* sock, char* buffer, size_t buffer_len)
{
  ssize_t recv_len;

  if (!sock)
    return -1;

#if defined(UMATTER_USE_OPENSSL)
  if (umatter_socket_isssl(sock) == false) {
#endif

    recv_len = recv(sock->id, buffer, buffer_len, 0);

#if defined(UMATTER_USE_OPENSSL)
  }
  else {
    recvLen = SSL_read(sock->ssl, buffer, bufferLen);
  }
#endif

  return recv_len;
}

/****************************************
 * umatter_socket_write
 ****************************************/

#define UMATTER_NET_SOCKET_SEND_RETRY_CNT 10
#define UMATTER_NET_SOCKET_SEND_RETRY_WAIT_MSEC 20

size_t umatter_socket_write(uMatterSocket* sock, const char* cmd, size_t cmd_len)
{
  ssize_t n_sent;
  size_t n_total_sent = 0;
  size_t cmd_pos = 0;
  int retry_cnt = 0;

  if (!sock)
    return 0;

  if (cmd_len <= 0)
    return 0;

  do {
#if defined(UMATTER_USE_OPENSSL)
    if (umatter_socket_isssl(sock) == false) {
#endif

      n_sent = send(sock->id, cmd + cmd_pos, cmd_len, 0);

#if defined(UMATTER_USE_OPENSSL)
    }
    else {
      nSent = SSL_write(sock->ssl, cmd + cmdPos, cmdLen);
    }
#endif

    if (n_sent <= 0) {
      retry_cnt++;
      if (UMATTER_NET_SOCKET_SEND_RETRY_CNT < retry_cnt) {
        n_total_sent = 0;
        break;
      }

      umatter_wait(UMATTER_NET_SOCKET_SEND_RETRY_WAIT_MSEC);
    }
    else {
      n_total_sent += n_sent;
      cmd_pos += n_sent;
      cmd_len -= n_sent;
      retry_cnt = 0;
    }

  } while (0 < cmd_len);

  return n_total_sent;
}
/****************************************
 * umatter_socket_readline
 ****************************************/

ssize_t umatter_socket_readline(uMatterSocket* sock, char* buffer, size_t buffer_len)
{
  ssize_t read_cnt;
  ssize_t read_len;
  char c;

  if (!sock)
    return -1;

  read_cnt = 0;
  while (read_cnt < (buffer_len - 1)) {
    read_len = umatter_socket_read(sock, &buffer[read_cnt], sizeof(char));
    if (read_len <= 0)
      return -1;
    read_cnt++;
    if (buffer[read_cnt - 1] == UMATTER_SOCKET_LF)
      break;
  }
  buffer[read_cnt] = '\0';
  if (buffer[read_cnt - 1] != UMATTER_SOCKET_LF) {
    do {
      read_len = umatter_socket_read(sock, &c, sizeof(char));
      if (read_len <= 0)
        break;
    } while (c != UMATTER_SOCKET_LF);
  }

  return read_cnt;
}

/****************************************
 * umatter_socket_skip
 ****************************************/

size_t umatter_socket_skip(uMatterSocket* sock, size_t skip_len)
{
  ssize_t read_cnt;
  ssize_t read_len;
  char c;

  if (!sock)
    return 0;

  read_cnt = 0;
  while (read_cnt < skip_len) {
    read_len = umatter_socket_read(sock, &c, sizeof(char));
    if (read_len <= 0)
      break;
    read_cnt++;
  }

  return read_cnt;
}

/****************************************
 * umatter_socket_sendto
 ****************************************/

size_t umatter_socket_sendto(uMatterSocket* sock, const char* addr, int port, const byte* data, size_t data_len)
{
  struct addrinfo* addr_info;
  ssize_t sent_len;
  bool is_bound_flag;

  if (!sock)
    return 0;

  if (!data && (data_len <= 0))
    return 0;

  is_bound_flag = umatter_socket_isbound(sock);
  sent_len = -1;

  if (umatter_socket_tosockaddrinfo(umatter_socket_getrawtype(sock), addr, port, &addr_info, true) == false)
    return -1;
  if (is_bound_flag == false)
    umatter_socket_setid(
        sock, socket(addr_info->ai_family, addr_info->ai_socktype, 0));

  /* Setting multicast time to live in any case to default */
  umatter_socket_setmulticastttl(sock,
      UMATTER_NET_SOCKET_MULTICAST_DEFAULT_TTL);

  if (0 <= sock->id)
    sent_len = sendto(sock->id, data, data_len, 0, addr_info->ai_addr, addr_info->ai_addrlen);

  umatter_net_socket_debug(UMATTER_LOG_NET_PREFIX_SEND,
      umatter_socket_getaddress(sock),
      addr,
      data,
      data_len);

  freeaddrinfo(addr_info);

  if (is_bound_flag == false)
    umatter_socket_close(sock);

  return sent_len;
}

/****************************************
 * umatter_socket_recv
 ****************************************/

ssize_t umatter_socket_recv(uMatterSocket* sock, uMatterDatagramPacket* dgm_pkt)
{
  ssize_t recv_len = 0;
  byte recv_buf[UMATTER_NET_SOCKET_DGRAM_RECV_BUFSIZE + 1];
  char remote_addr[UMATTER_NET_SOCKET_MAXHOST];
  char remote_port[UMATTER_NET_SOCKET_MAXSERV];
  char* local_addr;
  struct sockaddr_storage from;
  socklen_t from_len;

  if (!sock)
    return -1;

  from_len = sizeof(from);
  recv_len = recvfrom(sock->id, recv_buf, sizeof(recv_buf) - 1, 0, (struct sockaddr*)&from, &from_len);

  if (recv_len <= 0)
    return recv_len;

  umatter_socket_datagram_packet_setdata(dgm_pkt, recv_buf, recv_len);

  umatter_socket_datagram_packet_setlocalport(dgm_pkt,
      umatter_socket_getport(sock));
  umatter_socket_datagram_packet_setremoteaddress(dgm_pkt, "");
  umatter_socket_datagram_packet_setremoteport(dgm_pkt, 0);

  if (getnameinfo((struct sockaddr*)&from, from_len, remote_addr, sizeof(remote_addr), remote_port, sizeof(remote_port), NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
    umatter_socket_datagram_packet_setremoteaddress(dgm_pkt, remote_addr);
    umatter_socket_datagram_packet_setremoteport(dgm_pkt,
        umatter_str2int(remote_port));
  }

  local_addr = umatter_net_selectaddr((struct sockaddr*)&from);
  umatter_socket_datagram_packet_setlocaladdress(dgm_pkt, local_addr);

  umatter_net_socket_debug(UMATTER_LOG_NET_PREFIX_RECV, remote_addr, local_addr, recv_buf, recv_len);

  free(local_addr);

  return recv_len;
}

/****************************************
 * umatter_socket_setreuseaddress
 ****************************************/

bool umatter_socket_setreuseaddress(uMatterSocket* sock, bool flag)
{
  int sock_opt_ret;
#if defined(WIN32)
  bool optval;
#else
  int optval;
#endif

  if (!sock)
    return false;

#if defined(WIN32)
  optval = (flag == true) ? 1 : 0;
  sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
#else
  optval = (flag == true) ? 1 : 0;
  sock_opt_ret = setsockopt(sock->id, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
#if defined(USE_SO_REUSEPORT) || defined(TARGET_OS_MAC) || defined(TARGET_OS_IPHONE)
  if (sock_opt_ret == 0) {
    sock_opt_ret = setsockopt(sock->id, SOL_SOCKET, SO_REUSEPORT, (const char*)&optval, sizeof(optval));
  }
#endif
#endif

  return (sock_opt_ret == 0) ? true : false;
}

/****************************************
 * umatter_socket_setmulticastloop
 ****************************************/

bool umatter_socket_setmulticastloop(uMatterSocket* sock, bool flag)
{
  int sock_opt_ret;
#if defined(WIN32)
  bool optval;
#else
  int optval;
#endif

  if (!sock)
    return false;

#if defined(WIN32)
  optval = (flag == true) ? 1 : 0;
  sockOptRet = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&optval, sizeof(optval));
#else
  optval = (flag == true) ? 1 : 0;
  sock_opt_ret = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&optval, sizeof(optval));
#endif

  return (sock_opt_ret == 0) ? true : false;
}

/****************************************
 * umatter_socket_setmulticastttl
 ****************************************/

bool umatter_socket_setmulticastttl(uMatterSocket* sock, int val)
{
  int sock_opt_ret;
  int ttl;
  unsigned int len = 0;

  if (!sock)
    return false;

  ttl = val;
#if defined(WIN32)
  sockOptRet = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&ttl, sizeof(ttl));
#else
  sock_opt_ret = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, (const unsigned char*)&ttl, sizeof(ttl));
  if (sock_opt_ret == 0) {
    len = sizeof(ttl);
    getsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, (socklen_t*)&len);
  }
#endif

  return (sock_opt_ret == 0) ? true : false;
}

/****************************************
 * umatter_socket_settimeout
 ****************************************/

bool umatter_socket_settimeout(uMatterSocket* sock, int sec)
{
  int sock_opt_ret;

#if defined(WIN32)
  struct timeval timeout;
  timeout.tv_sec = sec;
  timeout.tv_usec = 0;

  if (!sock)
    return false;

  sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
  if (sockOptRet == 0)
    sockOptRet = setsockopt(sock->id, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
#else
  struct timeval timeout;
  timeout.tv_sec = (clock_t)sec;
  timeout.tv_usec = 0;

  if (!sock)
    return false;

  sock_opt_ret = setsockopt(sock->id, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
  if (sock_opt_ret == 0)
    sock_opt_ret = setsockopt(sock->id, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
#endif

  return (sock_opt_ret == 0) ? true : false;
}

/****************************************
 * umatter_socket_joingroup
 ****************************************/

bool umatter_socket_joingroup(uMatterSocket* sock, const char* mcast_addr, const char* if_addr)
{
  struct addrinfo hints;
  struct addrinfo *mcast_addr_info, *if_addr_info;

  /**** for IPv6 ****/
  struct ipv6_mreq ipv6mr;
  struct sockaddr_in6 toaddr6, ifaddr6;
  int scope_id;

  /**** for IPv4 ****/
  struct ip_mreq ipmr;
  struct sockaddr_in toaddr, ifaddr;

  bool join_success;
  int sock_opt_ret_code;

  if (!sock)
    return false;

  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;

  if (getaddrinfo(mcast_addr, NULL, &hints, &mcast_addr_info) != 0)
    return false;

  if (getaddrinfo(if_addr, NULL, &hints, &if_addr_info) != 0) {
    freeaddrinfo(mcast_addr_info);
    return false;
  }

  join_success = true;

  if (umatter_net_isipv6address(mcast_addr) == true) {
    memcpy(&toaddr6, mcast_addr_info->ai_addr, sizeof(struct sockaddr_in6));
    memcpy(&ifaddr6, if_addr_info->ai_addr, sizeof(struct sockaddr_in6));
    ipv6mr.ipv6mr_multiaddr = toaddr6.sin6_addr;
    scope_id = umatter_net_getipv6scopeid(if_addr);
    ipv6mr.ipv6mr_interface = scope_id /*if_nametoindex*/;

    sock_opt_ret_code = setsockopt(sock->id, IPPROTO_IPV6, IPV6_MULTICAST_IF, (char*)&scope_id, sizeof(scope_id));

    if (sock_opt_ret_code != 0)
      join_success = false;

    sock_opt_ret_code = setsockopt(sock->id, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char*)&ipv6mr, sizeof(ipv6mr));

    if (sock_opt_ret_code != 0)
      join_success = false;
  }
  else {
    memcpy(&toaddr, mcast_addr_info->ai_addr, sizeof(struct sockaddr_in));
    memcpy(&ifaddr, if_addr_info->ai_addr, sizeof(struct sockaddr_in));
    memcpy(&ipmr.imr_multiaddr.s_addr, &toaddr.sin_addr, sizeof(struct in_addr));
    memcpy(&ipmr.imr_interface.s_addr, &ifaddr.sin_addr, sizeof(struct in_addr));
    sock_opt_ret_code = setsockopt(sock->id, IPPROTO_IP, IP_MULTICAST_IF, (char*)&ipmr.imr_interface.s_addr, sizeof(struct in_addr));
    if (sock_opt_ret_code != 0)
      join_success = false;
    sock_opt_ret_code = setsockopt(sock->id, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&ipmr, sizeof(ipmr));

    if (sock_opt_ret_code != 0)
      join_success = false;
  }

  freeaddrinfo(mcast_addr_info);
  freeaddrinfo(if_addr_info);

  return join_success;
}

/****************************************
 * umatter_socket_tosockaddrin
 ****************************************/

bool umatter_socket_tosockaddrin(const char* addr, int port, struct sockaddr_in* sockaddr, bool is_bind_addr)
{
  umatter_socket_startup();

  memset(sockaddr, 0, sizeof(struct sockaddr_in));

  sockaddr->sin_family = AF_INET;
  sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
  sockaddr->sin_port = htons((unsigned short)port);

  if (is_bind_addr == true) {
    sockaddr->sin_addr.s_addr = inet_addr(addr);
    if (sockaddr->sin_addr.s_addr == INADDR_NONE) {
      struct hostent* hent = gethostbyname(addr);
      if (hent == NULL)
        return false;
      memcpy(&(sockaddr->sin_addr), hent->h_addr, hent->h_length);
    }
  }

  return true;
}

/****************************************
 * umatter_socket_tosockaddrinfo
 ****************************************/

bool umatter_socket_tosockaddrinfo(int sock_type, const char* addr, int port, struct addrinfo** addr_info, bool is_bind_addr)
{
  struct addrinfo hints;
  char port_str[32];
  int errorn;

  umatter_socket_startup();

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = sock_type;
  hints.ai_flags = /*AI_NUMERICHOST | */ AI_PASSIVE;
  sprintf(port_str, "%d", port);
  if ((errorn = getaddrinfo(addr, port_str, &hints, addr_info)) != 0)
    return false;
  if (is_bind_addr == true)
    return true;
  hints.ai_family = (*addr_info)->ai_family;
  freeaddrinfo(*addr_info);
  if ((errorn = getaddrinfo(NULL, port_str, &hints, addr_info)) != 0)
    return false;
  return true;
}
