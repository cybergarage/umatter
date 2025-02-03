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

#include <umatter/util/list.h>

/****************************************
 * umatter_list_header_init
 ****************************************/

bool umatter_list_header_init(uMatterList* list)
{
  if (!list)
    return false;

  list->headFlag = true;
  list->prev = list->next = list;

  return true;
}

/****************************************
 * umatter_list_node_init
 ****************************************/

bool umatter_list_node_init(uMatterList* list)
{
  if (!list)
    return false;

  list->headFlag = false;
  list->prev = list->next = list;

  return true;
}

/****************************************
 * umatter_list_size
 ****************************************/

size_t umatter_list_size(uMatterList* head_list)
{
  uMatterList* list;
  int list_cnt;

  if (!head_list)
    return 0;

  list_cnt = 0;
  for (list = umatter_list_next(head_list); list != NULL;
      list = umatter_list_next(list))
    list_cnt++;

  return list_cnt;
}

/****************************************
 * umatter_list_get
 ****************************************/

uMatterList* umatter_list_get(uMatterList* head_list, int index)
{
  uMatterList* list;
  int n;

  if (!head_list)
    return NULL;

  list = umatter_list_next(head_list);
  for (n = 0; n < index; n++) {
    if (!list)
      break;
    list = umatter_list_next(list);
  }

  return list;
}

/****************************************
 * umatter_list_insert
 ****************************************/

bool umatter_list_insert(uMatterList* prev_list, uMatterList* list)
{
  if (!prev_list || !list)
    return false;

  list->prev = prev_list;
  list->next = prev_list->next;
  prev_list->next->prev = list;
  prev_list->next = list;

  return true;
}

/****************************************
 * umatter_list_add
 ****************************************/

bool umatter_list_add(uMatterList* head_list, uMatterList* list)
{
  if (!head_list || !list)
    return false;

  if (!head_list->prev)
    return false;

  umatter_list_insert(head_list->prev, list);

  return true;
}

/****************************************
 * umatter_list_remove
 ****************************************/

bool umatter_list_remove(uMatterList* list)
{
  if (!list)
    return false;

  if (!list->prev || !list->next)
    return false;

  list->prev->next = list->next;
  list->next->prev = list->prev;
  list->prev = list->next = list;

  return true;
}

/****************************************
 * umatter_list_prev_circular
 ****************************************/

uMatterList* umatter_list_prev_circular(uMatterList* list)
{
  if (!list)
    return NULL;

  if (!list->prev)
    return NULL;

  if (list->prev->headFlag)
    list = list->prev;

  return list->prev;
}

/****************************************
 * umatter_list_prev
 ****************************************/

uMatterList* umatter_list_prev(uMatterList* list)
{
  if (!list)
    return NULL;

  if (!list->prev)
    return NULL;

  if (list->prev->headFlag)
    return NULL;

  return list->prev;
}

/****************************************
 * umatter_list_next_circular
 ****************************************/

uMatterList* umatter_list_next_circular(uMatterList* list)
{
  if (!list)
    return NULL;

  if (!list->next)
    return NULL;

  if (list->next->headFlag == true)
    list = list->next;

  return list->next;
}

/****************************************
 * umatter_list_gets
 ****************************************/

uMatterList* umatter_list_gets(uMatterList* head_list)
{
  if (!head_list)
    return NULL;

  return umatter_list_next(head_list);
}

/****************************************
 * umatter_list_next
 ****************************************/

uMatterList* umatter_list_next(uMatterList* list)
{
  if (!list)
    return NULL;

  if (!list->next)
    return NULL;

  if (list->next->headFlag == true)
    return NULL;

  return list->next;
}

/****************************************
 * umatter_list_clear
 ****************************************/

bool umatter_list_clear(uMatterList* head_list,
    UMATTER_LIST_DESTRUCTORFUNC dstructor_func)
{
  uMatterList *list, *next_list;

  if (!head_list)
    return false;

  list = umatter_list_next(head_list);
  while (list != NULL) {
    next_list = umatter_list_next(list);
    umatter_list_remove(list);
    if (dstructor_func != NULL) {
      dstructor_func(list);
    }
    else {
      free(list);
    }
    list = next_list;
  }

  return true;
}
