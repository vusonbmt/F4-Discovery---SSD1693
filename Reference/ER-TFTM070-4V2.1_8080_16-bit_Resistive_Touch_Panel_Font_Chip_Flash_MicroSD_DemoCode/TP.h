
/* touch panel interface define */
sbit DCLK	   =    P2^0;   //clock
sbit TPCS        =    P2^1; //chip select
sbit DIN       =    P2^2;	//data input
//sbit BUSY	   =	P2^3;  //busy
sbit DOUT      =    P2^4;	//data output
sbit IRQ       =    P2^5;   //Touch screen response signal detection




unsigned int TP_X,TP_Y;	   


//**********************************************************
void spistar()                                   
{
TPCS=1;
DCLK=1;
DIN=1;
DCLK=1;
}
//**********************************************************
void WriteCharTo7843(unsigned char num)         
{
unsigned char count=0;
DCLK=0;
for(count=0;count<8;count++)
{
num<<=1;
DIN=CY;
DCLK=0; _nop_();_nop_();_nop_();               
DCLK=1; _nop_();_nop_();_nop_();
}
}
//**********************************************************
unsigned int ReadFromCharFrom7843()             
{
unsigned char count=0;
unsigned int Num=0;
for(count=0;count<12;count++)
{
Num<<=1;
DCLK=1; _nop_();_nop_();_nop_();               
DCLK=0; _nop_();_nop_();_nop_();
if(DOUT) Num++;
}
return(Num);
}





void inttostr(int dd,unsigned char *str)
{
	str[0]=dd/10000+48;
	str[1]=(dd/1000)-((dd/10000)*10)+48;
	str[2]=(dd/100)-((dd/1000)*10)+48;
	str[3]=(dd/10)-((dd/100)*10)+48;
	str[4]=dd-((dd/10)*10)+48;
	str[5]=0;
}
void AD7843(void)              
{
 TPCS=0;
delay(10);                     
//while(BUSY);                
delay(10);
WriteCharTo7843(0x90);        
//while(BUSY);               
delay(10);
DCLK=1; _nop_();_nop_();_nop_();_nop_();
DCLK=0; _nop_();_nop_();_nop_();_nop_();
TP_Y=ReadFromCharFrom7843();
WriteCharTo7843(0xD0);       
DCLK=1; _nop_();_nop_();_nop_();_nop_();
DCLK=0; _nop_();_nop_();_nop_();_nop_();
TP_X=ReadFromCharFrom7843();
TPCS=1;
}



//show one Character
void showzifu(unsigned int x,unsigned int y,unsigned char value,unsigned int dcolor,unsigned int bgcolor)	
{  
	unsigned char i,j;
	unsigned char *temp=zifu;    
    LCD_SetPos(x,x+7,y,y+11); //Settings area      
	temp+=(value-32)*12;
	for(j=0;j<12;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<(7-i)))!=0)
			{
				Write_Data_byte(dcolor>>8,dcolor);
			} 
			else
			{
				Write_Data_byte(bgcolor>>8,bgcolor);
			}   
		}
		temp++;
	 }
}
//show String
void showzifustr(unsigned int x,unsigned int y,unsigned char *str,unsigned int dcolor,unsigned int bgcolor)	  
{  
	unsigned int x1,y1;
	x1=x;
	y1=y;
	while(*str!='\0')
	{	
		showzifu(x1,y1,*str,dcolor,bgcolor);
		x1+=7;
		str++;
	}	
}



void TPTEST(void)
{
	unsigned char ss[6];	
	unsigned int lx,ly,k,h;
	spistar();  
 	LCD_clear(0xffff);						
    showzifustr(30,5,"HELLOW!PLEASE TOUCH ME!  Welcome used 7 inch TFT LCD module",0xF800,0xffff);
    showzifustr(80,18,"TP TEST!",0,0xf800);	
	while(KEY)
	{
		if (IRQ==0)
		{  	
			AD7843();
			inttostr(TP_X,ss);
			showzifustr(10,305,"X:",0x001f,0xffff);
			showzifustr(25,305,ss,0xf800,0xffff);	
			inttostr(TP_Y,ss);
			showzifustr(80,305,"Y:",0xf800,0xffff);
			showzifustr(95,305,ss,0xf800,0xffff);	
			lx=(800-(((TP_X-480))*10/39));
			ly=(((TP_Y-240))*10/77);

			for(k=0;k<2;k++)
			{
			    for(h=0;h<2;h++)
				{
					 LCD_SetPos(lx+h,lx+1,ly+k,ly+1);
			         Write_Data_byte(0xf1,0x00);
				 }
			}
 
		}

    }



}













