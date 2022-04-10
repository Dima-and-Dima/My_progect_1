/*
 * Pilet_1.c
 *
 * Created: 13.06.2020 14:47:57
 *  Author: Prog
 */ 


# define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
//#include <math.h>
#include <util/twi.h>
#include "main.h"
#include <u8g2.h>
 u8g2_t u8g2;
#include "Encoder_driver.h"

#define MY_BORDER_SIZE 1
//-----------------------------------------------EEPROM  Memory  variables------------------------//
  uint16_t EEMEM eemem_flag_yes_no=0;
  
 //-----------------------------MAIN seting 1.x--//
  uint8_t EEMEM eemem_setup_boiler_temperature=40;
  uint8_t EEMEM eemem_boil_power_percent=60;
  uint8_t EEMEM eemem_m_hot_water=30;
  uint16_t EEMEM eemem_m_burner_temp=200;
 //volatile uint8_t HWS;
 //-----------------------------FAN Seting  2.x--//
  uint8_t EEMEM eemem_max_rotation_ignition=2;// 1
  uint8_t EEMEM eemem_rotation_ignition=1; //  2
  uint8_t EEMEM eemem_rotation_max_power=1;// 3
  uint8_t EEMEM eemem_rotation_min_power=1;// 4
  uint8_t EEMEM eemem_rotation_quenching=1;// 5
  uint8_t EEMEM eemem_rotation_cleaning=0;// 6
  uint8_t EEMEM eemem_time_blowing=5;// 8
  uint8_t EEMEM eemem_delay_time=1;// 9
  uint8_t EEMEM eemem_rotation_blowing=1;// 10
 //------end fan seting
 //----------------------------BUNKER Seting 3.x--//
  uint8_t EEMEM eemem_start_dose=0;// 0
  uint8_t EEMEM eemem_supply_cycle=1;// 1
  uint8_t EEMEM eemem_dose_max_power=1;// 2
  uint8_t EEMEM eemem_dose_min_power=1;// 3
  uint8_t EEMEM eemem_stoker_time_work=1;// 4
  uint8_t EEMEM eemem_stoker_time_delay=1;// 5
  uint8_t EEMEM eemem_stoker_time_continuation=1;// 6
  uint8_t EEMEM eemem_stoker_time_emptying=1;// 7
  uint8_t EEMEM eemem_stoker_fervor=20;// 8 
 //------end bunker seting
 //---------------------------------Lighter 4.x--//
  uint8_t EEMEM eemem_lighter_hysteresis=1;//0
  uint8_t EEMEM eemem_lighter_flame_delay=1;//1
  uint8_t EEMEM eemem_lighter_kindling_time=1;//2
  uint8_t EEMEM eemem_lighter_attempt_kindling=1;//3
  uint8_t EEMEM eemem_lighter_fuel_shortage=1;//4
 //volatile uint8_t lighter_ignition_stab;//5
  uint8_t EEMEM eemem_lighter_time_ignition_stab=1;//6
 //volatile uint8_t lighter_pelets_fan; //7
  uint8_t EEMEM eemem_lighter_woods_fan=0;//8
 //------end
 //--------------------------------- Cleaning mechanism 5.x--//
  uint8_t EEMEM eemem_mechanism_cleaning_work=1;
  uint8_t EEMEM eemem_mechanism_cleaning_delay=1;
 //------------------------- Pump heating seting 6.x--//
  uint8_t EEMEM eemem_pump_period_work=0;
 //------end
 //---------------------------------- Pump heating seting 7.x--//
  uint8_t EEMEM eemem_pump_hot_water_hystesis=1;//0
  uint8_t EEMEM eemem_pump_hot_water_increase=2;//1
  uint8_t EEMEM eemem_pump_hot_water_overspending=1;//2
  uint8_t EEMEM eemem_pump_hot_water_temp_work=30;//3
  uint8_t EEMEM eemem_pump_hot_water_hystesis_work=1;//4
 //------end
 //---------------------------------- Boiler seting 8.x--//
  uint8_t EEMEM eemem_boiler_min_temp=30;// 0
  uint8_t EEMEM eemem_boiler_max_temp=70;// 1
  uint8_t EEMEM eemem_boiler_top_hystes=1;// 2
  uint8_t EEMEM eemem_boiler_hyste_swich_burner=1;// 3
  uint8_t EEMEM eemem_boiler_protect=1;// 4
  uint8_t EEMEM eemem_boiler_temp_overheating=90;// 5
 //-------end
 //-----------------------------FAN2 Seting  9.x--//
 uint8_t EEMEM eemem_fan2_max_rotation_ignition=2;// 1
 uint8_t EEMEM eemem_fan2_rotation_ignition=1; //  2
 uint8_t EEMEM eemem_fan2_rotation_max_power=1;// 3
 uint8_t EEMEM eemem_fan2_rotation_min_power=1;// 4
 uint8_t EEMEM eemem_fan2_rotation_quenching=1;// 5
 uint8_t EEMEM eemem_fan2_rotation_cleaning=0;// 6
 uint8_t EEMEM eemem_fan2_time_blowing=5;// 8
 uint8_t EEMEM eemem_fan2_delay_time=1;// 9
 uint8_t EEMEM eemem_fan2_rotation_blowing=1;// 10
  
