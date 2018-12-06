/*******************************************************
Project : DC Motol Controller
Version : 1.0
Date    : 2018-12-04
Author  : A.Hatami
Company : Ateriad CGCR 

Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega16.h>
#include <alcd.h>
#include <stdio.h>
#include <delay.h>

void main(void)
{
    char command;
    char duty_cycle[16];
    int status;      
         
    // Ports configuration
    DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
    PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);
    DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (1<<DDB3) | (0<<DDB2) | (0<<DDB1) | (1<<DDB0);
    PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);
    DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
    PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);
    DDRD=(0<<DDD7) | (1<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
    PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);
                          
    // USART
    UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
    UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
    UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
    UBRRH=0x00;
    UBRRL=0x33;      
            
    // Timer
    TCCR0=(1<<WGM00) | (1<<COM01) | (0<<COM00) | (1<<WGM01) | (0<<CS02) | (1<<CS01) | (1<<CS00);
    TCNT0=0x00;
    OCR0=0x00;
    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

    lcd_init(16);       
    PORTB.0 = 0; 
    lcd_clear();
    lcd_puts("Welcome :D");
    status = 0;

    while (1)
    {
        command = getchar();
        
        if(command == '1')
        {       
            if(status == 0){    
                PORTB.0 = 1;
                lcd_clear();
                lcd_gotoxy(0, 0);
                lcd_puts("Power On");
                OCR0 = 160;
                sprintf(duty_cycle, "Speed : %d", OCR0);
                lcd_gotoxy(0, 1);
                lcd_puts(duty_cycle); 
                puts("Power On");
                status = 1;
            } 
        }
          
        if(command == '2')
        {     
            if(status == 1){          
                PORTB.0 = 0;
                lcd_clear();
                lcd_gotoxy(0, 0);
                lcd_puts("Power Off");  
                OCR0 = 0;   
                sprintf(duty_cycle, "Speed : %d", 0);
                lcd_gotoxy(0, 1);
                lcd_puts(duty_cycle); 
                puts(duty_cycle); 
                status = 0;
            }
        }
        
        if(command == '3')
        {                   
            if(OCR0 < 240) 
                OCR0 = OCR0 + 10;
            else
                OCR0 = 250;
            sprintf(duty_cycle, "Speed : %d", OCR0);
            lcd_gotoxy(0, 1);
            lcd_puts(duty_cycle); 
        }
        
        if(command == '4')
        {        
            if(OCR0 > 160) 
                OCR0 = OCR0 - 10;
            else
                OCR0 = 160;
            sprintf(duty_cycle, "Speed : %d", OCR0);
            lcd_gotoxy(0, 1);
            lcd_puts(duty_cycle); 
        }
    }
}
