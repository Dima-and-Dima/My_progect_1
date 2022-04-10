#ifndef Encoder_driver_H_INCLUDED
#define Encoder_driver_H_INCLUDED
# define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "main.h"

#define ESC_Button_Pin                    2
#define Start_Button_Pin                  4
#define Stop_Button_Pin                   5
#define Menu_Button_Pin                   3
#define Encoder_Button_Pin                3             
#define Encoder_PIN_1                     4
#define Encoder_PIN_2                     5
#define ESC_Button_P                      PINE
#define Start_Button_P                    PINB
#define Stop_Button_P                     PINB
#define Menu_Button_P                     PINE
#define Encoder_Button_P                  PINA
#define Encoder_P_1                       PINE
#define Encoder_P_2                       PINE
#define ESC_Button_DDR                    DDRE &=~ (1 << ESC_Button_Pin)
#define Start_Button_DDR                  DDRB &=~ (1 << Start_Button_Pin)
#define Stop_Button_DDR                   DDRB &=~ (1 << Stop_Button_Pin)
#define Menu_Button_DDR                   DDRE &=~ (1 << Menu_Button_Pin)
#define Encoder_Button_DDR                DDRA &=~ (1 << Encoder_Button_Pin)
#define Encoder_DDR_1	                  DDRE &=~ (1 << Encoder_PIN_1)
#define Encoder_DDR_2                     DDRE &=~ (1 << Encoder_PIN_2)
#define ESC_Button_PORT                   PORTE |= (1 << ESC_Button_Pin)
#define Start_Button_PORT                 PORTB |= (1 << Start_Button_Pin)
#define Stop_Button_PORT                  PORTB |= (1 << Stop_Button_Pin)
#define Menu_Button_PORT                  PORTE |= (1 << Menu_Button_Pin)
#define Encoder_Button_PORT               PORTA |= (1 << Encoder_Button_Pin)
#define Encoder_PORT_1                    PORTE |= (1 << Encoder_PIN_1)
#define Encoder_PORT_2                    PORTE |= (1 << Encoder_PIN_2)    // pull-up enabled   */

//static uint8_t val=0, val_tmp =0;
//static uint8_t i=1;
//static int8_t ch=0xFF;

void Encoder_Init(void);
void Debounce(unsigned long int new_sample);

uint8_t read_gray_code_from_encoder(void );
int NewState,OldState,Vol,upState,downState;


#endif