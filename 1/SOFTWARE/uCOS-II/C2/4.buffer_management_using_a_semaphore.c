BUF *BufReq(void)
{
	BUF *ptr;

	/*
	acquire a semaphore;
	disable interrupts;
	ptr = BufFreeList;
	BufFreeList = ptr->BufNext;
	Enable interrupts;
	return (ptr);
	*/
}