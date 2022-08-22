/* 
 * File:   Lab4.c
 * Author: Wenyuan Zhu
 * ID: 1646652
 * Created on November 25, 2021, 4:04 PM
 */


# define F_CPU 1000000UL
#include<stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "defines.h"
#include <ctype.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lcd.h"
#include "hd44780.h"


//declare global variables
volatile float Internal;
volatile float External;         
volatile int Desired_T=25;
volatile int Internal_i;
volatile int Internal_f;
volatile int External_i;
volatile int External_f;


//define functions for turning on the LED
void LED_on(void){
    PORTB|=(1<<PORTB0);
}
//define functions for turning off the LED
void LED_off(void){
    PORTB &= ~(1<< PORTB0);
}
//initialization of the three buttons
void Switch_init(void){ 
    DDRC &= ~(1<<DDC2);      //set PC 3 to 5 to input          
    DDRC &= ~(1<<DDC4);
    DDRC &= ~(1<<DDC5);
    PORTC |= (1<<PORTC3);       //turn on pull ups 
    PORTC |= (1<<PORTC4);
    PORTC |= (1<<PORTC5);
}

void Internal_init(void){
    ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<MUX3);       //Temperature sensor and AVCC with external Cap at AREF
    ADCSRA |= (1<<ADEN);
}

uint16_t Internal_read(uint8_t channel){
    //channel 8
    ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<MUX3)|(channel & 0x07);
    
  
    ADCSRA |= (1<<ADSC);
    
    while (!(ADCSRA & (1<<ADIF)));
    ADCSRA |= (1<<ADIF); 
    _delay_ms(260); 
    
    return(ADC);
}

//toggle mode function
int mode_select(int mode){
    if((!(PINC&(1<<PINC5))&&(mode==0))){
        mode = 1;
        while(!(PINC&(1<<PINC5))){
            }
    }
    if((!(PINC&(1<<PINC5))&&(mode==1))){
        mode = 0;
        while(!(PINC&(1<<PINC5))){
            }

    }
    return(mode);
}
void External_init(void){
    DDRC &= ~(1<<DDC0);      //Set PC0 as Input mode
    ADCSRA |= (1<<ADEN);
    ADMUX |= (0<<REFS1)|(1<<REFS0);
  
}
uint16_t External_read(uint8_t channel){
    //channel 0
    ADMUX |= (0<<REFS1)|(1<<REFS0)| (channel & 0x07);
    ADCSRA |= (1<<ADSC); 
    while (!(ADCSRA & (1<<ADIF)));
    ADCSRA |= (1<<ADIF); 
    _delay_ms(260); 
    return(ADC);

}
void Temperature_range (void){
    
        if(!(PINC&(1<<PINC4))){     //if the second button is pressed increase the desired Temperature
            Desired_T++;
            if(Desired_T>=35){
                Desired_T=35;         
            }

        }      
        if(!(PINC&(1<<PINC3))){     //if the first button is pressed decrease the desired Temperature
            Desired_T--;
            if(Desired_T<=10){
                Desired_T=10;         
            }

        }
}
FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
int main(void) {  
    //declare PB0 is output for LED
    DDRB|=(1<<DDB0);     //output mode of LED
    int mode = 0; //default mode is external mode
    stdout=&lcd_str;
    //initialize lcd and button
    lcd_init();
    Switch_init();
    
    while(1){
        //internal temperature sensor initialize
        //print the internal temperature
        Internal_init();
        Internal = Internal_read(8)*0.06; 
        int Internal_i = (int) Internal;
        int Internal_f = (int) (Internal-Internal_i)*10;
        fprintf(stdout, "%d.%d", Internal_i,Internal_f);
        
        //print print degree symbol
        hd44780_outdata(0xDF);
        hd44780_outdata(0x43);
        
        //determine which temperature should be the compared temperature
        //according to the mode, determine the arrow direction
        mode=mode_select(mode);
        if(mode==0){
            hd44780_outdata(0x3C);
            hd44780_outdata(0x2D);
        }
        else{
            hd44780_outdata(0x2D);
            hd44780_outdata(0x3E);
        }
        
        //initialize the external temperature sensor
        //convert to the correct temperature according to the voltage 
        //print the external temperature
        External_init();
        External = External_read(0)*0.488-150.3;            //SS=5V/2^10=4.88mV     4.88/10=0.5
        int External_i = (int) External;
        int External_f = (int) (External-External_i)*10;
        fprintf(stdout, "%d.%d ", External_i,External_f);

        //print the degree symbol
        hd44780_outdata(0xDF);

        fprintf(stdout, "C\x1b\xc0%d",Desired_T);
        
        hd44780_outdata(0xDF);
        hd44780_outdata(0x43);
        
        //determine whether the furnace is on or off
        //check for the external temperature
        if(mode==0){
            //without the deadband but higher than the desired temperature
            if(Internal>(Desired_T+0.5)){
              fprintf(stdout,"  off"); 
              LED_off();
            }
            if(Internal<(Desired_T-0.5)){
             //lower than the desired temperature, the furnace should be on
                fprintf(stdout,"   on");
                LED_on();
            }
        }
        //check for the internal temperature
        else{
            if(External>(Desired_T+0.5)){
              fprintf(stdout,"  off"); 
              LED_off();
            }
            if(External<(Desired_T-0.5)){
                fprintf(stdout,"   on");
                LED_on();
            }
        }
        
        hd44780_outcmd(128);//make the cursor back to the beginning
        
        //set the temperature range, from 10 to 35
        Temperature_range();
    }
    
    
    return (0);
}





