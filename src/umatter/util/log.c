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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <umatter/util/_log.h>
#include <umatter/util/mutex.h>

#if defined(WIN32)
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif

static uMatterLogLevel log_level = UMATTER_LOG_NONE;
static const char* log_error_s = UMATTER_LOG_ERROR_S;
static const char* log_warning_s = UMATTER_LOG_WARN_S;
static const char* log_info_s = UMATTER_LOG_INFO_S;
static const char* log_debug_s = UMATTER_LOG_DEBUG_S;

static const char* umatter_log_type2string(int type)
{
  switch (type) {
  case UMATTER_LOG_ERROR:
    return log_error_s;
    break;

  case UMATTER_LOG_WARN:
    return log_warning_s;
    break;

  case UMATTER_LOG_INFO:
    return log_info_s;
    break;

  case UMATTER_LOG_DEBUG:
    return log_debug_s;
    break;
  }

  return "";
}

void umatter_log_setlevel(uMatterLogLevel level) { log_level = level; }

void umatter_log_output(int severity, const char* file, int line_n, const char* function, const char* format, ...)
{
  va_list list;

  char msg[MAX_LOG_BUF], ts_prefix[MAX_LOG_BUF];
  time_t ts;
  struct tm* localts;
  size_t prefix_len = -1;

  if (log_level < severity)
    return;

  ts = time(NULL);
  localts = localtime(&ts);

  strftime(ts_prefix, MAX_LOG_BUF, "%F %T", localts);

  prefix_len = snprintf(msg, MAX_LOG_BUF, "%s : %s ", ts_prefix, umatter_log_type2string(severity));

  va_start(list, format);
  vsnprintf(msg + prefix_len, MAX_LOG_BUF - prefix_len, format, list);
  va_end(list);

  printf("%s\n", msg);
}
