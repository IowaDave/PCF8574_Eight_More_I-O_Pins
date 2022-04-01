/*
 * EightMorePins.cpp
 * Copyright 2022 by David G Sparks
 * All Rights Reserved
 *
 * Implement a class for using an 8574 chip
 * as a digital I/O multiplexer,
 * giving access to eight digital I/I pins.
 * Control the pins by reading from the
 * data register of the 8574,
 * or by writing to it. 
 *
 * LICENSE
 *
 * EightMorePins.cpp ("this code") is free software: you can redistribute it 
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

#include "EightMorePins.h"

  EightMorePins::EightMorePins () : 
       b8Register(0b11111111),
       I2C_Address(0x20), 
       interval(10)
       {}

  EightMorePins::EightMorePins (uint8_t x) : 
       b8Register(0b11111111),
       I2C_Address(x),
       interval(10)
       {}

  uint8_t EightMorePins::readB8 (void) {
    Wire.requestFrom(this->I2C_Address, 1);
    return (uint8_t) Wire.read();
  }

  void EightMorePins::writeB8 (uint8_t bitsToSet) {
    Wire.beginTransmission(this->I2C_Address);
    Wire.write(bitsToSet);
    Wire.endTransmission();
  }

  void EightMorePins::refresh (void) {
    uint8_t newRegister;
    static uint32_t lastDing = 0;
  
    this->b8Changed = false;  // default result
    if ((millis() - lastDing) >= this->interval)
    {
      newRegister = this->readB8();
      if (newRegister ^ this->b8Register) // they are different
      {
        this->b8Register = newRegister;
        this->b8Changed = true;
      }
      lastDing = millis();
    }   
  }

  char * EightMorePins::bitsAsString (void) {
    for (int i = 0; i < 8; i++) {
      if (this->b8Register & (1 << i)) {
        this->bitString[i] = '1';
      } else {
        this->bitString[i] = '0';
      }
    }
    return this->bitString;
  }

  int EightMorePins::pin (uint8_t pinNumber) {
    // return error if 0 <= pinNumber <= 7
    if ((pinNumber < 0) | (pinNumber > 7)) return -1;
    // OK, return value of the pin
    return ((this->b8Register & (1 << pinNumber)) >> pinNumber);
  }
