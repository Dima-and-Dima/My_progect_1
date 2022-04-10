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


//uint8_t EEMEM eememWindow=MAIN;
//uint8_t EEMEM eememWork_Mode=W_MONITORING;
  uint8_t EEMEM eememSTART_STOP;


  uint16_t EEMEM eemem_flag_yes_no=0;
  uint8_t EEMEM eeme_flag_yes_no_2=0;
  //uint16_t EEMEM eemem_flag_alarm=0;
  
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
  uint8_t EEMEM eemem_lighter_time_ignition_stab=1;//6
  uint8_t EEMEM eemem_lighter_woods_fan=0;//8
  uint8_t EEMEM eemem_lighter_quenching_time=1;//8 1->30 min
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
 //---------------------------------- Kotel seting 8.x--//
  uint8_t EEMEM eemem_kotel_min_temp=30;// 0
  uint8_t EEMEM eemem_kotel_max_temp=70;// 1
  uint8_t EEMEM eemem_kotel_top_hystes=1;// 2
  uint8_t EEMEM eemem_kotel_hyste_swich_burner=3;// 3
  uint8_t EEMEM eemem_kotel_protect=1;// 4
  uint8_t EEMEM eemem_kotel_temp_overheating=90;// 5
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

#define boiler_bunker_width 84
#define boiler_bunker_height 49
static unsigned char boiler_bunker_bits[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x07, 0xfe,
	0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x08, 0x01, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0xf8, 0x23, 0x00, 0x08, 0x01, 0x00, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x08, 0x22, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x08, 0x22, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x08, 0x22, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00,
	0x00, 0x08, 0x22, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
	0xf8, 0x23, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x07,
	0x20, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x80, 0x08, 0x20,
	0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x80, 0x10, 0x20, 0x00,
	0x08, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x80, 0x28, 0x20, 0x00, 0x08,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x45, 0x20, 0x00, 0x08, 0x01,
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x82, 0x20, 0x18, 0x08, 0x01, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x39, 0x21, 0x34, 0x08, 0x01, 0x00, 0x00,
	0x00, 0x01, 0x00, 0x80, 0x20, 0x22, 0x6a, 0x08, 0x01, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x40, 0xe0, 0x24, 0xd5, 0x08, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x20, 0x00, 0xa8, 0xaa, 0x09, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00,
	0x90, 0x13, 0x70, 0x55, 0x0b, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x88,
	0x28, 0xa7, 0xaa, 0x0e, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0xe4, 0x44,
	0x64, 0x55, 0x0d, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x82, 0xac,
	0xaa, 0x0a, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x61, 0x55,
	0x0d, 0x01, 0x00, 0x00, 0x00, 0x01, 0x80, 0x9c, 0x00, 0xa2, 0xaa, 0x0a,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x40, 0x44, 0x00, 0x64, 0x55, 0x0d, 0x01,
	0x00, 0x00, 0x00, 0x01, 0x20, 0x27, 0x00, 0xa8, 0xaa, 0x0a, 0x01, 0x00,
	0x00, 0x00, 0x01, 0x10, 0x10, 0x00, 0x70, 0x55, 0x0d, 0x01, 0x00, 0x00,
	0x00, 0x01, 0x10, 0x08, 0x00, 0xa0, 0xaa, 0x0a, 0x01, 0x00, 0x00, 0x00,
	0x01, 0x10, 0x04, 0x00, 0x60, 0x55, 0x0d, 0x01, 0x00, 0x00, 0x80, 0xff,
	0xff, 0xff, 0x1f, 0xa0, 0xaa, 0x0a, 0x01, 0x00, 0x00, 0x40, 0x01, 0x00,
	0x80, 0x10, 0x60, 0x55, 0x0d, 0x01, 0x00, 0x00, 0x20, 0x71, 0x00, 0x87,
	0x10, 0xa0, 0xaa, 0x0a, 0x01, 0x00, 0x00, 0x10, 0x81, 0x03, 0xb8, 0x10,
	0x60, 0x55, 0x0d, 0x01, 0x00, 0x00, 0x08, 0x01, 0x00, 0x80, 0x10, 0xa0,
	0xaa, 0x0a, 0x01, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x10, 0x60, 0x55,
	0x0d, 0x01, 0x08, 0x00, 0x0c, 0x01, 0x00, 0x84, 0x10, 0xa0, 0xaa, 0x0a,
	0x01, 0x49, 0x00, 0xf0, 0x3f, 0x10, 0x84, 0x1f, 0x60, 0x55, 0x0d, 0x01,
	0x2a, 0x00, 0x00, 0x21, 0x18, 0x04, 0x00, 0xe0, 0xff, 0x0f, 0x01, 0x1c,
	0x00, 0x00, 0x61, 0x50, 0x04, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
	0xff, 0x41, 0xfe, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
	0xc0, 0x14, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
	0x30, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x11,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xc3, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x3d, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };



#define Wood_boiler_width 51
#define Wood_boiler_height 49
static unsigned char Wood_boiler_bits[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x01, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x10,
	0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x01, 0xfe,
	0xff, 0xff, 0xff, 0xff, 0x07, 0x01, 0x08, 0x00, 0x0c, 0x01, 0x00, 0x04,
	0x01, 0x49, 0x00, 0xf0, 0x3f, 0x10, 0x04, 0x01, 0x2a, 0x00, 0x00, 0x21,
	0x18, 0x04, 0x01, 0x1c, 0x00, 0x00, 0x61, 0x50, 0x04, 0xff, 0xff, 0xff,
	0xff, 0x41, 0xfe, 0x04, 0x00, 0x08, 0x00, 0x00, 0xc0, 0x14, 0x04, 0x00,
	0x00, 0x00, 0x00, 0x80, 0x30, 0x06, 0x00, 0x00, 0x00, 0x00, 0x80, 0x11,
	0x01, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00 };



#define battery_width 21
#define battery_height 12
static unsigned char battery_bits[] = {
	0x04, 0x02, 0x01, 0x0e, 0x87, 0x03, 0xff, 0xff, 0x1f, 0x0e, 0x87, 0x03,
	0x0e, 0x87, 0x03, 0x0e, 0x87, 0x03, 0x0e, 0x87, 0x03, 0x0e, 0x87, 0x03,
0x0e, 0x87, 0x03, 0xff, 0xff, 0x1f, 0x0e, 0x87, 0x03, 0x04, 0x02, 0x01 };


#define line_batery_width 10
#define line_batery_height 42
static unsigned char line_batery_bits[] = {
	0xf8, 0x03, 0x08, 0x00, 0xff, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00,
	0x81, 0x00, 0x81, 0x00, 0xff, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00,
	0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00,
	0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00,
	0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00,
	0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x0f, 0x03, 0x00, 0x01,
0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0xff, 0x01 };


#define flame_width 17
#define flame_height 5
static unsigned char flame_bits[] = {
	0x04, 0x41, 0x00, 0x8e, 0xe3, 0x00, 0xdf, 0xf7, 0x01, 0xdf, 0xf7, 0x01,
0x8e, 0xe3, 0x00 };


#define cleaning_width 15
#define cleaning_height 1
static unsigned char cleaning_bits[] = {
0xc7, 0x71 };


#define elektro_ten_width 3
#define elektro_ten_height 9
static unsigned char elektro_ten_bits[] = {
0x02, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x02 };


#define FAN_2_width 16
#define FAN_2_height 16
static unsigned char FAN_2_bits[] = {
	0xff, 0x1f, 0x00, 0x20, 0x07, 0x42, 0x04, 0x83, 0x0c, 0x8a, 0xc8, 0x9f,
	0x98, 0x82, 0x10, 0xc6, 0x30, 0x22, 0x40, 0x18, 0xfe, 0x07, 0x02, 0x01,
0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0xfe, 0x01 };




#define boiler_width 39
#define boiler_height 45
static unsigned char boiler_bits[] = {
	0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00,
	0x02, 0x01, 0x00, 0x00, 0x00, 0x02, 0xff, 0x7f, 0x00, 0x00, 0x02, 0x01,
	0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00,
	0x00, 0x20, 0x00, 0x00, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x01, 0x00, 0x20,
	0x08, 0x00, 0x01, 0x00, 0x20, 0x08, 0x00, 0x01, 0x00, 0x20, 0x08, 0x00,
	0x01, 0x00, 0x20, 0x08, 0x00, 0x01, 0x00, 0x20, 0x08, 0x00, 0x01, 0x00,
	0x20, 0x08, 0x00, 0x01, 0x00, 0x20, 0x08, 0x00, 0x01, 0x00, 0x20, 0x08,
	0x00, 0x01, 0x00, 0x20, 0x08, 0x00, 0x01, 0x00, 0x20, 0x08, 0x00, 0x01,
	0x00, 0x20, 0x08, 0x00, 0x01, 0x00, 0x20, 0x08, 0x00, 0x01, 0x00, 0x20,
	0x08, 0x00, 0x01, 0x00, 0x20, 0x08, 0x00, 0xc1, 0xff, 0x3f, 0x08, 0x00,
	0x21, 0x00, 0x00, 0x08, 0x00, 0xc1, 0xff, 0x3f, 0x08, 0x00, 0x01, 0x00,
	0x40, 0x08, 0x00, 0xc1, 0xff, 0x3f, 0x08, 0x00, 0x21, 0x00, 0x00, 0x08,
	0x00, 0xc1, 0xff, 0x3f, 0x08, 0x00, 0x01, 0x00, 0x40, 0x08, 0x00, 0xfd,
	0xff, 0x3f, 0x08, 0x00, 0x05, 0x00, 0x00, 0x08, 0x00, 0xff, 0xff, 0xff,
	0x0f, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x60, 0x04, 0x00,
	0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x00,
	0x20, 0x04, 0x00, 0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x00, 0x20, 0x04,
0x00, 0x00, 0x00, 0x20, 0xfc, 0xff, 0xff, 0xff, 0x3f };







//-----------------------------------------------------------------------------end--------------------------------------//
void WriteValueToLCD(uint8_t val, int8_t x, int8_t y);
void Write16bitValueToLCD(uint16_t val, int8_t x, int8_t y);
uint8_t Convert_ADC_To_Celsium(uint16_t adc);
uint16_t Convert_PT1000_ADC_To_Celsium(uint16_t adc);
void Init_All_Port(void);
void Init_ADC(void);
void Boiler_Temperature(void);
void Main_(void);
void Boiler_Power_Percent(void);
void Fan_Setup(void);
void Namber_Setup(char* str, volatile uint8_t*);
void Namber_16bit_Setup(char* str, volatile uint16_t* n_template);
void Init_ALL_Variables(void);
void Init_Intterupt_6(void);


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
void TIMER16_4_Init(void);
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
	SHNEK,
	CO_TERMOSTAT
}eChanel; 

	

ISR(INT6_vect)//---------------------------------------------------- 50 Hz interrupt -------------------------------//
{
	unsigned char sreg;
	/* Save global interrupt flag */
	sreg = SREG;
	
	cli();
	
	if (fan1_spead_var==100)
	{
		fan1_spead_var=99;
	}
	if (fan2_spead_var==100)
	{
		fan2_spead_var=99;
	}
	
	//---------------------------------------------------- FANs ---------------------------------//
	
		
		if (flag_yes_no&(1<<FAN2_YES_NO_DEF))// if selected FAN2
		{
			if (!(flag_yes_no&(1<<FUEL_DEF)))// if  pelets mode
			{
				
				if (flag_yes_no&(1<<PELETS_FAN_DEF))// if selected FAN1+FAn2
				{
					//FAN1
					TIMSK3|=(1<<OCIE3A);
					TCCR3B|=(1<<CS32);//|(1<<CS30); // 16 Mhz  / 256 prescal
					OCR3A=(437-(437/100)*fan1_spead_var);// write var for adjustment
					
					//FAN2
					TIMSK4|=(1<<OCIE4A);
					TCCR4B|=(1<<CS42);//|(1<<CS30); // 16 Mhz  / 256 prescal
					OCR4A=(437-(437/100)*fan2_spead_var);// write var for adjustment // %
				}
				else
				{
					TIMSK3|=(1<<OCIE3A);
					TCCR3B|=(1<<CS32);//|(1<<CS30); // 16 Mhz  / 256 prescal
					OCR3A=(437-(437/100)*fan1_spead_var);// write var for adjustment
				}
				
			}
	     else//  WOOD mode
			{
				
				
				if (lighter_woods_fan==0)//   FAN1
				{
					//FAN1
					TIMSK3|=(1<<OCIE3A);
					TCCR3B|=(1<<CS32);//|(1<<CS30); // 16 Mhz  / 256 prescal
					OCR3A=(437-(437/100)*fan1_spead_var);// write var for adjustment
				}
				else if (lighter_woods_fan==1)//   FAN2
				{
					//FAN2
					TIMSK4|=(1<<OCIE4A);
					TCCR4B|=(1<<CS42);//|(1<<CS30); // 16 Mhz  / 256 prescal
					OCR4A=(437-(437/100)*fan2_spead_var);// write var for adjustment
				}
				else//  FAN1+ FAN2
				{
					//FAN1
					TIMSK3|=(1<<OCIE3A);
					TCCR3B|=(1<<CS32);//|(1<<CS30); // 16 Mhz  / 256 prescal
					OCR3A=(437-(437/100)*fan1_spead_var);// write var for adjustment
					
					//FAN2
					TIMSK4|=(1<<OCIE4A);
					TCCR4B|=(1<<CS42);//|(1<<CS30); // 16 Mhz  / 256 prescal
					OCR4A=(437-(437/100)*fan2_spead_var);// write var for adjustment
				}
				
			}
			
		}
		else// FAN1 selected. FAn2 dissabled
		{
			TIMSK3|=(1<<OCIE3A);
			TCCR3B|=(1<<CS32);//|(1<<CS30); // 16 Mhz  / 256 prescal
			OCR3A=(437-(437/100)*fan1_spead_var);// write var for adjustment

		}
	
	TCNT3H=0;
	TCNT3L=0;// ------------------ FAN_1
	
	TCNT4H=0;
	TCNT4L=0;// ------------------ FAN_2
	
	
	
	sei();
	SREG = sreg;
}
ISR(TIMER3_COMPA_vect)// FAN 1
{
	unsigned char sreg;
	/* Save global interrupt flag */
	sreg = SREG;
	cli();
	
	
		TIMSK3&=~(1<<OCIE3A);
		TCCR3B&=~((1<<CS32));//|(1<<CS30)); // 16 Mhz  / 256 prescal
		FAN1_ON;
		//FAN2_ON;
		_delay_us(100);
		//FAN2_OFF;
		FAN1_OFF;
	
	sei();
	SREG = sreg;
	
}

ISR(TIMER4_COMPA_vect)// FAN 2
{
	unsigned char sreg;
	/* Save global interrupt flag */
	sreg = SREG;
	cli();
		TIMSK4&=~(1<<OCIE4A);
		TCCR4B&=~(1<<CS42);//|(1<<CS30)); // 16 Mhz  / 256 prescal
		FAN2_ON;
		_delay_us(100);
		FAN2_OFF;
	
	sei();
	SREG = sreg;
}



