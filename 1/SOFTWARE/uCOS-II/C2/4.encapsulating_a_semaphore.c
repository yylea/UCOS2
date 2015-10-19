//in some situations, it's better to encapsulate the semaphore
//each task would not know that it is actually acquireing a semaphore when accessing the resource

INT8U CommSendCmd(char *cmd, char*response, INT16U timeout)
{
	/*
	acquire port's semaphore;
	send command to device;
	wait for reponse(with timeout);
	if(timed out)
	{
		release semaphore;
		return(error code);
	}
	else
	{
		release semaphore;
		return (no error);
	}
	*/

}