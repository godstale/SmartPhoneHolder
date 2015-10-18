#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3);  // write pins for BLE's TX, RX

int mosPin = 5;
boolean turnOn = false;

#define BUFFER_SIZE 3
char Buffer[BUFFER_SIZE] = {0x00,0x00,0x00};

unsigned long mosUpdateTime = 0;


void setup() {
  pinMode(13, OUTPUT);
  pinMode(mosPin, OUTPUT);
  
  Serial.begin(9600);
  btSerial.begin(9600);
  
  digitalWrite(13, LOW);
  digitalWrite(mosPin, LOW);
  
  Serial.println("Hello world~!!");
}

void loop() {
  while (btSerial.available() > 0) {
    char command = btSerial.read();
    Serial.write(command);
    
    // Add received byte to buffer
    for(int i=0; i<BUFFER_SIZE-1; i++) {
      Buffer[i] = Buffer[i+1];
    }
    Buffer[BUFFER_SIZE-1] = command;
    
    // check command
    if(Buffer[0] == 'O') {
      if(Buffer[1] == '2' && Buffer[2] == '0') {
        turnOn = false;
        mosUpdateTime = millis();
      } else if(Buffer[1] == '2' && Buffer[2] == '1') {
        turnOn = true;
        mosUpdateTime = millis();
      }
    }
  }  // End of if (btSerial.available() > 0)
  
  if(millis() - mosUpdateTime > 2000) {
    setScreen(turnOn);
    mosUpdateTime = millis();
  }
  
}  // End of loop


void setScreen(boolean isOn) {
  if(isOn) {
    digitalWrite(13, HIGH);
    digitalWrite(mosPin, HIGH);
    Serial.println("Turn on screen !!");
  } else {
    digitalWrite(13, LOW);
    digitalWrite(mosPin, LOW);
    Serial.println("Turn off screen !!");
  }
}