ISR(TIMER1_COMPA_vect)// Added  22-07-2020 !!!!!!!!!!!!!! WORK case
{
	unsigned char sreg;
	/* Save global interrupt flag */
	sreg = SREG;
   cli();	
    switch(eSTART_STOP)
    { 
	  case STOP:  // in stop mode we  monitoring  kotel temperature and CWU
	  {
            
		 if (start_stop_template!=STOP)
		 {
			 eeprom_write_byte(&eememSTART_STOP,STOP);
			 start_stop_template=STOP;
		 }
		 	

		  //------------------------------------------------------------------control overheating-----------------------------------------------//
		  
		 if (flag_alarm)// ALARM
		 {
			 if (flag_alarm&(1<<OWER_HEATING_ALARM)||flag_alarm&(1<<HOME_SENSOR_ALARM))
			 {
				 CO_OFF;
				 ELECTROTEN_OFF;
				 BURNER_AUGER_OFF;
				 CWU_OFF;
				 BUNKER_AUGER_OFF;
			 }
			 
			 
			 if (flag_alarm&(1<<KOTEL_SENSOR_ALARM))
			 {
				 flag_alarm&=~(1<<OWER_HEATING_ALARM);
			 }
			 
			 
			  if (flag_alarm&(1<<KOTEL_SENSOR_ALARM))
			 {
				CO_ON; 
			 }
			 else
			    CO_OFF;
			 
			 if (flag_yes_no&(1<<SESON_DEF))// if season - summer
			 {
				 //if (kotel_temperature>=(kotel_max_temp+kotel_protect)||flag_alarm&(1<<KOTEL_SENSOR_ALARM))// if T kotel > setup T ouerheating- CO on
				 if(flag_alarm&(1<<KOTEL_SENSOR_ALARM))
				 {
					 CWU_ON;             // Hot water on
					 ELECTROTEN_OFF;
				 }
				 
				 else 
				 {
					 CWU_OFF;
					 ELECTROTEN_OFF;
				 }
				 
				 
				 if (flag_yes_no&(1<<SELECT_CWU_DEF))
				 {
					 flag_alarm&=~(1<<NO_SELECT_CWU_IN_SUMMER);
				 }
			 }
			 else
			  
				   flag_alarm&=~(1<<NO_SELECT_CWU_IN_SUMMER);
				   
				    
	
			 
			 /*
			 else//  if season - winter
			  {
				  if (kotel_temperature>=(kotel_max_temp+kotel_protect))// if T kotel > setup T ouerheating- CO on
				  {
					  CO_OFF;
					  ELECTROTEN_OFF;
				  } 
				  
				  else if( kotel_temperature>=kotel_max_temp && kotel_temperature<=kotel_temp_overheating|| flag_alarm&(1<<KOTEL_SENSOR_ALARM))// 
				  {
					  CO_ON;
					  ELECTROTEN_OFF;
				  }
				  else if (kotel_temperature>=kotel_temp_overheating)
				  {
					  CO_OFF;
					  ELECTROTEN_OFF;
					  BURNER_AUGER_OFF;
					  CWU_OFF;
					  BUNKER_AUGER_OFF;
					  flag_alarm|=(1<<OWER_HEATING_ALARM);
				  }
			  }*/
			 
		 }
		 
		 else//  NO ALARM
		 {
			

			 //--------------------------------------------- CO and CWU in Sumer and Winter mode ----------------------//
			 
			 if(!(flag_yes_no&(1<<SESON_DEF)))// if WINTER mode, use CO and CWU flag_yes_no&(1<<SELECT_CWU_DEF)
			 {
				 if (flag_alarm&(1<<NO_SELECT_CWU_IN_SUMMER))
				 {
					 flag_alarm&=~(1<<NO_SELECT_CWU_IN_SUMMER);
				 }
				 
				 
				 //----------------CO ------------------------//
				 if (kotel_temperature<=(kotel_max_temp-1)&&(!(flag_yes_no&(1<<PUMP_HEATING_DEF)))&&kotel_temperature>kotel_min_temp-kotel_protect )// if  thermostat mode
				 {
					 if (IS_TERMOSTAT_OFF)// 
					 {
						 CO_OFF;
					 }
					 else if (IS_TERMOSTAT_ON)
					 {
						CO_ON; 
					 }
					 
					 flag_alarm&=~(1<<NO_SELECT_CWU_IN_SUMMER);
				 }
				 
				 
				 else if ((kotel_temperature<=(kotel_max_temp-1))&&(flag_yes_no&(1<<PUMP_HEATING_DEF)&&kotel_temperature>kotel_min_temp+kotel_protect ))// if auto mode
				 {
					 if (home_temperature<=setup_boiler_temperature-kotel_protect)// 
					 {
						 CO_ON;
					 }
					 else if (home_temperature>=setup_boiler_temperature)
					 {
						 CO_OFF;
					 }
					 
				 }
				 
				 
				 else if (kotel_temperature>kotel_max_temp&&kotel_temperature<=kotel_max_temp+kotel_protect)
				 {
					 CO_ON;
					 if (IS_ELECTROTEN_ON)
					 {
						 ELECTROTEN_OFF;
					 }
				 }
				 
				 else if ( kotel_temperature>(kotel_max_temp+kotel_protect+1) )
				 {
					 CWU_OFF;
					 CO_OFF;
				 }
				 
				 
				 else if(  kotel_temperature>kotel_temp_overheating  &&(!(flag_alarm&(1<<KOTEL_SENSOR_ALARM)))  )
				 {
					flag_alarm|=(1<<OWER_HEATING_ALARM);
					CWU_OFF;
					CO_OFF;
					ELECTROTEN_OFF;
					BURNER_AUGER_OFF;
					BUNKER_AUGER_OFF;
					break;
				 }
				 else if ( kotel_temperature<=(kotel_min_temp-kotel_protect))
				 {
					 CO_OFF;
				 }
				 
				 
				 //-----------------CWU --------//
				 
				 if(flag_yes_no&(1<<SELECT_CWU_DEF))
				 {
					 if (boiler_temperature<=(m_hot_water-pump_hot_water_hystesis_work)&&kotel_temperature>(kotel_min_temp+pump_hot_water_hystesis_work)&&!flag_alarm &&kotel_temperature<kotel_max_temp-1)//  temperatura gwu < setup temperature and t. gwu > temp. kotla
					 {
						 CWU_ON; // Hot water pump ON
					 }
					 
					 else if ( boiler_temperature>=m_hot_water ||flag_alarm&(1<<BOILER_SENSOR_ALARM) ||kotel_temperature<=(kotel_min_temp-kotel_protect) )
					 {
						 CWU_OFF;
					 }
				 }
				 else
				    CWU_OFF;
				 
			 }
			 
			 
			 
			 else// SUMMER mode , use CWU only!
			 {
				 if(flag_yes_no&(1<<SELECT_CWU_DEF))//
				 {
					 if (  (boiler_temperature<(m_hot_water-pump_hot_water_hystesis_work)&&kotel_temperature<(kotel_max_temp+kotel_protect-1))&&kotel_temperature>(kotel_min_temp+pump_hot_water_hystesis_work)&&!flag_alarm)//  temperatura gwu < setup temperature and t. gwu > temp. kotla
					 {
						 CWU_ON; // Hot water pump ON
					 }
					 
					 else if ((boiler_temperature>m_hot_water&&kotel_temperature<kotel_max_temp)||kotel_temperature<(kotel_min_temp-kotel_protect))
					 {
						 CWU_OFF;
					 }
					 flag_alarm&=~(1<<NO_SELECT_CWU_IN_SUMMER);
					 
					
					 
					 if (kotel_temperature>kotel_max_temp&&kotel_temperature<kotel_max_temp+kotel_protect)
					 {
						  CWU_ON; // Hot water pump ON
					 }
					 else if (kotel_temperature>kotel_max_temp+kotel_protect+1)
					 {
						 CWU_OFF;
					 }
				 }
				 else
				 flag_alarm|=(1<<NO_SELECT_CWU_IN_SUMMER);
			 }
			 //--------------------------------------------------
		 }
		  
		  
		  
		 //--------------------------------------------------------------------------- AUGER ---------------------------------//
		  
		  if(IS_BURNER_AUGER_ON||IS_BUNKER_AUGER_ON||IS_ELECTROTEN_ON)
		  {
			  if(!(flag_alarm&(1<<SHNEK_OWER_HEAT_ALARM)))
		          BURNER_AUGER_OFF;
		    BUNKER_AUGER_OFF;
			ELECTROTEN_OFF;
		  }
		  
		  
		  timer_template_stoker_dose=0;
		  flag_yes_no_2|=(1<<STOKER_WORK_DELAY_DEF);
		  
		  
		  if (kotel_temperature>=kotel_temp_overheating&&(!(flag_alarm&(1<<KOTEL_SENSOR_ALARM)))  )
		  {
			  flag_alarm|=(1<<OWER_HEATING_ALARM);
		  }
		  else if (kotel_temperature<=kotel_temp_overheating-3)// 3- hysteresis
		  {
			  flag_alarm&=~(1<<OWER_HEATING_ALARM);
		  }
	  
	  break;
	
     }
	  case START:
	  {
		  //---------------------------------------------------------- Power down -----------------------------------------------//
		  if (start_stop_template!=START && eWork_Mode!=W_AFTERBURNING && eWork_Mode!=W_ATTENUATION && eWork_Mode!=W_CLEANING && eWork_Mode!=W_EXTINGUISHING)
		  {
			  eeprom_write_byte(&eememSTART_STOP,START);
			  start_stop_template=START;
		  }
		  else if (start_stop_template!=STOP && (eWork_Mode==W_AFTERBURNING || eWork_Mode==W_ATTENUATION || eWork_Mode==W_CLEANING || eWork_Mode==W_EXTINGUISHING))
		  {
			  eeprom_write_byte(&eememSTART_STOP,STOP);
			  start_stop_template=STOP;
		  }
		  
		 //------------------------------------------------------------ CLEANING--------------------------------------------------//
		 if (cleaning_delay_timer>=((mechanism_cleaning_delay*60)/0.008) &&eWork_Mode!=W_MONITORING && eWork_Mode!=W_AFTERBURNING && eWork_Mode!=W_ATTENUATION && eWork_Mode!=W_CLEANING && eWork_Mode!=W_EXTINGUISHING)
		 {
			eWork_Mode=W_CLEANING;
			cleaning_delay_timer=0;
			//flag_work_mode_template=CLEANING_TIME;
			flag_cleaning_mode=CLEANING_TIME;
		 }
		 else
		   {
			   
			  if (eWork_Mode!=W_IGNITION)
			  {
				   cleaning_delay_timer++;
				   // flag_work_mode_template=0;
			  }
			  
			 
		   }
		 
		 
		 
		  
		  
	   //------------------------------------------------------------------ Alarm -------------------------------------------------//
	    if(flag_alarm&&eWork_Mode!=W_MONITORING&&eWork_Mode!=W_AFTERBURNING&&eWork_Mode!=W_ATTENUATION&&eWork_Mode!=W_CLEANING&&eWork_Mode!=W_EXTINGUISHING)
		{	  
		   if (flag_alarm&(1<<SHNEK_SENSOR_ALARM)||flag_alarm&(1<<BURNER_SENSOR_ALARM))
		    {
		    	eWork_Mode=W_ATTENUATION;
			
	   	    }
		   	else
			   {
				   eWork_Mode=W_AFTERBURNING;
			   }   
			
			timer_template_var=0;
			
		}
		  
	  else// if not alarm
	  {
		   //--------------------------------------------- CO and CWU in Samer and Winter mode ----------------------//
		   if(!(flag_yes_no&(1<<SESON_DEF)))// if WINTER mode, use CO and CWU flag_yes_no&(1<<SELECT_CWU_DEF)
		   {
			   //----------------CO ------------------------//
			   if (kotel_temperature<=(kotel_temp_overheating-kotel_protect)&&(!(flag_yes_no&(1<<PUMP_HEATING_DEF))) )
			   {
				   if (IS_TERMOSTAT_ON&&kotel_temperature>=kotel_min_temp)// 3 celsium- hysteresys
				   {
					   CO_ON;
				   }
				   else
				   CO_OFF;
				   
				   flag_alarm&=~(1<<NO_SELECT_CWU_IN_SUMMER);
			   }
			   else if (kotel_temperature<=(kotel_temp_overheating-kotel_protect)&&(flag_yes_no&(1<<PUMP_HEATING_DEF)))// Termostat work
			   {
				   if (home_temperature<=setup_boiler_temperature-2)// HYSTERESYS - 5 CELCIUM
				   {
					   CO_ON;
				   }
				   else if (home_temperature>=setup_boiler_temperature)
				   {
					   CO_OFF;
				   }                                                               // PUMP_HEATING_DEF - 0 ->THERM mode
				                                                                   //                  - 1 ->AUTO mode
				   
			   }
			   else if (kotel_temperature>=kotel_temp_overheating)
			   {
				   CO_OFF;
				   if (IS_ELECTROTEN_ON)
				   {
					   ELECTROTEN_OFF;
				   }
			   }
			  // else
			  // CO_ON;
			   //-----------------CWU --------//
			   
			   if(flag_yes_no&(1<<SELECT_CWU_DEF))
			   {
				   if (boiler_temperature<=(m_hot_water-pump_hot_water_hystesis_work)&&kotel_temperature>kotel_min_temp&&!flag_alarm)//  temperatura gwu < setup temperature and t. gwu > temp. kotla
				   {
					   CWU_ON; // Hot water pump ON
				   }
				   
				   else if (boiler_temperature>=m_hot_water||flag_alarm&(1<<BOILER_SENSOR_ALARM)||kotel_temperature<=(kotel_min_temp-kotel_protect))
				   {
					   CWU_OFF;
				   }
			   }
			   
		   }
		   else// SUMMER mode , use CWU only!
		   {
			   CO_OFF;
			   if(flag_yes_no&(1<<SELECT_CWU_DEF))//
			   {
				   if (boiler_temperature<=(m_hot_water-pump_hot_water_hystesis_work)&&kotel_temperature>kotel_min_temp&&!flag_alarm)//  temperatura gwu < setup temperature and t. gwu > temp. kotla
				   {
					   CWU_ON; // Hot water pump ON
				   }
				   
				   else if (boiler_temperature>=m_hot_water||flag_alarm&(1<<BOILER_SENSOR_ALARM)||kotel_temperature<=(kotel_min_temp-kotel_protect))
				   {
					   CWU_OFF;
				   }
				   else if (kotel_protect>=kotel_temp_overheating)
				   {
					   CWU_OFF;
					   flag_alarm|=(1<<OWER_HEATING_ALARM);
				   }
				   flag_alarm&=~(1<<NO_SELECT_CWU_IN_SUMMER);
			   }
			   
			   else
			      flag_alarm|=(1<<NO_SELECT_CWU_IN_SUMMER);
		   }
		   //--------------------------------------------------
		   
		   
		   
		   
		   if (kotel_temperature>=(kotel_max_temp+kotel_protect) &&!(flag_alarm&(1<<KOTEL_SENSOR_ALARM))  )
		   {
			   if (eWork_Mode!=W_AFTERBURNING&&eWork_Mode!=W_ATTENUATION&&eWork_Mode!=W_CLEANING&&eWork_Mode!=W_EXTINGUISHING)
			   {
				   eWork_Mode=W_MONITORING;
				   timer_template_var=0;
				   timer_template_stoker_dose=0;
				   timer_template_dose=0;
			   }
		   }
		   
		   
		   
	  }
	  
	  
	  
	  //-------------------------------------------------------- SHNEK ALARM --------------------------------// 
	   if (shnek_temperature>=stoker_fervor&&!flag_alarm)
	   {
		   BURNER_AUGER_ON;
		   flag_alarm|=(1<<SHNEK_OWER_HEAT_ALARM);
		   eWork_Mode=W_EXTINGUISHING;
		   timer_template_var=0;
		   //eSTART_STOP=STOP;
	   }
	   //-------------------------
	   
	  switch (eWork_Mode)
	   {
		   case W_MONITORING://1
		   { 
			   if(flag_alarm)
			   {
			     eWork_Mode=W_AFTERBURNING;
				 eUpdate=UPDATE;
				 if (IS_ELECTROTEN_ON||flag_yes_no&(1<<FUEL_DEF))         // if ten on, we must off then; or fuel==WOOD
				 {
					 ELECTROTEN_OFF;
				 }
				 break;
			   }	
				 
			  if(flag_yes_no&(1<<SELECT_CWU_DEF))// if selected CWU
			  {  
			    if (kotel_temperature<=kotel_min_temp||kotel_temperature<=(m_hot_water-pump_hot_water_hystesis_work))//--------------------------------------- if temp kotel < temp min_kotel. Go to IGNITION------//
			    {
				   eWork_Mode=W_IGNITION;
				   eUpdate=UPDATE;
			
				   //break;
			    }
			  }
			  else// if no selected CWU
			   {
				 if (kotel_temperature<=kotel_min_temp)//--------------------------------------- if temp kotel < temp min_kotel. Go to IGNITION------//
				 {
					 eWork_Mode=W_IGNITION;
					 eUpdate=UPDATE;
					 
					// break;
				 }  
			   }
			   
			   if (flag_fans_work)//  FAns work off
			   {
				   flag_fans_work=0;
				   EICRB&=~((1<<ISC61)|(1<<ISC60));
				   EIMSK&=~(1<<INT6); //Interrupt  INT6 ENABLE
				   
				   
			   }

			 break;
		   }// end monitoring case
		   
		  ///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
		   
		   
		   //---------------------------------------------------------------------CASE Ignition--------------------//
		   
		   
		   
		   case W_IGNITION://2
		   {
			   
			//--------------------------------------------------- Flame ----------------------------------//
			   if (time_sec>=25)
			   {
				   flame_y_position--;
				   time_sec=0;
			   }
			   else
			     time_sec++;
				 
			   if (flame_y_position<=37)
			   {
				   flame_y_position=42;
			   }	 
			//---------------------------------------------------------------------------------------------// 
			
			
			
			
			//---------------------------------------- STOKER burner and bunker  ---------------------------------//
			//////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			
			 if (!(flag_yes_no&(1<<FUEL_DEF)))//  PELLET
			 {
				
				 
				 if (flag_yes_no_2&(1<<FLAG_START_DOSE))// FIRST START 
				 {
					 
					 
					 
					 if ( (stoker_time_continuation>=supply_cycle || start_dose>=supply_cycle ) && flag_yes_no_2&(1<<STOKER_WORK_MODE_DEF))// AUTO
					 {
						  flag_yes_no_2&=~(1<<FLAG_START_DOSE);// END START DOSE
						  if (!flag_fans_work)
						  {
							  flag_fans_work=1;
						  }
						  
						  if (!(flag_yes_no&(ELECTROTEN_WORK_DEF)))
						  {
							  flag_yes_no|=(1<<ELECTROTEN_WORK_DEF); // we can turn on electroten
							  ELECTROTEN_ON;
						  }
					 }
					 
					 
					 
					 //------------------------------------------------ Burner  -----------------------------------//
					 
					 
					 if (!(flag_yes_no_2&(1<<STOKER_WORK_MODE_DEF))) // if stoker_work_mode == 0 (cycle mode)
					 {
						 
						 
						 if(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF))// ON
						 {
							 if (timer_template_stoker_dose<=(stoker_time_work/0.008))
							 {
								 BURNER_AUGER_ON;
								 timer_template_stoker_dose++;
							 }
							 else
							 {
								 timer_template_stoker_dose=0;
								 flag_yes_no_2&=~(1<<STOKER_WORK_DELAY_DEF);
								 
							 }
						 }
						 
						 else// OFF
						 {
							 if (timer_template_stoker_dose<=(stoker_time_delay/0.008))
							 {
								 BURNER_AUGER_OFF;
								 timer_template_stoker_dose++;
								 
								 
								 flag_yes_no_2&=~(1<<FLAG_START_DOSE);// END START DOSE
								 if (!flag_fans_work)
								 {
									 flag_fans_work=1;
								 }
								 
								 if (!(flag_yes_no&(ELECTROTEN_WORK_DEF)))
								 {
									 flag_yes_no|=(1<<ELECTROTEN_WORK_DEF); // we can turn on electroten
								 }
								 
							 }
							 else
							 {
								 timer_template_stoker_dose=0;
								 flag_yes_no_2|=(1<<STOKER_WORK_DELAY_DEF);
								 
							 }
						 }
						 
						 
					 }
					 else // stoker work mode==AUTO;  1
					 {
						 if(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF)) 
						 {
							
							 if (timer_template_dose>=stoker_time_emptying/0.008)
							 {
								 flag_yes_no_2&=~(1<<STOKER_WORK_DELAY_DEF);
								 BURNER_AUGER_OFF;
								 
								 if (stoker_time_continuation>=start_dose)
								 {
									 flag_yes_no_2&=~(1<<FLAG_START_DOSE);// END START DOSE
									 if (!flag_fans_work)
									 {
										 flag_fans_work=1;
									 }
									 
									 if (!(flag_yes_no&(ELECTROTEN_WORK_DEF)))
									 {
										 flag_yes_no|=(1<<ELECTROTEN_WORK_DEF); // we can turn on electroten
										 ELECTROTEN_ON;
									 }
									 
								 }
							 }
							 
							 else
							  {
								  if (IS_BURNER_AUGER_OFF)
								  {
									  BURNER_AUGER_ON;
								  }
							  }
							 
							 
						 }
					 }
				//-------------------------------------------------- Bunker ------------------------------------//	 
					
					if(timer_template_dose<=start_dose/0.008)
					{
						if(!(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF)) &&flag_yes_no_2&(1<<STOKER_WORK_MODE_DEF))// PELLET OFF;  AUTO mode
						{
							
							flag_yes_no_2|=(1<<STOKER_WORK_DELAY_DEF);
							
						}
						
						BUNKER_AUGER_ON;
						
						
						timer_template_dose++;
						eUpdate=UPDATE;
					}
					
					else if (timer_template_dose<(supply_cycle/0.008)&&timer_template_dose>(start_dose/0.008))
					{
						BUNKER_AUGER_OFF; //  BUNKER_OFF
						timer_template_dose++;
						
						
						
						if (start_dose>stoker_time_continuation)
						{
							
							
							flag_yes_no_2&=~(1<<FLAG_START_DOSE);// END START DOSE
							if (!flag_fans_work)
							{
								flag_fans_work=1;
							}
							
							if (!(flag_yes_no&(ELECTROTEN_WORK_DEF)))
							{
								flag_yes_no|=(1<<ELECTROTEN_WORK_DEF); // we can turn on electroten
								ELECTROTEN_ON;
							}
							
							
							
							
						}
						
						eUpdate=UPDATE;
					}
					
					else if (timer_template_dose>=supply_cycle/0.008)
					{
						timer_template_dose=0;
						//flag_yes_no_2&=~(1<<FLAG_START_DOSE);// End start dose
						
					}
					 
					 
					/////////////////////////////////////////////// END START DOSE ///////////////////////////////////////////// 
					 
				 }
				 
				 else// CONTINION 
				 {
					 if(burner_temperature<m_burner_temp-lighter_hysteresis-5)
					 {
						 ELECTROTEN_ON;
					 }
					
					//------------------------------------------------------- Burner  ---------------------//
					
					if (!(flag_yes_no_2&(1<<STOKER_WORK_MODE_DEF))) // if stoker_work_mode == 0 (cycle mode)
					{
						
						
						if(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF))// if stoker bunker on,
						{
							if (timer_template_stoker_dose<=(stoker_time_work/0.008))
							{
								BURNER_AUGER_ON;
								timer_template_stoker_dose++;
							}
							else
							{
								timer_template_stoker_dose=0;
								flag_yes_no_2&=~(1<<STOKER_WORK_DELAY_DEF);
								
							}
						}
						
						else//  
						{
							if (timer_template_stoker_dose<=(stoker_time_delay/0.008))
							{
								BURNER_AUGER_OFF;
								timer_template_stoker_dose++;
							}
							else
							{
								timer_template_stoker_dose=0;
								flag_yes_no_2|=(1<<STOKER_WORK_DELAY_DEF);
								
							}
						}
						
						
						
					}
					
						
					else // stoker work mode==AUTO;  1
					{
							if(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF))
							{
								
								if (timer_template_dose>= (stoker_time_continuation/0.008) )
								{
									flag_yes_no_2&=~(1<<STOKER_WORK_DELAY_DEF);
									BURNER_AUGER_OFF;
									
									if (stoker_time_continuation>=start_dose)
									{
										flag_yes_no_2&=~(1<<FLAG_START_DOSE);// END START DOSE
										if (!flag_fans_work)
										{
											flag_fans_work=1;
										}
										
										if (!(flag_yes_no&(ELECTROTEN_WORK_DEF)))
										{
											flag_yes_no|=(1<<ELECTROTEN_WORK_DEF); // we can turn on electroten
										}
										
									}
								}
								
								else
								{
									if (IS_BURNER_AUGER_OFF)
									{
										BURNER_AUGER_ON;
									}
								}
								
								
							}
					}

					//--------------------------------------------------------
					
					//----------------------------------------------------------BUNKER ------------------------------------------------------------//
					
					if(timer_template_dose<=((dose_min_power*supply_cycle)/100)/0.008)
					{
						if(!(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF)) &&flag_yes_no_2&(1<<STOKER_WORK_MODE_DEF))// if Pellets stoker off and auto mode
						{
							
							flag_yes_no_2|=(1<<STOKER_WORK_DELAY_DEF);
							
						}
						
						BUNKER_AUGER_ON;
						//BURNER_AUGER_ON;
						
						
						timer_template_dose++;
						eUpdate=UPDATE;
					}
					
					else if (timer_template_dose<(supply_cycle/0.008)&&timer_template_dose>( ((dose_min_power*supply_cycle)/100)/0.008) )
					{
						//BURNER_AUGER_OFF;
						BUNKER_AUGER_OFF; //  shnek on
						timer_template_dose++;
						if (!(flag_yes_no&(ELECTROTEN_WORK_DEF)))
						{
							flag_yes_no|=(1<<ELECTROTEN_WORK_DEF); // we can turn on electroten
							ELECTROTEN_ON;
						}
						if (!flag_fans_work)
						{
							flag_fans_work=1;
						}
						eUpdate=UPDATE;
					}
					
					else if (timer_template_dose>=supply_cycle/0.008)
					{
						timer_template_dose=0;
						
					}
					 
				 }
				 
				 
			 }// end if selected pelets mode
			 
			 
			 //-------------------------------------------------------- WOOD fuel --------------------------------//
			 else // WOOD
			 {
				BUNKER_AUGER_OFF;
				BURNER_AUGER_OFF; 
				ELECTROTEN_OFF;
				flag_fans_work=1;
			 }
			  
			//-------------------------------------------------------------end shnek pelets and bunker--------------//  
			 
			
			
			 
			 
			//------------------------------------------------------------control temperature ----------------------// 
			 
			   //------1. Burner  
			if(burner_temperature>=(m_burner_temp-lighter_hysteresis) && !(flag_alarm&(1<<BURNER_SENSOR_ALARM))) 
			 { 
			   if (burner_temperature>=m_burner_temp)  //if burner temperature > setup burner temperature (electroten off)
			   {
				 if(IS_ELECTROTEN_ON) 
				 { 
				   ELECTROTEN_OFF;
				   flag_yes_no&=~(1<<ELECTROTEN_WORK_DEF);
				 }
			   }
			   
				eUpdate=UPDATE;
				if(flag_yes_no&(1<<IGNITION_STAB_DEF))
				   eWork_Mode=W_IGNITION_STABILIZ;
				else
				   eWork_Mode=W_MAX_POWER;
				 
				   
				timer_template_var=0;
				kindli_time_template=0; // kindli counter reset to 0
				break;
			   //}
			 }
			 else if (flag_alarm&(1<<BURNER_SENSOR_ALARM))
			 {
				 eWork_Mode=W_CLEANING;
			 }
			 
			 
			//--------------------------------   
			
			   //2. CO and CWU
			 if (!(flag_yes_no&(1<<SESON_DEF)))// if seson== WINTER
			 {
				 if(flag_yes_no&(1<<SELECT_CWU_DEF)) //  With hot water function
				 {
					 if (m_hot_water>kotel_max_temp)//if T boiler> T kotel max 
					 {
						 if(kotel_temperature>=m_hot_water)
						 {
							 eWork_Mode=W_MIN_POWER;
							 eUpdate=UPDATE;
							 
							 if(IS_ELECTROTEN_ON)
							 {
								 ELECTROTEN_OFF;
								 flag_yes_no&=~(1<<ELECTROTEN_WORK_DEF);
							 }
						 }
						 
						 //break;
					 }
				   }
				 
				 else//  Without hot water function
				 {
					 if (kotel_temperature>=kotel_max_temp)
					 {
						 eWork_Mode=W_MIN_POWER;
						 eUpdate=UPDATE;
						 if(IS_ELECTROTEN_ON)
						 {
							 ELECTROTEN_OFF;
							 flag_yes_no&=~(1<<ELECTROTEN_WORK_DEF);
						 }
					 }
					 
					 // break;
				 }
				 //---
			 }
			 else// if season== SUMMER
			  {
				 if(flag_yes_no&(1<<SELECT_CWU_DEF)) //  With hot water function
				 {
					 if (m_hot_water>=kotel_max_temp)//if T boiler> T kotel max
					 {
						 if(kotel_temperature>=m_hot_water)
						 {
							 eWork_Mode=W_MIN_POWER;
							 eUpdate=UPDATE;
							 
							 if(IS_ELECTROTEN_ON)
							 {
								 ELECTROTEN_OFF;
								 flag_yes_no&=~(1<<ELECTROTEN_WORK_DEF);
							 }
						 }
						 
						 //break;
					 } 
					 
				  }
				  else
				  {
					  flag_alarm|=(1<<NO_SELECT_CWU_IN_SUMMER);
				  }
				  
			  }
			
			//------------------------------ 
           
            if (timer_template_var>=((lighter_kindling_time*60)/0.008))  //15m * 60 =900 max time;  15m* 60 /0.008 = 112500
            {
				
			    ++kindli_time_template;
				
				 if (kindli_time_template>=lighter_attempt_kindling) // counter> kindling time - alarm, empty fuel ERR-5
				 {
					 eWork_Mode=W_CLEANING;
					 ELECTROTEN_OFF;
					 eUpdate=UPDATE;
					 flag_cleaning_mode=0;
					 
					 flag_alarm|=(1<<FUEL_SHORTAGE_DEF);
					 flag_yes_no&=~(1<<ELECTROTEN_WORK_DEF);
					 BURNER_AUGER_OFF;
					 BUNKER_AUGER_OFF;
					 timer_template_var=0;
				 }
				
				//break;
            }
			else
			   timer_template_var++;
			//---
			
			//---------------------------------------------------- TEN work ---------------------------//
			
			if (flag_yes_no&(1<<ELECTROTEN_WORK_DEF))
			{
				ELECTROTEN_ON;
			}
			
			//---------------------------------------------------- FAN1 and FAN2 ---------------------------------//
			if (flag_fans_work)// FANs work start
			{
				
				if (flag_yes_no&(1<<FAN2_YES_NO_DEF))// if selected FAN2 
				{
					if (!(flag_yes_no&(1<<FUEL_DEF)))// if  pellets mode
					{
						if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						{
							EICRB|=((1<<ISC61)|(1<<ISC60));
							EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						}
						
						if (flag_yes_no&(1<<PELETS_FAN_DEF))// if selected FAN1+FAn2
						{
							fan1_spead_var=rotation_ignition;
							fan2_spead_var=fan2_rotation_ignition;
						}
						else
						{
							fan1_spead_var=rotation_ignition;
						}
						
					}
					else//  WOOD mode
					    {
							if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
							{
								EICRB|=((1<<ISC61)|(1<<ISC60));
								EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
							}
							
							if (lighter_woods_fan==0)//   FAN1
							{
								fan1_spead_var=rotation_ignition;
							}
							else if (lighter_woods_fan==1)//   FAN2
							{
								fan2_spead_var=fan2_rotation_ignition;
							}
							else//  FAN1+ FAN2
							{
								fan1_spead_var=rotation_ignition;
								fan2_spead_var=fan2_rotation_ignition;
							}
							
						}
					
						
				}
			    else
			   {
				   fan1_spead_var=rotation_ignition;
				   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
				   {
					   EICRB|=((1<<ISC61)|(1<<ISC60));
					   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
				   }
				   
				   if (timer_template_var>=((lighter_kindling_time*60)/0.008))  //15m * 60 =900 max time;  15m* 60 /0.008 = 112500
				   {
					   
					   ++kindli_time_template;
					   if (kindli_time_template>=lighter_attempt_kindling) // counter> kindling time - alarm, empty fuel ERR-5
					   {
						   eWork_Mode=W_CLEANING;
						   flag_cleaning_mode=0;
						   
						   ELECTROTEN_OFF;
						   eUpdate=UPDATE;
						   
						   flag_alarm|=(1<<FUEL_SHORTAGE_DEF);
						   flag_yes_no&=~(1<<ELECTROTEN_WORK_DEF);
						  // BURNER_AUGER_OFF;
						   //BUNKER_AUGER_OFF;
						   timer_template_var=0;
					   }
					   
					   //break;
				   }
				   else
				   {
					   flag_yes_no&=~(1<<ELECTROTEN_WORK_DEF);
					    timer_template_var++;
				   }
				  
				   //------------------------------------------
			   }		
				
			}
			
				 
			 break;
		   }//---------------------------end------------------//
		   
		   
		   //-------------------------------------------------------- case Ignition stab mode ----------------------------------------------//
		   case W_IGNITION_STABILIZ://3
		   {
			   //--------------------------------------------------- Flame ----------------------------------//
			   if (time_sec>=20)
			   {
				   flame_y_position--;
				   time_sec=0;
			   }
			   else
			   time_sec++;
			   
			   if (flame_y_position<=37)
			   {
				   flame_y_position=41;
			   }
			   //---------------------------------------------------------------------------------------------//
			   
			   //---------------------------------------------------- FAN1 and FAN2 ---------------------------------//
			   if (flag_fans_work)// FANs work start
			   {
				   
				   if (flag_yes_no&(1<<FAN2_YES_NO_DEF))// if selected FAN2
				   {
					   if (!(flag_yes_no&(1<<FUEL_DEF)))// if  pellets mode
					   {
						   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						   {
							   EICRB|=((1<<ISC61)|(1<<ISC60));
							   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						   }
						   
						   if (flag_yes_no&(1<<PELETS_FAN_DEF))// if selected FAN1+FAn2
						   {
							   fan1_spead_var=max_rotation_ignition;
							   fan2_spead_var=fan2_max_rotation_ignition;
						   }
						   else
						   {
							   fan1_spead_var=max_rotation_ignition;
						   }
						   
					   }
					   else//  WOOD mode
					   {
						   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						   {
							   EICRB|=((1<<ISC61)|(1<<ISC60));
							   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						   }
						   
						   if (lighter_woods_fan==0)//   FAN1
						   {
							   fan1_spead_var=max_rotation_ignition;
						   }
						   else if (lighter_woods_fan==1)//   FAN2
						   {
							   fan2_spead_var=fan2_max_rotation_ignition;
						   }
						   else//  FAN1+ FAN2
						   {
							   fan1_spead_var=max_rotation_ignition;
							   fan2_spead_var=fan2_max_rotation_ignition;
						   }
						   
					   }
					   
					   
				   }
				   else// 
				   {
					   fan1_spead_var=rotation_ignition;
					   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
					   {
						   EICRB|=((1<<ISC61)|(1<<ISC60));
						   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
					   }
					   
				   }
				   
			   }
		
			   
			   
			   //-------------------------------------------- Burner temperature>=setup burner temperature ----------------------------//
			   
			   if (burner_temperature>=m_burner_temp||flag_yes_no&(1<<FUEL_DEF))  //if burner temperature > setup burner temperature (electroten off)
			   {
				   if(IS_ELECTROTEN_ON)
				   {
					   ELECTROTEN_OFF;
					   flag_yes_no&=~(1<<ELECTROTEN_WORK_DEF);
				   }
				   eUpdate=UPDATE;
			   
			   }
			  
			 
			 
			   
            //---------------------------------------- Shnek burner and bunker  ---------------------------------//
            
            if (!(flag_yes_no&(1<<FUEL_DEF)))//PELLETS
            {
	            //------------------------------------------------------- Burner  ---------------------//
	            
	            if (!(flag_yes_no_2&(1<<STOKER_WORK_MODE_DEF))) // if stoker_work_mode == 0 (cycle mode)
	            {
		            
		            
		            if(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF))// if stoker bunker on,
		            {
			            if (timer_template_stoker_dose<=(stoker_time_work/0.008))
			            {
				            BURNER_AUGER_ON;
				            timer_template_stoker_dose++;
			            }
			            else
			            {
				            timer_template_stoker_dose=0;
				            flag_yes_no_2&=~(1<<STOKER_WORK_DELAY_DEF);
				            
			            }
		            }
		            
		            else
		            {
			            if (timer_template_stoker_dose<=(stoker_time_delay/0.008))
			            {
				            BURNER_AUGER_OFF;
				            timer_template_stoker_dose++;
			            }
			            else
			            {
				            timer_template_stoker_dose=0;
				            flag_yes_no_2|=(1<<STOKER_WORK_DELAY_DEF);
				            
			            }
		            }
		            
		            
		            
	            }
	            else // stoker work mode==AUTO;  1
	            {
		            if(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF))
		            {
			            
			            if (timer_template_dose>=stoker_time_continuation/0.008)
			            {
				            flag_yes_no_2&=~(1<<STOKER_WORK_DELAY_DEF);
				            BURNER_AUGER_OFF;
				            
				            if (stoker_time_continuation>=start_dose)
				            {
					            flag_yes_no_2&=~(1<<FLAG_START_DOSE);// END START DOSE
					            if (!flag_fans_work)
					            {
						            flag_fans_work=1;
					            }
					            
					            if (!(flag_yes_no&(ELECTROTEN_WORK_DEF)))
					            {
						            flag_yes_no|=(1<<ELECTROTEN_WORK_DEF); // we can turn on electroten
					            }
					            
				            }
			            }
			            
			            else
			            {
				            if (IS_BURNER_AUGER_OFF)
				            {
					            BURNER_AUGER_ON;
				            }
			            }
			            
			            
		            }
	            }

	            //--------------------------------------------------------
	            
	            //----------------------------------------------------------BUNKER ------------------------------------------------------------//
	            
	            if(timer_template_dose<=((dose_min_power*supply_cycle)/100)/0.008)
	            {
		            if(!(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF)) &&flag_yes_no_2&(1<<STOKER_WORK_MODE_DEF))// if Pellets stoker off and auto mode
		            {
			            
			            flag_yes_no_2|=(1<<STOKER_WORK_DELAY_DEF);
			            
		            }
		            
		            BUNKER_AUGER_ON;
		            //BURNER_AUGER_ON;
		            
		            
		            timer_template_dose++;
		            eUpdate=UPDATE;
	            }
	            
	            else if (timer_template_dose<(supply_cycle/0.008)&&timer_template_dose>( ((dose_min_power*supply_cycle)/100)/0.008) )
	            {
		            //BURNER_AUGER_OFF;
		            BUNKER_AUGER_OFF; //  shnek on
		            timer_template_dose++;
		            if (!(flag_yes_no&(ELECTROTEN_WORK_DEF)))
		            {
			            flag_yes_no|=(1<<ELECTROTEN_WORK_DEF); // we can turn on electroten
		            }
		            if (!flag_fans_work)
		            {
			            flag_fans_work=1;
		            }
		            eUpdate=UPDATE;
	            }
	            
	            else if (timer_template_dose>=supply_cycle/0.008)
	            {
		            timer_template_dose=0;
		            
	            }
	            
           // }
	            
            }// end if selected pelets mode
            
			
            else //if selected woods fuel, all shnek off
            {
	            BUNKER_AUGER_OFF;
	            BURNER_AUGER_OFF;
				ELECTROTEN_OFF;
	            flag_fans_work=1;
            }
            
			
			
            //-------------------------------------------------------------end shnek pelets and bunker--------------//
			
			
			
			
			  //----------------------------------------------------------------Control TEMPERATURE-------------------//
			  
			  
			   if(flag_yes_no&(1<<SESON_DEF)) //  SUMMER
			   {
	               if (boiler_temperature>=m_hot_water)
	               {
					   timer_template_var=0;
					   eWork_Mode=W_MIN_POWER;
					   eUpdate=UPDATE;
					   break;
	               }
			   }
			   
			   else//  WINTER
			   {
				   if (flag_yes_no&(1<<SELECT_CWU_DEF))
				   {
					   if (flag_yes_no&(1<<PUMP_HEATING_DEF))// AUTO mode
					   {
						   if (home_temperature>=setup_boiler_temperature)
						   {
							   timer_template_var=0;
							   eWork_Mode=W_MIN_POWER;
							   eUpdate=UPDATE;
							   break;
						   }
						   
					   }
					   else// THERMOSTAT mode
					   {
						   if (IS_TERMOSTAT_OFF)
						   {
							   timer_template_var=0;
							   eWork_Mode=W_MIN_POWER;
							   eUpdate=UPDATE;
							   break;
						   }
						   
					   }
					   
					   if (boiler_temperature>=m_hot_water)
					   {
						   timer_template_var=0;
						   eWork_Mode=W_MIN_POWER;
						   eUpdate=UPDATE;
						   break;
					   }
					   
				   }
				   
				  else
				   {
					   if (flag_yes_no&(1<<PUMP_HEATING_DEF))// AUTO mode
					   {
						   if (home_temperature>=setup_boiler_temperature)
						   {
							   timer_template_var=0;
							   eWork_Mode=W_MIN_POWER;
							   eUpdate=UPDATE;
							   break;
						   }
						   
					   }
					   else// THERMOSTAT mode
					   {
						   if (IS_TERMOSTAT_OFF)
						   {
							   timer_template_var=0;
							   eWork_Mode=W_MIN_POWER;
							   eUpdate=UPDATE;
							   break;
						   }
						   
					   }
				   }
				   
				   
				   
			   }
			   //------------------------------------------------------------------------------------//
			   
			   
			   if (timer_template_var>=((lighter_time_ignition_stab*60)/0.008)) // if timer ignition stabilization over, go to W_MAX_POWER ;  in minute 
			   {
				   timer_template_var=0;
				   eWork_Mode=W_MAX_POWER;
				   eUpdate=UPDATE;
				   break;
			   }
			   else
			      timer_template_var++;
			   
			   break;
		   }
		   
		   
		   
		   case W_MAX_POWER://4 ----------------------------W_MAX_POWER
		   {
			   //--------------------------------------------------- Flame ----------------------------------//
			   if (time_sec>=20)
			   {
				   flame_y_position--;
				   time_sec=0;
			   }
			   else
			       time_sec++;
			   
			   if (flame_y_position<=32)
			   {
				   flame_y_position=43;
			   }
			   
			   //-------------------------------------------------------------------------------------------------------//
			   
			   blowing_timer=0;
			   fan2_blowing_timer=0;
			   
			   
			    //--------------------------------------------- Burner temperature control -----------------------------//
			    
			    if ( !(flag_yes_no&(1<<LIGHTER_FLAME_DELAY_DEF)) )
			    {
				    if (burner_temperature<=lighter_hysteresis)// don't find flame
				    {
					    flag_yes_no|=(1<<LIGHTER_FLAME_DELAY_DEF);
				    }
				    
				    else
				    {
					    if (timer_template_var!=0)
					    {
						    timer_template_var=0;
					    }
				    }
			    }
				
				else if (burner_temperature>=m_burner_temp)  //if burner temperature > setup burner temperature (electroten off)
				{
					if(IS_ELECTROTEN_ON)
					{
						ELECTROTEN_OFF;
						flag_yes_no&=~(1<<ELECTROTEN_WORK_DEF);
					}
				}
				
			    else
			    {
				    if (timer_template_var>=(lighter_flame_delay/0.008))//
				    {
					    if (burner_temperature<=lighter_hysteresis)
					    {
						    eWork_Mode=W_IGNITION;
						    eUpdate=UPDATE;
						    timer_template_var=0;
						    flag_yes_no&=~(1<<LIGHTER_FLAME_DELAY_DEF);
						    blowing_timer=0;
						    cleaning_delay_timer=0;
						    
						    break;
					    }
					    
				    }
				    else
				      timer_template_var++;
			    }
			   
			   //---------------------------------------------------- FAN1 and FAN2 ---------------------------------//
			   if (flag_fans_work)// FANs work start
			   {
				   
				   if (flag_yes_no&(1<<FAN2_YES_NO_DEF))// if selected FAN2
				   {
					   if (!(flag_yes_no&(1<<FUEL_DEF)))// if  pellets mode
					   {
						   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						   {
							   EICRB|=((1<<ISC61)|(1<<ISC60));
							   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						   }
						   
						   if (flag_yes_no&(1<<PELETS_FAN_DEF))// if selected FAN1+FAn2
						   {
							   fan1_spead_var=max_rotation_ignition;
							   fan2_spead_var=fan2_max_rotation_ignition;
						   }
						   else
						   {
							   fan1_spead_var=max_rotation_ignition;
						   }
						   
					   }
					   else//  WOOD mode
					   {
						   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						   {
							   EICRB|=((1<<ISC61)|(1<<ISC60));
							   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						   }
						   
						   if (lighter_woods_fan==0)//   FAN1
						   {
							   fan1_spead_var=max_rotation_ignition;
						   }
						   else if (lighter_woods_fan==1)//   FAN2
						   {
							   fan2_spead_var=fan2_max_rotation_ignition;
						   }
						   else//  FAN1+ FAN2
						   {
							   fan1_spead_var=max_rotation_ignition;
							   fan2_spead_var=fan2_max_rotation_ignition;
						   }
						   
					   }
					   
					   
				   }
				   else//
				   {
					   fan1_spead_var=rotation_ignition;
					   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
					   {
						   EICRB|=((1<<ISC61)|(1<<ISC60));
						   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
					   }
					   
				   }
				   
			   }
			   //-------------------------------------------------------------------------------------------------------------------//
			   
			   
			   
			   
			   
			   //---------------------------------------- Shnek burner and bunker  ---------------------------------//
			   
			   if (!(flag_yes_no&(1<<FUEL_DEF)))//PELLETS
			   {
				   //------------------------------------------------------- Burner  ---------------------//
				   
				   if (!(flag_yes_no_2&(1<<STOKER_WORK_MODE_DEF))) // if stoker_work_mode == 0 (cycle mode)
				   {
					   
					   
					   if(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF))// if stoker bunker on,
					   {
						   if (timer_template_stoker_dose<=(stoker_time_work/0.008))
						   {
							   BURNER_AUGER_ON;
							   timer_template_stoker_dose++;
						   }
						   else
						   {
							   timer_template_stoker_dose=0;
							   flag_yes_no_2&=~(1<<STOKER_WORK_DELAY_DEF);
							   
						   }
					   }
					   
					   else
					   {
						   if (timer_template_stoker_dose<=(stoker_time_delay/0.008))
						   {
							   BURNER_AUGER_OFF;
							   timer_template_stoker_dose++;
						   }
						   else
						   {
							   timer_template_stoker_dose=0;
							   flag_yes_no_2|=(1<<STOKER_WORK_DELAY_DEF);
							   
						   }
					   }
					   
					   
					   
				   }
				   else // stoker work mode==AUTO;  1
				   {
					   if(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF))
					   {
						   
						   if (timer_template_dose>=stoker_time_continuation/0.008)
						   {
							   flag_yes_no_2&=~(1<<STOKER_WORK_DELAY_DEF);
							   BURNER_AUGER_OFF;
							   
							   if (stoker_time_continuation>=start_dose)
							   {
								   flag_yes_no_2&=~(1<<FLAG_START_DOSE);// END START DOSE
								   if (!flag_fans_work)
								   {
									   flag_fans_work=1;
								   }
								   
								   if (!(flag_yes_no&(ELECTROTEN_WORK_DEF)))
								   {
									   flag_yes_no|=(1<<ELECTROTEN_WORK_DEF); // we can turn on electroten
								   }
								   
							   }
						   }
						   
						   else
						   {
							   if (IS_BURNER_AUGER_OFF)
							   {
								   BURNER_AUGER_ON;
							   }
						   }
						   
						   
					   }
				   }

				   //--------------------------------------------------------
				   
				   //----------------------------------------------------------BUNKER ------------------------------------------------------------//
				   
				   if(timer_template_dose<=((dose_max_power*supply_cycle)/100)/0.008)
				   {
					   if(!(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF)) &&flag_yes_no_2&(1<<STOKER_WORK_MODE_DEF))// if Pellets stoker off and auto mode
					   {
						   
						   flag_yes_no_2|=(1<<STOKER_WORK_DELAY_DEF);
						   
					   }
					   
					   BUNKER_AUGER_ON;
					   //BURNER_AUGER_ON;
					   
					   
					   timer_template_dose++;
					   eUpdate=UPDATE;
				   }
				   
				   else if (timer_template_dose<(supply_cycle/0.008)&&timer_template_dose>( ((dose_max_power*supply_cycle)/100)/0.008) )
				   {
					   //BURNER_AUGER_OFF;
					   BUNKER_AUGER_OFF; //  shnek on
					   timer_template_dose++;
					   if (!(flag_yes_no&(ELECTROTEN_WORK_DEF)))
					   {
						   flag_yes_no|=(1<<ELECTROTEN_WORK_DEF); // we can turn on electroten
					   }
					   if (!flag_fans_work)
					   {
						   flag_fans_work=1;
					   }
					   eUpdate=UPDATE;
				   }
				   
				   else if (timer_template_dose>=supply_cycle/0.008)
				   {
					   timer_template_dose=0;
					   
				   }
				   
			   }
			   
		   //}// end if selected pelets mode
		   
		   
		   else //if selected woods fuel, all shnek off
		   {
			   BUNKER_AUGER_OFF;
			   BURNER_AUGER_OFF;
			   flag_fans_work=1;
		   }
		   
		   
		   
		   
		   
		    //----------------------------------------------------------------Control TEMPERATURE-------------------//
		    
		    
		    if(flag_yes_no&(1<<SESON_DEF)) //  SUMMER
		    {
			    if (boiler_temperature>=m_hot_water && kotel_temperature>kotel_min_temp+kotel_protect)
			    {
				    timer_template_var=0;
				    eWork_Mode=W_MIN_POWER;
				    eUpdate=UPDATE;
				    break;
			    }
		    }
		    
		    else//  WINTER
		    {
				//------------------------------------------ Control ELECTROTEN work ------------------------------//
				
				
			    if (flag_yes_no&(1<<SELECT_CWU_DEF))
			    {
				    if (flag_yes_no&(1<<PUMP_HEATING_DEF))// AUTO mode
				    {
					    if (home_temperature>=setup_boiler_temperature && kotel_temperature>kotel_min_temp+kotel_protect)
					    {
						    timer_template_var=0;
						    eWork_Mode=W_MIN_POWER;
						    eUpdate=UPDATE;
						    break;
					    }
					    
				    }
				    else// THERMOSTAT mode
				    {
					    if (IS_TERMOSTAT_OFF && kotel_temperature>kotel_min_temp+kotel_protect)
					    {
						    timer_template_var=0;
						    eWork_Mode=W_MIN_POWER;
						    eUpdate=UPDATE;
						    break;
					    }
					    
				    }
				    
				    if (boiler_temperature>=m_hot_water && kotel_temperature>kotel_min_temp+kotel_protect)
				    {
					    timer_template_var=0;
					    eWork_Mode=W_MIN_POWER;
					    eUpdate=UPDATE;
					    break;
				    }
				    
			    }
			    
			    else
			    {
				    if (flag_yes_no&(1<<PUMP_HEATING_DEF))// AUTO mode
				    {
					    if (home_temperature>=setup_boiler_temperature && kotel_temperature>kotel_min_temp+kotel_protect)
					    {
						    timer_template_var=0;
						    eWork_Mode=W_MIN_POWER;
						    eUpdate=UPDATE;
						    break;
					    }
					    
				    }
				    else// THERMOSTAT mode
				    {
					    if (IS_TERMOSTAT_OFF && kotel_temperature>kotel_min_temp+kotel_protect)
					    {
						    timer_template_var=0;
						    eWork_Mode=W_MIN_POWER;
						    eUpdate=UPDATE;
						    break;
					    }
					    
				    }
			    }
			    
			    
			    
		    }
		    //------------------------------------------------------------------------------------//
		   
		   /*if (kotel_temperature>=(kotel_max_temp+kotel_top_hystes) )
		   {
			   flag_work_mode_template=W_MIN_POWER;
			   timer_template_var=0;
			   eWork_Mode=W_AFTERBURNING;
			   eUpdate=UPDATE;
			   break;
		   }*/
		   
			   
			   break;
		   }
		   
		   
		   
		   case W_MODULATED_POWER://5
		   {
			   
			   break;
		   }
		   
		   
		   
		   
		   case W_MIN_POWER://6
		   {
			   if (time_sec>=25)
			   {
				   flame_y_position--;
				   time_sec=0;
			   }
			   else
			   time_sec++;
			   
			   if (flame_y_position<=38)
			   {
				   flame_y_position=43;
			   }
			  //----------------------------------------------------- Burner temperature control -----------------------------// 
			 
			  if ( !(flag_yes_no&(1<<LIGHTER_FLAME_DELAY_DEF)) )
			  {
				  if (burner_temperature<=lighter_hysteresis)// don't find flame
				  {
					 flag_yes_no|=(1<<LIGHTER_FLAME_DELAY_DEF);  
				  }
				  
				  else
				  {
					  if (timer_template_var!=0)
					  {
						  timer_template_var=0;
					  }
				  } 
			  }
			  
			  else if (burner_temperature>=m_burner_temp)  //if burner temperature > setup burner temperature (electroten off)
			  {
				  if(IS_ELECTROTEN_ON)
				  {
					  ELECTROTEN_OFF;
					  flag_yes_no&=~(1<<ELECTROTEN_WORK_DEF);
				  }
			  } 
			  
			  else
			  {
				  if (timer_template_var>=(lighter_flame_delay/0.008))//
				  {
					  if (burner_temperature<=lighter_hysteresis)
					  {
						  eWork_Mode=W_IGNITION;
						  eUpdate=UPDATE;
						  timer_template_var=0;
						  flag_yes_no&=~(1<<LIGHTER_FLAME_DELAY_DEF);
						  blowing_timer=0;
						  cleaning_delay_timer=0;
						  
						  break;
					  }
					 
				  }
				  else
				  timer_template_var++;
			  }
			   
			   
			    //-------------------------------------------------- BLOWING FAN1 and FAN2--------------------------------------//
			    
			    if (flag_yes_no&(1<<BLOWING_DEF) && (!(flag_yes_no&(1<<FAN2_YES_NO_DEF)))    )// FAN1
			    {
				    flag_work_mode_template=0;
				    
				    if (blowing_timer>=(delay_time*60)/0.008)// Blowing Time out
				    {
					    eWork_Mode=W_BLOWING;
					    //flag_work_mode_template=W_MIN_POWER;
					    blowing_timer=0;
					    
					    break;
				    }
				    else
				    {
					    blowing_timer++;
				    }
			    }
			    else if ( !(flag_yes_no&(1<<BLOWING_DEF))  &&  (flag_yes_no&(1<<FAN2_YES_NO_DEF)&& flag_yes_no&(1<<FAN2_BLOWING_DEF))  )// FAN2
			    {
				    
				    flag_work_mode_template=1;
				    
				    if (fan2_blowing_timer>=(fan2_delay_time*60)/0.008)// Blowing Time out
				    {
					    eWork_Mode=W_BLOWING;
					    //flag_work_mode_template=W_MIN_POWER;
					    blowing_timer=0;
					    
					    break;
				    }
				    else
				    {
					    fan2_blowing_timer++;
				    }
			    }
			    
			    else if(  (flag_yes_no&(1<<BLOWING_DEF))  &&  (flag_yes_no&(1<<FAN2_YES_NO_DEF)&& flag_yes_no&(1<<FAN2_BLOWING_DEF))   ) //FAN1+FAN2
			    {
				    flag_work_mode_template=2;
				    
				     if (blowing_timer>=(delay_time*60)/0.008)// Blowing Time out
				     {
					     eWork_Mode=W_BLOWING;
					     //flag_work_mode_template=W_MIN_POWER;
					     blowing_timer=0;
					     
					     break;
				     }
				     else
				     {
					     blowing_timer++;
				     }
			   
				}
				else
				{
					fan2_blowing_timer=0;
					blowing_timer=0;
				}
			   
			  //---------------------------------------------------- FAN1 and FAN2 ---------------------------------//
			  if (flag_fans_work)// FANs work start
			  {
				  
				  if (flag_yes_no&(1<<FAN2_YES_NO_DEF))// if selected FAN2
				  {
					  if (!(flag_yes_no&(1<<FUEL_DEF)))// if  pellets mode
					  {
						  if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						  {
							  EICRB|=((1<<ISC61)|(1<<ISC60));
							  EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						  }
						  
						  if (flag_yes_no&(1<<PELETS_FAN_DEF))// if selected FAN1+FAn2
						  {
							  fan1_spead_var=rotation_min_power;
							  fan2_spead_var=fan2_rotation_min_power;
						  }
						  else
						  {
							  fan1_spead_var=rotation_min_power;
						  }
						  
					  }
					  else//  WOOD mode
					  {
						  if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						  {
							  EICRB|=((1<<ISC61)|(1<<ISC60));
							  EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						  }
						  
						  if (lighter_woods_fan==0)//   FAN1
						  {
							  fan1_spead_var=rotation_min_power;
						  }
						  else if (lighter_woods_fan==1)//   FAN2
						  {
							  fan2_spead_var=fan2_rotation_min_power;
						  }
						  else//  FAN1+ FAN2
						  {
							  fan1_spead_var=rotation_min_power;
							  fan2_spead_var=fan2_rotation_min_power;
						  }
						  
					  }
					  
					  
				  }
				  else//
				  {
					  fan1_spead_var=rotation_min_power;
					  if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
					  {
						  EICRB|=((1<<ISC61)|(1<<ISC60));
						  EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
					  }
					  
				  }
				  
			  }
			  //-------------------------------------------------------------------------------------------------------------------//
			  
			  
			  
			  
			  
			  //---------------------------------------- Shnek burner and bunker  ---------------------------------//
			  
			  if (!(flag_yes_no&(1<<FUEL_DEF)))//PELLETS
			  {
				  //------------------------------------------------------- Burner  ---------------------//
				  
				  if (!(flag_yes_no_2&(1<<STOKER_WORK_MODE_DEF))) // if stoker_work_mode == 0 (cycle mode)
				  {
					  
					  
					  if(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF))// if stoker bunker on,
					  {
						  if (timer_template_stoker_dose<=(stoker_time_work/0.008))
						  {
							  BURNER_AUGER_ON;
							  timer_template_stoker_dose++;
						  }
						  else
						  {
							  timer_template_stoker_dose=0;
							  flag_yes_no_2&=~(1<<STOKER_WORK_DELAY_DEF);
							  
						  }
					  }
					  
					  else
					  {
						  if (timer_template_stoker_dose<=(stoker_time_delay/0.008))
						  {
							  BURNER_AUGER_OFF;
							  timer_template_stoker_dose++;
						  }
						  else
						  {
							  timer_template_stoker_dose=0;
							  flag_yes_no_2|=(1<<STOKER_WORK_DELAY_DEF);
							  
						  }
					  }
					  
					  
					  
				  }
				  else // stoker work mode==AUTO;  1
				  {
					  if(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF))
					  {
						  
						  if (timer_template_dose>=stoker_time_continuation/0.008)
						  {
							  flag_yes_no_2&=~(1<<STOKER_WORK_DELAY_DEF);
							  BURNER_AUGER_OFF;
							  
							  if (stoker_time_continuation>=start_dose)
							  {
								  flag_yes_no_2&=~(1<<FLAG_START_DOSE);// END START DOSE
								  if (!flag_fans_work)
								  {
									  flag_fans_work=1;
								  }
								  
								  if (!(flag_yes_no&(ELECTROTEN_WORK_DEF)))
								  {
									  flag_yes_no|=(1<<ELECTROTEN_WORK_DEF); // we can turn on electroten
								  }
								  
							  }
						  }
						  
						  else
						  {
							  if (IS_BURNER_AUGER_OFF)
							  {
								  BURNER_AUGER_ON;
							  }
						  }
						  
						  
					  }
				  }

				  //--------------------------------------------------------
				  
				  //----------------------------------------------------------BUNKER ------------------------------------------------------------//
				  
				  if(timer_template_dose<=((dose_min_power*supply_cycle)/100)/0.008)
				  {
					  if(!(flag_yes_no_2&(1<<STOKER_WORK_DELAY_DEF)) &&flag_yes_no_2&(1<<STOKER_WORK_MODE_DEF))// if Pellets stoker off and auto mode
					  {
						  
						  flag_yes_no_2|=(1<<STOKER_WORK_DELAY_DEF);
						  
					  }
					  
					  BUNKER_AUGER_ON;
					  //BURNER_AUGER_ON;
					  
					  
					  timer_template_dose++;
					  eUpdate=UPDATE;
				  }
				  
				  else if (timer_template_dose<(supply_cycle/0.008)&&timer_template_dose>( ((dose_min_power*supply_cycle)/100)/0.008) )
				  {
					  //BURNER_AUGER_OFF;
					  BUNKER_AUGER_OFF; //  shnek on
					  timer_template_dose++;
					  if (!(flag_yes_no&(ELECTROTEN_WORK_DEF)))
					  {
						  flag_yes_no|=(1<<ELECTROTEN_WORK_DEF); // we can turn on electroten
					  }
					  if (!flag_fans_work)
					  {
						  flag_fans_work=1;
					  }
					  eUpdate=UPDATE;
				  }
				  
				  else if (timer_template_dose>=supply_cycle/0.008)
				  {
					  timer_template_dose=0;
					  
				  }
				  
			  }
			  
		 // }// end if selected pelets mode
		  
		  
		  else //if selected woods fuel, all shnek off
		  {
			  BUNKER_AUGER_OFF;
			  BURNER_AUGER_OFF;
			  flag_fans_work=1;
		  }
		  
		  
		  
		  
		  
		  //-------------------------------------------------------------Control TEMPERATURE-------------------//
		  
		  
		  if(flag_yes_no&(1<<SESON_DEF)) //  SUMMER
		  {
			  if (boiler_temperature<=(m_hot_water-kotel_hyste_swich_burner) || kotel_temperature<=kotel_min_temp)
			  {
				  timer_template_var=0;
				  eWork_Mode=W_MAX_POWER;
				  eUpdate=UPDATE;
				  break;
			  }
		  }
		  
		  else//  WINTER
		  {
			  if (burner_temperature>=m_burner_temp)
			  {
				  if (IS_ELECTROTEN_ON)
				  {
					  ELECTROTEN_OFF;
				  }
			  }
			  
			  /*
			  if (kotel_temperature>=(kotel_max_temp+kotel_top_hystes) )
			  {
				  flag_work_mode_template=W_MIN_POWER;
				  timer_template_var=0;
				  eWork_Mode=W_AFTERBURNING;
				  eUpdate=UPDATE;
				  break;
			  }*/
			  
			  
			  if (flag_yes_no&(1<<SELECT_CWU_DEF))//  CWU+CO
			  {
				  if (flag_yes_no&(1<<PUMP_HEATING_DEF))// AUTO mode
				  {
					  if ( home_temperature<=(setup_boiler_temperature-kotel_hyste_swich_burner) || kotel_temperature<=kotel_min_temp)
					  {
						  timer_template_var=0;
						  eWork_Mode=W_MAX_POWER; 
						  eUpdate=UPDATE;
						  break;
					  }
					  
				  }
				  else// THERMOSTAT mode
				  {
					  if (IS_TERMOSTAT_ON || kotel_temperature<=kotel_min_temp)
					  {
						  timer_template_var=0;
						  eWork_Mode=W_MAX_POWER;
						  eUpdate=UPDATE;
						  break;
					  }
					  
				  }
				  
				  if (boiler_temperature<=(m_hot_water-kotel_hyste_swich_burner) || kotel_temperature<=kotel_min_temp)
				  {
					  timer_template_var=0;
					  eWork_Mode=W_MAX_POWER;
					  eUpdate=UPDATE;
					  break;
				  }
				  
			  }
			  
			  else //CO
			  {
				  if (flag_yes_no&(1<<PUMP_HEATING_DEF))// AUTO mode
				  {
					  if ( home_temperature<=(setup_boiler_temperature-kotel_hyste_swich_burner) || kotel_temperature<=kotel_min_temp)
					  {
						  timer_template_var=0;
						  eWork_Mode=W_MAX_POWER;
						  eUpdate=UPDATE;
						  break;
					  }
					  
				  }
				  else// THERMOSTAT mode
				  {
					  if (IS_TERMOSTAT_ON || kotel_temperature<=kotel_min_temp)
					  {
						  timer_template_var=0;
						  eWork_Mode=W_MAX_POWER;
						  eUpdate=UPDATE;
						  break;
					  }
					  
				  }
			  }
			  
			  
			  
		  }
		  //------------------------------------------------------------------------------------//
			   
			 
			 
			   
			   break;
		   }
		   
		   
		   
		   
		   case W_BLOWING://7
		   {
			   if (IS_ELECTROTEN_ON)
			   {
				   ELECTROTEN_OFF;
			   }
			   
			   //-----------------------------------------------------------//
			   
			   if (flag_fan1_fan2_blowing==0)// FAN1
			   {
				   if (blowing_timer>=time_blowing/0.008)
				   {
					   blowing_timer=0;
					   eWork_Mode=W_MIN_POWER;
					   
					   break;
				   }
				   else
				   {
					   blowing_timer++;
					   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
					   {
						   EICRB|=((1<<ISC61)|(1<<ISC60));
						   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
					   }
					   
					   
					   
				   }
				   
				   fan1_spead_var=rotation_blowing;
			   }
			   
			   else if(flag_fan1_fan2_blowing==1)// FAN2
			   {
				   if (blowing_timer>=fan2_time_blowing/0.008)
				   {
					   blowing_timer=0;
					   fan2_blowing_timer=0;
					   eWork_Mode=W_MIN_POWER;
					   
					   break;
				   }
				   else
				   {
					   blowing_timer++;
					   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
					   {
						   EICRB|=((1<<ISC61)|(1<<ISC60));
						   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
					   }
				   }
				   
				   
				   fan2_spead_var=fan2_rotation_blowing;
			   }
			   else// FAN1+FAN2
			   {
					   if (blowing_timer>=time_blowing/0.008)
					   {
						   blowing_timer=0;
						   fan2_blowing_timer=0;
						   eWork_Mode=W_MIN_POWER;
						   
						   break;
					   }
					   else
					   {
						   blowing_timer++;
						   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						   {
							   EICRB|=((1<<ISC61)|(1<<ISC60));
							   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						   }
						   
					   }
				  
			   
				   
				   fan1_spead_var=rotation_blowing;
				   fan2_spead_var=fan2_rotation_blowing;
			 }
			   
			  
			   break;
		   }
		   
		   
		   
		   
		   case W_AFTERBURNING://8
		   {
			   if (flag_alarm&(1<<BURNER_SENSOR_ALARM))
			   {
				   eWork_Mode=W_ATTENUATION;
				   timer_template_var=0;
			   }
			  
			  flag_fans_work=1;
			 
			   //---------------------------------------------------- FAN1 and FAN2 ---------------------------------//
			   if (flag_fans_work)// FANs work start
			   {
				   
				   if (flag_yes_no&(1<<FAN2_YES_NO_DEF))// if selected FAN2
				   {
					   if (!(flag_yes_no&(1<<FUEL_DEF)))// if  pellets mode
					   {
						   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						   {
							   EICRB|=((1<<ISC61)|(1<<ISC60));
							   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						   }
						   
						   if (flag_yes_no&(1<<PELETS_FAN_DEF))// if selected FAN1+FAn2
						   {
							   fan1_spead_var=rotation_quenching;
							   fan2_spead_var=fan2_rotation_quenching;
						   }
						   else
						   {
							   fan1_spead_var=rotation_quenching;
						   }
						   
					   }
					   else//  WOOD mode
					   {
						   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						   {
							   EICRB|=((1<<ISC61)|(1<<ISC60));
							   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						   }
						   
						   if (lighter_woods_fan==0)//   FAN1
						   {
							   fan1_spead_var=rotation_quenching;
						   }
						   else if (lighter_woods_fan==1)//   FAN2
						   {
							   fan2_spead_var=fan2_rotation_quenching;
						   }
						   else//  FAN1+ FAN2
						   {
							   fan1_spead_var=rotation_quenching;
							   fan2_spead_var=fan2_rotation_quenching;
						   }
						   
					   }
					   
					   
				   }
				   else//
				   {
					   fan1_spead_var=rotation_quenching;
					   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
					   {
						   EICRB|=((1<<ISC61)|(1<<ISC60));
						   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
					   }
					   
				   }
				   
			   }
			   //-------------------------------------------------------------------------------------------------------------------//
			 
			 
				   if (IS_BURNER_AUGER_ON||IS_BUNKER_AUGER_ON||IS_ELECTROTEN_ON)
				   {
					   BUNKER_AUGER_OFF;
					   BURNER_AUGER_OFF;
					   ELECTROTEN_OFF;
				   }
				   
				   if (timer_template_var>=(lighter_flame_delay/0.008))// if don't find flame, go to attenuation 
				   {
					   eWork_Mode=W_ATTENUATION;
					   eUpdate=UPDATE;
					   timer_template_var=0;
					   flag_yes_no&=~(1<<LIGHTER_FLAME_DELAY_DEF);
				   }
				   else
				      timer_template_var++;
			  // }
			   break;
		   }
		   
		   
		   
		   case W_ATTENUATION://9
		   {
			   
			   if (IS_ELECTROTEN_ON||IS_BUNKER_AUGER_ON||IS_BURNER_AUGER_ON)
			   {
				   ELECTROTEN_OFF;
				   BUNKER_AUGER_OFF;
				   BURNER_AUGER_OFF;
			   }
			   
			  //------------------------------------------- Quenching time --------------------------------------// 
			   if (timer_template_var>=((lighter_quenching_time*60)/0.008))
			   {
				   eWork_Mode=W_CLEANING;
				   eUpdate=UPDATE;
				   timer_template_var=0;
			   }
			   else
			      timer_template_var++;
				  
				
				
				if (!flag_fans_work)
				{
					flag_fans_work=1;
				}
				
				
				  
			  //---------------------------------------------------- FAN1 and FAN2 ---------------------------------//
			  if (flag_fans_work)// FANs work start
			  {
				  
				  if (flag_yes_no&(1<<FAN2_YES_NO_DEF))// if selected FAN2
				  {
					  if (!(flag_yes_no&(1<<FUEL_DEF)))// if  pellets mode
					  {
						  if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						  {
							  EICRB|=((1<<ISC61)|(1<<ISC60));
							  EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						  }
						  
						  if (flag_yes_no&(1<<PELETS_FAN_DEF))// if selected FAN1+FAn2
						  {
							  fan1_spead_var=rotation_quenching;
							  fan2_spead_var=fan2_rotation_quenching;
						  }
						  else
						  {
							  fan1_spead_var=rotation_quenching;
						  }
						  
					  }
					  else//  WOOD mode
					  {
						  if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						  {
							  EICRB|=((1<<ISC61)|(1<<ISC60));
							  EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						  }
						  
						  if (lighter_woods_fan==0)//   FAN1
						  {
							  fan1_spead_var=rotation_quenching;
						  }
						  else if (lighter_woods_fan==1)//   FAN2
						  {
							  fan2_spead_var=fan2_rotation_quenching;
						  }
						  else//  FAN1+ FAN2
						  {
							  fan1_spead_var=rotation_quenching;
							  fan2_spead_var=fan2_rotation_quenching;
						  }
						  
					  }
					  
					  
				  }
				  else//
				  {
					  fan1_spead_var=rotation_quenching;
					  if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
					  {
						  EICRB|=((1<<ISC61)|(1<<ISC60));
						  EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
					  }
					  
				  }
				  
			  }
			  //-------------------------------------------------------------------------------------------------------------------//	  
			
			
				  
			  
			   break;
		   }
		   case W_CLEANING://10
		   {
			   if (!flag_fans_work)
			   {
				   flag_fans_work=1;
			   }
			   
			   
			   if (IS_BUNKER_AUGER_ON||IS_ELECTROTEN_ON||IS_BURNER_AUGER_ON)//  Off ten and bunker
			   {
				   BUNKER_AUGER_OFF;
				   ELECTROTEN_OFF;
				   BURNER_AUGER_OFF;
			   }
			   
			   
			   if (time_sec>=40)
			   {
				   if(cleaning_x_position>=52)
				      cleaning_x_position=47;
				   else if(cleaning_x_position<=47)
				      cleaning_x_position=52;	
				   time_sec=0;
			   }
			   else
			   time_sec++;
			   
			   
			   if (timer_template_var>=(mechanism_cleaning_work/0.008))
			   {
				   //cleaning_delay_timer=0;
				   
				   //---------------------------------------------------------------//
				   if (flag_cleaning_mode==CLEANING_TIME)
				   {
					   eSTART_STOP=START;
					   
					   flag_cleaning_mode=CLEANING_TIME;
				   }
				   else  
				     {
						 eSTART_STOP=STOP;
						 
					 }
				   
				   
				   
				   //----------------------------------------------------------------//
				   
				   timer_template_stoker_dose=0;
				   flag_yes_no_2|=(1<<STOKER_WORK_DELAY_DEF);
				   
				   eWork_Mode=W_MONITORING;
				   eUpdate=UPDATE;
				   timer_template_var=0;
				   
				   if (flag_fans_work)
				   {
					   flag_fans_work=0;
					   EICRB&=~((1<<ISC61)|(1<<ISC60));
					   EIMSK&=~(1<<INT6); //Interrupt  INT6 ENABLE
					   
					   
				   }
			   }
			   
			   else
			       timer_template_var++;
			   
			   
			   cleaning_delay_timer=0;
			   
			   //---------------------------------------------------- FAN1 and FAN2 ---------------------------------//
			   if (flag_fans_work)// FANs work start
			   {
				   
				   if (flag_yes_no&(1<<FAN2_YES_NO_DEF))// if selected FAN2
				   {
					   if (!(flag_yes_no&(1<<FUEL_DEF)))// if  pellets mode
					   {
						   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						   {
							   EICRB|=((1<<ISC61)|(1<<ISC60));
							   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						   }
						   
						   if (flag_yes_no&(1<<PELETS_FAN_DEF))// if selected FAN1+FAn2
						   {
							   fan1_spead_var=rotation_cleaning;
							   fan2_spead_var=fan2_rotation_cleaning;
						   }
						   else
						   {
							   fan1_spead_var=rotation_cleaning;
						   }
						   
					   }
					   else//  WOOD mode
					   {
						   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
						   {
							   EICRB|=((1<<ISC61)|(1<<ISC60));
							   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
						   }
						   
						   if (lighter_woods_fan==0)//   FAN1
						   {
							   fan1_spead_var=rotation_cleaning;
						   }
						   else if (lighter_woods_fan==1)//   FAN2
						   {
							   fan2_spead_var=fan2_rotation_cleaning;
						   }
						   else//  FAN1+ FAN2
						   {
							   fan1_spead_var=rotation_cleaning;
							   fan2_spead_var=fan2_rotation_cleaning;
						   }
						   
					   }
					   
					   
				   }
				   else//
				   {
					   fan1_spead_var=rotation_cleaning;
					   if(!(EIMSK&(1<<INT6)))  //if Interrupt INT6 not enable
					   {
						   EICRB|=((1<<ISC61)|(1<<ISC60));
						   EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
					   }
					   
				   }
				   
			   }
			   //-------------------------------------------------------------------------------------------------------------------//
			   
			   
			   
			   
			   break;
		   }
		   
		   
		   case W_FILLING://11
		   {
			   
			   break;
		   }
		   
		   
		   case W_EXTINGUISHING://12
		   {
			   
			   
			   
			   if (IS_BUNKER_AUGER_ON||IS_ELECTROTEN_ON||flag_fans_work)
			   {
				  FAN1_OFF;
				  FAN2_OFF;
				  BUNKER_AUGER_OFF;
				  ELECTROTEN_OFF; 
			   }
			   
			   if (timer_template_var>=(stoker_time_emptying/0.008))// 
			   {
				   BURNER_AUGER_OFF;
				   timer_template_var=0;
				   eWork_Mode=W_CLEANING;
			   }
			   else
			   {
				   BURNER_AUGER_ON;
				   timer_template_var++;
			   }
			   
			   break;
		   }
		   
		}
		
		break;  
	  }
    }
  //}
  TCCR1B|=(1<<CS12);//|(1<<CS30); // 16 Mhz  / 256 prescal
  TCNT1H=0;
  TCNT1L=0;
  TIMSK1|=(1<<OCIE1A);
  OCR1AH=0x01;
  OCR1AL=0xF4; //1 milisec
  
  SREG = sreg;
  sei();
  
}

