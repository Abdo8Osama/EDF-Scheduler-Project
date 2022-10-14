/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )


#define Button1_Task_PERIOD			50
#define Button2_Task_PERIOD         50
#define Periodic_Tx_Task_PERIOD     100
#define Uart_Rx_Task_PERIOD         20
#define Load_1_Task_PERIOD          10
#define Load_2_Task_PERIOD          100
#define LED_Task_PERIOD          100

/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/

TaskHandle_t LED_Task_Handler = NULL;
TaskHandle_t Button1_Task_Handler = NULL;
TaskHandle_t Button2_Task_Handler = NULL;
TaskHandle_t Load_1_Task_Handler = NULL;
TaskHandle_t Load_2_Task_Handler = NULL;
TaskHandle_t Periodic_Tx_Task_Handler = NULL;
TaskHandle_t Uart_Rx_Task_Handler = NULL;


volatile static char  str1 = ' ' ;
volatile static char  str2 = ' ' ;
volatile static char  *strB1 = " " ;
volatile static char  *strB2 = " " ;
pinState_t curr_button1_State = PIN_IS_LOW ;
pinState_t curr_button2_State = PIN_IS_LOW ;

void LED_Task ( void * pvParameters )
{
	    for( ;; )
			{
							GPIO_write(PORT_1 , PIN8 , PIN_IS_HIGH);
							//GPIO_write(PORT_1 , PIN8 , PIN_IS_LOW);

				if(str1 == 'r')
				{
				//str1 = ' ' ;
			  GPIO_write(PORT_0 , PIN3 , PIN_IS_HIGH);
			  GPIO_write(PORT_0 , PIN4 , PIN_IS_LOW);
				}
				else if(str1 == 'f'){
				//str1 = ' ' ;
				GPIO_write(PORT_0 , PIN4 , PIN_IS_HIGH);
				GPIO_write(PORT_0 , PIN3 , PIN_IS_LOW);
				}else{
				GPIO_write(PORT_0 , PIN3 , PIN_IS_LOW);
				GPIO_write(PORT_0 , PIN4 , PIN_IS_LOW);
				}
				if(str2 == 'r'){
				//str2 = ' ' ;
				GPIO_write(PORT_0 , PIN5 , PIN_IS_HIGH);
				GPIO_write(PORT_0 , PIN6 , PIN_IS_LOW);
				}
				else if(str2 == 'f'){
				//str2 = ' ' ;
				GPIO_write(PORT_0 , PIN5 , PIN_IS_LOW);
				GPIO_write(PORT_0 , PIN6 , PIN_IS_HIGH);
				}
				else{
				GPIO_write(PORT_0 , PIN5 , PIN_IS_LOW);
				GPIO_write(PORT_0 , PIN6 , PIN_IS_LOW);
				}
					GPIO_write(PORT_1 , PIN8 , PIN_IS_LOW);
					vTaskDelay(100);
			}
}

void Button1_Task ( void * pvParameters )
{

    pinState_t prev_button1_State = PIN_IS_LOW ;

    for( ;; )
    {
						  GPIO_write(PORT_1 , PIN1 , PIN_IS_HIGH);
							//GPIO_write(PORT_1 , PIN1 , PIN_IS_LOW);

        /* Task code goes here. */
					curr_button1_State = GPIO_read(PORT_0 , PIN0 ); 
					if( curr_button1_State != prev_button1_State)
					{
								if( curr_button1_State == PIN_IS_HIGH)
								{
									/*rising Edge code*/
									str1 = 'r' ;			
								}
								else if( curr_button1_State == PIN_IS_LOW)
								{
									/*falling Edge code*/
									str1 = 'f' ;									
								}

					}
					prev_button1_State = curr_button1_State ;
					GPIO_write(PORT_1 , PIN1 , PIN_IS_LOW);

				   vTaskDelay( 50 );
    }
}

void Button2_Task ( void * pvParameters )
{
    
    pinState_t prev_button2_State = PIN_IS_LOW ;

    for( ;; )
    {
			      	GPIO_write(PORT_1 , PIN2 , PIN_IS_HIGH);
     					//GPIO_write(PORT_1 , PIN2 , PIN_IS_LOW);

        /* Task code goes here. */
					curr_button2_State = GPIO_read(PORT_0 , PIN1 ); 
					if( curr_button2_State != prev_button2_State)
					{
								if( curr_button2_State == PIN_IS_HIGH)
								{
									/*rising Edge code*/
									str2 = 'r' ;								  									
								}
								else if( curr_button2_State == PIN_IS_LOW)
								{
										/*falling Edge code*/
									  str2 = 'f' ;																	
								}

					}
					prev_button2_State = curr_button2_State ;

					GPIO_write(PORT_1 , PIN2 , PIN_IS_LOW);
					vTaskDelay( 50 );
    }
}