//---------------------------------------------------end--------------------------------------------//
 unsigned const char main_without_hot_water[]  = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x1f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00,
	0x00, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
	0x00, 0x04, 0x00, 0x00, 0xe0, 0x8f, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
	0xf8, 0x07, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x00, 0x20, 0x88, 0x00, 0x20,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01,
	0x20, 0x88, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0xff, 0x04, 0x00, 0x00,
	0x00, 0x3c, 0x10, 0x02, 0x20, 0x88, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
	0x81, 0x04, 0x00, 0x00, 0x00, 0x24, 0x18, 0x04, 0x20, 0x88, 0x00, 0x20,
	0x00, 0x00, 0x00, 0x00, 0x81, 0x04, 0x00, 0x00, 0x00, 0x64, 0x50, 0x04,
	0xe0, 0x8f, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x81, 0x04, 0x00, 0x00,
	0x00, 0x44, 0xfe, 0x04, 0x1c, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
	0x81, 0x04, 0x00, 0x00, 0x00, 0xc4, 0x14, 0x04, 0x22, 0x80, 0x00, 0x20,
	0x00, 0x00, 0x00, 0x00, 0x81, 0x04, 0x00, 0x00, 0x00, 0x84, 0x30, 0x06,
	0x42, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0xff, 0x04, 0x00, 0x00,
	0x00, 0x84, 0x11, 0x01, 0xa2, 0x80, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0xc2, 0x00, 0x14, 0x81, 0x00, 0x20,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0xf4, 0x3f, 0x00,
	0x08, 0x82, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00,
	0x00, 0x14, 0x08, 0x00, 0xe4, 0x84, 0xd0, 0x20, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x04, 0x00, 0x00, 0x00, 0x14, 0x08, 0x00, 0x82, 0x88, 0xa8, 0x21,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x14, 0x08, 0x00,
	0x81, 0x93, 0x54, 0x23, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00,
	0x00, 0x14, 0x08, 0x80, 0x00, 0xa0, 0xaa, 0x26, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x04, 0x00, 0x00, 0x00, 0xf4, 0x0f, 0x40, 0x4e, 0xc0, 0x55, 0x2d,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x20,
	0xa2, 0x9c, 0xaa, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00,
	0x00, 0x04, 0x00, 0x90, 0x13, 0x91, 0x55, 0x35, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x08, 0xb2, 0xaa, 0x2a,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04,
	0x04, 0x84, 0x55, 0x35, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00,
	0x00, 0x04, 0x00, 0x72, 0x02, 0x88, 0xaa, 0x2a, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x11, 0x01, 0x90, 0x55, 0x35,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x80, 0x9c,
	0x00, 0xa0, 0xaa, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00,
	0x00, 0x04, 0x40, 0x40, 0x00, 0xc0, 0x55, 0x35, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x40, 0x20, 0x00, 0x80, 0xaa, 0x2a,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x40, 0x10,
	0x00, 0x80, 0x55, 0x35, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x20, 0x08,
	0x02, 0xfe, 0xff, 0xff, 0x7f, 0x80, 0xaa, 0x2a, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x04, 0x70, 0x1c, 0x07, 0x05, 0x00, 0x00, 0x42, 0x80, 0x55, 0x35,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0xf8, 0xbe, 0x8f, 0xc4, 0x01, 0x1c,
	0x42, 0x80, 0xaa, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0xf8, 0xbe,
	0x4f, 0x04, 0x0e, 0xe0, 0x42, 0x80, 0x55, 0x35, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x04, 0x70, 0x1c, 0x27, 0x04, 0x00, 0x00, 0x42, 0x80, 0xaa, 0x2a,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x43, 0x80, 0x55, 0x35, 0x00, 0x20, 0x10, 0x08, 0x08, 0x04, 0x00, 0x00,
	0x30, 0x04, 0x00, 0x10, 0x42, 0x80, 0xaa, 0x2a, 0x00, 0x70, 0x38, 0x1c,
	0x08, 0x04, 0x00, 0x00, 0xc0, 0xff, 0x40, 0x10, 0x7e, 0x80, 0x55, 0x35,
	0x00, 0xf8, 0xff, 0xff, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x84, 0x60, 0x10,
	0x00, 0x80, 0xff, 0x3f, 0x00, 0x70, 0x38, 0x1c, 0x00, 0x05, 0x00, 0x00,
	0x00, 0x84, 0x41, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x38, 0x1c,
	0x00, 0xfd, 0xff, 0xff, 0xff, 0x07, 0xf9, 0x13, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x70, 0x38, 0x1c, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x53, 0x10,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x38, 0x1c, 0x00, 0x01, 0x00, 0x00,
	0x00, 0x00, 0xc2, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x38, 0x1c,
	0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x46, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x70, 0x38, 0x1c, 0x00, 0x01, 0x00, 0x00, 0x00, 0xf8, 0x0f, 0x03,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00,
	0x00, 0x08, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x38, 0x1c,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x20, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00 };


 const unsigned char main_with_hot_water[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x1f,
	0x00, 0x00, 0x10, 0x08, 0x00, 0xf8, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00,
	0x00, 0x80, 0x00, 0x20, 0x00, 0x00, 0x10, 0x08, 0x00, 0x04, 0x00, 0x00,
	0x00, 0x04, 0x00, 0x00, 0xe0, 0x8f, 0x00, 0x20, 0x00, 0x00, 0x10, 0xf8,
	0xff, 0x07, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x00, 0x20, 0x88, 0x00, 0x20,
	0x00, 0x00, 0x10, 0x08, 0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01,
	0x20, 0x88, 0x00, 0x20, 0x00, 0x00, 0x10, 0x08, 0xff, 0x04, 0x00, 0x00,
	0x00, 0x3c, 0x10, 0x02, 0x20, 0x88, 0x00, 0x20, 0x00, 0x00, 0xf0, 0x0f,
	0x81, 0x04, 0x00, 0x00, 0x00, 0x24, 0x18, 0x04, 0x20, 0x88, 0x00, 0x20,
	0x00, 0x00, 0x00, 0x01, 0x81, 0x04, 0x00, 0x00, 0x00, 0x64, 0x50, 0x04,
	0xe0, 0x8f, 0x00, 0x20, 0xf8, 0xff, 0xff, 0x7f, 0x81, 0x04, 0x00, 0x00,
	0x00, 0x44, 0xfe, 0x04, 0x1c, 0x80, 0x00, 0x20, 0x08, 0x00, 0x00, 0x41,
	0x81, 0x04, 0x00, 0x00, 0x00, 0xc4, 0x14, 0x04, 0x22, 0x80, 0x00, 0x20,
	0x08, 0x00, 0x00, 0x41, 0x81, 0x04, 0x00, 0x00, 0x00, 0x84, 0x30, 0x06,
	0x42, 0x80, 0x00, 0x20, 0x08, 0x00, 0x00, 0x41, 0xff, 0x04, 0x00, 0x00,
	0x00, 0x84, 0x11, 0x01, 0xa2, 0x80, 0x00, 0x20, 0x08, 0x00, 0x00, 0x41,
	0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0xc2, 0x00, 0x14, 0x81, 0x00, 0x20,
	0x08, 0x00, 0x00, 0x41, 0x08, 0x04, 0x00, 0x00, 0x00, 0xf4, 0x3f, 0x00,
	0x08, 0x82, 0x60, 0x20, 0x08, 0x00, 0x00, 0x41, 0x08, 0x04, 0x00, 0x00,
	0x00, 0x14, 0x08, 0x00, 0xe4, 0x84, 0xd0, 0x20, 0x08, 0x00, 0x00, 0x41,
	0x08, 0x04, 0x00, 0x00, 0x00, 0x14, 0x08, 0x00, 0x82, 0x88, 0xa8, 0x21,
	0x08, 0x00, 0x00, 0x41, 0x08, 0x04, 0x00, 0x00, 0x00, 0x14, 0x08, 0x00,
	0x81, 0x93, 0x54, 0x23, 0x08, 0x00, 0x00, 0x41, 0x08, 0x04, 0x00, 0x00,
	0x00, 0x14, 0x08, 0x80, 0x00, 0xa0, 0xaa, 0x26, 0x08, 0x00, 0x00, 0x41,
	0x08, 0x04, 0x00, 0x00, 0x00, 0xf4, 0x0f, 0x40, 0x4e, 0xc0, 0x55, 0x2d,
	0x08, 0x00, 0x00, 0x41, 0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x20,
	0xa2, 0x9c, 0xaa, 0x3a, 0x08, 0x00, 0x00, 0x41, 0x08, 0x04, 0x00, 0x00,
	0x00, 0x04, 0x00, 0x90, 0x13, 0x91, 0x55, 0x35, 0x08, 0x00, 0x00, 0x41,
	0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x08, 0xb2, 0xaa, 0x2a,
	0x08, 0x00, 0x00, 0x41, 0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04,
	0x04, 0x84, 0x55, 0x35, 0x08, 0xfe, 0xff, 0x41, 0x08, 0x04, 0x00, 0x00,
	0x00, 0x04, 0x00, 0x72, 0x02, 0x88, 0xaa, 0x2a, 0x08, 0x01, 0x00, 0x40,
	0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x11, 0x01, 0x90, 0x55, 0x35,
	0x08, 0xfe, 0xff, 0x41, 0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x80, 0x9c,
	0x00, 0xa0, 0xaa, 0x2a, 0x08, 0x00, 0x00, 0x42, 0x08, 0x04, 0x00, 0x00,
	0x00, 0x04, 0x40, 0x40, 0x00, 0xc0, 0x55, 0x35, 0x08, 0xfe, 0xff, 0x41,
	0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x40, 0x20, 0x00, 0x80, 0xaa, 0x2a,
	0x08, 0x01, 0x00, 0x40, 0x08, 0x04, 0x00, 0x00, 0x00, 0x04, 0x40, 0x10,
	0x00, 0x80, 0x55, 0x35, 0x08, 0xfe, 0xff, 0x41, 0x08, 0x04, 0x20, 0x08,
	0x02, 0xfe, 0xff, 0xff, 0x7f, 0x80, 0xaa, 0x2a, 0x08, 0x00, 0x00, 0x42,
	0x08, 0x04, 0x70, 0x1c, 0x07, 0x05, 0x00, 0x00, 0x42, 0x80, 0x55, 0x35,
	0xe8, 0xff, 0xff, 0x41, 0x08, 0x04, 0xf8, 0xbe, 0x8f, 0xc4, 0x01, 0x1c,
	0x42, 0x80, 0xaa, 0x2a, 0x28, 0x00, 0x00, 0x40, 0x08, 0x04, 0xf8, 0xbe,
	0x4f, 0x04, 0x0e, 0xe0, 0x42, 0x80, 0x55, 0x35, 0xf8, 0xff, 0xff, 0x7f,
	0x08, 0x04, 0x70, 0x1c, 0x27, 0x04, 0x00, 0x00, 0x42, 0x80, 0xaa, 0x2a,
	0x20, 0x00, 0x00, 0x00, 0x08, 0x04, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x43, 0x80, 0x55, 0x35, 0x20, 0x20, 0x10, 0x08, 0x08, 0x04, 0x00, 0x00,
	0x30, 0x04, 0x00, 0x10, 0x42, 0x80, 0xaa, 0x2a, 0x20, 0x70, 0x38, 0x1c,
	0x08, 0x04, 0x00, 0x00, 0xc0, 0xff, 0x40, 0x10, 0x7e, 0x80, 0x55, 0x35,
	0x20, 0xf8, 0xff, 0xff, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x84, 0x60, 0x10,
	0x00, 0x80, 0xff, 0x3f, 0x20, 0x70, 0x38, 0x1c, 0x00, 0x05, 0x00, 0x00,
	0x00, 0x84, 0x41, 0x11, 0x00, 0x00, 0x00, 0x00, 0x20, 0x70, 0x38, 0x1c,
	0x00, 0xfd, 0xff, 0xff, 0xff, 0x07, 0xf9, 0x13, 0x00, 0x00, 0x00, 0x00,
	0x20, 0x70, 0x38, 0x1c, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x53, 0x10,
	0x00, 0x00, 0x00, 0x00, 0x20, 0x70, 0x38, 0x1c, 0x00, 0x01, 0x00, 0x00,
	0x00, 0x00, 0xc2, 0x18, 0x00, 0x00, 0x00, 0x00, 0x20, 0x70, 0x38, 0x1c,
	0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x46, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x20, 0x70, 0x38, 0x1c, 0x00, 0x01, 0x00, 0x00, 0x00, 0xf8, 0x0f, 0x03,
	0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00,
	0x00, 0x08, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x38, 0x1c,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x20, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00 };