ISR(ADC_vect)
{
	unsigned char sreg;
	/* Save global interrupt flag */
	sreg = SREG;
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
			burner_temperature=Convert_PT1000_ADC_To_Celsium(ADC);
			if(flag_yes_no&(1<<SELECT_CWU_DEF))
			{
			   eChanel=BOILER;
			}
			else
			   {
				   eChanel=SHNEK;
				   boiler_temperature=0;
				   flag_alarm&=~(1<<BOILER_SENSOR_ALARM);
			   }
			permission_read=TRUE;
			if(template_temperature_all!=burner_temperature)
			   eUpdate=UPDATE;
			if(burner_temperature==0xFFFF)
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
			  if (flag_yes_no&(1<<PUMP_HEATING_DEF))
			  {
				 eChanel=CO_TERMOSTAT; 
			  }
			  else
			     eChanel=KOTEL;
				 
			  permission_read=TRUE;
			  if(template_temperature_all!=shnek_temperature)
			     eUpdate=UPDATE;
			  if(shnek_temperature==0xFF)
			  {
				  //shnek_temperature=-1;
				  flag_alarm|=(1<<SHNEK_SENSOR_ALARM);
			  }
			  else
			     flag_alarm&=~(1<<SHNEK_SENSOR_ALARM);
			  break;
		  }
		  
		  case CO_TERMOSTAT:
		  {
			  template_temperature_all=home_temperature;
			  home_temperature=Convert_ADC_To_Celsium(ADC);
			  eChanel=KOTEL;
			  permission_read=TRUE;
			  if(template_temperature_all!=home_temperature)
			  eUpdate=UPDATE;
			  if(home_temperature==0xFF)
			  {
				  //shnek_temperature=-1;
				  flag_alarm|=(1<<HOME_SENSOR_ALARM);
			  }
			  else
			  flag_alarm&=~(1<<HOME_SENSOR_ALARM);
			  break;
		  }
		  
            
	  }
		  	
		  
	//}
	 
	SREG = sreg;
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
	
	//fan_glob_val=35;
	count_glob_val=1;
	eUpdate=UPDATE;
	eWindow=MAIN;
	eWork_Mode=W_MONITORING;
	//eSTART_STOP=STOP;
	//eFuel_Type=PELLETS;
	//f_clear_lcd=1;
	f_timer_0=0;
	flag_fans_work=0;
	
	cleaning_x_position=52;
	
	time_sec=0;
	flame_y_position=43;
	
	f_curent_pos=0;
	kotel_temperature=0;
	burner_temperature=0;
	boiler_temperature=0;
	shnek_temperature=0;
	
	timer_template_var=0;
	timer_template_dose=0;
	timer_template_stoker_dose=0;
	blowing_timer=0;
	cleaning_delay_timer=0;
	
	flag_alarm=0;
	permission_read=TRUE;
	eDirection=NOT_MOVE;
	
	//-
	// Must deler !!!!!!!
	time_count=0;
	//
	Encoder_Init();
	Init_ADC();
	Init_ALL_Variables();
	Init_All_Port();
	
	f_ecoder_button=0;
	Init_Intterupt_6();
	TIMER2_Init();
	TIMER16_3_Init();
	TIMER16_1_Init();
	TIMER16_4_Init();  
	////-----------------------------------------------------------------//
	PORTD&=~((1<<PD0));
	  
	u8g2_Setup_st7567_enh_dg128064i_f( &u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_avr_gpio_and_delay );
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	
	// full buffer example, setup procedure ends in _f
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_t0_11_tr);
	
	//u8g2_SetFont(&u8g2, u8g2_font_t0_12b_tf);
	u8g2_SetContrast(&u8g2, 170);
	sei();
