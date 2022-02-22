/*
 * icu.h
 *
 *  Created on: Feb 21, 2022
 *      Author: abdelrahman
 */

#ifndef ICU_H_
#define ICU_H_

#include "std_types.h"  /*for typdefs*/

/*
 * Description : this enum contains six members:
 *   - No_clock      : value 0 as there is no input clock to ICU
 *   - F_CPU         : value 1 as the clock frequency is equal to F_CPU
 *   - F_CPU_8       : value 2 as the clock frequency is F_CPU / 8
 *   - F_CPU_64      : value 3 as the clock frequency is F_CPU / 64
 *   - F_CPU_256     : value 4 as the clock frequency is F_CPU / 256
 *   - F_CPU_1024    : value 5 as the clock frequency is F_CPU / 1024
 */
typedef enum
{
	no_clock,f_cpu,f_cpu_8,f_cpu_64,f_cpu_256,f_cpu_1024
}ICU_Prescaler;
/*
 * Description : this enum contains two members:
 *  - falling : value 0 corresponding to falling edge of input signal
 *  - rising  : value 1 corresponding to rising  edge of input signal
 */
typedef enum
{
	falling,rising
}ICU_EdgeType;
/*
 * Description : this structure contains two members :
 *    - clock prescale
 *    - edge type
 */
typedef struct
{
	ICU_EdgeType edge;
	ICU_Prescaler prescale;
}ICU_ConfigurationType;
/*
 * Description :
 *        this function :
 *          	1. Set the required clock.
 * 				2. Set the required edge detection.
 * 				3. Enable the Input Capture Interrupt.
 *			 	4. Initialize Timer1 Registers
 * Input : pointer to ICU_ConfigurationType structure
 * Output: Void
 * Asynchronous function
 */
void ICU_init(const ICU_ConfigurationType *  );
/*
 * Description : this function setting the Callback function
 * by equating the global static pointer to function which initialized
 * by NULL pointer until it updated by this function to points to the
 * callback funtion
 */
void ICU_settingCallback(void (*)(void));
/*
 * Description : this function is setting the edge detection
 * during runtime
 * Input : ICU_EdgeType edge rising or falling
 * Output: void
 */
void ICU_setEdgeDetection(ICU_EdgeType);
/*
 * Description : this function get the capture time of the detected edge
 * Input : void
 * Output: uint16 input captured value
 * synchronous function
 */
uint16 ICU_getInputCaptureValue(void);
/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void ICU_clearTimerValue(void);

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void ICU_DeInit(void);


#endif /* ICU_H_ */