//-----------------------------------------------------------------------------end--------------------------------------//
void WriteValueToLCD(uint8_t val, int8_t x, int8_t y);
uint8_t Convert_ADC_To_Celsium(uint16_t adc);
void Init_All_Port(void);
void Init_ADC(void);
void Boiler_Temperature(void);
void Main_(void);
void Boiler_Power_Percent(void);
void Fan_Setup(void);
void Namber_Setup(char* str, volatile uint8_t*);
void Init_ALL_Variables(void);

#define DISPLAY_CLK_DIR DDRB
#define DISPLAY_CLK_PORT PORTB
#define DISPLAY_CLK_PIN 1

#define DISPLAY_DATA_DIR DDRB
#define DISPLAY_DATA_PORT PORTB
#define DISPLAY_DATA_PIN 2

#define DISPLAY_CS_DIR DDRD
#define DISPLAY_CS_PORT PORTD
#define DISPLAY_CS_PIN 5

#define DISPLAY_DC_DIR DDRD
#define DISPLAY_DC_PORT PORTD
#define DISPLAY_DC_PIN 6

#define DISPLAY_RESET_DIR DDRB
#define DISPLAY_RESET_PORT PORTB
#define DISPLAY_RESET_PIN 0

#define To_LCD_main_4_width 128
#define To_LCD_main_4_height 64


void TIMER2_Init(void);
void TIMER16_3_Init(void);
void TIMER16_1_Init(void);
void Timer_0_Init(void);

void TIM16_WriteTCNT3( unsigned int i );
void TIM16_WriteOCR3A(uint16_t);

const int8_t mas_d[]={'0','1','2','3','4','5','6','7','8','9'};
	
 uint16_t R=0;
 double V_out=0;
 uint8_t R_temperature_1;
 double double_koeficient=0;
 uint8_t int_koeficient=0;
 uint8_t t_celcium=0;
 
 
 u8g2_uint_t yy;
 u8g2_uint_t line_height;
 uint8_t title_lines;
 
 
 
enum enumChannel  //  Select input chanel ADC
{
	KOTEL,
	BURNER,
	BOILER,
	SHNEK
}eChanel; 
 	

ISR(INT6_vect)
{
	cli();
	TIMSK3|=(1<<OCIE3A);
	TCCR3B|=(1<<CS32);//|(1<<CS30); // 16 Mhz  / 256 prescal
	OCR3A=fan_glob_val*12;
	//_delay_ms(1);
	//OCR3A=875;
	TCNT3H=0;
	TCNT3L=0;
	sei();
}
ISR(TIMER1_COMPA_vect)// Added  22-07-2020 !!!!!!!!!!!!!! WORK case
{
   cli();	
    switch(eSTART_STOP)
    {
	  case STOP:
	  {
		  if ((kotel_temperature>=boiler_temp_overheating)||(flag_alarm&(1<<BOILER_SENSOR_ALARM)))//
		  {
			  CO_ON;
			  PORTC&=~(1<<PC3);
		  }
		  else
		  {
		      CO_OFF;
			  PORTC|=(1<<PC3);
		  }
		 
		  break;
	  }
	  case START:
	  {
	   
		switch (eWork_Mode)
		{
		   case W_MONITORING://1
		   {
			  //------------------------------------------------------------------------------------------//
			  /*
			  if(flag_yes_no&(1<<SELECT_CWU_DEF))
			  {
				  if (boiler_temperature<m_hot_water)
				  {
					  
				  }
			  }
			  */
			  if (kotel_temperature<=boiler_min_temp)//--------------------------------------- if temp kotel < temp min_kotel. Go to IGNITION------//
			  {
				  eWork_Mode=W_IGNITION;
			  }
			  
			  if(flag_yes_no&(1<<SELECT_CWU_DEF))
			  {
				  if (boiler_temperature<=m_hot_water&&boiler_temperature>kotel_temperature)//  temperatura gwu < setup temperature and t. gwu > temp. kotla
				  {
					 CWU_ON; 
				  }
				  else if (boiler_temperature>=m_hot_water)
				  {
					  CWU_OFF;
				  }
			  }
			  
			  if (kotel_temperature>=boiler_temp_overheating)//---------------------------------Alarm owerheating----------------------------------//
			  {
				  CO_ON;
				  PORTC&=~(1<<PC3);
				  eSTART_STOP=STOP;
				  eWork_Mode=W_MONITORING;
			  }
			  
			 break;
		   }
		   case W_IGNITION://2
		   {
			   
			   break;
		   }
		   case W_IGNITION_STABILIZ://3
		   {
			   
			   break;
		   }
		   case W_MAX_POWER://4
		   {
			   
			   break;
		   }
		   case W_MODULATED_POWER://5
		   {
			   
			   break;
		   }
		   case W_MIN_POWER://6
		   {
			   
			   break;
		   }
		   case W_BLOWING://7
		   {
			   
			   break;
		   }
		   case W_AFTERBURNING://8
		   {
			   
			   break;
		   }
		   case W_ATTENUATION://9
		   {
			   
			   break;
		   }
		   case W_CLEANING://10
		   {
			   
			   break;
		   }
		   case W_FILLING://11
		   {
			   
			   break;
		   }
		   case W_EXTINGUISHING://12
		   {
			   
			   break;
		   }
		   
		}
		
		break;  
	  }
  }
  TCCR1B|=(1<<CS12);//|(1<<CS30); // 16 Mhz  / 256 prescal
  TCNT1H=0;
  TCNT1L=0;
  TIMSK1|=(1<<OCIE1A);
  OCR1AH=0x01;
  OCR1AL=0xF4; //1 milisec
  sei();
}
ISR(TIMER3_COMPA_vect)
{
	cli();
	TIMSK3&=~(1<<OCIE3A);
	TCCR3B&=~((1<<CS32));//|(1<<CS30)); // 16 Mhz  / 256 prescal
	PORTA&=~(1<<7);
	_delay_us(100);
	PORTA|=(1<<7);
	sei();
}

ISR(ADC_vect)
{
	cli();
	ADCSRA |=(1<<ADIF);
	switch (eChanel)
	{
		case KOTEL:
		   {
			 template_temperature_all=kotel_temperature;
			 kotel_temperature=Convert_ADC_To_Celsium(ADC);
			 eChanel=BURNER;
			 permission_read=TRUE;
			 if(template_temperature_all!=kotel_temperature)
			    eUpdate=UPDATE;
			 if(kotel_temperature==0xFF)
			   {
			     //kotel_temperature=-1;
				 flag_alarm|=(1<<KOTEL_SENSOR_ALARM);
			   }
			   else
			    flag_alarm&=~(1<<KOTEL_SENSOR_ALARM);
			 break;  
		   }
		case BURNER:
		  {
			  
			template_temperature_all=burner_temperature;  
			burner_temperature=Convert_ADC_To_Celsium(ADC);
			if(flag_yes_no&(1<<SELECT_CWU_DEF))
			{
			   eChanel=BOILER;
			}
			else
			   eChanel=SHNEK;
			permission_read=TRUE;
			if(template_temperature_all!=burner_temperature)
			   eUpdate=UPDATE;
			if(burner_temperature==0xFF)
			{
				//burner_temperature=-1;
				flag_alarm|=(1<<BURNER_SENSOR_ALARM);
			}
			else
			flag_alarm&=~(1<<BURNER_SENSOR_ALARM);   
			break;
	   	  }	 
		case BOILER:
		  {
			  template_temperature_all=boiler_temperature;
			  boiler_temperature=Convert_ADC_To_Celsium(ADC);
			  eChanel=SHNEK;
			  permission_read=TRUE;
			  if(template_temperature_all!=boiler_temperature)
			    eUpdate=UPDATE;
			  if(boiler_temperature==0xFF)
			  {
				  //boiler_temperature=-1;
				  flag_alarm|=(1<<BOILER_SENSOR_ALARM);
			  }
			  else
			  flag_alarm&=~(1<<BOILER_SENSOR_ALARM);	 	
			  break;
		  }
		  case SHNEK:
		  {
			  template_temperature_all=shnek_temperature;
			  shnek_temperature=Convert_ADC_To_Celsium(ADC);
			  eChanel=KOTEL;
			  permission_read=TRUE;
			  if(template_temperature_all!=shnek_temperature)
			  eUpdate=UPDATE;
			 // if(shnek_temperature==0xFF)
			  //{
				  //shnek_temperature=-1;
				//  flag_alarm|=(1<<SHNEK_SENSOR_ALARM);
			//  }
			//  else
			//  flag_alarm&=~(1<<SHNEK_SENSOR_ALARM);
			  break;
		  }
		  	
		  
	}
	 
	
	sei();
}
#define P_CPU_NS (1000000000UL / F_CPU)



