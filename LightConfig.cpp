#include "LightConfig.hpp"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include "helper.hpp"

int strcpy_s(char * dest, size_t destsz, const char * src) {
	int i;
	for (i = 0; i < destsz && src[i]; i++)
		dest[i] = src[i];
	if (i == destsz)
	{
		dest[0] = '\0';
		return -1;
	}
	return 0;
}

size_t strnlen_s( const char * str, size_t strsz ) {
	int i;
	for (i = 0; i < strsz && str[i]; i++);
	return i;
}


bool LightConfigClass::_spiffsBegin() {
  if (!_spiffsBegan)
    _spiffsBegan = SPIFFS.begin();
  return _spiffsBegan;
}

bool LightConfigClass::save() {
	if (SPIFFS.exists(CONFIG_FILE_PATH))
		SPIFFS.remove(CONFIG_FILE_PATH);
	StaticJsonBuffer<CONFIG_JSON_BUF_MAX> jbuf;
	JsonObject &j_root = jbuf.createObject();
	j_root["mode"] = _mode;
	if (_mode != 0) {
		j_root["wifi_ssid"] = _wifi_ssid;
		j_root["wifi_passwd"] = _wifi_passwd;
	}
	j_root["name"] = _name;
	j_root["passwd"] = _passwd;
	j_root["startup_color"] = colorToUint32(_startup_color);
	char jstr[CONFIG_JSON_TEXT_MAX+1];
	if (j_root.measureLength() > CONFIG_JSON_TEXT_MAX) return false;
	j_root.printTo(jstr, CONFIG_JSON_TEXT_MAX+1);
	File file = SPIFFS.open(CONFIG_FILE_PATH, "w");
	if (!file)
	{
		Serial.println("Open file for write failed");
		return false;
	}
	file.print(jstr);
	file.close();
	return true;
}

bool LightConfigClass::defaultConfig() {
	
	_mode = 0;
	strcpy(_name, Helper::getWiFiSSID());
	_passwd[0] = '\0';
	_startup_color = 0x00ffffff;
	return save();
}

bool LightConfigClass::load() {
	_wifi_changed = true;
	_spiffsBegin();
	File file = SPIFFS.open(CONFIG_FILE_PATH, "r");
	if (!file) {
		Serial.println("Use the default config");
		return defaultConfig();
	}
	if (file.size() > CONFIG_JSON_BUF_MAX) {
		Serial.println("File is too large");
		return false;
	}
	char buffer[CONFIG_JSON_TEXT_MAX + 1];
	file.readBytes(buffer, file.size());
	file.close();
	StaticJsonBuffer<CONFIG_JSON_BUF_MAX> jbuffer;
	JsonObject& root = jbuffer.parseObject(buffer);
	if(!root.success()) {
		Serial.println("Json parse failed");
		return false;
	}
	int 	local_mode			= root["mode"];
	String 	local_wifi_ssid 	= root["wifi_ssid"];
	String 	local_wifi_passwd 	= root["wifi_passwd"];
	String 	local_name 			= root["name"];
	String 	local_passwd 		= root["passwd"];
	uint32_t local_color_value	= root["startup_color"];
	if (local_mode != 0 && local_mode != 1) {
		Serial.print("Mode error: ");
		Serial.println(local_mode);
		return false;
	}
	if (local_mode == 1) {
		if (local_wifi_ssid.length() > WIFI_SSID_MAX) {
			Serial.println("SSID too long");
			return false;
		}
		if (local_wifi_passwd.length() > WIFI_PASSWD_MAX) {
			Serial.println("WiFi passwd too long");
			return false;
		}
	}
	if (local_name.length() > NAME_MAX) {
		Serial.println("Name too long");
		return false;
	}
	if (local_passwd.length() != PASSWD_LEN && local_passwd.length() != 0) {
		Serial.println("Passwd length error");
		return false;
	}
	_mode = local_mode;
	strcpy_s(_wifi_ssid, 	WIFI_SSID_MAX+1 , 	local_wifi_ssid.c_str());
	strcpy_s(_wifi_passwd, 	WIFI_PASSWD_MAX+1 , local_wifi_passwd.c_str());
	strcpy_s(_name, 		NAME_MAX+1 , 		local_name.c_str());
	strcpy_s(_passwd, 		PASSWD_LEN+1 , 		local_passwd.c_str());
	_startup_color = uint32ToColor(local_color_value);
	return true;
}

const char* LightConfigClass::name() {
	return _name;
}

bool LightConfigClass::name(const char* nm) {
	if (strnlen_s(nm, NAME_MAX+1) == NAME_MAX+1)
		return false;
	strcpy(_name, nm);
	return true;
}

bool LightConfigClass::chk_name(const char* nm) {
	int len = strnlen_s(nm, NAME_MAX+1);
	if (len == NAME_MAX+1 || len == 0)
		return false;
	return true;
}

const char* LightConfigClass::passwd() {
	return _passwd;
}

bool LightConfigClass::passwd(const char* pwd) {
	if (strnlen_s(pwd, PASSWD_LEN+1) != PASSWD_LEN)
		return false;
	strcpy(_passwd, pwd);
	return true;
}

bool LightConfigClass::chk_passwd(const char* pwd) {
	int len = strnlen_s(pwd, PASSWD_LEN+1);
	if (len != PASSWD_LEN || len == 0)
		return false;
	return true;
}

const char* LightConfigClass::wifiPasswd() {
	return _wifi_passwd;
}

bool LightConfigClass::wifiPasswd(const char* pwd) {
	if (strnlen_s(pwd, WIFI_PASSWD_MAX+1) == WIFI_PASSWD_MAX+1)
		return false;
	strcpy(_wifi_passwd, pwd);
	return true;
}

bool LightConfigClass::chk_wifiPasswd(const char* pwd) {
	int len = strnlen_s(pwd, WIFI_PASSWD_MAX+1);
	if (len == WIFI_PASSWD_MAX+1 || len == 0)
		return false;
	return true;
}



const char* LightConfigClass::wifiSSID() {
	return _wifi_ssid;
}

bool LightConfigClass::wifiSSID(const char* ssid) {
	if (strnlen_s(ssid, WIFI_SSID_MAX+1) == WIFI_PASSWD_MAX+1)
		return false;
	strcpy(_wifi_ssid, ssid);
	return true;
}

bool LightConfigClass::chk_wifiSSID(const char* ssid) {
	int len = strnlen_s(ssid, WIFI_SSID_MAX+1);
	if (len == WIFI_PASSWD_MAX+1)
		return false;
	strcpy(_wifi_ssid, ssid);
	return true;
}

int  LightConfigClass::mode() {
	return _mode;
}

bool LightConfigClass::mode(int m) {
	if (m != 0 && m != 1)
		return false;
	_mode = m;
	_wifi_changed = true;
	return true;
}

bool LightConfigClass::chk_mode(int m) {
	if (m != 0 && m != 1)
		return false;
	_mode = m;
	return true;
}

const Color& LightConfigClass::startColor() {
	return _startup_color;
}

bool LightConfigClass::startColor(const Color &color) {
	_startup_color = color;
	return true;
}


LightConfigClass LightConfig;
