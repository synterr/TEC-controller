
void lcd_init (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (void);
void lcd_home(void);

void lcd_noDisplay(void);
void lcd_display(void);
void lcd_noCursor(void);
void lcd_cursor(void);
void lcd_noBlink(void);
void lcd_blink(void);

void lcd_scrollDisplayLeft(void);
void lcd_scrollDisplayRight(void);
void lcd_leftToRight(void);
void lcd_rightToLeft(void);
void lcd_autoscroll(void);
void lcd_noAutoscroll(void);
