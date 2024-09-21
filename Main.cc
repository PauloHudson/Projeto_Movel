// C++ code
//
/*
  LiquidCrystal Library - Hello World

   Demonstrates the use of a 16x2 LCD display.
  The LiquidCrystal library works with all LCD
  displays that are compatible with the  Hitachi
  HD44780 driver. There are many of them out
  there, and you  can usually tell them by the
  16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
  * LCD RS pin to digital pin 12
  * LCD Enable pin to digital pin 11
  * LCD D4 pin to digital pin 5
  * LCD D5 pin to digital pin 4
  * LCD D6 pin to digital pin 3
  * LCD D7 pin to digital pin 2
  * LCD R/W pin to ground
  * LCD VSS pin to ground
  * LCD VCC pin to 5V
  * 10K resistor:
  * ends to +5V and ground
  * wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008  by David
  A. Mellis
  library modified 5 Jul 2009  by Limor Fried
  (http://www.ladyada.net)
  example added 9 Jul 2009  by Tom Igoe
  modified 22 Nov 2010  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystal
*/

#include <LiquidCrystal.h>

int buzzer = 6;

int bot1 = 10;
int bot2 = 9;
  
int led1 = 7;
int led2 = 8;

int seconds = 0;

LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);

void setup()
{
  lcd_1.begin(16, 2); 
  pinMode(bot1, INPUT);  // true
  pinMode(bot2, INPUT);  // false
  pinMode(buzzer, OUTPUT); // AUDIO
  pinMode(led1, OUTPUT);  // LED @1
  pinMode(led2, OUTPUT); //LED @2
  
  lcd_1.print("hello world!");
  Serial.begin(9600);
}

void loop()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting
  // begins with 0):
  noTone(buzzer);
  int button_True = digitalRead(bot1);
  int button_False = digitalRead(bot2);

  if (button_True == HIGH) {
    digitalWrite(led1, HIGH);
	delay(300);
    
  
    Serial.println("ligado");
    digitalWrite(led1, LOW);
    
  }
  
  if (button_False == HIGH) {
    digitalWrite(led2, HIGH);
    delay(300);
    Serial.println("ligado_2");
    
    digitalWrite(led2, LOW);
    
   
  
  }
  lcd_1.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd_1.print(seconds);
  delay(1000); // Wait for 1000 millisecond(s)
  seconds += 1;
}
