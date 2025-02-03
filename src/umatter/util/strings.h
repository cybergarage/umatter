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

#ifndef _UMATTER_UTIL_STRING_H_
#define _UMATTER_UTIL_STRING_H_

#include <stdlib.h>
#include <umatter/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

#define UMATTER_STRING_MEMORY_ALLOCATION_UNIT 64

/* UINT_MAX : 4294967295U */
#define UMATTER_STRING_INTEGER_BUFLEN 16

/* ULONG_MAX : 4294967295UL */
#define UMATTER_STRING_LONG_BUFLEN 32

/*  ULLONG_MAX : 18446744073709551615ULL */
#define UMATTER_STRING_LONGLONG_BUFLEN 32

#define UMATTER_STRING_FLOAT_BUFLEN 64
#define UMATTER_STRING_DOUBLE_BUFLEN 64

/****************************************
 * Function
 ****************************************/

char* umatter_strdup(const char* str);
size_t umatter_strlen(const char* str);
char* umatter_strcpy(char* dst, const char* src);
char* umatter_strcat(char* dst, const char* src);
int umatter_strcmp(const char* str1, const char* str2);
int umatter_strncmp(const char* str1, const char* str2, size_t nchars);
int umatter_strcasecmp(const char* str1, const char* str2);
bool umatter_streq(const char* str1, const char* str2);
bool umatter_strcaseeq(const char* str1, const char* str2);
ssize_t umatter_strchr(const char* str, const char* chars, size_t nchars);
ssize_t umatter_strrchr(const char* str, const char* chars, size_t nchars);
ssize_t umatter_strstr(const char* haystack, const char* needle);
char* umatter_strtrimwhite(char* str);
char* umatter_strtrim(char* str, char* delim, size_t ndelim);
char* umatter_strltrim(char* str, char* delim, size_t ndelim);
char* umatter_strrtrim(char* str, char* delim, size_t ndelim);
char* umatter_strncpy(char* str1, const char* str2, size_t cnt);
char* umatter_strncat(char* str1, const char* str2, size_t cnt);
bool umatter_strloc(const char* str, char** buf);

const char* umatter_int2str(int value, char* buf, size_t buf_size);
const char* umatter_long2str(long value, char* buf, size_t buf_size);
const char* umatter_float2str(float value, char* buf, size_t buf_size);
const char* umatter_double2str(double value, char* buf, size_t buf_size);
const char* umatter_sizet2str(size_t value, char* buf, size_t buf_size);
const char* umatter_ssizet2str(ssize_t value, char* buf, size_t buf_size);

#define umatter_str2int(value) (value ? atoi(value) : 0)
#define umatter_str2long(value) (value ? atol(value) : 0)
#define umatter_strhex2long(value) (value ? strtol(value, NULL, 16) : 0)
#define umatter_strhex2ulong(value) (value ? strtoul(value, NULL, 16) : 0)
#define umatter_str2float(value) ((float)(value ? atof(value) : 0.0))
#define umatter_str2double(value) (value ? atof(value) : 0.0)
#define umatter_str2sizet(value) ((size_t)(value ? atol(value) : 0))
#define umatter_str2ssizet(value) ((ssize_t)(value ? atol(value) : 0))

/****************************************
 * Data Type
 ****************************************/

typedef struct UEchoString {
  char* value;
  size_t mem_size;
  size_t value_size;
} uMatterString;

typedef struct UEchoStringTokenizer {
  char* value;
  char* delim;
  size_t delim_cnt;
  size_t next_start_pos;
  size_t last_pos;
  char* curr_token;
  char* next_token;
  char rep_token;
  bool has_next_tokens;
} uMatterStringTokenizer;

/****************************************
 * Function (StringTokenizer)
 ****************************************/

uMatterStringTokenizer* umatter_string_tokenizer_new(const char* str,
    const char* delim);
void umatter_string_tokenizer_delete(uMatterStringTokenizer* str_token);
bool umatter_string_tokenizer_hasmoretoken(uMatterStringTokenizer* str_token);
char* umatter_string_tokenizer_nexttoken(uMatterStringTokenizer* str_token);
char* umatter_string_tokenizer_nextalltoken(uMatterStringTokenizer* str_token);

/****************************************
 * Function (String)
 ****************************************/

uMatterString* umatter_string_new(void);
void umatter_string_delete(uMatterString* str);
void umatter_string_clear(uMatterString* str);

void umatter_string_setvalue(uMatterString* str, const char* value);
void umatter_string_setintvalue(uMatterString* str, int value);
void umatter_string_setlongvalue(uMatterString* str, long value);
void umatter_string_setfloatvalue(uMatterString* str, float value);
void umatter_string_setdoublevalue(uMatterString* str, double value);
void umatter_string_setnvalue(uMatterString* str, const char* value, size_t len);
void umatter_string_setpointervalue(uMatterString* str, char* value, size_t len);

char* umatter_string_getvalue(uMatterString* str);
#define umatter_string_getintvalue(str) \
  umatter_str2int(umatter_string_getvalue(str))
#define umatter_string_getlongvalue(str) \
  umatter_str2long(umatter_string_getvalue(str))
#define umatter_string_getfloatvalue(str) \
  umatter_str2float(umatter_string_getvalue(str))
#define umatter_string_getdoublevalue(str) \
  umatter_str2double(umatter_string_getvalue(str))

size_t umatter_string_length(uMatterString* str);

char* umatter_string_addvalue(uMatterString* str, const char* value);
char* umatter_string_naddvalue(uMatterString* str, const char* value, size_t value_len);
char* umatter_string_addrepvalue(uMatterString* str, const char* value, size_t repeat_cnt);
char* umatter_string_naddrepvalue(uMatterString* str, const char* value, size_t value_len, size_t repeat_cnt);

char* umatter_string_replace(uMatterString* str, char* from_str[], char* to_str[], size_t from_str_cnt);

size_t umatter_string_getmemorysize(uMatterString* str);

#ifdef __cplusplus
}
#endif

#endif
