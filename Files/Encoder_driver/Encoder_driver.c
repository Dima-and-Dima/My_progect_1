#include "Encoder_driver.h"
#include "u8g2.h"
#include "main.h"
extern u8g2_t u8g2;

volatile unsigned long int debounced_state = 0,debounced_stateOld = 0;

volatile uint8_t pin_state=1;

enum enumButton
{
	ENCODER_BUTTON,
	MENU_BUTTON,
	ESC_BUTTON,
	START_BUTTON,
	STOP_BUTTON,
	}eButton;
	
static unsigned char state, cnt0, cnt1;
unsigned char delta, toggle;
unsigned char debounce(unsigned char sample)
{
	
	
	delta = sample ^ state;
	  
	cnt1 = cnt1 ^ cnt0;
	cnt0 = ~cnt0;
	
	cnt0 &= delta;
	cnt1 &= delta;
	
	toggle = cnt0 & cnt1;
	state ^= toggle;
	
	return state;
}
void Encoder_Init(void)
{
	Encoder_DDR_1;
	Encoder_DDR_2;
	Encoder_Button_DDR;
	Menu_Button_DDR;
	ESC_Button_DDR;
	Start_Button_DDR;
	Stop_Button_DDR;
    Encoder_PORT_1;
	Encoder_PORT_2;
	Encoder_Button_PORT;
	Menu_Button_PORT;
	ESC_Button_PORT;
	Start_Button_PORT;
	Stop_Button_PORT;
}
void TIMER2_Init()
{
	//--------------------------------------------------------TIMER2___----------------------------------------------
	TCCR2B|=(1<<CS22);//|(1<<CS20)|(1<<CS21);//|(1<<CS20);   // 16 MHz/
	TIMSK2|=(1<<TOIE2);
	TCNT2=0x00;
}

uint8_t read_gray_code_from_encoder(void )
{
	uint8_t val=0;

	if(!bit_is_clear(Encoder_P_2,Encoder_PIN_2))
	val |= (1<<1);

	if(!bit_is_clear(Encoder_P_1, Encoder_PIN_1))
	val |= (1<<0);
	
	return val;
}

