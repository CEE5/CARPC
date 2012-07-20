/*


 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int test = 0;
int LED1=0;
int LED2=0;
int exit=0;



void displayZu(){
    int i;
      for(i=0;i<150;i++){
          PORTB |=(1<<PB7);
          _delay_us(550);
          PORTB &= ~(1<<PB7);

          PORTB |=(1<<PB6);
          _delay_us(2400);
          PORTB &= ~(1<<PB6);

          _delay_ms(20);
      }


}

void displayAuf(){
     int i;
     for(i=0;i<150;i++){
          PORTB |=(1<<PB7);
          _delay_us(2400);
          PORTB &= ~(1<<PB7);

          PORTB |=(1<<PB6);
          _delay_us(550);
          PORTB &= ~(1<<PB6);
          _delay_ms(20);
      }


}

void harakiri(){

    displayZu();


    //AKKU aus
    PORTD &=~(1<<PD7);

}

ISR(INT0_vect)
{
    cli();
    GICR &= ~(1<<INT0);
    sei();

    if(LED1==0)
    {
        PORTB |=(1<<PB1);
        LED1 =1;
    }
    else
    {
        PORTB &= ~(1<<PB1);
        LED1 =0;
    }

    test++;



    switch(test)
    {

    case 1:
        PORTB |=(1<<PB0);
        break;

    case 2:
        PORTD |=(1<<PD7);
        break;

    case 3:
        PORTD |=(1<<PD6);
        break;

    case 4:
        PORTD |=(1<<PD5);
        break;

    case 6:
        harakiri();
    break;
    default:
        ;
    }
    _delay_ms(5);

    cli();
    GICR |= (1<<INT0);
    sei();
}

ISR(INT1_vect )
{
    cli();
    GICR &= ~(1<<INT1);
    sei();
    _delay_ms(1);


    if(LED2==0)
    {
        PORTB |=(1<<PB0);
        LED2 =1;
        displayAuf();
    }
    else
    {
        PORTB &= ~(1<<PB0);
        LED2 =0;
        displayZu();
    }



    _delay_ms(5);
    cli();
    GICR |= (1<<INT1);
    sei();
}


void init_TIMER(){

    /**
                  --Timer0--             --Timer1--           --Timer2--

    /1                     CS00                 CS10                     CS20
    /8               CS01                CS11                   CS21
    /32                                                         CS21    CS20
    /64              CS01  CS00          CS11   CS10     CS22
    /128                                                 CS22            CS20
    /256        CS02                CS12                 CS22    CS21
    /1024       CS02       CS00     CS12        CS10     CS22    CS21    CS20

    */

     // Timer 0 konfigurieren
    TCCR1B = (1<<CS12)|(1<<CS10);
    TCNT1 = 0xffff-3906;

    // Overflow Interrupt erlauben
    //TIMSK |= (1<<TOIE1);


}

ISR(TIMER1_OVF_vect)
{
    //1 Sekunde
    TCNT1 = 0xffff-3906;


     if(LED1==0)
    {
        PORTB |=(1<<PB1);
        LED1 =1;
    }
    else
    {
        PORTB &= ~(1<<PB1);
        LED1 =0;
    }

//POWER-BUTTON ||----------
    if(exit==100){

        PORTD |=(1<<PD5);

    }
    if(exit == 102){
        PORTD &= ~(1<<PD5);

    }
//POWER-BUTTON ---------||


    if(exit >=200){
        harakiri();
    }
    else{
        exit++;
    }
}


int main(void)
{
    //Taster PULL-UP
    PORTD |=((1<<PD2)|(1<<PD3));

    //LEDs
    DDRB |=((1<<PB0)|(1<<PB1));



    //RELAISs
    DDRD |= ((1<<PD7)|(1<<PD6)|(1<<PD5));

    //SERVO
    DDRB |= ((1<<PB6)|(1<<PB7));




    //INTERRUPTS
    MCUCR |=(1<<ISC11); //int1 falling
    MCUCR |=(1<<ISC01); // int0 falling
    GICR |= ((1<<INT1) |(1<<INT0)); // exInt enable int0 & int1


    //AKKU nutzen
    PORTD |=(1<<PD7);

    displayAuf();


//POWER-BUTTON
    int h;
    PORTD |=(1<<PD5);
    for(h=0;h<20;h++){
        _delay_ms(100);

    }
    PORTD &= ~(1<<PD5);

    init_TIMER();


    sei();

uint8_t in;
    while(1)
    {

        in = PINC;
        if(!(in & (1<<PC5))){
            if(!(TIMSK &(1<<TOIE1))){
                TIMSK |= (1<<TOIE1);
            }
        }
        else{
            TIMSK &= ~(1<<TOIE1);
        }




    }

    return 0;
}

