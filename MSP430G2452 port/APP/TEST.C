/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #1
*********************************************************************************************************
*/

#include "../SOURCE/INCLUDES.H"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                        10       /* Number of identical tasks                          */

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

//the stacks must be declared having a type OS_STK
//
OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
//OS_STK is 16-bit value, thus the size of stack is 1024 Bytes
OS_STK        TaskStartStk[TASK_STK_SIZE];
char          TaskData[N_TASKS];                      /* Parameters to pass to each task               */
//RandomSem is a binary semaphore handle created by calling OSSemCreate()
OS_EVENT     *RandomSem;                              

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

        void  Task(void *data);                       /* Function prototypes of tasks                  */
        void  TaskStart(void *data);                  /* Function prototypes of Startup task           */
static  void  TaskStartCreateTasks(void);

/*$PAGE*/
/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

void  main (void)
{
    //must have before in voke any other services
    //it creates two tasks: an idle task whcih excutes when no other task is ready to run
    //and a statistic task, which computes CPU usage
    OSInit();                                              /* Initialize uC/OS-II                      */

    //four parameters
    //1. pointer to the task's address
    //2. pointer to data that you want to pass to the task(task function's parameter
    //3. the task's top of stack (stack grows downwards)
    //4. the priority of this task, number from 0-62 lower the number, higher the priority
    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);

    //giving control to ucOSII, create at least one task before call this
    //OSStart select the most important task to run first, in this case TaskStart()
    OSStart();                                             /* Start multitasking                       */

    //no return in embedded system main function, nowhere return to
    return 0;
}


/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/

//*pdata is the 2nd parameter in OSTaskCreate()
void  TaskStart (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    char       s[100];
    INT16S     key;

    //fake usage of pdata
    pdata = pdata;                                         /* Prevent compiler warning                 */

    //this is called to determine the speed of your CPU, allows ucOSII to know
    //what percentage of the CPU is actually being used by all the tasks
    OSStatInit();                                          /* Initialize uC/OS-II's statistics         */

	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	P1DIR |= 0x01;					// Set P1.0 to output direction

	for(;;) {
		volatile unsigned int i;	// volatile to prevent optimization

		P1OUT ^= 0x01;				// Toggle P1.0 using exclusive-OR

		i = 10000;					// SW Delay
		do i--;
		while(i != 0);
	}
}

