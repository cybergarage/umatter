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

#include <umatter/util/thread.h>

/****************************************
 * umatter_threadlist_new
 ****************************************/

uMatterThreadList* umatter_threadlist_new(void)
{
  uMatterThreadList* thread_list;

  thread_list = (uMatterThreadList*)malloc(sizeof(uMatterThreadList));

  if (!thread_list)
    return NULL;

  umatter_list_header_init((uMatterList*)thread_list);
  thread_list->runnable_flag = false;
  thread_list->action = NULL;
  thread_list->user_data = NULL;

  return thread_list;
}

/****************************************
 * umatter_threadlist_delete
 ****************************************/

void umatter_threadlist_delete(uMatterThreadList* thread_list)
{
  if (!thread_list)
    return;

  umatter_threadlist_clear(thread_list);
  free(thread_list);
}

/****************************************
 * umatter_threadlist_start
 ****************************************/

bool umatter_threadlist_start(uMatterThreadList* thread_list)
{
  uMatterThreadList* thread;

  if (!thread_list)
    return false;

  for (thread = umatter_threadlist_gets(thread_list); thread != NULL;
      thread = umatter_thread_next(thread)) {
    umatter_thread_start(thread);
  }

  return true;
}

/****************************************
 * umatter_threadlist_stop
 ****************************************/

bool umatter_threadlist_stop(uMatterThreadList* thread_list)
{
  uMatterThreadList* thread;

  if (!thread_list)
    return false;

  for (thread = umatter_threadlist_gets(thread_list); thread != NULL;
      thread = umatter_thread_next(thread))
    umatter_thread_stop(thread);

  return true;
}
