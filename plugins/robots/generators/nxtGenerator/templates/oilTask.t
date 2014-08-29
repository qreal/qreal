TASK TASK_@@NAME@@
{
	AUTOSTART = @@AUTOSTART@@
	{
		APPMODE = appmode1;
	};
	PRIORITY = 1; /* lowest priority */
	ACTIVATION = 1;
	SCHEDULE = FULL;
	STACKSIZE = 512;
};
