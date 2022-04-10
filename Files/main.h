#ifndef main_H_INCLUDED
#define main_H_INCLUDED
#include <avr/eeprom.h>
//---------------------------------------------------------flag_yes_no------------------------------------//
#define FUEL_DEF               0// 0-Pellet;  1- Wood
#define SESON_DEF              1// 0 - Winter; 1- Summer
#define MODULATION_DEF         2   
#define BLOWING_DEF            3
#define PELETS_FAN_DEF         4
#define IGNITION_STAB_DEF      5
#define PUMP_HEATING_DEF       6
#define MODUL_POWER_BURNER_DEF 7
#define TERMOSTAT_WORK_DEF     8// 0 - NORM;  1- ADAPT
#define FAN2_YES_NO_DEF        9// 0- No Selected; 1- Selected
#define FAN2_MODULATION_DEF    10
#define FAN2_BLOWING_DEF       11
#define SELECT_CWU_DEF         12
#define DEFINE_DEF             13
#define ELECTROTEN_WORK_DEF    14
#define LIGHTER_FLAME_DELAY_DEF  15
//-------------------------------------------------------- flag_yes_no_2 ----------------------------------//
#define STOKER_WORK_MODE_DEF   0 //0- Cycle;  1- Auto
#define STOKER_WORK_DELAY_DEF  1// 0-Delay;   1-Work
#define IS_FAN1_ON             2
#define IF_FAN2_ON             3
#define FLAG_START_DOSE        4
//-------------------------- 
//-------------------------------------------------------flag_alarm---------------------------------------//
#define OWER_HEATING_ALARM     0
#define KOTEL_SENSOR_ALARM     1
#define BOILER_SENSOR_ALARM    2
#define BURNER_SENSOR_ALARM    3
#define SHNEK_SENSOR_ALARM     4
#define FUEL_SHORTAGE_DEF      5
#define SHNEK_OWER_HEAT_ALARM  6
#define HOME_SENSOR_ALARM      7
#define NO_SELECT_CWU_IN_SUMMER 8 
//--------------------------

#define CLEANING_TIME          33

#define TRUE                  1
#define FALSE                 0

//--------------------------------------------------------------------- Fans work flag----------------------------------------//
#define FAN1_DEF               0
#define FAN2_DEF               1
#define ELECTROTEN_DEF         2
//--------------------------

#define CO_ON              PORTC&=~(1<<PC4)
#define CO_OFF             PORTC|=(1<<PC4)//                  CO
#define IS_CO_ON           !(PINC&(1<<PC4))           

#define CWU_ON             PORTC&=~(1<<PC5)
#define CWU_OFF            PORTC|=(1<<PC5)
#define IS_CWU_ON          !(PINC&(1<<PC5))

#define SOUND_ON           PORTA&=~(1<<PA4)
#define SOUND_OFF          PORTA|=(1<<PA4)

#define BURNER_AUGER_ON    PORTC&=~(1<<PC0)
#define BURNER_AUGER_OFF   PORTC|=(1<<PC0)
#define IS_BURNER_AUGER_ON   !(PINC&(1<<PC0))
#define IS_BURNER_AUGER_OFF  PINC&(1<<PC0)

#define BUNKER_AUGER_ON    PORTC&=~(1<<PC1)
#define BUNKER_AUGER_OFF   PORTC|=(1<<PC1)
#define IS_BUNKER_AUGER_ON   !(PINC&(1<<PC1))

#define CLEANING_MECH_ON   PORTC&=~(1<<PC2)
#define CLEANING_MECH_OFF  PORTC|=(1<<PC2)

#define ELECTROTEN_ON      PORTC&=~(1<<PC3)
#define ELECTROTEN_OFF     PORTC|=(1<<PC3)
#define IS_ELECTROTEN_OFF   PINC&(1<<PC3)
#define IS_ELECTROTEN_ON  !(PINC&(1<<PC3)) 



