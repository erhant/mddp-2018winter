/*
TURUNCU 03
KIRMIZI 04
*/

#define INFRARED_REFERENCE 200 // Reference analog value from the infrared sensor, below means black, above means not black.

#define DELAY_MS 1000
// Infrared Sensor x8 FRONT (Using Analog IN Pins)
const int irA1 = A0;
const int irA2 = A1;
const int irA3 = A2;
const int irA4 = A3;
const int irA5 = A4;
const int irA6 = A5;
const int irA7 = A6;
const int irA8 = A7;
int infraredValues [8] = {0};
bool lineDetected [8] = {false};


void setup() 
{
  Serial.begin(9600); // Serial Monitor communication on channel Serial
  Serial.println("SETUP STARTED...");
  
  pinMode(irA1, INPUT); // Infrared 1
  pinMode(irA2, INPUT);
  pinMode(irA3, INPUT);
  pinMode(irA4, INPUT);
  pinMode(irA5, INPUT);
  pinMode(irA6, INPUT);
  pinMode(irA7, INPUT); 
  pinMode(irA8, INPUT); // Infrared 8

  
  Serial.println("SETUP COMPLETED...");
}

void loop() 
{
      
    infraredValues[0] = analogRead(irA1);
    infraredValues[1] = analogRead(irA2);
    infraredValues[2] = analogRead(irA3);
    infraredValues[3] = analogRead(irA4);
    infraredValues[4] = analogRead(irA5);
    infraredValues[5] = analogRead(irA6);
    infraredValues[6] = analogRead(irA7);
    infraredValues[7] = analogRead(irA8);
    
    for (int i = 0; i<8; i++) 
    {
//      Serial.print(infrared_values[i]);
//      Serial.print(' ');
      if (infraredValues[i] > INFRARED_REFERENCE) 
      {
        lineDetected[i] = true; 
        Serial.print("B "); 
      } 
      else 
      { 
        lineDetected[i] = false; 
        Serial.print("W ");
      }
    }
    Serial.println("");
    bool middleDetected = false;
    bool leftDetected = false;
    bool rightDetected = false;
    if (lineDetected[3] && lineDetected[4]) {
      middleDetected = true;
      Serial.print('M');
    } else {
      Serial.print('-');
    }
    if (lineDetected[5] && lineDetected[6] && lineDetected[7]) {
      leftDetected = true;
      Serial.print('L');
    } else {
      Serial.print('-');
    }
    if(lineDetected[0] && lineDetected[1] && lineDetected[2]) {
      rightDetected = true;
      Serial.print('R');
    } else {
      Serial.print('-');
    }
    Serial.println("");
    Serial.println("");
    delay(DELAY_MS);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
