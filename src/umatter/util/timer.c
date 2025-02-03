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

#include <umatter/util/timer.h>

#include <limits.h>

#if defined(WIN32)
#include <time.h>
#include <windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif

/****************************************
 * umatter_time_wait
 ****************************************/

void umatter_wait(clock_t mtime)
{
#if defined(WIN32)
  Sleep(mtime);
#else
  usleep(((useconds_t)(mtime * 1000)));
#endif
}

/****************************************
 * umatter_time_wait
 ****************************************/

void umatter_waitrandom(clock_t mtime)
{
  double factor;
  long wait_time;

  factor = (double)rand() / (double)RAND_MAX;
  wait_time = (long)((double)mtime * factor);
  umatter_wait(wait_time);
}

/****************************************
 * umatter_time_wait
 ****************************************/

clock_t umatter_getcurrentsystemtime(void)
{
  return (size_t)(time((time_t*)NULL));
}

/****************************************
 * umatter_random
 ****************************************/

float umatter_random(void)
{
  static bool seed_done = false;

  if (seed_done == false) {
    srand((int)(umatter_getcurrentsystemtime() % INT_MAX));
    seed_done = true;
  }

  return (float)rand() / (float)RAND_MAX;
}
