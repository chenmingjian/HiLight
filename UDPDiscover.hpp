#ifndef __UDPDISCOVER_HPP__
#define __UDPDISCOVER_HPP__

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class UDPDiscoverClass {
public:
	UDPDiscoverClass();
	void begin();
	void loop();
private:
	WiFiUDP udp;
};

extern UDPDiscoverClass UDPDiscover;

#endif