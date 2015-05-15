/*************************************************** 
 * This is a test example for the Adafruit Trellis w/HT16K33
 * 
 * Designed specifically to work with the Adafruit Trellis 
 * ----> https://www.adafruit.com/products/1616
 * ----> https://www.adafruit.com/products/1611
 * 
 * These displays use I2C to communicate, 2 pins are required to  
 * interface
 * Adafruit invests time and resources providing this open source code, 
 * please support Adafruit and open-source hardware by purchasing 
 * products from Adafruit!
 * 
 * Written by Limor Fried/Ladyada for Adafruit Industries.  
 * MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include "Adafruit_Trellis.h"

/*************************************************** 
 * This example shows reading buttons and setting/clearing buttons in a loop
 * "momentary" mode has the LED light up only when a button is pressed
 * "latching" mode lets you turn the LED on/off when pressed
 * 
 * Up to 8 matrices can be used but this example will show 4 or 1
 ****************************************************/

#define MOMENTARY 0
#define LATCHING 1
// set the mode here
#define MODE MOMENTARY 



Adafruit_Trellis matrix0 = Adafruit_Trellis();

// uncomment the below to add 3 more matrices

Adafruit_Trellis matrix1 = Adafruit_Trellis();
 Adafruit_Trellis matrix2 = Adafruit_Trellis();
 Adafruit_Trellis matrix3 = Adafruit_Trellis();
 // you can add another 4, up to 8
 

// Just one
//Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);
// or use the below to select 4, up to 8 can be passed in
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0, &matrix1, &matrix2, &matrix3);

// set to however many you're working with here, up to 8
#define NUMTRELLIS 4

#define numKeys (NUMTRELLIS * 16)

// Connect Trellis Vin to 5V and Ground to ground.
// Connect the INT wire to pin #A2 (can change later!)
#define INTPIN A2
// Connect I2C SDA pin to your Arduino SDA line
// Connect I2C SCL pin to your Arduino SCL line
// All Trellises share the SDA, SCL and INT pin! 
// Even 8 tiles use only 3 wires max


int Vval = 0;
int Pval = 0;
String out = "";
String oldOut = "";


void setup() {
  Serial.begin(9600);

  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);

  // begin() with the addresses of each panel in order
  // I find it easiest if the addresses are in order
  //trellis.begin(0x70);  // only one
   trellis.begin(0x70, 0x71, 0x72, 0x73);  // or four!

  // light up all the LEDs in order
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
  // then turn them off
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
}


void loop() {
  delay(60); // 30ms delay is required, dont remove me!

  String out = "";
  
  
  Vval = analogRead(A0);

  if (Vval > 999)
    out = "V"+String(Vval)+"X";
  else if (Vval > 99) 
    out = "V0"+String(Vval)+"X";
  else if (Vval > 9)
    out = "V00"+String(Vval)+"X";
  else 
    out = "V000"+String(Vval)+"X";

  //if (out.equals(oldOut));
  //else
    Serial.println(out);  
  
    Pval = analogRead(A3);

  if (Pval > 999)
    out = "P"+String(Pval)+"X";
  else if (Pval > 99) 
    out = "P0"+String(Pval)+"X";
  else if (Pval > 9)
    out = "P00"+String(Pval)+"X";
  else 
    out = "P000"+String(Pval)+"X";

  //if (out.equals(oldOut));
  //else
    Serial.println(out);


  int val = 0;
  char dats[6];
  String str = "";

  if (Serial.available() > 0) {
    //val = Serial.read();

    //Serial.println(val);

    Serial.readBytesUntil('X', dats, 6);

    for (uint8_t i=1; i<5; i++){
      Serial.println(dats[i]);
      str+=dats[i];
    }

    val = str.toInt();

    //Serial.println(val);

    if (dats[0] == 84){
      if (trellis.isLED(val))
        trellis.clrLED(val);
      else
        trellis.setLED(val);    
      trellis.writeDisplay();
      //Serial.println((int)Serial.peek()-'0');

    }



  }

  if (MODE == MOMENTARY) {
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
        out = "";
        // if it was pressed, turn it on
        if (trellis.justPressed(i)) {
          out+="T00";
          if (i < 10){
            out+="0"; 
          } 
          out+=i;
          out+="X";
          Serial.println(out);
          trellis.setLED(i);
        } 
        out = "";
        // if it was released, turn it off
        if (trellis.justReleased(i)) {
          out+="T00";
          if (i < 10){
            out+="0"; 
          } 
          out+=i;
          out+="X";
          Serial.println(out);
          trellis.clrLED(i);
        }
        //if (trellis.isLED(i))
        //  Serial.print("1");
        //else
        //  Serial.print("0");
      }
      //Serial.println();
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }
  }

  if (MODE == LATCHING) {
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
        // if it was pressed...
        if (trellis.justPressed(i)) {
          Serial.println(i);
          // Alternate the LED
          if (trellis.isLED(i))
            trellis.clrLED(i);
          else
            trellis.setLED(i);
        } 
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }
  }
}













