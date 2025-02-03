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

/****************************************
 * umatter_string_tokenizer_new
 ****************************************/

uMatterStringTokenizer* umatter_string_tokenizer_new(const char* value,
    const char* delim)
{
  uMatterStringTokenizer* str_token;

  str_token = (uMatterStringTokenizer*)malloc(sizeof(uMatterStringTokenizer));

  if (NULL != str_token) {
    str_token->value = umatter_strdup(value);
    str_token->delim = umatter_strdup(delim);
    str_token->delim_cnt = umatter_strlen(str_token->delim);
    str_token->next_start_pos = 0;
    str_token->last_pos = umatter_strlen(value) - 1;
    str_token->curr_token = NULL;
    str_token->next_token = NULL;
    umatter_string_tokenizer_nexttoken(str_token);
  }

  return str_token;
}

/****************************************
 * umatter_string_tokenizer_delete
 ****************************************/

void umatter_string_tokenizer_delete(uMatterStringTokenizer* str_token)
{
  free(str_token->value);
  free(str_token->delim);
  free(str_token);
}

/****************************************
 * umatter_string_tokenizer_hasmoretoken
 ****************************************/

bool umatter_string_tokenizer_hasmoretoken(uMatterStringTokenizer* str_token)
{
  return str_token->has_next_tokens;
}

/****************************************
 * umatter_string_tokenizer_nexttoken
 ****************************************/

char* umatter_string_tokenizer_nexttoken(uMatterStringTokenizer* str_token)
{
  size_t token_cnt;
  size_t i, j;

  str_token->curr_token = str_token->next_token;
  str_token->next_token = NULL;
  str_token->has_next_tokens = false;
  str_token->rep_token = '\0';

  token_cnt = 0;
  for (i = str_token->next_start_pos; i <= str_token->last_pos; i++) {
    bool is_delim_char = false;
    for (j = 0; j < str_token->delim_cnt; j++) {
      if (str_token->value[i] == str_token->delim[j]) {
        is_delim_char = true;
        if (token_cnt == 0) {
          str_token->next_start_pos = i + 1;
          break;
        }
        str_token->has_next_tokens = true;
        str_token->rep_token = str_token->value[i];
        str_token->value[i] = '\0';
        str_token->next_token = str_token->value + str_token->next_start_pos;
        str_token->next_start_pos = i + 1;
      }
    }
    if (str_token->has_next_tokens == true)
      break;
    if (is_delim_char == false)
      token_cnt++;
  }

  if (str_token->has_next_tokens == false && 0 < token_cnt) {
    str_token->has_next_tokens = true;
    str_token->next_token = str_token->value + str_token->next_start_pos;
    str_token->next_start_pos = str_token->last_pos + 1;
  }

  return str_token->curr_token;
}

/****************************************
 * umatter_string_tokenizer_nextalltoken
 ****************************************/

char* umatter_string_tokenizer_nextalltoken(uMatterStringTokenizer* str_token)
{
  size_t next_token_len;

  next_token_len = umatter_strlen(str_token->next_token);
  str_token->next_token[next_token_len] = str_token->rep_token;
  str_token->curr_token = str_token->next_token;
  str_token->next_token = NULL;
  str_token->has_next_tokens = false;
  return str_token->curr_token;
}
