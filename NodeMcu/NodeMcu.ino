// #include <ArduinoDMX.h>

// #include <ArduinoRS485.h>


#include <SoftwareSerial.h>
#include <Wire.h>

#define RE D8
#define DE D7

// #define RO D4
// #define DI D5


const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xB5, 0xCC};
const byte phos[] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xE4, 0x0C};
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};

// const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
// const byte phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
// const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

// byte values[8]; // Only 7 bytes are expected in response
HardwareSerial& modbus = Serial1; // Use Serial1 for ESP8266
byte values[7];
SoftwareSerial mod (RE, DE);

void setup() {
  Serial.begin(9600);
  mod.begin(4800); // Adjust baud rate based on sensor specifications
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  // digitalWrite(RE, LOW);
  // digitalWrite(DE, LOW);
  // pinMode(RO, OUTPUT);
  // pinMode(DI, OUTPUT);


}

bool isSensorConnected() {
  delay(10);

  // Send a simple test command (modify based on sensor documentation)
  Serial1.flush();
  byte testCmd[] = {0x01, 0x00}; // Replace with a command known to return a response
  Serial.println("01");
  if (mod.write(testCmd, sizeof(testCmd)) != sizeof(testCmd)) {
    Serial.println("02");
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    return false; // Write failed, potentially indicating a disconnected sensor
  }

  delay(1000); // Allow sensor some time to respond
  Serial1.flush();
  Serial.println("03");
  // Check for at least one byte of response (modify based on expected response format)
  if (modbus.available() == 0) {
    Serial.println("04");
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    return false; // No response received, potentially indicating a disconnected sensor
  }

  Serial1.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  Serial.println("05");
  return true; // Write successful, response received (potential connection)
}


void loop() {
  Serial.println(isSensorConnected());
  byte val1, val2, val3;

  // Read and display Nitrogen value
  val1 = nitrogen();
  Serial.print("Nitrogen: ");
  Serial.println(val1);  // Assuming data format is integer (modify if necessary)
  delay(2500);

  // Read and display Phosphorus value
  // val2 = phosphorous();
  // Serial.print("Phosphorous: ");
  // Serial.println(val2);  // Assuming data format is integer (modify if necessary)
  // delay(250);

  // // // // Read and display Potassium value
  // val3 = potassium();
  // Serial.print("Potassium: ");
  // Serial.println(val3);  // Assuming data format is integer (modify if necessary)
  // delay(5000);
}

byte nitrogen() {
  // clear the receive buffer
  // mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);

  // write out the message
  for (uint8_t i = 0; i < sizeof(nitro); i++ ) mod.write( nitro[i] );

  // wait for the transmission to complete
  mod.flush();
  
  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(200);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  return values[4];
}

 
byte phosphorous() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (Serial1.write(phos, sizeof(phos)) == 8) {
    Serial1.flush();
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      //Serial.print(Serial1.read(),HEX);
      values[i] = Serial1.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}

byte potassium() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (Serial1.write(pota, sizeof(pota)) == 8) {
    Serial1.flush();
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 7; i++) {
      //Serial.print(Serial1.read(),HEX);
      values[i] = Serial1.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  return values[4];
}

int hexToDec(byte b) {
  if (b >= '0' && b <= '9') {
    return b - '0';
  } else if (b >= 'A' && b <= 'F') {
    return b - 'A' + 10;
  } else {
    // Handle invalid characters (optional)
    return -1;
  }
}
