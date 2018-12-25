#include <Servo.h>

// map: 
Servo rocky_2;
Servo rocky_1;

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(9600);
  rocky_2.attach(10);  // attaches to the pin
  rocky_1.attach(11);
  rocky_2.write(180); // Initial 180
  rocky_1.write(0); // Initail 0

  
}
void loop() {
   


  ////////////
  // 20 12 2018
  // kapa
  rocky_2.write(180); // Initial 180
  rocky_1.write(0); // Initail 0

  delay(200); 
  
  // ac
  rocky_2.write(90); // Initial 180
  rocky_1.write(90); // Initail 0
  ////////////


delay(200); 
}
