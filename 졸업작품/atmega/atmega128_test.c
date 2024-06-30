//=================
// CodeVisionAVR
//=================
#include <mega128.h>
#include <stdio.h>
#include <delay.h>
//====모터 포트 정의 
#define       Laser         PORTB.0
 
unsigned int Yaw_angle=90;
unsigned int i=0;

//=====서보모터 좌우 
void Servo_Yaw(int angle1){ // angle : -90도 ~ +90도
int i;
angle1=angle1-90;
if(angle1<-85)angle1=-85;
if(angle1> 85)angle1= 85;
 i=(angle1+ 90)*20 +1500;
 OCR1A=i; //PB5
}
//=====서보모터 상하 
void Servo_Pitch(int angle2){ // angle : -90도 ~ +90도
int j;
angle2=angle2-90;
if(angle2<-85)angle2=-85;
if(angle2> 85)angle2= 85;
 j=(angle2+ 90)*20 +1500;
 OCR1B=j; //PB6
}

//=====================================================================//
// USART0 
//=====================================================================//
void TX0_char(char c){ while(!(UCSR0A&0x20)); UDR0=c; }
//char RX0_CHAR(void){ while(!(UCSR0A&0x80)); return UDR0; }      // 수신함수
void TX0_STR(char *s){ while(*s)TX0_char(*s++); }

//cp로 부터 자료가 들어오면 인터럽트 발생 들어온 자료를 Bluetooth_Data변수에 저장
interrupt [USART0_RXC] void usart0_rx_isr(void)     
{ // 수신 인터럽트
    //Bt_Data=UDR0; 
}

void Set_INI(void)
{                           
Servo_Yaw(Yaw_angle);
Laser = 0;
        
delay_ms(1000);
}

//==================================================================
// Main Routinue
//==================================================================
void main(void){

DDRB=0xff;
DDRE=0xfe; // PE3 out 

//RC서보모터
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
DDRE=0b11111110; // RXD0=PORTE.0=입력으로설정, TXD0=PORTE.1=출력으로설정
UCSR0A=0x00;
UCSR0B=0x98;
UCSR0C=0x06;
UBRR0H=0x00;
UBRR0L=0x67; //9600


Set_INI();
// Global 인터럽트 인에이블
#asm("sei")
//========
// 본문 시작
//========    
    while(1){     
        
        Laser=1;

        for(i=0;i<60;i++){   
            Yaw_angle=Yaw_angle+1;
            Servo_Yaw(Yaw_angle);
            delay_ms(50);
        }     
        
        Laser=0;     
        
        for(i=0;i<60;i++){   
            Yaw_angle=Yaw_angle-1;
            Servo_Yaw(Yaw_angle);
            delay_ms(50);
        }   
        
        Yaw_angle=90;
        Servo_Yaw(Yaw_angle);     
        
        delay_ms(1000);

    };
}