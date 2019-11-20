#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC2_PLL3// System Clock Postscaler Selection bits ([Primary Oscillator Src: /2][96 MHz PLL Src: /3])
#pragma config FOSC = XTPLL_XT  // Oscillator Selection bits (XT oscillator, PLL enabled (XTPLL))
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

#define _XTAL_FREQ 32000000UL   //Para que el compilador sepa la frecuencia de xtal

#include <xc.h>
#include "LCD.h"
#include "Serial.h"

int indice = 0;
unsigned int w;
unsigned char recibido[100];


void main(void) {
    arrancaLCD();
    uc_config();
    while(1){
        
    }
}

void send_LCD(void){
    if(indice<16){
            POS_CURSOR(1,0);
            for (w=0;w<indice;w++){
//                if(w==2){
//                ENVIA_CHAR(recibido[w]-0x02);}
//                else{
                ENVIA_CHAR(recibido[w]);}
//            }
        }
        else if((indice-1)<32){
            POS_CURSOR(1,0);
            unsigned char i = 0;
            for(i = 0; i<16; i++){
                ENVIA_CHAR(recibido[i]);
            }
            POS_CURSOR(2,0);
            for(i = 16; i<indice; i++){
                ENVIA_CHAR(recibido[i]);
            }
        }
        else{
            int ran = (indice+1);
            for(int b=0;b<ran;b++){
                POS_CURSOR(1,0);
                unsigned char i = 0;
                for(i = 0; i<16; i++){
                    if((i+b)<=indice-1){
                        ENVIA_CHAR(recibido[(i+b)]);
                    }
                    else{
                        ENVIA_CHAR(0x20);   
                    }
                }
                POS_CURSOR(2,0);
                for(i = 16; i<32; i++){
                    if((i+b)<=indice-1){
                        ENVIA_CHAR(recibido[(i+b)]);
                    }
                    else{
                        ENVIA_CHAR(0x20);   
                     }
                }
                if(b+1<ran){
                    __delay_ms(500);
                }
                else{
                    __delay_ms(2000);
                }
            }
        }
}

void clear(void){
    for(int i = 0;i<100;i++){
        recibido[i]=0x20;
    }
    indice = 0;
    BORRAR_LCD();
}

void __interrupt () RCIsr(void){
    PIR1bits.RC1IF = 0;
    if (RCREG == '}'){
        clear();
    }
    else if (RCREG == '`'){
        send_LCD();
    }
    else{
        __delay_us(150);
        recibido[indice] = RCREG;
        indice++;
    }
} 

