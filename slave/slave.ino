//Slave
#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
  #include <espnow.h>
  #include <user_interface.h>
}

#define WIFI_DEFAULT_CHANNEL 1

//ControllerのSTATION_IFのMACアドレスを書く
uint8_t mac[] = {0x5C,0xCF,0x7F,0x84,0xD5,0x18};

void setup() {
  //シリアル通信の速度変更はここを書き換える．
  Serial.begin(19200);
  delay(10);

  //This pilot lamp expressing activation.
  //pinMode(13, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("foobar", "12345678", 1, 0);

  if (esp_now_init() == 0) {
    //Serial.println("init");
  } else {
    //Serial.println("init failed");
    ESP.restart();
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);

  esp_now_register_recv_cb([](uint8_t *macaddr, uint8_t *data, uint8_t len) {
    for (int i = 0; i < len; i++) {
      Serial.write(data[i]);
    }
  });

  esp_now_register_send_cb([](uint8_t* macaddr, uint8_t status) {
  });

  int res = esp_now_add_peer(mac, (uint8_t)ESP_NOW_ROLE_CONTROLLER,(uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);
}

void loop() {
}
