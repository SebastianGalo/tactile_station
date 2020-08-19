#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>
#include "Adafruit_MPR121.h"
#include <Wire.h>
#include <millisDelay.h>

SoftwareSerial mySerial(10, 11); // RX, TX
DFPlayerMini_Fast myMP3;




#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;



void setup() {
  Serial.begin (115200);
  mySerial.begin(9600);

  myMP3.begin(mySerial);

  Serial.println("Setting volume to max");
  //Volume level, max is 30
  myMP3.volume(30);
  delay(20);

  myMP3.sleep();



  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
}

void loop() {

  // Get the currently touched pads
  currtouched = cap.touched();



  // if it *is* touched and *wasnt* touched before, send data!
  for (uint8_t i = 0; i < 12; i++) {




    //Pin reading and corresponding track is played
    if (cap.touched() & (1 << i)) {
      myMP3.wakeUp();
      myMP3.play(i);
      while (myMP3.isPlaying() == true) {
        delay (2000);
      }


      myMP3.sleep();
    }


  }

  // reset our state
  lasttouched = currtouched;
}
