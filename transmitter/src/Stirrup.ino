/*
	vf0.1 TRANSMITTER for gamma-FM
	copyright 2014 Liam Marshall (@archimedespi on Github)

	Thanks to Adafruit for the Adafruit-Si4713-Library.
	Thanks to Steven Cogswell (@scogswell) for ArduinoSerialCommand, and @kroimon for
	their modifications to that library.

	This file is part of Stirrup.

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

#include <Wire.h>
#include <Adafruit_Si4713.h>

#include <SerialCommand.h>
#include <Logging.h>

#include "constants.h"

#define FAKE_RADIO // For testing. Turns off all radio calls.

#define RESETPIN 12

#define FMSTATION 10230 // 10230 = 102.30 MHz. The value's in KHz

#define TXPOWER 115 // Between 88 and 115 dBuV

#define RDS_STATION "gammaFM"
#define RDS_BUFFER "gammaFM/HearHear transmitter vf0.1"

#ifndef FAKE_RADIO
Adafruit_Si4713 radio = Adafruit_Si4713(RESETPIN);
#endif

SerialCommand sCmd;

void setup()
{
	// Start up serial
	Serial.begin(9600);
	while (!Serial) {
    	; // wait for serial port to connect. Needed for ATmega32u4 based boards (i.e. γ-FM)
  	}

	// Print out some license stuff
	Serial.println(F("γ-FM :: Transmitter"));
	Serial.println(F("γ-FM/HearHear copyright (C) 2014 Liam Marshall"));
	Serial.println(F("This program comes with ABSOLUTELY NO WARRANTY."));
	Serial.println(F("This is free software, and you are welcome to redistribute it"));
	Serial.println(F("under certain conditions."));
	Serial.println();

	#ifndef FAKE_RADIO
	if (! radio.begin()) {
		error("No radio?");
	}
	#endif

	Logging::info("Setting TX power to " + String(TXPOWER));
	#ifndef FAKE_RADIO
	radio.setTXpower(115);
	#endif

	Logging::info_nonl("Tuning to ");
	Serial.print(FMSTATION / 100);
	Serial.print('.');
	Serial.println(FMSTATION % 100);
	#ifndef FAKE_RADIO
	radio.tuneFM(FMSTATION);
	#endif

	// Grab the tune status
	#ifndef FAKE_RADIO
	radio.readTuneStatus();	
	Logging::info("Current frequency: " + radio.currFreq);
	Logging::info("Current dBuV: " + radio.currdBuV);
	Logging::info("Current ANTcap: " + radio.currAntCap);
	#endif

	// Start RDS/RDBS transmission
	#ifndef FAKE_RADIO
	radio.beginRDS();
	radio.setRDSstation(RDS_STATION);
	radio.setRDSbuffer(RDS_BUFFER);
	#endif

	Logging::info("RDS on!");
	Logging::info("<RDS> Station = " RDS_STATION);
	Logging::info("<RDS> Buffer = " RDS_BUFFER);

	setupCommands();
}

void setupCommands() {
	sCmd.addCommand("tune", tuneCommand);
	sCmd.addCommand("save", saveCommand);
	sCmd.addCommand("rds_station", rdsStationCommand);
	sCmd.addCommand("rds_buffer", rdsBufferCommand);
	sCmd.setDefaultHandler(unrecognizedCommand);
}

void tuneCommand() {
	double tuneMHz;
	char *arg;

	arg = sCmd.next();
	if (arg == NULL) {
		return;
	}
	tuneMHz = strtod(arg, NULL);
	int tuneKHz = tuneMHz * 100;
	Logging::info_nonl("Tuning radio to ");
	Serial.print(tuneMHz);
	Serial.println("MHz");
	#ifndef FAKE_RADIO
	radio.tuneFM(tuneKHz);
	#endif
}

void saveCommand() {
	Logging::info("Saving settings to EEPROM");
}

void rdsStationCommand() {
}

void rdsBufferCommand() {
}

void unrecognizedCommand(const char *command) {
	Logging::warning("Unrecognized command: " + String(command) + ".");
}

void loop()
{
	sCmd.readSerial();
}