#define FAN2_ON            PORTA&=~(1<<PA6) 
#define FAN2_OFF           PORTA|=(1<<PA6)
//#define IS_FAN2_ON         !(PINA&(1<<PA6)) 

#define FAN1_ON            PORTA&=~(1<<PA7)
#define FAN1_OFF           PORTA|=(1<<PA7)                
//#define IS_FAN1_ON         !(PINA&(1<<PA7))

#define TERMOSTAT_IN       PORTF|=(1<<PF3)
#define IS_TERMOSTAT_ON    !(PINF&(1<<PF3))
#define IS_TERMOSTAT_OFF   (PINF&(1<<PF3))

void Read_Temperature(void);
void Defaut_Set(void);
// DELET !!!!!!!!!!!!!!!!!!!!!!!!!!
uint8_t time_count;
//

volatile uint8_t flag_work_mode_template;
volatile uint8_t flag_cleaning_mode;


volatile uint8_t time_sec;
volatile uint8_t flame_y_position;//  start flame position
volatile uint8_t cleaning_x_position;// start cleaning position


volatile uint16_t flag_yes_no;
volatile uint8_t  flag_yes_no_2;
volatile uint16_t flag_alarm;
volatile uint8_t flag_fans_work;
volatile uint8_t flag_fan1_fan2_blowing;





//volatile uint8_t flag_ignition;
//
volatile int8_t count_glob_val;
volatile int8_t f_ecoder_button;

 //volatile int8_t f_clear_lcd;
 
 volatile uint8_t f_timer_0;
