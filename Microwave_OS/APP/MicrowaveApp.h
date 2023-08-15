/*****************************************************************/
/***********		Author: Ahmed Mohamed 			**************/
/***********		Layer: APP			 			**************/
/***********		Component: Microwave	 		**************/
/***********		Version: 1.00		 			**************/
/***********		Date: 15-8-2023		 			**************/
/*****************************************************************/


#ifndef MICROWAVEAPP_H_
#define MICROWAVEAPP_H_


#define APP_SET_TIME				0
#define APP_REMAINING_DISPLAY_TIME  1
#define APP_CLOSE_DOOR				2
#define APP_PUT_FOOD				3
#define APP_CLOSE_DOOR_PUT_FOOD     4


#define APP_STOP_HEATING			0
#define APP_START_HEATING			1


#define APP_SENSOR_OFF			    0
#define APP_SENSOR_ON			    1

#define APP_BUTTON_OFF			    1
#define APP_BUTTON_ON			    0



#define APP_HEATER_PORT					 DIO_u8PORTA
#define APP_LAMP_PORT					 DIO_u8PORTA
#define APP_MOTOR_PORT					 DIO_u8PORTA

#define APP_DOOR_SENSOR_PORT			 DIO_u8PORTA
#define APP_WEIGH_SENSOR_PORT			 DIO_u8PORTA

#define APP_HEATER_PIN					 DIO_u8PIN0
#define APP_LAMP_PIN					 DIO_u8PIN1
#define APP_MOTOR_PIN					 DIO_u8PIN2

#define APP_DOOR_SENSOR_PIN		 		 DIO_u8PIN3
#define APP_WEIGH_SENSOR_PIN	 		 DIO_u8PIN4

void APP_voidInitializeSystem(void);
void APP_voidCreateSystemTasks(void);
void APP_voidGetPressedKey(void);
void APP_voidDisplayLCD(void);
void APP_voidUpdateSensorReadings(void);
void APP_voidUpdateCurrentMode(void);
void APP_voidUpdateOutputState(void);

#endif /* MICROWAVEAPP_H_ */
