/*
 Erhan YILMAZ 20.04.2014
 MSP430 Denetleyiciler için HD44780 kontrolörlü 2x16 Karakter LCD C Kütüphanesi
 www.fatihinanc.com sitesinden alinip uyarlanmistir. 1MHz de MSP430G2553 denetleyicisi ile denenmistir.
*/

#include <msp430.h>     // msp430 baslik dosyasi ekleniyor
#include "LCD.h"      // LCD baslik dosyasi   ekleniyor
// LCDye karakter yazan fonkisyon
void LCD_Karakter_Yaz(char veri)
{
  unsigned char bSayac;
  LCD_RS(1);
  for(bSayac=0;bSayac<40;bSayac++);
  LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F)|((veri) & 0xF0);
  EN_AC_KAPA();
  LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F)|(veri<<4 & 0xF0);
  EN_AC_KAPA();
}
// LCDye komut yazan fonkiyon
void LCD_Komut_Yaz(unsigned char komut)
{
  unsigned char bSayac;
  LCD_RS(0);
  for(bSayac=0;bSayac<40;bSayac++);
  LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F)|((komut) & 0xF0);
  EN_AC_KAPA();
  LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F)|(komut<<4 & 0xF0);
  EN_AC_KAPA();
}
// LCD ekrani temizleyip kursor pozisyonunu baslengiça getriren fonksiyon
void LCD_Temizle(void)
{
 unsigned int  bSayac;
  LCD_Komut_Yaz(0x01);
  for(bSayac=0;bSayac<10000;bSayac++);
}
// LCDye yazi yazdiran fonksiyon
void LCD_Yazi_Yaz(const char* yazi)
{
  while(*yazi)
  LCD_Karakter_Yaz(*yazi++);
}
// LCD kursörü istenilen yere götüren fonksiyon
void LCD_Git_XY(char x, char y)
{
  if(x==1)
  LCD_Komut_Yaz(0x80+((y-1)%16));
  else
  LCD_Komut_Yaz(0xC0+((y-1)%16));
}
// LCD baslangiç ayarlarini yapan fonksiyon
void LCD_Ayarla()
{
  unsigned int bSayac1,bSayac2;
  LCD_DATA_PORT_DIR     |=(BIT4 + BIT5 + BIT6 + BIT7);
  LCD_DATA_PORT_SEL     &= ~(BIT4 + BIT5 + BIT6 + BIT7);
  LCD_DATA_PORT_SEL2    &= ~(BIT4 + BIT5 + BIT6 + BIT7);

  LCD_CONTROL_PORT_DIR  |=  (LCD_EN_BIT + LCD_RS_BIT);
  LCD_CONTROL_PORT_SEL  &= ~(LCD_EN_BIT + LCD_RS_BIT);
  LCD_CONTROL_PORT_SEL2 &= ~(LCD_EN_BIT + LCD_RS_BIT);

  LCD_DATA_PORT |=(BIT4 + BIT5 + BIT6 + BIT7);
  LCD_CONTROL_PORT  &= ~(LCD_EN_BIT + LCD_RS_BIT);
  for(bSayac1=0;bSayac1<500;bSayac1++)
  for(bSayac2=0;bSayac2<200;bSayac2++);
  LCD_RS(0);
  LCD_EN(0);
  for(bSayac1=0;bSayac1<2000;bSayac1++);
  LCD_Komut_Yaz(0x28);  // 4 Bit, çift Satir LCD
  LCD_Komut_Yaz(0x0C);  // imleç Gizleniyor
  LCD_Komut_Yaz(0x06);  // Saga dogru yazma aktif
  LCD_Komut_Yaz(0x80);  // LCD Birinci Satir Konumunda
  LCD_Komut_Yaz(0x28);  // 4 Bit, çift Satir LCD
 // LCD_Temizle();        // Ekran Temizleniyor
}
