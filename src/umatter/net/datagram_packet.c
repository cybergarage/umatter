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

#include <umatter/net/socket.h>

/****************************************
 * umatter_socket_datagram_packet_new
 ****************************************/

uMatterDatagramPacket* umatter_socket_datagram_packet_new(void)
{
  uMatterDatagramPacket* dgm_pkt;

  dgm_pkt = (uMatterDatagramPacket*)malloc(sizeof(uMatterDatagramPacket));

  if (!dgm_pkt)
    return NULL;

  dgm_pkt->data = NULL;
  dgm_pkt->data_len = 0;

  dgm_pkt->local_address = umatter_string_new();
  umatter_socket_datagram_packet_setlocalport(dgm_pkt, 0);

  dgm_pkt->remote_address = umatter_string_new();
  umatter_socket_datagram_packet_setremoteport(dgm_pkt, 0);

  return dgm_pkt;
}

/****************************************
 * umatter_socket_datagram_packet_delete
 ****************************************/

void umatter_socket_datagram_packet_delete(uMatterDatagramPacket* dgm_pkt)
{
  if (!dgm_pkt)
    return;

  umatter_socket_datagram_packet_clear(dgm_pkt);

  umatter_string_delete(dgm_pkt->local_address);
  umatter_string_delete(dgm_pkt->remote_address);

  free(dgm_pkt);
}

/****************************************
 * umatter_socket_datagram_packet_setdata
 ****************************************/

bool umatter_socket_datagram_packet_setdata(uMatterDatagramPacket* dgm_pkt,
    const byte* data,
    size_t data_len)
{
  if (!dgm_pkt)
    return false;

  umatter_socket_datagram_packet_clear(dgm_pkt);

  if (!data || (data_len <= 0))
    return true;

  dgm_pkt->data = malloc(data_len);
  if (!dgm_pkt->data)
    return false;

  memcpy(dgm_pkt->data, data, data_len);
  dgm_pkt->data_len = data_len;

  return true;
}

/****************************************
 * umatter_socket_datagram_packet_clear
 ****************************************/

bool umatter_socket_datagram_packet_clear(uMatterDatagramPacket* dgm_pkt)
{
  if (!dgm_pkt)
    return false;

  if (dgm_pkt->data) {
    free(dgm_pkt->data);
    dgm_pkt->data = NULL;
  }
  dgm_pkt->data_len = 0;

  return true;
}

/****************************************
 * umatter_socket_datagram_packet_copy
 ****************************************/

bool umatter_socket_datagram_packet_copy(uMatterDatagramPacket* dst_dgm_pkt,
    uMatterDatagramPacket* src_dgm_pkt)
{
  if (!dst_dgm_pkt || !src_dgm_pkt)
    return false;

  umatter_socket_datagram_packet_setdata(
      dst_dgm_pkt, umatter_socket_datagram_packet_getdata(src_dgm_pkt), umatter_socket_datagram_packet_getlength(src_dgm_pkt));
  umatter_socket_datagram_packet_setlocaladdress(
      dst_dgm_pkt, umatter_socket_datagram_packet_getlocaladdress(src_dgm_pkt));
  umatter_socket_datagram_packet_setlocalport(
      dst_dgm_pkt, umatter_socket_datagram_packet_getlocalport(src_dgm_pkt));
  umatter_socket_datagram_packet_setremoteaddress(
      dst_dgm_pkt,
      umatter_socket_datagram_packet_getremoteaddress(src_dgm_pkt));
  umatter_socket_datagram_packet_setremoteport(
      dst_dgm_pkt, umatter_socket_datagram_packet_getremoteport(src_dgm_pkt));

  return true;
}
