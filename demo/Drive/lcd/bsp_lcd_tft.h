#ifndef __BSP_LCD_TFT_H_
#define __BSP_LCD_TFT_H_		 

//主要用于UCGUI的接口函数
void LCD_TFT_PutPixel(u16 _usX , u16 _usY, u16 _usColor);                             //画1个像素
u16 LCD_TFT_GetPixel(u16 _usX , u16 _usY);                                            //读取1个像素
void LCD_TFT_DrawHLine(u16 _usX1 , u16 _usY1 , u16 _usX2 , u16 _usColor);             //绘制一条水平线
void LCD_TFT_DrawHColorLine(u16 _usX1 , u16 _usY1, u16 _usWidth, const u16 *_pColor); //绘制一条彩色水平线
void LCD_TFT_DrawVLine(u16 _usX1 , u16 _usY1 , u16 _usY2 , u16 _usColor);             //绘制一条垂直线
void LCD_TFT_DrawHColorLine(u16 _usX1 , u16 _usY1, u16 _usWidth, const u16 *_pColor); //绘制一条彩色垂直线
void LCD_TFT_FillRect(u16 _usX, u16 _usY, u16 _usHeight, u16 _usWidth, u16 _usColor); //填充矩形

void Draw_Circle(u16 x0,u16 y0,u8 r);                                //画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);                   //画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);              //画矩形
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);               //显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);                //显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);       //显示 数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p); //显示一个字符串,12/16字体

#endif //__BSP_LCD_TFT_H_ 
