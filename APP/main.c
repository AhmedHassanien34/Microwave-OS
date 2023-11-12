/**************************************************************************************/
/********************			Author: Ahmed Mohamed			***********************/
/********************			Layer: APP			 			***********************/
/********************			Component: Microwave_RTOS		***********************/
/********************			Version: 1.00		 			***********************/
/********************			Date: 15-8-2023	 				***********************/
/**************************************************************************************/

/********************************************************************************************/
/* Include libraries                                                                        */
/********************************************************************************************/
#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "Math_Interface.h"

#include <util/delay.h>

/********************************************************************************************/
/* Include Interface Files in MCAL                                                          */
/********************************************************************************************/
#include "TIMER_interface.h"
#include "DIO_interface.h"

/********************************************************************************************/
/* Include Interface Files in OS	                                                        */
/********************************************************************************************/
#include "RTOS_interface.h"

/********************************************************************************************/
/* Include Interface Files in HAL                                                           */
/********************************************************************************************/
#include "DELAY_Interface.h"
#include "KPD_interface.h"
#include "CLCD_interface.h"

/********************************************************************************************/
/* Include Interface File (APP)                                                          */
/********************************************************************************************/
#include "MicrowaveApp.h"


/*********************************************************** Global Variables *****************************************************************/
/*Weigh sensor status*/
volatile u8 APP_u8WeightSensorState = APP_SENSOR_OFF;

/*Door sensor status*/
volatile u8 APP_u8DoorSensorState   = APP_SENSOR_OFF;

/*Pressed key value*/
volatile u8 APP_u8PressedKey	    = 'k';

/*Current app mode*/
volatile u8 APP_u8AppMode	 = APP_SET_TIME;

/*Current app output mode*/
volatile u8 APP_u8OutputState = APP_STOP_HEATING;

/*heating time*/
volatile u32 APP_u32SetTime       =  0 ;

/*heating remaining time*/
volatile u32 APP_u32RemainingTime =  0 ;

/*blank string to clear line*/
u8  *APP_u8PtrClearDisplayedString  = "                ";

int main(void)
{
	/*System Initialization*/
	APP_voidInitializeSystem();

	/*Create system tasks*/
	APP_voidCreateSystemTasks();

	/*OS Start*/
	RTOS_voidStart();

	/*Task running in between ticks*/
	while(1);

}

void APP_voidInitializeSystem(void)
{
	/*DIO Initialization*/
	DIO_u8SetPinDirection(APP_HEATER_PORT, APP_HEATER_PIN, DIO_u8PIN_OUTPUT);
	DIO_u8SetPinDirection(APP_LAMP_PORT, APP_LAMP_PIN, DIO_u8PIN_OUTPUT);
	DIO_u8SetPinDirection(APP_MOTOR_PORT, APP_MOTOR_PIN, DIO_u8PIN_OUTPUT);

	/*sensors initialization*/
	DIO_u8SetPinDirection(APP_DOOR_SENSOR_PORT, APP_DOOR_SENSOR_PIN, DIO_u8PIN_OUTPUT);
	DIO_u8SetPinDirection(APP_WEIGH_SENSOR_PORT, APP_WEIGH_SENSOR_PIN, DIO_u8PIN_OUTPUT);

	/*Initialize Keypad*/
	KPD_voidInit();

	/*Initialize LCD*/
	CLCD_voidInit();
	CLCD_voidSendString("Set Heating Time");
}

void APP_voidCreateSystemTasks(void)
{
	RTOS_u8CreateTask(0 , 2 , APP_voidGetPressedKey , 1);
	RTOS_u8CreateTask(1 , 2 , APP_voidUpdateSensorReadings , 1);
	RTOS_u8CreateTask(2 , 2 , APP_voidUpdateCurrentMode , 2);
	RTOS_u8CreateTask(3 , 2 , APP_voidDisplayLCD , 3);
	RTOS_u8CreateTask(4 , 2 , APP_voidUpdateOutputState , 3);
}

void APP_voidGetPressedKey(void)
{
	/*get pressed key value*/
	u8 Local_u8Temp = KPD_u8GetPressedKey();

	/*any key pressed check*/
	if (Local_u8Temp != '?')
	{
		/*return pressed key value*/
		APP_u8PressedKey = Local_u8Temp;
	}
	else /*no key pressed*/
 	{
		/*return dummy value*/
		APP_u8PressedKey = 'K';

	}

}

