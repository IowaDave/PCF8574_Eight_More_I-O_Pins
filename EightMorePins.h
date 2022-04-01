/*
 * EightMorePins.h
 * Copyright 2022 by David G Sparks
 * All Rights Reserved
 *
 * Define a class for using an 8574 chip
 * as a digital I/O multiplexer,
 * giving access to eight digital I/I pins.
 * Control the pins by reading from the 8574
 * data register of the 8574,
 * or by writing to it. 
 *
 * LICENSE
 *
 * EightMorePins.h ("this code") is free software: you can redistribute it 
 * and/or modify it under the terms of the GNU General Public License as published by the 
 * Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * 
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

// #include <inttypes.h>
#include "Arduino.h"
#include <Wire.h>

class EightMorePins {
  private:
  uint8_t I2C_Address;
  uint8_t b8Register;
  bool b8Changed;
  char bitString[9];
  uint16_t interval;

  void writeB8 (uint8_t);
  uint8_t readB8 (void);
  void refreshB8 (void);
  bool intervalElapsed();

  public:
  EightMorePins ();
  EightMorePins (uint8_t);
      
  uint8_t getRegister (void)
  { return this->readB8 (); }
  
  void setRegister (uint8_t bitsToSet)
  { this->writeB8(bitsToSet); }
  
  void refresh (void);
  
  bool changed (void)
  { return this->b8Changed; }
  
  char * bitsAsString (void);

  int pin (uint8_t pinNumber);

  void setInterval(uint16_t interval)
  { this->interval = interval; }

  uint16_t getInterval()
  { return this->interval; }
};
