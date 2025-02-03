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

#include <umatter/util/mutex.h>

#include <errno.h>

/****************************************
 * umatter_mutex_new
 ****************************************/

uMatterMutex* umatter_mutex_new(void)
{
  uMatterMutex* mutex;

  mutex = (uMatterMutex*)malloc(sizeof(uMatterMutex));

  if (!mutex)
    return NULL;

#if defined(WIN32)
  mutex->mutex_id = CreateMutex(NULL, false, NULL);
#else
  pthread_mutex_init(&mutex->mutex_id, NULL);
#endif

  return mutex;
}

/****************************************
 * umatter_mutex_delete
 ****************************************/

bool umatter_mutex_delete(uMatterMutex* mutex)
{
  if (!mutex)
    return false;

#if defined(WIN32)
  CloseHandle(mutex->mutex_id);
#else
  pthread_mutex_destroy(&mutex->mutex_id);
#endif
  free(mutex);

  return true;
}

/****************************************
 * umatter_mutex_lock
 ****************************************/

bool umatter_mutex_lock(uMatterMutex* mutex)
{
  if (!mutex)
    return false;

#if defined(WIN32)
  WaitForSingleObject(mutex->mutex_id, INFINITE);
#else
  pthread_mutex_lock(&mutex->mutex_id);
#endif

  return true;
}

/****************************************
 * umatter_mutex_unlock
 ****************************************/

bool umatter_mutex_unlock(uMatterMutex* mutex)
{
  if (!mutex)
    return false;

#if defined(WIN32)
  ReleaseMutex(mutex->mutex_id);
#else
  pthread_mutex_unlock(&mutex->mutex_id);
#endif
  return true;
}
