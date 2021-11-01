/*
 */
#include "Registers.h"
#include "Std_Types.h"
#include "DIO.h"
#include "Macros.h"
#define LED_PORT 0 
#define LED_PIN  0
#define TIMER_REGISTER TCNT1
#define OVER_FLOW_TICKS 65536
#define CPU_CLK_IN_KHZ 8000
volatile uint32 Number_Of_Counts = 0;

int main(void)
{

	DIO_Vid_Set_Pin_Direcition(LED_PORT,LED_PIN,OUTPUT);
	DIO_Vid_Set_Pin_Value(LED_PORT,LED_PIN,0);
    TIMER_init ();
    while(1)
    {
		TIMER_start();
		if (Timer1_Calculate_Time_ms(TIMER_get()) > 50)
		{
			TIMER_stop();
			TOGGLE_BIT(PORTA,LED_PIN);
			TIMER_set();
		}
	}

    return 0;
}

uint32 TIMER_get()
{
	uint32 total_ticks = 0x0;
	/* calculate total number of ticks*/
	total_ticks = (OVER_FLOW_TICKS * Number_Of_Counts) + TIMER_REGISTER;

	/* clear variables for new usage*/
	Number_Of_Counts = 0x0;
	TIMER_REGISTER = 0x0;


	return total_ticks;
}


float32 Timer1_Calculate_Time_ms(uint32 total_ticks)
{
	/* calculate time in milliseconds */
	float32 time_ms =  (float32)total_ticks / CPU_CLK_IN_KHZ;
	return time_ms;
}

void TIMER_start()
{
    TCCR1B=(1<<CS10);
}

void TIMER_stop()
{
    TCCR1B=0x0;
}
void TIMER_set()
{
    Number_Of_Counts = 0x0;
    TCNT1 = 0x0;
}
void TIMER_init ()
{
    	/* Set TCNT1 Register = 0 */

	TCNT1 = 0x0;

	/* Normal port operation, OC1A/OC1B disconnected Normal Waveform Generation Mode */
	TCCR1A = 0x00;
	TCCR1B = 0x00;

	/* Clear interrupt flag */
	SET_BIT(TIFR,TOV1);

	/*  Enable TOIE1 Interrupt */
	SET_BIT(TIMSK,TOIE1);

	/* Enable Global Interrupt */
	SET_BIT(SREG,7);

}
#define __INTR_ATTRS used,externally_visible
void __vector_9(void)__attribute__((signal,__INTR_ATTRS));
void __vector_9(void)
{
	Number_Of_Counts ++;
}

