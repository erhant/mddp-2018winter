/*
Author: Erhan Tezcan
Date: 25.11.2018
Title: Multidisciplinary Project - Arudino Code v0.2

KABLO RENK LEJAND
KIRMIZI:  VCC 5V
KAHVERENGI: GND

BT YESILI RX1
BT MAVISI TX1

*/
#include <Servo.h>

#define DEBUG 1 // 0 ise Bluetooth Serial haberlesme 1 ise Bilgisayar Serial haberlesmesi

#define INFRARED_REFERENCE 200 // Reference analog value from the infrared sensor, below means black, above means not black.

#define ULTRASOUND_DISTANCE_MIN 2 // In case ultrasound result is mistaken (e.g. greater than 3000cm) we assign it to min (because that is usually when it is mistaken)
#define ULTRASOUND_DISTANCE_CONDITION 5 // The cm value for the robot to pick up the box. Robot stays this much cm away from the box in front of it.

#define DELAY_MS 50 // Delay for the loop() function in milliseconds

// Motor Driver (Using PWM Pins)
const int enB = 2; // Motor driver pin enableB, ANALOG controls speed
const int in4 = 3; // Motor driver pin in1, DIGITAL controls direction with in3
const int in3 = 4; // Motor driver pin in1, DIGITAL controls direction with in4
const int in2 = 5; // Motor driver pin in1, DIGITAL  cotrols direcion with in1
const int in1 = 6; // Motor driver pin in1, DIGITAL controls directions with in2
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
enum speed {
  yavas,
  normal,
  hizli
};
enum speed currentSpeed = yavas;
enum movement currentMovement = stopped;

// Bluetooth Module (Using TX1, RX1 Pins for Serial 1)
String command; // input from Serial channel

// Ultrasound Modules (Using Digital Pins)
bool ultrasound_enabled = false;

const int ultrasound_front_echo = 42;
const int ultrasound_front_trig = 44;
unsigned long ultrasound_front_duration;
int ultrasound_front_distance;

const int ultrasound_left_echo = 48;
const int ultrasound_left_trig = 50;
unsigned long ultrasound_left_duration;
int ultrasound_left_distance;

const int ultrasound_right_echo = 26;
const int ultrasound_right_trig = 28;
unsigned long ultrasound_right_duration;
int ultrasound_right_distance;

// Infrared Sensor x8 FRONT (Using Analog IN Pins)
bool linefollower_enabled = false;
const int irA1 = A0;
const int irA2 = A1;
const int irA3 = A2;
const int irA4 = A3;
const int irA5 = A4;
const int irA6 = A5;
const int irA7 = A6;
const int irA8 = A7;
int infrared_values [8] = {0};
bool line_detected [8] = {false};
bool left_detected;
bool middle_detected;
bool right_detected;

// Robot Arm (takes values 0..179) (Using PWM Pins)
Servo arm_left;
Servo arm_right;
Servo hand_left;
Servo hand_right;

const int arm_left_pin = 8;
const int arm_right_pin = 9;
const int hand_right_pin = 10;
const int hand_left_pin = 11;

// Algorithm 
int right_turns = 0;
int target_right_turns = -1;
int left_turns = 0;
int target_left_turns = -1;

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
  pinMode(ultrasound_left_trig, OUTPUT);
  pinMode(ultrasound_left_echo, INPUT);
  pinMode(ultrasound_right_trig, OUTPUT);
  pinMode(ultrasound_right_echo, INPUT);

  pinMode(irA1, INPUT); // Infrared 1
  pinMode(irA2, INPUT);
  pinMode(irA3, INPUT);
  pinMode(irA4, INPUT);
  pinMode(irA5, INPUT);
  pinMode(irA6, INPUT);
  pinMode(irA7, INPUT); 
  pinMode(irA8, INPUT); // Infrared 8

  arm_left.attach(arm_left_pin);
  arm_right.attach(arm_right_pin);
  hand_left.attach(hand_left_pin);
  hand_right.attach(hand_right_pin);
  raise_arm();
  open_hand();
  
  println("SETUP COMPLETED...");
}

