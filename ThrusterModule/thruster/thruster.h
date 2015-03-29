#ifndef _THRUSTER_H_
#define _THRUSTER_H_

#ifndef THRUSTER_THREAD_STACK_SIZE
#define THRUSTER_THREAD_STACK_SIZE   256
#endif

#ifndef THRUSTER_THREAD_PRIORITY
#define THRUSTER_THREAD_PRIORITY     (LOWPRIO + 2)
#endif

#define CAN_ID_THRUSTER 0x1
#define CAN_ID_MANIP 0x11
#define CAN_ID_SERVER 0x3

extern WORKING_AREA(wa_thruster, THRUSTER_THREAD_STACK_SIZE);

//static struct ThValues receiveData(void);

#ifdef __cplusplus
extern "C" {
#endif
  msg_t thruster_thread(void *p);
#ifdef __cplusplus
}
#endif

#endif /* _WEB_H_ */