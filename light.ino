#include <vector>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "StaticJsonBufferModify.hpp"
#include "Constant.hpp"
#include "LightMode.hpp"
#include "helper.hpp"
#include "LightConfig.hpp"
#include "UDPDiscover.hpp"
#include "WiFiTask.hpp"


const char *ssid = "HZ_2.4G";
const char *password = "lab6305*";
ESP8266WebServer server(23333);
LightModeHandler *lightHandlerPtr = LightModeHandlers[0];

using namespace std;

void basePostJsonHandler( String (*next)(JsonObject& arguments, JsonObject& result) ) {
	static StaticJsonBufferModify<RequestJsonMax+ResponseJsonMax> jbuffer;
	static char jstr_response[ResponseJsonMax];
	jbuffer.reset();
	JsonObject& result = jbuffer.createObject();
	const String& _jstr = server.arg("plain");
	const String& jstr = (_jstr == "") ? String("{}") : _jstr;
	Serial.print("recv: ");
	Serial.println(jstr);
	if (jstr.length() > RequestJsonMax) {
		goto json_error;
	}

{ // make g++ happy
	
	JsonObject& params = jbuffer.parseObject(jstr);
	// fre jstr memory
	if (!params.success())
		goto json_error;
	
	String next_error = next(params, result);
	if (next_error != "")
	{
		result["result"] = false;
		result["msg"] = next_error;
		goto emit;
	}
	else
	{
		result["result"] = true;
		goto emit;
	}
}

json_error:
	result["result"] = false;
	result["msg"] = "Invalid or too big json";
	goto emit;
emit:
	result.printTo( jstr_response, ResponseJsonMax );
	server.send(200, ContentTypeJson, jstr_response);

}

String modeSetHandler(JsonObject& arguments, JsonObject& result) {

	const String& mode = arguments["mode"];
	JsonArray& j_params = arguments["params"];
	for ( LightModeHandler* p : LightModeHandlers) 
	{
		if (mode == p->name()) {
			//vector<uint32_t> &params = lightHandlerPtr->getParms();
			//params.clear();
			vector<uint32_t> params;
			for (unsigned int i = 0; i < j_params.size(); i++) 
			{
				uint32_t value = j_params[i];
				params.emplace_back(value);
			}
			if (p->begin(std::move(params)))
			{
				lightHandlerPtr = p;
				return String();
			}
			else
			{
				return mode + " begin failed";
			}
		}
	}
	return String("Cannot found LightModeHandler : ") + mode;
}

String heartHandler(JsonObject& arguments, JsonObject& result) {
	result["heart"] = "beat";
	return String();
}

String settingHandler(JsonObject& arguments, JsonObject& result) {
	bool modify = false;
	const char* name = NULL;
	const char* passwd = NULL;
	const char* wifi_ssid = NULL;
	const char* wifi_passwd = NULL;
	uint32_t start_color;		bool modify_color = false;
	int      mode;				bool modify_mode = false;
	if (arguments.containsKey("name"))
	{
		name = arguments["name"];
		if (!LightConfig.chk_name(name))
			return String("name error");
	}
	if (arguments.containsKey("passwd"))
	{
		passwd = arguments["passwd"];
		if (!LightConfig.chk_passwd(passwd))
			return String("passwd error");
	}
	if (arguments.containsKey("wifi_ssid"))
	{
		wifi_ssid = arguments["wifi_ssid"];
		if (!LightConfig.chk_wifiSSID(wifi_ssid))
			return String("wifi ssid error");
	}
	if (arguments.containsKey("wifi_passwd"))
	{
		wifi_passwd = arguments["wifi_passwd"];
		if (!LightConfig.chk_wifiPasswd(wifi_passwd))
			return String("wifi passwd error");
	}
	if (arguments.containsKey("mode"))
	{
		modify_mode = true;
		mode = arguments["mode"];
		if (!LightConfig.chk_mode(mode))
			return String("mode error");
	}
	if (arguments.containsKey("start_color"))
	{
		modify_color = true;
		start_color = arguments["start_color"];
		if (!LightConfig.chk_startColor(uint32ToColor(start_color)))
			return String("start color error");
	}

	if (modify_mode)
	{
		if (mode)		// AP mode
		{
			if (wifi_ssid == NULL || wifi_passwd == NULL)
			{
				return String("miss ssid or passwd");
			}
		}
	}

	if (name || passwd || modify_mode || modify_color)
	{
		if (name)
			LightConfig.name(name);
		if (passwd)
			LightConfig.passwd(passwd);
		if (modify_color)
			LightConfig.startColor(uint32ToColor(start_color));
		if (modify_mode)
		{
			LightConfig.mode(mode);
			if (mode)
			{
				LightConfig.wifiSSID(wifi_ssid);
				LightConfig.wifiPasswd(wifi_passwd);
			}
		}
		if (!LightConfig.save())
			return "save error";
	}
	return String();
}

void w_modeSetHandler() {
	basePostJsonHandler(modeSetHandler);
}

void w_heartHandler() {
	basePostJsonHandler(heartHandler);
}

void w_settingHandler() {
	basePostJsonHandler(settingHandler);
}



void setup_1()
{
	Serial.begin(115200);
	delay(1);
	Serial.println();
	uint32_t parms[] = {0, 0x00ffffff};
	lightHandlerPtr->begin(2, parms);

	server.on("/mode", HTTP_POST, w_modeSetHandler);
	server.on("/heart", w_heartHandler);
	server.begin();
	WiFi.mode ( WIFI_STA );
	WiFi.begin ( ssid, password );
	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay(1);
		loop();
		Serial.print(".");
	}
	Serial.print("IPAddress: ");
	Serial.println ( WiFi.localIP() );

	LightConfig.load();
	LightConfig.print();
	UDPDiscover.begin();
	
}

void setup_2()
{
	char pwd[33];
	Serial.begin(115200);
	delay(1);
	Serial.println();
	uint32_t parms[] = {0, 0x00ffffff};
	lightHandlerPtr->begin(2, parms);

	server.on("/mode", HTTP_POST, w_modeSetHandler);
	server.on("/heart", w_heartHandler);
	server.begin();
	WiFi.mode ( WIFI_AP );
	const char* ssid = Helper::getWiFiSSID();
	Helper::convertPasswd(pwd, ssid);
	WiFi.softAP(ssid, pwd);
	Serial.print("ssid  : ");
	Serial.println(ssid);
	Serial.print("passwd: ");
	Serial.println(pwd);

	LightConfig.load();
	LightConfig.print();
	UDPDiscover.begin();
}

void setup()
{
	Serial.begin(115200);
	delay(1);
	Serial.println();
	uint32_t parms[] = {0, 0x00ffffff};
	lightHandlerPtr->begin(2, parms);
	server.on("/mode", HTTP_POST, w_modeSetHandler);
	server.on("/heart", w_heartHandler);
	server.on("/setting", HTTP_POST, w_settingHandler);
	server.begin();
	bool ret = LightConfig.load();
	Serial.print("load: ");
	Serial.println(ret ? "true" : "false");
	UDPDiscover.begin();
}



void loop()
{
//	Serial.printf("%10d", millis());
//	Serial.println(" loop");
	server.handleClient();
	lightHandlerPtr->loop();
	UDPDiscover.loop();
	WiFiTask.loop();
}

