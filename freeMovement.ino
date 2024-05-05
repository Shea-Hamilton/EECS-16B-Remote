#include <IRremote.h>

#define LEFT_MOTOR                  5 
#define LEFT_ENCODER                2
#define RIGHT_MOTOR                 6
#define RIGHT_ENCODER               3

#define FORWARD_SPEED               1000
#define REVERSE_SPEED               
#define TURN_SPEED                  150
#define SLOW_RIGHT_SPEED            165

const int greenLed = 11;
const int redLed = 8;
const int yellowLed = 7;
const int trigPin = 13;
const int echoPin = 12;
const int buzzerPin = 9;

IRrecv IR(10);

unsigned long lastIRCode = 0;



void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  pinMode(LEFT_MOTOR, OUTPUT);
  pinMode(RIGHT_MOTOR, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  IR.enableIRIn();
  Serial.begin(38400);
}

void loop() {
  if (IR.decode()) {
    unsigned long irCode = IR.decodedIRData.decodedRawData;
    // Only update lastIRCode if it's different from the last received command
    if (irCode != lastIRCode) {
      lastIRCode = irCode;
    }
    IR.resume();
  }
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  // Calculate distance in cm
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  // Check if the distance is less than a threshold
  if (distance < 43) { // Adjust the threshold distance as needed
    digitalWrite(LEFT_MOTOR, LOW);
    digitalWrite(RIGHT_MOTOR, LOW);
    turnAround();
  } else {
    // If no obstacle detected, execute the last received command
    executeCommand(lastIRCode);
  }
}

void executeCommand(unsigned long irCode) {
  switch(irCode) {
    case 3125148420: // UP button
      driveForward();
      break;
    case 3091725060: // LEFT button
      turnLeft();
      break;
    case 3075013380: // RIGHT button
      turnRight();
      break;
    case 3108436740: // Horn
      triggerBuzzer();
      break;
    case 3141860100: // Stop
      stopMotors();
      break;
    default:
      // Do nothing or handle other cases
      break;
  }
 
}

void driveForward() {
  digitalWrite(LEFT_MOTOR, HIGH);
  analogWrite(RIGHT_MOTOR, SLOW_RIGHT_SPEED); 
  digitalWrite(redLed, LOW);
  digitalWrite(yellowLed,LOW);
  digitalWrite(greenLed, HIGH);
}

void turnLeft() {
  digitalWrite(LEFT_MOTOR, LOW);
  digitalWrite(RIGHT_MOTOR, HIGH);
  digitalWrite(redLed, LOW);
  digitalWrite(yellowLed,LOW);
  digitalWrite(greenLed, HIGH);
}

void turnRight() {
  digitalWrite(RIGHT_MOTOR, LOW);
  digitalWrite(LEFT_MOTOR, HIGH);
  digitalWrite(redLed, LOW);
  digitalWrite(yellowLed,LOW);
  digitalWrite(greenLed, HIGH);
}
void turnAround() {
  digitalWrite(LEFT_MOTOR, LOW);
  digitalWrite(RIGHT_MOTOR, HIGH);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed,HIGH);
  delay(350);
  digitalWrite(yellowLed,LOW);
}
void stopMotors() {
  digitalWrite(LEFT_MOTOR, LOW);
  digitalWrite(RIGHT_MOTOR, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed,LOW);
  digitalWrite(redLed, HIGH);
}
void triggerBuzzer() {
  // Activate the buzzer
  digitalWrite(buzzerPin, HIGH);
  delay(100); // Buzzer duration (adjust as needed)
  digitalWrite(buzzerPin, LOW);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);
}
