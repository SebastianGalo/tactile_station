#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>
#include "Adafruit_MPR121.h"
#include <Wire.h>

SoftwareSerial mySerial(10, 11); // RX, TX
DFPlayerMini_Fast myMP3;



// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();



void setup() {
  Serial.begin (115200);
  mySerial.begin(9600);

  myMP3.begin(mySerial);

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

  // Search inputs for touch
  for (uint8_t i = 0; i < 12; i++) {


    if (cap.touched() & (1 << i)) {
      myMP3.wakeUp();
      myMP3.play(i + 1);
      while (myMP3.isPlaying()) {
        myMP3.play(i + 1);
        break;
      }
      myMP3.sleep();
    }
  }



  //Loop end
}