uint8_t u8x8_avr_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	uint8_t cycles;

	switch(msg)
	{
		case U8X8_MSG_DELAY_NANO:     // delay arg_int * 1 nano second
		// At 20Mhz, each cycle is 50ns, the call itself is slower.
		break;
		case U8X8_MSG_DELAY_100NANO:    // delay arg_int * 100 nano seconds
		// Approximate best case values...
		#define CALL_CYCLES 26UL
		#define CALC_CYCLES 4UL
		#define RETURN_CYCLES 4UL
		#define CYCLES_PER_LOOP 4UL

		cycles = (100UL * arg_int) / (P_CPU_NS * CYCLES_PER_LOOP);

		if(cycles > CALL_CYCLES + RETURN_CYCLES + CALC_CYCLES)
		break;

		__asm__ __volatile__ (
		"1: sbiw %0,1" "\n\t" // 2 cycles
		"brne 1b" : "=w" (cycles) : "0" (cycles) // 2 cycles
		);
		break;
		case U8X8_MSG_DELAY_10MICRO:    // delay arg_int * 10 micro seconds
		for(int i=0 ; i < arg_int ; i++)
		_delay_us(10);
		break;
		case U8X8_MSG_DELAY_MILLI:      // delay arg_int * 1 milli second
		for(int i=0 ; i < arg_int ; i++)
		_delay_ms(1);
		break;
		default:
		return 0;
	}
	return 1;
}


uint8_t u8x8_avr_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	// Re-use library for delays

	switch(msg)
	{
		case U8X8_MSG_GPIO_AND_DELAY_INIT:  // called once during init phase of u8g2/u8x8
		DISPLAY_CLK_DIR |= 1<<DISPLAY_CLK_PIN;
		DISPLAY_DATA_DIR |= 1<<DISPLAY_DATA_PIN;
		DISPLAY_CS_DIR |= 1<<DISPLAY_CS_PIN;
		DISPLAY_DC_DIR |= 1<<DISPLAY_DC_PIN;
		DISPLAY_RESET_DIR |= 1<<DISPLAY_RESET_PIN;
		break;              // can be used to setup pins
		case U8X8_MSG_GPIO_SPI_CLOCK:        // Clock pin: Output level in arg_int
		if(arg_int)
		DISPLAY_CLK_PORT |= (1<<DISPLAY_CLK_PIN);
		else
		DISPLAY_CLK_PORT &= ~(1<<DISPLAY_CLK_PIN);
		break;
		case U8X8_MSG_GPIO_SPI_DATA:        // MOSI pin: Output level in arg_int
		if(arg_int)
		DISPLAY_DATA_PORT |= (1<<DISPLAY_DATA_PIN);
		else
		DISPLAY_DATA_PORT &= ~(1<<DISPLAY_DATA_PIN);
		break;
		case U8X8_MSG_GPIO_CS:        // CS (chip select) pin: Output level in arg_int
		if(arg_int)
		DISPLAY_CS_PORT |= (1<<DISPLAY_CS_PIN);
		else
		DISPLAY_CS_PORT &= ~(1<<DISPLAY_CS_PIN);
		break;
		case U8X8_MSG_GPIO_DC:        // DC (data/cmd, A0, register select) pin: Output level in arg_int
		if(arg_int)
		DISPLAY_DC_PORT |= (1<<DISPLAY_DC_PIN);
		else
		DISPLAY_DC_PORT &= ~(1<<DISPLAY_DC_PIN);
		break;
		
		case U8X8_MSG_GPIO_RESET:     // Reset pin: Output level in arg_int
		if(arg_int)
		DISPLAY_RESET_PORT |= (1<<DISPLAY_RESET_PIN);
		else
		DISPLAY_RESET_PORT &= ~(1<<DISPLAY_RESET_PIN);
		break;
		default:
		if (u8x8_avr_delay(u8x8, msg, arg_int, arg_ptr))	// check for any delay msgs
		return 1;
		u8x8_SetGPIOResult(u8x8, 1);      // default return value
		break;
	}
	return 1;
}


