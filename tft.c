/*------------------------------------------------
                包含头文件
------------------------------------------------*/
#include "stm32f10x_lib.h"
#include"tft.h"
/*-----------------------------------------------------------------------
                           液晶端口定义
-----------------------------------------------------------------------*/
#define	TFT_WR	(1<<11)  //写数据控制线		PB11
#define	TFT_RS	(1<<10)  //数据/命令选择端口	PB10
#define	TFT_CS	(1<<9)  //片选端口  	    PB9
#define	TFT_RST	(1<<8)  //硬复位			PB8
//彩屏RD引脚接3.3V 悬空或者接5V可能导致不能正常工作


#define TFT_RS_SET(x)   GPIOB->ODR=(GPIOB->ODR&~TFT_RS)|(x ? TFT_RS:0)
#define TFT_CS_SET(x)   GPIOB->ODR=(GPIOB->ODR&~TFT_CS)|(x ? TFT_CS:0)													     
#define TFT_RST_SET(x)  GPIOB->ODR=(GPIOB->ODR&~TFT_RST)|(x ? TFT_RST:0)	
#define TFT_WR_SET(x)   GPIOB->ODR=(GPIOB->ODR&~TFT_WR)|(x ? TFT_WR:0)	
 

#define DATAOUT(x) GPIOA->ODR=(GPIOA->ODR&0xffffff00)|(x&0xff);//定义8位数据操作端口  
//unsigned char code pic[];
#define X_MAX 240
#define Y_MAX 400
/*------------------------------------------------
             		清屏函数
------------------------------------------------*/
void ClearScreen(unsigned int bColor)
{
 unsigned int i,j;
 LCD_SetPos(0,X_MAX-1,0,Y_MAX-1);//
 for (i=0;i<Y_MAX;i++)
	{
	
	   for (j=0;j<X_MAX;j++)
	       Write_Data_U16(bColor);

	}
}

/*------------------------------------------------
             写8x16字符函数
------------------------------------------------*/
#include "8X16.h"
void LCD_PutChar8x16(unsigned short x, unsigned short y, char c, unsigned int fColor, unsigned int bColor)
{
 unsigned int i,j;
 LCD_SetPos(x,x+8-1,y,y+16-1);
 for(i=0; i<16;i++) {
		unsigned char m=Font8x16[c*16+i];
		for(j=0;j<8;j++) {
			if((m&0x80)==0x80) {
				Write_Data_U16(fColor);
				}
			else {
				Write_Data_U16(bColor);
				}
			m<<=1;
			}
		}
}

/*------------------------------------------------
              写字符串函数
------------------------------------------------*/
void LCD_PutChar(unsigned short x, unsigned short y, char c, unsigned int fColor, unsigned int bColor) {

		LCD_PutChar8x16( x, y, c, fColor, bColor );
	}

/*------------------------------------------------
             写16x16汉字函数
------------------------------------------------*/
#include "GB1616.h"	//16*16汉字字模

void PutGB1616(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor){
	unsigned int i,j,k;

	LCD_SetPos(x,  x+16-1,y, y+16-1);

	for (k=0;k<64;k++) { //64标示自建汉字库中的个数，循环查询内码
	  if ((codeGB_16[k].Index[0]==c[0])&&(codeGB_16[k].Index[1]==c[1])){ 
    	for(i=0;i<32;i++) {
		  unsigned short m=codeGB_16[k].Msk[i];
		  for(j=0;j<8;j++) {
			if((m&0x80)==0x80) {
				Write_Data_U16(fColor);
				}
			else {
				Write_Data_U16(bColor);
				}
			m<<=1;
			} 
		  }
		}  
	  }	
	}
/*------------------------------------------------
                写字符串函数
------------------------------------------------*/
void LCD_PutString(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor) {
	 unsigned char l=0;
	while(*s) {
		if( *s < 0x80) 
		    {
			LCD_PutChar(x+l*8,y,*s,fColor,bColor);
			s++;l++;
			}
		else
		    {
			PutGB1616(x+l*8,y,(unsigned char*)s,fColor,bColor);
			s+=2;l+=2;
			}
		}
	}
