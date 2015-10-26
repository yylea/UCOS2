void Some_uCOS_II_Service(arguments)
{
    
    //typedef unsigned short OS_CPU_SR;                /* Define size of CPU status register (PSW = 16 bits) */
    //defined in OS_CPU.H
    //when you select method 3, OS_ENTER_CRITICAL and OS_EXIT_CRITICAL always assume
    //the presence of the cpu_sr variable.
    //in other word, you need to decalre cpu_sr variable in your function
    OS_CPU_SR cpu_sr;
    
    //to enter critical section, a function provided by the compiler vendor is called to
    //obtain the current state of PSW (condition code register, processor flag, whatever called)
    cpu_sr = get_processor_psw();

    //another processor called function to disable the interrupts
    disbale_interrupts();

    /*critical section of code*/

    //process specific code to reenable the interrupts
    set_processor_psw(cpu_sr);
}

