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

#ifndef _UMATTER_UTIL_TIME_H_
#define _UMATTER_UTIL_TIME_H_

#include <umatter/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Function
 ****************************************/

float umatter_random(void);

void umatter_wait(clock_t mtime);
void umatter_waitrandom(clock_t mtime);

#define umatter_sleep(val) umatter_wait(val)
#define umatter_sleeprandom(val) umatter_waitrandom(val)

clock_t umatter_getcurrentsystemtime(void);

#ifdef __cplusplus
}
#endif

#endif
