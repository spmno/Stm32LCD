
/*------------------------------------------------
                ����ͷ�ļ�
------------------------------------------------*/
#include "stm32f10x_lib.h"
#include"tft.h"
/*------------------------------------------------
             		������
------------------------------------------------*/
void  main(void)
{   

TFT_Initial();//��ʼ��
while(1)
{
ClearScreen(0x0000);
ClearScreen(0x00ff);
ClearScreen(0xff00);


show_colour_bar();	//��ʾ����
ClearScreen(0xff00);
Show_RGB (0,240,0,320,0xf800);//ˢ��320x240��С����
ClearScreen(0xff00);
LCD_PutString(40,40,"��������ϵͳ",0x0000,0xffff);
LCD_PutString(40,60,"Design by pangyb ",0x0000,0xf800);
LCD_PutString(40,100,"����ʦ����ó�Ʒ",0x07e0,0xf800);
while(1);       	//ˢ��ֹͣ
	}
}
