
/** Put this in the src folder **/

#include "PCF8574.h"
#include "i2c-lcd.h"
#include "tools.h"

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define SLAVE_ADDRESS_LCD 0x27 << 1 // change this according to ur setup

#define PIN_RS    (1 << 0)
#define PIN_EN    (1 << 2)
#define BACKLIGHT (1 << 3)
#define LCD_D(n)  (1 << n)

static uint8_t displayfunction = 0;
static uint8_t displaycontrol = 0;
static uint8_t displaymode = 0;
	

void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|BACKLIGHT|PIN_EN;
	data_t[1] = data_u|BACKLIGHT;
	data_t[2] = data_l|BACKLIGHT|PIN_EN;
	data_t[3] = data_l|BACKLIGHT;

	PCF_WriteMulti (SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4);
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|PIN_RS|BACKLIGHT|PIN_EN;
	data_t[1] = data_u|PIN_RS|BACKLIGHT;
	data_t[2] = data_l|PIN_RS|BACKLIGHT|PIN_EN;
	data_t[3] = data_l|PIN_RS|BACKLIGHT;

	PCF_WriteMulti (SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4);
}

void lcd_clear (void)
{
	lcd_send_cmd (LCD_CLEARDISPLAY);
  delay_nops(10*DLMUL);
}

void lcd_home(){
	lcd_send_cmd(LCD_RETURNHOME);  // set cursor position to zero
	delay_nops(10*DLMUL);
}

void lcd_put_cur(int row, int col)
{
int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > 3 ) {
		row = 0;    // we count rows starting w/0
	}
    lcd_send_cmd (LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcd_init (void)
{
	// 4 bit initialisation
	delay_nops(50*DLMUL);//HAL_Delay(50);  // wait for >40ms
  
  uint8_t data_t[4];
  
  // initialize 4 bit long interface (DB4-DB7 data pins used)
  data_t[0] = LCD_D(4) | LCD_D(5) | PIN_EN;
  data_t[1] = LCD_D(4) | LCD_D(5);
  PCF_WriteMulti(SLAVE_ADDRESS_LCD, data_t, 2);
  delay_nops(5*DLMUL);

  data_t[0] = LCD_D(4) | LCD_D(5) | PIN_EN;
  data_t[1] = LCD_D(4) | LCD_D(5);
  PCF_WriteMulti(SLAVE_ADDRESS_LCD, data_t, 2);
  delay_nops(1*DLMUL);

  data_t[0] = LCD_D(4) | LCD_D(5) | PIN_EN;
  data_t[1] = LCD_D(4) | LCD_D(5);
  PCF_WriteMulti(SLAVE_ADDRESS_LCD, data_t, 2);

  data_t[0] = LCD_D(5) | PIN_EN;
  data_t[1] = LCD_D(5);
  PCF_WriteMulti(SLAVE_ADDRESS_LCD, data_t, 2);

  // set # lines, font size, etc. 
  displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
	lcd_send_cmd(LCD_FUNCTIONSET | displayfunction);  
	
	// turn the display on with no cursor or blinking default
	displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	lcd_send_cmd(LCD_DISPLAYCONTROL | displaycontrol); 
  
  lcd_clear();
  
	// Initialize to default text direction (for roman languages)
	displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	
	// set the entry mode
	lcd_send_cmd(LCD_ENTRYMODESET | displaymode);
  lcd_home();

}

void lcd_noDisplay() {
	displaycontrol &= ~LCD_DISPLAYON;
	lcd_send_cmd(LCD_DISPLAYCONTROL | displaycontrol);
}

void lcd_display() {
	displaycontrol |= LCD_DISPLAYON;
	lcd_send_cmd(LCD_DISPLAYCONTROL | displaycontrol);
}

// Turns the underline cursor on/off
void lcd_noCursor() {
	displaycontrol &= ~LCD_CURSORON;
	lcd_send_cmd(LCD_DISPLAYCONTROL | displaycontrol);
}
void lcd_cursor() {
	displaycontrol |= LCD_CURSORON;
	lcd_send_cmd(LCD_DISPLAYCONTROL | displaycontrol);
}

// Turn on and off the blinking cursor
void lcd_noBlink() {
	displaycontrol &= ~LCD_BLINKON;
	lcd_send_cmd(LCD_DISPLAYCONTROL | displaycontrol);
}
void lcd_blink() {
	displaycontrol |= LCD_BLINKON;
	lcd_send_cmd(LCD_DISPLAYCONTROL | displaycontrol);
}

// These commands scroll the display without changing the RAM
void lcd_scrollDisplayLeft(void) {
	lcd_send_cmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void lcd_scrollDisplayRight(void) {
	lcd_send_cmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void lcd_leftToRight(void) {
	displaymode |= LCD_ENTRYLEFT;
	lcd_send_cmd(LCD_ENTRYMODESET | displaymode);
}

// This is for text that flows Right to Left
void lcd_rightToLeft(void) {
	displaymode &= ~LCD_ENTRYLEFT;
	lcd_send_cmd(LCD_ENTRYMODESET | displaymode);
}

// This will 'right justify' text from the cursor
void lcd_autoscroll(void) {
	displaymode |= LCD_ENTRYSHIFTINCREMENT;
	lcd_send_cmd(LCD_ENTRYMODESET | displaymode);
}

// This will 'left justify' text from the cursor
void lcd_noAutoscroll(void) {
	displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	lcd_send_cmd(LCD_ENTRYMODESET | displaymode);
}


void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}
