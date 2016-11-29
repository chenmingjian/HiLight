#include "WiFiTask.hpp"
#include "LightConfig.hpp"
#include "helper.hpp"
#include <Arduino.h>
#include <ESP8266WiFi.h>

WiFiTaskClass::WiFiTaskClass() {

}

void WiFiTaskClass::begin() {
	LightConfig.print("WiFiTaskClass::begin");
}

void WiFiTaskClass::loop() {
	if (LightConfig.wifiChanged())
	{
		if (LightConfig.mode() == 0)
		{
			Serial.println("AP mode");
			WiFi.mode(WIFI_AP);
			char pwd[33];
			const char *ssid;
			ssid = Helper::getWiFiSSID();
			WiFi.softAP(ssid, Helper::convertPasswd(pwd, ssid));
		}
		else
		{
			Serial.println("STA mode");
			WiFi.mode(WIFI_STA);
			WiFi.begin(LightConfig.wifiSSID(), LightConfig.wifiPasswd());
		}
	}
}

WiFiTaskClass WiFiTask;