int main(void)
{
	fan_glob_val=35;
	count_glob_val=1;
	eUpdate=UPDATE;
	eWindow=MAIN;
	eFuel_Type=PELLETS;
	//f_clear_lcd=1;
	f_timer_0=0;
	//f_selection_list=0;
	f_curent_pos=0;
	kotel_temperature=0;
	burner_temperature=0;
	boiler_temperature=0;
	flag_alarm=0;
	permission_read=TRUE;
	eDirection=NOT_MOVE;
	Encoder_Init();
	Init_ADC();
	Init_ALL_Variables();
	Init_All_Port();
	//--------------------------------------------------------INT6__----------------------------------------------------
	DDRE&=~(1<<PE6);//INT6   
	PORTE&=~(1<<PE6);
	EICRB|=((1<<ISC61)|(1<<ISC60));
	EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
	f_ecoder_button=0;
	TIMER2_Init();
	TIMER16_3_Init();
	TIMER16_1_Init();  
	////-----------------------------------------------------------------//
	PORTD&=~((1<<PD0));
	sei();  
	u8g2_Setup_st7567_enh_dg128064i_f( &u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_avr_gpio_and_delay );
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	
	// full buffer example, setup procedure ends in _f
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_t0_11_tr);
	
	//u8g2_SetFont(&u8g2, u8g2_font_t0_12b_tf);
	u8g2_SetContrast(&u8g2, 170);
	                                                                // WHILE //
	while(1)
	{
		
		 if(eWindow==MAIN) 
		  {
			 Main_();	
	       }
		   
		   
		   		
		 // ----------------------------------------------------Selection list  !!!!!!!! ---------------------------------------------------//
		 else if(eWindow==SETUP)  
		 {
			 u8g2_ClearBuffer(&u8g2);
			 u8g2_SelectionList(&u8g2,"Seting",f_curent_pos+1, "1 Main\n2 Fan 1\n3 Bunker\n4 Lighter\n5 Clean\n6 Heating\n7 Hot water\n8 Boiler\n9 Fan 2");
		 }
		 //------------------------------------------end
		 //------------------------------------------------------ Main Setup 1.0------------------------------------------------------------//
		  else if (eWindow==MAIN_SETING)       // Seting list -> Main seting
		  {
			  u8g2_ClearBuffer(&u8g2);
			  u8g2_SelectionList(&u8g2, "Main", f_curent_pos+1,"1.1 Fuel type\n1.2 Boiler temperature\n1.3 Boiler power\n1.4 Season\n1.5 Hot water\n1.6 Burner off\n1.7 Select CWU\n1.8 Default" );
		  }
		 //-------------------------------------------------------Fan Setup 2.0 -------------------------------------------------------------//
		 else if (eWindow==FAN_SETUP)
		 {
			 u8g2_ClearBuffer(&u8g2);
			 u8g2_SelectionList(&u8g2,"Fun",f_curent_pos+1, "2.1 Modulation\n2.2 Ignition\n2.3 Ign. fan rot.\n2.4 Max power\n2.5 Min power\n2.6 Quenching\n2.7 Cleaning\n2.8 Blowing\n2.9 Blowing time\n2.10 Break blowing\n2.11 Blowing rotation ");
		 }
		 //------------------------------------------end
		 //-------------------------------------------------------Bunker Setup 3.0 ----------------------------------------------------------//
		 else if (eWindow==BUNKER_SETUP)
		 {
			 u8g2_ClearBuffer(&u8g2);
			 u8g2_SelectionList(&u8g2,"Bunker",f_curent_pos+1, "3.1 S Dose\n3.2 S Cycle\n3.3 Dose max power\n3.4 Dose min power\n3.5 S Time work\n3.6 S delay\n3.7 S Continuation\n3.8 S Emptying\n3.9 S Fervor");
		 }
		 //------------------------------------------end
		 //-------------------------------------------------------Lighter 4.0 ---------------------------------------------------------------//
		 else if (eWindow==LIGHTER_SETUP)
		 {
			 u8g2_ClearBuffer(&u8g2);
			 u8g2_SelectionList(&u8g2,"Lighter",f_curent_pos+1, "4.1 Hysteresis\n4.2 Flame delay\n4.3 Kindling time\n4.4 Attempt kind. \n4.5 Fuel shortage\n4.6 Ignition stab\n4.7 Time ign. stab\n4.8 Pelet fan\n4.9 Wood fan");
		 }
		 //------------------------------------------end
		  //-------------------------------------------------------Clean mechanism 5.0 -------------------------------------------------------//
		  else if (eWindow==CLEAN_MECHANISM_SETUP)
		  {
			  u8g2_ClearBuffer(&u8g2);
			  u8g2_SelectionList(&u8g2,"Cleaning",f_curent_pos+1, "5.1 Time clean\n5.2 Time delay");
		  }
		 //------------------------------------------end
          //------------------------------------------------------- Pump_Heating 6.0 -------------------------------------------------------------//
		  else if (eWindow==PUMP_HEATING_SETUP)
		  {
			  u8g2_ClearBuffer(&u8g2);
			  u8g2_SelectionList(&u8g2,"Pump heating",f_curent_pos+1, "6.1 Mode");
		  }
		 //------------------------------------------end 
		 //------------------------------------------------------- Pump hot water 7.0 ------------------------------------------------------------//
		 else if (eWindow==PUMP_HOT_WATER_SETUP)
		 {
			 u8g2_ClearBuffer(&u8g2);
			 u8g2_SelectionList(&u8g2,"Hot water",f_curent_pos+1, "7.1 Hysterises\n7.2 Increase\n7.3 Overspending\n7.4 Temp work\n7.5 Hyster. work");
		 }
		 //------------------------------------------end
	     //------------------------------------------------------- Boiler 8.0 -------------------------------------------------------------------//
	     else if (eWindow==BOILER_SETUP)
	     {
		     u8g2_ClearBuffer(&u8g2);
		     u8g2_SelectionList(&u8g2,"Boiler",f_curent_pos+1, "8.1 Min temp.\n8.2 Max temp.\n8.3 Top hyster.\n8.4 Hys. swich\n8.5 Protect\n8.6 Overheating\n8.7 Termo workMPB\n8.8 Termo work");
	     }
	     //------------------------------------------end
		 //------------------------------------------------------- Fan Setup 9.0 ----------------------------------------------------------------//
		 else if (eWindow==FAN_2_SETUP)
		 {
			 u8g2_ClearBuffer(&u8g2);
			 u8g2_SelectionList(&u8g2,"Fun",f_curent_pos+1, "9.1 Yes,No\n9.2 Modulation\n9.3 Ignition\n9.4 Ign. fan rot.\n9.5 Max power\n9.6 Min power\n9.7 Quenching\n9.8 Cleaning\n9.9 Blowing\n9.10 Blowing time\n9.11 Break blowing\n9.12 Blowing rotation ");
		 }
		 //------------------------------------------end
		 //---------------------------------------Main Seting  1 ---------------------------//
		 else if (eWindow==FUEL_TYPE)
		 { 
			 if (flag_yes_no&(1<<FUEL_DEF))//(eFuel_Type==PELLETS)
			 {
				 f_curent_pos=1;
			 }
			 else
			    f_curent_pos=0;
			 u8g2_SelectionList(&u8g2,"Fuel Type",f_curent_pos+1, "Pillets\nWoods");
			 f_curent_pos=0;
		 }
		 
		 else if (eWindow==BOIL_TEMPERATURE)
		 {
		    //Boiler_Temperature();
			Namber_Setup("Boiler Temperatur",&setup_boiler_temperature);

		 } 
		 else if (eWindow==BOILER_POWER)
		 {
			 // Boiler_Power_Percent();
			 Namber_Setup("Boiler Power",&boil_power_percent);
		 }
		 else if (eWindow==SEASON)
		 {
			  if (flag_yes_no&(1<<SESON_DEF)) //(eSeason==SUMER)
			  {
				  f_curent_pos=1;
			  }
			  else
			    f_curent_pos=0;
				
				
			  u8g2_SelectionList(&u8g2,"PC Season",f_curent_pos+1, "WINTER\nSUMMER");
			  f_curent_pos=0;
		 }
		 else if (eWindow==M_HOT_WATER)
		 {
			 // Boiler_Power_Percent();
			 Namber_Setup("Hot water T",&m_hot_water);
		 }
		 else if (eWindow==M_BURNER_TEMP)
		 {
			  //Boiler_Power_Percent();
			 Namber_Setup("Burner temp.",&m_burner_temp);
		 }
		 else if (eWindow==M_SELECT_CWU)
		 {
			 if (flag_yes_no&(1<<SELECT_CWU_DEF)) //
			 {
				 f_curent_pos=1;
			 }
			 else
			 f_curent_pos=0;
			 
			 u8g2_SelectionList(&u8g2,"Select CWU",f_curent_pos+1, "NO\nYES");
			 f_curent_pos=0;
		 }
		 else if (eWindow==M_DEFAULT)
		 {
			 if (flag_yes_no&(1<<DEFINE_DEF)) //
			 {
				 f_curent_pos=1;
			 }
			 else
			 f_curent_pos=0;
			 
			 u8g2_SelectionList(&u8g2,"Select CWU",f_curent_pos+1, "NO\nYES");
			 f_curent_pos=0;
		 }
		 //--------------------------------------------------------------FAN setup 2.0--------------------------------------//
		 else if (eWindow==MODULATION) // 0
		 {
			if (flag_yes_no&(1<<MODULATION_DEF)) //(eSeason==SUMER)
			{
				f_curent_pos=1;
			}
			else
			f_curent_pos=0;
			
			u8g2_SelectionList(&u8g2,"Modulation ignition",f_curent_pos+1, "NO\nYES");
			f_curent_pos=0;
		 }
		 
		   else if(eWindow==HEATING)// Ignition // 1
		   {
			 Namber_Setup("Max rotat Ignition ",&max_rotation_ignition);
		   }
		    else if(eWindow==HEATING_FAN_ROTATION)// 2
		    {
			    Namber_Setup("Rotation ign.",&rotation_ignition);
		    }
		    else if(eWindow==MAX_POWER)// 3
		    {
			  Namber_Setup("Max rotat Power",&rotation_max_power);
		    }
		     else if(eWindow==MIN_POWER)//  4
		     {
			   Namber_Setup("Min rotation Power",&rotation_min_power);
		     }
		      else if(eWindow==QUENCHING)//  5
		      {
			    Namber_Setup("Quenching rotation",&rotation_quenching);
		      }
			  else if(eWindow==CLEANING)//  6
			  {
				Namber_Setup("Cleaning rotation",&rotation_cleaning);
			  }
			 else if (eWindow==BLOWING)
			 {
				 if (flag_yes_no&(1<<BLOWING_DEF)) // 7
				 {
					 f_curent_pos=1;
				 }
				 else
				 f_curent_pos=0;
				 
				 u8g2_SelectionList(&u8g2,"Blowing",f_curent_pos+1, "NO\nYES");
				 f_curent_pos=0;
			  } 
			 else if(eWindow==BLOWING_TIME)// 8
			 {
				 Namber_Setup("Blowing break",&time_blowing);
			 }
			 else if(eWindow==BLOWING_BREAK_TIME)// 9
			 {
				 Namber_Setup("Blowing break",&delay_time);
			 }
			 else if(eWindow==BLOWING_ROTATION)// 10
			 {
				 Namber_Setup("Blowing rotation",&rotation_blowing);
			 }
			 //----------------------------------------------------------Bunker setup 3.0 ------------------------------------//
		
			 else if(eWindow==START_DOSE)// Ignition  0
			 {
				 Namber_Setup("Start dose",&start_dose);
			 }
			 else if(eWindow==SUPPLY_CYCLE)//  1
			 {
				 Namber_Setup("Supply cycle",&supply_cycle);
			 }
			 else if(eWindow==DOSE_MAX_POWER)// 2
			 {
				 Namber_Setup("Dose max pow",&dose_max_power);
			 }
			 else if(eWindow==DOSE_MIN_POWER)// 3
			 {
				 Namber_Setup("Dose min pow",&dose_min_power);
			 }
			 else if(eWindow==STOKER_TIME_WORK)// 4
			 {
				 Namber_Setup("Stok time work",&stoker_time_work);
			 }
			 else if (eWindow==STOKER_TIME_DELAY)// 5
			 {
				 Namber_Setup("Stok work",&stoker_time_delay);
			 }
			 else if(eWindow==STOKER_TIME_CONTINUATION)// 6
			 {
				 Namber_Setup("Stok continuation",&stoker_time_continuation);
			 }
			  else if(eWindow==STOKER_TIME_EMPTYING)// 7
			  {
				  Namber_Setup("Stok continuation",&stoker_time_emptying);
			  }
			 else if(eWindow==STOKER_FERVOR)// 8
			 {
				 Namber_Setup("Stok ferver",&stoker_fervor);
			 }	 
			 //----------------------------------------------------------Lighter setup 4.0 ------------------------------------//
			 
			 else if(eWindow==HYSTERESIS)//   0
			 {
				 Namber_Setup("Hysteresis",&lighter_hysteresis);
			 }
			 else if(eWindow==FLAME_DELAY)//  1
			 {
				 Namber_Setup("Flame delay",&lighter_flame_delay);
			 }
			 else if(eWindow==KINDLING_TIME)// 2
			 {
				 Namber_Setup("Kindling time",&lighter_kindling_time);
			 }
			 else if(eWindow==ATTEMPT_KINDLING)// 3
			 {
				 Namber_Setup("Attem. kindl.",&lighter_attempt_kindling);
			 }
			 else if(eWindow==FUEL_SHORTAGE)// 4
			 {
				 Namber_Setup("Fuel shortage",&lighter_fuel_shortage);
			 }
			 else if (eWindow==IGNITION_STAB)// 5
			 {
				 if (flag_yes_no&(1<<IGNITION_STAB_DEF)) //
				 {
					 f_curent_pos=1;
				 }
				 else
				 f_curent_pos=0;
				 
				 u8g2_SelectionList(&u8g2,"Ignit. stab",f_curent_pos+1, "YES\nNO");
				 f_curent_pos=0;
			 }
			 else if(eWindow==TIME_IGNITION_STAB)// 6
			 {
				 Namber_Setup("Time ign. stab",&lighter_time_ignition_stab);
			 }
			 else if(eWindow==PELETS_FAN)// 7
			 {
				 if (flag_yes_no&(1<<PELETS_FAN_DEF)) //
				 {
					 f_curent_pos=1;
				 }
				 else
				 f_curent_pos=0;
				 
				 u8g2_SelectionList(&u8g2,"Pelet fan",f_curent_pos+1, "FAN1\nFAN1+FAN2");
				 f_curent_pos=0;
			 }
			 else if(eWindow==WOODS_FAN)// 8
			 {
				 u8g2_SelectionList(&u8g2,"Wood fan",f_curent_pos+1, "FAN1\nFAN2\nFAN1+FAN2");
			 }
			 //--------------------------------------------------------- Clean mechanism 5.0 -------------------------------//
			 else if(eWindow==MECHA_CLEAN_WORK)//   0
			 {
				 Namber_Setup("Clean time",&mechanism_cleaning_work);
			 }
			 else if(eWindow==MECHA_CLEAN_DELAY)//  1
			 {
				 Namber_Setup("Delay time",&mechanism_cleaning_delay);
			 }
			 //--------------------------------------------------------- Pump_HEATING seting 6.0 -------------------------------//
			 else if(eWindow==PUMP_HEATING_MODE)//   0
			 {
				  if (flag_yes_no&(1<<PUMP_HEATING_DEF)) 
				  {
					  f_curent_pos=1;
				  }
				  else
				  f_curent_pos=0;
				  
				  u8g2_SelectionList(&u8g2,"PUMP HEATING",f_curent_pos+1, "TERMO\nAUTO");
				  f_curent_pos=0;
			 }
			//--------------------------------------------------------- Pump hot water 7.0 -------------------------------//
			 else if(eWindow==PUMP_HOT_WATER_HYSTESIS)//   0
			 {
				 Namber_Setup("Hysterises",&pump_hot_water_hystesis);
			 }
			 else if(eWindow==PUMP_HOT_WATER_INCREASE)//   1
			 {
				 Namber_Setup("Increase",&pump_hot_water_increase);
			 }
			 else if(eWindow==PUMP_HOT_WATER_OVERSPENDING)//   2
			 {
				 Namber_Setup("Hysterises",&pump_hot_water_overspending);
			 }
			 else if(eWindow==PUMP_HOT_WATER_TEMP_WORK)//  3
			 {
				 Namber_Setup("Temp. work",&pump_hot_water_temp_work);
			 }
			 else if(eWindow==PUMP_HOT_WATER_HYSTESIS_WORK)//   4
			 {
				 Namber_Setup("Hyst. work",&pump_hot_water_hystesis_work);
			 }
		   //---------------------------------------------------------- Boiler 8.0 -------------------------------//
		     else if(eWindow==BOILER_MIN_TEMP)//   0
		     {
			     Namber_Setup("Min temp",&boiler_min_temp);
		     }
			 else if(eWindow==BOILER_MAX_TEMP)//   1
			 {
				 Namber_Setup("Min temp",&boiler_max_temp);
			 }
			 else if(eWindow==BOILER_TOP_HYSTES)//   2
			 {
				 Namber_Setup("Top hyster",&boiler_top_hystes);
			 }
			 else if(eWindow==BOILER_HYSTES_SWICH_BURNER)//   3
			 {
				 Namber_Setup("Hyster. swich",&boiler_hyste_swich_burner);
			 }
			 else if(eWindow==BOILER_HYSTES_PROTECT)//   4
			 {
				 Namber_Setup("Hyster. protect",&boiler_protect);
			 }
			 else if(eWindow==BOILER_TEMP_OVERHEATING)//   5
			 {
				 Namber_Setup("Temp. overheat.",&boiler_temp_overheating);
			 }
			 else if(eWindow==BOILER_MODUL_POWER_BURNER)//   6
			 {
				 if (flag_yes_no&(1<<MODUL_POWER_BURNER_DEF))
				 {
					 f_curent_pos=1;
				 }
				 else
				 f_curent_pos=0;
				 
				 u8g2_SelectionList(&u8g2,"Mod. pow. bur.",f_curent_pos+1, "YES\nNO");
				 f_curent_pos=0;
			 }
			 else if(eWindow==BOILER_TERMOSTAT_WORK)//   7
			 {
				if (flag_yes_no&(1<<TERMOSTAT_WORK_DEF))
				{
					f_curent_pos=1;
				}
				else
				f_curent_pos=0;
				
				u8g2_SelectionList(&u8g2,"Termo. work",f_curent_pos+1, "NORM\nADAPT");
				f_curent_pos=0;
			 }
			 //--------------------------------------------------------------FAN setup 9.0------------------------------//
			 else if (eWindow==FAN2_YES_NO) // 0
			 {
				 if (flag_yes_no&(1<<FAN2_YES_NO_DEF)) //(eSeason==SUMER)
				 {
					 f_curent_pos=1;
				 }
				 else
				 f_curent_pos=0;
				 
				 u8g2_SelectionList(&u8g2,"Fan2 Yes,No",f_curent_pos+1, "NO\nYES");
				 f_curent_pos=0;
			 }
			 else if (eWindow==FAN2_MODULATION) // 0
			 {
				 if (flag_yes_no&(1<<FAN2_MODULATION_DEF)) //(eSeason==SUMER)
				 {
					 f_curent_pos=1;
				 }
				 else
				 f_curent_pos=0;
				 
				 u8g2_SelectionList(&u8g2,"Modulation ignition",f_curent_pos+1, "NO\nYES");
				 f_curent_pos=0;
			 }
			 
			 else if(eWindow==FAN2_HEATING)// Ignition // 1
			 {
				 Namber_Setup("Max rotat Ignition ",&fan2_max_rotation_ignition);
			 }
			 else if(eWindow==FAN2_HEATING_FAN_ROTATION)// 2
			 {
				 Namber_Setup("Rotation ign.",&fan2_rotation_ignition);
			 }
			 else if(eWindow==FAN2_MAX_POWER)// 3
			 {
				 Namber_Setup("Max rotat Power",&fan2_rotation_max_power);
			 }
			 else if(eWindow==FAN2_MIN_POWER)//  4
			 {
				 Namber_Setup("Min rotation Power",&fan2_rotation_min_power);
			 }
			 else if(eWindow==FAN2_QUENCHING)//  5
			 {
				 Namber_Setup("Quenching rotation",&fan2_rotation_quenching);
			 }
			 else if(eWindow==FAN2_CLEANING)//  6
			 {
				 Namber_Setup("Cleaning rotation",&fan2_rotation_cleaning);
			 }
			 else if (eWindow==FAN2_BLOWING)
			 {
				 if (flag_yes_no&(1<<FAN2_BLOWING_DEF)) // 7
				 {
					 f_curent_pos=1;
				 }
				 else
				 f_curent_pos=0;
				 
				 u8g2_SelectionList(&u8g2,"Blowing Fan2",f_curent_pos+1, "NO\nYES");
				 f_curent_pos=0;
			 }
			 else if(eWindow==FAN2_BLOWING_TIME)// 8
			 {
				 Namber_Setup("Blowing break",&fan2_time_blowing);
			 }
			 else if(eWindow==FAN2_BLOWING_BREAK_TIME)// 9
			 {
				 Namber_Setup("Blowing break",&fan2_delay_time);
			 }
			 else if(eWindow==FAN2_BLOWING_ROTATION)// 10
			 {
				 Namber_Setup("Blowing rotation",&fan2_rotation_blowing);
			 }
		     
	}
}
//----------------------------------------------------------------------------------------Main_ -----------------------------------//
void Main_(void)
{
  eUpdate=UPDATE;
  for(;;)
   {		
	
	 if(flag_alarm)
	 {
		 eUpdate=UPDATE;
		 eSTART_STOP=STOP;
	 }
	
     if (eUpdate==UPDATE)
	 {  
	    u8g2_ClearBuffer(&u8g2);
		
		if(flag_yes_no&(1<<SELECT_CWU_DEF))
		{
			u8g2_DrawXBM(&u8g2,0,0,128,64,main_with_hot_water);
			
		    WriteValueToLCD(boiler_temperature,7,34);		  
		}
		else
		 {
		  u8g2_DrawXBM(&u8g2,0,0,128,64,main_without_hot_water);  //
		 }
		
		 if(flag_alarm)
		 {
			 
			 uint8_t var=60;
			 u8g2_DrawStr(&u8g2,30,8,"Eror:");
			 
			 if (flag_alarm&(1<<KOTEL_SENSOR_ALARM))
			 {
				 u8g2_DrawStr(&u8g2,var,8,"1,");
				 var=var+11;
			 }
			 
			 
			if(flag_yes_no&(1<<SELECT_CWU_DEF))
			{ 
			 if (flag_alarm&(1<<BOILER_SENSOR_ALARM))
			 {
				 u8g2_DrawStr(&u8g2,var,8,"2,");
				 var=var+11;
			 }
			}
			
			
			 if (flag_alarm&(1<<BURNER_SENSOR_ALARM))
			 {
				 u8g2_DrawStr(&u8g2,var,8,"3 ");
				 var=var+11;
			 }
			 
		 }	
			
			WriteValueToLCD(kotel_temperature,52,27);

			WriteValueToLCD(burner_temperature, 97,62);
			
		//-------------------------------------------------------------------- CO -------------------------------------//	
		 if (IS_CO_ON)
		 {
			 u8g2_DrawBox(&u8g2,32, 19, 7, 6);
		 }
		 else
		 {
		    u8g2_DrawFrame(&u8g2,32, 19, 8, 7);
		 }
		  
		  
		 if(eSTART_STOP==START)
		 {
			 //u8g2_ClearBuffer(&u8g2);
			 
			 u8g2_DrawStr(&u8g2,1,8,"WORK");
			 
		 }
		 else if (eSTART_STOP==STOP)
		 {
			
			 
			 u8g2_DrawStr(&u8g2,1,8,"STOP");
		 }
		 
		 
		 u8g2_SendBuffer(&u8g2);
		
		 eUpdate=NO_UPDATE;
	   }
	   if (eWindow!=MAIN)
	   {
		   return;
	   }
	 }
}
//------------------------------------------------------------------end -----------------------------------------------------//

