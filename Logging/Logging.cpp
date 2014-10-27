#include "Logging.h"

void Logging::info(String message) {
	Serial.println(String(INFO_SYMBOL) + String(" ") + String(message));
}

void Logging::info_nonl(String message) {
	Serial.print(String(INFO_SYMBOL) + String(" ") + String(message));
}

void Logging::warning(String message) {
	Serial.println(String(WARNING_SYMBOL) + String(" ") + String(message));
}

void Logging::error(String message) {
	Serial.print(String(ERROR_SYMBOL) + String(" ") + String(message));
	while (true) {
	} // Loop forever. This ensures that the code stops running after a problem
}