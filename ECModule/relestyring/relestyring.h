/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file web.h
 * @brief HTTP server wrapper thread macros and structures.
 * @addtogroup WEB_THREAD
 * @{
 */

#ifndef _RELESTYRING_H_
#define _RELESTYRING_H_

#ifndef RELE_THREAD_STACK_SIZE
#define RELE_THREAD_STACK_SIZE   128
#endif

#ifndef RELE_THREAD_PRIORITY
#define RELE_THREAD_PRIORITY     (LOWPRIO + 2)
#endif

extern WORKING_AREA(waRelestyring, RELE_THREAD_STACK_SIZE);

#ifdef __cplusplus
extern "C" {
#endif
  msg_t relestyring(void *p);
#ifdef __cplusplus
}
#endif

#endif /* _RELESTYRING_H_ */

/** @} */
