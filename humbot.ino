#include <MeMCore.h>
#include <SoftwareSerial.h>

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

MeUltrasonicSensor ultraSensor(PORT_4);
SoftwareSerial bluetooth(10, 11); // RX, TX

const int BUFFER_SIZE = 64; // Adjust this based on your message length
const float SPEED_MULT = 0.5;

char buffer[BUFFER_SIZE];
int index = 0;

void setup() {
  Serial.begin(9600);     // Serial communication with the USB
  //BTSerial.begin(9600);   // Serial communication with Bluetooth module
}

void loop() {
  readSerialPort();
  readSonicDistanceSensor();
  delay(50);
}

void readSonicDistanceSensor() {
  Serial.print("Distance : ");
  Serial.print(ultraSensor.distanceCm() );
  Serial.println(" cm");
}

void readBluetooth() {
  /*
  if (BTSerial.available()) {
    char c = BTSerial.read();
    Serial.print(c);
  } 

  if (bluetooth.available() > 0) {
    char command = bluetooth.read();
    //executeCommand(command);
  }
  */
}

void readSerialPort() {
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();

    if (receivedChar == '\n') {
      // End of message reached, process the complete message
      buffer[index] = '\0'; // Null-terminate the string
      processMessage(buffer);

      // Reset buffer for the next message
      index = 0;
    } else {
      // Add the character to the buffer
      if (index < BUFFER_SIZE - 1) {
        buffer[index++] = receivedChar;
      }
    }
  }
}


void processMessage(const char *message) {
  Serial.print("Received Message: ");
  Serial.println(message);

  if(strcmp(message, "forward") == 0) {
    Serial.print("Moving forward");
    forward();
  }
  if(strcmp(message, "backward") == 0) {
    Serial.print("Moving backward");
    backward();
  }
  if(strcmp(message, "left") == 0) {
    Serial.print("Moving left");
    turnLeft();
  }
  if(strcmp(message, "right") == 0) {
    Serial.print("Moving right");
    turnRight();
  }
  if(strcmp(message, "stop") == 0) {
    Serial.print("Stopping");
    stop();
  }
}

void forward() {
  motor1.run(-255 * SPEED_MULT);
  motor2.run(255 * SPEED_MULT);
}

void backward() {
  motor1.run(255 * SPEED_MULT);
  motor2.run(-255 * SPEED_MULT);
}

void turnLeft() {
  motor1.run(255 * SPEED_MULT);
  motor2.run(255 * SPEED_MULT);
}

void turnRight() {
  motor1.run(-255 * SPEED_MULT);
  motor2.run(-255 * SPEED_MULT);
}

void stop() {
  motor1.run(0);
  motor2.run(0);
}
