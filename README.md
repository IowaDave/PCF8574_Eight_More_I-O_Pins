# Eight More I/O Pins with I2C

A PCF8574 I2C multiplexer chip can add eight digital I/O pins to a microcontroller project.

### Introduction

The PCF8574 integrated circuit provides eight, digital I/O pins. 

Your project can access the pins through an I2C connection. It can both read and set the binary value, 0 or 1, for each pin. This means it can detect signals received from peripherals attached to the pins, and write control signals out to them.

The integrated circuit can be used directly. Plug it into a breadboard as shown in Figure x, below.

This project includes a C++ library named EightMorePins that you can use for interacting with the PCF8574. The library has been tested in the Arduino IDE for AVR-based Arduinos and for ESP8266s.

An example project illustrates using the library with eight pushbuttons.

### Contents
* [Using the Library](#using-the-library)
* [Library Methods](#library-methods)
* [Connecting to the PCF8574](#connecting-to-the-pcf8574)
* [Using The I/O Pins](#using-the-i/o-pins)
* [A Really Wild Application](#a-really-wild-application)
* [Switching LEDs and Other Circuits](#switching-leds-and-other-circuits)
* [An Example With Eight Pushbuttons](#an-example-with-eight-pushbuttons)
* [You Probably Already Used A PCF8574 But Did Not Know It](#you-probably-already-used-a-pcf8574-but-did-not-know-it)


### Using the Library

The easiest way to get the library is to download this repository as a .zip file and import in into your Arduino IDE through the Sketch menu:  

```
Sketch > Include Library > Add .ZIP library...
```

If you prefer to do things the hard way, you can choose to copy and paste the library's two code files into an Arduino IDE project:

* EightMorePins.h
* EightMorePins.cpp


The library defines a C++ class called EightMorePins. Your code will need to include the library and create a  variable (a.k.a, in GeekSpeak, an object, or more formally, "an instance of the class") to work with it. Like this example:

```
#include "EightMorePins.h"

EightMorePins myButtons;
```

This step also determines the I2C address where your code expects to find the 8574. For more information about how the library handles I2C addresses, see the discussion of the "Constructor", below.

### Library Methods

Note that values and properties associated with the class are accessed through methods in the following list. 

* Default Constructor
    * Called when declaring an object (instance) of the class, for example:
    * ```EightMorePins myButtons;```
    * This example initializes ```myButtons``` to use the default I2C address, 0x20.

* Optional Alternative Constructor
    * Provide a different I2C address when declaring the object, for example:
    * ```EightMorePins mySensors(0x21);```
    * This example initializes ```mySensors``` to use the I2C address of 0x21.

* ```void refresh (void)```
    * reads the value of the data register on the PCF8574, and
    * stores a copy of that value in an internal variable

* ```uint8_t getRegister (void)``` 
    * Returns an 8-bit unsigned byte representing the most recently-stored internal copy of the data register on the PCF8574.
    * Typically, you would call ```refresh()``` first, to update the internal copy.

* ```void setRegister (uint8_t bitsToSet)```
    * Writes an 8-bit byte to the data register of the PCF8574.
    * Set or clear bits in the byte to regulate pins on the PCF8574.

* ```bool changed (void)```
    * Every call to ```refresh()``` compares the newly-obtained value of the data register of the OCF8574 to the previously-obtained value.
    * This method returns:
        * *true* if the values were different, indicating a change has taken place.
        * *false* if the values were the same, indicating no-change.

* ```char * bitsAsString (void)```
    * Returns a pointer to a C-type string representing the bits in the local copy of the data register as '1' or '0'.

* ```int pin (uint8_t pinNumber)``
   * Returns the value of the specified pin.
   * Returns an error code, &endash;1, if pinNumber is negative, or if it is greater than 7.

* ```void setInterval (uint16_t interval)```
    * Sets the number of milliSeconds that ```refresh()``` will delay before taking the next reading of the data register on the PCF8574.
    * The default value is 10 mS, and is established automatically when the object (instance) is declared.
    * Relevant values are from 0 (zero) to 0xffff (65535), inclusive.
    * Setting the value of the interval to 0 effectively transforms the ```refresh()``` method into a kind of ```readNow()``` command.
    
* ```uint16_t getInterval (void)```
    * Retrieves the current setting of the ```refresh()``` delay interval.

### Connecting to the PCF8574
The integrated circuit is available in several different shapes and sizes. This project targets the PDIP16 package. "PDIP" stands for Plastic Dual In-line Pins, I think. 

The diagram of the package in Figure x, below, labels the pins with their functions. A notch at one end of the package establishes their locations. 



The following list describes a minimal set of connections that I used for the example project. Caution: my list might contain an error or be incomplete. The official datasheets are the only reliable source of information about how to make connections in your specific situation.

1. "A0" - ground
2. "A1" - ground
3. "A2" - ground
4. "P0" - peripheral
5. "P1" - peripheral
6. "P2" - peripheral
7. "P3" - peripheral
8. "VSS" - ground
9. "P4" - peripheral
10. "P5" - peripheral
11. "P6" - peripheral
12. "P7" - peripheral
13. "INT" - no connection
14. "SCL" - microcontroller's SCL bus; needs a pullup resistor
15. "SDA" - microcontroller's SDA bus, needs a pullup resistor
16. "VCC" - positive voltage, same level as the microcontroller

**Discussion**

*Pins 1-3* select the I2C address of the PCF8574 chip. Grounding them selects the default address: 0x20. You can change the address by pulling up these pins to the VCC voltage level through a resistor. A0 adds 1 to the address, A1 adds 2, and A2 adds 4.

* By the way, there is a near-identical twin device called the PCF8574A. The only difference is that the default I2C address of the PCF8574A is 0x38. It works the same, otherwise.

*Pin 13 "INT"* provides an active-low interrupt signal. I'm leaving that pin out of scope for this article because I don't (yet) understand it well enough. If you choose to read this pin, it has to be pulled up to VCC externally.

*Pins 14-15*, the I2C lines, need to be pulled up by resistors. Note that the Arduino Wire library activates the internal resistors of an Arduino. I found that to be sufficient for my little example. Your situation may need a different value for the I2C pullup resistors. This article does not discuss how to determine I2C resistor values.

*Pin 16 VCC* is interesting. Should there be capacitors? The Texas Instruments datasheet recommends attaching two capacitors: a "small" one to tame noisy power plus a "larger" one to sustain the chip through potentially greater voltage fluctuations. They don't say how small or large. 

I supplied regulated power to the chip and did not experience any issues, even with no capacitor attached. But then, I decided it was finally time for me to buy some capacitors. You'll see two, that I somewhat arbitrarily chose, in the photo of the example, Figure x.

### Using The I/O Pins
Datasheets for the PCF8574 describe pins 4-7 (P0 - P3) and 9-12 (P4 - P7)  as "quasi-bidirectional ports." They speak of a port as being used in two "modes": input and output. But there is no way to set a mode of operation inside the PCF8574. What does "mode" mean? At the time of writing, I remained a little bit unsure.

My best guess is that the mode of a port pin is determined by what kind of device attaches to it.  In other words, whether the port pin will receive a signal, or emit one.

* *Input*: A sensor or a switch would most likely be an input device. Likewise would be a signal originating in another microprocessor. Generally, your code would *read from* the port connected to an input device. The example in this project uses pushbuttons, a kind of switch.

* *Output*: An indicator or actuator, such as an LED, a relay, or an external PWM controller, would most likely be an output device. Generally, your code would *write to* the port connected to an output device. 

* Some devices, such as a Liquid Crystal Display, can be both outputs and inputs. This article will address LEDs but not any other kind of output device, and says nothing more about LCDs.

Access to the I/O ports of the PCF8574 takes place through its one-and-only, 8-bit data register. 8 bits, 8 ports, get it? Bit #0 corresponds to Pin #0, Bit #7 to Pin #7, and all like that in between. When you write to or read from the device, you set or read all 8 pins at once.

* A value of 1 on a bit means:
    * for input, the attached peripheral has pulled the port pin up to near VCC voltage potential.
    * for output, about 100 micro-Amps of current at near VCC voltage is being sourced to the port pin through an internal pullup resistor.

* A value of 0 on a bit means:
    * for input, the attached peripheral has pulled down the port pin to near ground potential.
    * for output, the port pin will sink current.

---

Caution! Two things about these ports need emphasis here.

1. Set the bit to 1 initially for any port attached to an input device. This is important to do because it protects the port pin against damage caused by too much current flowing in from the input device. Afterward, as long as you do not write a 0 to the bit, it is not necessary to write a 1 to the port every time before you read it.
2. A port with its bit set to 0 can sink about 100 times as much current as it can source. This property of the device affects the way an I/O port might be used to switch a circuit on and off. The LED discussion that follows will explain this aspect in more detail.

---

To read the ports, read the data register through the I2C connection into an 8-bit byte of the microcontroller's memory. Then examine the bits to determine the status of the ports. For example:


```
EightMorePins myPins; // declare class instance object using default I2C address 0x20
myPins.refresh(); // read the data register and store it internally
byte pinBits = myPins.getRegister(); // copy the data register into a local variable
// access the bits in the pinBits variable
```

Writing to the ports means first preparing an 8-bit byte in the microcontroller's memory, setting or clearing bits according to the devices attached to the corresponding ports. Then use I2C to write the byte to the register on the PCF8574. For example:

```
EightMorePins myPins(0x21); // declare class instance using I2C address 0x21
byte pinBits = 0b11110111; // output port P3 to 0, the others to 1
myPins.setRegister(pinBits); // write the bits to the PCF8574
```

### A Really Wild Application

Take a look at this diagram, excerpted from the Texas Instruments datasheet. 



It shows:

* SDA, SCL and INT connecting to a microcontroller, with 10K-Ohm pullup resistors attached.
* A0, A1 and A2 hard-wired to ground, holding the I2C address to its built-in default.
* Six of the I/O pins being used, each for a different purpose. The arrows offer a clue to the direction in which data flows at each pin:
    * P0 is in an **output** connected to an LED. The arrangement might look odd. It's discussed in detail later in this article.
    * P1 is an **input**, set to receive a signal from the interrupt output of Subsystem 1.
    * P2 is an **output**, able to send a "reset" signal to Subsystem 2.
    * P3 is an **output**, able to send an "enable" signal to a controlled device.
    * P4 is an **input**, able to receive an alarm signal from Subsystem 3
    * P5 is an **input**, connected to a pushbutton switch. The pin would detect the presence of voltage (importantly -- through a current limiting resistor) when the pushbutton is depressed. 

A note about P5, as illustrated above. The example program in this article wires up its pushbuttons in a different, potentially safer configuration.

### Switching LEDs and Other Circuits

The I/O ports on a PCF8574 work differently compared to those on an Arduino or other microcontroller. They put out very little current when set to HIGH (that is, writing their corresponding bit to 1). The amount is less than 1 mA. Maybe a lot less. It is a *signal*, not a *source*. It is not enough to light up an LED, nor to actuate any other circuit that needs much current.

By contrast, the I/O ports on a PCF8574 can *sink* (that is, convey to ground) a significant amount of current. How much? See the datasheets for details. Really. You want to get familiar with the maximum current limits they specify. 

I think it might be safe to push 10 mA through a port pin that way. This is plenty to illuminate an LED, though perhaps not to its brightest.

The way it works is to turn the LED around, reversing it compared to what you might be familiar with from connecting LEDs to an Arduino.

For a PCF8574, connect the LED's *cathode* (the shorter lead, near the flat spot on the rim) to the port pin. Connect the *anode* to a resistor, and then connect the resistor to positive power.

The resistor value should be sufficient to limit current to 10 mA. Consider the example of a 5-volt power supply and a red LED that "drops" 2 volts. The resistor has to abate the other 3 volts. By Ohm's Law, if I'm calculating this correctly, resistance R = V/I where V = 5 volts and I = 0.01 amps. 3 / 0.01 = 300 Ohms of resistance. I'd choose at least a 330-ohm resistor to stay on the safe side.

Your code would turn the LED "on" by writing a 0 to the relevant port bit. The 0 allows the port to sink current, completing the circuit. Conversely, writing a 1 to the port bit disrupts the current-sink and turns off the LED.

There is an interesting further note about LEDs in the Texas Instruments datasheet. Unlike Arduinos, a port pin on a PCF8574 does not have a high-impedance "input" mode. The port pin may continue to accept current through the LED even when its port bit is set to 1. It will do this while the voltage *at the pin* remains lower compared to the voltage driving the LED. Apparently (I do not fully understand this) the fact that the LED "drops" voltage can result in such a difference. I suspect it's a concern mainly for situations where power supply is limited, such as when running on batteries.

TI suggests connecting a *second*, high-value resistor between VCC and the port pin, in parallel with the LED and its current-limiting resistor. Why? I imagine it's to pull up the voltage at the port to near VCC, neutralizing the electrical potential across the LED and minimizing current flow.

### An Example With Eight Pushbuttons

The example program, Eight_buttons, demonstrates using the Library with a prototype board having eight pushbuttons. See Figure x, below.



Each button connects to one of the Ports, P0 through P7, and to ground. The data register bits in the PCF8574 are initialized explicitly to 11111111. This activates the internal pullups on the ports. 
* Pressing a button connects a port to ground, taking its corresponding bit in the data register to 0. 
* Releasing the button breaks the connection to ground. The internal pullup restores the corresponding pin to a value of 1.

The program polls the PCF8574 repeatedly. A state machine inside the ```refresh()``` method limits how frequently the code actually reads the device. By default it skips an interval of 10 milliseconds between readings, or about 100 times per second. The interval can be increased or decreased with the ```setInterval()``` method.

The program makes use of the ```changed()``` and the ```bitsAsString()``` methods to print the PCF8574's register bits each time they change. 

It uses the ```pin()``` method to obtain the value of pin 4 for printing.

Why does the program use polling, rather than an interrupt?  Several reasons. 
* I2C, like Serial, also uses interrupts. As a general rule, Serial and I2C operations should not take place inside an interrupt service routine. Instead, they belong in the main loop() part of the program. Which means we would be polling *something*, a boolean value perhaps, anyway.
* I found that the program would freeze up pretty quickly when I tried it with interrupts. Most likely, I just don't understand well enough (yet) how to use the PCF8574 with interrupts.
* Meanwhile, polling works reliably. And it turns out that the use of a state machine in the ```refresh()``` method affords an effect of multi-threading. I saw no perceptible difference in responsiveness between polling and interrupts -- until the interrupts stop working, of course.

### You Probably Already Used A PCF8574 But Did Not Know It

The popular I2C version of a Liquid Crystal Display (LCD) incorporates a so-called "backpack" board soldered to the 12 digital I/O pins of the actual LCD device. The LCD actually is controlled through (at least 8 of) those 12 pins.

It is possible to wire an LCD directly to 8 pins on a Arduino and control it that way. Most people prefer to work with the I2C version.

What do you know? The I2C interface on most of the backpacks is a PCF8574, or its near-identical twin the PCF8574A.

See? If you've ever used an I2C-driven LCD, you've probably used a PCF8574. Now you know these tiny integrated circuits can do so much more for you than that!









