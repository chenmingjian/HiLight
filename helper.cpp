#include <Arduino.h>
#include <md5.h>
#include "helper.hpp"
#include "Constant.hpp"

static char _ssid[NAME_PRIFIX_LEN + 1 + 8 + 1] = {0};		//	CMGNAKED-FFFFFFFF

static const char _prefix[] = {0xb3, 0xc2, 0xc3, 0xf7, 0xbc, 0xfc};
static const char _suffix[] = {0xc2, 0xe3, 0xcc, 0xe5};

static inline char _hex_lower(int x) {
	if (x <= 9)
		return x + '0';
	else
		return x + 'a' - 10;
}

char* Helper::convertPasswd(char *buffer /* need 33 bytes */, const char* raw_passwd) {
	md5_context_t ctx;
	MD5Init(&ctx);
	MD5Update(&ctx, (uint8_t*)_prefix, 6);
	MD5Update(&ctx, (uint8_t*)raw_passwd, strlen(raw_passwd));
	MD5Update(&ctx, (uint8_t*)_suffix, 4);
	MD5Final((uint8_t*)buffer, &ctx);
	MD5Init(&ctx);
	MD5Update(&ctx, (uint8_t*)_prefix, 6);
	MD5Update(&ctx, (uint8_t*)buffer, 16);
	MD5Update(&ctx, (uint8_t*)_suffix, 4);
	MD5Final((uint8_t*)buffer, &ctx);
	for (int i = 15, j = 30; i >= 0; i--, j-=2) {
		char ch = buffer[i];
		buffer[j] = _hex_lower(ch >> 4);
		buffer[j+1] = _hex_lower(ch & 0x0f);
	}
	buffer[32] = '\0';
	return buffer;
}

const char* Helper::getDeviceId() {
	getWiFiSSID();
	return _ssid + NAME_PRIFIX_LEN + 1;
}

const char* Helper::getWiFiSSID() {
	if (!_ssid[0])
		sprintf(_ssid, "%s-%06x%02x", NAME_PRIFIX, ESP.getChipId(), ESP.getFlashChipId() & 0xff );
	return _ssid;
}

