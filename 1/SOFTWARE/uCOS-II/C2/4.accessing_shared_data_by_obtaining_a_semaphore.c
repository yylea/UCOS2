OS_EVENT *SharedDataSem

void Function(void)
{
	INT8U err;

	OSSemPend(SharedDataSem, 0, &err);
	//you can access shared data in here (interrupts are recognized)
	OSSemPost(SharedDataSem);
}