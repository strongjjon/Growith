
#pragma used+
sfrb PINF=0;
sfrb PINE=1;
sfrb DDRE=2;
sfrb PORTE=3;
sfrb ADCL=4;
sfrb ADCH=5;
sfrw ADCW=4;      
sfrb ADCSRA=6;
sfrb ADMUX=7;
sfrb ACSR=8;
sfrb UBRR0L=9;
sfrb UCSR0B=0xa;
sfrb UCSR0A=0xb;
sfrb UDR0=0xc;
sfrb SPCR=0xd;
sfrb SPSR=0xe;
sfrb SPDR=0xf;
sfrb PIND=0x10;
sfrb DDRD=0x11;
sfrb PORTD=0x12;
sfrb PINC=0x13;
sfrb DDRC=0x14;
sfrb PORTC=0x15;
sfrb PINB=0x16;
sfrb DDRB=0x17;
sfrb PORTB=0x18;
sfrb PINA=0x19;
sfrb DDRA=0x1a;
sfrb PORTA=0x1b;
sfrb EECR=0x1c;
sfrb EEDR=0x1d;
sfrb EEARL=0x1e;
sfrb EEARH=0x1f;
sfrw EEAR=0x1e;   
sfrb SFIOR=0x20;
sfrb WDTCR=0x21;
sfrb OCDR=0x22;
sfrb OCR2=0x23;
sfrb TCNT2=0x24;
sfrb TCCR2=0x25;
sfrb ICR1L=0x26;
sfrb ICR1H=0x27;
sfrw ICR1=0x26;   
sfrb OCR1BL=0x28;
sfrb OCR1BH=0x29;
sfrw OCR1B=0x28;  
sfrb OCR1AL=0x2a;
sfrb OCR1AH=0x2b;
sfrw OCR1A=0x2a;  
sfrb TCNT1L=0x2c;
sfrb TCNT1H=0x2d;
sfrw TCNT1=0x2c;  
sfrb TCCR1B=0x2e;
sfrb TCCR1A=0x2f;
sfrb ASSR=0x30;
sfrb OCR0=0x31;
sfrb TCNT0=0x32;
sfrb TCCR0=0x33;
sfrb MCUCSR=0x34;
sfrb MCUCR=0x35;
sfrb TIFR=0x36;
sfrb TIMSK=0x37;
sfrb EIFR=0x38;
sfrb EIMSK=0x39;
sfrb EICRB=0x3a;
sfrb RAMPZ=0x3b;
sfrb XDIV=0x3c;
sfrb SPL=0x3d;
sfrb SPH=0x3e;
sfrb SREG=0x3f;
#pragma used-

#asm
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x20
	.EQU __sm_mask=0x1C
	.EQU __sm_powerdown=0x10
	.EQU __sm_powersave=0x18
	.EQU __sm_standby=0x14
	.EQU __sm_ext_standby=0x1C
	.EQU __sm_adc_noise_red=0x08
	.SET power_ctrl_reg=mcucr
	#endif
#endasm

typedef char *va_list;

#pragma used+

char getchar(void);

void putchar(char c);

void puts(char *str);
void putsf(char flash *str);
int printf(char flash *fmtstr,...);
int sprintf(char *str, char flash *fmtstr,...);
int vprintf(char flash * fmtstr, va_list argptr);
int vsprintf(char *str, char flash * fmtstr, va_list argptr);

char *gets(char *str,unsigned int len);
int snprintf(char *str, unsigned int size, char flash *fmtstr,...);
int vsnprintf(char *str, unsigned int size, char flash * fmtstr, va_list argptr);

int scanf(char flash *fmtstr,...);
int sscanf(char *str, char flash *fmtstr,...);

#pragma used-

#pragma library stdio.lib

#pragma used+

void delay_us(unsigned int n);
void delay_ms(unsigned int n);

#pragma used-

unsigned int Yaw_angle=85;      
unsigned int Pitch_angle=95;    

unsigned char PcData=0;

void Servo_Yaw(int angle_Yaw){ 
int PB5;
angle_Yaw=angle_Yaw-90;
if(angle_Yaw<-85)angle_Yaw=-88;
if(angle_Yaw> 85)angle_Yaw= 88;
PB5=(angle_Yaw+ 90)*20 +1500;
OCR1A=PB5; 
}
void Servo_Pitch(int angle_Pitch){ 
int PB6;
angle_Pitch=angle_Pitch-90;
if(angle_Pitch<-85)angle_Pitch=-88;
if(angle_Pitch> 85)angle_Pitch= 88;
PB6=(angle_Pitch+ 90)*20 +1500;
OCR1B=PB6; 
}

interrupt [19] void usart0_rx_isr(void)     
{ 
PcData=UDR0; 
}
void Set_INI(void)
{                           
Servo_Yaw(Yaw_angle);
Servo_Pitch(Pitch_angle);
PORTB.0 = 0;

delay_ms(1000);
}

void main(void){

DDRB=0xff;
DDRE=0xfe; 

TCCR1A=0xAA; 
TCCR1B=0x1A; 
OCR1A=3000; 
OCR1B=3000; 
ICR1=47999; 

DDRE=0b11111110; 
UCSR0A=0x00;
UCSR0B=0x98;
(*(unsigned char *) 0x95)=0x06;
(*(unsigned char *) 0x90)=0x00;
UBRR0L=0x67; 

Set_INI();
#asm("sei")
while(1){  

if(PcData=='L'){ 
PORTB.0=0;
Yaw_angle=Yaw_angle-1;
Servo_Yaw(Yaw_angle); 
PcData=0;
}       

if(PcData=='R'){   
PORTB.0=0;
Yaw_angle=Yaw_angle+1;
Servo_Yaw(Yaw_angle); 
PcData=0;
}   

if(PcData=='U'){ 
PORTB.0=0;
Pitch_angle=Pitch_angle-1;
Servo_Pitch(Pitch_angle); 
PcData=0;
}   

if(PcData=='D'){ 
PORTB.0=0;
Pitch_angle=Pitch_angle+1;
Servo_Pitch(Pitch_angle);
PcData=0;
}  

if(PcData=='S'){
PORTB.0=1;
delay_ms(500);
PORTB.0=0;
delay_ms(200);
PcData=0;
}

};
}
