// Automatisk kjøring av manipulator
#include "ch.h"
#include "hal.h"
#include "manip.h"
#include <math.h>

WORKING_AREA(wa_automanip_thread, AUTOMANIP_THREAD_STACK_SIZE);

int16_t a1;
int16_t a2;
int16_t a3;
int16_t x;
int16_t y;
int16_t r;
int16_t v1;
int16_t v2;
int16_t v3;
int16_t psi;
int16_t phi;
int16_t v1g;
int16_t v2g;
int16_t v3g;
int16_t puls1;
int16_t puls2;
int16_t puls3;
int16_t i;
int16_t k;

msg_t automanip_thread(void *p){
(void)p;

// Lengden av leddene
a1 = 320;
a2 = 200;
a3 = 110;

// punktet x og y
x = 200;
y = 300;
r = sqrt(x*x+y*y);

// finner vinkel 2
v2 = acos((r*r-a1*a1-a2*a2)/(2*a1*a2));

phi = atan2(x,y);
psi = acos((r*r+a1*a1-a2*a2)/(2*a1*r));

v1 = phi + psi;

v3 = v1 - v2;

v1g = v1*(180/3.14);
v2g = v2*(180/3.14);
v3g = v3*(180/3.14);

puls1 = v1g/0.9;
puls2 = v2g/0.9;
puls3 = v3g/0.9;

i = 90-v1;
k 180-30-v2; 

void pwmtellerM1(PWMDriver *pwmp){        // Tellefunksjon 1
 (void)pwmp;
 i--;
 	if(i<=0){
 		pwmDisableChannel(&PWMD1,0)
 	}
}

void pwmtellerM3(PWMDriver *pwmp){        // Tellefunksjon 3
 (void)pwmp;
 k--;
	if(k<=0){
		pwmDisableChannel(&PWMD4,0);
	}
}

if(i>0){
	pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000)); //PFM på PA8
}
if(k>0){
	pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000)); //PFM på PB6
}

}
