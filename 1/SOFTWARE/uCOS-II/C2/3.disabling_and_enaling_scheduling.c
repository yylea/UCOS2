//accessing shared data by disabling and enabling scheduling
//you should avoid disabling the scheduler because it defeats the purpose of having a kernel in the first place

void Function(void)
{
	OSSchedLock();
	/*
	you can access shared data in here (interrupts are still recoginizeds)
	*/
	OSSchedUnlock();
}
