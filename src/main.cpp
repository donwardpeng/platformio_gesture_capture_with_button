#include <Arduino_LSM9DS1.h>

const int numSamples = 119;

const int buttonPin = 12;     // the number of the pushbutton pin
const int ledPin =  11;      // the number of the LED pin

int buttonState = 0;    
int samplesRead = numSamples;
float ax_buffer[120];
float ay_buffer[120];
float az_buffer[120];
float gx_buffer[120];
float gy_buffer[120];
float gz_buffer[120];

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // print the header
  Serial.println("aX,aY,aZ,gX,gY,gZ");
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;

  // wait for significant motion
  while (samplesRead == numSamples) {
    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPin);
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
      // turn LED on:
      digitalWrite(ledPin, HIGH);
      samplesRead = 0;
      Serial.println();
      Serial.print("Button pushed event");
      buttonState = 0;
    } else {
      // turn LED off 
      digitalWrite(ledPin, LOW);
    }
  }

  // check if the all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) {
    // check if both new acceleration and gyroscope data is
    // available
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      // read the acceleration and gyroscope data
      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      samplesRead++;
      ax_buffer[samplesRead] = aX;
      ay_buffer[samplesRead] = aY;
      az_buffer[samplesRead] = aZ;
      gx_buffer[samplesRead] = gX;
      gy_buffer[samplesRead] = gY;
      gz_buffer[samplesRead] = gZ;
            
      if (samplesRead == numSamples) {
        for(int i = 0; i <= numSamples; i++){
          Serial.print(i);
          Serial.print(',');
          Serial.print(ax_buffer[i], 3);
          Serial.print(',');
          Serial.print(ay_buffer[i], 3);
          Serial.print(',');
          Serial.print(az_buffer[i], 3);
          Serial.print(',');
          Serial.print(gx_buffer[i], 3);
          Serial.print(',');
          Serial.print(gy_buffer[i], 3);
          Serial.print(',');
          Serial.print(gz_buffer[i], 3);
          Serial.println();
        }
        // add an empty line if it's the last sample        
        Serial.println();
      }
    }
  }
}
