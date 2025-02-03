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

#include <stdlib.h>

#include <umatter/util/bytes.h>

/****************************************
 * umatter_bytes_toint
 ****************************************/

int umatter_bytes_toint(byte* byte_data, size_t byte_size)
{
  int idx;
  int n;
  int val = 0;

  for (n = 0; n < (int)byte_size; n++) {
    idx = ((int)byte_size - 1) - n;
    val += byte_data[idx] << (n * 8);
  }

  return val;
}

/****************************************
 * umatter_bytes_fromint
 ****************************************/

byte* umatter_bytes_fromint(int val, size_t bytes_size)
{
  byte* bytes_data;
  int idx;
  int n;

  bytes_data = malloc(bytes_size);
  for (n = 0; n < (int)bytes_size; n++) {
    idx = ((int)bytes_size - 1) - n;
    bytes_data[idx] = ((val >> (n * 8)) & 0xFF);
  }

  return bytes_data;
}