// volatile int8_t fan_glob_val;
 //volatile int8_t fan2_glob_val;
 volatile uint8_t start_stop_template;

 volatile int8_t f_sel_list;
 volatile uint8_t f_curent_pos;
 volatile uint8_t f_menu_timer_0;
 volatile uint8_t fan1_spead_var;
 volatile uint8_t fan2_spead_var;
 
 volatile uint32_t blowing_timer;
 volatile uint32_t fan2_blowing_timer;
 volatile uint32_t cleaning_delay_timer;
 
 volatile uint32_t timer_template_var;
 volatile uint8_t kindli_time_template; 
 volatile uint16_t timer_template_dose;
 volatile uint16_t timer_template_stoker_dose;
 volatile uint16_t timer_count_delay_flame;
 //---------------------------------- For ADC function (current var)-------//
 volatile uint8_t kotel_temperature;  
 volatile uint16_t burner_temperature;
 volatile uint8_t boiler_temperature;
 volatile uint8_t shnek_temperature;
 volatile uint8_t home_temperature;
   
 volatile uint8_t template_temperature_all;
 volatile uint8_t permission_read;
 //-----------------------------MAIN seting 1.x---------------------------//
 volatile uint8_t setup_boiler_temperature;
 volatile uint8_t boil_power_percent;
 volatile uint8_t m_hot_water;
 volatile uint16_t m_burner_temp;
 //volatile uint8_t HWS;
 //-----------------------------FAN Seting  2.x --------------------------//
 volatile uint8_t max_rotation_ignition;// 1
 volatile uint8_t rotation_ignition; //  2 
 volatile uint8_t rotation_max_power;// 3
 volatile uint8_t rotation_min_power;// 4
 volatile uint8_t rotation_quenching;// 5
 volatile uint8_t rotation_cleaning;// 6
 volatile uint8_t time_blowing;// 8
 volatile uint8_t delay_time;// 9
 volatile uint8_t rotation_blowing;// 10
 //------end fan seting
 //----------------------------BUNKER Seting 3.x---------------------------//
 volatile uint8_t start_dose;//0
 volatile uint8_t supply_cycle;//1
 volatile uint8_t dose_max_power;// 2
 volatile uint8_t dose_min_power;// 3
 volatile uint8_t stoker_time_work;// 4
 volatile uint8_t stoker_time_delay;// 5
 volatile uint8_t stoker_time_continuation;
 volatile uint8_t stoker_time_emptying;
 volatile uint8_t stoker_fervor;
 //------end bunker seting
 //----------------------------------------Lighter 4.x-------------------//
 volatile uint8_t lighter_hysteresis;//0
 volatile uint8_t lighter_flame_delay;//1
 volatile uint8_t lighter_kindling_time;//2
 volatile uint8_t lighter_attempt_kindling;//3
 volatile uint8_t lighter_fuel_shortage;//4
 volatile uint8_t lighter_time_ignition_stab;//6
 volatile uint8_t lighter_woods_fan;//8
 volatile uint8_t lighter_quenching_time;//9    5.13
 //------end
 //---------------------------------------- Cleaning mechanism 5.x-------//
 volatile uint8_t mechanism_cleaning_work;// 0
 volatile uint8_t mechanism_cleaning_delay;// 1
 //---------------------------------------- Pump CO seting 6.x------//
 volatile uint8_t pump_period_work;
 //------end 
 //---------------------------------------- Pump heating seting 7.x------//
 volatile uint8_t pump_hot_water_hystesis;//0
 volatile uint8_t pump_hot_water_increase;//1
 volatile uint8_t pump_hot_water_overspending;//2
 volatile uint8_t pump_hot_water_temp_work;//3
 volatile uint8_t pump_hot_water_hystesis_work;//4  
  //------end 
 //---------------------------------------- Kotel seting 8.x------------//
 volatile uint8_t kotel_min_temp;// 0
 volatile uint8_t kotel_max_temp;// 1
 volatile uint8_t kotel_top_hystes;// 2
 volatile uint8_t kotel_hyste_swich_burner;// 3
 volatile uint8_t kotel_protect;// 4
 volatile uint8_t kotel_temp_overheating;// 5
 //-------end
 //-----------------------------FAN Seting  9.x --------------------------//
 volatile uint8_t fan2_max_rotation_ignition;// 1
 volatile uint8_t fan2_rotation_ignition; //  2
 volatile uint8_t fan2_rotation_max_power;// 3
 volatile uint8_t fan2_rotation_min_power;// 4
 volatile uint8_t fan2_rotation_quenching;// 5
 volatile uint8_t fan2_rotation_cleaning;// 6
 volatile uint8_t fan2_time_blowing;// 8
 volatile uint8_t fan2_delay_time;// 9
 volatile uint8_t fan2_rotation_blowing;// 10
 //------end fan seting
 //volatile uint8_t 
 enum enumWindow
 {
	 //----------------------------------------Main page----------------//
	 MAIN,
	 //MAIN_WITH_HOT_WAER,
	 SETUP,
	 //-------------------------------------Main seting 1.0 ---------------//
	 MAIN_SETING,
	 FUEL_TYPE,
	 BOIL_TEMPERATURE,
     BOILER_POWER,
     SEASON,
	 M_HOT_WATER,
	 M_BURNER_TEMP,
	 M_SELECT_CWU,
	 M_DEFAULT,
	 //---------------------------------------FAN Setup 2.0-------------// 
	 FAN_SETUP,
	 MODULATION,//0
	 HEATING, // kindling/ ignition//1
	 HEATING_FAN_ROTATION,//2
	 MAX_POWER,//3
	 MIN_POWER,//4
	 QUENCHING,//5
	 CLEANING,//6
	 BLOWING,//7
	 BLOWING_TIME,//8
	 BLOWING_BREAK_TIME,//9
	 BLOWING_ROTATION,//10
	 //---------------end
	 //--------------------------------------Bunker 3.0-----------------//
	 BUNKER_SETUP,
	 START_DOSE,  // 0
	 SUPPLY_CYCLE,// 1
	 DOSE_MAX_POWER,// 2
	 DOSE_MIN_POWER,// 3
	 STOKER_TIME_WORK,// 4
	 STOKER_TIME_DELAY,// 5
	 STOKER_TIME_CONTINUATION,// 6
	 STOKER_TIME_EMPTYING,// 7
	 STOKER_FERVOR,// 8
	 STOKER_MODE_WORK,//9
	 //---------------end
	 //--------------------------------------Lighter 4.0--------------------//
	 LIGHTER_SETUP,//main
	 HYSTERESIS, //0
	 FLAME_DELAY, //1
	 KINDLING_TIME, //2
	 ATTEMPT_KINDLING, //3
	 FUEL_SHORTAGE, //4
	 IGNITION_STAB,//5
	 TIME_IGNITION_STAB,//6
	 PELETS_FAN,//7   //FAN1 (0), FAN1+FAN2 (1)
	 WOODS_FAN,//8   // FAN1(0), FAN2(1), FAN1 + FAN2 (2) 
	 QUENCHING_TIME,
	 //---------------end
	 //-------------------------------------- Cleaning mechanism 5.0------------//
	 CLEAN_MECHANISM_SETUP,// main
	 MECHA_CLEAN_WORK,// 0
	 MECHA_CLEAN_DELAY,// 1
	 //---------------end
	 //---------------------------------------- Heating pump seting 6.x----------//
	 PUMP_HEATING_SETUP,//0
	 PUMP_HEATING_MODE,//1
	 //---------------end
	 //---------------------------------------- Pump_hot_water seting 7.x--------//
	 PUMP_HOT_WATER_SETUP,
	 PUMP_HOT_WATER_HYSTESIS,//0
	 PUMP_HOT_WATER_INCREASE,//1
	 PUMP_HOT_WATER_OVERSPENDING,//2
	 PUMP_HOT_WATER_TEMP_WORK,//3
	 PUMP_HOT_WATER_HYSTESIS_WORK,//4
	 //---------------end
	 //--------------------------------------- Boiler seting 8.x ----------------//
	 BOILER_SETUP,
	 BOILER_MIN_TEMP,// 0
	 BOILER_MAX_TEMP,// 1
	 BOILER_TOP_HYSTES,// 2
	 BOILER_HYSTES_SWICH_BURNER,// 3
	 BOILER_HYSTES_PROTECT,// 4
	 BOILER_TEMP_OVERHEATING,// 5
	 BOILER_MODUL_POWER_BURNER,//6
	 BOILER_TERMOSTAT_WORK,// 7
	 //---------------end
	 //---------------------------------------FAN Setup 9.x---------------------//
	 FAN_2_SETUP,
	 FAN2_YES_NO,//0
	 FAN2_MODULATION,//1
	 FAN2_HEATING, // kindling/ ignition//2
	 FAN2_HEATING_FAN_ROTATION,//3
	 FAN2_MAX_POWER,//4
	 FAN2_MIN_POWER,//5
	 FAN2_QUENCHING,//6
	 FAN2_CLEANING,//7
	 FAN2_BLOWING,//8
	 FAN2_BLOWING_TIME,//9
	 FAN2_BLOWING_BREAK_TIME,//10
	 FAN2_BLOWING_ROTATION//11
	 //---------------end
	 }eWindow;
 //volatile int8_t 
enum enumWork_Mode
{
	//W_IN_STOP_MODE,//0
	W_MONITORING,//1
	W_IGNITION,//2
	W_IGNITION_STABILIZ,//3
	W_MAX_POWER,//4
	W_MODULATED_POWER,//5
	W_MIN_POWER,//6
	W_BLOWING,//7
	W_AFTERBURNING,//8
	W_ATTENUATION,//9
	W_CLEANING,//10
	W_FILLING,//11
	W_EXTINGUISHING,//12
}eWork_Mode;


enum enumSTART_STOP
{
	STOP,
	START
	}eSTART_STOP;

enum enumUpdate
{
	NO_UPDATE,
	UPDATE
	}eUpdate;
enum enumDirection
{
	UP,
	DOWN,
	NOT_MOVE
	}eDirection;


#endif