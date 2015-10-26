//the task can delete itself upon complete.
//Note that the task is not actually delteted,
//UCOSII simply doesn't know about the task anymore,
//so the task does not run
//Also, if the task calls OSTaskDel(), the task never returns

void YourTask(void *pdata)
{
    /*USER CODE*/
    OSTaskDel(OS_PRIO_SELF);
}

//author doesn't recommend to use priorities 0,1,2,3,OS_LOWEST_PRIO-3,OS_LOWEST_PRIO-2,OS_LOWEST_PRIO-1,
//and OS_LOWEST_PRIO because he might use them in future versions of UCOSII
//do not use OS_LOWEST_PRIO becuase it is a #define constant defined in the file OS_CFG.H

//in order for UCOSII to manage your task , you must create a task by passing its address aliong 
//with other arguments to one if two functions:
//OSTaskCreate(), OSTaskCreateExt(), these two function are expalined in Chapter 4, "Task Management"

