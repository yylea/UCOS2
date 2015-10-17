/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #3
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define          TASK_STK_SIZE     512                /* Size of each task's stacks (# of WORDs)       */

#define          TASK_START_ID       0                /* Application tasks                             */
#define          TASK_CLK_ID         1
#define          TASK_1_ID           2
#define          TASK_2_ID           3
#define          TASK_3_ID           4
#define          TASK_4_ID           5
#define          TASK_5_ID           6

#define          TASK_START_PRIO    10                /* Application tasks priorities                  */
#define          TASK_CLK_PRIO      11
#define          TASK_1_PRIO        12
#define          TASK_2_PRIO        13
#define          TASK_3_PRIO        14
#define          TASK_4_PRIO        15
#define          TASK_5_PRIO        16

#define          MSG_QUEUE_SIZE     20                /* Size of message queue used in example         */

/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef struct {
    char    TaskName[30];                           //name to a task
    INT16U  TaskCtr;                                //keep track of how many times a task is executed
    INT16U  TaskExecTime;                           //how long a task takes to execute
    INT32U  TaskTotExecTime;                        //totoal execution time
} TASK_USER_DATA;

/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

OS_STK          TaskStartStk[TASK_STK_SIZE];          /* Startup    task stack                         */
OS_STK          TaskClkStk[TASK_STK_SIZE];            /* Clock      task stack                         */
OS_STK          Task1Stk[TASK_STK_SIZE];              /* Task #1    task stack                         */
OS_STK          Task2Stk[TASK_STK_SIZE];              /* Task #2    task stack                         */
OS_STK          Task3Stk[TASK_STK_SIZE];              /* Task #3    task stack                         */
OS_STK          Task4Stk[TASK_STK_SIZE];              /* Task #4    task stack                         */
OS_STK          Task5Stk[TASK_STK_SIZE];              /* Task #5    task stack                         */

TASK_USER_DATA  TaskUserData[7];                      //an array to hold each task's info

//like a mailbox except that message queue can hold more pointers
OS_EVENT       *MsgQueue;                             /* Message queue pointer                         */
//an array of messages (pointers) to hold the message in the queue
void           *MsgQueueTbl[20];                      /* Storage for messages                          */

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

        void  TaskStart(void *data);                  /* Function prototypes of tasks                  */
static  void  TaskStartCreateTasks(void);
static  void  TaskStartDispInit(void);
static  void  TaskStartDisp(void);
        void  TaskClk(void *data);
        void  Task1(void *data);
        void  Task2(void *data);
        void  Task3(void *data);
        void  Task4(void *data);
        void  Task5(void *data);
        void  DispTaskStat(INT8U id);

/*$PAGE*/
/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

