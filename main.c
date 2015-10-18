#include <msp430.h> 


#pragma vector=TIMER0_A0_VECTOR
__interrupt void timerA(void) {
	P1OUT ^= BIT0;

}



#pragma vector=PORT1_VECTOR        // throw code for button press timer in this interupt
__interrupt void p1(void){
	if(P1IFG & BIT3 & P1IES)
	{
		P1IES ^= BIT3;		// toggle the interrupt edge
		P1IFG &= ~BIT3;    // clear the bit3 interrupt flag
		TA0CCR0 = 2500; 	//triger timer every .5 secs
		TAR = 0;

	}
	else if(P1IFG & BIT3 & ~P1IES)
	{
		P1IES ^= BIT3;
		P1IFG &= ~BIT3;
		TA0CCR0 = 0;
		TAR = 0;

		if(P1OUT & BIT0)  //
		{					//
			P1OUT ^= BIT0;	//
		}					//
	}

}


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	const unsigned char XIN = BIT6;
	const unsigned char XOUT = BIT7;
    //Set up unused pins
    P1SEL = 0;
    P1DIR = 0xFF;
    P1OUT = 0;

    //  P2SEL = 0;
    //  P2DIR = 0xFF;
    //  P2OUT = 0;

	P2DIR = ~XIN;
	P2OUT = 0;
	P2SEL = XIN | XOUT;



	//Set up the crystal oscillator as the clock
	//ACLK divider is 0
	BCSCTL1 |= DIVA_0;
	//12.5pF cap for the crystal oscillator
	BCSCTL3 |= XCAP_3;


	//Set to mode 1 (count up to TACCR0), divide by 8, input is ACLK, interrupt enabled
	TA0CTL = MC_1 | TASSEL_1 | ID_3;

	TA0CCTL0 |= CCIE;

    //Set up the push button as input
    P1DIR &= ~BIT3;
    //Turn on the pull up resistor
    P1REN |= BIT3;
    P1OUT |= BIT3;


    P1DIR |= BIT0;


    P1IES &= BIT3;		//set up interrupt on falling edge
    P1IFG &= ~BIT3;
    P1IE |= BIT3;		//this selects the button as the interupt



    __enable_interrupt();


	
	return 0;
}
