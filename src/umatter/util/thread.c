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

#if !defined(WIN32)
#include <signal.h>
#endif

#include <string.h>
#include <umatter/util/thread.h>
#include <umatter/util/timer.h>

static void umatter_sig_handler(int sign);

/****************************************
 * Thread Function
 ****************************************/

#if defined(WIN32)
static DWORD WINAPI Win32ThreadProc(LPVOID lpParam)
{
  uMatterThread* thread;

  thread = (uMatterThread*)lpParam;
  if (thread->action != NULL)
    thread->action(thread);

  return 0;
}
#else
static void* posix_thread_proc(void* param)
{
  sigset_t set;
  struct sigaction actions;
  uMatterThread* thread = (uMatterThread*)param;

  sigfillset(&set);
  sigdelset(&set, SIGQUIT);
  pthread_sigmask(SIG_SETMASK, &set, NULL);

  memset(&actions, 0, sizeof(actions));
  sigemptyset(&actions.sa_mask);
  actions.sa_flags = 0;
  actions.sa_handler = umatter_sig_handler;
  sigaction(SIGQUIT, &actions, NULL);

  if (thread->action != NULL)
    thread->action(thread);

  return 0;
}
#endif

/****************************************
 * umatter_thread_new
 ****************************************/

uMatterThread* umatter_thread_new(void)
{
  uMatterThread* thread;

  thread = (uMatterThread*)malloc(sizeof(uMatterThread));

  if (!thread)
    return NULL;

  umatter_list_node_init((uMatterList*)thread);

  thread->runnable_flag = false;
  thread->action = NULL;
  thread->user_data = NULL;

  return thread;
}

/****************************************
 * umatter_thread_delete
 ****************************************/

bool umatter_thread_delete(uMatterThread* thread)
{
  if (!thread)
    return false;

  if (thread->runnable_flag == true) {
    umatter_thread_stop(thread);
  }

  umatter_thread_remove(thread);

  free(thread);

  return true;
}

/****************************************
 * umatter_thread_start
 ****************************************/

bool umatter_thread_start(uMatterThread* thread)
{
  if (!thread)
    return false;

  thread->runnable_flag = true;

#if defined(WIN32)
  thread->hThread = CreateThread(NULL, 0, Win32ThreadProc, (LPVOID)thread, 0, &thread->threadID);
#else
  pthread_attr_t thread_attr;
  if (pthread_attr_init(&thread_attr) != 0) {
    thread->runnable_flag = false;
    return false;
  }

  if (pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) != 0) {
    thread->runnable_flag = false;
    pthread_attr_destroy(&thread_attr);
    return false;
  }

  if (pthread_create(&thread->p_thread, &thread_attr, posix_thread_proc, thread) != 0) {
    thread->runnable_flag = false;
    pthread_attr_destroy(&thread_attr);
    return false;
  }
  pthread_attr_destroy(&thread_attr);
#endif

  return true;
}

/****************************************
 * umatter_thread_stop
 ****************************************/

bool umatter_thread_stop(uMatterThread* thread)
{
  if (!thread)
    return false;

  if (thread->runnable_flag == true) {
    thread->runnable_flag = false;
#if defined(WIN32)
    TerminateThread(thread->hThread, 0);
    WaitForSingleObject(thread->hThread, INFINITE);
#else
    pthread_kill(thread->p_thread, 0);
    /* Now we wait one second for thread termination instead of using
     * pthread_join */
    umatter_sleep(UMATTER_THREAD_MIN_SLEEP);
#endif
  }

  return true;
}

/****************************************
 * umatter_thread_restart
 ****************************************/

bool umatter_thread_restart(uMatterThread* thread)
{
  umatter_thread_stop(thread);
  return umatter_thread_start(thread);
}

/****************************************
 * umatter_thread_isrunnable
 ****************************************/

bool umatter_thread_isrunnable(uMatterThread* thread)
{
  if (!thread)
    return false;

#if !defined(WIN32)
  pthread_testcancel();
#endif

  return thread->runnable_flag;
}

/****************************************
 * umatter_thread_isrunning
 ****************************************/

bool umatter_thread_isrunning(uMatterThread* thread)
{
  if (!thread)
    return false;

  return thread->runnable_flag;
}

/****************************************
 * umatter_thread_setaction
 ****************************************/

void umatter_thread_setaction(uMatterThread* thread, uMatterThreadFunc func)
{
  if (!thread)
    return;

  thread->action = func;
}

/****************************************
 * umatter_thread_setuserdata
 ****************************************/

void umatter_thread_setuserdata(uMatterThread* thread, void* value)
{
  if (!thread)
    return;

  thread->user_data = value;
}

/****************************************
 * umatter_thread_getuserdata
 ****************************************/

void* umatter_thread_getuserdata(uMatterThread* thread)
{
  if (!thread)
    return NULL;

  return thread->user_data;
}

/****************************************
 * umatter_sig_handler
 ****************************************/

static void umatter_sig_handler(int sign) { }
