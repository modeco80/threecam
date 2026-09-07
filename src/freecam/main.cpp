#include "utils/log.hpp"

extern "C" int modMain() {
	utilLog(LogInfo, "Hello World?");
	return 0;
}