//-------------------------------------------------------------------- WHILE -------------------------------------------------------------//
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
			 u8g2_SelectionList(&u8g2,"Bunker",f_curent_pos+1, "3.1 S Dose\n3.2 S Cycle\n3.3 Dose max power\n3.4 Dose min power\n3.5 S Time work\n3.6 S delay\n3.7 S Continuation\n3.8 S Emptying\n3.9 S Fervor\n3.10 S Mode work");
		 }
		 //------------------------------------------end
		 //-------------------------------------------------------Lighter 4.0 ---------------------------------------------------------------//
		 else if (eWindow==LIGHTER_SETUP)
		 {
			 u8g2_ClearBuffer(&u8g2);
			 u8g2_SelectionList(&u8g2,"Lighter",f_curent_pos+1, "4.1 Hysteresis\n4.2 Flame delay\n4.3 Kindling time\n4.4 Attempt kind. \n4.5 Fuel shortage\n4.6 Ignition stab\n4.7 Time ign. stab\n4.8 Pilet fan\n4.9 Wood fan\n4.10 Quenching time");
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
			 eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
		 }
		 
		 else if (eWindow==BOIL_TEMPERATURE)
		 {
		    //Boiler_Temperature();
			Namber_Setup("Boiler Temperatur",&setup_boiler_temperature);
			eeprom_write_byte(&eemem_setup_boiler_temperature, setup_boiler_temperature);

		 } 
		 else if (eWindow==BOILER_POWER)
		 {
			 // Boiler_Power_Percent();
			 Namber_Setup("Boiler Power",&boil_power_percent);
			 eeprom_write_byte(&eemem_boil_power_percent, boil_power_percent);
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
			  
			  eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
		 }
		 else if (eWindow==M_HOT_WATER)
		 {
			 // Boiler_Power_Percent();
			 Namber_Setup("Hot water(`C)",&m_hot_water);
			 
			 eeprom_write_byte(&eemem_m_hot_water, m_hot_water);
		 }
		 else if (eWindow==M_BURNER_TEMP)
		 {
			  //Boiler_Power_Percent();
			 Namber_16bit_Setup("Burner temp.(`C)",&m_burner_temp);
			 
			 eeprom_write_word(&eemem_m_burner_temp, m_burner_temp);
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
			 
			 eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
		 }
		 else if (eWindow==M_DEFAULT)
		 {
			 if (flag_yes_no&(1<<DEFINE_DEF)) //
			 {
				 f_curent_pos=1;
			 }
			 else
			 f_curent_pos=0;
			 
			 u8g2_SelectionList(&u8g2,"Default",f_curent_pos+1, "NO\nYES");
			 f_curent_pos=0;
			 
			 eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
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
			
			eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
		 }
		 
		   else if(eWindow==HEATING)// Ignition // 1
		   {
			 Namber_Setup("Max rotat Ignition ",&max_rotation_ignition);
			 
			 eeprom_write_byte(&eemem_max_rotation_ignition, max_rotation_ignition);
		   }
		    else if(eWindow==HEATING_FAN_ROTATION)// 2
		    {
			    Namber_Setup("Rotation ign.",&rotation_ignition);
				
				eeprom_write_byte(&eemem_rotation_ignition, rotation_ignition);
		    }
		    else if(eWindow==MAX_POWER)// 3
		    {
			  Namber_Setup("Max rotat Power",&rotation_max_power);
			  
			  eeprom_write_byte(&eemem_rotation_max_power, rotation_max_power);
		    }
		     else if(eWindow==MIN_POWER)//  4
		     {
			   Namber_Setup("Min rotation Power",&rotation_min_power);
			   
			   eeprom_write_byte(&eemem_rotation_min_power, rotation_min_power);
		     }
		      else if(eWindow==QUENCHING)//  5
		      {
			    Namber_Setup("Quenching rotation",&rotation_quenching);
				
				eeprom_write_byte(&eemem_rotation_quenching, rotation_quenching);
		      }
			  else if(eWindow==CLEANING)//  6
			  {
				Namber_Setup("Cleaning rotation",&rotation_cleaning);
				
				eeprom_write_byte(&eemem_rotation_cleaning, rotation_cleaning);
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
				 
				 eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
			  } 
			 else if(eWindow==BLOWING_TIME)// 8
			 {
				 Namber_Setup("Blowing break",&time_blowing);
				 
				 eeprom_write_byte(&eemem_time_blowing, time_blowing);
			 }
			 else if(eWindow==BLOWING_BREAK_TIME)// 9
			 {
				 Namber_Setup("Blowing break",&delay_time);
				 
				 eeprom_write_byte(&eemem_delay_time, delay_time);
			 }
			 else if(eWindow==BLOWING_ROTATION)// 10
			 {
				 Namber_Setup("Blowing rotation",&rotation_blowing);
				 
				 eeprom_write_byte(&eemem_rotation_blowing, rotation_blowing);
			 }
			 //----------------------------------------------------------Bunker setup 3.0 ------------------------------------//
		
			 else if(eWindow==START_DOSE)// Ignition  0
			 {
				 Namber_Setup("Start dose(sec)",&start_dose);
				 
				 eeprom_write_byte(&eemem_start_dose, start_dose);
			 }
			 else if(eWindow==SUPPLY_CYCLE)//  1
			 {
				 Namber_Setup("Supply cycle(sec)",&supply_cycle);
				 
				 eeprom_write_byte(&eemem_supply_cycle, supply_cycle);
			 }
			 else if(eWindow==DOSE_MAX_POWER)// 2
			 {
				 Namber_Setup("Dose max pow(%)",&dose_max_power);
				 
				 eeprom_write_byte(&eemem_dose_max_power, dose_max_power);
			 }
			 else if(eWindow==DOSE_MIN_POWER)// 3
			 {
				 Namber_Setup("Dose min pow(%)",&dose_min_power);
				 
				 eeprom_write_byte(&eemem_dose_min_power, dose_min_power);
			 }
			 else if(eWindow==STOKER_TIME_WORK)// 4
			 {
				 Namber_Setup("Stok time work(sec)",&stoker_time_work);
				 
				 eeprom_write_byte(&eemem_stoker_time_work, stoker_time_work);
			 }
			 else if (eWindow==STOKER_TIME_DELAY)// 5
			 {
				 Namber_Setup("Stok work(sec)",&stoker_time_delay);
				 
				 eeprom_write_byte(&eemem_stoker_time_delay, stoker_time_delay);
			 }
			 else if(eWindow==STOKER_TIME_CONTINUATION)// 6
			 {
				 Namber_Setup("Stok continuation(sec)",&stoker_time_continuation);
				 
				 eeprom_write_byte(&eemem_stoker_time_continuation, stoker_time_continuation);
			 }
			  else if(eWindow==STOKER_TIME_EMPTYING)// 7
			  {
				  Namber_Setup("Stok continuation(sec)",&stoker_time_emptying);
				  
				  eeprom_write_byte(&eemem_stoker_time_emptying, stoker_time_emptying);
			  }
			 else if(eWindow==STOKER_FERVOR)// 8
			 {
				 Namber_Setup("Stoker ferver(`C)",&stoker_fervor);
				 
				 eeprom_write_byte(&eemem_stoker_fervor, stoker_fervor);
			 }
			 else if (eWindow==STOKER_MODE_WORK)// 5
			 {
				 if (flag_yes_no_2&(1<<STOKER_WORK_MODE_DEF)) //
				 {
					 f_curent_pos=1;
				 }
				 else
				 f_curent_pos=0;
				 
				 u8g2_SelectionList(&u8g2,"Stok. work mode",f_curent_pos+1, "CYCLE\nAUTO");
				 f_curent_pos=0;
				 
				 eeprom_write_word(&eemem_flag_yes_no,flag_yes_no_2);
			 }	 
			 //----------------------------------------------------------Lighter setup 4.0 ------------------------------------//
			 
			 else if(eWindow==HYSTERESIS)//   0
			 {
				 Namber_Setup("Hysteresis ('C)",&lighter_hysteresis);
				 
				 eeprom_write_byte(&eemem_lighter_hysteresis, lighter_hysteresis);
			 }
			 else if(eWindow==FLAME_DELAY)//  1
			 {
				 Namber_Setup("Flame delay (sec)",&lighter_flame_delay);
				 
				 eeprom_write_byte(&eemem_lighter_flame_delay, lighter_flame_delay);
			 }
			 else if(eWindow==KINDLING_TIME)// 2
			 {
				 Namber_Setup("Kindling time (min)",&lighter_kindling_time);
				 
				 eeprom_write_byte(&eemem_lighter_kindling_time, lighter_kindling_time);
			 }
			 else if(eWindow==ATTEMPT_KINDLING)// 3
			 {
				 Namber_Setup("Attem. kindl.",&lighter_attempt_kindling);
				 
				 eeprom_write_byte(&eemem_lighter_attempt_kindling, lighter_attempt_kindling);
			 }
			 else if(eWindow==FUEL_SHORTAGE)// 4
			 {
				 Namber_Setup("Fuel shortage (min)",&lighter_fuel_shortage);
				 
				 eeprom_write_byte(&eemem_lighter_fuel_shortage, lighter_fuel_shortage);
			 }
			 else if (eWindow==IGNITION_STAB)// 5
			 {
				 if (flag_yes_no&(1<<IGNITION_STAB_DEF)) //
				 {
					 f_curent_pos=1;
				 }
				 else
				 f_curent_pos=0;
				 
				 u8g2_SelectionList(&u8g2,"Ignit. stab (min)",f_curent_pos+1, "YES\nNO");
				 f_curent_pos=0;
				 
				 eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
			 }
			 else if(eWindow==TIME_IGNITION_STAB)// 6
			 {
				 Namber_Setup("Time ign. stab",&lighter_time_ignition_stab);
				 
				 eeprom_write_byte(&eemem_lighter_time_ignition_stab, lighter_time_ignition_stab);
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
				 
				 eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
			 }
			 else if(eWindow==WOODS_FAN)// 8
			 {
				 f_curent_pos=lighter_woods_fan;
				 u8g2_SelectionList(&u8g2,"Wood fan",f_curent_pos+1, "FAN1\nFAN2\nFAN1+FAN2");
				 //lighter_woods_fan=f_curent_pos;
				 f_curent_pos=0;
				 
				 eeprom_write_byte(&eemem_lighter_woods_fan, lighter_woods_fan);
			 }
			 else if(eWindow==QUENCHING_TIME)// 9
			 {
				 Namber_Setup("Quenching time",&lighter_quenching_time);
				 
				 eeprom_write_byte(&eemem_lighter_quenching_time, lighter_quenching_time);
			 }
			 //--------------------------------------------------------- Clean mechanism 5.0 -------------------------------//
			 else if(eWindow==MECHA_CLEAN_WORK)//   0
			 {
				 Namber_Setup("Clean time (sec)",&mechanism_cleaning_work);
				 
				 eeprom_write_byte(&eemem_mechanism_cleaning_work, mechanism_cleaning_work);
			 }
			 else if(eWindow==MECHA_CLEAN_DELAY)//  1
			 {
				 Namber_Setup("Delay time (min`)",&mechanism_cleaning_delay);
				 
				 eeprom_write_byte(&eemem_mechanism_cleaning_delay, mechanism_cleaning_delay);
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
				  
				  eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
			 }
			//--------------------------------------------------------- Pump hot water 7.0 -------------------------------//
			 else if(eWindow==PUMP_HOT_WATER_HYSTESIS)//   0
			 {
				 Namber_Setup("Hysterises(`C)",&pump_hot_water_hystesis);
				 
				 eeprom_write_byte(&eemem_pump_hot_water_hystesis, pump_hot_water_hystesis);
			 }
			 else if(eWindow==PUMP_HOT_WATER_INCREASE)//   1
			 {
				 Namber_Setup("Increase(`C)",&pump_hot_water_increase);
				 
				 eeprom_write_byte(&eemem_pump_hot_water_increase, pump_hot_water_increase);
			 }
			 else if(eWindow==PUMP_HOT_WATER_OVERSPENDING)//   2
			 {
				 Namber_Setup("Hysterises(`C)",&pump_hot_water_overspending);
				 
				 eeprom_write_byte(&eemem_pump_hot_water_overspending, pump_hot_water_overspending);
			 }
			 else if(eWindow==PUMP_HOT_WATER_TEMP_WORK)//  3
			 {
				 Namber_Setup("Temp. work(`C)",&pump_hot_water_temp_work);
				 
				 eeprom_write_byte(&eemem_pump_hot_water_temp_work, pump_hot_water_temp_work);
			 }
			 else if(eWindow==PUMP_HOT_WATER_HYSTESIS_WORK)//   4
			 {
				 Namber_Setup("Hyst. work(`C)",&pump_hot_water_hystesis_work);
				 
				 eeprom_write_byte(&eemem_pump_hot_water_hystesis_work, pump_hot_water_hystesis_work);
			 }
		   //---------------------------------------------------------- Boiler 8.0 -------------------------------//
		     else if(eWindow==BOILER_MIN_TEMP)//   0
		     {
			     Namber_Setup("Min temp(`C)",&kotel_min_temp);
				 
				 eeprom_write_byte(&eemem_kotel_min_temp, kotel_min_temp);
		     }
			 else if(eWindow==BOILER_MAX_TEMP)//   1
			 {
				 Namber_Setup("Max temp(`C)",&kotel_max_temp);
				 
				 eeprom_write_byte(&eemem_kotel_max_temp, kotel_max_temp);
			 }
			 else if(eWindow==BOILER_TOP_HYSTES)//   2
			 {
				 Namber_Setup("Top hyster(`C)",&kotel_top_hystes);
				 
				 eeprom_write_byte(&eemem_kotel_top_hystes, kotel_top_hystes);
			 }
			 else if(eWindow==BOILER_HYSTES_SWICH_BURNER)//   3
			 {
				 Namber_Setup("Hyster. swich(`C)",&kotel_hyste_swich_burner);
				 
				 eeprom_write_byte(&eemem_kotel_hyste_swich_burner, kotel_hyste_swich_burner);
			 }
			 else if(eWindow==BOILER_HYSTES_PROTECT)//   4
			 {
				 Namber_Setup("Hyster. protect (`C)",&kotel_protect);
				 
				 eeprom_write_byte(&eemem_kotel_protect, kotel_protect);
			 }
			 else if(eWindow==BOILER_TEMP_OVERHEATING)//   5
			 {
				 Namber_Setup("Temp. overheat.(`C)",&kotel_temp_overheating);
				 
				 eeprom_write_byte(&eemem_kotel_temp_overheating, kotel_temp_overheating);
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
				 
				 eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
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
				
				eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
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
				 
				 eeprom_write_word(&eemem_flag_yes_no, flag_yes_no);
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
				 
				 eeprom_write_word(&eemem_flag_yes_no, flag_yes_no);
			 }
			 
			 else if(eWindow==FAN2_HEATING)// Ignition // 1
			 {
				 Namber_Setup("Max rotat Ignition(%) ",&fan2_max_rotation_ignition);
				 
				 eeprom_write_byte(&eemem_fan2_max_rotation_ignition, fan2_max_rotation_ignition);
			 }
			 else if(eWindow==FAN2_HEATING_FAN_ROTATION)// 2
			 {
				 Namber_Setup("Rotation ign.(%)",&fan2_rotation_ignition);
				 
				 eeprom_write_byte(&eemem_fan2_rotation_ignition, fan2_rotation_ignition);
			 }
			 else if(eWindow==FAN2_MAX_POWER)// 3
			 {
				 Namber_Setup("Max rotat Power",&fan2_rotation_max_power);
				 
				 eeprom_write_byte(&eemem_fan2_rotation_max_power, fan2_rotation_max_power);
			 }
			 else if(eWindow==FAN2_MIN_POWER)//  4
			 {
				 Namber_Setup("Min rotation Power",&fan2_rotation_min_power);
				 
				 eeprom_write_byte(&eemem_fan2_rotation_min_power, fan2_rotation_min_power);
			 }
			 else if(eWindow==FAN2_QUENCHING)//  5
			 {
				 Namber_Setup("Quenching rotation",&fan2_rotation_quenching);
				 
				 eeprom_write_byte(&eemem_fan2_rotation_quenching, fan2_rotation_quenching);
			 }
			 else if(eWindow==FAN2_CLEANING)//  6
			 {
				 Namber_Setup("Cleaning rotation",&fan2_rotation_cleaning);
				 
				 eeprom_write_byte(&eemem_fan2_rotation_cleaning, fan2_rotation_cleaning);
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
				 
				 eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
			 }
			 else if(eWindow==FAN2_BLOWING_TIME)// 8
			 {
				 Namber_Setup("Blowing break",&fan2_time_blowing);
				 
				 eeprom_write_byte(&eemem_fan2_time_blowing, fan2_time_blowing);
			 }
			 else if(eWindow==FAN2_BLOWING_BREAK_TIME)// 9
			 {
				 Namber_Setup("Blowing break",&fan2_delay_time);
				 
				 eeprom_write_byte(&eemem_fan2_delay_time, fan2_delay_time);
			 }
			 else if(eWindow==FAN2_BLOWING_ROTATION)// 10
			 {
				 Namber_Setup("Blowing rotation",&fan2_rotation_blowing);
				 
				 eeprom_write_byte(&eemem_fan2_rotation_blowing, fan2_rotation_blowing);
			 }
		     
	}
}
//----------------------------------------------------------------------------------------Main_ -----------------------------------//
void Main_(void)
{
  eUpdate=UPDATE;
  for(;;)
   {		
	
	eUpdate=UPDATE;
	
	// if(flag_alarm&&eWork_Mode!=W_AFTERBURNING&&eWork_Mode!=W_ATTENUATION&&eWork_Mode!=W_CLEANING)
	 //{
		//eWork_Mode=W_AFTERBURNING;
	 //}
	
   if (eUpdate==UPDATE)
	{  
	    u8g2_ClearBuffer(&u8g2);
		
			

		if (!(flag_yes_no&(1<<SESON_DEF)))// if seson == WINTER
		{
			
			
			if(flag_yes_no&(1<<SELECT_CWU_DEF))// if  selected CWU (hot water)
			{
				//----------------------------------------------- fuel type --------------------------//
				if (flag_yes_no&(1<<FUEL_DEF))// if fuel==wood
				{
					u8g2_DrawXBM(&u8g2, 42, 14, Wood_boiler_width, Wood_boiler_height, Wood_boiler_bits);
					u8g2_DrawStr(&u8g2,96,27,"wood");
					u8g2_DrawStr(&u8g2,96,37,"fuel");
				}
				else
				{
					u8g2_DrawXBM(&u8g2, 42, 14, boiler_bunker_width, boiler_bunker_height, boiler_bunker_bits);
				}
				//---------------------------------------------
				
				
				//u8g2_DrawXBM(&u8g2, 11 ,49, boiler_bunker_width, boiler_bunker_height, battery_bits);
				u8g2_DrawXBM(&u8g2, 3, 14, boiler_width, boiler_height, boiler_bits);
				
				if (flag_yes_no&(1<<FAN2_YES_NO_DEF))
				{
					u8g2_DrawXBM(&u8g2, 75, 17, FAN_2_width, FAN_2_height, FAN_2_bits);
				}
				if (eWork_Mode==W_IGNITION)
				{
					
					
					u8g2_DrawXBM(&u8g2, 51, flame_y_position, flame_width, flame_height,flame_bits);// y==43 , start position
					
				}
				
				WriteValueToLCD(boiler_temperature,7,34);
			}
			else
			{
				
				//----------------------------------------------- fuel type --------------------------//
				if (flag_yes_no&(1<<FUEL_DEF))// if fuel==wood
				{
					u8g2_DrawXBM(&u8g2, 42, 14, Wood_boiler_width, Wood_boiler_height, Wood_boiler_bits);
					
					u8g2_DrawStr(&u8g2,96,27,"wood");
					u8g2_DrawStr(&u8g2,96,37,"fuel");
				}
				else
				{
					u8g2_DrawXBM(&u8g2, 42, 14, boiler_bunker_width, boiler_bunker_height, boiler_bunker_bits);
				}
				//---------------------------------------------

				
				if (flag_yes_no&(1<<FAN2_YES_NO_DEF))
				{
					u8g2_DrawXBM(&u8g2, 75, 17, FAN_2_width, FAN_2_height, FAN_2_bits);
				}
				
				if (eWork_Mode==W_IGNITION||eWork_Mode==W_IGNITION_STABILIZ||eWork_Mode==W_MAX_POWER||eWork_Mode==W_MIN_POWER)
				{
					u8g2_DrawXBM(&u8g2, 51, flame_y_position, flame_width, flame_height,flame_bits);// y==43
				}
			}
			
			u8g2_DrawXBM(&u8g2, 11, 49, battery_width, battery_height, battery_bits);
			u8g2_DrawXBM(&u8g2, 32, 17, line_batery_width, line_batery_height, line_batery_bits);
			if (flag_yes_no&(1<<SELECT_CWU_DEF))
			{
				u8g2_DrawLine(&u8g2,31,17, 35,17);
			}
			
		}
		else// if season SUMER
		{
			
				//----------------------------------------------- fuel type --------------------------//
				if (flag_yes_no&(1<<FUEL_DEF))// if fuel==wood
				{
					u8g2_DrawXBM(&u8g2, 42, 14, Wood_boiler_width, Wood_boiler_height, Wood_boiler_bits);
					
					
				}
				else
				{
					u8g2_DrawXBM(&u8g2, 42, 14, boiler_bunker_width, boiler_bunker_height, boiler_bunker_bits);
				}
				//---------------------------------------------
				
				
				//u8g2_DrawXBM(&u8g2, 11 ,49, boiler_bunker_width, boiler_bunker_height, battery_bits);
				if(flag_yes_no&(1<<SELECT_CWU_DEF))
				{
					u8g2_DrawXBM(&u8g2, 3, 14, boiler_width, boiler_height, boiler_bits);
					WriteValueToLCD(boiler_temperature,7,34);
				}
				else// is not selected cwu, write text
				{
					u8g2_DrawStr(&u8g2,8,23,"NO");
					u8g2_DrawStr(&u8g2,2,33,"select");
					u8g2_DrawStr(&u8g2,7,43,"CWU!");
				}
				
				if (flag_yes_no&(1<<FAN2_YES_NO_DEF))
				{
					u8g2_DrawXBM(&u8g2, 75, 17, FAN_2_width, FAN_2_height, FAN_2_bits);
				}
				if (eWork_Mode==W_IGNITION)
				{
					
					
					u8g2_DrawXBM(&u8g2, 51, flame_y_position, flame_width, flame_height,flame_bits);// y==43 , start position
					
				}
				 
			
		}
	 	
		
	 if(flag_alarm&&eWork_Mode!=W_AFTERBURNING&&eWork_Mode!=W_ATTENUATION&&eWork_Mode!=W_CLEANING&&eWork_Mode!=W_EXTINGUISHING&&eSTART_STOP==STOP)
		 { 
			 
			 if (flag_alarm&(1<<KOTEL_SENSOR_ALARM)||flag_yes_no&(1<<SELECT_CWU_DEF)||flag_alarm&(1<<BURNER_SENSOR_ALARM)||flag_alarm&(1<<SHNEK_SENSOR_ALARM)||flag_alarm&(1<<FUEL_SHORTAGE_DEF)||flag_alarm&(1<<SHNEK_OWER_HEAT_ALARM)||flag_alarm&(1<<NO_SELECT_CWU_IN_SUMMER)||flag_alarm&(1<<OWER_HEATING_ALARM))
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
					 u8g2_DrawStr(&u8g2,var,8,"3, ");
					 var=var+11;
				 }
				 
				 if (flag_alarm&(1<<SHNEK_SENSOR_ALARM))
				 {
					 u8g2_DrawStr(&u8g2,var,8,"4, ");
					 var=var+11;
				 }
				 
				 if (flag_alarm&(1<<FUEL_SHORTAGE_DEF))
				 {
					 u8g2_DrawStr(&u8g2,var,8,"5 ");
					 var=var+11;
				 }
				 if (flag_alarm&(1<<SHNEK_OWER_HEAT_ALARM))
				 {
					 u8g2_DrawStr(&u8g2,var,8,"6 ");
					 var=var+11;
				 }
				 if (flag_alarm&(1<<NO_SELECT_CWU_IN_SUMMER))
				 {
					 u8g2_DrawStr(&u8g2,var,8,"7 ");
					 var=var+11;
				 }
				
					 
					 if (flag_alarm&(1<<OWER_HEATING_ALARM))
					 {
						 u8g2_DrawStr(&u8g2,var,8,"8 ");
						 var=var+11;
					 }
				
				 if (flag_alarm&(1<<HOME_SENSOR_ALARM))
				 {
					 u8g2_DrawStr(&u8g2,var,8,"9 ");
					 var=var+11;
				 }
				 
				 
			 }
			 else
			   {
			     Write16bitValueToLCD(flag_alarm,60,8);
				 flag_alarm=0;
			   }
			 
		 }	
		else
		{
			if (eWork_Mode==W_MONITORING)
			{
				u8g2_DrawStr(&u8g2,30,8,"Monitoring");
			}
			else if (eWork_Mode==W_IGNITION)
			{
				u8g2_DrawStr(&u8g2,30,8,"Ignition");
			}
			else if (eWork_Mode==W_IGNITION_STABILIZ)
			{
				u8g2_DrawStr(&u8g2,30,8,"Ignition stab.");
			}
			else if (eWork_Mode==W_MAX_POWER)
			{
				u8g2_DrawStr(&u8g2,30,8,"Max Power");
			}
			else if (eWork_Mode==W_MODULATED_POWER)
			{
				u8g2_DrawStr(&u8g2,30,8,"Modul. power");
			}
			else if (eWork_Mode==W_MIN_POWER)
			{
				u8g2_DrawStr(&u8g2,30,8,"Min Power");
			}
			else if (eWork_Mode==W_ATTENUATION)
			{
				u8g2_DrawStr(&u8g2,30,8,"Attenuation");
			}
			else if (eWork_Mode==W_AFTERBURNING)
			{
				u8g2_DrawStr(&u8g2,30,8,"After burning");
			}
			else if (eWork_Mode==W_CLEANING)
			{
				u8g2_DrawStr(&u8g2,30,8,"Cleaning");
				u8g2_DrawXBM(&u8g2, cleaning_x_position, 50, cleaning_width, cleaning_height, cleaning_bits);
			}
			else if (eWork_Mode==W_EXTINGUISHING)
			{
				u8g2_DrawStr(&u8g2,30,8,"Extinguishing");
			}
			else if (eWork_Mode==W_BLOWING)
			{
				u8g2_DrawStr(&u8g2,30,8,"Blowing");
			}
		}
			
			
			
			WriteValueToLCD(kotel_temperature,52,27);
			
			Write16bitValueToLCD(burner_temperature, 47,64);
			
			if (!(flag_yes_no&(1<<FUEL_DEF)))// pelets fuel 
			{
				WriteValueToLCD(shnek_temperature, 97,62);
			}
			
		//-------------------------------------------------------------------- CO -------------------------------------//	
		 if (!(flag_yes_no&(1<<SESON_DEF)))// if WINTER
		 {
			 if (IS_CO_ON)
			 {
				 u8g2_DrawBox(&u8g2,32, 19, 7, 6);
			 }
			 else
			 {
				 u8g2_DrawFrame(&u8g2,32, 19, 8, 7);
			 }
		 }
		 
		 //-------------------------------------------------------------------- CWU -------------------------------------//
		 if (flag_yes_no&(1<<SELECT_CWU_DEF))// if CWU sected
		 {
			 if (IS_CWU_ON)
			 {
				 u8g2_DrawBox(&u8g2,20, 14, 7, 6);
			 }
			 else
			 {
				 u8g2_DrawFrame(&u8g2,20, 14, 8, 7);
			 }
		 } 
		  
		if (!(flag_yes_no&(1<<FUEL_DEF)))// if PELETS fuel
		{
			//-------------------------------------------------------------------- BURNER --------------------------------//
			if (IS_BURNER_AUGER_ON)
			{
				u8g2_DrawBox(&u8g2,97, 43, 5, 7);
			}
			else
			{
				u8g2_DrawFrame(&u8g2,97, 43, 6, 8);
			}
			
			//-------------------------------------------------------------------- BUNKER -------------------------------//
			if (IS_BUNKER_AUGER_ON)
			{
				u8g2_DrawBox(&u8g2,101, 16, 6, 6);
			}
			else
			{
				u8g2_DrawFrame(&u8g2,101, 16, 7, 6);
			}
		}
		 
		//////////////
		 if (flag_fans_work)// FANs work start
		 {
			 
			 if (flag_yes_no&(1<<FAN2_YES_NO_DEF))// if selected FAN2
			 {
				 if (!(flag_yes_no&(1<<FUEL_DEF)))// if  pellets mode
				 {
					 
					 if (flag_yes_no&(1<<PELETS_FAN_DEF))// if selected FAN1+FAn2
					 {
						 //fan1_spead_var=rotation_ignition;
						 //fan2_spead_var=fan2_rotation_ignition;
						 
						 u8g2_DrawBox(&u8g2, 75, 57, 7, 6);// FAN1
						 u8g2_DrawBox(&u8g2, 76, 27, 7, 6);// FAN2
					 }
					 else
					 {
						 //fan1_spead_var=rotation_ignition;
						 
						 u8g2_DrawBox(&u8g2, 75, 57, 7, 6);// FAN1
					 }
					 
				 }
				 else//  WOOD mode
				 {
					 
					 if (lighter_woods_fan==0)//   FAN1
					 {
						 u8g2_DrawBox(&u8g2, 75, 57, 7, 6);// FAN1
					 }
					 else if (lighter_woods_fan==1)//   FAN2
					 {
						 u8g2_DrawBox(&u8g2, 76, 27, 7, 6);// FAN2
					 }
					 else//  FAN1+ FAN2
					 {
						 u8g2_DrawBox(&u8g2, 75, 57, 7, 6);// FAN1
						 u8g2_DrawBox(&u8g2, 76, 27, 7, 6);// FAN2
					 }
					 
				 }
				 
				 
			 }
			 else
			 {
				 u8g2_DrawBox(&u8g2, 75, 57, 7, 6);// FAN1
			 }
			 
		 }
		 else// FANs stop
		 {
			 if (flag_fans_work)// FANs work start
			 {
				 
				 if (flag_yes_no&(1<<FAN2_YES_NO_DEF))// if selected FAN2
				 {
					 if (!(flag_yes_no&(1<<FUEL_DEF)))// if  pellets mode
					 {
						 
						 if (flag_yes_no&(1<<PELETS_FAN_DEF))// if selected FAN1+FAn2
						 {
							 //fan1_spead_var=rotation_ignition;
							 //fan2_spead_var=fan2_rotation_ignition;
							 
							 u8g2_DrawFrame(&u8g2, 75, 57, 8, 6);//FAN1
							 u8g2_DrawFrame(&u8g2, 76, 27, 8, 6);// FAN2
						 }
						 else
						 {
							 //fan1_spead_var=rotation_ignition;
							 
							 u8g2_DrawFrame(&u8g2, 75, 57, 8, 6);//FAN1
						 }
						 
					 }
					 else//  WOOD mode
					 {
						 
						 if (lighter_woods_fan==0)//   FAN1
						 {
							 u8g2_DrawFrame(&u8g2, 75, 57, 8, 6);//FAN1
						 }
						 else if (lighter_woods_fan==1)//   FAN2
						 {
							 u8g2_DrawFrame(&u8g2, 76, 27, 8, 6);// FAN2
						 }
						 else//  FAN1+ FAN2
						 {
							 u8g2_DrawFrame(&u8g2, 75, 57, 8, 6);//FAN1
							 u8g2_DrawFrame(&u8g2, 76, 27, 8, 6);// FAN2
						 }
						 
					 }
					 
					 
				 }
				 else
				 {
					 u8g2_DrawFrame(&u8g2, 75, 57, 8, 6);//FAN1
				 }
				 
			 }
		 }
		
		 //---------------------------------------
		 
		 if (IS_ELECTROTEN_ON)
		 {
			 u8g2_DrawXBM(&u8g2, 69, 33, elektro_ten_width, elektro_ten_height, elektro_ten_bits);
		 }
		 
		//----------------------------------------------------------------------- START_STOP ----------------------------//  
		 if(eSTART_STOP==START)
		 {
			 //u8g2_ClearBuffer(&u8g2);
			 
			 u8g2_DrawStr(&u8g2,1,8,"WORK");
			 
		 }
		 else if (eSTART_STOP==STOP)
		 {
			
			 
			 u8g2_DrawStr(&u8g2,1,8,"STOP");
		 }
		 
		// WriteValueToLCD(home_temperature,110,8);
		 
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

void Namber_16bit_Setup(char* str, volatile uint16_t* n_template)
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
			Write16bitValueToLCD(*(n_template),55,40);
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
	
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADIE)|(1<<ADPS1)|(1<<ADPS0);  // clock/ 128, ADC Enable, ADC Start
	eChanel=KOTEL;
}
void Init_All_Port(void)
{
	//
	DDRA|=(1<<PA7);
	PORTA|=(1<<PA7);                                               // FAN_1
	
	DDRA|=(1<<PA6);
	PORTA|=(1<<PA6);                                               // FAN_2
	//
	DDRA|=(1<<PA4);
	PORTA|=(1<<PA4);                                               // SOUND
	
	DDRB|=(1<<PB6);
	PORTB|=(1<<PB6);                                               // LED_PWM
	
	DDRC|=(1<<PC3);
	PORTC|=(1<<PC3);                                               // Burner
	
	DDRC|=(1<<PC4);
	PORTC|=(1<<PC4);                                               // CO pump	                                             // T_x
	
	DDRC|=(1<<PC5);                                                // CWU pump
	PORTC|=(1<<PC5);
	
	
	DDRC|=(1<<PC0);                                                // Burner
	PORTC|=(1<<PC0);
	
	DDRC|=(1<<PC1);                                                //Bunker
	PORTC|=(1<<PC1);
	
	DDRF=0x00;
	//PORTF=0x1F;
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
			ADMUX&=~((1<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADMUX|=(1<<MUX2);
			ADCSRA|=(1<<ADSC);
			break;
		}
		case CO_TERMOSTAT:
		{
			ADMUX&=~((1<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0));
			ADMUX|=(1<<MUX0)|(1<<MUX1);
			ADCSRA|=(1<<ADSC);
			break;
		}
	}
}