void  main (void)
{
    PC_DispClrScr(DISP_BGND_BLACK);                        /* Clear the screen                         */

    OSInit();                                              /* Initialize uC/OS-II                      */

    PC_DOSSaveReturn();                                    /* Save environment to return to DOS        */

    PC_VectSet(uCOS, OSCtxSw);                             /* Install uC/OS-II's context switch vector */

    //Initialized elapsed time measurement function that is used to
    //measure the execution time of OSTaskStkChk()
    PC_ElapsedInit();                                      /* Initialized elapsed time measurement     */

    strcpy(TaskUserData[TASK_START_ID].TaskName, "StartTask");
    OSTaskCreateExt(TaskStart,                             //pointer to task's function
                    (void *)0,                             //the parameter to the task's function
                    &TaskStartStk[TASK_STK_SIZE - 1],      //top of the stack
                    TASK_START_PRIO,                       //priority
                    TASK_START_ID,                         //task ID
                    &TaskStartStk[0],                      //bottom of the stack
                    TASK_STK_SIZE,                         //stack size
                    &TaskUserData[TASK_START_ID],          //Task Control Block TCB can store a pointer to a user-provided data structure
                                                           //this allows to extend the functionality of UCOSII
                    0);                                    //a set of additional options
    OSStart();                                             /* Start multitasking                       */
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                               STARTUP TASK
*********************************************************************************************************
*/

void  TaskStart (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    INT16S     key;


    pdata = pdata;                                         /* Prevent compiler warning                 */

    TaskStartDispInit();                                   /* Setup the display                        */

    OS_ENTER_CRITICAL();                                   /* Install uC/OS-II's clock tick ISR        */
    PC_VectSet(0x08, OSTickISR);
    PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
    OS_EXIT_CRITICAL();

    OSStatInit();                                          /* Initialize uC/OS-II's statistics         */

    MsgQueue = OSQCreate(&MsgQueueTbl[0], MSG_QUEUE_SIZE); /* Create a message queue                   */

    TaskStartCreateTasks();

    for (;;) {
        TaskStartDisp();                                  /* Update the display                       */

        if (PC_GetKey(&key)) {                             /* See if key has been pressed              */
            if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
                PC_DOSReturn();                            /* Yes, return to DOS                       */
            }
        }

        OSCtxSwCtr = 0;                                    /* Clear the context switch counter         */
        OSTimeDly(OS_TICKS_PER_SEC);                       /* Wait one second                          */
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                      CREATE APPLICATION TASKS
*********************************************************************************************************
*/

void  TaskStartCreateTasks (void)
{
    strcpy(TaskUserData[TASK_CLK_ID].TaskName, "Clock Task");
    OSTaskCreateExt(TaskClk,
                    (void *)0,
                    &TaskClkStk[TASK_STK_SIZE - 1],
                    TASK_CLK_PRIO,
                    TASK_CLK_ID,
                    &TaskClkStk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_CLK_ID],
                    0);

    strcpy(TaskUserData[TASK_1_ID].TaskName, "MsgQ Rx Task");
    OSTaskCreateExt(Task1,
                    (void *)0,
                    &Task1Stk[TASK_STK_SIZE - 1],
                    TASK_1_PRIO,
                    TASK_1_ID,
                    &Task1Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_1_ID],
                    0);

    strcpy(TaskUserData[TASK_2_ID].TaskName, "MsgQ Tx Task #2");
    OSTaskCreateExt(Task2,
                    (void *)0,
                    &Task2Stk[TASK_STK_SIZE - 1],
                    TASK_2_PRIO,
                    TASK_2_ID,
                    &Task2Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_2_ID],
                    0);

    strcpy(TaskUserData[TASK_3_ID].TaskName, "MsgQ Tx Task #3");
    OSTaskCreateExt(Task3,
                    (void *)0,
                    &Task3Stk[TASK_STK_SIZE - 1],
                    TASK_3_PRIO,
                    TASK_3_ID,
                    &Task3Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_3_ID],
                    0);

    strcpy(TaskUserData[TASK_4_ID].TaskName, "MsgQ Tx Task #4");
    OSTaskCreateExt(Task4,
                    (void *)0,
                    &Task4Stk[TASK_STK_SIZE - 1],
                    TASK_4_PRIO,
                    TASK_4_ID,
                    &Task4Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_4_ID],
                    0);

    strcpy(TaskUserData[TASK_5_ID].TaskName, "TimeDlyTask");
    OSTaskCreateExt(Task5,
                    (void *)0,
                    &Task5Stk[TASK_STK_SIZE - 1],
                    TASK_5_PRIO,
                    TASK_5_ID,
                    &Task5Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_5_ID],
                    0);
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                        INITIALIZE THE DISPLAY
*********************************************************************************************************
*/

