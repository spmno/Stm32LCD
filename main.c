
/*------------------------------------------------
                包含头文件
------------------------------------------------*/
#include "stm32f10x_lib.h"
#include"tft.h"
/*------------------------------------------------
             		主函数
------------------------------------------------*/
void  main(void)
{   

TFT_Initial();//初始化
while(1)
{
ClearScreen(0x0000);
ClearScreen(0x00ff);
ClearScreen(0xff00);


show_colour_bar();	//显示彩条
ClearScreen(0xff00);
Show_RGB (0,240,0,320,0xf800);//刷出320x240的小区域
ClearScreen(0xff00);
LCD_PutString(40,40,"大鹏智能系统",0x0000,0xffff);
LCD_PutString(40,60,"Design by pangyb ",0x0000,0xf800);
LCD_PutString(40,100,"孙老师大课堂出品",0x07e0,0xf800);
while(1);       	//刷完停止
	}
}
