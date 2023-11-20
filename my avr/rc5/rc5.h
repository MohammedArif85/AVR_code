/*
 * RC5 protocol implementation.
 *
 * Contributed by Georg-Johann Lay <georgjohann@web.de>
 *
 * @Author  : Georg-Johann Lay
 * @Date    : 2007-02-02
 * @File    : rc5.h
 * @Language: ISO C 99
 * @Purpose : Implementation of RC5 receiver software
 */

/*
  This implementation assumes a ONE to be encoded as HIGH-LOW transition
  and a ZERO encoded as LOW-HIGH transition, i.e. just the other way
  round as specified by the RC5 contract.
  This inversion comes from the IR receiver's open collector output driver
  that inverts the signal.

  The RC5 signal consists of 14 bits
  ==================================

  AGC (Automatic Gain Control)
      These two bits allow the RC5 receiver hardware's input amplifier to adapt
      to the signal level.

  TOGGLE bit (aka FLIP bit)
      This bit toggles each time a new key is pressed. That allows to
      distinguish between holding down a key and pressing a key twice or more.

  ADDRESS
      Five address bits to select a device (0=TV, 20=CD, ...).

  COMMAND
      Six bits of very information. To support more than 64 commands, newer
      RC5 like protocols use a seventh bit whose complement is transmitted
      as second AGC bit.

  Thus, an RC5 frame looks like this:

  G ~C6  F  A4 A3 A2 A1 A0  C5 C4 C3 C2 C1 C0

  With C6 always 0 (i.e. second AGC always 1) if just 64 commands are supported.

  An RC5 bit
  =========

  consists of two parts: The first half bit and the second half bit which are
  separated by the respective signal level transition. Between two bits
  there may or may not be a transition.
  A Bit lasts 1728 µs.
*/

#ifndef _RC5_H_
#define _RC5_H_

#include <inttypes.h>

/*
  A typical snippet of C code will look like this:
  (Sample code for AVR and avr-gcc.
   Compile `rc5-avr.c' and link against the generated object.

   Compile:
      avr-gcc -Os -c rc5-avr.c -mmcu=... -DF_CPU=...)


  #include <avr/io.h>
  #include <avr/interrupt.h>

  #include "rc5.h"

  int main (void)
  {
      // Initialize Hardware
      // Allow any RC5 device address
      rc5_init (RC5_ALL);

      // enable all interrupts
      sei();

      // main loop
      while (1)
      {
          // New RC5 frame arrived?
          if (rc5.flip >= 0)
          {
              // Yes:
              // evaluate RC5 data in `rc5' or make a local copy
              // of it for later use
              ...

              // Tell receiver software to get more RC5 frames
              rc5.flip = -1;
          } // done RC5
      } // main loop

      // We never come here, thus no return needed.
      // It would be dead, anyway...
  }
*/


/*
  The structure that represents an RC5 frame.
  If rc5.flip = -1, then the receiver will listen for a new RC5 frame
  an pass it in `rc5' upon reception, provided the address matches.
  After reception of a frame, `rc5.flip' will be 0 or 1.
  After you got the RC5 data, just set rc5.flip to -1 again to indicate
  you read the data and the next RC5 frame may be received.
*/
typedef struct
{
    // The RC5 code (command)
    uint8_t code;

    // The device address: 0=TV ...
    uint8_t addr;

    // The flip bit
    volatile int8_t flip;

    // This is not used by the RC5 software. It may be used by the application.
    // The main purpose of this field is to make `rc5_t' to have a size
    // of 4 bytes (instead of 3). This is much better if the user makes a local
    // copy of the rc5 object because the copy then can be hold in registers
    // and saves the overhead of a frame.
    char info;
} rc5_t;

// Publish `rc5'.
extern rc5_t rc5;

/*
  Initialize the RC5 receiver software.
  Pass the device address to listen to in `addr'.
  If the MSB of `addr' is set, then all device addresses are accepted.
  You may use `RC5_ALL' as address in that case.
*/
extern void rc5_init (uint8_t addr);

#define RC5_ALL 0xff

// There is an extension of the RC5 protocol that features a seventh
// command bit and thereby extends the number of commands from 64 to 128.
// This bit's complement is shipped as the second AGC bit. To activate the
// recognition of such RC5 frames (i.e. frames with command = 64...127)
// just define the following macro or add `-DRC5_RC6' to the compiler's
// command line arguments.

// #define RC5_RC6

#endif /* _RC5_H_ */
