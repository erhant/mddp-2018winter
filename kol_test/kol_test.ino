#include <Servo.h>

// map: 
Servo kol_sol;
Servo kol_sag;
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(9600);
  kol_sag.attach(9);  // attaches the servo on pin 9 to the servo object
  kol_sol.attach(8);
  kol_sag.write(170);
  kol_sol.write(10);
}
void loop() {


    kol_sag.write(10);
    kol_sol.write(170);
    delay(1000);
    kol_sag.write(170);
    kol_sol.write(10);
    delay(1000);
  
//  if (Serial.available()) {
//    String str = Serial.readString();
//    if (str == "+") {
//      pos = max(0,pos - 10);
//    } else if (str == "-") {
//      pos = min(180,pos + 10);
//    } else {
//      // do nothing
//    }
//    kol.write(pos);
//  }
//  delay(1000);  
  

//  for (pos = 0; pos <= 179; pos += 8) { // goes from 0 degrees to 180 degrees
//    kol1.write(min(pos,179));
//    kol2.write(min(180-pos,179));
//    Serial.println(pos);
//    delay(50);                       // waits 15ms for the servo to reach the position
//    
//  }
//  delay(1000);
//  
//  for (pos = 179; pos >= 0; pos -= 8) { // goes from 180 degrees to 0 degrees
//    kol1.write(max(pos,0));
//    kol2.write(max(abs(pos-180),0));
//    Serial.println(pos);
//    delay(50);                       // waits 15ms for the servo to reach the position
// }
//  delay(1000);

}
