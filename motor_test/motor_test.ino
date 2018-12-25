#define DELAY_MS 2000

#define FORWARD 1 
#define BACKWARD 2

const int enB = 2; // Motor driver pin enableB, ANALOG controls speed
const int in4 = 3; // Motor driver pin in1, DIGITAL controls direction with in3
const int in3 = 4; // Motor driver pin in1, DIGITAL controls direction with in4
const int in2 = 5; // Motor driver pin in1, DIGITAL  cotrols direcion with in1
const int in1 = 6; // Motor driver pin in1, DIGITAL controls directions with in2
const int enA = 7; // Motor driver pin enableA, ANALOG controls speed

void setup() 
{
  
  Serial.begin(9600); // Serial for debugging from pc
  Serial.println("STARTING SETUP");
  
  // All pins are output for motor driver
    pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);  
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  

  Serial.println("COMPLETE");

}

void loop() 
{

  // ileri
  analogWrite(enA,  0);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enB, 0); 
    digitalWrite(in3, LOW);
    digitalWrite(in4,  HIGH);
    delay(10); 

    analogWrite(enA,  0);  
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);  
  // Right Pair
    analogWrite(enB, 0); 
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW); 

    delay(100);
//    wheelsTurn(25, FORWARD, 25, FORWARD); // Forward
//    delay(DELAY_MS);
//   wheelsTurn(25, BACKWARD, 25, BACKWARD); // Backward
//    delay(DELAY_MS);
//   wheelsTurn(25, BACKWARD, 25, FORWARD); // Turns left IN PLACE
//    delay(DELAY_MS);
//    wheelsTurn(25, FORWARD, 25, BACKWARD); // Turns right IN PLACE
//    delay(DELAY_MS);
//   wheelsStop(); // any other
//    delay(DELAY_MS);
   
}

void wheelsTurn(int speedLeftPair, int directionLeftPair, int speedRightPair, int directionRightPair) {
  /*
  speedLeftPair -> 0 .. 255
  speedRightPair -> 0 .. 255
  directionLeftPair -> FORWARD | BACKWARD
  directionRightPair -> FORWARD | BACKWARD
  */
	if (directionLeftPair == FORWARD) {
		analogWrite(enA,  speedLeftPair);
		digitalWrite(in1, LOW);
		digitalWrite(in2, HIGH);
    Serial.println("SOL ILERI");
	} else {
		analogWrite(enA,  speedLeftPair);
		digitalWrite(in1, HIGH);
		digitalWrite(in2, LOW);
    Serial.println("SOL GERI");
	}
	// Right Pair
	if (directionRightPair == FORWARD) {
		analogWrite(enB, speedRightPair); 
		digitalWrite(in3, LOW);
		digitalWrite(in4,  HIGH); 
    Serial.println("SAG ILERI");
	} else {
		analogWrite(enB, speedRightPair); 
		digitalWrite(in3, HIGH);
		digitalWrite(in4, LOW); 
    Serial.println("SAG GERI");
	}
}

void wheelsStop() {
	// Left Pair
    analogWrite(enA,  0);  
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);  
	// Right Pair
    analogWrite(enB, 0); 
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW); 
    Serial.println("DUR");
}
