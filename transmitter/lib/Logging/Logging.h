#ifndef LOGGING_H
#define LOGGING_H

#include <Arduino.h>

#define INFO_SYMBOL "[info]"
#define WARNING_SYMBOL "[/warning/]"
#define ERROR_SYMBOL "[!ERROR!]"

class Logging {
public:
	static void info(String message);
	static void info_nonl(String message);
	static void warning(String message);
	static void error(String message);
};

#endif