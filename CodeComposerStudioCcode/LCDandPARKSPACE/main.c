#include <msp430.h>
#include "LCD.h"




int x;//Park yeri sayýsý// Number of parking spaces
int i;//Bariyerlerin açýlýþ hýzý//opening speed of barriers
int a=0;//Kapalý parka girildiðinde LCD de yazan yazýlarýn ayarlanmasý.// Adjusting the texts written on the LCD when entering the closed parking lot.
int c=0;//Otopark isminin gelen yazýlardan sonra tekrar getirmek için kullanýlan deðiþken.// The variable used to bring back the name of the parking lot after the incoming texts.
int d=0;//Giriþ yapan araç için park yerlerinin durumunu göstermek içi 1 yapýlýr.// Set to 1 to show the status of the parking spaces for the vehicle entering.

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;//Watchdog timeri durdur.// Stop the watchdog timer.

  if (CALBC1_1MHZ == 0xFF)
  {
    while(1);//wait till the processor is fully calibrated//iþlemci tamamen kalibre edilene kadar bekleyin
  }

  BCSCTL1 = CALBC1_1MHZ;//Dahili osilatörü 1MHz'e ayarla//set the internal oscillator to 1MHz
  DCOCTL = CALDCO_1MHZ;

  P2DIR =0xEA;//P2.0-2-4 portlarýný giriþ yaptýk.// We have entered the P2.0-2-4 ports.
  P2OUT =0x15;//Giriþ portlarýný low olarak  ayarladýk.// We set the input ports to low.
  P2REN=0x15;//Enable resistor on  P2.0-2-4//P2.0-2-4'te direnci etkinleþtir
  P2DIR |=BIT1;//p2.1 portunu giriþ yaptýk oldu// we have entered the p2.1 port
      P2OUT |=BIT1;
      P2SEL =0x02;//P2.1 Timer olarak ayarlanýyor.// It is set to P2.1 Timer.
  P1DIR &=~BIT3;//P1.3 portunu giriþ yaptýk// We entered the P1.3 port
  P1OUT |=BIT3;//Giriþ portunu low olarak  ayarladýk.//We set the input port to low.
  P1REN|=BIT3;//Enable resistor on  P1.3//Enable resistor on P1.3



  P1SEL=(BIT1+BIT2);//P1.1 = RXD, P1.2=TXD
  P1SEL2=(BIT1+BIT2);//P1.1 = RXD, P1.2=TXD

  UCA0CTL1|=UCSSEL_2;//UART Ayarlarý, Saat kaynaðý SMCLK//UART Settings, Clock source SMCLK
  UCA0BR0=104;//1MHz 9600 bud ayarý///1MHz 9600 bud tuning
  UCA0BR1=0;// 1MHz 9600 baud ayarý///1MHz 9600 bud tuning
  UCA0MCTL=UCBRF_0+UCBRS_1;// UART Baud hassas ayar// UART Baud fine tuning
  UCA0CTL1&=~UCSWRST;// USCI birimini hazýrla// prepare the USCI volume
  IE2|=UCA0RXIE;// USCI_A0 RX kesmesini aç// turn on USCI_A0 RX interrupt

  //PWM ayarlarý//PWM settings
  TA1CCR0 = 20000;
  TA1CCR1 =1600;
  TA1CCTL1 = OUTMOD_7;
  TA1CTL = TASSEL_2|MC_1;

  LCD_Ayarla();
  __delay_cycles(100000);
      LCD_Temizle();
          LCD_Git_XY(1,1); LCD_Yazi_Yaz("TIGER SMART PARK");
          LCD_Git_XY(2,5); LCD_Yazi_Yaz("WELCOME");c++;
        __bis_SR_register(GIE);