/*------------------------------------------------
              指定区域显示指定RGB颜色
------------------------------------------------*/
void Show_RGB (unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1,unsigned int Color)
{
	unsigned int i,j;
	LCD_SetPos(x0,x1,y0,y1);
	for (i=y0;i<=y1;i++)
	{
	   for (j=x0;j<=x1;j++)
	       Write_Data_U16(Color);
	}
}

/*------------------------------------------------
                显示彩条
------------------------------------------------*/
void show_colour_bar (void)//8条色彩

{   unsigned char GAP=50;
	int V,H;
	LCD_SetPos(0,X_MAX-1,0,Y_MAX-1);

	for(H=0;H<X_MAX;H++)
	{
		for(V=0;V<GAP*1;V++)
		Write_Data_U16(0xf800);
	}

	for(H=0;H<X_MAX;H++)
	{
		for(V=GAP*1;V<GAP*2;V++)
		Write_Data_U16(0x07e0);
	}

	for(H=0;H<X_MAX;H++)
	{
		for(V=GAP*2;V<GAP*3;V++)
		Write_Data_U16(0x001f);
	}

	for(H=0;H<X_MAX;H++)
	{
		for(V=GAP*3;V<GAP*4;V++)
		Write_Data_U16(0xffe0);
	}

	for(H=0;H<X_MAX;H++)
	{
		for(V=GAP*4;V<GAP*5;V++)
		Write_Data_U16(0xf81f);
	}

	for(H=0;H<X_MAX;H++)
	{
		for(V=GAP*5;V<GAP*6;V++)
		Write_Data_U16(0x07ff);
	}

	for(H=0;H<X_MAX;H++)
	{
		for(V=GAP*6;V<GAP*7;V++)
		Write_Data_U16(0xffff);
	}

	for(H=0;H<X_MAX;H++)
	{
		for(V=GAP*7;V<GAP*8;V++)
		Write_Data_U16(0x0000);
	}

}
/*------------------------------------------------
             显示图片函数
------------------------------------------------*/
/*void show_photo(void)
{
	unsigned char j;
	unsigned int i;
	unsigned long s=0;

	LCD_SetPos(0,240,0,320);//320x240

	for (i=0;i<75;i++)
	{
	for (j=0;j<240;j++)
	Write_Data_U16(0xffff);
		
	}

	for (i=0;i<170;i++)
	{
	for (j=0;j<55;j++)
		Write_Data_U16(0xffff);

		for (j=0;j<130;j++)
		Write_Data_U16(pic[s++],pic[s++]);
		
		for (j=0;j<55;j++)
		Write_Data_U16(0xffff);
	}

    for (i=0;i<75;i++)
	{
	for (j=0;j<240;j++)
	Write_Data_U16(0xffff);
		
	}
}
*/
/*------------------------------------------------
              写16位数据函数
------------------------------------------------*/
void  Write_Data_U16(unsigned int y)
{
	unsigned char m,n;
	m=y>>8;
	n=y;
	LCD_Write_Data(m);
    LCD_Write_Data(n);

}
/*------------------------------------------------
                写指令函数
------------------------------------------------*/
void LCD_Write_Command(unsigned char u)
{
  	
	TFT_CS_SET(0); 
	TFT_RS_SET(0);

	DATAOUT(u);
	TFT_WR_SET(0); 
	TFT_WR_SET(1);


	TFT_CS_SET(1); 
}

/*------------------------------------------------
                 写数据函数
------------------------------------------------*/
void LCD_Write_Data(unsigned char u)
{	
	TFT_CS_SET(0); 
	TFT_RS_SET(1);

	DATAOUT(u);
	TFT_WR_SET(0); 
	TFT_WR_SET(1);


	TFT_CS_SET(1); 
}
void WriteCom(unsigned char a,unsigned char b)
{
LCD_Write_Command(a);
LCD_Write_Command(b);
}