void APP_voidDisplayLCD(void)
{
	/*Display mode check*/
	/*Display set time mode*/
	if ( APP_u8AppMode == APP_SET_TIME )
	{
		/*Display set Heating Time message in line1*/
		CLCD_voidGoToXY(0,0);
		CLCD_voidSendString(APP_u8PtrClearDisplayedString);
		CLCD_voidGoToXY(0,0);
		CLCD_voidSendString("Set Heating Time");

		/*Display pressed keys in line 2*/
		CLCD_voidGoToXY(1,0);
		CLCD_voidSendString(APP_u8PtrClearDisplayedString);
		CLCD_voidGoToXY(1,0);
		CLCD_voidSendNumber(&APP_u32SetTime);
	}

	/*Display remaining time mode*/
	else if ( APP_u8AppMode == APP_REMAINING_DISPLAY_TIME )
	{
		/*Display Remaining Time message in line1*/
		CLCD_voidGoToXY(0,0);
		CLCD_voidSendString(APP_u8PtrClearDisplayedString);
		CLCD_voidGoToXY(0,0);
		CLCD_voidSendString("Remaining Time");

		/*Display remaining heating time in line2*/
		CLCD_voidGoToXY(1,0);
		CLCD_voidSendString(APP_u8PtrClearDisplayedString);
		CLCD_voidGoToXY(1,0);
		CLCD_voidSendNumber(&APP_u32RemainingTime);
	}

	/*Display close door message*/
	else if ( APP_u8AppMode == APP_CLOSE_DOOR )
	{
		/*Display Close Door message in line1*/
		CLCD_voidClearScreen ();
		CLCD_voidGoToXY(0,0);
		CLCD_voidSendString("Close Door");

	}
	/*Display put food message*/
	else if ( APP_u8AppMode == APP_PUT_FOOD )
	{
		/*Display Put food message message in line1*/
		CLCD_voidClearScreen ();
		CLCD_voidGoToXY(0,0);
		CLCD_voidSendString("Put Food First");
	}
	/*Display put food and close door message*/
	else if ( APP_u8AppMode == APP_CLOSE_DOOR_PUT_FOOD )
	{
		/*Display CloseDoorPutFood message in line1*/
		CLCD_voidClearScreen ();
		CLCD_voidGoToXY(0,0);
		CLCD_voidSendString("CloseDoorPutFood");
	}
}

void APP_voidUpdateSensorReadings(void)
{
	/*Check pressed key*/
	if (APP_u8PressedKey == 'A')
	{
		/*Toggle Weigh sensor state*/
		APP_u8WeightSensorState ^= 1;

	}
	else if (APP_u8PressedKey == 'B')
	{
		/*Toggle Door sensor state*/
		APP_u8DoorSensorState ^= 1;
	}
}