void loop() 
{
  if (available()) 
  {
      command = readString();
      print("Command recieved: ");
      println(command);
      if (command == "ileri") 
      {
        //go_forward(255);
        currentMovement = going_forward;
      } 
      else if (command == "geri") 
      {
        //go_backward(255);
        currentMovement = going_backward;
      } 
      else if (command == "sag") 
      {
        //turn_right(255);
        currentMovement = turning_right;
      } 
      else if (command == "sol") 
      {
        //turn_left(255);
        currentMovement = turning_left;
      } 
      else if (command == "dur") 
      {
        //stop();
        currentMovement = stopped;
      } 
      else if (command == "toggle ultrasound")
      {
        if (ultrasound_enabled) 
        {
          ultrasound_enabled = false;
        } 
        else 
        {
          ultrasound_enabled = true;
        }
      } 
      else if (command == "toggle infrared") 
      {
        if (linefollower_enabled) 
        {
          linefollower_enabled = false;
        } 
        else 
        {
          linefollower_enabled = true;
        }
      } 
      else if (command == "kol kaldir") {
        raise_arm();
      }
      else if (command == "kol indir") {
        lower_arm();
      }
      else if (command == "el ac") {
        open_hand();
      }
      else if (command == "el kapa") {
        close_hand();
      }
      else if (command == "kutu al") {
        box_pickup_routine();
      }
      else if (command == "hizlan") {
        if (currentSpeed == yavas) {
          currentSpeed = normal;
        } else if (currentSpeed == normal) {
          currentSpeed = hizli;
        }
      } 
      else if (command == "yavasla") {
        if (currentSpeed == normal) {
          currentSpeed = yavas;
        } else if (currentSpeed == hizli) {
          currentSpeed = normal;
        }
      }
      else if (command == "robot durum") {
        printStatus();
      }
      else 
      {
        println("UNKNOWN COMMAND");
      }
    } 
    else 
    {
      //println("Awaiting command...");
    }
   
   
    if (ultrasound_enabled) 
    {
      println("ULTRASOUND DATA:\n\n");
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

      // RIGHT
      digitalWrite(ultrasound_right_trig, LOW);
      delayMicroseconds(2);
      digitalWrite(ultrasound_right_trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(ultrasound_right_trig, LOW);
      ultrasound_right_duration = pulseIn(ultrasound_right_echo, HIGH); // LOW______HIGH----(t)------LOW____ This function returns the time t 
      ultrasound_right_distance = ultrasound_right_duration / 29 / 2;
      if(ultrasound_right_duration == 0)
      {
        println("Warning: Ultrasound Sensor Error");
      } 
      else 
      {
        print("Right Distance: ");
        if (ultrasound_right_distance > 1250) 
        { 
          ultrasound_right_distance = ULTRASOUND_DISTANCE_MIN; 
        } // If the result is bugged set it to min (it is bugged at below 3cm)
        print(ultrasound_right_distance);
        println(" cm");
      }

      // LEFT
      digitalWrite(ultrasound_left_trig, LOW);
      delayMicroseconds(2);
      digitalWrite(ultrasound_left_trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(ultrasound_left_trig, LOW);
      ultrasound_left_duration = pulseIn(ultrasound_left_echo, HIGH); // LOW______HIGH----(t)------LOW____ This function returns the time t 
      ultrasound_left_distance = ultrasound_left_duration / 29 / 2;
      if(ultrasound_left_duration == 0)
      {
        println("Warning: Ultrasound Sensor Error");
      } 
      else 
      {
        print("Left Distance: ");
        if (ultrasound_left_distance > 1250) 
        { 
          ultrasound_left_distance = ULTRASOUND_DISTANCE_MIN; 
        } // If the result is bugged set it to min (it is bugged at below 3cm)
        print(ultrasound_left_distance);
        println(" cm");
      }
    }


    if (linefollower_enabled) 
    {
      infrared_values[0] = analogRead(irA1); // RIGHT
      infrared_values[1] = analogRead(irA2);
      infrared_values[2] = analogRead(irA3);
      infrared_values[3] = analogRead(irA4);
      infrared_values[4] = analogRead(irA5);
      infrared_values[5] = analogRead(irA6);
      infrared_values[6] = analogRead(irA7);
      infrared_values[7] = analogRead(irA8); // LEFT
      for (int i = 7; i>=0; i--) 
      {
        if (infrared_values[i] < INFRARED_REFERENCE) 
        {
          line_detected[i] = true;
        } 
        else 
        {
          line_detected[i] = false;
        }
      }   
      
      if (line_detected[0] && line_detected[1] && line_detected[2]) {
        left_detected = true;
      }
      if (line_detected[3] && line_detected[4]) {
        middle_detected = true;
      }
      if (line_detected[5] && line_detected[6] && line_detected[7]) {
        right_detected = true;
      }

      if (!left_detected && !middle_detected && !right_detected) {
        currentMovement = stopped;
      } else {
        // Something is detected
        if (middle_detected && right_detected) {
          // Turn right with respect to the algorithm
          linefollow_turn_right_conditional();
        } else if (middle_detected && left_detected) {
          // Turn left with respect to the algorithm
          linefollow_turn_left_conditional();
        } else if (middle_detected) {
          // Just go forward
          currentMovement = going_forward;
        }
      }
  }
  movementRoutine(); // Should run every loop()
  delay(DELAY_MS);
}

// Kacinci sagdan donulecegne gore saga donme rutinini aktiflestiren fonksiyon
void linefollow_turn_right_conditional() {
   if (currentMovement == going_forward) {
      right_turns++;
      if (target_right_turns == right_turns) {
        linefollow_turn_right_routine();
      }
   }
}

// Kacinci soldan donulecegine gore sola donme rutnini aktiflestiren fonksiyon
void linefollow_turn_left_conditional() {
  if (currentMovement == going_forward) {
      left_turns++;
      if (target_left_turns == left_turns) {
         linefollow_turn_left_routine();
      }
  }
}

void linefollow_turn_right_routine() {
    /*
     * Robot bu esnade infrared ile ORTA ve SAG taraftan sinyal almakta
     * Bu rutin sonunda robot SAGA donmus ve sadece ORTA taraftan sinyal almakta olmalidir 
     */
}

void linefollow_turn_left_routine() {
    /*
     * Robot bu esnada infrared ile ORTA ve SOL taraftan sinyal almakta
     * Bu rutin sonunda robot SOLA donmus ve sadece ORTA taraftan sinyal almakta olmalidir
     */
}

void linefollow_turn_full_routine() {
    /*
     * Robot bu esnada kutuyu almis bulunmakta
     * Bu rutin sonunda kutu tasir halde iken 180* donmus ve sadece ORTA taraftan sinyal almakta olmaldir
     */
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
    digitalWrite(in4,  HIGH); 
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
    digitalWrite(in4,  HIGH);
  }
  if (currentSpeed == yavas) {
    delay(5);
  } else if (currentSpeed == normal) {
    delay(10);
  } else if (currentSpeed == hizli) {
    delay(18);
  }
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); 
}

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

