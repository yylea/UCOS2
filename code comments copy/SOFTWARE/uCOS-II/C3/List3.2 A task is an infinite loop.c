//the return type must always be declared void
//the parameter pointer is void point
//the pointer a universal vehicle used to pass your task the address of a variable, a structure or even the address of a tasks
void YourTask(void *pdata)
{
    for (;;)
    {
        /*USER CODE*/
        //call one of the UCOSII's services:
        OSFlagPend();
        OSMboxPend();
        OSMutexPend();
        OSQPend();
        OSSemPend();
        OSTaskDel(OS_PRIO_SELF);
        OSTaskSuspend(OS_PRIO_SELF);
        OSTimeDly();
        OSTimeDlyHMSM(); 
        /*USER CODE*/
    }
}