void APP_voidUpdateCurrentMode(void)
{

	/*Time before reset count*/
	static u8 Local_u8TimeCount          = 0;
	/*number of key pressed*/
	static u8 Local_u8InputNumberCounter = 0;
	/*input time array*/
	static u8 APP_u8SetTimeArr[4]		 = {0};

	/*Increment time counter each time to update mode*/
	Local_u8TimeCount++;

	/*current mode check*/
	if (APP_u8AppMode == APP_REMAINING_DISPLAY_TIME)
	{
		/*current mode is display remaining time*/
		/*Pressed key check*/
		if (APP_u8PressedKey == 'B')
		{
			/*if Door was opened during heating*/
			/*stop heating*/
			APP_u8OutputState = APP_STOP_HEATING;
			/*Change mode to close door mode*/
			APP_u8AppMode	  = APP_CLOSE_DOOR;
			/*Save remaining time*/
			APP_u32SetTime	  = APP_u32RemainingTime;
		}
		else if ( (APP_u8PressedKey == 'C') || (APP_u32RemainingTime == 0) )
		{
			/*reset set time */
			APP_u32SetTime			   = 0;
			/*reset timer counter*/
			Local_u8TimeCount		   = 0;
			/*reset pressed key counter*/
			Local_u8InputNumberCounter = 0;

			/*stop heating*/
			APP_u8OutputState		   = APP_STOP_HEATING;
			/*Change mode to Set time mode*/
			APP_u8AppMode			   = APP_SET_TIME;

		}/*end of Pressed key check*/

		/*check time passed*/
		if (Local_u8TimeCount == 5) /*if 1 seconds passed*/
		{
			/*zero time counter to count 1 second again*/
			Local_u8TimeCount  = 0;
			/*Decrement remaining time by 1*/
			APP_u32RemainingTime--;

		}/*end of time passed check*/
	}
	else /*all other modes*/
	{
		/*Button pressed check*/
		if ( (APP_u8PressedKey >= 0) && (APP_u8PressedKey < 10) )
		{
			/*reset time count*/
			Local_u8TimeCount = 0;

			/*number was pressed*/
			APP_u8AppMode	  = APP_SET_TIME;
			/*zero time counter start time wait from beginning*/

			/*accept 4 digit number*/
			if (Local_u8InputNumberCounter < 4)
			{
				/*save pressed key in set time array*/
				APP_u8SetTimeArr[Local_u8InputNumberCounter] = APP_u8PressedKey;
				/*reset set time variable*/
				APP_u32SetTime = 0;

				/*loop inputed keys*/
				for (u8 i = 0; i < (Local_u8InputNumberCounter+1) ;i++)
				{
					/*calculate set time value*/
					u32 Local_u32Result = MATH_u32GetExponential(10,(Local_u8InputNumberCounter-i));
					APP_u32SetTime = APP_u32SetTime + ( ((u32)APP_u8SetTimeArr[i]) * Local_u32Result ) ;

				}

			}

			/*increment number of key pressed*/
			Local_u8InputNumberCounter++;

		}
		else if (APP_u8PressedKey == 'C') /*cancel button pressed*/
		{
			/*reset set time */
			APP_u32SetTime			   = 0;
			/*reset timer counter*/
			Local_u8TimeCount		   = 0;
			/*reset pressed key counter*/
			Local_u8InputNumberCounter = 0;
			/*Change mode to set time mode*/
			APP_u8AppMode			   = APP_SET_TIME;

		}
		else if (APP_u8PressedKey == 'D') /**/
		{
			/*zero time counter start time wait from beginning*/
			Local_u8TimeCount = 0;

			/*sensors check*/
			/*when time is set, food in microwave, and door is closed start heating*/
			if( (APP_u32SetTime != 0) && (APP_u8WeightSensorState == APP_SENSOR_OFF) && (APP_u8DoorSensorState == APP_SENSOR_OFF) )
			{
				/*set remaining time with set time value*/
				APP_u32RemainingTime = APP_u32SetTime;
				/*start heating process*/
				APP_u8OutputState = APP_START_HEATING;
				/*Change mode to display remaining time mode*/
				APP_u8AppMode = APP_REMAINING_DISPLAY_TIME;
			}

			/*door is opened*/
			else if( (APP_u32SetTime != 0) && (APP_u8WeightSensorState == APP_SENSOR_OFF) && (APP_u8DoorSensorState == APP_SENSOR_ON) )
			{
				/*stop heating process*/
				APP_u8OutputState = APP_STOP_HEATING;
				/*change mode to close door*/
				APP_u8AppMode	  = APP_CLOSE_DOOR;
			}

			/*there is no food*/
			else if ( (APP_u32SetTime != 0) && (APP_u8WeightSensorState == APP_SENSOR_ON) && (APP_u8DoorSensorState == APP_SENSOR_OFF) )
			{
				/*stop heating process*/
				APP_u8OutputState = APP_STOP_HEATING;
				/*change mode to put food*/
				APP_u8AppMode	  = APP_PUT_FOOD;
			}

			/*door is open and there is n food*/
			else if ( (APP_u32SetTime != 0) && (APP_u8WeightSensorState == APP_SENSOR_ON) && (APP_u8DoorSensorState == APP_SENSOR_ON) )
			{
				/*stop heating process*/
				APP_u8OutputState = APP_STOP_HEATING;
				/*change mode to close door and put food*/
				APP_u8AppMode	  = APP_CLOSE_DOOR_PUT_FOOD;
			}
		}

		/*check time passed*/
		if (Local_u8TimeCount == 50) /*if 3 seconds passed*/
		{
			/*set mode to set time*/
			APP_u8AppMode	   = APP_SET_TIME;
			/*Make display temp current mode*/
			APP_u32SetTime     = 0;
			/*reset time counter*/
			Local_u8TimeCount  = 0;
			/*reset number of pressed key counter*/
			Local_u8InputNumberCounter = 0;
		}
	}
}


void APP_voidUpdateOutputState(void)
{
	/*Set heater on or off*/
	DIO_u8SetPinValue(APP_HEATER_PORT, APP_HEATER_PIN,APP_u8OutputState);
	/*Set Motor on or off*/
	DIO_u8SetPinValue(APP_LAMP_PORT, APP_LAMP_PIN,APP_u8OutputState);
	/*Set Lamp on or off*/
	DIO_u8SetPinValue(APP_MOTOR_PORT, APP_MOTOR_PIN,APP_u8OutputState);

	/*Set door sensor indication led on or off*/
	DIO_u8SetPinValue(APP_DOOR_SENSOR_PORT, APP_DOOR_SENSOR_PIN, APP_u8DoorSensorState);
	/*Set Weight sensor indication led on or off*/
	DIO_u8SetPinValue(APP_WEIGH_SENSOR_PORT, APP_WEIGH_SENSOR_PIN, APP_u8WeightSensorState);
}
