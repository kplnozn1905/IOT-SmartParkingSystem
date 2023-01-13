#include <msp430.h>
#include "LCD.h"




int x;//Park yeri say�s�// Number of parking spaces
int i;//Bariyerlerin a��l�� h�z�//opening speed of barriers
int a=0;//Kapal� parka girildi�inde LCD de yazan yaz�lar�n ayarlanmas�.// Adjusting the texts written on the LCD when entering the closed parking lot.
int c=0;//Otopark isminin gelen yaz�lardan sonra tekrar getirmek i�in kullan�lan de�i�ken.// The variable used to bring back the name of the parking lot after the incoming texts.
int d=0;//Giri� yapan ara� i�in park yerlerinin durumunu g�stermek i�i 1 yap�l�r.// Set to 1 to show the status of the parking spaces for the vehicle entering.

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;//Watchdog timeri durdur.// Stop the watchdog timer.

  if (CALBC1_1MHZ == 0xFF)
  {
    while(1);//wait till the processor is fully calibrated//i�lemci tamamen kalibre edilene kadar bekleyin
  }

  BCSCTL1 = CALBC1_1MHZ;//Dahili osilat�r� 1MHz'e ayarla//set the internal oscillator to 1MHz
  DCOCTL = CALDCO_1MHZ;

  P2DIR =0xEA;//P2.0-2-4 portlar�n� giri� yapt�k.// We have entered the P2.0-2-4 ports.
  P2OUT =0x15;//Giri� portlar�n� low olarak  ayarlad�k.// We set the input ports to low.
  P2REN=0x15;//Enable resistor on  P2.0-2-4//P2.0-2-4'te direnci etkinle�tir
  P2DIR |=BIT1;//p2.1 portunu giri� yapt�k oldu// we have entered the p2.1 port
      P2OUT |=BIT1;
      P2SEL =0x02;//P2.1 Timer olarak ayarlan�yor.// It is set to P2.1 Timer.
  P1DIR &=~BIT3;//P1.3 portunu giri� yapt�k// We entered the P1.3 port
  P1OUT |=BIT3;//Giri� portunu low olarak  ayarlad�k.//We set the input port to low.
  P1REN|=BIT3;//Enable resistor on  P1.3//Enable resistor on P1.3



  P1SEL=(BIT1+BIT2);//P1.1 = RXD, P1.2=TXD
  P1SEL2=(BIT1+BIT2);//P1.1 = RXD, P1.2=TXD

  UCA0CTL1|=UCSSEL_2;//UART Ayarlar�, Saat kayna�� SMCLK//UART Settings, Clock source SMCLK
  UCA0BR0=104;//1MHz 9600 bud ayar�///1MHz 9600 bud tuning
  UCA0BR1=0;// 1MHz 9600 baud ayar�///1MHz 9600 bud tuning
  UCA0MCTL=UCBRF_0+UCBRS_1;// UART Baud hassas ayar// UART Baud fine tuning
  UCA0CTL1&=~UCSWRST;// USCI birimini haz�rla// prepare the USCI volume
  IE2|=UCA0RXIE;// USCI_A0 RX kesmesini a�// turn on USCI_A0 RX interrupt

  //PWM ayarlar�//PWM settings
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

    //Girilen x de�erine g�re park yerleri a��l�yor.
    // Parking spaces are opened according to the entered x value.
    switch(x){

    case 4:   if((P1IN&BIT3)){UCA0TXBUF ='A';//Engelin �n� kapal� de�ilse tx=A//If the obstacle is not blocked tx=A
                if(d==1){LCD_Git_XY(1,1);LCD_Yazi_Yaz("P1:E");}//d=0 ile ba�l�yor kart okuma i�lemlerinden d=1 oluyor ve LCD de park yeri durumu yaz�l�yor.
                // starts with d=0, becomes d=1 from card reading operations, and the parking lot status is written on the LCD.
              }
              else if(!(P1IN&BIT3)){UCA0TXBUF ='B';//Engelin �n� kapal� ise tx=B ve// If the obstacle is closed, tx=B and
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
//G�nderilen verilere g�re yap�lan i�lemler
// Operations based on the sent data
#pragma vector=USCIAB0RX_VECTOR
      __interrupt void isr(void)
      {

          if(UCA0RXBUF=='a'){
                        LCD_Temizle();
                        LCD_Git_XY(1,1); // Kurs�r� 1.sat�r 1.s�tuna g�t�r.// Move the cursor to row 1, column 1.
                        LCD_Yazi_Yaz("Tap Your Card"); // Ekrana Tap Your Card yazd�r.// Print Tap Your Card to the screen.
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



          //3 mod bariyer h�z� fonksiyonlar�
          // mod barrier speed functions
          //Yava�//Slow
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
                //H�zl�//Fast
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
                //Park yer say�s� belirleme
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
           //LCD deki yaz�y� giri� ekran�na d�nd�rmek i�in
                // To return the text on the LCD to the login screen
           else if(UCA0RXBUF=='x'){
               d=0;c=0;

                                 }



      }