void Periodic_Tx_Task ( void * pvParameters )
{
    for( ;; )
    {
			GPIO_write(PORT_1 , PIN3 , PIN_IS_HIGH);
			//GPIO_write(PORT_1 , PIN3 , PIN_IS_LOW);

		if(str1 == 'r')
				{
				//str1 = ' ' ;
				strB1 = "rising" ;
				}
				else if(str1 == 'f'){
				//str1 = ' ' ;
				strB1 = "falling" ;
				}
				if(str2 == 'r'){
				//str2 = ' ' ;
				strB2 = "rising" ;
				}
				else if(str2 == 'f'){
				//str2 = ' ' ;
				strB2 = "falling" ;
				}
				else{
				}
			
			GPIO_write(PORT_1 , PIN3 , PIN_IS_LOW);
			vTaskDelay( 100 );
    }
}

void Uart_Rx_Task ( void * pvParameters )
{
	portBASE_TYPE xUart_res1;
	portBASE_TYPE xUart_res2;
    for( ;; )
    {
			GPIO_write(PORT_1 , PIN4 , PIN_IS_HIGH);
			//GPIO_write(PORT_1 , PIN4 , PIN_IS_LOW);
       
								 xUart_res1 = vSerialPutString( (const signed char * const) strB1, sizeof(strB1));
								 xUart_res2 = vSerialPutString( (const signed char * const) strB2, sizeof(strB2));

			GPIO_write(PORT_1 , PIN4 , PIN_IS_LOW);
			vTaskDelay( 20 );

    }
}



void Load_1_Task ( void * pvParameters )
{
		int i = 0 ;
	    for( ;; )
			{
				for( i = 0 ;i < 6000 ; i++ )
				{
            	GPIO_write(PORT_1 , PIN5 , PIN_IS_HIGH);

				}
				      GPIO_write(PORT_1 , PIN5 , PIN_IS_LOW);
							vTaskDelay( 10 );
			}
}

void Load_2_Task ( void * pvParameters )
{
    int i = 0 ;
	    for( ;; )
			{
				for( i = 0 ;i < 14400 ; i++ )
				{
            	GPIO_write(PORT_1 , PIN6 , PIN_IS_HIGH);

				}
				      GPIO_write(PORT_1 , PIN6 , PIN_IS_LOW);
							vTaskDelay( 100 );
			}
}



void vApplicationIdleHook (void)
{ 
	/*IdleHook function*/
	GPIO_write(PORT_1 , PIN7 , PIN_IS_HIGH);
	GPIO_write(PORT_1 , PIN7 , PIN_IS_LOW);

}


void vApplicationTickHook (void)
{ 
	/*IdleHook function*/
		GPIO_write(PORT_1 , PIN0 , PIN_IS_HIGH);
	  GPIO_write(PORT_1 , PIN0 , PIN_IS_LOW);

}



/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

	
    /* Create Tasks here */
		xTaskPeriodicCreate( LED_Task , "LED Task" , configMINIMAL_STACK_SIZE , ( void * ) 0 , 1 , &LED_Task_Handler , LED_Task_PERIOD); 
		xTaskPeriodicCreate( Button1_Task , "Button1 Task" , configMINIMAL_STACK_SIZE , ( void * ) 0 , 1 , &Button1_Task_Handler , Button1_Task_PERIOD); 
		xTaskPeriodicCreate( Button2_Task , "Button2 Task" , configMINIMAL_STACK_SIZE , ( void * ) 0 , 1 , &Button2_Task_Handler ,Button2_Task_PERIOD); 
		xTaskPeriodicCreate( Periodic_Tx_Task , "Periodi Tx Task" , configMINIMAL_STACK_SIZE , ( void * ) 0 , 1 , &Periodic_Tx_Task_Handler , Periodic_Tx_Task_PERIOD); 
		xTaskPeriodicCreate( Uart_Rx_Task , "Uart Rx Task" , configMINIMAL_STACK_SIZE , ( void * ) 0 , 1 , &Uart_Rx_Task_Handler ,Uart_Rx_Task_PERIOD); 
		xTaskPeriodicCreate( Load_1_Task , "Load 1 Task" , configMINIMAL_STACK_SIZE , ( void * ) 0 , 1 , &Load_1_Task_Handler , Load_1_Task_PERIOD); 
		xTaskPeriodicCreate( Load_2_Task , "Load 2 Task" , configMINIMAL_STACK_SIZE , ( void * ) 0 , 1 , &Load_2_Task_Handler , Load_2_Task_PERIOD); 


	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


