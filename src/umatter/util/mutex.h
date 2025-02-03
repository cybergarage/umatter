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

#ifndef _UMATTER_UTIL_MUTEX_H_
#define _UMATTER_UTIL_MUTEX_H_

#include <umatter/typedef.h>

#if defined(WIN32)
#include <winsock2.h>
#else
#include <pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Types
 ****************************************/

typedef struct UEchoMutex {
#if defined(WIN32)
  HANDLE mutex_id;
#else
  pthread_mutex_t mutex_id;
#endif
} uMatterMutex;

/****************************************
 * Functions
 ****************************************/

uMatterMutex* umatter_mutex_new(void);
bool umatter_mutex_delete(uMatterMutex* mutex);

bool umatter_mutex_lock(uMatterMutex* mutex);
bool umatter_mutex_unlock(uMatterMutex* mutex);

#ifdef __cplusplus

} /* extern "C" */

#endif

#endif
