//https://doc.micrium.com/display/osiiidoc/Bilateral+Rendez-vous
//like a circle
Task1()
{
	for (;;)
	{
		/*
		Perform operation;
		Signal task #2;
		Wait for signal from task #2;
		Continue operation;
		*/
	}
}

Task2()
{
	for (;;)
	{
		/*
		Perform operation;
		Signal task #1;
		Wait for signal from task #1;
		Continue operation;
		*/
	}
}