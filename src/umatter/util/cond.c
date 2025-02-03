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

#include <umatter/util/cond.h>

#include <errno.h>

/****************************************
 * umatter_cond_new
 ****************************************/

uMatterCond* umatter_cond_new(void)
{
  uMatterCond* cond;

  cond = (uMatterCond*)malloc(sizeof(uMatterCond));

  if (!cond)
    return NULL;

  pthread_mutex_init(&cond->mutex_id, NULL);
  pthread_cond_init(&cond->cond_id, NULL);

  return cond;
}

/****************************************
 * umatter_cond_delete
 ****************************************/

bool umatter_cond_delete(uMatterCond* cond)
{
  if (!cond)
    return false;

  pthread_mutex_destroy(&cond->mutex_id);
  pthread_cond_destroy(&cond->cond_id);

  free(cond);

  return true;
}

/****************************************
 * umatter_cond_wait
 ****************************************/

bool umatter_cond_wait(uMatterCond* cond)
{
  if (!cond)
    return false;

  pthread_mutex_lock(&cond->mutex_id);
  pthread_cond_wait(&cond->cond_id, &cond->mutex_id);
  pthread_mutex_unlock(&cond->mutex_id);

  return true;
}

/****************************************
 * umatter_cond_timedwait
 ****************************************/

bool umatter_cond_timedwait(uMatterCond* cond, clock_t mtime)
{
  struct timespec to;
  int c;

  if (!cond)
    return false;

  to.tv_sec = time(NULL) + (mtime / CLOCKS_PER_SEC);
  to.tv_nsec = 0;

  pthread_mutex_lock(&cond->mutex_id);
  c = pthread_cond_timedwait(&cond->cond_id, &cond->mutex_id, &to);
  pthread_mutex_unlock(&cond->mutex_id);

  return (c == 0) ? true : false;
}

/****************************************
 * umatter_cond_signal
 ****************************************/

bool umatter_cond_signal(uMatterCond* cond)
{
  if (!cond)
    return false;

  pthread_mutex_lock(&cond->mutex_id);
  pthread_cond_signal(&cond->cond_id);
  pthread_mutex_unlock(&cond->mutex_id);

  return true;
}