static  void  TaskStartDispInit (void)
{
/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
    PC_DispStr( 0,  0, "                         uC/OS-II, The Real-Time Kernel                         ", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
    PC_DispStr( 0,  1, "                                Jean J. Labrosse                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  2, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  3, "                                    EXAMPLE #3                                  ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  4, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  5, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  6, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  7, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  8, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  9, "Task Name         Counter  Exec.Time(uS)   Tot.Exec.Time(uS)  %Tot.             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 10, "----------------- -------  -------------   -----------------  -----             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 11, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 12, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 13, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 14, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 15, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 16, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 17, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 18, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 19, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 20, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 21, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 22, "#Tasks          :        CPU Usage:     %                                       ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 23, "#Task switch/sec:                                                               ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 24, "                            <-PRESS 'ESC' TO QUIT->                             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY + DISP_BLINK);
/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                           UPDATE THE DISPLAY
*********************************************************************************************************
*/

static  void  TaskStartDisp (void)
{
    char   s[80];


    sprintf(s, "%5d", OSTaskCtr);                                  /* Display #tasks running               */
    PC_DispStr(18, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

#if OS_TASK_STAT_EN > 0
    sprintf(s, "%3d", OSCPUUsage);                                 /* Display CPU usage in %               */
    PC_DispStr(36, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
#endif

    sprintf(s, "%5d", OSCtxSwCtr);                                 /* Display #context switches per second */
    PC_DispStr(18, 23, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

    sprintf(s, "V%1d.%02d", OSVersion() / 100, OSVersion() % 100); /* Display uC/OS-II's version number    */
    PC_DispStr(75, 24, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

    switch (_8087) {                                               /* Display whether FPU present          */
        case 0:
             PC_DispStr(71, 22, " NO  FPU ", DISP_FGND_YELLOW + DISP_BGND_BLUE);
             break;

        case 1:
             PC_DispStr(71, 22, " 8087 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
             break;

        case 2:
             PC_DispStr(71, 22, "80287 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
             break;

        case 3:
             PC_DispStr(71, 22, "80387 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
             break;
    }
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                               TASK #1
*********************************************************************************************************
*/

void  Task1 (void *pdata)
{
    char  *msg;
    INT8U  err;


    pdata = pdata;
    for (;;) {
        //returns the pointer to the message, MsgQueue is the queue being created using MsgQueue = OSQCreate(&MsgQueueTbl[0], MSG_QUEUE_SIZE); 
        msg = (char *)OSQPend(MsgQueue, 0, &err);                   //wait forever for a message to arrive
        PC_DispStr(70, 13, msg, DISP_FGND_YELLOW + DISP_BGND_BLUE); //display the message when it arrives
        OSTimeDlyHMSM(0, 0, 0, 100);                                //delayed for 100ms to allow you to see the message received
    }
}

/*
*********************************************************************************************************
*                                               TASK #2
*********************************************************************************************************
*/

void  Task2 (void *pdata)
{
    char  msg[20];


    pdata = pdata;
    strcpy(&msg[0], "Task 2");
    for (;;) {
        OSQPost(MsgQueue, (void *)&msg[0]);                 //post the message string "Task 2" to the queue
        OSTimeDlyHMSM(0, 0, 0, 500);                        //waits for half a second before sending another message
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                               TASK #3
*********************************************************************************************************
*/

void  Task3 (void *pdata)
{
    char  msg[20];


    pdata = pdata;
    strcpy(&msg[0], "Task 3");
    for (;;) {
        OSQPost(MsgQueue, (void *)&msg[0]);
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

/*
*********************************************************************************************************
*                                               TASK #4
*********************************************************************************************************
*/

void  Task4 (void *pdata)
{
    char  msg[20];


    pdata = pdata;
    strcpy(&msg[0], "Task 4");
    for (;;) {
        OSQPost(MsgQueue, (void *)&msg[0]);
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                               TASK #5
*********************************************************************************************************
*/

void  Task5 (void *pdata)
{
    pdata = pdata;
    for (;;) {
        OSTimeDlyHMSM(0, 0, 0, 100);            //task 5 does nothing except delaying itself for 100ms
    }
}

/*
*********************************************************************************************************
*                                               CLOCK TASK
*********************************************************************************************************
*/

void  TaskClk (void *pdata)
{
    char  s[40];


    pdata = pdata;
    for (;;) {
        PC_GetDateTime(s);
        PC_DispStr(60, 23, s, DISP_FGND_YELLOW + DISP_BGND_BLUE); //display current date and time once a 500ms
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                      DISPLAY TASK RELATED STATISTICS
*********************************************************************************************************
*/

void  DispTaskStat (INT8U id)
{
    char  s[80];


    sprintf(s, "%-18s %05u      %5u          %10ld",
            TaskUserData[id].TaskName,
            TaskUserData[id].TaskCtr,
            TaskUserData[id].TaskExecTime,
            TaskUserData[id].TaskTotExecTime);
    PC_DispStr(0, id + 11, s, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
}



//hooks functions provide a easy way to add functionality to UCOSII
//hooks are usually found in OS_CPU_C.C file
//but if define OS_CPU_HOOKS_EN to 0, you can declare the hook function in a differnt file
//OS_CPU_HOOKS_EN can be found in OS_CFG.H
/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*********************************************************************************************************
*/
void  OSInitHookBegin (void)
{
}

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*********************************************************************************************************
*/
void  OSInitHookEnd (void)
{
}

/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*********************************************************************************************************
*/
void  OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*********************************************************************************************************
*/
void  OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*********************************************************************************************************
*/
void  OSTaskIdleHook (void)
{
}

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*********************************************************************************************************
*/

//when OS_TASK_STAT_EN is enabled in OS_CFG.H, the statistic task OSTaskStat() calls the user-definable function
//OSTaskStatHook, this function is called every second
void  OSTaskStatHook (void)
{
    char    s[80];
    INT8U   i;
    INT32U  total;
    INT8U   pct;


    total = 0L;                                          /* Totalize TOT. EXEC. TIME for each task */
    for (i = 0; i < 7; i++) {
        //total exec time of all tasks
        total += TaskUserData[i].TaskTotExecTime;
        DispTaskStat(i);                                 /* Display task data                      */
    }
    if (total > 0) {
        for (i = 0; i < 7; i++) {                        /* Derive percentage of each task         */
            pct = 100 * TaskUserData[i].TaskTotExecTime / total;
            sprintf(s, "%3d %%", pct);
            PC_DispStr(62, i + 11, s, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
        }
    }
    if (total > 1000000000L) {                           /* Reset total time counters at 1 billion */
        for (i = 0; i < 7; i++) {
            TaskUserData[i].TaskTotExecTime = 0L;
        }
    }
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*********************************************************************************************************
*/
//allows us to measure the execution time of each task,
//keeps track of how often each task executes
//and accumulates total execution times of each task
//****OSTaskSwHook() is called when UCOSII switches from a low priority task to a higher priority task
void  OSTaskSwHook (void)
{
    INT16U           time;
    TASK_USER_DATA  *puser;


    time  = PC_ElapsedStop();                    /* This task is done                                  */
    PC_ElapsedStart();                           /* Start for next task                                */
    //global pointer OSTCBCur points to the TCB of the current task
    //that's the last second paramter in OSTaskCreateExt()
    puser = OSTCBCur->OSTCBExtPtr;               /* Point to used data                                 */
    //check if the pointer is not NULL
    //the two default tasks: idle task and statistic task do not contain a TCB externsion pointer
    if (puser != (TASK_USER_DATA *)0) {
        //calculate how many times this task being called
        puser->TaskCtr++;                        /* Increment task counter                             */
        puser->TaskExecTime     = time;          /* Update the task's execution time                   */
        puser->TaskTotExecTime += time;          /* Update the task's total execution time             */
    }
}

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*********************************************************************************************************
*/
void  OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}

/*
*********************************************************************************************************
*                                               TICK HOOK
*********************************************************************************************************
*/
void  OSTimeTickHook (void)
{
}
