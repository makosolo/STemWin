#include "sys.h"

#include "bsp_lcd.h"
#include "font.h"

#include "bsp_lcd_tft.h"

/*******************************************************************************
* 函数名	: LCD_DrawLine
* 描述  	: 画线
* 参数  	: x1,y1:起点坐标
*             x2,y2:终点坐标 
* 返回值	: 无
*******************************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
    
	delta_x = x2-x1; //计算坐标增量 
	delta_y = y2-y1; 
	uRow = x1; 
	uCol = y1;
    
	if(delta_x > 0) //设置单步方向
    {
        incx = 1;  
    }
	else if(delta_x == 0) //垂直线
    {
        incx = 0; 
    }
	else 
    {
        incx = -1;
        delta_x = -delta_x;
    } 
    
	if(delta_y > 0)
    {
        incy = 1;
    }
	else if(delta_y==0) //水平线
    {
        incy = 0;
    }
	else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    
	if( delta_x>delta_y) //选取基本增量坐标轴
    {
        distance = delta_x; 
    }
	else 
    {
        distance = delta_y; 
    }
    
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow, uCol);//画点
        
		xerr+=delta_x ; 
		yerr+=delta_y ; 
        
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
        
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

/*******************************************************************************
* 函数名	: LCD_DrawRectangle
* 描述  	: 画矩形
* 参数  	: x1,y1:起点坐标
*             x2,y2:终点坐标 
* 返回值	: 无
*******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

/*******************************************************************************
* 函数名	: Draw_Circle
* 描述  	: 在指定位置画一个指定大小的圆
* 参数  	: x1,y1:中心点
*             r:半径
* 返回值	: 无
*******************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
    
	a = 0;
    b = r;	  
	di = 3-(r<<1); //判断下个点位置的标志
    
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
        
		//使用Bresenham算法画圆
		if(di < 0) 
        {
            di += 4*a+6;
        }            
		else
		{
			di += 10+4*(a-b);   
			b--;
		} 						    
	}
} 									  

/*******************************************************************************
* 函数名	: LCD_ShowChar
* 描述  	: 在指定位置显示一个字符
* 参数  	: x,y:起始坐标
*             num:要显示的字符
*             size:字体大小 12/16
*             mode:叠加方式(1)还是非叠加方式(0)
* 返回值	: 无
*******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u16 colortemp = POINT_COLOR;
    
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	if(!mode) //非叠加方式
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12) temp=asc2_1206[num][t]; //调用1206字体
			else temp = asc2_1608[num][t];		 //调用1608字体 
            
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80) POINT_COLOR = colortemp;
				else POINT_COLOR = BACK_COLOR;
                
				LCD_DrawPoint(x,y);
                
				temp<<=1;
				y++;
                
				if(x >= lcddev.width) //超区域了
                {
                    POINT_COLOR = colortemp;
                    return;
                }
                
				if((y-y0) == size)
				{
					y = y0;
					x++;
                    
					if(x >= lcddev.width) //超区域了
                    {
                        POINT_COLOR=colortemp;
                        return;
                    }
                    
					break;
				}
			}  	 
	    }    
	}else//叠加方式
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12) temp = asc2_1206[num][t]; //调用1206字体
			else temp = asc2_1608[num][t];		   //调用1608字体 
            
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80) LCD_DrawPoint(x,y); 
				temp<<=1;
				y++;
                
				if(x >= lcddev.height)//超区域了
                {
                    POINT_COLOR=colortemp;
                    return;
                }
                
				if((y-y0) == size)
				{
					y = y0;
					x++;
					if(x >= lcddev.width) //超区域了
                    {
                        POINT_COLOR = colortemp;
                        return;
                    }
                    
					break;
				}
			}  	 
	    }     
	}
    
	POINT_COLOR = colortemp;	    	   	 	  
}

/*******************************************************************************
* 函数名	: LCD_Pow
* 描述  	: m^n函数
* 参数  	: 无
* 返回值	: m^n次方.
*******************************************************************************/
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

/*******************************************************************************
* 函数名	: LCD_ShowNum
* 描述  	: 显示数字,高位为0,则不显示
* 参数  	: x,y :起点坐标	
*             num:数值(0~4294967295)
*             len :数字的位数
*             size:字体大小
*             color:颜色
* 返回值	: 无
*******************************************************************************/
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
}

