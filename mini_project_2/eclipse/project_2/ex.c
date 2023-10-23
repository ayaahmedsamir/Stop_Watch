#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

unsigned char tick1=0;
unsigned char flag1;
unsigned char sec1=0;
unsigned char sec2=0;
unsigned char min1=0;
unsigned char min2=0;
unsigned char hr1=0;
unsigned char hr2=0;


ISR (TIMER1_COMPA_vect)
{

		sec1++;
		if (sec1==10)
		{
			sec2++;
			sec1=0;
		}
		if (sec2==6)
		{
			min1++;
			sec2=0;
			sec1=0;
		}
		if (min1==9)
		{
			min2++;
			min1=0;
			sec1=0;
			sec2=0;
		}
		if (min2==6)
		{
			hr1++;
			sec1=0;
			sec2=0;
			min1=0;
			min2=0;
		}
		if (hr1==9)
		{
			hr2++;
			hr1=0;
			min2=0;
			min1=0;
			sec1=0;
			sec2=0;
		}
}


ISR(INT0_vect)
{
	sec1=0;
	sec2=0;
	min1=0;
	min2=0;
	hr1=0;
	hr2=0;

}

ISR(INT1_vect)
{
	TCCR1B &= ~(1<<CS12);
	TCCR1B &= ~(1<<CS11);
	TCCR1B &= ~(1<<CS10);
}

ISR(INT2_vect)
{
	TCCR1B |= (1<<CS12);
	TCCR1B |= (1<<CS10);


}


void TIMER1_COMP(void) // function to trigger interrupt of timer1//
{

	TCCR1A = (1<<FOC1A);
	OCR1A = 1000;
	TCNT1 = 0;
	TIMSK = (1<<OCIE1A);
	SREG |= (1<<7);
	TCCR1B = (1<<CS12)|(1<<CS10)|(1<<WGM12);

}

void INT0_int (void) //function to trigger interrupt of INT0//
{
	MCUCR|= (1<<ISC01);
	GICR|=(1<<INT0);
	SREG|=(1<<7);
}
void INT1_int(void)
{
	MCUCR= (1<<ISC11)|(1<<ISC10);
	GICR|=(1<<INT1);
	SREG|=(1<<7);
}
void INT2_int(void)
{
	MCUCSR &= ~(1<<ISC2);
	GICR|=(1<<INT2);
	SREG|=(1<<7);
}


void display_7segment(void) // function to display numbers on seven segment//
{

	PORTC = (PORTC & 0xF0)  | sec1;
	PORTA =( PORTA & 0xC0 ) | (1<<0); // enable first pin in PORTA //
	_delay_ms(2);
	PORTA &=~(1<<0);

	PORTC = (PORTC & 0xF0)  | sec2;
	PORTA =( PORTA & 0xC0 ) | (1<<1); // enable second pin in PORTA //
	_delay_ms(2);
	PORTA &=~(1<<1);

	PORTC = (PORTC & 0xF0)  | min1;
	PORTA =( PORTA & 0xC0 ) | (1<<2); // enable third pin in PORTA //
	_delay_ms(2);
	PORTA &=~(1<<2);

	PORTC = (PORTC & 0xF0)  | min2;
	PORTA =( PORTA & 0xC0 ) | (1<<3); // enable fourth pin in PORTA //
	_delay_ms(2);
	PORTA &=~(1<<3);

	PORTC = (PORTC & 0xF0)  | hr1;
	PORTA =( PORTA & 0xC0 ) | (1<<4); // enable fifth pin in PORTA //
	_delay_ms(2);
	PORTA &=~(1<<4);

	PORTC = (PORTC & 0xF0)  | hr2;
	PORTA =( PORTA & 0xC0 ) | (1<<5); // enable sixth pin in PORTA //
	_delay_ms(2);
	PORTA &=~(1<<5);

}


int main (void)
{
	DDRC = DDRC | 0x0F; // make the first 4 pins in PORTC as output //
	DDRA = DDRA | 0x3F; // make the first 6 pins in PORTA as output //
	DDRD &= ~(1<<2); // make pin2 in PORTD as input//
	DDRD &= ~(1<<3); // make pin3 in PORTD as input (INT1)//
	DDRB &= ~(1<<2); // make pin2 in PORTB as input//



	PORTC = PORTC & 0xF0; // set the first 4 pins in PORTC to 0 //
	PORTA = PORTA | 0x3F; // set the first 6 pins in PORTA to 1 //
	PORTD = PORTD | 0x04; // activate internal resistor in PORTD at pin2//
	PORTB = PORTB | 0x04; // activate internal resistor in PORTB at pin2//

	TIMER1_COMP();
	INT0_int();
	INT1_int();
	INT2_int();

	while (1)
	{
		display_7segment();

	}
}
