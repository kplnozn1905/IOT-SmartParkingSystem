/*
 *  File            :   LCD_162_1_0_1.h
 *  Date created    :   08/08/2016
 *  Author          :   Nguyen Chinh Thuy
 *  Tool            :   CCS6.1.2.00015 (C programming)
 *  Current version :   1.0.1
 *  Description     :   LCD 16x2 library
 *  Target object   :   MSP430G2553
 */
/**************************************************************************************************
 *  PRE-DEFINITIONs
 *************************************************************************************************/
#ifndef LCD_162_1_0_1_H_
#define LCD_162_1_0_1_H_

#ifdef __cplusplus
extern "C" {
#endif
// LCD baðlantýsýnda kullanýlacak pinlerin tanýmlamalarý
#define LCD_DATA_PORT P1OUT
#define LCD_DATA_PORT_DIR P1DIR
#define LCD_DATA_PORT_SEL P1SEL
#define LCD_DATA_PORT_SEL2 P1SEL2
#define LCD_CONTROL_PORT P2OUT
#define LCD_CONTROL_PORT_DIR P2DIR
#define LCD_CONTROL_PORT_SEL P2SEL
#define LCD_CONTROL_PORT_SEL2 P2SEL2
#define LCD_RS_BIT BIT7
#define LCD_EN_BIT BIT6
// LCD sürme iþleminde kullanýlan makrolar
#define EN_AC_KAPA() LCD_EN(1),LCD_EN(0)
#define LCD_RS(x) ( (x) ? (LCD_CONTROL_PORT |= LCD_RS_BIT) :(LCD_CONTROL_PORT &= ~LCD_RS_BIT) )
#define LCD_EN(x) ( (x) ? (LCD_CONTROL_PORT |= LCD_EN_BIT) :(LCD_CONTROL_PORT &= ~LCD_EN_BIT) )
// LCD kütüphanesinde kullanýlan fonksiyon prototipleri
void LCD_Komut_Yaz(unsigned char); // LCD ye komut göndermeye yarar
void LCD_Temizle(void); // LCD ekraný temizler
void LCD_Yazi_Yaz(const char*); // LCDye string ifade yazar
void LCD_Git_XY(char,char); // LCDde kursörü konumlandýrýr
void LCD_Ayarla(void); // LCD baþlangýçayarlarý yapýlýr
void LCD_Karakter_Yaz(char); // LCDye tek karakter yazmak için kullanýlýr
#ifdef __cplusplus
}
#endif
#endif /*__LCD_H */
