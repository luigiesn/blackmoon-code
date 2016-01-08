#ifndef _APP_H_
#define	_APP_H_

#include "defs.h"

#define ADC_FEEDBACK_CH 1

// mode selector
#define MODE TEST
#define PID_CONTROLLER 0
#define TEST 1

void App_Boostrap(void);

void App_Init(void);

void App_Process(void);

#endif	/* _APP_H_ */

