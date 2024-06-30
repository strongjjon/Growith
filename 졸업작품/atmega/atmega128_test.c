//=================
// CodeVisionAVR
//=================
#include <mega128.h>
#include <stdio.h>
#include <delay.h>
//====���� ��Ʈ ���� 
#define       Laser         PORTB.0
 
unsigned int Yaw_angle=90;
unsigned int i=0;

//=====�������� �¿� 
void Servo_Yaw(int angle1){ // angle : -90�� ~ +90��
int i;
angle1=angle1-90;
if(angle1<-85)angle1=-85;
if(angle1> 85)angle1= 85;
 i=(angle1+ 90)*20 +1500;
 OCR1A=i; //PB5
}
//=====�������� ���� 
void Servo_Pitch(int angle2){ // angle : -90�� ~ +90��
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
//char RX0_CHAR(void){ while(!(UCSR0A&0x80)); return UDR0; }      // �����Լ�
void TX0_STR(char *s){ while(*s)TX0_char(*s++); }

//cp�� ���� �ڷᰡ ������ ���ͷ�Ʈ �߻� ���� �ڷḦ Bluetooth_Data������ ����
interrupt [USART0_RXC] void usart0_rx_isr(void)     
{ // ���� ���ͷ�Ʈ
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

//RC��������
TCCR1A=0xAA; // FAST PWM
TCCR1B=0x1A; // 8����=0.5usec
OCR1A=3000; // 1500usec=0��
OCR1B=3000; // 1500usec=0��
ICR1=47999; // 0.5usec*48000=24000usec=41.67Hz

// UART0 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART0 Receiver: On
// USART0 Transmitter: On
// USART0 Mode: Asynchronous
// USART0 Baud Rate: 9600
DDRE=0b11111110; // RXD0=PORTE.0=�Է����μ���, TXD0=PORTE.1=������μ���
UCSR0A=0x00;
UCSR0B=0x98;
UCSR0C=0x06;
UBRR0H=0x00;
UBRR0L=0x67; //9600


Set_INI();
// Global ���ͷ�Ʈ �ο��̺�
#asm("sei")
//========
// ���� ����
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