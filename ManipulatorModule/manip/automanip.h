

#ifndef _AUTOMANIP_H_
#define _AUTOMANIP_H_

#ifndef AUTOMANIP_THREAD_STACK_SIZE
#define AUTOMANIP_THREAD_STACK_SIZE   1024
#endif

#ifndef AUTOMANIP_THREAD_PRIORITY
#define AUTOMANIP_THREAD_PRIORITY     (LOWPRIO + 2)
#endif

extern WORKING_AREA(wa_automanip_thread, AUTOMANIP_THREAD_STACK_SIZE);
extern void pwmtellerM1(PWMDriver *pwmp);
extern void pwmtellerM2(PWMDriver *pwmp);
extern void pwmtellerM3(PWMDriver *pwmp);
extern void pwmtellerM4(PWMDriver *pwmp);
extern void pwmtellerM5(PWMDriver *pwmp);

#ifdef __cplusplus
extern "C" {
#endif
  msg_t automanip_thread(void *p);
#ifdef __cplusplus
}
#endif

#endif /* _WEB_H_ */

/** @} */
