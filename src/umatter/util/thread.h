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

#ifndef _UMATTER_UTIL_THREAD_H_
#define _UMATTER_UTIL_THREAD_H_

#include <umatter/typedef.h>
#include <umatter/util/list.h>

#include <umatter/util/timer.h>
#if defined(WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <signal.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

#define UMATTER_THREAD_MIN_SLEEP 200

/****************************************
 * Data Type
 ****************************************/

typedef struct UEchoThread {
  UMATTER_LIST_STRUCT_MEMBERS

  bool runnable_flag;

#if defined(WIN32)
  HANDLE hThread;
  DWORD threadID;
#else
  pthread_t p_thread;
#endif

  void (*action)(struct UEchoThread*);
  void* user_data;
} uMatterThread, uMatterThreadList;

typedef void (*uMatterThreadFunc)(uMatterThread*);

/****************************************
 * Function
 ****************************************/

uMatterThread* umatter_thread_new(void);
bool umatter_thread_delete(uMatterThread* thread);

bool umatter_thread_start(uMatterThread* thread);
bool umatter_thread_stop(uMatterThread* thread);
bool umatter_thread_restart(uMatterThread* thread);
bool umatter_thread_isrunnable(uMatterThread* thread);
bool umatter_thread_isrunning(uMatterThread* thread);

void umatter_thread_setaction(uMatterThread* thread, uMatterThreadFunc action_func);
void umatter_thread_setuserdata(uMatterThread* thread, void* data);
void* umatter_thread_getuserdata(uMatterThread* thread);

#define umatter_thread_next(thread) \
  (uMatterThread*)umatter_list_next((uMatterList*)thread)
#define umatter_thread_remove(thread) umatter_list_remove((uMatterList*)thread)

/****************************************
 * Function (Thread List)
 ****************************************/

uMatterThreadList* umatter_threadlist_new(void);
void umatter_threadlist_delete(uMatterThreadList* thread_list);

#define umatter_threadlist_clear(threadList)   \
  umatter_list_clear((uMatterList*)threadList, \
      (UMATTER_LIST_DESTRUCTORFUNC)umatter_thread_delete)
#define umatter_threadlist_size(threadList) \
  umatter_list_size((uMatterList*)threadList)
#define umatter_threadlist_gets(threadList) \
  (uMatterThread*)umatter_list_next((uMatterList*)threadList)
#define umatter_threadlist_add(threadList, thread) \
  umatter_list_add((uMatterList*)threadList, (uMatterList*)thread)
#define umatter_threadlist_remove(thread) \
  umatter_list_remove((uMatterList*)thread)

bool umatter_threadlist_start(uMatterThreadList* thread_list);
bool umatter_threadlist_stop(uMatterThreadList* thread_list);

#ifdef __cplusplus

} /* extern "C" */

#endif

#endif
