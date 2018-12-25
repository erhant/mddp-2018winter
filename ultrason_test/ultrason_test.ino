#define DELAY_MS 1000

// Ultrasound Module FRONT
bool ultrasound_enabled = true;
const int ultrasound_echo = 42;
const int ultrasound_trig = 44;
unsigned long ultrasound_duration;
int ultrasound_distance;


void setup() 
{
  Serial.begin(9600); // Serial Monitor communication
  pinMode(ultrasound_trig, OUTPUT);
  pinMode(ultrasound_echo, INPUT);

}

void loop() 
{
  digitalWrite(ultrasound_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasound_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasound_trig, LOW);
  ultrasound_duration = pulseIn(ultrasound_echo, HIGH); // LOW______HIGH----(t)------LOW____ This function returns the time t 
  ultrasound_distance = ultrasound_duration / 29 / 2;
  if(ultrasound_duration == 0){
    Serial.println("Warning: Ultrasound Sensor Error");
  } else {
    Serial.print("Distance: ");
    Serial.print(ultrasound_distance);
    Serial.println(" cm");
  }
    
  delay(DELAY_MS);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