void WriteData(unsigned char a,unsigned char b)
{
LCD_Write_Data(a);
LCD_Write_Data(b);
}
/*------------------------------------------------
                延时函数
------------------------------------------------*/
void delayms(unsigned int count)
{
    int i,j;                                                                                
    for(i=0;i<count;i++)                                                                    
       {
	     for(j=0;j<260;j++);
       }                                                                                     
}



/*------------------------------------------------
             	液晶初始化代码
------------------------------------------------*/

void TFT_Initial(void)
{ 
  RCC->APB2ENR|=0X0000001C;//先使能外设IO PORTA,B,C时钟
	
	GPIOA->CRL=0X33333333; //PA0-7 推挽输出
	//GPIOA->ODR|=0X00FF;//全部输出高
	GPIOB->CRH=0X33333333; //PB8-15 推挽输出
	//GPIOB->ODR|=0X00FF;//全部输出高
	/*GPIOB->CRL&=0X000FFFFF;//PB5-7 推挽输出
	GPIOB->CRL|=0X33300000;
	GPIOB->CRH&=0XFFFFFFF0;//PB8   推挽输出
	GPIOB->CRH|=0X00000003;
	GPIOB->ODR|=0X01E0;//5-8 输出高

	GPIOC->CRL&=0XFF00FFFF;//PC4,5 推挽输出
	GPIOC->CRL|=0X00330000;
	GPIOC->ODR|=0X0030;//4,5 输出高	 */
	TFT_RST_SET(0);
	delayms(20);
	TFT_RST_SET(1);
	delayms(200);
 // Synchronization after reset
WriteCom(0x06,0x06);WriteData(0X00,0X00);
delayms(100);
WriteCom(0x00,0x07);WriteData(0X00,0X01);
delayms(5);
WriteCom(0x01,0x10);WriteData(0X00,0X01);
delayms(5);
WriteCom(0x01,0x00);WriteData(0X17,0XB0);
WriteCom(0x01,0x01);WriteData(0X01,0X47);
WriteCom(0x01,0x02);WriteData(0X01,0X9D);
WriteCom(0x01,0x03);WriteData(0X36,0X00);
WriteCom(0x02,0x81);WriteData(0X00,0X10);
delayms(5);
WriteCom(0x01,0x02);WriteData(0X01,0XBD);
delayms(5);
WriteCom(0x00,0x00);WriteData(0X00,0X00);
WriteCom(0x00,0x01);WriteData(0X01,0X00);
WriteCom(0x00,0x02);WriteData(0X01,0X00);
WriteCom(0x00,0x03);WriteData(0X10,0XB0);
WriteCom(0x00,0x06);WriteData(0X00,0X00);
WriteCom(0x00,0x08);WriteData(0X02,0X02);

WriteCom(0x00,0x09);WriteData(0X00,0X01);
WriteCom(0x00,0x0B);WriteData(0X00,0X10);
WriteCom(0x00,0x0C);WriteData(0X00,0X00);
WriteCom(0x00,0x0F);WriteData(0X00,0X00);
WriteCom(0x00,0x07);WriteData(0X00,0X01);

WriteCom(0x00,0x10);WriteData(0X00,0X11);
WriteCom(0x00,0x11);WriteData(0X03,0X01);
WriteCom(0x00,0x12);WriteData(0X03,0X00);
WriteCom(0x00,0x20);WriteData(0X02,0X1E);
WriteCom(0x00,0x21);WriteData(0X02,0X02);

WriteCom(0x00,0x90);WriteData(0X08,0X00);
WriteCom(0x00,0x92);WriteData(0X00,0X00);
WriteCom(0x01,0x00);WriteData(0X12,0XB0);
delayms(10);
WriteCom(0x01,0x01);WriteData(0X01,0X47);
delayms(10);
WriteCom(0x01,0x02);WriteData(0X01,0XBE);
delayms(10);
WriteCom(0x01,0x03);WriteData(0X2B,0X00);
delayms(10);
WriteCom(0x01,0x07);WriteData(0X00,0X00);
delayms(10);
WriteCom(0x01,0x10);WriteData(0X00,0X01);
delayms(10);
WriteCom(0x02,0x10);WriteData(0X00,0X00);
WriteCom(0x02,0x11);WriteData(0X00,0XEF);
WriteCom(0x02,0x12);WriteData(0X00,0X00);
WriteCom(0x02,0x13);WriteData(0X01,0X8F);
WriteCom(0x02,0x00);WriteData(0X00,0X00);

WriteCom(0x02,0x01);WriteData(0X00,0X00);
WriteCom(0x02,0x80);WriteData(0X00,0X00);
WriteCom(0x02,0x81);WriteData(0X00,0X07);
WriteCom(0x02,0x82);WriteData(0X00,0X00);
delayms(10);
WriteCom(0x03,0x00);WriteData(0X01,0X01);
WriteCom(0x03,0x01);WriteData(0X09,0X29);
WriteCom(0x03,0x02);WriteData(0X0F,0X2C);
WriteCom(0x03,0x03);WriteData(0X2C,0X0F);
WriteCom(0x03,0x04);WriteData(0X29,0X09);
WriteCom(0x03,0x05);WriteData(0X01,0X01);
WriteCom(0x03,0x06);WriteData(0X19,0X04);
WriteCom(0x03,0x07);WriteData(0X04,0X19);
WriteCom(0x03,0x08);WriteData(0X06,0X05);

WriteCom(0x03,0x09);WriteData(0X04,0X03);
WriteCom(0x03,0x0A);WriteData(0X0E,0X06);
WriteCom(0x03,0x0B);WriteData(0X0E,0X00);
WriteCom(0x03,0x0C);WriteData(0X00,0X0E);
WriteCom(0x03,0x0D);WriteData(0X06,0X0E);
WriteCom(0x03,0x0E);WriteData(0X03,0X04);
WriteCom(0x03,0x0F);WriteData(0X05,0X06);
WriteCom(0x04,0x00);WriteData(0X35,0X00);
WriteCom(0x04,0x01);WriteData(0X00,0X01);

WriteCom(0x04,0x04);WriteData(0X00,0X00);
WriteCom(0x05,0x00);WriteData(0X00,0X00);
WriteCom(0x05,0x01);WriteData(0X00,0X00);
WriteCom(0x05,0x02);WriteData(0X00,0X00);
WriteCom(0x05,0x03);WriteData(0X00,0X00);
WriteCom(0x05,0x04);WriteData(0X00,0X00);
WriteCom(0x05,0x05);WriteData(0X00,0X00);
WriteCom(0x06,0x00);WriteData(0X00,0X00);
WriteCom(0x06,0x06);WriteData(0X00,0X00);
WriteCom(0x06,0xF0);WriteData(0X00,0X00);
WriteCom(0x07,0xF0);WriteData(0X54,0X20);
WriteCom(0x07,0xF3);WriteData(0X28,0X0F);
WriteCom(0x07,0xF4);WriteData(0X00,0X22);
WriteCom(0x07,0xF5);WriteData(0X00,0X01);
WriteCom(0x07,0xF0);WriteData(0X00,0X00);
WriteCom(0x00,0x07);WriteData(0X01,0X73);
delayms(5);
WriteCom(0x00,0x07);WriteData(0X00,0X61);
delayms(5);
WriteCom(0x00,0x07);WriteData(0X01,0X73);
delayms(500);
WriteCom(0x02,0x02);		
}

/*------------------------------------------------
函数名称:LCD_DefineDispWindow
功    能:定义显示窗体
参    数:x0:  窗体中X坐标中较小者
	 x1:  窗体中X坐标中较大者
	 y0:  窗体中Y坐标中较小者
	 y1:  窗体中Y坐标中较大者 
返 回 值:无
------------------------------------------------*/
static void LCD_SetPos(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1)
{

WriteCom(0x02,0x10);WriteData(x0>>8,x0);
WriteCom(0x02,0x11);WriteData(x1>>8,x1);
WriteCom(0x02,0x12);WriteData(y0>>8,y0);
WriteCom(0x02,0x13);WriteData(y1>>8,y1);

WriteCom(0x02,0x00);WriteData(0X00,0X00);
WriteCom(0x02,0x01);WriteData(0X00,0X00);
WriteCom(0x02,0x02);
}
