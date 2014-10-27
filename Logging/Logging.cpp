/*	This file is part of Stirrup.

	Stirrup is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Stirrup is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Stirrup.  If not, see <http://www.gnu.org/licenses/>.
*/

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