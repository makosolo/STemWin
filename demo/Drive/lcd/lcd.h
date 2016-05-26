#ifndef _LCD_H_
#define _LCD_H_

//»­±ÊÑÕÉ«
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //×ØÉ«
#define BRRED 			 0XFC07 //×ØºìÉ«
#define GRAY  			 0X8430 //»ÒÉ«

void LCD_init(void);

void LCD_Clear(u16 color);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_DrawPoint(int x,int y,u16 color);
u16 LCD_ReadPoint(int x, int y);
void Refurbish_LCD(void *tp);

#endif//_LCD_H_
