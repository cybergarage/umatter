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

#include <umatter/util/strings.h>

#include <string.h>

/* Define amount of extra characters allocated on each realloc, with this we
   can avoid many small subsequent reallocs, which takes lots of time */
#define UMATTER_STRING_REALLOC_EXTRA 16

/****************************************
 * umatter_string_new
 ****************************************/

uMatterString* umatter_string_new(void)
{
  uMatterString* str;

  str = (uMatterString*)malloc(sizeof(uMatterString));

  if (NULL != str) {
    str->value = NULL;
    str->mem_size = 0;
    str->value_size = 0;
  }

  return str;
}

/****************************************
 * umatter_string_delete
 ****************************************/

void umatter_string_delete(uMatterString* str)
{
  if (NULL != str) {
    umatter_string_clear(str);
    free(str);
  }
}

/****************************************
 * umatter_string_delete
 ****************************************/

void umatter_string_clear(uMatterString* str)
{
  if (NULL != str) {
    if (str->value != NULL) {
      free(str->value);
      str->value = NULL;
      str->mem_size = 0;
      str->value_size = 0;
    }
  }
}

/****************************************
 * umatter_string_setvalue
 ****************************************/

void umatter_string_setvalue(uMatterString* str, const char* value)
{
  if (NULL != str) {
    if (value != NULL)
      umatter_string_setnvalue(str, value, umatter_strlen(value));
  }
}

/****************************************
 * umatter_string_setintvalue
 ****************************************/

void umatter_string_setintvalue(uMatterString* str, int value)
{
  char buf[UMATTER_STRING_INTEGER_BUFLEN];

  umatter_string_setvalue(str, umatter_int2str(value, buf, sizeof(buf)));
}

/****************************************
 * umatter_string_setlongvalue
 ****************************************/

void umatter_string_setlongvalue(uMatterString* str, long value)
{
  char buf[UMATTER_STRING_LONG_BUFLEN];

  umatter_string_setvalue(str, umatter_long2str(value, buf, sizeof(buf)));
}

/****************************************
 * umatter_string_setnvalue
 ****************************************/

void umatter_string_setnvalue(uMatterString* str, const char* value, size_t len)
{
  if (NULL != str) {
    umatter_string_clear(str);
    if (value != NULL) {
      str->value_size = len;
      str->mem_size = str->value_size + 1;
      str->value = (char*)malloc(str->mem_size * sizeof(char));

      if (NULL == str->value) {
        return;
      }

      /* memcpy works better with non-zero-terminated data than strncpy */
      memcpy(str->value, value, len);
      str->value[len] = '\0';
    }
  }
}

/****************************************
 * umatter_string_setpointervalue
 ****************************************/

void umatter_string_setpointervalue(uMatterString* str, char* value, size_t len)
{
  if (NULL != str) {
    umatter_string_clear(str);
    str->value = value;
    str->value_size = len;
    str->mem_size = str->value_size + 1;
  }
}

/****************************************
 * umatter_string_getvalue
 ****************************************/

char* umatter_string_getvalue(uMatterString* str)
{
  return (NULL != str) ? str->value : NULL;
}

/****************************************
 * umatter_string_getmemorysize
 ****************************************/

size_t umatter_string_getmemorysize(uMatterString* str)
{
  if (NULL == str)
    return 0;

  return str->mem_size;
}

/****************************************
 * umatter_string_length
 ****************************************/

size_t umatter_string_length(uMatterString* str)
{
  if (NULL == str)
    return 0;

  if (str->value == NULL)
    return 0;

  return str->value_size;
}

/****************************************
 * umatter_string_add
 ****************************************/

char* umatter_string_addvalue(uMatterString* str, const char* value)
{
  return umatter_string_naddvalue(str, value, umatter_strlen(value));
}

/****************************************
 * umatter_string_add
 ****************************************/

char* umatter_string_naddvalue(uMatterString* str, const char* value, size_t value_len)
{
  char* new_value = NULL;
  size_t new_mem_size = 0;

  if (NULL == str)
    return NULL;

  if (value == NULL || value_len <= 0) {
    /* Empty string, nothing to add */
    return umatter_string_getvalue(str);
  }

  /* Check, if we need to allocate memory for the new data */
  new_mem_size = str->value_size + value_len + 1;
  if (new_mem_size > str->mem_size || str->value == NULL) {
    /* realloc also some extra in order to avoid multiple reallocs */
    new_mem_size += UMATTER_STRING_REALLOC_EXTRA;
    new_value = realloc(str->value, new_mem_size * sizeof(char));

    if (new_value == NULL) {
      /* Memory allocation failed, bail out */
      return NULL;
    }

    str->mem_size = new_mem_size;
    str->value = new_value;
  }

  /* memcpy works better with non-zero-terminated data
     than strncpy */
  memcpy(str->value + str->value_size, value, value_len);

  str->value_size += value_len;

  /* In case this is a string, append a termination character */
  str->value[str->value_size] = '\0';

  return umatter_string_getvalue(str);
}

/****************************************
 * umatter_string_addrep
 ****************************************/

char* umatter_string_addrepvalue(uMatterString* str, const char* value, size_t repeat_cnt)
{
  int n;

  for (n = 0; n < repeat_cnt; n++)
    umatter_string_addvalue(str, value);

  return umatter_string_getvalue(str);
}

/****************************************
 * umatter_string_naddrep
 ****************************************/

char* umatter_string_naddrepvalue(uMatterString* str, const char* value, size_t value_len, size_t repeat_cnt)
{
  int n;

  for (n = 0; n < repeat_cnt; n++)
    umatter_string_naddvalue(str, value, value_len);

  return umatter_string_getvalue(str);
}

/****************************************
 * umatter_string_replace
 ****************************************/

char* umatter_string_replace(uMatterString* str, char* from_str[], char* to_str[], size_t from_str_cnt)
{
  char* org_value = NULL;
  size_t org_value_len = 0;
  int n = 0;
  int copy_pos = 0;
  size_t* from_str_len = NULL;
  uMatterString* rep_value = NULL;
  bool is_replaced = false;

  if (NULL == str)
    return NULL;

  rep_value = umatter_string_new();

  from_str_len = (size_t*)malloc(sizeof(size_t) * from_str_cnt);

  if (NULL == from_str_len) {
    umatter_string_delete(rep_value);
    return NULL;
  }

  for (n = 0; n < from_str_cnt; n++)
    from_str_len[n] = umatter_strlen(from_str[n]);

  org_value = umatter_string_getvalue(str);
  org_value_len = umatter_string_length(str);

  copy_pos = 0;
  while (copy_pos < org_value_len) {
    is_replaced = false;
    for (n = 0; n < from_str_cnt; n++) {
      if (strncmp(from_str[n], org_value + copy_pos, from_str_len[n]) == 0) {
        umatter_string_addvalue(rep_value, to_str[n]);
        copy_pos += from_str_len[n];
        is_replaced = true;
        continue;
      }
    }
    if (is_replaced == true)
      continue;
    umatter_string_naddvalue(rep_value, org_value + copy_pos, 1);
    copy_pos++;
  }

  free(from_str_len);

  umatter_string_setvalue(str, umatter_string_getvalue(rep_value));

  umatter_string_delete(rep_value);

  return umatter_string_getvalue(str);
}
