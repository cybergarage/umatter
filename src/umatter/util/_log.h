
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

#ifndef _UMATTER_UTIL_LOG_INTERNAL_H_
#define _UMATTER_UTIL_LOG_INTERNAL_H_

#include <umatter/util/log.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LOG_BUF 2048

#define UMATTER_LOG_ERROR_S "ERROR"
#define UMATTER_LOG_WARN_S "WARN"
#define UMATTER_LOG_INFO_S "INFO"
#define UMATTER_LOG_DEBUG_S "DEBUG"

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _UMATTER_UTIL_LOG_INTERNAL_H_ */
