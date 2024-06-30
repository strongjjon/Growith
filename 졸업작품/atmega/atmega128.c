#include <mega128.h>
#include <stdio.h>
#include <delay.h>

#define       Laser         PORTB.0
 
unsigned int Yaw_angle=85;      
unsigned int Pitch_angle=95;    

//=====통신중 수신받은 코드 값 저장
unsigned char PcData=0;

//=====서보모터 좌우
void Servo_Yaw(int angle_Yaw){ // angle : -90도 ~ +90도
int PB5;
angle_Yaw=angle_Yaw-90;
if(angle_Yaw<-85)angle_Yaw=-88;
if(angle_Yaw> 85)angle_Yaw= 88;
 PB5=(angle_Yaw+ 90)*20 +1500;
 OCR1A=PB5; //PB5       
}
//=====서보모터 상하
void Servo_Pitch(int angle_Pitch){ // angle : -90도 ~ +90도
int PB6;
angle_Pitch=angle_Pitch-90;
if(angle_Pitch<-85)angle_Pitch=-88;
if(angle_Pitch> 85)angle_Pitch= 88;
 PB6=(angle_Pitch+ 90)*20 +1500;
 OCR1B=PB6; //PB6           
}

//=============USART0 
interrupt [USART0_RXC] void usart0_rx_isr(void)     
{ 
    PcData=UDR0; 
}
//==============
void Set_INI(void)
{                           
Servo_Yaw(Yaw_angle);
Servo_Pitch(Pitch_angle);
Laser = 0;
        
delay_ms(1000);
}

//==============
// Main Routinue
void main(void){

DDRB=0xff;
DDRE=0xfe; // PE3 out 

//서보모터
TCCR1A=0xAA; // FAST PWM
TCCR1B=0x1A; // 8분주=0.5usec
OCR1A=3000; // 1500usec=0도
OCR1B=3000; // 1500usec=0도
ICR1=47999; // 0.5usec*48000=24000usec=41.67Hz

// UART0 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART0 Receiver: On
// USART0 Transmitter: On
// USART0 Mode: Asynchronous
// USART0 Baud Rate: 9600
DDRE=0b11111110; // RXD0=PORTE.0=입력으로 설정, TXD0=PORTE.1=출력으로 설정
UCSR0A=0x00;
UCSR0B=0x98;
UCSR0C=0x06;
UBRR0H=0x00;
UBRR0L=0x67; //9600

Set_INI();
// Global 인터럽트 인에이블
#asm("sei")
//========
// 본문시작
//========    
    while(1){  
    
        if(PcData=='L'){ 
         Laser=0;
         Yaw_angle=Yaw_angle-1;
         Servo_Yaw(Yaw_angle); 
         PcData=0;
        }       
        
        if(PcData=='R'){   
         Laser=0;
         Yaw_angle=Yaw_angle+1;
         Servo_Yaw(Yaw_angle); 
         PcData=0;
        }   
         
        if(PcData=='U'){ 
         Laser=0;
         Pitch_angle=Pitch_angle-1;
         Servo_Pitch(Pitch_angle); 
         PcData=0;
        }   
        
        if(PcData=='D'){ 
         Laser=0;
         Pitch_angle=Pitch_angle+1;
         Servo_Pitch(Pitch_angle);
         PcData=0;
        }  
        
        if(PcData=='S'){
         Laser=1;
         delay_ms(500);
         Laser=0;
         delay_ms(200);
         PcData=0;
        }
        
    };
}