while(1){
    LCD_Ayarla();
    if(c==0){
        LCD_Temizle();
        LCD_Git_XY(1,1); LCD_Yazi_Yaz("TIGER SMART PARK2");
        LCD_Git_XY(2,5); LCD_Yazi_Yaz("WELCOME");
        c++;
    }

    //Girilen x deðerine göre park yerleri açýlýyor.
    // Parking spaces are opened according to the entered x value.
    switch(x){

    case 4:   if((P1IN&BIT3)){UCA0TXBUF ='A';//Engelin önü kapalý deðilse tx=A//If the obstacle is not blocked tx=A
                if(d==1){LCD_Git_XY(1,1);LCD_Yazi_Yaz("P1:E");}//d=0 ile baþlýyor kart okuma iþlemlerinden d=1 oluyor ve LCD de park yeri durumu yazýlýyor.
                // starts with d=0, becomes d=1 from card reading operations, and the parking lot status is written on the LCD.
              }
              else if(!(P1IN&BIT3)){UCA0TXBUF ='B';//Engelin önü kapalý ise tx=B ve// If the obstacle is closed, tx=B and
              if(d==1){LCD_Git_XY(1,1);LCD_Yazi_Yaz("P1:F");}
              }
            __delay_cycles(150000);
              if((P2IN&BIT0)){UCA0TXBUF ='C';
              if(d==1){LCD_Git_XY(1,6);LCD_Yazi_Yaz("P2:E GOOD");}
              }
              else if(!(P2IN&BIT0)){UCA0TXBUF ='D';
              if(d==1){LCD_Git_XY(1,6);LCD_Yazi_Yaz("P2:F GOOD");}
              }
              __delay_cycles(150000);
              if((P2IN&BIT2)){UCA0TXBUF ='E';
              if(d==1){LCD_Git_XY(2,1);LCD_Yazi_Yaz("P3:E");}
              }
              else if(!(P2IN&BIT2)){UCA0TXBUF ='F';
              if(d==1){LCD_Git_XY(2,1);LCD_Yazi_Yaz("P3:F");}
              }
              __delay_cycles(150000);
              if((P2IN&BIT4)){UCA0TXBUF ='G';
              if(d==1){LCD_Git_XY(2,6);LCD_Yazi_Yaz("P4:E PARKS");}
              }
              else if(!(P2IN&BIT4)){ UCA0TXBUF ='H';
              if(d==1){LCD_Git_XY(2,6);LCD_Yazi_Yaz("P4:F PARKS");}
              }
              __delay_cycles(150000);
               break;
    case 3:
        if(!(P1IN&BIT3)&&(a==0)){
            LCD_Temizle();
            LCD_Git_XY(1,1);LCD_Yazi_Yaz("PARKING'S CLOSED");
             LCD_Git_XY(2,1);LCD_Yazi_Yaz("PLEASE RELOCATE...");
                a=1;
            }
        else if((P1IN&BIT3)&&a==1) {
        LCD_Temizle();
        LCD_Git_XY(1,1);LCD_Yazi_Yaz("TIGER SMART PARK");
        LCD_Git_XY(2,5);LCD_Yazi_Yaz("WELCOME");
                a=0;
        }

        if((P2IN&BIT0)){UCA0TXBUF ='C';
        if(d==1){LCD_Git_XY(1,1);LCD_Yazi_Yaz("P1:C P2:E GOOD");}
        }
        else if(!(P2IN&BIT0)){UCA0TXBUF ='D';
        if(d==1){LCD_Git_XY(1,1);LCD_Yazi_Yaz("P1:C P2:F GOOD");}
        }
        __delay_cycles(150000);
        if((P2IN&BIT2)){UCA0TXBUF ='E';
        if(d==1){LCD_Git_XY(2,1);LCD_Yazi_Yaz("P3:E");}
        }
        else if(!(P2IN&BIT2)){UCA0TXBUF ='F';
        if(d==1){LCD_Git_XY(2,1);LCD_Yazi_Yaz("P3:F");}
        }
        __delay_cycles(150000);
        if((P2IN&BIT4)){UCA0TXBUF ='G';
        if(d==1){LCD_Git_XY(2,6);LCD_Yazi_Yaz("P4:E PARKS");}
        }
        else if(!(P2IN&BIT4)){ UCA0TXBUF ='H';
        if(d==1){LCD_Git_XY(2,6);LCD_Yazi_Yaz("P4:F PARKS");}}

        __delay_cycles(150000);
         break;
    case 2:
        if(!((P1IN&BIT3)&&(P2IN&BIT0))&&(a==0)){
                    LCD_Temizle();
                    LCD_Git_XY(1,1);LCD_Yazi_Yaz("PARKING'S CLOSED");
                                 LCD_Git_XY(2,1);LCD_Yazi_Yaz("PLEASE RELOCATE...");
                        a=1;
                    }
                else if((P1IN&BIT3)&&(P2IN&BIT0)&&(a==1)){
                    LCD_Temizle();
                LCD_Git_XY(1,1); LCD_Yazi_Yaz("TIGER SMART PARK");
                        LCD_Git_XY(2,5); LCD_Yazi_Yaz("WELCOME");
                        a=0;
                }
                if((P2IN&BIT2)){UCA0TXBUF ='E';
                if(d==1){LCD_Git_XY(1,1);LCD_Yazi_Yaz("P1:C P2:C GOOD");
                LCD_Git_XY(2,1);LCD_Yazi_Yaz("P3:E");
                }
                }
                else if(!(P2IN&BIT2)){UCA0TXBUF ='F';
                if(d==1){LCD_Git_XY(1,1);LCD_Yazi_Yaz("P1:C P2:C GOOD");
                LCD_Git_XY(2,1);LCD_Yazi_Yaz("P3:F");}
                }
                __delay_cycles(150000);
                if((P2IN&BIT4)){UCA0TXBUF ='G';
                if(d==1){LCD_Git_XY(2,6);LCD_Yazi_Yaz("P4:E PARKS");}
                }
                else if(!(P2IN&BIT4)){ UCA0TXBUF ='H';
                if(d==1){LCD_Git_XY(2,6);LCD_Yazi_Yaz("P4:F PARKS");}
                }
                __delay_cycles(150000);
                 break;
    default:break;
    }






}
}
//Gönderilen verilere göre yapýlan iþlemler
// Operations based on the sent data
#pragma vector=USCIAB0RX_VECTOR
      __interrupt void isr(void)
      {

          if(UCA0RXBUF=='a'){
                        LCD_Temizle();
                        LCD_Git_XY(1,1); // Kursörü 1.satýr 1.sütuna götür.// Move the cursor to row 1, column 1.
                        LCD_Yazi_Yaz("Tap Your Card"); // Ekrana Tap Your Card yazdýr.// Print Tap Your Card to the screen.
                          LCD_Git_XY(2,1);
                         LCD_Yazi_Yaz("Rfid Scaning..");

                    }
          else if(UCA0RXBUF=='b'){
                        LCD_Temizle();
                        LCD_Git_XY(1,1);
                        LCD_Yazi_Yaz("Good Bye");
                        LCD_Git_XY(2,1);
                        LCD_Yazi_Yaz("Have A Nice Day");}
          else if(UCA0RXBUF=='c'){
                        LCD_Temizle();
                        LCD_Git_XY(1,1);
                        LCD_Yazi_Yaz("Checking Balance");
                        LCD_Git_XY(2,1);
                        LCD_Yazi_Yaz("Please wait!!!");}
          else if(UCA0RXBUF=='d'){
                        LCD_Temizle();
                        LCD_Git_XY(1,1);
                        LCD_Yazi_Yaz("Inefficient Fund");
                        LCD_Git_XY(2,1);
                        LCD_Yazi_Yaz("Please Recharge..");

              }
          else if(UCA0RXBUF=='e'){
                        LCD_Temizle();
                        d=1;}
          else if(UCA0RXBUF=='y'){
              LCD_Temizle();
                         LCD_Git_XY(1,1);
                                      LCD_Yazi_Yaz("SORRY PARKING");
                                      LCD_Git_XY(2,1);
                                      LCD_Yazi_Yaz("SPACES ARE FULL");
                                      __delay_cycles(1000000);
                                      c=0;




          }



          //3 mod bariyer hýzý fonksiyonlarý
          // mod barrier speed functions
          //Yavaþ//Slow
                if(UCA0RXBUF=='2')
                               {
                    if(TA1CCR1 ==1600){
                                             for (i=1600; i>=650; i--) {
                                             TA1CCR1 = i;
                                              _delay_cycles(400);

                                             }}
                                              else if(TA1CCR1 ==650){
                                                for (i=650; i<=1600; i++)
                                                 {
                                                 TA1CCR1 = i;
                                                  __delay_cycles(400);c=0;
                                                     }}

                                                d=0;c=0;
                               }
                //Orta//Middle
                  else if(UCA0RXBUF=='1')
                                       {
                      if(TA1CCR1 ==1600){
                                               for (i=1600; i>=650; i--) {
                                               TA1CCR1 = i;
                                                _delay_cycles(200);

                                               }}
                                                else if(TA1CCR1 ==650){
                                                  for (i=650; i<=1600; i++)
                                                   {
                                                   TA1CCR1 = i;
                                                    __delay_cycles(200);c=0;
                                                       }}

                                                  d=0;c=0;
                                       }
                //Hýzlý//Fast
                  else if(UCA0RXBUF=='0')
                                       {
                      if(TA1CCR1 ==1600){
                                               for (i=1600; i>=650; i--) {
                                               TA1CCR1 = i;
                                                _delay_cycles(20);

                                               }}
                                                else if(TA1CCR1 ==650){
                                                  for (i=650; i<=1600; i++)
                                                   {
                                                   TA1CCR1 = i;
                                                    __delay_cycles(20);c=0;
                                                       }}

                                                  d=0;c=0;
                                       }
                //Park yer sayýsý belirleme
                // Determining the number of parking spaces
           else if(UCA0RXBUF=='A'){
                           x=4;
           }
           else if(UCA0RXBUF=='B'){
                           x=3;

           }
           else if(UCA0RXBUF=='C'){
                           x=2;

                      }
           //LCD deki yazýyý giriþ ekranýna döndürmek için
                // To return the text on the LCD to the login screen
           else if(UCA0RXBUF=='x'){
               d=0;c=0;

                                 }



      }

