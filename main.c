#pragma config FOSC = HS      // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#include "LCD_Lib.c"
#define _XTAL_FREQ  4000000
    //Khai bao bien toan cuc
unsigned char hour=00,minute=00,second=00; // unsigned là bien nguyen duong
unsigned int giatri;  
////khai bao ham
void __interrupt() ngatB(void);
void delay_1ms();
void delay_ms(int x);
unsigned int ADC_Read(unsigned char channel);

int main() {
    //thiet lap ngat
   TRISD = 0;
    RD3 = 1;
    RD2 = 1;
    TRISB4 = 1;
    TRISB5 = 1;
    TRISB6 = 1;
    RBIF=0;
    GIE = 1;
    RBIE =1;
    PEIE =1;
    //thiet lap timer0
    T0CS = 0; // ch?n timer 0
    PSA = 0; // gan bo chia truoc timer / 1 là cho WDT
    PS0 = 1;
    PS1 = 0; // 100 chia 4
    PS2 = 0;
    TMR0 = 0;
// thiet lap ADC
    TRISA0 = 1;
    ADCON0 = 1;
    ADCON1 = 0xf0;
    ADRESH = 0;
    ADRESL = 0;
    
    LCD_Begin(); // start LCD 
    while(1){
        LCD_Goto(1,1);
        LCD_Print("Temperature:");
        LCD_Goto(1,2);
        LCD_Print("Clock:");
        for(;hour<24;hour++){
            LCD_Goto(7,2);
            LCD_WriteNum(hour);
            LCD_Goto(9,2);
            LCD_PutC(':');
            for(;minute<60;minute++){
                LCD_Goto(10,2);
                LCD_WriteNum(minute);
                LCD_Goto(12,2);
                LCD_PutC(':');
                for(;second<60;second++){
                    LCD_Goto(13,2);
                    LCD_WriteNum(second);
                    delay_ms(800);
                    //quet nhiet
                    giatri = ADC_Read(0); // Read analog voltage and convert it to degree Celsius (0.489 = 500/1023)
                    LCD_Goto(13,1);
                    LCD_WriteNum(giatri/2.046);//cam bien nhiet do LM35
                    delay_ms(200);    //100 ms delay  
                }
                second=0;
            }
            minute=0;
        }
        hour=0;
    }
    return 0;           
}
void __interrupt() ngatB(void){
    while(RB4==0){
        delay_ms(200);
        hour++;
        if(hour >= 24)
            hour = 0;
        LCD_Goto(7,2);
        LCD_WriteNum(hour);
        LCD_Goto(9,2);
        LCD_PutC(':');
    }
    while(RB5==0){
        delay_ms(200);
        minute++;
        if(minute >= 60){
            minute = 0;
        }
        LCD_Goto(10,2);
        LCD_WriteNum(minute);
        LCD_Goto(12,2);
        LCD_PutC(':');
    }
    if(RB6 == 0){
        second = 0;
        LCD_Goto(13,2);
        LCD_WriteNum(second);
    }
    RBIF = 0;
}
  
void delay_1ms(){   
    while(!T0IF);
    TMR0 = 6;
    T0IF = 0;           
}

void delay_ms(int x){
    for(int i=0; i<x; i++)
        delay_1ms();
}

unsigned int ADC_Read(unsigned char channel)
{
 ADCON0 |= channel<< 1; //Setting the required Bits
  delay_ms(2); //Acquisition time to charge hold capacitor
  GO_nDONE = 1; //Initializes A/D Conversion
  while(GO_nDONE); //Wait for A/D Conversion to complete
  giatri= (unsigned int)ADRESH*256 + ADRESL;
  return giatri; //Returns Result
}