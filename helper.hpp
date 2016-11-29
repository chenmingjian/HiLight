#ifndef __HELPER_HPP__
#define __HELPER_HPP__

class Helper {
public:
	static       char* convertPasswd(char *buffer /* need 33 bytes */, const char* raw_passwd);
	static const char* getDeviceId();
	static const char* getWiFiSSID();
};

#endif

