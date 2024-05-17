/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "Board_LED.h"
#include <stdio.h>

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread;                        // thread id
 
void Thread (void *argument);                   // thread function
 
int Init_Thread (void) {
 
  tid_Thread = osThreadNew(Thread, NULL, NULL);
  if (tid_Thread == NULL) {
    return(-1);
  }
 
  return(0);
}

void Thread (void *argument) {
	LED_Initialize();
  while (1) {
    osThreadYield();                            // suspend thread
		printf("WHY");
		LED_Off(1);
		osDelay(500);
		LED_On(1);
		osDelay(500);
  }
}




void Init_TIMER0(void){	
	NVIC_EnableIRQ(TIMER0_IRQn); // Enable timer0 on NVIC
	LPC_TIMER0->MCR |= 3; //set first and 2nd bit of MCR to 1, enable interrupt on match and then clear
	LPC_CCU1->CLK_M4_TIMER0_CFG |= 0x1; // enable timer0 clock
	LPC_TIMER0->TCR = 0x2; // reset timer0
	//LPC_TIMER0->CTCR = 0; // not needed
  //LPC_TIMER0->CCR = 0; // not needed
	//LPC_TIMER0->PC = 0;
	LPC_TIMER0->PR = 0; // set prescaler , doesnt matter?
	//LPC_TIMER0->EMR = 0;
	
	LPC_TIMER0->MR0 = 10000000; // amount of microseconds, this is the match value
	LPC_TIMER0->TCR = 0x1; // Enable timer0
}

static int x=0;
void increment(void);

//short function to minimize interrupt, alt use semaphore or flag in combination with a thread and set sem/flag inside IRQ_handler
void increment(void){
	if(x>100){
	x=0;
	}
	x++;
}

void TIMER0_IRQHandler(void){
	LPC_TIMER0->IR = 1; //Clear flag
	increment();
}
 
__NO_RETURN static void app_main (void *argument) {
  (void)argument;
  // ...
  for (;;) {}
}
 
int main (void) {
  // System Initialization
  SystemCoreClockUpdate();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
	Init_TIMER0();
	Init_Thread();
	
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
