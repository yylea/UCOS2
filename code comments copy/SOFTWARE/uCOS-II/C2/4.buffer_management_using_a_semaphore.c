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

void BufRel(BUF *ptr)
{
	/*
	Disable interrupts;
	ptr->BufNext = BufFreeList;
	BufFreeList = ptr;
	Enable interrupts;
	Release semaphore;
	*/
}