#include <Stepper.h>

//variable to change accordingly 
const int stepsPerRevolution = 200; // change this to fit the number of steps per revolution
const int stepsPerRotation = 2048;
const int rotation = 1; //number of rotation when pressure detects
const int rpm = 60; //set rpm of the motor
const int pressureLimit = 15; //crosses this limit will rotate the motor to tie
const int stepUntie = 500; //step per press for untie button

//pin number initialization
const int buttonTiePin = 2; //Tie button input pin
const int buttonUntiePin = 3; //Untie button input pin
const int FSR_Pin = A0; //analog pin 0 for pressure reading

//variable for checking and others
int FSRReading = 0; //Force sensor reading
bool tied = false; //for checking if it is tied or not
int countStep = 0; //count of steps of motor
int buttonStateTie = 0; //check if the tie button is pressed
int buttonStateUntie = 0; //check if the intie button is pressed

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  // set the speed at rpm:
  myStepper.setSpeed(rpm);

  //set pin as INPUT
  pinMode(buttonTiePin, INPUT);
  pinMode(buttonUntiePin, INPUT);

  //setting the INPUT pin HIGH
  digitalWrite(buttonTiePin, HIGH);
  digitalWrite(buttonUntiePin, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:

  FSRReading = analogRead(FSR_Pin);
  buttonStateTie = digitalRead(buttonTiePin);
  buttonStateUntie = digitalRead(buttonUntiePin);
  

 //when pressure will we detected
  if(FSRReading > pressureLimit && !tied){
    Serial.println(FSRReading);
     Serial.println("Tied using sensor");
    if(countStep == 0){
      Serial.println("Full Rotation needed");
      myStepper.step(rotation * stepsPerRotation);  
    }else if(countStep < 0){
      Serial.println("More than Full Rotation needed");
      myStepper.step((rotation * stepsPerRotation) + abs(countStep));  
    }else{
      Serial.println("Less Than Full Rotation needed");
      myStepper.step((rotation * stepsPerRotation) - countStep);  
    }
    countStep = rotation * stepsPerRotation;
    tied = true;
    Serial.println(countStep);
  }

  //reseting tied state when no pressure detects
  if(FSRReading < pressureLimit && tied && countStep < (rotation * stepsPerRotation)){
    Serial.println("Shoe Removed");
    tied = false;
  }

  //when tie button will be pressed
  if(buttonStateTie == LOW){
    Serial.println("Tie Button");
    myStepper.step(stepsPerRevolution);
    countStep += stepsPerRevolution;
    Serial.println(countStep);
  }

  //when untie button will be pressed
  if(buttonStateUntie == LOW && countStep > 0){
    Serial.println("Untie Button");
    if(countStep < stepUntie){
      myStepper.step(-countStep);
      countStep-=countStep;
    }else{
      myStepper.step(-stepUntie);
      countStep-=stepUntie;
    }
    Serial.println(countStep);
  }
  
}
