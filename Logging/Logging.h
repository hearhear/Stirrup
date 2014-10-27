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