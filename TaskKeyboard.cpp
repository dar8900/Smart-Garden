#include "TaskKeyboard.h"
#include "Smart-Garden.h"
#include "Keyboard.h"

#ifdef TASK_KEYBOARD
void TaskKeyboard(void *pvParameters)  // This is a task.
{
	(void) pvParameters;

	for (;;)
	{
		CheckButtons();
		OsDelay(25);
	}
}
#endif