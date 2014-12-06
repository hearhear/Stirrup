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

/*  Structured Arduino EEPROM access macros
	from https://projectgus.com/2010/07/eeprom-access-with-arduino/
*/
#include <avr/eeprom.h>
#define eeprom_read_to(dst_p, eeprom_field, dst_size) eeprom_read_block(dst_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(dst_size, sizeof((__eeprom_data*)0)->eeprom_field))
#define eeprom_read(dst, eeprom_field) eeprom_read_to(&dst, eeprom_field, sizeof(dst))
#define eeprom_write_from(src_p, eeprom_field, src_size) eeprom_write_block(src_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(src_size, sizeof((__eeprom_data*)0)->eeprom_field))
#define eeprom_write(src, eeprom_field) { typeof(src) x = src; eeprom_write_from(&x, eeprom_field, sizeof(x)); }
#define MIN(x,y) ( x > y ? y : x )

#include "constants.h"

//#define FAKE_RADIO // For testing. Turns off all radio calls.

#define RESETPIN 12

long tuneKHz = 10230; // 10230 = 102.30 MHz. The value's in KHz

int TXdBuV = 115; // Between 88 and 115 dBuV

#define RDS_STATION "gammaFM"
#define RDS_BUFFER "gammaFM/HearHear transmitter vf0.1"

#ifndef FAKE_RADIO
Adafruit_Si4713 radio = Adafruit_Si4713(RESETPIN);
#endif

SerialCommand sCmd;

// Data stored in the EEPROM
struct __eeprom_data {
	long frequencyKHz;
	int transmissionPower;
	int v;
};

void setup()
{
	// Start up serial
	Serial.begin(19200);
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
		Logging::error("No radio?");
	}
	#endif


	Logging::info("Loading settings from EEPROM");
	loadCommand();
	Logging::info("Current settings are: ");
	settingsCommand();

	Logging::info("Setting TX power to " + String(TXdBuV) + "dBuV");
	#ifndef FAKE_RADIO
	radio.setTXpower(115);
	#endif

	Logging::info_nonl("Tuning to ");
	Serial.print(tuneKHz);
	Serial.println(F(" KHz"));
	#ifndef FAKE_RADIO
	radio.tuneFM(tuneKHz);
	#endif

	// Grab the tune status
	#ifndef FAKE_RADIO
	radio.readTuneStatus();	
	Logging::info("Current frequency: " + String(radio.currFreq));
	Logging::info("Current dBuV: " + String(radio.currdBuV));
	Logging::info("Current ANTcap: " + String(radio.currAntCap));
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
	sCmd.addCommand("power", powerCommand);
	sCmd.addCommand("save", saveCommand);
	sCmd.addCommand("load", loadCommand);
	sCmd.addCommand("settings", settingsCommand);
	sCmd.setDefaultHandler(unrecognizedCommand);
}

void tuneCommand() {
	char *arg;

	arg = sCmd.next();
	if (arg == NULL) {
		Logging::warning("`tune` must have arguments!");
		return;
	}
	tuneKHz = strtol(arg, NULL, 0);
	Logging::info_nonl("Tuning radio to ");
	Serial.print(tuneKHz);
	Serial.println("KHz");
	#ifndef FAKE_RADIO
	radio.tuneFM(tuneKHz);
	#endif
}

void powerCommand() {
	char *arg;

	arg = sCmd.next();
	if (arg == NULL) {
		Logging::warning("`power` must have arguments!");
		return;
	}
	TXdBuV = strtol(arg, NULL, 0);
	Logging::info_nonl("Setting TX power to ");
	Serial.print(TXdBuV);
	Serial.println("dBuV");
	#ifndef FAKE_RADIO
	radio.setTXpower(TXdBuV);
	#endif
}

void saveCommand() {
	Logging::info("Saving settings to EEPROM");
	eeprom_write(tuneKHz, frequencyKHz);
	eeprom_write(TXdBuV, transmissionPower);
}

void loadCommand() {
	Logging::info("Loading settings from EEPROM");
	eeprom_read(tuneKHz, frequencyKHz);
	eeprom_read(TXdBuV, transmissionPower);
	// Logging::info("tuneKHz = " + String(tuneKHz));
	// Logging::info("TXdBuV =" + String(TXdBuV));
	Logging::info("Updating settings from loaded data...");
	#ifndef FAKE_RADIO
	#endif
}

void settingsCommand() {
	Logging::info("Frequency (KHz): " + String(tuneKHz));
	Logging::info("Transmission power (dBuV): " + String(TXdBuV));
}

void unrecognizedCommand(const char *command) {
	Logging::warning("Unrecognized command: " + String(command) + ".");
}

void loop()
{
	sCmd.readSerial();
}
