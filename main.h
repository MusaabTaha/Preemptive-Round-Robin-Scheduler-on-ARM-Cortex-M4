/*
 * main.h
 *
 *  Created on: Apr 17, 2025
 *      Author: Musaab
 */

#ifndef MAIN_H_
#define MAIN_H_

#define MAX_TASKS  4




/* Stack Addresses */
#define TASK_STACK_SIZE           	   1024U
#define SCHEDULER_STACK_SIZE           1024U

#define START_OF_THE_STACK        0x20000000U
#define SRAM_SIZE                 ((256) * (1024))
#define SRAM_END           	      ((START_OF_THE_STACK) + (SRAM_SIZE))


#define TASK1_STACK_START       SRAM_END
#define TASK2_STACK_START       ((SRAM_END) - (1 * TASK_STACK_SIZE))
#define TASK3_STACK_START       ((SRAM_END) - (2 * TASK_STACK_SIZE))
#define TASK4_STACK_START       ((SRAM_END) - (3 * TASK_STACK_SIZE))
#define SCHEDULER_STACK_START   ((SRAM_END) - (4 * TASK_STACK_SIZE))

#define TICK_HZ          1000U
#define HSI_CLOCK        16000000U
#define SYSTICK_TIM_CLK  HSI_CLOCK
#define DUYMMY_XPSR      0x01000000



#endif /* MAIN_H_ */
