#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t receiverAddress[] = {0xA8, 0x42, 0xE3, 0xCA, 0xC6, 0xC8};

// Structure to send data
struct PackageData {
  byte servoValue;
  byte servo1Value;
  byte servo2Value;
  byte servo3Value;

};

PackageData data;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(35, INPUT);
  pinMode(33, INPUT);
  pinMode(34, INPUT);
  pinMode(32, INPUT);
  

}

void loop() {
  int potValue = analogRead(35); // Read the analog value from the first potentiometer
  int servoValue = map(potValue, 0, 4093, 0, 180); // Map the potentiometer value to servo range (0-180)

  data.servoValue = servoValue; // Assign the servo value to the data structure
  Serial.print("Poten 1: ");
  Serial.print(servoValue);
  Serial.print("\n");

  int pot1Value = analogRead(34); // Read the analog value from the second potentiometer
  int servo1Value = map(pot1Value, 0, 4093, 0, 180); // Map the potentiometer value to servo range (0-180)

  data.servo1Value = servo1Value; // Assign the second potentiometer's servo value to the data structure
  Serial.print("Poten 2: ");
  Serial.print(servo1Value);
  Serial.print("\n");

  int pot2Value = analogRead(33); // Read the analog value from the second potentiometer
  int servo2Value = map(pot2Value, 0, 4093, 0, 180); // Map the potentiometer value to servo range (0-180)

  data.servo2Value = servo2Value; // Assign the second potentiometer's servo value to the data structure
  Serial.print("Poten 3: ");
  Serial.print(servo2Value);
  Serial.print("\n");

  int pot3Value = analogRead(32); // Read the analog value from the second potentiometer
  int servo3Value = map(pot3Value, 0, 4093, 0, 180); // Map the potentiometer value to servo range (0-180)

  data.servo3Value = servo3Value; // Assign the second potentiometer's servo value to the data structure
  Serial.print("Poten 4: ");
  Serial.print(servo3Value);
  Serial.print("\n");



  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *)&data, sizeof(data));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }

  delay(10); // Adjust the delay according to your requirement
}