void Namber_Setup(char* str, volatile uint8_t* n_template)
{
	u8g2_ClearDisplay(&u8g2);
	uint8_t e_template;
	e_template=eWindow; 
	u8g2_DrawStr(&u8g2,20,9,str);
	u8g2_DrawHLine(&u8g2, 0, 12, 128);
	u8g2_DrawHLine(&u8g2, 0, 13, 128);
	u8g2_SendBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_profont22_tn);
	for(;;)
	{
		if(eUpdate==UPDATE){
			u8g2_ClearBuffer(&u8g2);
			WriteValueToLCD(*(n_template),55,40);
			u8g2_UpdateDisplayArea(&u8g2,6, 3, 6, 2);
			eUpdate=NO_UPDATE;
		}
		if(eWindow!=e_template)
		{
			u8g2_SetFont(&u8g2, u8g2_font_t0_11_tr);
			return ;
		}
	}
}

void Init_ADC(void)
{
	ADMUX=0;       //ADC 0
	DDRF&=(1<<PF0);//  PORT F In_mode
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADIE)|(1<<ADPS1)|(1<<ADPS0);  // clock/ 128, ADC Enable, ADC Start
	eChanel=KOTEL;
}
void Init_All_Port(void)
{
	//
	DDRA|=(1<<PA7);
	PORTA|=(1<<PA7);                                               // FAN_2
	
	DDRA|=(1<<PA6);
	PORTA|=(1<<PA6);                                               // FAN_1
	//
	DDRA|=(1<<PA4);
	PORTA|=(1<<PA4);                                               // SOUND
	
	DDRB|=(1<<PB6);
	PORTB|=(1<<PB6);                                               // LED_PWM
	
	DDRC|=(1<<PC3);
	PORTC|=(1<<PC3);                                               // Burner
	
	DDRC|=(1<<PC4);
	PORTC|=(1<<PC4);                                               // CO pump
}

