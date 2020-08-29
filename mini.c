#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>

unsigned int clock_second1 =0;
unsigned int clock_second2 =0;
unsigned int clock_minute1=0;
unsigned int clock_minute2=0;
unsigned int clock_hour1=0;
unsigned int clock_hour2=0;


void INT0_Init(void)
{
	SREG  &= ~(1<<7);
	DDRD &=~(1<<PD2);
	PORTD |= (1<<PD2);
	MCUCR |=(1<<ISC01);
	GICR |=(1<<INT0);
	SREG |= (1 << 7);


}
ISR(INT0_vect){
	clock_second1 =0;
	clock_second2 =0;
     clock_minute1=0;
    clock_minute2=0;
	 clock_hour1=0;
	 clock_hour2=0;
}

void INT1_Init(void){
	SREG  &= ~(1<<7);
	DDRD &=~(1<<PD3);
	MCUCR |=(1<<ISC11)|(1<<ISC10);
	GICR |=(1<<INT1);
	SREG |= (1 << 7);

}
ISR(INT1_vect){
	TCCR1B&= ~(1<<CS12)& ~(1<<CS10)&~(1<<CS11);
}

void INT2_Init(void)
{
	SREG  &= ~(1<<7);
	DDRB &=~(1<<PB2);
	PORTB |= (1<<PB2);
	MCUCSR |=(1<<ISC2);
	GICR |=(1<<INT2);
	SREG |= (1 << 7);


}
ISR(INT2_vect){

	TCCR1B|=(1<<CS12)|(1<<CS10);
}

void INIT_timer1(){

		TIMSK |= (1 << OCIE1A);
		SREG |= (1 << 7);
		TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
		OCR1A = 1000;
	    TCCR1A |= (1 << FOC1A) | (1 << FOC1B);



}
ISR (TIMER1_COMPA_vect){
	clock_second1++;
    if (clock_second1==10)
	    {
    	 clock_second2++;
           clock_second1=0;
	    }
	 if (clock_second2==6)
	    {
	    	clock_minute1++;
	    	clock_second2=0;
	    }
    if (clock_minute1==10)
	        {
	        	clock_minute2++;
	        	clock_minute1=0;
	        }

	  if (clock_minute2==6)
	    {
	    	clock_hour1++;
	    	clock_minute2=0;
	    }
	  if (clock_hour1==10)
	        {
	        	clock_hour2++;
	        	clock_hour1=0;
	        }
	  if (clock_hour2==2 &&clock_hour1==4 )
	    {
	    	clock_hour2=0;
	    	clock_hour1=0;
	    	clock_minute2=0;
	    	clock_minute1=0;
	    	clock_second1=0;
	    	clock_second2=0;
	    }
}

int main()
{
	DDRA |=(0x3F);
	DDRC |=(0x0F);
	PORTA &=~(0x3F);
	PORTC &=(0XF0);

	INIT_timer1();
	INT0_Init();
	INT1_Init();
	INT2_Init();

	while(1)
	{
		PORTA = (1<<PA0);
		PORTC = (clock_second1);
		_delay_ms(2);

        PORTA =(1<<PA1);
        PORTC =(clock_second2);
		_delay_ms(2);

        PORTA =(1<<PA2);
        PORTC = (clock_minute1);
       _delay_ms(2);

        PORTA =(1<<PA3);
        PORTC =  (clock_minute2);
		_delay_ms(2);

        PORTA =(1<<PA4);
        PORTC =  (clock_hour1);
		_delay_ms(2);

        PORTA = (1<<PA5);
        PORTC =(clock_hour2);
        _delay_ms(2);
}
}
