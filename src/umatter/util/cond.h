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

#ifndef _UMATTER_UTIL_COND_H_
#define _UMATTER_UTIL_COND_H_

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

typedef struct UEchoCond {
  pthread_mutex_t mutex_id;
  pthread_cond_t cond_id;
} uMatterCond;

/****************************************
 * Functions
 ****************************************/

uMatterCond* umatter_cond_new(void);
bool umatter_cond_delete(uMatterCond* cond);

bool umatter_cond_wait(uMatterCond* cond);
bool umatter_cond_timedwait(uMatterCond* cond, clock_t mtime);
bool umatter_cond_signal(uMatterCond* cond);

#ifdef __cplusplus

} /* extern "C" */

#endif

#endif
