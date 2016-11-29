#include "UDPDiscover.hpp"
#include "LightConfig.hpp"

/*
 *	discover packet
 *		nbyte		content			len		
 *		0-7			"DISCOVER"		8
 *		8-23		TYPE			16
 *	response packet
 *		nbyte		content			len
 *		0-7			"I'M HERE"		8
 *		8-23		TYPE			16
 *		24-55		name			32
 */

#include "UDPDiscover.hpp"

static uint16_t constexpr port = 25187;

extern "C" {
	struct discover_packet {
		char header[8];
		char type[16];
		char name[32];
	};
}

UDPDiscoverClass::UDPDiscoverClass()
{
}

void UDPDiscoverClass::begin()
{
	udp.begin(port);
}

void UDPDiscoverClass::loop()
{
	struct discover_packet packet;
	if (udp.parsePacket() == 0)
		return;
	IPAddress ip = udp.remoteIP();
	uint16_t  port = udp.remotePort();
	if (udp.read((char*)&packet, 24) != 24)
		return;
	if (memcmp(packet.header, "DISCOVER", 8) != 0)
		return;
	if (packet.type[0] != '\0' && strcmp(packet.type, "Light") != 0)
		return;
	strcpy(packet.header, "I'M HERELight");
	memset(packet.name, 0, 32);
	strcpy(packet.name, LightConfig.name());
	udp.beginPacket(ip, port);
	udp.write((char*)&packet, 56);
	udp.endPacket();
}

UDPDiscoverClass UDPDiscover;

