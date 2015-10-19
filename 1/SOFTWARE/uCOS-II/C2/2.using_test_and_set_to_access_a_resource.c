//test and set operation,
//if you are not using a kernel, two functions could agree that to access a resource,
//they must check a global variable and if the variable is 0,
//the function has access to the resource
Disable interrupts;
if ('access variable' is 0)
{
	set variable to 1;
	reenable interrupts;
	access the resource;
	disable interrupts;
	set the 'access variable' back to 0;
	reenable interrupts;
}
else
{
	reenable interrupts;
	you dont have access to the resource, try back later;
}