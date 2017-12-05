#include <msp430.h> 
#include "System.h"


/*
 * main.c
 *
 *  Created on: Nov 10, 2017
 *      Author: Diogo Pina Manique
 */

#define RED_LED BIT0
#define GREEN_LED BIT6

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	System_HWInitialization();

	_enable_interrupts();

	// Set LPM3 (CPU, MCLK, SMCLK, DCO, DC generator disabled. ACLK enabled)
	_low_power_mode_3();

	return 0;
}

// TimerA0 Vector 0 -> CCR0 interrupts
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0A0_ISR(){

        //P1OUT ^= RED_LED;
}

// TimerA0 Vector 1 -> CCR1, CCR2 and Overflow interrupts
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0A1_ISR(){

    switch(__even_in_range(TA0IV, TA0IV_TAIFG)){ // switch on TA0 Interrupt Vector from 0 to TAIFG (10)
        case TA0IV_NONE:
            break; // No interrupt
        case TA0IV_TACCR1: // CCR1 interrupt
            P1OUT ^= GREEN_LED;
            break;
        case TA0IV_TACCR2:
            break; // CCR2 interrupt
            // 6, 8 reserved
        case TA0IV_TAIFG: // Overflow - quando TAR (registo contagem do timer) passa do valor máximo (0xFFFF quando em modo continuo, valor em CCR0 para os outros) para 0 - Colocar flag TAIE para activar este interrupt
            break;
    }
}

/* Modo up-down
 *
 * __CCR0__________________ upper bound
 *           /\      /\
 * __CCR1___/__\ __ /__\___
 *         /    \  /    \
 * __0____/______\/______\_
 * L2 acende -> L1 acende -> L2 apaga-> L2 acende -> L1 apaga -> L2 apaga
 * LED1->CCR0
 * LED2->CCR1
 *
 */
