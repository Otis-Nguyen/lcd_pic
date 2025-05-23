
#pragma warning disable 520
#define _XTAL_FREQ    4000000
#include <stdint.h>
#include <xc.h>

#define LCD_FIRST_ROW          0x80
#define LCD_SECOND_ROW         0xC0
#define LCD_THIRD_ROW          0x94
#define LCD_FOURTH_ROW         0xD4
#define LCD_CLEAR              0x01
#define LCD_RETURN_HOME        0x02
#define LCD_ENTRY_MODE_SET     0x04
#define LCD_CURSOR_OFF         0x0C
#define LCD_UNDERLINE_ON       0x0E
#define LCD_BLINK_CURSOR_ON    0x0F
#define LCD_MOVE_CURSOR_LEFT   0x10
#define LCD_MOVE_CURSOR_RIGHT  0x14
#define LCD_TURN_ON            0x0C
#define LCD_TURN_OFF           0x08
#define LCD_SHIFT_LEFT         0x18
#define LCD_SHIFT_RIGHT        0x1E

#ifndef LCD_TYPE
   #define LCD_TYPE 2           // 0=5x7, 1=5x10, 2=2 lines
#endif

#define LCD_RS       PORTDbits.RD2
#define LCD_EN       PORTDbits.RD3
#define LCD_D4       PORTDbits.RD4
#define LCD_D5       PORTDbits.RD5
#define LCD_D6       PORTDbits.RD6
#define LCD_D7       PORTDbits.RD7

#define LCD_RS_DIR   TRISDbits.TRISD2
#define LCD_EN_DIR   TRISDbits.TRISD3
#define LCD_D4_DIR   TRISDbits.TRISD4
#define LCD_D5_DIR   TRISDbits.TRISD5
#define LCD_D6_DIR   TRISDbits.TRISD6
#define LCD_D7_DIR   TRISDbits.TRISD7
        
__bit RS;

void LCD_Write_Nibble(uint8_t n);
void LCD_Cmd(uint8_t Command);
void LCD_Goto(uint8_t col, uint8_t row);
void LCD_PutC(char LCD_Char);
void LCD_Print(char* LCD_Str);
void LCD_Begin();
void LCD_WriteNum(unsigned char num);
void LCD_Write_Nibble(uint8_t n)
{
  LCD_RS = RS;
  LCD_D4 = n & 0x01;
  LCD_D5 = (n >> 1) & 0x01;
  LCD_D6 = (n >> 2) & 0x01;
  LCD_D7 = (n >> 3) & 0x01;

  // send enable pulse
  LCD_EN = 0;
  __delay_us(1);
  LCD_EN = 1;
  __delay_us(1);
  LCD_EN = 0;
  __delay_us(100);
}

void LCD_Cmd(uint8_t Command)
{
  RS = 0;
  LCD_Write_Nibble(Command >> 4);
  LCD_Write_Nibble(Command);
  if((Command == LCD_CLEAR) || (Command == LCD_RETURN_HOME))
    __delay_ms(2);
}

void LCD_Goto(uint8_t col, uint8_t row)
{
  switch(row)
  {
    case 2:
      LCD_Cmd(LCD_SECOND_ROW + col - 1);
      break;
    case 3:
      LCD_Cmd(LCD_THIRD_ROW  + col - 1);
      break;
    case 4:
      LCD_Cmd(LCD_FOURTH_ROW + col - 1);
    break;
    default:      // case 1:
      LCD_Cmd(LCD_FIRST_ROW  + col - 1);
  }

}
void LCD_WriteNum(unsigned char num)
{
    unsigned char digit;
    //digit = num/100;
    //LCD_PutC(digit + '0');           //write tram+ma ASCII cua '0'
    digit = num/10;
    digit = digit % 10;
    LCD_PutC(digit + '0');           //write chuc
    digit = num % 100;
    digit = digit % 10;
    LCD_PutC(digit + '0');           //write dvi
}

void LCD_PutC(char LCD_Char)
{
  RS = 1;
  LCD_Write_Nibble(LCD_Char >> 4);
  LCD_Write_Nibble(LCD_Char );
}

void LCD_Print(char* LCD_Str)
{
  uint8_t i = 0;
  RS = 1;
  while(LCD_Str[i] != '\0')
  {
    LCD_Write_Nibble(LCD_Str[i] >> 4);
    LCD_Write_Nibble(LCD_Str[i++] );
  }
}

void LCD_Begin()
{
  
  LCD_RS_DIR = 0;
  LCD_EN_DIR = 0;
  LCD_D4_DIR = 0;
  LCD_D5_DIR = 0;
  LCD_D6_DIR = 0;
  LCD_D7_DIR = 0;
RS = 0;

  LCD_RS     = 0;
  LCD_EN     = 0;
  LCD_D4     = 0;
  LCD_D5     = 0;
  LCD_D6     = 0;
  LCD_D7     = 0;
  __delay_ms(40);
  LCD_Cmd(3);
  __delay_ms(5);
  LCD_Cmd(3);
  __delay_ms(5);
  LCD_Cmd(3);
  __delay_ms(5);
  LCD_Cmd(LCD_RETURN_HOME);
  __delay_ms(5);
  LCD_Cmd(0x20 | (LCD_TYPE << 2));
  __delay_ms(50);
  LCD_Cmd(LCD_TURN_ON);
  __delay_ms(50);
  LCD_Cmd(LCD_CLEAR);
  __delay_ms(50);
  LCD_Cmd(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
  __delay_ms(50);
}