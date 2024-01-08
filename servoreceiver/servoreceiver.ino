#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define SIGNAL_TIMEOUT 3000 // This is signal timeout in milliseconds. We will reset the data if no signal

unsigned long lastRecvTime = 0;

struct PacketData {
  byte servoValue;
  byte servo1Value;
  byte servo2Value;
  byte servo3Value;

};

PacketData receiverData;
Servo servo;
Servo servo1;
Servo servo2;
Servo servo3;


// Assign default input received values

void mapAndWriteValues() {
  Serial.println("Servo 1");
  servo.write(receiverData.servoValue);
  Serial.println(receiverData.servoValue);
  Serial.print("");


  Serial.println("Servo 2");
  servo1.write(receiverData.servo1Value);
  Serial.println(receiverData.servo1Value);
  Serial.print("");

  Serial.println("Servo 3");
  servo2.write(receiverData.servo2Value);
  Serial.println(receiverData.servo2Value);
  Serial.print("");

  Serial.println("Servo 4");
  servo3.write(receiverData.servo3Value);
  Serial.println(receiverData.servo3Value);
  Serial.print("");


}


// Callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  if (len == 0) {
    return;
  }
  memcpy(&receiverData, incomingData, sizeof(receiverData));
  mapAndWriteValues();
  lastRecvTime = millis();
}

void setUpPinModes() {
  servo.attach(27); // Assuming the servo is connected to GPIO pin 32
  servo1.attach(26);
  servo2.attach(25); // Assuming the servo is connected to GPIO pin 32
  servo3.attach(33);

  mapAndWriteValues();
}

void setup() {
  setUpPinModes();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Check for signal loss
  unsigned long now = millis();
  if (now - lastRecvTime > SIGNAL_TIMEOUT) {
    
    mapAndWriteValues();
    
  }
}