String readString() {
    #if DEBUG == 1
      return Serial.readString();
    #else
      return Serial1.readString();
    #endif 
}

void close_hand() {
  hand_right.write(90); // Initial 180
  hand_left.write(90); // Initail 0
  
}

void open_hand() {
  hand_right.write(179); // Initial 180
  hand_left.write(0); // Initail 0
}

void lower_arm() {
  arm_right.write(30); // Initiail 10
  arm_left.write(150); // Initial 170
  
}

void raise_arm() {
  arm_right.write(170); // Initial 10
  arm_left.write(10); // Initial 170
}

void turn_right(int value) {
  wheelsTurn(value, 1, value, 2);
}

void turn_left(int value) {
  wheelsTurn(value, 2, value, 1);
}

void go_forward(int value) {
  wheelsTurn(value, 1, value, 1);
}

void go_backward(int value) {
  wheelsTurn(value, 2, value, 2);
}

void box_pickup_routine() {
  lower_arm();
  delay(1500);
  close_hand();
  delay(1500);
  raise_arm();
  delay(1500);  
}

void box_drop_routine(int shelf) {
  /*
   * Kutuyu rafa birakma rutini
   * 
   * 
   */
}

void printStatus() {
  println("Robot status:");
  if (linefollower_enabled) {
    println("Line Following enabled.");
  }
  if (ultrasound_enabled) {
    println("Ultrasound enabled.");
  }
  print("MOVEMENT: ");
  println(currentMovement);
  print("SPEED: ");
  println(currentSpeed);
}

// ASAGIDAKI KODLAR ESKI HAREKET MEKANIZMASINA AITTIR
void wheelsTurn(int speedLeftPair, enum direction directionLeftPair, int speedRightPair, enum direction directionRightPair) {
  /*
  speedLeftPair -> 0 .. 255
  speedRightPair -> 0 .. 255
  directionLeftPair -> FORWARD | BACKWARD
  directionRightPair -> FORWARD | BACKWARD
  */
  print(speedLeftPair);
  print(directionLeftPair);
  print(speedRightPair);
  print(directionRightPair);
  if (directionLeftPair == forward) {
    analogWrite(enA,  speedLeftPair);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    analogWrite(enA,  speedLeftPair);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  // Right Pair
  if (directionRightPair == forward) {
    analogWrite(enB, speedRightPair); 
    digitalWrite(in3, LOW);
    digitalWrite(in4,  HIGH); 
  } else {
    analogWrite(enB, speedRightPair); 
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW); 
  }
}

void stop() 
{
    analogWrite(enA,  0);  
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);  
    analogWrite(enB, 0); 
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW); 
}