void Read_Temperature(void) 
{
	switch(eChanel)
	{
		case KOTEL:
		{
			ADMUX&=~((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADCSRA|=(1<<ADSC);
			break;
		}
		case BURNER:
		{
			ADMUX&=~((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADMUX|=(1<<MUX0);
			ADCSRA|=(1<<ADSC);
			break;
		}
		case BOILER:
		{
			ADMUX&=~((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADMUX|=(1<<MUX1);
			ADCSRA|=(1<<ADSC);
			break;
		}
		case SHNEK:
		{
			ADMUX&=~((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADMUX|=(1<<MUX2);
			ADCSRA|=(1<<ADSC);
			break;
		}
	}
}

void Init_ALL_Variables(void)
{
	//eeprom_read_byte();
	
	//-----------------------------MAIN seting 1.x--//
	setup_boiler_temperature=eeprom_read_byte(&eemem_setup_boiler_temperature);
	boil_power_percent=eeprom_read_byte(&eemem_boil_power_percent);
	m_hot_water=eeprom_read_byte(&eemem_m_hot_water);
	m_burner_temp=eeprom_read_word(&eemem_m_burner_temp);
	//volatile uint8_t HWS;
	//-----------------------------FAN Seting  2.x--//
	max_rotation_ignition=eeprom_read_byte(&eemem_max_rotation_ignition);// 1
	rotation_ignition=eeprom_read_byte(&eemem_rotation_ignition); //  2
	rotation_max_power=eeprom_read_byte(&eemem_rotation_max_power);// 3
	rotation_min_power=eeprom_read_byte(&eemem_rotation_min_power);// 4
	rotation_quenching=eeprom_read_byte(&eemem_rotation_quenching);// 5
	rotation_cleaning=eeprom_read_byte(&eemem_rotation_cleaning);// 6
	time_blowing=eeprom_read_byte(&eemem_time_blowing);// 8
	delay_time=eeprom_read_byte(&eemem_delay_time);// 9
	rotation_blowing=eeprom_read_byte(&eemem_rotation_blowing);// 10
	//------end fan seting
	//----------------------------BUNKER Seting 3.x--//
	start_dose=eeprom_read_byte(&eemem_start_dose);
	supply_cycle=eeprom_read_byte(&eemem_supply_cycle);
	dose_max_power=eeprom_read_byte(&eemem_dose_max_power);
	dose_min_power=eeprom_read_byte(&eemem_dose_min_power);
	stoker_time_work=eeprom_read_byte(&eemem_stoker_time_work);// 4
	stoker_time_delay=eeprom_read_byte(&eemem_stoker_time_delay);
	stoker_time_continuation=eeprom_read_byte(&eemem_stoker_time_continuation);
	stoker_time_emptying=eeprom_read_byte(&eemem_stoker_time_emptying);
	stoker_fervor=eeprom_read_byte(&eemem_stoker_fervor);
	//------end bunker seting
	//---------------------------------Lighter 4.x--//
	lighter_hysteresis=eeprom_read_byte(&eemem_lighter_hysteresis);//0
	lighter_flame_delay=eeprom_read_byte(&eemem_lighter_flame_delay);//1
	lighter_kindling_time=eeprom_read_byte(&eemem_lighter_kindling_time);//2
	lighter_attempt_kindling=eeprom_read_byte(&eemem_lighter_attempt_kindling);//3
	lighter_fuel_shortage=eeprom_read_byte(&eemem_lighter_fuel_shortage);//4
	//volatile uint8_t lighter_ignition_stab;//5
	lighter_time_ignition_stab=eeprom_read_byte(&eemem_lighter_time_ignition_stab);//6
	//volatile uint8_t lighter_pelets_fan; //7
	lighter_woods_fan=eeprom_read_byte(&eemem_lighter_woods_fan);//8
	//------end
	//--------------------------------- Cleaning mechanism 5.x--//
	mechanism_cleaning_work=eeprom_read_byte(&eemem_mechanism_cleaning_work);
	mechanism_cleaning_delay=eeprom_read_byte(&eemem_mechanism_cleaning_delay);
	//------------------------- Pump heating seting 6.x--//
	pump_period_work=eeprom_read_byte(&eemem_pump_period_work);
	//------end
	//---------------------------------- Pump heating seting 7.x--//
	pump_hot_water_hystesis=eeprom_read_byte(&eemem_pump_hot_water_hystesis);//0
	pump_hot_water_increase=eeprom_read_byte(&eemem_pump_hot_water_increase);//1
	pump_hot_water_overspending=eeprom_read_byte(&eemem_pump_hot_water_overspending);//2
	pump_hot_water_temp_work=eeprom_read_byte(&eemem_pump_hot_water_temp_work);//3
	pump_hot_water_hystesis_work=eeprom_read_byte(&eemem_pump_hot_water_hystesis_work);//4
	//------end
	//---------------------------------- Boiler seting 8.x--//
	boiler_min_temp=eeprom_read_byte(&eemem_boiler_min_temp);// 0
	boiler_max_temp=eeprom_read_byte(&eemem_boiler_max_temp);// 1
	boiler_top_hystes=eeprom_read_byte(&eemem_boiler_top_hystes);// 2
	boiler_hyste_swich_burner=eeprom_read_byte(&eemem_boiler_hyste_swich_burner);// 3
	boiler_protect=eeprom_read_byte(&eemem_boiler_protect);// 4
	boiler_temp_overheating=eeprom_read_byte(&eemem_boiler_temp_overheating);// 5
	//-------end
	//-----------------------------FAN2 Seting  9.x--//
	fan2_max_rotation_ignition=eeprom_read_byte(&eemem_fan2_max_rotation_ignition);// 1
	fan2_rotation_ignition=eeprom_read_byte(&eemem_fan2_rotation_ignition); //  2
	fan2_rotation_max_power=eeprom_read_byte(&eemem_fan2_rotation_max_power);// 3
	fan2_rotation_min_power=eeprom_read_byte(&eemem_fan2_rotation_min_power);// 4
	fan2_rotation_quenching=eeprom_read_byte(&eemem_fan2_rotation_quenching);// 5
	fan2_rotation_cleaning=eeprom_read_byte(&eemem_fan2_rotation_cleaning);// 6
	fan2_time_blowing=eeprom_read_byte(&eemem_fan2_time_blowing);// 8
	fan2_delay_time=eeprom_read_byte(&eemem_fan2_delay_time);// 9
	fan2_rotation_blowing=eeprom_read_byte(&eemem_fan2_rotation_blowing);// 10
}

void WriteValueToLCD(uint8_t val, int8_t x, int8_t y)
{
	if (val==0xFF)
	{
		u8g2_DrawStr(&u8g2,x,y,"--");
		return;
	}
	int8_t width;
	uint8_t val_temp=0;
	width=u8g2_GetMaxCharWidth(&u8g2);
	
	 if (val/100)
	{
		u8g2_DrawGlyph(&u8g2,x,y,mas_d[val/100]);
		val_temp=val%100;
		u8g2_DrawGlyph(&u8g2,x+1+width,y,mas_d[(val_temp/10)]);  
		u8g2_DrawGlyph(&u8g2,x+1+(width*2),y,mas_d[(val_temp%10)]);
	}
	else if (val/10)
	{
		u8g2_DrawGlyph(&u8g2,x,y,mas_d[(val/10)]);
		u8g2_DrawGlyph(&u8g2,x+1+width,y,mas_d[(val%10)]);
	}
	else 
	{
		u8g2_DrawGlyph(&u8g2,x,y,mas_d[val]);
	}
}
void TIMER16_3_Init(void)//--------------------------------------------------------TIMER_16_3___-------------------------//
{
	
	TCCR3B|=(1<<CS32);//|(1<<CS30); // 16 Mhz  / 256 prescal
	TCNT3H=0;
	TCNT3L=0;
	TIMSK3|=(1<<OCIE3A);
	OCR3AH=0x01;
	OCR3AL=0xF4; //1 milisec
	//
}
void TIMER16_1_Init(void)//--------------------------------------------------------TIMER16_1_Init----------------------------//
{
	TCCR1B|=(1<<CS12);//|(1<<CS30); // 16 Mhz  / 256 prescal
	TCNT1H=0;
	TCNT1L=0;
	TIMSK1|=(1<<OCIE1A);
	OCR1AH=0x01;
	OCR1AL=0xF4; //1 milisec
}

void TIM16_WriteTCNT3( uint16_t i )
{
	unsigned char sreg;
	/* Save global interrupt flag */
	sreg = SREG;
	/* Disable interrupts */
	cli();
	/* Set TCNTn to i */
	TCNT3 = i;
	/* Restore global interrupt flag */
	SREG = sreg;
	sei();
}

void TIM16_WriteOCR3A(uint16_t ocr)
{
	unsigned char sreg;
	/* Save global interrupt flag */
	sreg = SREG;
	/* Disable interrupts */
	cli();
	/* Set TCNTn to i */
	TCNT3 = ocr;
	/* Restore global interrupt flag */
	SREG = sreg;
	sei();
}
uint8_t Convert_ADC_To_Celsium(uint16_t adc)
{
	uint8_t celsium=0xFF;
	V_out=(adc*2.71)/1023;
	R=-1.0*((2.71*3000*V_out)/((2.71*V_out)-(5*2.71)));
	if(R>=1630&&R<=1772)                                 // 0  ->  10 celsium
	{
		if(R==1630)
		R=1631;
		double_koeficient=R-1630;
		double_koeficient=double_koeficient/14;
		int_koeficient=double_koeficient;
		if ((uint8_t)((double_koeficient-(double)int_koeficient)*10)>=5)
		{
			celsium=int_koeficient+1;
		}
		
		else
		celsium=int_koeficient;
	}
	else if (R>=1773&&R<=1922)                          // 10 ->  20 celsium
	{
		if(R==1773)
		R=1774;
		double_koeficient=R-1773;
		double_koeficient=double_koeficient/14.9;
		int_koeficient=double_koeficient;
		if (((uint8_t)((double_koeficient-(double)int_koeficient)*10))>=5)
		{
			int_koeficient=int_koeficient+1;
			celsium=int_koeficient+10;
		}
		else
		celsium=int_koeficient+10;
	}
	else if(R>=1923&&R<=2000)                          // 20 ->  25 celsium
	{
		if(R==1923)
		R=1924;
		double_koeficient=R-1923;
		double_koeficient=double_koeficient/15;
		int_koeficient=double_koeficient;
		if (((uint8_t)((double_koeficient-(double)int_koeficient)*10))>=5)
		{
			int_koeficient=int_koeficient+1;
			celsium=int_koeficient+20;
		}
		else
		celsium=int_koeficient+20;
	}
	else if(R>=2001&&R<=2080)                         // 25 -> 30 celsium
	{
		if(R==2001)
		R=2002;
		double_koeficient=R-2001;
		double_koeficient=double_koeficient/15.5;
		int_koeficient=double_koeficient;
		if (((uint8_t)((double_koeficient-(double)int_koeficient)*10))>=5)
		{
			int_koeficient=int_koeficient+1;
			celsium=int_koeficient+25;
		}
		else
		celsium=int_koeficient+25;
		
	}
	else if(R>=2081&&R<=2245)                         // 30 -> 40 celsium
	{
		if(R==2081)
		R=2082;
		double_koeficient=R-2081;
		double_koeficient=double_koeficient/16.4; 
		int_koeficient=double_koeficient;
		if (((uint8_t)((double_koeficient-(double)int_koeficient)*10))>=5)
		{
			int_koeficient=int_koeficient+1;
			celsium=int_koeficient+30;
		}
		else
		celsium=int_koeficient+30;
	}
	else if(R>=2246&&R<=2417)                         // 40 -> 50 celsium
	{
		if(R==2246)
		R=2247;
		double_koeficient=R-2246;
		double_koeficient=double_koeficient/17.2;
		int_koeficient=(uint8_t)double_koeficient;
		if (((uint8_t)((double_koeficient-(double)int_koeficient)*10))>=5)
		{
			int_koeficient=int_koeficient+1;
			celsium=int_koeficient+40;
		}
		else
		celsium=int_koeficient+40;
	}
	return celsium;
}

uint16_t Convert_ADC_To_Celsium(uint16_t adc)
{
	uint16_t celsium=0xFFFF;
	uint16_t int_16_coficient;
	
	V_out=(adc*2.71)/1023;
	R=-1.0*((2.71*3000*V_out)/((2.71*V_out)-(5*2.71)));
	if(R>=1000&&R<1037)                                 // 0  ->  10 celsium
	{
		if(R==1000)
		R=1001;
		double_koeficient=R-1000;
		double_koeficient=double_koeficient/3.5;
		int_16_coficient=double_koeficient;
		//if ((uint16_t)((double_koeficient-(double)int_koeficient)*10)>=5)
		//{
			//celsium=int_16_coficient+1;
		//}
		
		//else
		celsium=int_16_coficient;
	}
	if(R>=1037&&R<1075)                                 // 10  ->  20 celsium
	{
		if(R==1037)
		R=1038;
		double_koeficient=R-1037;
		double_koeficient=double_koeficient/3.8;
		int_16_coficient=double_koeficient;
		//if ((uint16_t)((double_koeficient-(double)int_koeficient)*10)>=5)
		//{
		//	int_16_coficient=int_16_coficient+1;
		//	celsium=int_16_coficient+10;
		//}
		
		//else
		celsium=int_16_coficient;
	}
