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

#ifndef _UMATTER_UTIL_LIST_H_
#define _UMATTER_UTIL_LIST_H_

#include <umatter/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

typedef void (*UMATTER_LIST_DESTRUCTORFUNC)(void*);

#define UMATTER_LIST_STRUCT_MEMBERS \
  bool headFlag;                    \
  struct _uMatterList* prev;        \
  struct _uMatterList* next;

/****************************************
 * Data Type
 ****************************************/

typedef struct _uMatterList {
  UMATTER_LIST_STRUCT_MEMBERS
} uMatterList;

/****************************************
 * Functions
 ****************************************/

bool umatter_list_header_init(uMatterList* list);
bool umatter_list_node_init(uMatterList* list);

bool umatter_list_insert(uMatterList* prev_list, uMatterList* list);
bool umatter_list_add(uMatterList* head_list, uMatterList* list);
bool umatter_list_remove(uMatterList* list);

size_t umatter_list_size(uMatterList* head_list);

uMatterList* umatter_list_gets(uMatterList* head_list);
uMatterList* umatter_list_get(uMatterList* head_list, int index);

uMatterList* umatter_list_prev_circular(uMatterList* list);
uMatterList* umatter_list_prev(uMatterList* list);
uMatterList* umatter_list_next_circular(uMatterList* list);
uMatterList* umatter_list_next(uMatterList* list);

bool umatter_list_clear(uMatterList* head_list,
    UMATTER_LIST_DESTRUCTORFUNC dstructor_func);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
