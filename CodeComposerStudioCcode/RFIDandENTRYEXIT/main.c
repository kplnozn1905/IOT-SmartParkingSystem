#include "MFRC522_1_0_1.h"
#include "msp430G2553.h"
#include "CHECK_ID_1_0_1.h"
#include "USCI_UART_1_0_1.h"
#include "CLOCK_1_0_1.h"
#include "GPIO_1_0_1.h"
#include "USCI_SPI_1_0_1.h"
#include "WATCHDOG_1_0_1.h"

/* Variables */
unsigned char arrID[5];
unsigned char arr[100];
unsigned char varStatus;
int i;
int a=0;
char c;
/****************************************************************************************
 *  MAIN ROUTINE
 ***************************************************************************************/
void main (void)
{
    /* SETUP */
    //-CLOCK & WDT-//
    clkSetup(CLK_DCO_1MHZ);
    wdtSetup(WDT_OFF, WDT_TIME_8MS);

    //-GPIO-//

    //-UART-//

   // P1OUT=0x00;
    P1DIR &=~(BIT3+BIT4);
      P1OUT |=(BIT3+BIT4);
      P1REN|=(BIT3+BIT4);
      P2DIR |=BIT1;//p2.1oldu
            P2OUT |=BIT1;
            P2SEL =0x02;
            P1SEL=(BIT1+BIT2);//P1.1 = RXD, P1.2=TXD
              P1SEL2=(BIT1+BIT2);//P1.1 = RXD, P1.2=TXD

              UCA0CTL1|=UCSSEL_2;//UART Ayarlarý, Saat kaynaðý SMCLK//UART Settings, Clock source SMCLK
              UCA0BR0=104;//1MHz 9600 bud ayarý///1MHz 9600 bud tuning
              UCA0BR1=0;// 1MHz 9600 baud ayarý///1MHz 9600 bud tuning
              UCA0MCTL=UCBRF_0+UCBRS_1;// UART Baud hassas ayar// UART Baud fine tuning
              UCA0CTL1&=~UCSWRST;// USCI birimini hazýrla// prepare the USCI volume
              IE2|=UCA0RXIE;// USCI_A0 RX kesmesini aç// turn on USCI_A0 RX interrupt


    TA1CCR0 = 20000;
      TA1CCR1 =1600;
      TA1CCTL1 = OUTMOD_7;
      TA1CTL = TASSEL_2|MC_1;


    //-SPI-//
    spiB0Setup(SPI_MASTER, 2, INT_SPIB0_RX_OFF, INT_SPIB0_TX_OFF);

    //-MFRC522-//
    rfidSetup(RFID_PORT_2, BIT0, BIT2);
    __delay_cycles(1000000);

    //IE2|=UCA0RXIE;

//---------------------------------------------------------------------------------------
    /* PROCESS */
       while(1)

       {

           if( !(P1IN&BIT3))//Is there a car at the entrance
           {
               if(a==0){
                   UCA0TXBUF ='a';//there is a car at the entrance for  led

               a=1;

               }


                  varStatus = rfidGetCardID(arrID);
                             //-Process signal-//
                                if (varStatus == MI_OK)
                                {

                                    txdToStr(arrID[1]);

                                }
           }
           else if((P1IN&BIT3)){
               a=0;
           }
          if( !(P1IN&BIT4))//Is there a car at the exit
           {
               //UCA0TXBUF ='b';//Data transmission for led when car comes to the output
               txdStr("b");
           }

          __delay_cycles(1000000);
           __bis_SR_register(GIE);
       }

}
#pragma vector=USCIAB0RX_VECTOR
      __interrupt void isr(void)
      {

        if(UCA0RXBUF=='3')
                     {
                        if(TA1CCR1 ==1600){
                            for (i=1600; i>=650; i--) {
                           TA1CCR1 = i;
                          __delay_cycles(400);

                       }}
                        else if(TA1CCR1 ==650){
                              for (i=650; i<=1600; i++)
                              {
                                   TA1CCR1 = i;
                                    __delay_cycles(400);

                               }}

                 }
        else if(UCA0RXBUF=='4')
                             {
                                if(TA1CCR1 ==1600){
                                    for (i=1600; i>=650; i--) {
                                   TA1CCR1 = i;
                                  __delay_cycles(200);

                               }}
                                else if(TA1CCR1 ==650){
                                      for (i=650; i<=1600; i++)
                                      {
                                           TA1CCR1 = i;
                                            __delay_cycles(200);

                                       }}

                         }
        else if(UCA0RXBUF=='5')
                             {
                                if(TA1CCR1 ==1600){
                                    for (i=1600; i>=650; i--) {
                                   TA1CCR1 = i;
                                  __delay_cycles(20);

                               }}
                                else if(TA1CCR1 ==650){
                                      for (i=650; i<=1600; i++)
                                      {
                                           TA1CCR1 = i;
                                            __delay_cycles(20);

                                       }}

                         }


}



