/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.30 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information

Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011 
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/
#include "touch.h" 
#include "GUI.h"

void GUI_TOUCH_X_ActivateX(void) {
}

void GUI_TOUCH_X_ActivateY(void) {
}

int  GUI_TOUCH_X_MeasureX(void) {
  return 0;//TP_Read_XOY(0XD0);  //CMD_RDX=0XD0;
}

int  GUI_TOUCH_X_MeasureY(void) {
  return 0;//TP_Read_XOY(0X90);  //CMD_RDX=0XD0;
}

///**
//  *****************************************************************************
//  * @Name   : GUI获取触摸坐标值
//  *
//  * @Brief  : none
//  *
//  * @Input  : none
//  *
//  * @Output : none
//  *
//  * @Return : none
//  *****************************************************************************
//**/
//void GUI_TOUCH_X_GetXY(void)
//{
//	u16 temp=0;
//	
//	//在EEPROM存储的相对位置
//	//Touch_Adjust_Base     --> TOUCH_AD_y0  --> = (TOUCH_AD_x0 << 8) + TOUCH_AD_y0
//	//Touch_Adjust_Base + 1 --> TOUCH_AD_x0
//	//Touch_Adjust_Base + 2 --> TOUCH_AD_x1 低8位
//	//Touch_Adjust_Base + 3 --> TOUCH_AD_x1 高8位
//	//Touch_Adjust_Base + 4 --> TOUCH_AD_y1 低8位
//	//Touch_Adjust_Base + 5 --> TOUCH_AD_y1 高8位
//	
//	temp        = (u16)EEPROM_ReadLenByte(Write_add, Touch_Adjust_Base, 2);  //读取数据
//	TOUCH_AD_x1 = temp / 256;
//	TOUCH_AD_y0 = temp % 256;
//	
//	TOUCH_AD_x1 = (int)EEPROM_ReadLenByte(Write_add, Touch_Adjust_Base+2, 2);  //读取数据
//	TOUCH_AD_y1 = (int)EEPROM_ReadLenByte(Write_add, Touch_Adjust_Base+4, 2);  //读取数据
//	
//	GUI_TOUCH_Calibrate(GUI_COORD_X, 0, 240, TOUCH_AD_y0, TOUCH_AD_y1);  //读取Y值
//	GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, 320, TOUCH_AD_x0, TOUCH_AD_x1);  //读取X值
//}

///**
//  *****************************************************************************
//  * @Name   : GUI触摸屏校准
//  *
//  * @Brief  : none
//  *
//  * @Input  : none
//  *
//  * @Output : none
//  *
//  * @Return : none
//  *****************************************************************************
//**/
//void Touch_Adjust(void)
//{	
//	GUI_CURSOR_Show();
//	GUI_CURSOR_Select(&GUI_CursorCrossL);
//	GUI_SetBkColor(GUI_WHITE);
//	GUI_SetColor(GUI_BLACK);
//	GUI_Clear();
//	
//	GUI_SetDefaultFont(GUI_FONT_8X16_ASCII);
//	GUI_DispStringAt("Measurement of", 30, 80);
//	GUI_DispStringAt("A/D converter values", 30, 80+16);
//	
//	while (1)
//	{
// 		GUI_PID_STATE TouchState;
// 		int xPhys, yPhys;
//		GUI_TOUCH_GetState(&TouchState);  /* Get the touch position in pixel */
//		xPhys = GUI_TOUCH_GetxPhys();     /* Get the A/D mesurement result in x */
//		yPhys = GUI_TOUCH_GetyPhys();     /* Get the A/D mesurement result in y */
//		
//		/* Display the measurement result */
//		GUI_SetColor(GUI_BLUE);	
//		GUI_DispStringAt("Analog input:", 30, 80+32);
//		
//		GUI_DispStringAt("x:", 30, 80+48);
//		GUI_DispDecAt(xPhys, 30+16, 80+48, 4);
//		
//		GUI_DispStringAt("y:", 30+50, 80+48);
//		GUI_DispDecAt(yPhys, 30+66, 80+48, 4);
//		
//		/* Display the according position */
//		GUI_SetColor(GUI_RED);
//		
//		GUI_DispStringAt("Position:", 30, 160);
//		
//		GUI_DispStringAt("x:", 30, 160+16);
//		GUI_DispDecAt(TouchState.x, 30+16, 160+16, 4);
//		
//		GUI_DispStringAt("y:", 30+50, 160+16);
//		GUI_DispDecAt(TouchState.y, 30+66, 160+16, 4);
//		
//		/* Wait a while */
//		delay_ms(100);
//	};
//}

