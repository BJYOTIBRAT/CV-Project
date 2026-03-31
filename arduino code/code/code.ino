#include <Servo.h>

Servo xServo; // Servo object for the X-axis servo
Servo yServo; // Servo object for the Y-axis servo

int xPin = 10; // Pin for the X-axis servo
int yPin = 11; // Pin for the Y-axis servo

const int sound1Pin = A0; // AO pin of the first microphone
const int sound2Pin = A1; // AO pin of the second microphone

int xCurrentAngle = 90; // Initial position of the X-axis servo
int yCurrentAngle = 90; // Initial position of the Y-axis servo

int stepDelay = 10; // Delay for smooth movement (in milliseconds)
int soundThreshold = 195; // Threshold for sound intensity

void setup() {
  xServo.attach(xPin); // Attach the X-axis servo to pin 10
  yServo.attach(yPin); // Attach the Y-axis servo to pin 11

  xServo.write(xCurrentAngle); // Set initial position to 90 degrees
  yServo.write(yCurrentAngle); // Set initial position to 90 degrees

  Serial.begin(9600); // Start serial communication
}

void smoothMove(Servo &servo, int &currentAngle, int targetAngle) {
  // Gradually move the servo to the target angle
  while (currentAngle != targetAngle) {
    if (currentAngle < targetAngle) {
      currentAngle++;
    } else if (currentAngle > targetAngle) {
      currentAngle--;
    }
    servo.write(currentAngle); // Move servo step by step
    delay(stepDelay); // Small delay for smooth movement
  }
}

void loop() {
  // Check if data is available on the serial port
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n'); // Read incoming data until newline
    int commaIndex = data.indexOf(','); // Find the comma separating X and Y values

    if (commaIndex != -1) {
      // Extract and convert X and Y angles
      int xAngle = data.substring(0, commaIndex).toInt();
      int yAngle = data.substring(commaIndex + 1).toInt();

      // Validate the received angles are within the 0-180 range
      if (xAngle >= 0 && xAngle <= 180 && yAngle >= 0 && yAngle <= 180) {
        Serial.print("Received X: ");
        Serial.print(xAngle);
        Serial.print(" | Y: ");
        Serial.println(yAngle);

        // Smoothly move the servos to the new angles
        smoothMove(xServo, xCurrentAngle, xAngle);
        smoothMove(yServo, yCurrentAngle, yAngle);
      } else {
        Serial.println("Invalid Angles Received!");
      }
    }
  }

  // Read analog values from sound sensors
  int sound1Value = analogRead(sound1Pin);
  int sound2Value = analogRead(sound2Pin);

  // Log the readings of both sensors
  Serial.print("Sensor 1: ");
  Serial.print(sound1Value);
  Serial.print(" | Sensor 2: ");
  Serial.println(sound2Value);

  // Check sound levels and move the X-axis servo accordingly
  if (sound1Value > soundThreshold) {
    Serial.println("Sensor 1 triggered: Moving X Servo to 45 degrees");
    smoothMove(xServo, xCurrentAngle, 45);
  } else if (sound2Value > soundThreshold) {
    Serial.println("Sensor 2 triggered: Moving X Servo to 135 degrees");
    smoothMove(xServo, xCurrentAngle, 135);
  }

  delay(500); // Wait for 500ms before the next reading
};
