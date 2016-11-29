#ifndef __WIFITASK_HPP__
#define __WIFITASK_HPP__

class WiFiTaskClass {
public:
	static constexpr int MODE_START = -1;
	static constexpr int MODE_AP    = 0;
	WiFiTaskClass();
	void begin();
	void loop();
private:
	int _mode;
};

extern WiFiTaskClass WiFiTask;

#endif