/*******************************************************************************
* 函数名	: LCD_ShowxNum
* 描述  	: 显示数字,高位为0,还是显示
* 参数  	: x,y :起点坐标	
*             num:数值(0~999999999)
*             len :数字的位数
*             size:字体大小
*             mode: [7]:0,不填充;1,填充0.
*                   [6:1]:保留
*                   [0]:0,非叠加显示;1,叠加显示
* 返回值	: 无
*******************************************************************************/
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow = 0;
    
	for(t=0;t<len;t++)
	{
		temp = (num/LCD_Pow(10,len-t-1))%10;
        
		if((enshow == 0) && (t < (len-1)))
		{
			if(temp == 0)
			{
				if(mode&0X80) LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}
            else 
            {  
                enshow = 1;
            }
		}
        
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 

/*******************************************************************************
* 函数名	: LCD_ShowString
* 描述  	: 显示字符串
* 参数  	: x,y :起点坐标	
*             width,height:区域大小
*             size:字体大小
*             *p:字符串起始地址
* 返回值	: 无
*******************************************************************************/
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0 = x;
    
	width+=x;
	height+=y;
    
    while((*p<='~') && (*p>=' '))//判断是不是非法字符!
    {
        if(x >= width) 
        {
            x = x0;
            y+=size;
        }
        
        if(y >= height) break;//退出
        
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

/******************************************************************************
                          以下函数用于优化GUI
******************************************************************************/
/*******************************************************************************
* 函数名	: LCD_TFT_PutPixel
* 描述  	: 画1个像素 （主要用于UCGUI的接口函数）
* 参数  	: _usX,_usY : 像素坐标
*			  _usColor  ：像素颜色
* 返回值	: 无
*******************************************************************************/
void LCD_TFT_PutPixel(u16 _usX , u16 _usY, u16 _usColor)
{
    //设置光标位置
    LCD->LCD_REG = 0x2A;
    LCD->LCD_RAM = _usX>>8;
    LCD->LCD_RAM = _usX&0XFF;
    
    LCD->LCD_REG = 0x2B;
    LCD->LCD_RAM = _usY>>8;
    LCD->LCD_RAM = _usY&0XFF;

	LCD->LCD_REG = 0X2C;//开始写入GRAM

    LCD->LCD_RAM = _usColor;
}

/*******************************************************************************
* 函数名	: LCD_TFT_GetPixel
* 描述  	: 读取1个像素 （主要用于UCGUI的接口函数）
* 参数  	: _usX,_usY : 像素坐标
*			  _usColor  ：像素颜色
* 返回值	: 无
*******************************************************************************/
u16 LCD_TFT_GetPixel(u16 _usX , u16 _usY)
{
    u16 r = 0,g = 0,b = 0;
    
    //设置光标位置
    LCD->LCD_REG = 0x2A;
    LCD->LCD_RAM = _usX>>8;
    LCD->LCD_RAM = _usX&0XFF;
    
    LCD->LCD_REG = 0x2B;
    LCD->LCD_RAM = _usY>>8;
    LCD->LCD_RAM = _usY&0XFF;

    LCD->LCD_REG = 0x2E;//发送读GRAM指令
    
    if(LCD->LCD_RAM) r = 0; //dummy Read
 	r = LCD->LCD_RAM; //实际坐标颜色
    
    b = LCD->LCD_RAM; 
    g = r&0XFF;//第一次读取的是RG的值,R在前,G在后,各占8位
    g <<= 8;

	return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));//需要公式转换一下
}

/*******************************************************************************
* 函数名	: LCD_TFT_DrawHLine
* 描述  	: 绘制一条水平线 （主要用于UCGUI的接口函数）
* 参数  	: _usX1    :起始点X坐标
*			  _usY1    :水平线的Y坐标
*			  _usX2    :结束点X坐标
*			  _usColor :颜色
* 返回值	: 无
*******************************************************************************/
void LCD_TFT_DrawHLine(u16 _usX1 , u16 _usY1 , u16 _usX2 , u16 _usColor)
{
	u16 i;

    LCD_Set_Window(_usX1, _usY1, _usX2 - _usX1 + 1, 1);//设置显示窗口

	LCD->LCD_REG = 0X2C;//开始写入GRAM

	/* 写显存 */
	for (i = 0; i <_usX2-_usX1 + 1; i++)
	{
		LCD->LCD_RAM = _usColor;
	}
}

/*******************************************************************************
* 函数名	: LCD_TFT_DrawHColorLine
* 描述  	: 绘制一条彩色水平线 （主要用于UCGUI的接口函数）
* 参数  	: _usX1    :起始点X坐标
*			  _usY1    :水平线的Y坐标
*			  _usWidth ：直线的宽度
*			  _usColor :颜色
* 返回值	: 无
*******************************************************************************/
void LCD_TFT_DrawHColorLine(u16 _usX1 , u16 _usY1, u16 _usWidth, const u16 *_pColor)
{
	u16 i;
	
    LCD_Set_Window(_usX1, _usY1, _usWidth, 1);//设置显示窗口
    
	LCD->LCD_REG = 0X2C;//开始写入GRAM

	/* 写显存 */
	for (i = 0; i <_usWidth; i++)
	{
		LCD->LCD_RAM = *_pColor++;
	}
}

/*******************************************************************************
* 函数名	: LCD_TFT_DrawVLine
* 描述  	: 画垂直平线 （主要用于UCGUI的接口函数）
* 参数  	: X,Y的坐标和颜色
*			  _usY1    :水平线的Y坐标
*			  _usX2    :结束点X坐标
*			  _usColor :颜色
* 返回值	: 无
*******************************************************************************/
void LCD_TFT_DrawVLine(u16 _usX1 , u16 _usY1 , u16 _usY2 , u16 _usColor)
{
	u16 i;
	
    LCD_Set_Window(_usX1, _usY1, 1, _usY2 - _usY1 + 1);//设置显示窗口

	LCD->LCD_REG = 0X2C;//开始写入GRAM
    
	for (i = _usY1; i <=_usY2; i++)
	{	
		LCD->LCD_RAM = _usColor;
	}
}

/*******************************************************************************
* 函数名	: LCD_TFT_FillRect
* 描述  	: 填充矩形 （主要用于UCGUI的接口函数）
* 参数  	: _usX,_usY：矩形左上角的坐标
*			  _usHeight ：矩形的高度
*			  _usWidth  ：矩形的宽度
*			  _usColor :颜色
* 返回值	: 无
*******************************************************************************/
void LCD_TFT_FillRect(u16 _usX, u16 _usY, u16 _usHeight, u16 _usWidth, u16 _usColor)
{
	u32 i;

	/*
	 ---------------->---
	|(_usX，_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/
    LCD_Set_Window(_usX, _usY, _usWidth, _usHeight);//设置显示窗口

	LCD->LCD_REG = 0X2C;//开始写入GRAM
    
	for (i = 0; i < _usHeight * _usWidth; i++)
	{
		LCD->LCD_RAM = _usColor;
	}
}
