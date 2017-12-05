

#include "System.h"
#include <msp430.h>

/*
 * System.c
 *
 *  Created on: Nov 10, 2017
 *      Author: Diogo Pina Manique
 */

#define ACLK_FREQ 25
#define TICK_DIVISOR_CCR0 ACLK_FREQ
#define TICK_DIVISOR_CCR1 (TICK_DIVISOR_CCR0*(99/100.0))
//#define TICK_DIVISOR_CCR0 50
//#define TICK_DIVISOR_CCR1 49

void setupPorts();
void setupTimerA();
void setupClock();

void System_HWInitialization(void){

    setupPorts();
    setupClock();
    setupTimerA();
}

void setupPorts(){

    // Port1 as output (1.0 red led, 1.6 green led)
    P1DIR = 0xFF;
    P1OUT = 0;


    // Unused Ports as output (outputting 0)
    P2DIR = 0xFF;
    P2OUT = 0;

    P3DIR = 0xFF;
    P3OUT = 0;

}

void setupClock(){

    // Config DCO as 1MHz
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;

    // Set VLO (12KHz) as source for ACLK
    BCSCTL3 |= LFXT1S_2;
}

void setupTimerA(){

    // CCR0 defines upper bound on up and up-down timer modes
    TACCR0 = TICK_DIVISOR_CCR0;
    TACCR1 = TICK_DIVISOR_CCR1;

    //source is ACLK, up-down mode
    TACTL = TASSEL_1 | MC_3 | TACLR; // Overflow interrupt -> | TAIE (sets TAIFG)

    // enable interrupts for CCR0 & CCR1
    TACCTL0 |= CCIE;
    TACCTL1 |= CCIE;
}


