/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Musaab
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include <stdio.h>
#include <main.h>

void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void systick_timer_init(uint32_t tick_hz);
__attribute__ ((naked)) void scheduler_stack_init(uint32_t top_of_stack_scheduler);
__attribute__ ((naked)) void switch_to_psp(void);
void tasks_stack_init(void);
void enable_processor_faults(void);
void switch_to_psp(void);

uint8_t  current_task = 0;
uint32_t handlers_of_task[MAX_TASKS];
uint32_t tasks_psp[MAX_TASKS] = {TASK1_STACK_START,TASK2_STACK_START,TASK3_STACK_START,TASK4_STACK_START};

int main(void){

	enable_processor_faults();

	scheduler_stack_init(SCHEDULER_STACK_START);

	handlers_of_task[0] = (uint32_t)task1_handler;
	handlers_of_task[1] = (uint32_t)task2_handler;
	handlers_of_task[2] = (uint32_t)task3_handler;
	handlers_of_task[3] = (uint32_t)task4_handler;

	tasks_stack_init();
	systick_timer_init(TICK_HZ);

	switch_to_psp();

	task2_handler();

    /* Loop forever */
	for(;;);
}

void task1_handler(void){

	while(1){

		printf("This is task1\n");

	}

}

void task2_handler(void){

	while(1){

		printf("This is task2\n");

	}

}

void task3_handler(void){

	while(1){

		printf("This is task3\n");

	}

}

void task4_handler(void){

	while(1){

		printf("This is task4\n");

	}

}

void systick_timer_init(uint32_t tick_hz){

	uint32_t *pSRVR = (uint32_t*) 0xE000E014;
	uint32_t *pSCSR = (uint32_t*) 0xE000E010;

	uint32_t count = (SYSTICK_TIM_CLK/tick_hz) - 1;

	*pSRVR &= ~(0x00FFFFFF);
	*pSRVR |= count;

	*pSCSR |= (1 << 1);
	*pSCSR |= (1 << 2);
	*pSCSR |= (1 << 0);

}

__attribute__((naked)) void scheduler_stack_init(uint32_t top_of_stack_scheduler){

	__asm volatile("MSR MSP,%0"::"r"(top_of_stack_scheduler):);
	__asm volatile("BX LR");
}


void tasks_stack_init(void){

	uint32_t *pPSP;

	for(int i = 0;i<MAX_TASKS;i++){

		pPSP = (uint32_t*) tasks_psp[i];

		pPSP--;
		*pPSP = DUYMMY_XPSR; //0x0100000

		pPSP--;
		*pPSP = handlers_of_task[i]; //0x0100000

		pPSP--;
		*pPSP = 0xFFFFFFFD; //0x00100000

		for(int j = 0;j<13;j++){

			pPSP--;
			*pPSP = 0; //0x00100000

		}

		tasks_psp[i] = (uint32_t) pPSP;

	}

}

void enable_processor_faults(void){

	uint32_t *pSHCSR = (uint32_t*)0xE000ED24;

	*pSHCSR |= ( 1 << 16); //mem manage
	*pSHCSR |= ( 1 << 17); //bus fault
	*pSHCSR |= ( 1 << 18); //usage fault

}


void psp_getting(void){

	return tasks_psp[current_task];

}

void psp_saving(uint32_t stack_addr){

	tasks_psp[current_task] = stack_addr;

}

void update_task(void){

	current_task++;
	current_task %= MAX_TASKS;

}


__attribute__((naked)) void switch_to_psp(void){

	__asm volatile ("PUSH {LR}");
	__asm volatile ("BL psp_getting");
	__asm volatile ("MSR PSP,R0");
	__asm volatile ("POP {LR}");

	__asm volatile ("MOV R0,#0X02");
	__asm volatile ("MSR CONTROL,R0");
	__asm volatile ("BX LR");


}

__attribute__((naked)) void SysTick_Handler(void){

	__asm volatile ("MRS R0,PSP");
	__asm volatile ("STMDB  R0!,{R4-R11}");

	__asm volatile ("PUSH {LR}");

	__asm volatile ("BL psp_saving");

	__asm volatile ("BL update_task");

	__asm volatile ("BL psp_getting");

	__asm volatile ("LDMIA R0!,{R4-R11}");

	__asm volatile ("MSR PSP,R0");

	__asm volatile ("POP {LR}");

	__asm volatile ("BX LR");
}

void HardFault_Handler(void)
{
	printf("Exception : Hardfault\n");
	while(1);
}


void MemManage_Handler(void)
{
	printf("Exception : MemManage\n");
	while(1);
}

void BusFault_Handler(void)
{
	printf("Exception : BusFault\n");
	while(1);
}
