#ifndef RECEIVER_H
#define RECEIVER_H

#ifndef RECEIVER_THREAD_STACK_SIZE
#define RECEIVER_THREAD_STACK_SIZE   1024
#endif

#ifndef RECEIVER_THREAD_PORT
#define RECEIVER_THREAD_PORT         50000
#endif

/* Contents */

extern WORKING_AREA(wa_receiver, RECEIVER_THREAD_STACK_SIZE);

extern msg_t receiver_thread(void *p);

#endif