void Init_ALL_Variables(void)
{
	//eWindow=eeprom_read_byte(&eememWindow);
	//eWork_Mode=eeprom_read_byte(&eememWork_Mode);
	eSTART_STOP=eeprom_read_byte(&eememSTART_STOP);
	//eFuel_Type=eeprom_read_byte(&eememFuel_Type);
	//eSeason=eeprom_read_byte(&eememSeson);
	//eFAN_Selection=eeprom_read_byte(&eememFAN_Selectiom);
	
	//eeprom_read_byte();
	flag_yes_no=eeprom_read_word(&eemem_flag_yes_no);
	flag_yes_no_2=eeprom_read_byte(&eeme_flag_yes_no_2);
	//flag_alarm=eeprom_read_word(&eemem_flag_alarm);
	flag_alarm=0;
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
	lighter_quenching_time=eeprom_read_byte(&eemem_lighter_quenching_time);//9
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
	kotel_min_temp=eeprom_read_byte(&eemem_kotel_min_temp);// 0
	kotel_max_temp=eeprom_read_byte(&eemem_kotel_max_temp);// 1
	kotel_top_hystes=eeprom_read_byte(&eemem_kotel_top_hystes);// 2
	kotel_hyste_swich_burner=eeprom_read_byte(&eemem_kotel_hyste_swich_burner);// 3
	kotel_protect=eeprom_read_byte(&eemem_kotel_protect);// 4
	kotel_temp_overheating=eeprom_read_byte(&eemem_kotel_temp_overheating);// 5
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
void Defaut_Set(void)
{
	eUpdate=UPDATE;
	
	eWindow=MAIN;
	//eeprom_write_byte(&eememWindow,eWindow);
	
	eWork_Mode=W_MONITORING;
	//eeprom_write_byte(&eememWork_Mode,eWork_Mode);
	
	eSTART_STOP=STOP;
	//eeprom_write_byte(&eememSTART_STOP,eSTART_STOP);
	
	//eFuel_Type=PELLETS;
	//eeprom_write_byte(&eememFuel_Type,eFuel_Type);
	
	//eSeason=WINTER;
	//eeprom_write_byte(&eememSeson, eSeason);
	
	//eFAN_Selection=FAN1;
	//eeprom_write_byte(&eememFAN_Selectiom,eFAN_Selection);
	
	
	//f_clear_lcd=1;
	f_timer_0=0;
	flag_fans_work=0;
	
	cleaning_x_position=52;
	
	time_sec=0;
	flame_y_position=43;
	
	f_curent_pos=0;
	kotel_temperature=0;
	burner_temperature=0;
	boiler_temperature=0;
	shnek_temperature=0;
	
	timer_template_var=0;
	timer_template_dose=0;
	timer_template_stoker_dose=0;
	
	flag_alarm=0;
	permission_read=TRUE;
	eDirection=NOT_MOVE;

	
	flag_yes_no=0;
	eeprom_write_word(&eemem_flag_yes_no,flag_yes_no);
	
	flag_yes_no_2=0;
	eeprom_write_byte(&eeme_flag_yes_no_2, flag_yes_no_2);
	
	//flag_alarm=0;
	//eeprom_read_word(&eemem_flag_alarm);
	
	//-----------------------------MAIN seting 1.x--//
	setup_boiler_temperature=50;
	eeprom_write_byte(&eemem_setup_boiler_temperature,setup_boiler_temperature);
	
	boil_power_percent=90;
	eeprom_write_byte(&eemem_boil_power_percent,boil_power_percent);
	
	m_hot_water=45;
	eeprom_write_byte(&eemem_m_hot_water,m_hot_water);
	
	m_burner_temp=200;
	eeprom_write_word(&eemem_m_burner_temp,m_burner_temp);
	//volatile uint8_t HWS;
	
	
	//-----------------------------FAN Seting  2.x--//
	max_rotation_ignition=30; //30%  spead rotation;  1->100%
	eeprom_write_byte(&eemem_max_rotation_ignition,max_rotation_ignition);// 1
	
	rotation_ignition=5; //5% spead rotation; 1->100%
	eeprom_write_byte(&eemem_rotation_ignition,rotation_ignition); //  2
	
	rotation_max_power=100; //100 % spead rotation; 1->100%
	eeprom_write_byte(&eemem_rotation_max_power,rotation_max_power);// 3
	
	rotation_min_power=45;// 1->100%
	eeprom_write_byte(&eemem_rotation_min_power,rotation_min_power);// 4
	
	rotation_quenching=50;// 50%; 1->100%
	eeprom_write_byte(&eemem_rotation_quenching,rotation_quenching);// 5
	
	rotation_cleaning=70;// 50%;  1->100%
	eeprom_write_byte(&eemem_rotation_cleaning,rotation_cleaning);// 6
	
	time_blowing=15;//  30 sec;  5->60 sec
	eeprom_write_byte(&eemem_time_blowing,time_blowing);// 8
	
	delay_time=1;// 20 min;  1->99 min
	eeprom_write_byte(&eemem_delay_time,delay_time);// 9
	
	rotation_blowing=100;// 50%;  1->100%
	eeprom_write_byte(&eemem_rotation_blowing,rotation_blowing);// 10
	//------end fan seting
	
	
	//----------------------------BUNKER Seting 3.x--//
	start_dose=15;// 20sec;  1->250 sec
	eeprom_write_byte(&eemem_start_dose,start_dose);
	
	supply_cycle=25;// 60 sec; 1->250 sec
	eeprom_write_byte(&eemem_supply_cycle,supply_cycle);
	
	dose_max_power=80; // 100%;  1->100%
	eeprom_write_byte(&eemem_dose_max_power,dose_max_power);
	
	dose_min_power=10;// 40%; 1->100%
	eeprom_write_byte(&eemem_dose_min_power,dose_min_power);
	
	stoker_time_work=10;// 1->99 sec
	eeprom_write_byte(&eemem_stoker_time_work,stoker_time_work);// 4
	
	stoker_time_delay=20;// 1->99 sec
	eeprom_write_byte(&eemem_stoker_time_delay,stoker_time_delay);
	
	stoker_time_continuation=15; //40 sec; 1-99sec. work in auto mode
	eeprom_write_byte(&eemem_stoker_time_continuation,stoker_time_continuation);
	
	stoker_time_emptying=40; //40 sev; 1-99 sec
	eeprom_write_byte(&eemem_stoker_time_emptying,stoker_time_emptying);
	
	stoker_fervor=60;// 60 celsium;  20->99 sec
	eeprom_write_byte(&eemem_stoker_fervor,stoker_fervor);
	//------end bunker seting
	
	
	//---------------------------------Lighter 4.x--//
	lighter_hysteresis=100;// 5 celsium;  1-->250 celsium 
	eeprom_write_byte(&eemem_lighter_hysteresis,lighter_hysteresis);//0
	
	lighter_flame_delay=5;// 1-->255 sec
	eeprom_write_byte(&eemem_lighter_flame_delay,lighter_flame_delay);//1
	
	lighter_kindling_time=2;//10 minute;    1-->15 min
	eeprom_write_byte(&eemem_lighter_kindling_time,lighter_kindling_time);//2
	
	lighter_attempt_kindling=3;//2;  1-->10 attempt
	eeprom_write_byte(&eemem_lighter_attempt_kindling,lighter_attempt_kindling);//3
	
	lighter_fuel_shortage=30;// 30 min;  1-->99 minut
	eeprom_write_byte(&eemem_lighter_fuel_shortage,lighter_fuel_shortage);//4
	
	//volatile uint8_t lighter_ignition_stab;//5
	lighter_time_ignition_stab=1;// 5 min;  1-->99
	eeprom_write_byte(&eemem_lighter_time_ignition_stab,lighter_time_ignition_stab);//6
	
	//volatile uint8_t lighter_pelets_fan; //7
	lighter_woods_fan=0;//0- use FAN1; 1-use FAN2; 2-use FAN1+FAN2
	eeprom_write_byte(&eemem_lighter_woods_fan,lighter_woods_fan);//8
	
	lighter_quenching_time=1; // 1 min;  1-> 30 min
	eeprom_write_byte(&eemem_lighter_quenching_time, lighter_quenching_time);//9
	//------end
	
	
	//--------------------------------- Cleaning mechanism 5.x--//
	mechanism_cleaning_work=15;// 150 sec;  1-->255 sec
	eeprom_write_byte(&eemem_mechanism_cleaning_work,mechanism_cleaning_work);
	
	mechanism_cleaning_delay=20;// 20 min;  1-->200 sec
	eeprom_write_byte(&eemem_mechanism_cleaning_delay,mechanism_cleaning_delay);
	
	//------------------------- Pump heating seting 6.x--//
	pump_period_work=0;// 0- TERMO,  1- AUTO
	eeprom_write_byte(&eemem_pump_period_work,pump_period_work);// 
	//------end
	
	
	//---------------------------------- Pump heating seting 7.x--//
	pump_hot_water_hystesis=5; //5 cilsium;  1-->20 celsium
	eeprom_write_byte(&eemem_pump_hot_water_hystesis,pump_hot_water_hystesis);//0
	
	pump_hot_water_increase=5;//5 celsium; 2-->20 celsium
	eeprom_write_byte(&eemem_pump_hot_water_increase, pump_hot_water_increase);//1
	
	pump_hot_water_overspending=2;//2 min;  1-->10 min
	eeprom_write_byte(&eemem_pump_hot_water_overspending,pump_hot_water_overspending);//2
	
	pump_hot_water_temp_work=50;// 50 celsium;  30-->60 celsium
	eeprom_write_byte(&eemem_pump_hot_water_temp_work,pump_hot_water_temp_work);//3
	
	pump_hot_water_hystesis_work=5;// 5 celsium;  1-->9 celsium
	eeprom_write_byte(&eemem_pump_hot_water_hystesis_work, pump_hot_water_hystesis_work);//4
	//------end
	
	
	//---------------------------------- Boiler seting 8.x--//
	kotel_min_temp=40;// 40C;   30-->69 celsium
	eeprom_write_byte(&eemem_kotel_min_temp, kotel_min_temp);// 0
	
	kotel_max_temp=70;// 90 C;   70-->90 celsium
	eeprom_write_byte(&eemem_kotel_max_temp, kotel_max_temp);// 1
	
	kotel_top_hystes=5;// 5 C;  1-->20 celsium
	eeprom_write_byte(&eemem_kotel_top_hystes, kotel_top_hystes);// 2
	
	kotel_hyste_swich_burner=2;// 1C;  1-->9 celsium
	eeprom_write_byte(&eemem_kotel_hyste_swich_burner, kotel_hyste_swich_burner);// 3
	
	kotel_protect=3;// 2 C;  1-->5 celsium
	eeprom_write_byte(&eemem_kotel_protect,kotel_protect);// 4
	
	kotel_temp_overheating=90;// 98 C;  90-->99 celsium
	eeprom_write_byte(&eemem_kotel_temp_overheating, kotel_temp_overheating);// 5
	//-------end
	//-----------------------------FAN2 Seting  9.x--//
	fan2_max_rotation_ignition=30;//70%  spead rotation;  1->100%
	eeprom_write_byte(&eemem_fan2_max_rotation_ignition, fan2_max_rotation_ignition);// 1
	
	fan2_rotation_ignition=50;
	eeprom_write_byte(&eemem_fan2_rotation_ignition,fan2_rotation_ignition); //  2
	
	fan2_rotation_max_power=100;
	eeprom_write_byte(&eemem_fan2_rotation_max_power,fan2_rotation_max_power);// 3
	
	fan2_rotation_min_power=40;
	eeprom_write_byte(&eemem_fan2_rotation_min_power, fan2_rotation_min_power);// 4
	
	fan2_rotation_quenching=50;
	eeprom_write_byte(&eemem_fan2_rotation_quenching, fan2_rotation_quenching);// 5
	
	fan2_rotation_cleaning=50; // 1 -> 100% 
	eeprom_write_byte(&eemem_fan2_rotation_cleaning, fan2_rotation_cleaning);// 6
	
	fan2_time_blowing=30;// 5 -> 60 sec
	eeprom_write_byte(&eemem_fan2_time_blowing, fan2_time_blowing);// 8
	
	fan2_delay_time=20;// 1 -> 99 min
	eeprom_write_byte(&eemem_fan2_delay_time, fan2_delay_time);// 9
	
	fan2_rotation_blowing=50;// 1 -> 100%
	eeprom_write_byte(&eemem_fan2_rotation_blowing, fan2_rotation_blowing);// 10
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

void Write16bitValueToLCD(uint16_t val, int8_t x, int8_t y)
{
	if (val==0xFFFF)
	{
		u8g2_DrawStr(&u8g2,x,y,"--");
		return;
	}
	int8_t width;
	uint16_t val_temp=0;
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

void TIMER16_3_Init(void)//--------------------------------------------------------TIMER_16_3___Init-------------------------//
{
	
	TCCR3B|=(1<<CS32);//|(1<<CS30); // 16 Mhz  / 256 prescal
	TCNT3H=0;
	TCNT3L=0;
	TIMSK3|=(1<<OCIE3A);
	//OCR3AH=0x01;
	//OCR3AL=0xF4; //--- milisec
	//
}
void TIMER16_4_Init(void)//--------------------------------------------------------TIMER_16_5___Init-------------------------//
{
	
	TCCR4B|=(1<<CS42);//|(1<<CS30); // 16 Mhz  / 256 prescal
	TCNT4H=0;
	TCNT4L=0;
	TIMSK4|=(1<<OCIE4A);
	//OCR3AH=0x01;
	//OCR3AL=0xF4; //--- milisec
	//
}
void TIMER16_1_Init(void)//--------------------------------------------------------TIMER16_1_Init----------------------------//
{
	TCCR1B|=(1<<CS12);//|(1<<CS30); // 16 Mhz  / 256 prescal
	TCNT1H=0;
	TCNT1L=0;
	TIMSK1|=(1<<OCIE1A);
	OCR1AH=0x01;
	OCR1AL=0xF4; // 500  = 125/sec==0.008 sec
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
	
	else if(R>=2418&&R<=2597)                         // 50 -> 60 celsium
	{
		if(R==2418)
		R=2419;
		double_koeficient=R-2418;
		double_koeficient=double_koeficient/17.9;
		int_koeficient=(uint8_t)double_koeficient;
		if (((uint8_t)((double_koeficient-(double)int_koeficient)*10))>=5)
		{
			int_koeficient=int_koeficient+1;
			celsium=int_koeficient+50;
		}
		else
		celsium=int_koeficient+50;
	}
	
	else if(R>=2598&&R<=2785)                         // 60 -> 70 celsium
	{
		if(R==2598)
		R=2599;
		double_koeficient=R-2598;
		double_koeficient=double_koeficient/18.7;
		int_koeficient=(uint8_t)double_koeficient;
		if (((uint8_t)((double_koeficient-(double)int_koeficient)*10))>=5)
		{
			int_koeficient=int_koeficient+1;
			celsium=int_koeficient+60;
		}
		else
		celsium=int_koeficient+60;
	}
	
	else if(R>=2786&&R<=2980)                         // 70 -> 80 celsium
	{
		if(R==2786)
		R=2787;
		double_koeficient=R-2786;
		double_koeficient=double_koeficient/19.4;
		int_koeficient=(uint8_t)double_koeficient;
		if (((uint8_t)((double_koeficient-(double)int_koeficient)*10))>=5)
		{
			int_koeficient=int_koeficient+1;
			celsium=int_koeficient+70;
		}
		else
		celsium=int_koeficient+70;
	}
	
	else if(R>=2981&&R<=3182)                         // 80 -> 90 celsium
	{
		if(R==2981)
		R=2982;
		double_koeficient=R-2981;
		double_koeficient=double_koeficient/20.1;
		int_koeficient=(uint8_t)double_koeficient;
		if (((uint8_t)((double_koeficient-(double)int_koeficient)*10))>=5)
		{
			int_koeficient=int_koeficient+1;
			celsium=int_koeficient+80;
		}
		else
		celsium=int_koeficient+80;
	}
	
	else if(R>=3183&&R<=3393)                         // 90 -> 100 celsium
	{
		if(R==3183)
		R=3184;
		double_koeficient=R-3183;
		double_koeficient=double_koeficient/20.9;
		int_koeficient=(uint8_t)double_koeficient;
		if (((uint8_t)((double_koeficient-(double)int_koeficient)*10))>=5)
		{
			int_koeficient=int_koeficient+1;
			celsium=int_koeficient+90;
		}
		else
		celsium=int_koeficient+90;
	}
	else if(R>=3393&&R<=3500)                         // 100 -> 110 celsium
	{
		if(R==3393)
		R=3394;
		double_koeficient=R-3393;
		double_koeficient=double_koeficient/21.4;
		int_koeficient=(uint8_t)double_koeficient;
		if (((uint8_t)((double_koeficient-(double)int_koeficient)*10))>=5)
		{
			int_koeficient=int_koeficient+1;
			celsium=int_koeficient+100;
		}
		else
		celsium=int_koeficient+100;
	}
	
	return celsium;
}

uint16_t Convert_PT1000_ADC_To_Celsium(uint16_t adc)
{
	uint16_t celsium16_t=0xFFFF;
	
	V_out=(adc*2.71)/1023;
	R=-1.0*((2.71*3000*V_out)/((2.71*V_out)-(5*2.71)));
	
	if(R>=1002&&R<1037)                                 // 0  ->  10 celsium
	{
		/*
		if(R==1002)
		R=1003;
		double_koeficient=R-1002;
		celsium16_t=(uint16_t)(double_koeficient/3.5);
		*/
		
		if (R>=9800&&R<1002)// 0
		{
			celsium16_t=0;
		}
		else if (R>=1002&&R<1007)// 1
		{
			celsium16_t=1;
		}
		else if (R>=1007&&R<1009)// 2
		{
			celsium16_t=2;
		}
		else if (R>=1009&&R<1013)// 3
		{
			celsium16_t=3;
		}
		else if (R>=1013&&R<1017)// 4
		{
			celsium16_t=4;
		}
		else if (R>=1017&&R<1021)// 5
		{
			celsium16_t=5;
		}
		else if (R>=1021&&R<1025)// 6
		{
			celsium16_t=6;
		}
		else if (R>=1025&&R<1029)// 7
		{
			celsium16_t=7;
		}
		else if (R>=1029&&R<1033)// 8
		{
			celsium16_t=8;
		}
		else if (R>=1033&&R<1037)// 9
		{
			celsium16_t=9;
		}
		
	}
	
	else if(R>=1037&&R<1076)                                 // 10  ->  20 celsium
	{
		/*
		if(R==1037)
		R=1038;
		double_koeficient=R-1037;
		celsium16_t=(uint16_t)(double_koeficient/3.9);
		celsium16_t=celsium16_t+10;
		*/
	    if (R>=1037&&R<1040)// 0
		{
			celsium16_t=10;
		}
		else if (R>=1040&&R<1044)// 1
		{
			celsium16_t=11;
		}
		else if (R>=1044&&R<1048)// 2
		{
			celsium16_t=12;
		}
		else if (R>=1048&&R<1052)// 3
		{
			celsium16_t=13;
		}
		else if (R>=1052&&R<1056)// 4
		{
			celsium16_t=14;
		}
		else if (R>=1056&&R<1060)// 5
		{
			celsium16_t=15;
		}
		else if (R>=1060&&R<1064)// 6
		{
			celsium16_t=16;
		}
		else if (R>=1064&&R<1068)// 7
		{
			celsium16_t=17;
		}
		else if (R>=1068&&R<1072)// 8
		{
			celsium16_t=18;
		}
		else if (R>=1072&&R<1076)// 9
		{
			celsium16_t=19;
		}
		
	}
	else if (R>=1076&&R<1115)                                // 20 -> 30 celsium
	{
		/*
		if(R==1076)
		R=1077;
		double_koeficient=R-1075;
		celsium16_t=(uint16_t)(double_koeficient/3.9);
		celsium16_t=celsium16_t+20;
		*/
		
		if (R>=1076&&R<1079)// 0
		{
			celsium16_t=20;
		}
		else if (R>=1079&&R<1083)// 1
		{
			celsium16_t=21;
		}
		else if (R>=1083&&R<1087)// 2
		{
			celsium16_t=22;
		}
		else if (R>=1087&&R<1091)// 3
		{
			celsium16_t=23;
		}
		else if (R>=1091&&R<1095)// 4
		{
			celsium16_t=24;
		}
		else if (R>=1095&&R<1099)// 5
		{
			celsium16_t=25;
		}
		else if (R>=1099&&R<1103)// 6
		{
			celsium16_t=26;
		}
		else if (R>=1103&&R<1107)// 7
		{
			celsium16_t=27;
		}
		else if (R>=1107&&R<1111)// 8
		{
			celsium16_t=28;
		}
		else if (R>=1111&&R<1115)// 9
		{
			celsium16_t=29;
		}
		
	}
	
	else if (R>=1115&&R<1153)                                // 30 -> 40 celsium
	{
	    /*	
		if(R==1114)
		R=1115;
		double_koeficient=R-1114;
		double_koeficient=double_koeficient/3.9;
		int_16_coficient=double_koeficient;
		celsium16_t=int_16_coficient+30+3;
		*/
		if (R>=1115&&R<1118)// 0
		{
			celsium16_t=30;
		}
		else if (R>=1118&&R<1122)// 1
		{
			celsium16_t=31;
		}
		else if (R>=1122&&R<1126)// 2
		{
			celsium16_t=32;
		}
		else if (R>=1126&&R<1130)// 3
		{
			celsium16_t=33;
		}
		else if (R>=1130&&R<1134)// 4
		{
			celsium16_t=34;
		}
		else if (R>=1134&&R<1138)// 5
		{
			celsium16_t=35;
		}
		else if (R>=1138&&R<1142)// 6
		{
			celsium16_t=36;
		}
		else if (R>=1142&&R<1146)// 7
		{
			celsium16_t=37;
		}
		else if (R>=1146&&R<1150)// 8
		{
			celsium16_t=38;
		}
		else if (R>=1150&&R<1153)// 9
		{
			celsium16_t=39;
		}
		
	}
	
	else if (R>=1153&&R<1192)                                // 40 -> 50 celsium
	{
		/*
		if(R==1153)
		R=1154;
		double_koeficient=R-1153;
		double_koeficient=double_koeficient/3.9;
		int_16_coficient=double_koeficient;
		celsium16_t=int_16_coficient+40+3;
		*/
		if (R>=1153&&R<1157)// 0
		{
			celsium16_t=40;
		}
		else if (R>=1157&&R<1161)// 1
		{
			celsium16_t=41;
		}
		else if (R>=1161&&R<1165)// 2
		{
			celsium16_t=42;
		}
		else if (R>=1165&&R<1168)// 3
		{
			celsium16_t=43;
		}
		else if (R>=1168&&R<1172)// 4
		{
			celsium16_t=44;
		}
		else if (R>=1172&&R<1176)// 5
		{
			celsium16_t=45;
		}
		else if (R>=1176&&R<1180)// 6
		{
			celsium16_t=46;
		}
		else if (R>=1180&&R<1184)// 7
		{
			celsium16_t=47;
		}
		else if (R>=1184&&R<1188)// 8
		{
			celsium16_t=48;
		}
		else if (R>=1188&&R<1192)// 9
		{
			celsium16_t=49;
		}
	}
	
	else if (R>=1192&&R<1230)                                // 50 -> 60 celsium
	{
		/*
		if(R==1192)
		R=1193;
		double_koeficient=R-1192;
		double_koeficient=double_koeficient/3.8;
		int_16_coficient=double_koeficient;
		celsium16_t=int_16_coficient+50+3;
		*/
		if (R>=1192&&R<1196)// 0
		{
			celsium16_t=50;
		}
		else if (R>=1196&&R<1200)// 1
		{
			celsium16_t=51;
		}
		else if (R>=1200&&R<1204)// 2
		{
			celsium16_t=52;
		}
		else if (R>=1204&&R<1208)// 3
		{
			celsium16_t=53;
		}
		else if (R>=1208&&R<1211)// 4
		{
			celsium16_t=54;
		}
		else if (R>=1211&&R<1215)// 5
		{
			celsium16_t=55;
		}
		else if (R>=1215&&R<1218)// 6
		{
			celsium16_t=56;
		}
		else if (R>=1218&&R<1222)// 7
		{
			celsium16_t=57;
		}
		else if (R>=1222&&R<1226)// 8
		{
			celsium16_t=58;
		}
		else if (R>=1226&&R<1230)// 9
		{
			celsium16_t=59;
		}
	}
	
	else if (R>=1230&&R<1269)                                // 60 -> 70 celsium
	{
		/*
		if(R==1230)
		R=1231;
		double_koeficient=R-1230;
		double_koeficient=double_koeficient/3.8;
		int_16_coficient=double_koeficient;
		celsium16_t=int_16_coficient+60+3;
		*/
		if (R>=1230&&R<1234)// 0
		{
			celsium16_t=60;
		}
		else if (R>=1234&&R<1238)// 1
		{
			celsium16_t=61;
		}
		else if (R>=1238&&R<1242)// 2
		{
			celsium16_t=62;
		}
		else if (R>=1242&&R<1246)// 3
		{
			celsium16_t=63;
		}
		else if (R>=1246&&R<1250)// 4
		{
			celsium16_t=64;
		}
		else if (R>=1250&&R<1253)// 5
		{
			celsium16_t=65;
		}
		else if (R>=1253&&R<1257)// 6
		{
			celsium16_t=66;
		}
		else if (R>=1257&&R<1261)// 7
		{
			celsium16_t=67;
		}
		else if (R>=1261&&R<1265)// 8
		{
			celsium16_t=68;
		}
		else if (R>=1265&&R<1269)// 9
		{
			celsium16_t=69;
		}
	}
	
	else if (R>=1269&&R<1308)                                // 70 -> 80 celsium
	{
		/*
		if(R==1268)
		R=1269;
		double_koeficient=R-1268;
		double_koeficient=double_koeficient/3.9;
		int_16_coficient=double_koeficient;
		celsium16_t=int_16_coficient+70+3;
		*/
		if (R>=1269&&R<1272)// 0
		{
			celsium16_t=70;
		}
		else if (R>=1272&&R<1276)// 1
		{
			celsium16_t=71;
		}
		else if (R>=1276&&R<1280)// 2
		{
			celsium16_t=72;
		}
		else if (R>=1280&&R<1284)// 3
		{
			celsium16_t=73;
		}
		else if (R>=1284&&R<1288)// 4
		{
			celsium16_t=74;
		}
		else if (R>=1288&&R<1292)// 5
		{
			celsium16_t=75;
		}
		else if (R>=1292&&R<1296)// 6
		{
			celsium16_t=76;
		}
		else if (R>=1296&&R<1300)// 7
		{
			celsium16_t=77;
		}
		else if (R>=1300&&R<1304)// 8
		{
			celsium16_t=78;
		}
		else if (R>=1304&&R<1308)// 9
		{
			celsium16_t=79;
		}
	}
	
	else if (R>=1308&&R<1345)                                // 80 -> 90 celsium
	{
		/*
		if(R==1307)
		R=1308;
		double_koeficient=R-1307;
		double_koeficient=double_koeficient/3.8;
		int_16_coficient=double_koeficient;
		celsium16_t=int_16_coficient+80+3;
		*/
		if (R>=1308&&R<1311)// 0
		{
			celsium16_t=80;
		}
		else if (R>=1311&&R<1314)// 1
		{
			celsium16_t=81;
		}
		else if (R>=1314&&R<1322)// 2
		{
			celsium16_t=82;
		}
		else if (R>=1320&&R<1324)// 3
		{
			celsium16_t=83;
		}
		else if (R>=1322&&R<1326)// 4
		{
			celsium16_t=84;
		}
		else if (R>=1326&&R<1330)// 5
		{
			celsium16_t=85;
		}
		else if (R>=1330&&R<1334)// 6
		{
			celsium16_t=86;
		}
		else if (R>=1334&&R<1338)// 7
		{
			celsium16_t=87;
		}
		else if (R>=1338&&R<1341)// 8
		{
			celsium16_t=88;
		}
		else if (R>=1341&&R<1345)// 9
		{
			celsium16_t=89;
		}
	}
	
	else if (R>=1345&&R<1383)                                // 90 -> 100 celsium
	{
		/*
		if(R==1345)
		R=1346;
		double_koeficient=R-1345;
		double_koeficient=double_koeficient/3.8;
		int_16_coficient=double_koeficient;
		celsium16_t=int_16_coficient+90+3;
		*/
		if (R>=1345&&R<1349)// 0
		{
			celsium16_t=90;
		}
		else if (R>=1349&&R<1353)// 1
		{
			celsium16_t=91;
		}
		else if (R>=1353&&R<1357)// 2
		{
			celsium16_t=92;
		}
		else if (R>=1357&&R<1360)// 3
		{
			celsium16_t=93;
		}
		else if (R>=1360&&R<1364)// 4
		{
			celsium16_t=94;
		}
		else if (R>=1364&&R<1368)// 5
		{
			celsium16_t=95;
		}
		else if (R>=1368&&R<1372)// 6
		{
			celsium16_t=96;
		}
		else if (R>=1372&&R<1376)// 7
		{
			celsium16_t=97;
		}
		else if (R>=1376&&R<1380)// 8
		{
			celsium16_t=98;
		}
		else if (R>=1380&&R<1383)// 9
		{
			celsium16_t=99;
		}
	}
	else if (R>=1383&&R<1422)                                // 100 -> 110 celsium
	{
		/*
		if(R==1345)
		R=1346;
		double_koeficient=R-1345;
		double_koeficient=double_koeficient/3.8;
		int_16_coficient=double_koeficient;
		celsium16_t=int_16_coficient+90+3;
		*/
		if (R>=1383&&R<1387)// 0
		{
			celsium16_t=100;
		}
		else if (R>=1387&&R<1391)// 1
		{
			celsium16_t=101;
		}
		else if (R>=1391&&R<1395)// 2
		{
			celsium16_t=102;
		}
		else if (R>=1395&&R<1398)// 3
		{
			celsium16_t=103;
		}
		else if (R>=1398&&R<1402)// 4
		{
			celsium16_t=104;
		}
		else if (R>=1402&&R<1406)// 5
		{
			celsium16_t=105;
		}
		else if (R>=1406&&R<1410)// 6
		{
			celsium16_t=106;
		}
		else if (R>=1410&&R<1414)// 7
		{
			celsium16_t=107;
		}
		else if (R>=1414&&R<1418)// 8
		{
			celsium16_t=108;
		}
		else if (R>=1418&&R<1422)// 9
		{
			celsium16_t=109;
		}
	}
	else if (R>=1422&&R<1459)                                // 110 -> 120 celsium
	{
		/*
		if(R==1345)
		R=1346;
		double_koeficient=R-1345;
		double_koeficient=double_koeficient/3.8;
		int_16_coficient=double_koeficient;
		celsium16_t=int_16_coficient+90+3;
		*/
		if (R>=1422&&R<1424)// 0
		{
			celsium16_t=110;
		}
		else if (R>=1424&&R<1428)// 1
		{
			celsium16_t=111;
		}
		else if (R>=1428&&R<1432)// 2
		{
			celsium16_t=112;
		}
		else if (R>=1432&&R<1436)// 3
		{
			celsium16_t=113;
		}
		else if (R>=1436&&R<1440)// 4
		{
			celsium16_t=114;
		}
		else if (R>=1440&&R<1444)// 5
		{
			celsium16_t=115;
		}
		else if (R>=1444&&R<1448)// 6
		{
			celsium16_t=116;
		}
		else if (R>=1448&&R<1452)// 7
		{
			celsium16_t=117;
		}
		else if (R>=1452&&R<1456)// 8
		{
			celsium16_t=118;
		}
		else if (R>=1456&&R<1459)// 9
		{
			celsium16_t=119;
		}
	}
	else if (R>=1459&&R<1496)                                // 120 -> 130 celsium
	{
		/*
		if(R==1345)
		R=1346;
		double_koeficient=R-1345;
		double_koeficient=double_koeficient/3.8;
		int_16_coficient=double_koeficient;
		celsium16_t=int_16_coficient+90+3;
		*/
		if (R>=1459&&R<1462)// 0
		{
			celsium16_t=120;
		}
		else if (R>=1462&&R<1466)// 1
		{
			celsium16_t=121;
		}
		else if (R>=1466&&R<1470)// 2
		{
			celsium16_t=122;
		}
		else if (R>=1470&&R<1474)// 3
		{
			celsium16_t=123;
		}
		else if (R>=1474&&R<1477)// 4
		{
			celsium16_t=124;
		}
		else if (R>=1477&&R<1482)// 5
		{
			celsium16_t=125;
		}
		else if (R>=1482&&R<1485)// 6
		{
			celsium16_t=126;
		}
		else if (R>=1485&&R<1489)// 7
		{
			celsium16_t=127;
		}
		else if (R>=1489&&R<1493)// 8
		{
			celsium16_t=128;
		}
		else if (R>=1493&&R<1496)// 9
		{
			celsium16_t=129;
		}
	}
	else if (R>=1496&&R<1534)                                // 130 -> 140 celsium
	{
		/*
		if(R==1345)
		R=1346;
		double_koeficient=R-1345;
		double_koeficient=double_koeficient/3.8;
		int_16_coficient=double_koeficient;
		celsium16_t=int_16_coficient+90+3;
		*/
		if (R>=1496&&R<1500)// 0
		{
			celsium16_t=130;
		}
		else if (R>=1500&&R<1504)// 1
		{
			celsium16_t=131;
		}
		else if (R>=1504&&R<1508)// 2
		{
			celsium16_t=132;
		}
		else if (R>=1508&&R<1512)// 3
		{
			celsium16_t=133;
		}
		else if (R>=1512&&R<1515)// 4
		{
			celsium16_t=134;
		}
		else if (R>=1515&&R<1519)// 5
		{
			celsium16_t=135;
		}
		else if (R>=1519&&R<1523)// 6
		{
			celsium16_t=136;
		}
		else if (R>=1523&&R<1527)// 7
		{
			celsium16_t=137;
		}
		else if (R>=1527&&R<1530)// 8
		{
			celsium16_t=138;
		}
		else if (R>=1530&&R<1534)// 9
		{
			celsium16_t=139;
		}
	}
	else if (R>=1534&&R<1571)                                // 140 -> 150 celsium
	{
		/*
		if(R==1345)
		R=1346;
		double_koeficient=R-1345;
		double_koeficient=double_koeficient/3.8;
		int_16_coficient=double_koeficient;
		celsium16_t=int_16_coficient+90+3;
		*/
		if (R>=1534&&R<1538)// 0
		{
			celsium16_t=140;
		}
		else if (R>=1538&&R<1542)// 1
		{
			celsium16_t=141;
		}
		else if (R>=1542&&R<1546)// 2
		{
			celsium16_t=142;
		}
		else if (R>=1546&&R<1549)// 3
		{
			celsium16_t=143;
		}
		else if (R>=1549&&R<1553)// 4
		{
			celsium16_t=144;
		}
		else if (R>=1553&&R<1557)// 5
		{
			celsium16_t=145;
		}
		else if (R>=1557&&R<1561)// 6
		{
			celsium16_t=146;
		}
		else if (R>=1561&&R<1564)// 7
		{
			celsium16_t=147;
		}
		else if (R>=1564&&R<1568)// 8
		{
			celsium16_t=148;
		}
		else if (R>=1568&&R<1571)// 9
		{
			celsium16_t=149;
		}
	}
	else if (R>=1571&&R<1608)                                // 150 -> 160 celsium
	{
	  if(R==1571)
	  R=1572;
	  double_koeficient=R-1571;
	  celsium16_t=(uint16_t)(double_koeficient/3.7);
	  celsium16_t=celsium16_t+150;
	}
	else if (R>=1608&&R<1646)                                // 160 -> 170 celsium
	{
		if(R==1608)
		R=1609;
		double_koeficient=R-1609;
		celsium16_t=(uint16_t)(double_koeficient/3.8);
		celsium16_t=celsium16_t+160;
	}
	else if (R>=1646&&R<1683)                                // 170 -> 180 celsium
	{
		if(R==1646)
		R=1647;
		double_koeficient=R-1646;
		celsium16_t=(uint16_t)(double_koeficient/3.7);
		celsium16_t=celsium16_t+170;
	}
	else if (R>=1683&&R<1720)                                // 180 -> 190 celsium
	{
		if(R==1683)
		R=1684;
		double_koeficient=R-1683;
		celsium16_t=(uint16_t)(double_koeficient/3.7);
		celsium16_t=celsium16_t+180;
	}
	else if (R>=1720&&R<1756)                                // 190 -> 200 celsium
	{
		if(R==1720)
		R=1721;
		double_koeficient=R-1720;
		celsium16_t=(uint16_t)(double_koeficient/3.6);
		celsium16_t=celsium16_t+190;
	}
	else if (R>=1756&&R<1793)                                // 200 -> 210 celsium
	{
		if(R==1756)
		R=1757;
		double_koeficient=R-1756;
		celsium16_t=(uint16_t)(double_koeficient/3.7);
		celsium16_t=celsium16_t+200;
	}
	else if (R>=1793&&R<1830)                                // 210 -> 220 celsium
	{
		if(R==1793)
		R=1794;
		double_koeficient=R-1793;
		celsium16_t=(uint16_t)(double_koeficient/3.7);
		celsium16_t=celsium16_t+210;
	}
	else if (R>=1830&&R<1866)                                // 220 -> 230 celsium
	{
		if(R==1830)
		R=1831;
		double_koeficient=R-1830;
		celsium16_t=(uint16_t)(double_koeficient/3.6);
		celsium16_t=celsium16_t+220;
	}
	else if (R>=1866&&R<1902)                                // 230 -> 240 celsium
	{
		if(R==1866)
		R=1861;
		double_koeficient=R-1866;
		celsium16_t=(uint16_t)(double_koeficient/3.6);
		celsium16_t=celsium16_t+230;
	}
	else if (R>=1902&&R<1938)                                // 240 -> 250 celsium
	{
		if(R==1902)
		R=1903;
		double_koeficient=R-1902;
		celsium16_t=(uint16_t)(double_koeficient/3.6);
		celsium16_t=celsium16_t+240;
	}
	else if (R>=1938&&R<1976)                                // 250 -> 260 celsium
	{
		if(R==1938)
		R=1939;
		double_koeficient=R-1938;
		celsium16_t=(uint16_t)(double_koeficient/3.8);
		celsium16_t=celsium16_t+250;
	}
	else if (R>=1976&&R<2010)                                // 260 -> 270 celsium
	{
		if(R==1976)
		R=1977;
		double_koeficient=R-1976;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+260;
	}
	else if (R>=2010&&R<2048)                                // 270 -> 280 celsium
	{
		if(R==2010)
		R=2011;
		double_koeficient=R-2010;
		celsium16_t=(uint16_t)(double_koeficient/3.8);
		celsium16_t=celsium16_t+270;
	}
	else if (R>=2048&&R<2083)                                // 280 -> 290 celsium
	{
		if(R==2048)
		R=2049;
		double_koeficient=R-2048;
		celsium16_t=(uint16_t)(double_koeficient/3.5);
		celsium16_t=celsium16_t+280;
	}
	else if (R>=2083&&R<2119)                                // 290 -> 300 celsium
	{
		if(R==2083)
		R=2084;
		double_koeficient=R-2083;
		celsium16_t=(uint16_t)(double_koeficient/3.6);
		celsium16_t=celsium16_t+290;
	}
	else if (R>=2119&&R<2154)                                // 300 -> 310 celsium
	{
		if(R==2119)
		R=2120;
		double_koeficient=R-2119;
		celsium16_t=(uint16_t)(double_koeficient/3.5);
		celsium16_t=celsium16_t+300;
	}
	else if (R>=2154&&R<2190)                                // 310 -> 320 celsium
	{
		if(R==2154)
		R=2155;
		double_koeficient=R-2154;
		celsium16_t=(uint16_t)(double_koeficient/3.6);
		celsium16_t=celsium16_t+310;
	}
	else if (R>=2190&&R<2225)                                // 320 -> 330 celsium
	{
		if(R==2190)
		R=2191;
		double_koeficient=R-2190;
		celsium16_t=(uint16_t)(double_koeficient/3.5);
		celsium16_t=celsium16_t+320;
	}
	else if (R>=2225&&R<2260)                                // 330 -> 340 celsium
	{
		if(R==2225)
		R=2226;
		double_koeficient=R-2225;
		celsium16_t=(uint16_t)(double_koeficient/3.5);
		celsium16_t=celsium16_t+330;
	}
	else if (R>=2260&&R<2295)                                // 340 -> 350 celsium
	{
		if(R==2260)
		R=2261;
		double_koeficient=R-2260;
		celsium16_t=(uint16_t)(double_koeficient/3.5);
		celsium16_t=celsium16_t+340;
	}
	else if (R>=2295&&R<2330)                                // 350 -> 360 celsium
	{
		if(R==2295)
		R=2296;
		double_koeficient=R-2295;
		celsium16_t=(uint16_t)(double_koeficient/3.5);
		celsium16_t=celsium16_t+350;
	}
	else if (R>=2330&&R<2365)                                // 360 -> 370 celsium
	{
		if(R==2330)
		R=2331;
		double_koeficient=R-2330;
		celsium16_t=(uint16_t)(double_koeficient/3.5);
		celsium16_t=celsium16_t+360;
	}
	else if (R>=2365&&R<2400)                                // 370 -> 380 celsium
	{
		if(R==2365)
		R=2364;
		double_koeficient=R-2365;
		celsium16_t=(uint16_t)(double_koeficient/3.5);
		celsium16_t=celsium16_t+370;
	}
	else if (R>=2400&&R<2434)                                // 380 -> 390 celsium
	{
		if(R==2400)
		R=2401;
		double_koeficient=R-2400;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+380;
	}
	else if (R>=2434&&R<2469)                                // 390 -> 400 celsium
	{
		if(R==2434)
		R=2435;
		double_koeficient=R-2434;
		celsium16_t=(uint16_t)(double_koeficient/3.5);
		celsium16_t=celsium16_t+390;
	}
	else if (R>=2469&&R<2504)                                // 400 -> 410 celsium
	{
		if(R==2469)
		R=2470;
		double_koeficient=R-2469;
		celsium16_t=(uint16_t)(double_koeficient/3.5);
		celsium16_t=celsium16_t+400;
	}
	else if (R>=2504&&R<2538)                                // 410 -> 420 celsium
	{
		if(R==2504)
		R=2505;
		double_koeficient=R-2504;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+410;
	}
	else if (R>=2538&&R<2572)                                // 420 -> 430 celsium
	{
		if(R==2538)
		R=2539;
		double_koeficient=R-2538;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+420;
	}
	else if (R>=2572&&R<2606)                                // 430 -> 440 celsium
	{
		if(R==2572)
		R=2573;
		double_koeficient=R-2572;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+430;
	}
	else if (R>=2606&&R<2640)                                // 440 -> 450 celsium
	{
		if(R==2606)
		R=2607;
		double_koeficient=R-2606;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+440;
	}
	else if (R>=2640&&R<2674)                                // 450 -> 460 celsium
	{
		if(R==2640)
		R=2641;
		double_koeficient=R-2640;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+450;
	}
	else if (R>=2674&&R<2708)                                // 460 -> 470 celsium
	{
		if(R==2674)
		R=2675;
		double_koeficient=R-2674;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+460;
	}
	else if (R>=2708&&R<2742)                                // 470 -> 480 celsium
	{
		if(R==2708)
		R=2709;
		double_koeficient=R-2708;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+470;
	}
	else if (R>=2742&&R<2775)                                // 480 -> 490 celsium
	{
		if(R==2742)
		R=2743;
		double_koeficient=R-2743;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+480;
	}
	else if (R>=2775&&R<2808)                                // 490 -> 500 celsium
	{
		if(R==2775)
		R=2776;
		double_koeficient=R-2775;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+490;
	}
	else if (R>=2808&&R<2842)                                // 500 -> 510 celsium
	{
		if(R==2808)
		R=2809;
		double_koeficient=R-2808;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+500;
	}
	else if (R>=2842&&R<2875)                                // 510 -> 520 celsium
	{
		if(R==2842)
		R=2843;
		double_koeficient=R-2842;
		celsium16_t=(uint16_t)(double_koeficient/3.4);
		celsium16_t=celsium16_t+510;
	}
	
	return celsium16_t;
}


void Init_Intterupt_6(void)
{
	//--------------------------------------------------------INT6__----------------------------------------------------//
	DDRE&=~(1<<PE6);//INT6
	PORTE&=~(1<<PE6);
	//EICRB|=((1<<ISC61)|(1<<ISC60));
	//EIMSK|=(1<<INT6); //Interrupt  INT6 ENABLE
}