//Controller
//ESPのRXDとXbeeの３ピン（DIN）をつなぐと何故か動く．
#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
	#include <espnow.h>
	#include <user_interface.h>
}

#define WIFI_DEFAULT_CHANNEL 1

//SlaveのSOFTAP_IFのMACアドレスを書く
uint8_t mac[] = {0x5E,0xCF,0x7F,0x85,0x07,0xBE};

void setup() {
	//シリアル通信の速度変更はここを書き換える．
	Serial.begin(19200);
	WiFi.mode(WIFI_STA);

  //This pilot lamp expressing activation.
	//pinMode(13, OUTPUT);

	if (esp_now_init()==0) {
		//Serial.println("direct link  init ok");
	} else {
		//Serial.println("dl init failed");
		ESP.restart();
		return;
	}

	esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
	esp_now_register_recv_cb([](uint8_t *macaddr, uint8_t *data, uint8_t len) {
	});

	esp_now_register_send_cb([](uint8_t* macaddr, uint8_t status) {
	});

	//uint8_t message[] = { 0x42};
  //esp_now_send(mac, message, sizeof(message));

	int res = esp_now_add_peer(mac, (uint8_t)ESP_NOW_ROLE_SLAVE,(uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);
}

void loop() {
  delay(1);
  if(Serial.available() > 0){
    int input = Serial.read();
    uint8_t rcv_data = input;
    esp_now_send(mac, &rcv_data, sizeof(rcv_data));
  }
}