ISR(TIMER0_OVF_vect)            //   Button intterupt
{
	unsigned char sreg;
	/* Save global interrupt flag */
	sreg = SREG;
  cli();
  switch(eButton)
   {
	 case MENU_BUTTON:
	 {
	   pin_state=(Menu_Button_P&(1<<Menu_Button_Pin));
	   debounce(pin_state);
	   if(f_timer_0==2)
	   {
		   //PORTA&=~(1<<PA4);
		   SOUND_ON;
	     if(debounce(pin_state)==0)
	     {
		   if(count_glob_val>=9)
		       count_glob_val=0;
		    count_glob_val++;
		   if(eWindow==MAIN)  
		   {
			   eUpdate=UPDATE;
			   //f_clear_lcd=1;
			   eWindow=SETUP;
			   //f_curent_pos=0;
		   }
		   else if(eWindow==SETUP)
		   {
			   eUpdate=UPDATE;
			   //f_clear_lcd=1;
			  // f_curent_pos=0;
			   eWindow=MAIN;
			  
		   }
		   else if (eWindow==MAIN_SETING)
		   {
			   eUpdate=UPDATE;
			   //f_clear_lcd=1;
			  // f_curent_pos=0;
			   eWindow=SETUP;
		   }
		   else if (eWindow==FAN_SETUP)
		   {
			  eUpdate=UPDATE;
			  //f_clear_lcd=1;
			 // f_curent_pos=0;
			  eWindow=SETUP; 
		   }
		  
			//------------------------------------------- MAIN SEtup -----------------------//
			else if (eWindow==FUEL_TYPE)
			{
				 eUpdate=UPDATE;
			
				eWindow=MAIN_SETING;
			 }
			else if (eWindow==BOIL_TEMPERATURE)
			{
				eUpdate=UPDATE;
				
				eWindow=MAIN_SETING;
			}
			else if (eWindow==BOILER_POWER)
			{
				eUpdate=UPDATE;
				
				eWindow=MAIN_SETING;
			}
			else if (eWindow==SEASON)
			{
				eUpdate=UPDATE;
				
				eWindow=MAIN_SETING;
			}
			else if (eWindow==M_HOT_WATER)
			{
				eUpdate=UPDATE;
				
				eWindow=MAIN_SETING;
			}
			else if (eWindow==M_BURNER_TEMP)
			{
				eUpdate=UPDATE;
				
				eWindow=MAIN_SETING;
			}
			else if (eWindow==M_SELECT_CWU)
			{
				eUpdate=UPDATE;
				
				eWindow=MAIN_SETING;
			}
			else if (eWindow==M_DEFAULT)
			{
				eUpdate=UPDATE;
				
				eWindow=MAIN_SETING;
			}
			
			//----------------------------------------------------------Fan setup 2.0----------------------//
			
			else if (eWindow==MODULATION)// 0
			{
				eUpdate=UPDATE;
				/*
				f_clear_lcd=1;
				if (f_sel_list==1)
				{
					//eSeason=SUMER;
					flag_yes_no|=(1<<MODULATION_DEF);
				}
				else //if (f_sel_list==1)
				{
					//eSeason=WINTER;
					flag_yes_no&=~(1<<MODULATION_DEF);
				}
				//f_curent_pos=0;
				*/
				eWindow=FAN_SETUP;
			}
			else if (eWindow==HEATING)//1
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_SETUP;
			}
			else if (eWindow==HEATING_FAN_ROTATION)//2
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_SETUP;
			}
			else if (eWindow==MAX_POWER)//3
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_SETUP;
			}
			else if (eWindow==MIN_POWER)//4
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_SETUP;
			}
			else if (eWindow==QUENCHING)//5
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_SETUP;
			}
			else if (eWindow==CLEANING)//6
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_SETUP;
			}
			else if (eWindow==BLOWING)//7
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_SETUP;
			}
			else if (eWindow==BLOWING_TIME)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_SETUP;
			}
			else if (eWindow==BLOWING_BREAK_TIME)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_SETUP;
			}
			else if (eWindow==BLOWING_ROTATION)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_SETUP;
			}
			//----------------end fan seting 2.0
			
			//------------------------------------------------------BUNKER seting 3.0------------------------------------//
			
			else if (eWindow==BUNKER_SETUP)  //Bunker seting
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=SETUP;//  main listing
			}
			else if (eWindow==START_DOSE)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=BUNKER_SETUP;
			}
			else if (eWindow==SUPPLY_CYCLE)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=BUNKER_SETUP;
			}
			else if (eWindow==DOSE_MAX_POWER)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=BUNKER_SETUP;
			}
			else if (eWindow==DOSE_MIN_POWER)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=BUNKER_SETUP;
			}
			else if (eWindow==STOKER_TIME_WORK)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=BUNKER_SETUP;
			}
			else if (eWindow==STOKER_TIME_DELAY)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=BUNKER_SETUP;
			}
			else if (eWindow==STOKER_TIME_CONTINUATION)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=BUNKER_SETUP;
			}
			else if (eWindow==STOKER_TIME_EMPTYING)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=BUNKER_SETUP;
			}
			else if (eWindow==STOKER_FERVOR)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=BUNKER_SETUP;
			}
			else if (eWindow==STOKER_MODE_WORK)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=BUNKER_SETUP;
			}	
	   // }
	 //------------------------------------------------------ LIGHTER seting 4.0------------------------------------//
	 
	   else if (eWindow==LIGHTER_SETUP)
	   {
		 eUpdate=UPDATE;
		 //f_clear_lcd=1;
		 //f_curent_pos=0;
		 eWindow=SETUP;//  main listing !!!!! 00000
	   }
	   else if (eWindow==HYSTERESIS) // 0
	   {
		 eUpdate=UPDATE;
		 //f_clear_lcd=1;
		 //f_curent_pos=0;
		 eWindow=LIGHTER_SETUP;
	   }
	   else if (eWindow==FLAME_DELAY)// 1
	   {
		 eUpdate=UPDATE;
		 //f_clear_lcd=1;
		 //f_curent_pos=0;
		 eWindow=LIGHTER_SETUP;
	   }
	   else if (eWindow==KINDLING_TIME)//2
	   {
		 eUpdate=UPDATE;
		// f_clear_lcd=1;
		 //f_curent_pos=0;
		 eWindow=LIGHTER_SETUP;
	   }
	   else if (eWindow==ATTEMPT_KINDLING)//3
	   {
		 eUpdate=UPDATE;
		// f_clear_lcd=1;
		 //f_curent_pos=0;
		 eWindow=LIGHTER_SETUP;
	    }
	     else if (eWindow==FUEL_SHORTAGE)//4
	    {
	  	 eUpdate=UPDATE;
		// f_clear_lcd=1;
		 //f_curent_pos=0;
		 eWindow=LIGHTER_SETUP;
	     }
	    else if (eWindow==IGNITION_STAB)//5
	     {
		 eUpdate=UPDATE;
		// f_clear_lcd=1;
		 //f_curent_pos=0;
		 eWindow=LIGHTER_SETUP;
	     }
	     else if (eWindow==TIME_IGNITION_STAB)//6
	     {
		 eUpdate=UPDATE;
		// f_clear_lcd=1;
		 //f_curent_pos=0;
		 eWindow=LIGHTER_SETUP;
	     }
	     else if (eWindow==PELETS_FAN)//7
	     {
		 eUpdate=UPDATE;
		// f_clear_lcd=1;
		 //f_curent_pos=0;
		 eWindow=LIGHTER_SETUP;
	     }
	     else if (eWindow==WOODS_FAN)//8
	     {
		 eUpdate=UPDATE;
		 //f_clear_lcd=1;
		 //f_curent_pos=0;
		 eWindow=LIGHTER_SETUP;
	     }
		 else if (eWindow==QUENCHING_TIME)//9
		 {
			 eUpdate=UPDATE;
			 //f_clear_lcd=1;
			 //f_curent_pos=0;
			 eWindow=LIGHTER_SETUP;
		 }
	   //------------------------------------------------------ Cleaning mechanism 5.0------------------------------------//
	   
	     else if (eWindow==CLEAN_MECHANISM_SETUP)
	     {
		   eUpdate=UPDATE;
		 //  f_clear_lcd=1;
		   //f_curent_pos=0;
		   eWindow=SETUP;//  main listing !!!!! 00000
	     }
	     else if (eWindow==MECHA_CLEAN_WORK) // 0
	     {
		   eUpdate=UPDATE;
		  // f_clear_lcd=1;
		   //f_curent_pos=0;
		   eWindow=CLEAN_MECHANISM_SETUP;
	     }
	     else if (eWindow==MECHA_CLEAN_DELAY)
	     {
	 	   eUpdate=UPDATE;
		   //f_clear_lcd=1;
		   //f_curent_pos=0;
		   eWindow=CLEAN_MECHANISM_SETUP;//  main listing !!!!! 00000
	     }
		 //--------------------------------------Pump heating seting 6.0--------------------//
		 else if (eWindow==PUMP_HEATING_SETUP) 
		 {
			 eUpdate=UPDATE;
			// f_clear_lcd=1;
			 //f_curent_pos=0;
			 eWindow=SETUP;
		 }
		 else if (eWindow==PUMP_HEATING_MODE)
		 {
			 eUpdate=UPDATE;
			// f_clear_lcd=1;
			 //f_curent_pos=0;
			 eWindow=PUMP_HEATING_SETUP;// 
		 }
		  //------------------------------------Pump hot water seting 7.0-------------------//
		 else if (eWindow==PUMP_HOT_WATER_SETUP)
		 {
			 eUpdate=UPDATE;
			// f_clear_lcd=1;
			 //f_curent_pos=0;
			 eWindow=SETUP;
		 }
		 else if (eWindow==PUMP_HOT_WATER_HYSTESIS)//0
		 {
			 eUpdate=UPDATE;
			 //f_clear_lcd=1;
			 //f_curent_pos=0;
			 eWindow=PUMP_HOT_WATER_SETUP;//
		 }
		 else if (eWindow==PUMP_HOT_WATER_INCREASE)//1
		 {
			 eUpdate=UPDATE;
			// f_clear_lcd=1;
			 //f_curent_pos=0;
			 eWindow=PUMP_HOT_WATER_SETUP;//
		 }
		 else if (eWindow==PUMP_HOT_WATER_OVERSPENDING)//2
		 {
			 eUpdate=UPDATE;
			// f_clear_lcd=1;
			 //f_curent_pos=0;
			 eWindow=PUMP_HOT_WATER_SETUP;//
		 }
		 else if (eWindow==PUMP_HOT_WATER_TEMP_WORK)//3
		 {
			 eUpdate=UPDATE;
			// f_clear_lcd=1;
			 //f_curent_pos=0;
			 eWindow=PUMP_HOT_WATER_SETUP;//
		 }
		 else if (eWindow==PUMP_HOT_WATER_HYSTESIS_WORK)//4
		 {
			 eUpdate=UPDATE;
			 //f_clear_lcd=1;
			 //f_curent_pos=0;
			 eWindow=PUMP_HOT_WATER_SETUP;//
		 }
	   //------------------------------------Pump hot water seting 8.0-------------------//
	   else if (eWindow==BOILER_SETUP)
	   {
		   eUpdate=UPDATE;
		   //f_clear_lcd=1;
		   //f_curent_pos=0;
		   eWindow=SETUP;
	   }
	    else if (eWindow==BOILER_MIN_TEMP)// 0
	    {
		    eUpdate=UPDATE;
		   // f_clear_lcd=1;
		    //f_curent_pos=0;
		    eWindow=BOILER_SETUP;
	    } 
	    else if (eWindow==BOILER_MAX_TEMP)// 1
	    {
		    eUpdate=UPDATE;
		   // f_clear_lcd=1;
		    //f_curent_pos=0;
		    eWindow=BOILER_SETUP;
	    }
	   else if (eWindow==BOILER_TOP_HYSTES)// 2
	   {
		   eUpdate=UPDATE;
		  // f_clear_lcd=1;
		   //f_curent_pos=0;
		   eWindow=BOILER_SETUP;
	   }
	   else if (eWindow==BOILER_HYSTES_SWICH_BURNER)// 3
	   {
		   eUpdate=UPDATE;
		  // f_clear_lcd=1;
		   //f_curent_pos=0;
		   eWindow=BOILER_SETUP;
	   }
	   else if (eWindow==BOILER_HYSTES_PROTECT)// 4
	   {
		   eUpdate=UPDATE;
		   //f_clear_lcd=1;
		   //f_curent_pos=0;
		   eWindow=BOILER_SETUP;
	   }
	   else if (eWindow==BOILER_TEMP_OVERHEATING)// 5
	   {
		   eUpdate=UPDATE;
		  // f_clear_lcd=1;
		   //f_curent_pos=0;
		   eWindow=BOILER_SETUP;
	   }
	   else if (eWindow==BOILER_MODUL_POWER_BURNER)// 6
	   {
		   eUpdate=UPDATE;
		  // f_clear_lcd=1;
		   //f_curent_pos=0;
		   eWindow=BOILER_SETUP;
	   }
	   else if (eWindow==BOILER_TERMOSTAT_WORK)// 7
	   {
		   eUpdate=UPDATE;
		  // f_clear_lcd=1;
		   //f_curent_pos=0;
		   eWindow=BOILER_SETUP;
	   }	
	   //---------------------------------------------------Fan setup 9.0----------------------//
		 else if (eWindow==FAN_2_SETUP)// 0
		 {
			 eUpdate=UPDATE;
			 
			 eWindow=SETUP;
		 }
		   
		   else if (eWindow==FAN2_YES_NO)// 0
		   {
			   eUpdate=UPDATE;
			   
			   eWindow=FAN_2_SETUP;
		   }	
			else if (eWindow==FAN2_MODULATION)// 1
			{
				eUpdate=UPDATE;
				
				eWindow=FAN_2_SETUP;
			}
			else if (eWindow==FAN2_HEATING)//
			{
				eUpdate=UPDATE;
				
				eWindow=FAN_2_SETUP;
			}
			else if (eWindow==FAN2_HEATING_FAN_ROTATION)//2
			{
				eUpdate=UPDATE;
				
				eWindow=FAN_2_SETUP;
			}
			else if (eWindow==FAN2_MAX_POWER)//3
			{
				eUpdate=UPDATE;
				
				eWindow=FAN_2_SETUP;
			}
			else if (eWindow==FAN2_MIN_POWER)//4
			{
				eUpdate=UPDATE;
				
				eWindow=FAN_2_SETUP;
			}
			else if (eWindow==FAN2_QUENCHING)//5
			{
				eUpdate=UPDATE;
				
				eWindow=FAN_2_SETUP;
			}
			else if (eWindow==FAN2_CLEANING)//6
			{
				eUpdate=UPDATE;
				
				eWindow=FAN_2_SETUP;
			}
			else if (eWindow==FAN2_BLOWING)//7
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_2_SETUP;
			}
			else if (eWindow==FAN2_BLOWING_TIME)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_SETUP;
			}
			else if (eWindow==FAN2_BLOWING_BREAK_TIME)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_2_SETUP;
			}
			else if (eWindow==FAN2_BLOWING_ROTATION)
			{
				eUpdate=UPDATE;
				//f_clear_lcd=1;
				//f_curent_pos=0;
				eWindow=FAN_2_SETUP;
			}
			//----------------end fan seting 2.0	 
	   	
		
		 
	   }
	 }
	 
	 //if(f_timer_0>=2)
	 // {
		  //PORTA|=(1<<PA4);
		  //SOUND_OFF;
	 // }
	 
	 if(f_timer_0>=17)
	     {  
	      f_timer_0=0;
		  TCCR0B&=~((1<<CS02)|(1<<CS00));
		  TIMSK0&=~(1<<TOIE0);
	     }
	    else
	     {
	      TIMSK0|=(1<<TOIE0);
	      f_timer_0++;
	     }
	  break;
    }
	  //-------------------------------------------------------------- Encoder button ---------------------------------//
   case ENCODER_BUTTON:
    {
		  pin_state=(Encoder_Button_P&(1<<Encoder_Button_Pin));
		  debounce(pin_state);
		  
		  if(f_timer_0==2)
		    {
				//PORTA&=~(1<<PA4); // sound on
				SOUND_ON;
			  if(debounce(pin_state)==0)
			  {
				switch(eWindow)  
				{ 
				  case SETUP:                    // Setup listing  START!!!!  1
				  {
				     
					  if(f_curent_pos==0) // MAIN 1
					  {	
					    eWindow=MAIN_SETING;        //  Setup listing -> Main listing  1
						//f_clear_lcd=1;
						eUpdate=UPDATE;
					  }
					  else if(f_curent_pos==1)// FAN 2
					  {
						  eWindow=FAN_SETUP;
						  //f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==2)// BUNKER 3
					  {
						  eWindow=BUNKER_SETUP;     // Setup listing -> Boiler setup    3
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==3)// LIGHTER 4
					  {
						  eWindow=LIGHTER_SETUP;     
						  //f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==4)// CLEAN 5
					  {
						  eWindow=CLEAN_MECHANISM_SETUP;    
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==5)// PUMP_HEATING 6
					  {
						  eWindow=PUMP_HEATING_SETUP;
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==6)// PUMP HOT WATER 7
					  {
						  eWindow=PUMP_HOT_WATER_SETUP;
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==7)// BOILER SETUP 8
					  {
						  eWindow=BOILER_SETUP;
						 
						  eUpdate=UPDATE;
					  }
					   else if (f_curent_pos==8)// FAN2 SETUP 9
					   {
						   eWindow=FAN_2_SETUP;
						   
						   eUpdate=UPDATE;
					   }
					   
				     break;
				  } // end case
				 
				  //--------------------------------------First page (MAIN) 1.0--------------------------------//
				  case MAIN_SETING:                  // Setup listing -> Main seting 
				  { 
					   if(f_curent_pos==0)
					   {
						   eWindow=FUEL_TYPE;        // Setup listing -> Main seting -> Fuel type  1.1
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;         
					   }
					   else if(f_curent_pos==1)
					   {
						   eWindow=BOIL_TEMPERATURE; // Setup listing -> Main seting -> Boil temperature 1.2
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==2)     // Setup listing -> Main seting -> Boil power 1.3
					   {
						   eWindow=BOILER_POWER;
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==3)
					   {
						   eWindow=SEASON;
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   
					   else if (f_curent_pos==4)
					   {
						   eWindow=M_HOT_WATER;
						   // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					  
					   else if (f_curent_pos==5)
					   {
						   eWindow=M_BURNER_TEMP;
						   // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==6)
					   {
						   eWindow=M_SELECT_CWU;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==7)// Default
					   {
						   eWindow=M_DEFAULT;
						   
						   eUpdate=UPDATE;
					   }
					   break;
					   
				  }// end case MAIN_SETING
				 //--------------------------------- flag_yes_no 
				  case FUEL_TYPE:
				  {
					  if(f_sel_list==1)
					  {
						  flag_yes_no|=(1<<FUEL_DEF);
					  }
					  else 
					   flag_yes_no&=~(1<<FUEL_DEF);
					 break;  
				  }
				   case SEASON:
				   {
					   if(f_sel_list==1)
					   {
						   flag_yes_no|=(1<<SESON_DEF);
					   }
					   else
					   flag_yes_no&=~(1<<SESON_DEF);
					   break;
				   }
				  case M_SELECT_CWU:
				  {
					  if(f_sel_list==1)
					  {
						  flag_yes_no|=(1<<SELECT_CWU_DEF);
					  }
					  else
					  flag_yes_no&=~(1<<SELECT_CWU_DEF);
					  break;
				  } 
				  case M_DEFAULT:
				  {
					  if(f_sel_list==1)
					  {
						  flag_yes_no|=(1<<DEFINE_DEF);
						  Defaut_Set();
						  flag_yes_no&=~(1<<DEFINE_DEF);
						  eWindow=MAIN;
					  }
					  else
					    {
					      flag_yes_no&=~(1<<DEFINE_DEF);
						  eWindow=MAIN_SETING;
						}
					  break;
				  }
				  //----------------------------------
				  //---------------------------------------FAN page (2.0)--------------------------------------//
				  
				  case FAN_SETUP:                  // Setup listing -> Main seting
				  {
					  if(f_curent_pos==0)
					  {
						  eWindow=MODULATION;        // Setup listing -> Main seting -> Fuel type  1.1
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if(f_curent_pos==1)
					  {
						  eWindow=HEATING; // 
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if(f_curent_pos==2)
					  {
						  eWindow=HEATING_FAN_ROTATION; 
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==3)    
					  {
						  eWindow=MAX_POWER;
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==4)
					  {
						  eWindow=MIN_POWER;
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==5)
					  {
						  eWindow=QUENCHING;
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==6)
					  {
						  eWindow=CLEANING;
						  //f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==7)
					  {
						  eWindow=BLOWING;
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==8)
					  {
						  eWindow=BLOWING_TIME;
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==9)
					  {
						  eWindow=BLOWING_BREAK_TIME;
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  else if (f_curent_pos==10)
					  {
						  eWindow=BLOWING_ROTATION;
						 // f_clear_lcd=1;
						  eUpdate=UPDATE;
					  }
					  break;
				  }// end case FAN_SETUP
				  case MODULATION: 
				  {
					  if(f_sel_list==1)
					  {
						  flag_yes_no|=(1<<MODULATION_DEF);
					  }
					  else
					  flag_yes_no&=~(1<<MODULATION_DEF);
					  break;
				  }
				  case BLOWING:
				  {
					 if(f_sel_list==1)
					 {
						 flag_yes_no|=(1<<BLOWING_DEF);
					 }
					 else
					 flag_yes_no&=~(1<<BLOWING_DEF);
					 break; 
				  }
				  
				  //--------------------- end bunker 2.0
				  //---------------------------------------------- BUNKER 3.0----------------------//
				   
				   case BUNKER_SETUP:                  // Setup listing -> Main seting
				   {
					   if(f_curent_pos==0)
					   {
						   eWindow=START_DOSE;        // Setup listing -> Main seting -> Fuel type  1.1
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if(f_curent_pos==1)
					   {
						   eWindow=SUPPLY_CYCLE; // Setup listing -> Main seting -> Boil temperature 1.2
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==2)     // Setup listing -> Main seting -> Boil power 1.3
					   {
						   eWindow=DOSE_MAX_POWER;
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==3)
					   {
						   eWindow=DOSE_MIN_POWER;
						   //f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==4)
					   {
						   eWindow=STOKER_TIME_WORK;
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==5)
					   {
						   eWindow=STOKER_TIME_DELAY;
						   //f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==6)
					   {
						   eWindow=STOKER_TIME_CONTINUATION;
						   //f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==7)
					   {
						   eWindow=STOKER_TIME_EMPTYING;
						   //f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==8)
					   {
						   eWindow=STOKER_FERVOR;
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==9)
					   {
						   eWindow=STOKER_MODE_WORK;
						   // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   break;
				   }// end case BUNKER_SETUP
				   //--------------------end bunker 3.0
				   //--------------------------------------Lighter 4.0--------------------//
				   
				   case LIGHTER_SETUP:                  // Setup listing -> LIGHTER_SETUP
				   {
					   if(f_curent_pos==0)
					   {
						   eWindow=HYSTERESIS; // 0      
						   //f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if(f_curent_pos==1)
					   {
						   eWindow=FLAME_DELAY; // 1
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==2)     
					   {
						   eWindow=KINDLING_TIME;// 2
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==3)
					   {
						   eWindow=ATTEMPT_KINDLING;// 3
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==4)
					   {
						   eWindow=FUEL_SHORTAGE;// 4
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==5)
					   {
						   eWindow=IGNITION_STAB;// 5
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==6)
					   {
						   eWindow=TIME_IGNITION_STAB;// 6
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==7)
					   {
						   eWindow=PELETS_FAN;// 7
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==8)
					   {
						   eWindow=WOODS_FAN;// 8
						  // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   else if (f_curent_pos==9)
					   {
						   eWindow=QUENCHING_TIME;// 9
						   // f_clear_lcd=1;
						   eUpdate=UPDATE;
					   }
					   
					   break;
				    } // end case LIGHTER_SETUP
					    //--------------------------------------Lighter 5.0--------------------//
					    
				     case WOODS_FAN:                  // Setup listing -> Cleaning mechanism_SETUP
					    {
						    if(f_curent_pos==0)
						    {
							    lighter_woods_fan=0;
							 //   f_clear_lcd=1;
							    eUpdate=UPDATE;
						    }
						    else if(f_curent_pos==1)
						    {
							    lighter_woods_fan=1;
							  //  f_clear_lcd=1;
							    eUpdate=UPDATE;
						    }
							else if(f_curent_pos==2)
							{
								lighter_woods_fan=2;
								//  f_clear_lcd=1;
								eUpdate=UPDATE;
							}
						  break;
						}// end case CLEAN_MECHANISM_SETUP
						
						case CLEAN_MECHANISM_SETUP:                  // Setup listing -> Cleaning mechanism_SETUP
						{
							if(f_curent_pos==0)
							{
								eWindow=MECHA_CLEAN_WORK; // 0
								//   f_clear_lcd=1;
								eUpdate=UPDATE;
							}
							else if(f_curent_pos==1)
							{
								eWindow=MECHA_CLEAN_DELAY; // 1
								//  f_clear_lcd=1;
								eUpdate=UPDATE;
							}
							break;
						}// end case CLEAN_MECHANISM_SETUP
						
						
						//--------------------------------------Lighter 6.0--------------------//
						case PUMP_HEATING_SETUP:            
						{
							if(f_curent_pos==0)
							{
								eWindow=PUMP_HEATING_MODE;
								eUpdate=UPDATE;
							}
							break;
						}// end case
					  //--------------------------------------Pump heating seting 7.0----------//	
					  case PUMP_HOT_WATER_SETUP:                 
					  {
						  if(f_curent_pos==0)
						  {
							  eWindow=PUMP_HOT_WATER_HYSTESIS; // 0
							//  f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  else if(f_curent_pos==1)
						  {
							  eWindow=PUMP_HOT_WATER_INCREASE; // 1
							//  f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  else if (f_curent_pos==2)
						  {
							  eWindow=PUMP_HOT_WATER_OVERSPENDING;// 2
							//  f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  else if (f_curent_pos==3)
						  {
							  eWindow=PUMP_HOT_WATER_TEMP_WORK;// 3
							//  f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  else if (f_curent_pos==4)
						  {
							  eWindow=PUMP_HOT_WATER_HYSTESIS_WORK;// 4
							//  f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  break;
					  } // end case Pump
					  
					  //--------------------------------------------Boiler 8.0-----------------//
					  case BOILER_SETUP:
					  {
						  if(f_curent_pos==0)
						  {
							  eWindow=BOILER_MIN_TEMP; // 0
							 // f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  else if(f_curent_pos==1)
						  {
							  eWindow=BOILER_MAX_TEMP; // 1
							 // f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  else if (f_curent_pos==2)
						  {
							  eWindow=BOILER_TOP_HYSTES;// 2
							 // f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  else if (f_curent_pos==3)
						  {
							  eWindow=BOILER_HYSTES_SWICH_BURNER;// 3
							//  f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  else if (f_curent_pos==4)
						  {
							  eWindow=BOILER_HYSTES_PROTECT;// 4
							 // f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  else if (f_curent_pos==5)
						  {
							  eWindow=BOILER_TEMP_OVERHEATING;// 5
							//  f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  else if (f_curent_pos==6)
						  {
							  eWindow=BOILER_MODUL_POWER_BURNER;// 6
							//  f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  else if (f_curent_pos==7)
						  {
							  eWindow=BOILER_TERMOSTAT_WORK;// 7
							 // f_clear_lcd=1;
							  eUpdate=UPDATE;
						  }
						  break;
					  } // end case Boiler	
					 //---------------------------------------FAN_2    9.0------------------------------------//
					 
					 case FAN_2_SETUP:                  
					 {
						 if(f_curent_pos==0)
						 {
							 eWindow=FAN2_YES_NO;
							 // f_clear_lcd=1;
							 eUpdate=UPDATE;
						 }
						 if(f_curent_pos==1)// 0
						 {
							 eWindow=FAN2_MODULATION;        
							 // f_clear_lcd=1;
							 eUpdate=UPDATE;
						 }
						 else if(f_curent_pos==2)
						 {
							 eWindow=FAN2_HEATING; //
							 // f_clear_lcd=1;
							 eUpdate=UPDATE;
						 }
						 else if(f_curent_pos==3)
						 {
							 eWindow=FAN2_HEATING_FAN_ROTATION;
							 // f_clear_lcd=1;
							 eUpdate=UPDATE;
						 }
						 else if (f_curent_pos==4)
						 {
							 eWindow=FAN2_MAX_POWER;
							 // f_clear_lcd=1;
							 eUpdate=UPDATE;
						 }
						 else if (f_curent_pos==5)
						 {
							 eWindow=FAN2_MIN_POWER;
							 // f_clear_lcd=1;
							 eUpdate=UPDATE;
						 }
						 else if (f_curent_pos==6)
						 {
							 eWindow=FAN2_QUENCHING;
							 // f_clear_lcd=1;
							 eUpdate=UPDATE;
						 }
						 else if (f_curent_pos==7)
						 {
							 eWindow=FAN2_CLEANING;
							 //f_clear_lcd=1;
							 eUpdate=UPDATE;
						 }
						 else if (f_curent_pos==8)
						 {
							 eWindow=FAN2_BLOWING;
							 // f_clear_lcd=1;
							 eUpdate=UPDATE;
						 }
						 else if (f_curent_pos==9)
						 {
							 eWindow=FAN2_BLOWING_TIME;
							 // f_clear_lcd=1;
							 eUpdate=UPDATE;
						 }
						 else if (f_curent_pos==10)
						 {
							 eWindow=FAN2_BLOWING_BREAK_TIME;
							 // f_clear_lcd=1;
							 eUpdate=UPDATE;
						 }
						 else if (f_curent_pos==11)
						 {
							 eWindow=FAN2_BLOWING_ROTATION;
							 // f_clear_lcd=1;
							 eUpdate=UPDATE;
						 }
						 break;
					 }// end case FAN_SETUP
					  
					 case FAN2_YES_NO:              // 9.0   Fan 2   YES_NO
					 {
						 if(f_sel_list==1)
						 {
							 flag_yes_no|=(1<<FAN2_YES_NO_DEF);
						 }
						 else
						 flag_yes_no&=~(1<<FAN2_YES_NO_DEF);
						 break;
					 }// end case
					 case FAN2_MODULATION:              // 9.0   Fan 2   
					 {
						 if(f_sel_list==1)
						 {
							 flag_yes_no|=(1<<FAN2_MODULATION_DEF);
						 }
						 else
						 flag_yes_no&=~(1<<FAN2_MODULATION_DEF);
						 break;
					 }// end case
					 case FAN2_BLOWING:              // 9.0   Fan 2   
					 {
						 if(f_sel_list==1)
						 {
							 flag_yes_no|=(1<<FAN2_BLOWING_DEF);
						 }
						 else
						 flag_yes_no&=~(1<<FAN2_BLOWING_DEF);
						 break;
					 }// end case
					 
					 
					 case BOILER_MODUL_POWER_BURNER:                  
					 {
						if(f_sel_list==1)
						{
							flag_yes_no|=(1<<MODUL_POWER_BURNER_DEF);
						}
						else
						flag_yes_no&=~(1<<MODUL_POWER_BURNER_DEF);
						break;
					 }// end case 
					 case BOILER_TERMOSTAT_WORK:                  
					 {
						 if(f_sel_list==1)
						 {
							 flag_yes_no|=(1<<TERMOSTAT_WORK_DEF);
						 }
						 else
						 flag_yes_no&=~(1<<TERMOSTAT_WORK_DEF);
						 break;
					 }// end case
					 case STOKER_MODE_WORK:
					 {
						 if(f_sel_list==1)
						 {
							 flag_yes_no_2|=(1<<STOKER_WORK_MODE_DEF);
						 }
						 else
						 flag_yes_no_2&=~(1<<STOKER_WORK_MODE_DEF);
						 break;
					 }// end case
					 case PELETS_FAN:              //5.15 // 
					 {
						 if(f_sel_list==1)
						 {
							 flag_yes_no|=(1<<PELETS_FAN_DEF);
						 }
						 else
						 flag_yes_no&=~(1<<PELETS_FAN_DEF);
						 break;
					 }// end case
					 case PUMP_HEATING_MODE:              //6. //
					 {
						 if(f_sel_list==1)
						 {
							 flag_yes_no|=(1<<PUMP_HEATING_DEF);
						 }
						 else
						 flag_yes_no&=~(1<<PUMP_HEATING_DEF);
						 break;
					 }// end case
					 
					 
						
				   }// end swich(window) 
				   
				}// end if(debaunce())
				
			 }//end if(t_time_0)
			 
		 // }// case encoder_button
		 
		 
		  if(f_timer_0==3)
		   SOUND_ON;
		      
		  if(f_timer_0>=17)
		  {
			  f_timer_0=0;
			  TCCR0B&=~((1<<CS02)|(1<<CS00));
			  TIMSK0&=~(1<<TOIE0);
		  }
		  else
		  {
			  TIMSK0|=(1<<TOIE0);
			  f_timer_0++;
		  }
		 break;  
	  }// end case encoder_button
	  
	  case ESC_BUTTON:
	  {
		 pin_state=(ESC_Button_P&(1<<ESC_Button_Pin));
		 debounce(pin_state);
		 
		 if(f_timer_0==2)
		 { 
		   if (eWindow!=MAIN)
		   {
			  eWindow=MAIN;   
		   }
		   SOUND_ON; 
		   eUpdate=UPDATE;
		 }
		
		  
		  if(f_timer_0>=17)
		  {
			  f_timer_0=0;
			  TCCR0B&=~((1<<CS02)|(1<<CS00));
			  TIMSK0&=~(1<<TOIE0);
		  }
		  else
		  {
			  TIMSK0|=(1<<TOIE0);
			  f_timer_0++;
		  }
		  
		 break;
	  }
	  
	  case START_BUTTON:
	  {
		  pin_state=(Start_Button_P&(1<<Start_Button_Pin));
		  debounce(pin_state);
		 
		  if(f_timer_0==2)
		  {
			  SOUND_ON;
		     if(!flag_alarm&&eSTART_STOP!=START)
			  {
			    eSTART_STOP=START;
				
				
				
				flag_yes_no_2|=(1<<FLAG_START_DOSE);
				timer_template_stoker_dose=0;
				timer_template_dose=0;
				timer_template_var=0;
				
			    eUpdate=UPDATE;
			    eWork_Mode=W_MONITORING;
				
			   
					kindli_time_template=0;  //reset kindli time 
					timer_template_dose=0; 
					flame_y_position=43;
				
			  }
		  }
		  
		  if(f_timer_0>=17)
		  {
			  f_timer_0=0;
			  TCCR0B&=~((1<<CS02)|(1<<CS00));
			  TIMSK0&=~(1<<TOIE0);
		  }
		  else
		  {
			  TIMSK0|=(1<<TOIE0);
			  f_timer_0++;
		  }
		  
	    break;
	  }
	  case STOP_BUTTON:
	  {
		  pin_state=(Stop_Button_P&(1<<Stop_Button_Pin));
		  debounce(pin_state);
		  //flag_fans_work=0;
		  EICRB&=~((1<<ISC61)|(1<<ISC60));
		  EIMSK&=~(1<<INT6); //Interrupt  INT6 ENABLE
		  
		  if(f_timer_0==2)
		  {
			  if (flag_alarm&(1<<FUEL_SHORTAGE_DEF)||flag_alarm&(1<<SHNEK_OWER_HEAT_ALARM)||flag_alarm&(1<<OWER_HEATING_ALARM))
			  {
				  flag_alarm&=~(1<<FUEL_SHORTAGE_DEF);
				  flag_alarm&=~(1<<SHNEK_OWER_HEAT_ALARM);
				  flag_alarm&=~(1<<OWER_HEATING_ALARM);
			  }
			  SOUND_ON;
			  
			  if (eWork_Mode==W_MONITORING)
			  {
				  
				  eSTART_STOP=STOP;
				  flag_yes_no_2|=(1<<FLAG_START_DOSE);
				  //eeprom_write_byte(&eememSTART_STOP,eSTART_STOP);
				  
				 
			  }
			  
			  eUpdate=UPDATE;
			  flame_y_position=43; // flame start  position
			  flag_fans_work=0;
			  BURNER_AUGER_OFF;
			  BUNKER_AUGER_OFF;
			  ELECTROTEN_OFF;
			  
			  timer_template_dose=0;
			  
			  if (eWork_Mode!=W_MONITORING&&eWork_Mode!=W_AFTERBURNING&&eWork_Mode!=W_ATTENUATION&&eWork_Mode!=W_CLEANING)
			  {
				  if (flag_alarm&(1<<BURNER_SENSOR_ALARM))
				  {
					  eWork_Mode=W_ATTENUATION;
				  }
				  else
				      eWork_Mode=W_AFTERBURNING;
			  }
			  else if(eWork_Mode==W_AFTERBURNING||eWork_Mode==W_ATTENUATION)
			      eWork_Mode=W_CLEANING;
			  else
			  {
			      eWork_Mode=W_MONITORING;
				  eSTART_STOP=STOP;
				  flag_yes_no_2|=(1<<FLAG_START_DOSE);
				  
			  }
			  
		  }
		  
		  if(f_timer_0>=17)
		  {
			  f_timer_0=0;
			  TCCR0B&=~((1<<CS02)|(1<<CS00));
			  TIMSK0&=~(1<<TOIE0);
		  }
		  else
		  {
			  TIMSK0|=(1<<TOIE0);
			  f_timer_0++;
		  }
	    break;
	  }
	}// end swich (eButton)
	
	
	sei();
	SREG=sreg;
}
	
ISR(TIMER2_OVF_vect)
	{
		unsigned char sreg;
		/* Save global interrupt flag */
		sreg = SREG;
		cli();
		
		 if(!(PINA&(1<<PA4))) //------sound------//
		   SOUND_OFF;
		   
		
		if(permission_read==TRUE)
		{
		  Read_Temperature();
		  permission_read=FALSE;
		}
		  
		if ((Encoder_Button_P&(1<<Encoder_Button_Pin))==0)
		{
			if(f_timer_0==0){
				TCCR0B|=(1<<CS02)|(1<<CS00);
				TIMSK0|=(1<<TOIE0);
				f_timer_0=1;
				eButton=ENCODER_BUTTON;
			}
		}
		else if((Menu_Button_P&(1<<Menu_Button_Pin))==0)
		{
			if(f_timer_0==0){
				TCCR0B|=(1<<CS02)|(1<<CS00);
				TIMSK0|=(1<<TOIE0);
				f_timer_0=1;
				eButton=MENU_BUTTON;
			}
		}
		else if((ESC_Button_P&(1<<ESC_Button_Pin))==0)
		{
			if(f_timer_0==0){
				TCCR0B|=(1<<CS02)|(1<<CS00);
				TIMSK0|=(1<<TOIE0);
				f_timer_0=1;
				eButton=ESC_BUTTON;
			}
		}
		else if((Start_Button_P&(1<<Start_Button_Pin))==0)
		{
			if(f_timer_0==0){
				TCCR0B|=(1<<CS02)|(1<<CS00);
				TIMSK0|=(1<<TOIE0);
				f_timer_0=1;
				eButton=START_BUTTON;
				eUpdate=UPDATE;
			}
		}
		else if((Stop_Button_P&(1<<Stop_Button_Pin))==0)
		{
			if(f_timer_0==0){
				TCCR0B|=(1<<CS02)|(1<<CS00);
				TIMSK0|=(1<<TOIE0);
				f_timer_0=1;
				eButton=STOP_BUTTON;
				eUpdate=UPDATE;
			}
		}
		
		
		NewState=read_gray_code_from_encoder();
		if(NewState!=OldState)
		{
			switch(OldState)
			{
				case 2:
				{
					if(NewState == 3) upState++;
					if(NewState == 0) downState++;
					break;
				}
				
				case 0:
				{					
					if(NewState == 2) upState++;
					if(NewState == 1) downState++;
					break;
				}
				case 1:
				{
					if(NewState == 0) upState++;
					if(NewState == 3) downState++;
					break;
				}
				
				case 3:
				{
					if(NewState == 1) upState++;
					if(NewState == 2) downState++;
					break;
				}
				
			}
			OldState=NewState;
			
			if (upState >= 4)
			{
			  upState=0;
			  eUpdate=UPDATE;
	
			  switch(eWindow)
			  {
				  case MAIN:
				  {	  
			        // Nead write for main encoder 
					
					break;
				  }
                  //----------------------------------------------- MMain seting 1.0--------------------------//
				  case BOIL_TEMPERATURE:
				  {
					  if (setup_boiler_temperature>=68)
					  {
						  setup_boiler_temperature=9;
					  }
					  ++setup_boiler_temperature;
					  
					  break;
				  }
				  case BOILER_POWER:
				  {
					  if (boil_power_percent>=100)
					  {
						  boil_power_percent=60;
					  }
					  ++boil_power_percent;
					  break;
				  }
				  case M_HOT_WATER://   30->60 celcium
				  {
					  if (m_hot_water>=60)
					  {
						  m_hot_water=29;
					  }
					  ++m_hot_water;
					  break;
				  }
				  case M_BURNER_TEMP://   200->500 celcium
				  {
					  if (m_burner_temp>=500)
					  {
						 m_burner_temp=199;
					  }
					  ++m_burner_temp;
					  break;
				  }
				  //--------------------------------------------------FAN setting 2.0-----------------------------//
				  case HEATING:// 1  1->100%
				  {
					  if (max_rotation_ignition>=100)
					  {
						  max_rotation_ignition=0;
					  }
					  ++max_rotation_ignition;
					  break;
				  }
				  case HEATING_FAN_ROTATION:// 2  1->100%
				  {
					  if (rotation_ignition>=100)
					  {
						  rotation_ignition=0;
					  }
					  ++rotation_ignition;
					  break;
				  }
				  case MAX_POWER:// 3  1->100%
				  {
					  if (rotation_max_power>=100)
					  {
						  rotation_max_power=0;
					  }
					  ++rotation_max_power;
					  break;
				  }
				  case MIN_POWER:// 4  1->100%
				  {
					  if (rotation_min_power>=100)
					  {
						  rotation_min_power=0;
					  }
					  ++rotation_min_power;
					  break;
				  }
				  case QUENCHING:// 5  1->100%
				  {
					  if (rotation_quenching>=100)
					  {
						  rotation_quenching=0;
					  }
					  ++rotation_quenching;
					  break;
				  }
				  case CLEANING:// 6  1->100%
				  {
					  if (rotation_cleaning>=100)
					  {
						  rotation_cleaning=0;
					  }
					  ++rotation_cleaning;
					  break;
				  }
				  case BLOWING_TIME:// 8  5->60 sec 
				   {
					   if (time_blowing>=60)
					   {
						   time_blowing=4;
					   }
					   ++time_blowing;
					   break;
				   }
				   case BLOWING_BREAK_TIME:// 9  1->99 min
				   {
					   if (delay_time>=99)
					   {
						   delay_time=0;
					   }
					   ++delay_time;
					   break;
				   }
				   case BLOWING_ROTATION:// 10  1->100 %
				   {
					   if (rotation_blowing>=100)
					   {
						   rotation_blowing=0;
					   }
					   ++rotation_blowing;
					   break;
				   }
				   //--------------------------------------------------------- BUNKER setting 3.0 ----------------------------------//
				    case START_DOSE: //0    0 - >250 sec
				    {
						
					    if (start_dose>=250)
					    {
						    start_dose=0;
					    }
					    ++start_dose;
					    break;
				    }
				    case SUPPLY_CYCLE://1  1-> 250 sec
				    {
					    if (supply_cycle>=250)
					    {
						    supply_cycle=0;
					    }
					    ++supply_cycle;
					    break;
				    }
				    case DOSE_MAX_POWER://2 1->100 %
				    {
					    if (dose_max_power>=100)
					    {
						    dose_max_power=0;
					    }
					    ++dose_max_power;
					    break;
				    }
				    case DOSE_MIN_POWER://3  1->100 %
				    {
					    if (dose_min_power>=100)
					    {
						    dose_min_power=0;
					    }
					    ++dose_min_power;
					    break;
				    }
				    case STOKER_TIME_WORK://4  1->99 sec
				    {
					    if (stoker_time_work>=100)
					    {
						    stoker_time_work=0;
					    }
					    ++stoker_time_work;
					    break;
				    }
				    case STOKER_TIME_DELAY://5  1->99 sec
				    {
					    if (stoker_time_delay>=100)
					    {
						    stoker_time_delay=0;
					    }
					    ++stoker_time_delay;
					    break;
				    }
				    case STOKER_TIME_CONTINUATION://6 1->99 sec
				    {
					    if (stoker_time_continuation>=100)
					    {
						    stoker_time_continuation=0;
					    }
					    ++stoker_time_continuation;
					    break;
				    }
				    case STOKER_TIME_EMPTYING://7  1->99 sec
				    {
					    if (stoker_time_emptying>=100)
					    {
						    stoker_time_emptying=0;
					    }
					    ++stoker_time_emptying;
					    break;
				    }
					case STOKER_FERVOR://8   20->99 Celsium
					{
						if (stoker_fervor>=99)
						{
							stoker_fervor=19;
						}
						++stoker_fervor;
						break;
					}
					
					 //--------------------------------------------------------- LIGHTER setting 4.0 ----------------------------------//
					 case HYSTERESIS: //0
					 {
						 if (lighter_hysteresis>=250) // 1-> 250 Celcium
						 {
							 lighter_hysteresis=0;
						 }
						 ++lighter_hysteresis;
						 break;
					 }
					 case FLAME_DELAY: //1
					 {
						 if (lighter_flame_delay>=250)//1-> 250 secund
						 {
							 lighter_flame_delay=0;
						 }
						 ++lighter_flame_delay;
						 break;
					 }
					 case KINDLING_TIME://2
					 {
						 if (lighter_kindling_time>=15)//1->15 minut
						 {
							 lighter_kindling_time=0;
						 }
						 ++lighter_kindling_time;
						 break;
					 }
					 case ATTEMPT_KINDLING://3
					 {
						 if (lighter_attempt_kindling>=10)//1->10 count
						 {
							 lighter_attempt_kindling=0;
						 }
						 ++lighter_attempt_kindling;
						 break;
					 }
					 case FUEL_SHORTAGE: // 4
					 {
						 if (lighter_fuel_shortage>=99)//1->99 minut
						 {
							 lighter_fuel_shortage=0;
						 }
						 ++lighter_fuel_shortage;
						 break;
					 }
					 
					 case TIME_IGNITION_STAB:// 5   1->99
					 {
						 if (lighter_time_ignition_stab>=99)
						 {
							lighter_time_ignition_stab=0;
						 }
						 ++lighter_time_ignition_stab;
						 break;
					 }
					 
					 case QUENCHING_TIME:
					 {
						 if (lighter_quenching_time>=30)
						 {
							 lighter_quenching_time=0;
						 }
						 ++lighter_quenching_time;
						 break;
					 }
					 
				 //---------------------------------------- Cleaning mechanism 5.x-------//
				  case MECHA_CLEAN_WORK: // 0  1->900 sec
				  {
					  if (mechanism_cleaning_work>=200)
					  {
						  mechanism_cleaning_work=0;
					  }
					  ++mechanism_cleaning_work;
					  break;
				  }
				  
				  case MECHA_CLEAN_DELAY:// 1   1->200 sec
				  {
					  if (mechanism_cleaning_delay>=900)
					  {
						  mechanism_cleaning_delay=0;
					  }
					  ++mechanism_cleaning_delay;
					  break;
				  }
				  //-------------------------------------------------------------- Cleaning mechanism 7.x-------//
				  case PUMP_HOT_WATER_HYSTESIS: // 0  1->20 celsium
				  {
					  if (pump_hot_water_hystesis>=20)
					  {
						  pump_hot_water_hystesis=0;
					  }
					  ++pump_hot_water_hystesis;
					  break;
				  }
				  case PUMP_HOT_WATER_INCREASE: // 1  2->20 celsium
				  {
					  if (pump_hot_water_increase>=20)
					  {
						  pump_hot_water_increase=1;
					  }
					  ++pump_hot_water_increase;
					  break;
				  }
				  case PUMP_HOT_WATER_OVERSPENDING: // 2  1->10 min
				  {
					  if (pump_hot_water_overspending>=10)
					  {
						  pump_hot_water_overspending=0;
					  }
					  ++pump_hot_water_overspending;
					  break;
				  }
				  case PUMP_HOT_WATER_TEMP_WORK: // 3  30->60 celsium
				  {
					  if (pump_hot_water_temp_work>=60)
					  {
						  pump_hot_water_temp_work=29;
					  }
					  ++pump_hot_water_temp_work;
					  break;
				  }
				  case PUMP_HOT_WATER_HYSTESIS_WORK: // 4  1->9 celsium
				  {
					  if (pump_hot_water_hystesis_work>=9)
					  {
						  pump_hot_water_hystesis_work=0;
					  }
					  ++pump_hot_water_hystesis_work;
					  break;
				  }	 
				  //case FAN_SETUP
			    //--------------------------------------------------------------- Kotel seting 8.x--------//
				 case BOILER_MIN_TEMP: // 0  30->69 celsium
				 {
					 if (kotel_min_temp>=69)
					 {
						 kotel_min_temp=29;
					 }
					 ++kotel_min_temp;
					 break;
				 }
				 case BOILER_MAX_TEMP: // 1  70->90 celsium
				 {
					 if (kotel_max_temp>=90)
					 {
						 kotel_max_temp=69;
					 }
					 ++kotel_max_temp;
					 break;
				 }
				 case BOILER_TOP_HYSTES: // 2  1->20 celsium
				 {
					 if (kotel_top_hystes>=20)
					 {
						kotel_top_hystes=0;
					 }
					 ++kotel_top_hystes;
					 break;
				 }
				 case BOILER_HYSTES_SWICH_BURNER: // 3  1->9 celsium
				 {
					 if (kotel_hyste_swich_burner>=9)
					 {
						 kotel_hyste_swich_burner=0;
					 }
					 ++kotel_hyste_swich_burner;
					 break;
				 }
				 case BOILER_HYSTES_PROTECT: // 4  1->5 celsium
				 {
					 if (kotel_protect>=5)
					 {
						 kotel_protect=0;
					 }
					 ++kotel_protect;
					 break;
				 }
				 case BOILER_TEMP_OVERHEATING: // 5  90->99 celsium
				 {
					 if (kotel_temp_overheating>=99)
					 {
						 kotel_temp_overheating=89;
					 }
					 ++kotel_temp_overheating;
					 break;
				 }
				 //--------------------------------------------------FAN2 setting 9.x-----------------------------//
				  case FAN2_HEATING:// 1  1->100%
				  {
					  if (fan2_max_rotation_ignition>=100)
					  {
						  fan2_max_rotation_ignition=0;
					  }
					  ++fan2_max_rotation_ignition;
					  break;
				  }
				  case FAN2_HEATING_FAN_ROTATION:// 2  1->100%
				  {
					  if (fan2_rotation_ignition>=100)
					  {
						  fan2_rotation_ignition=0;
					  }
					  ++fan2_rotation_ignition;
					  break;
				  }
				  case FAN2_MAX_POWER:// 3  1->100%
				  {
					  if (fan2_rotation_max_power>=100)
					  {
						  fan2_rotation_max_power=0;
					  }
					  ++fan2_rotation_max_power;
					  break;
				  }
				  case FAN2_MIN_POWER:// 4  1->100%
				  {
					  if (fan2_rotation_min_power>=100)
					  {
						  fan2_rotation_min_power=0;
					  }
					  ++fan2_rotation_min_power;
					  break;
				  }
				  case FAN2_QUENCHING:// 5  1->100%
				  {
					  if (fan2_rotation_quenching>=100)
					  {
						  fan2_rotation_quenching=0;
					  }
					  ++fan2_rotation_quenching;
					  break;
				  }
				  case FAN2_CLEANING:// 6  1->100%
				  {
					  if (fan2_rotation_cleaning>=100)
					  {
						  fan2_rotation_cleaning=0;
					  }
					  ++fan2_rotation_cleaning;
					  break;
				  }
				  case FAN2_BLOWING_TIME:// 8  5->60 sec 
				   {
					   if (fan2_time_blowing>=60)
					   {
						   fan2_time_blowing=4;
					   }
					   ++fan2_time_blowing;
					   break;
				   }
				   case FAN2_BLOWING_BREAK_TIME:// 9  1->99 min
				   {
					   if (fan2_delay_time>=99)
					   {
						   fan2_delay_time=0;
					   }
					   ++fan2_delay_time;
					   break;
				   }
				   case FAN2_BLOWING_ROTATION:// 10  1->100 %
				   {
					   if (fan2_rotation_blowing>=100)
					   {
						   fan2_rotation_blowing=0;
					   }
					   ++fan2_rotation_blowing;
					   break;
				   }
				 
				 
				 
				  default:
				  {
					  eDirection=DOWN;
					  break;
				  }
				 
			  }	
    	  					
			}
			if (downState >=4)
			{
				eUpdate=UPDATE;
				downState=0;
				
				switch(eWindow)//(encoder_flag)
				{
					case MAIN:
					{
						// Nead write for encoder action
						break;
					}
					
					case BOIL_TEMPERATURE:
					{
						if (setup_boiler_temperature<=10)
						{
							setup_boiler_temperature=69;
						}
						--setup_boiler_temperature;
						break;
					}
					
					case BOILER_POWER:
					{
						if (boil_power_percent<=60)
						{
							boil_power_percent=101;
						}
						--boil_power_percent;
						break;
					}
					case M_HOT_WATER://   30->60 celcium
					{
						if (m_hot_water<=30)
						{
							m_hot_water=61;
						}
						--m_hot_water;
						break;
					}
					case M_BURNER_TEMP://   200->500 celcium
					{
						if (m_burner_temp<=200)
						{
							m_burner_temp=501;
						}
						--m_burner_temp;
						break;
					}
					//-------------------------------------------------FAN 2.0---------------------------//
					case HEATING://1
					{
						if (max_rotation_ignition<=1)
						{
							max_rotation_ignition=101;
						}
						--max_rotation_ignition;
						break;
					}
					case HEATING_FAN_ROTATION://2
					{
						if (rotation_ignition<=1)
						{
							rotation_ignition=101;
						}
						--rotation_ignition;
						break;
					}
					
					case MAX_POWER://3
					{
						if (rotation_max_power<=1)
						{
							rotation_max_power=101;
						}
						--rotation_max_power;
						break;
					}
					case MIN_POWER://4
					{
						if (rotation_min_power<=1)
						{
							rotation_min_power=101;
						}
						--rotation_min_power;
						break;
					}
					case QUENCHING://5
					{
						if (rotation_quenching<=1)
						{
							rotation_quenching=101;
						}
						--rotation_quenching;
						break;
					}
					case CLEANING://6
					{
						if (rotation_cleaning<=1)
						{
							rotation_cleaning=101;
						}
						--rotation_cleaning;
						break;
					}
					case BLOWING_TIME://8   5->60 sec
					{
						if (time_blowing<=5)
						{
							time_blowing=61;
						}
						--time_blowing;
						break;
					}
					case BLOWING_BREAK_TIME://9  1->99 min
					{
						if (delay_time<=1)
						{
							delay_time=100;
						}
						--delay_time;
						break;
					}
					case BLOWING_ROTATION://10  1->100 %
					{
						if (rotation_blowing<=1)
						{
							rotation_blowing=101;
						}
						--rotation_blowing;
						break;
					}
					//--------------------------------------------- Bunker  3.0 -------------------------------//
					
					 case START_DOSE: // 0  0->250 sec
					 {
						 if (start_dose<=1)
						 {
							 start_dose=251;
						 }
						 --start_dose;
						 break;
					 }
					 case SUPPLY_CYCLE: // 1  0->250 sec
					 {
						 if (supply_cycle<=1)
						 {
							 supply_cycle=251;
						 }
						 --supply_cycle;
						 break;
					 }
					 case DOSE_MAX_POWER:// 2  1->100 %
					 {
						 if (dose_max_power<=1)
						 {
							 dose_max_power=101;
						 }
						 --dose_max_power;
						 break;
					 }
					 case DOSE_MIN_POWER:// 3  1->100%
					 {
						 if (dose_min_power<=1)
						 {
							 dose_min_power=101;
						 }
						 --dose_min_power;
						 break;
					 }
					 case STOKER_TIME_WORK:// 4  1->99 sec
					 {
						 if (stoker_time_work<=1)
						 {
							 stoker_time_work=100;
						 }
						 --stoker_time_work;
						 break;
					 }
					 case STOKER_TIME_DELAY:// 5 1->99 sec
					 {
						 if (stoker_time_delay<=1)
						 {
							 stoker_time_delay=100;
						 }
						 --stoker_time_delay;
						 break;
					 }
					 case STOKER_TIME_CONTINUATION:// 6  1->99 sec
					 {
						 if (stoker_time_continuation<=1)
						 {
							 stoker_time_continuation=100;
						 }
						 --stoker_time_continuation;
						 break;
					 }
					 case STOKER_TIME_EMPTYING:// 7   1->99 sec
					 {
						 if (stoker_time_emptying<=1)
						 {
							 stoker_time_emptying=101;
						 }
						 --stoker_time_emptying;
						 break;
					 }
					 case STOKER_FERVOR:// 8  20->99 celsium
					 {
						 if (stoker_fervor<=1)
						 {
							 stoker_fervor=100;
						 }
						 --stoker_fervor;
						 break;
					 }
					//--------------------------------------------------------- LIGHTER setting 4.0 ----------------------------------//
					case HYSTERESIS: //0
					{
						if (lighter_hysteresis<=1) // 0-> 250 Celcium
						{
							lighter_hysteresis=251;
						}
						--lighter_hysteresis;
						break;
					}
					case FLAME_DELAY: //1
					{
						if (lighter_flame_delay<=1)//0-> 250 secund
						{
							lighter_flame_delay=251;
						}
						--lighter_flame_delay;
						break;
					}
					case KINDLING_TIME://2
					{
						if (lighter_kindling_time<=1)//1->15 minut
						{
							lighter_kindling_time=16;
						}
						--lighter_kindling_time;
						break;
					}
					case ATTEMPT_KINDLING://3
					{
						if (lighter_attempt_kindling<=1)//1->10 count
						{
							lighter_attempt_kindling=11;
						}
						--lighter_attempt_kindling;
						break;
					}
					case FUEL_SHORTAGE://4
					{
						if (lighter_fuel_shortage<=1)//1->99 minut
						{
							lighter_fuel_shortage=100;
						}
						--lighter_fuel_shortage;
						break;
					}
					
					case TIME_IGNITION_STAB://6
					{
						if (lighter_time_ignition_stab<=1)
						{
							lighter_time_ignition_stab=100;
						}
						--lighter_time_ignition_stab;
						break;
					}
					
					case QUENCHING_TIME://6
					{
						if (lighter_quenching_time<=1)
						{
							lighter_quenching_time=31;
						}
						--lighter_quenching_time;
						break;
					}
					
					
					//---------------------------------------- Cleaning mechanism 5.x-------//
					case MECHA_CLEAN_WORK: // 0  1->900 sec
					{
						if (mechanism_cleaning_work<=1)
						{
							mechanism_cleaning_work=201;
						}
						--mechanism_cleaning_work;
						break;
					}
					
					case MECHA_CLEAN_DELAY:// 1   1->200 sec
					{
						if (mechanism_cleaning_delay<=1)
						{
							mechanism_cleaning_delay=901;
						}
						--mechanism_cleaning_delay;
						break;
					}
					//---------------------------------------- Cleaning mechanism 7.x-------//
					case PUMP_HOT_WATER_HYSTESIS: // 0  1->20 celsium
					{
						if (pump_hot_water_hystesis<=1)
						{
							pump_hot_water_hystesis=21;
						}
						--pump_hot_water_hystesis;
						break;
					}
					case PUMP_HOT_WATER_INCREASE: // 1  2->20 celsium
					{
						if (pump_hot_water_increase<=1)
						{
							pump_hot_water_increase=21;
						}
						--pump_hot_water_increase;
						break;
					}
					case PUMP_HOT_WATER_OVERSPENDING: // 2  1->10 min
					{
						if (pump_hot_water_overspending<=1)
						{
							pump_hot_water_overspending=11;
						}
						--pump_hot_water_overspending;
						break;
					}
					case PUMP_HOT_WATER_TEMP_WORK: // 3  30->60 celsium
					{
						if (pump_hot_water_temp_work<=30)
						{
							pump_hot_water_temp_work=61;
						}
						--pump_hot_water_temp_work;
						break;
					}
					case PUMP_HOT_WATER_HYSTESIS_WORK: // 4  1->9 celsium
					{
						if (pump_hot_water_hystesis_work<=1)
						{
							pump_hot_water_hystesis_work=10;
						}
						--pump_hot_water_hystesis_work;
						break;
					}
					//---------------------------------------- Kotel seting 8.x--------//
					case BOILER_MIN_TEMP: // 0  30->69 celsium
					{
						if (kotel_min_temp<=30)
						{
							kotel_min_temp=70;
						}
						--kotel_min_temp;
						break;
					}
					case BOILER_MAX_TEMP: // 1  70->90 celsium
					{
						if (kotel_max_temp<=70)
						{
							kotel_max_temp=91;
						}
						--kotel_max_temp;
						break;
					}
					case BOILER_TOP_HYSTES: // 2  1->20 celsium
					{
						if (kotel_top_hystes<=1)
						{
							kotel_top_hystes=21;
						}
						--kotel_top_hystes;
						break;
					}
					case BOILER_HYSTES_SWICH_BURNER: // 3  1->9 celsium
					{
						if (kotel_hyste_swich_burner<=1)
						{
							kotel_hyste_swich_burner=10;
						}
						--kotel_hyste_swich_burner;
						break;
					}
					case BOILER_HYSTES_PROTECT: // 4  1->5 celsium
					{
						if (kotel_protect<=1)
						{
							kotel_protect=6;
						}
						--kotel_protect;
						break;
					}
					case BOILER_TEMP_OVERHEATING: // 5  90->99 celsium
					{
						if (kotel_temp_overheating<=90)
						{
							kotel_temp_overheating=100;
						}
						--kotel_temp_overheating;
						break;
					}
					//-------------------------------------------------FAN_2 9.0---------------------------//
					case FAN2_HEATING://1
					{
						if (fan2_max_rotation_ignition<=1)
						{
							fan2_max_rotation_ignition=101;
						}
						--fan2_max_rotation_ignition;
						break;
					}
					case FAN2_HEATING_FAN_ROTATION://2
					{
						if (fan2_rotation_ignition<=1)
						{
							fan2_rotation_ignition=101;
						}
						--fan2_rotation_ignition;
						break;
					}
					
					case FAN2_MAX_POWER://3
					{
						if (fan2_rotation_max_power<=1)
						{
							fan2_rotation_max_power=101;
						}
						--fan2_rotation_max_power;
						break;
					}
					case FAN2_MIN_POWER://4
					{
						if (fan2_rotation_min_power<=1)
						{
							fan2_rotation_min_power=101;
						}
						--fan2_rotation_min_power;
						break;
					}
					case FAN2_QUENCHING://5
					{
						if (fan2_rotation_quenching<=1)
						{
							fan2_rotation_quenching=101;
						}
						--fan2_rotation_quenching;
						break;
					}
					case FAN2_CLEANING://6
					{
						if (fan2_rotation_cleaning<=1)
						{
							fan2_rotation_cleaning=101;
						}
						--fan2_rotation_cleaning;
						break;
					}
					case FAN2_BLOWING_TIME://8   5->60 sec
					{
						if (fan2_time_blowing<=5)
						{
							fan2_time_blowing=61;
						}
						--fan2_time_blowing;
						break;
					}
					case FAN2_BLOWING_BREAK_TIME://9  1->99 min
					{
						if (fan2_delay_time<=1)
						{
							fan2_delay_time=100;
						}
						--fan2_delay_time;
						break;
					}
					case FAN2_BLOWING_ROTATION://10  1->100 %
					{
						if (fan2_rotation_blowing<=1)
						{
							fan2_rotation_blowing=101;
						}
						--fan2_rotation_blowing;
						break;
					}
					
					
					default:
					 {
					    eDirection=UP;
						break;
					 }
				   }
				}
		}
		TCNT2=0x00;
		
		
		sei();
		SREG=sreg;
}

