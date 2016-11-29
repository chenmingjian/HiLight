#ifndef __LIGHTCONFIG_HPP__
#define __LIGHTCONFIG_HPP__

#include "DataStruct.hpp"
#include "Constant.hpp"

class LightConfigClass {
public:
	static constexpr int NAME_MAX 				= 32;
	static constexpr int PASSWD_LEN 			= 32;
	static constexpr int WIFI_SSID_MAX 			= 32;
	static constexpr int WIFI_PASSWD_MAX		= 32;
	static constexpr int CONFIG_JSON_BUF_MAX	= 160;
	static constexpr int CONFIG_JSON_TEXT_MAX	= 255;
	bool		 save();
	bool		 load();
	const char*  name();
	bool		 name(const char* nm);
	bool		 chk_name(const char* nm);
	const char*  passwd();
	bool		 passwd(const char* pwd);
	bool		 chk_passwd(const char* pwd);
	const char*  wifiSSID();
	bool		 wifiSSID(const char* nm);
	bool		 chk_wifiSSID(const char* nm);
	const char*  wifiPasswd();
	bool		 wifiPasswd(const char* pwd);
	bool		 chk_wifiPasswd(const char* pwd);
	const Color& startColor();
	bool		 startColor(const Color &color);
	bool		 chk_startColor(const Color &color) { return true; }
	int 		 mode();
	bool		 mode(int m);
	bool		 chk_mode(int m);
	bool		 wifiChanged() { bool ret = _wifi_changed; _wifi_changed = false; return ret; }
	void		 print(const char* header = "LightConfig")  {
		Serial.println(header);
		Serial.printf("  name       : %s\n", name());
		Serial.printf("  passwd     : %s\n", passwd());
		Serial.printf("  mode       : %s\n", _mode ? "Sta" : "AP");
		Serial.printf("  wifiSSID   : %s\n", wifiSSID());
		Serial.printf("  wifiPasswd : %s\n", wifiPasswd());
	}

private:
	char _name[NAME_MAX+1];
	char _passwd[PASSWD_LEN+1];
	char _wifi_ssid[WIFI_SSID_MAX+1];
	char _wifi_passwd[WIFI_PASSWD_MAX+1];
	Color _startup_color;
	// 0: config mode, in this mode, esp8266 act as WIFI AP
	// 1: work mode,   in this mode, esp8266 act as WIFI Station
	int  _mode;

private:
	bool _wifi_changed;
	bool _spiffsBegan;
	bool _spiffsBegin();
	bool defaultConfig();
};

extern LightConfigClass LightConfig;

#endif

