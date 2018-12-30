#include <Servo.h>

#define DEBUG 0
// 0 ise Bluetooth Serial haberlesme 1 ise Bilgisayar Serial haberlesmesi

#define ULTRASOUND_DISTANCE_MIN 2
#define ULTRASOUND_DISTANCE_CONDITION 5

int delay_motor = 20; // Delay for the loop() function in milliseconds

// Motor Driver (Using PWM Pins)
const int enB = 2; // Motor driver pin enableB, ANALOG controls speed
const int in4 = 3; // Motor driver pin in1, DIGITAL controls direction with in3
const int in3 = 4; // Motor driver pin in2, DIGITAL controls direction with in4
const int in2 = 5; // Motor driver pin in3, DIGITAL  cotrols direcion with in1
const int in1 = 6; // Motor driver pin in4, DIGITAL controls directions with in2
const int enA = 7; // Motor driver pin enableA, ANALOG controls speed
enum direction {
  forward,
  backward
};
enum movement {
  stopped,
  going_forward,
  going_backward,
  turning_right,
  turning_left  
};
enum movement currentMovement = stopped;

// Bluetooth Module (Using TX1, RX1 Pins for Serial 1)
String command; // input from Serial channel
int bt_delay = 200;

// Robot Arm (takes values 0..179) (Using PWM Pins)
Servo hand_left;
Servo hand_right;
bool hand_closed = true;
const int hand_right_pin = 10;
const int hand_left_pin = 11;

// Ultrasound
const int ultrasound_front_echo = 42;
const int ultrasound_front_trig = 44;
unsigned long ultrasound_front_duration;
int ultrasound_front_distance;

void setup() 
{
  Serial.begin(9600); // Serial Monitor communication on channel Serial
  Serial1.begin(9600); // Bluetooth communcation on channel Seria1
  println("SETUP STARTED...");
  
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);  
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(ultrasound_front_trig, OUTPUT);
  pinMode(ultrasound_front_echo, INPUT);
  
  hand_left.attach(hand_left_pin);
  hand_right.attach(hand_right_pin);
  open_hand();
  println("SETUP COMPLETED...");
}

void loop() 
{
  if (available()) 
  {
    // Android'deki bluetooth haberlesme programindaki Joystick arayuzu ile gelecek komutlar
      command = readStringUntil('X');
      print("Command recieved: ");
      println(command);
      Serial.println(command);
        if (command == "ileri") 
      {
        currentMovement = going_forward;
      } 
      else if (command == "geri") 
      {
        currentMovement = going_backward;
      } 
      else if (command == "sag") 
      {
        currentMovement = turning_right;
      } 
      else if (command == "sol") 
      {
        currentMovement = turning_left;
      } 
      else if (command == "dur") 
      {
        currentMovement = stopped;
      } 
      else if (command == "toggle el") {
        if (hand_closed) {
          hand_closed = false;
          // El acilacak
          open_hand();
        } else {
          hand_closed = true;
          // El kapanacak
          close_hand();
        }
        if (currentMovement != stopped ) {
          currentMovement = stopped;
        }
      } 
// 5 cok hizli
// 10 cok hizli
// 15 hizli
// 20 hizli
// 30 iyi
// < 35 yoksa traktor
      else if (command == "yavasla") {
        delay_motor+=5;
        if (delay_motor >= 30) {
          delay_motor = 30;
        }
      } 
      else if (command == "hizlan") {
        delay_motor-=5;
         if (delay_motor <= 15) {
          delay_motor = 15;
        }
      }
      else 
      {
        println("UNKNOWN COMMAND");
      }

      if (command != "") {
        delay(bt_delay);
      }      
  } 
  else 
  {
    //println("Awaiting command...");
  }

  ultrasonRoutine();
  movementRoutine(); // Should run every loop()
  
}

void ultrasonRoutine() {
  // FRONT
  digitalWrite(ultrasound_front_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasound_front_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasound_front_trig, LOW);
  ultrasound_front_duration = pulseIn(ultrasound_front_echo, HIGH); // LOW______HIGH----(t)------LOW____ This function returns the time t 
  ultrasound_front_distance = ultrasound_front_duration / 29 / 2;
  if(ultrasound_front_duration == 0)
  {
    println("Warning: Ultrasound Sensor Error");
  } 
  else 
  {
    print("Front Distance: ");
    if (ultrasound_front_distance > 1250) 
    { 
      ultrasound_front_distance = ULTRASOUND_DISTANCE_MIN; 
    } // If the result is bugged set it to min (it is bugged at below 3cm)
    print(ultrasound_front_distance);
    println(" cm");
  }

  if (ultrasound_front_distance <= 15) {
    currentMovement = stopped;
  }
}

void movementRoutine() {
  analogWrite(enA,  10);
  analogWrite(enB,  10);
  if (currentMovement == stopped) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);  
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW); 
  } else if (currentMovement == going_forward) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH); 
  } else if (currentMovement == going_backward) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW); 
  } else if (currentMovement == turning_right) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW); 
  } else if (currentMovement == turning_left) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

  if (currentMovement != stopped) {
    delay(5);
  
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);  
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW); 

    if (currentMovement == turning_right || currentMovement == turning_left ) {
      int faster_delay_motor = delay_motor - 5;
         if (faster_delay_motor <= 15) {
          faster_delay_motor = 15;
        }
      delay(faster_delay_motor);
    } else {
      delay(delay_motor);
    }
    
  }
}

void close_hand() {
  set_hand(60);  
}

void open_hand() {
  set_hand(0);
}

void set_hand(int degree) {
  if (degree < 0 ) {
    degree = 0;
  } 
  if (degree > 90) {
    degree = 90;
  }
  int x = 180 - degree;
  hand_right.write(x);
  hand_left.write(degree);
}

/////////////////////////////////////////////////
void debugPrint(String str) {
  #if DEBUG == 1
      Serial.print(str);
  #endif
}

void debugPrint(int str) {
  #if DEBUG == 1
      Serial.print(str);
  #endif
}

void print(String str) {
    #if DEBUG == 1
      Serial.print(str);
    #else
      Serial1.print(str);
    #endif
}

void println(String str) {
    #if DEBUG == 1
      Serial.println(str);
    #else
      Serial1.println(str);
    #endif 
}

void print(int str) {
    #if DEBUG == 1
      Serial.print(str);
    #else
      Serial1.print(str);
    #endif
}

void println(int str) {
    #if DEBUG == 1
      Serial.println(str);
    #else
      Serial1.println(str);
    #endif 
}

bool available() {
    #if DEBUG == 1
      return Serial.available();
    #else
      return Serial1.available();
    #endif 
}

String readStringUntil(char c) {
    #if DEBUG == 1
      return Serial.readStringUntil(c);
    #else
      return Serial1.readStringUntil(c);
    #endif 
}
