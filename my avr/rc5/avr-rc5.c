
/*
 * RC5 implementation for some AVR derivatives.
 *
 * Contributed by Georg-Johann Lay <georgjohann@web.de>
 *
 * @Author  : Georg-Johann Lay
 * @Date    : 2007-02-02
 * @File    : rc5-avr.c
 * @Language: ISO C 99
 * @Purpose : Implementation of RC5 receiver, target dependent part
 *            for some AVR derivatives.
 */

/*
  For the best results, use optimization '-Os' (optimize for size).

  On one hand, we want the implementation to be as efficient as
  possible because we run on a hardware with quite limited resources (AVR).
  On the other hand, we want the code to be as portable as can be.
  Therefore, we follow an approach that looks a bit odd at first sight:
  We include a C source.
  (You may rename it to `foo.h' and feel more familiar.)

  This has two reasons: First, we keep the target independent part of
  the implementation separated from the target dependent part.
  Second, including the C code (in contrast to linking against an
  object or a library) supplies the compiler with a great amount
  of additional information. In particular, it is supplied with the
  very source code. That information, in turn, allows optimization
  strategies like inlining, better register allocation, etc.

  For this reason, the target dependent part and the target independent
  part are intertwined and the abstraction from the hardware is just on
  the source level, not on the object level. (This was our goal with
  respect to optimization!). However, this code may serve as a starting
  point if you want to implement a hardware abstraction by means of
  objects, i.e. supply it in a library.

  On AVR, the code eats up at about 320 bytes of your valuable flash and
  needs at about 20 bytes of SRAM total (static and dynamic).
  The incraese of interrupt respond times can be analysed statically,
  but I did not investigate in that.

  avr-gcc does not support all AVR derivatives on C level. Smaller AVRs
  are just supported on assembler level. To get a start for an assembler
  implementation, compile the module for an AVR that is "close" to
  the AVR you like and then look at avr-gcc's assembler output.

  - Peek at avr-gcc's assembler output
      > avr-gcc -S rc5-avr.c -fverbose-asm -Os -mmcu=... -DF_CPU=...
    or
      > avr-gcc -c rc5-avr.c -fverbose-asm -Os -mmcu=... -DF_CPU=... -save-temps

  - Compile with debug info and disassemble the object (elf32-avr):
      > avr-gcc -g -c rc5-avr.c -Os -mmcu=... -DF_CPU=...
      > avr-objdump -h -S -j .text rc5-avr.o
    Disassembling an Intel HEX file or the like is not recommended because
    any debug, symbol or source information will be lost.

  Again, the ISR implementation will be mostly hardware independent.
  The hardware dependent stuff accumulates in `rc5_init', so you best
  write that routine by hand. Concerning the ISRs, there is some potential
  for optimization; particularly in the ISRs' prologue and epilogue.
*/

// Test if we are avr-gcc
#if !defined (__GNUC__) || !defined (__AVR__)
#   error This code is hardware dependent and intended for avr-gcc!
#endif // avr-gcc

#include <avr/io.h>
#include <avr/interrupt.h>

// On older avr-gcc we need `avr/signal.h' (nowadays in `avr/interrupt.h').
#ifndef SIGNAL
#    include <avr/signal.h>
#endif

#include "rc5.h"

// The timer's counter register, counting up. As Timer0 comes along
// with the least resources, we use that timer. It is an 8 bit timer
// that overflows at 255 to 0 and generates a timer overflow IRQ each
// time it does so.
// If you use a 16 bit timer, make sure it has the same behaviour, e.g.
// by using its "clear timer on compare match" functionality.
#define RC5_TIMER_REG TCNT0

// The hardware port the IR receiver is connected to: INT0
// You may also use INT1 or use PCINT and filter for events that come
// from the IR receiver. You may also configure the analog comparator
// to work as external events' interrupt generator.
// !!! CHANGING THIS PORT REQUIRES ADJUSTING BOTH THE EXTERNAL
// !!! INTERRUPT'S SIGNAL AND THE RC5_XINT_ENABLE() FUNCTION.
#define RC5_PIN_REG   PIND
#define RC5_PAD       PORTD2

// We need the frequency the controller is running at.
// `F_CPU' is a commonly used identifier for it.
#ifndef F_CPU
#   error Please define F_CPU by adding a source line like #define F_CPU ...
#   error or by specifying the -D compiler option like '-DF_CPU=...'
#endif // F_CPU

/*
  Set `RC5_PRESCALE' to some reasonable default. It is the prescaler
  to use with the RC5 timer. As you can imagine, I did not test it
  for every possible F_CPU, so I hope for the best...
  Newer AVR derivatives provide a more fine grained prescaler.

  Allow `RC5_PRESCALE' to be defined on the command line with "-DRC5_..."
*/
#ifndef RC5_PRESCALE
#   if F_CPU >= 12000000
#       define RC5_PRESCALE 1024
#   elif F_CPU >= 4000000
#       define RC5_PRESCALE 256
#   else
#       define RC5_PRESCALE 64
#  endif // switch F_CPU
#endif // RC5_PRESCALE

// The implementation of target independent RC5 stuff
#include "rc5.c"

//////////////////////////////////////////////////////////////////////////////
// The interrupt service routines (ISRs)
//////////////////////////////////////////////////////////////////////////////

// I implemented the two ISRs as SIGNAL, i.e. they cannot be interrupted
// because the I-flag is unset by the AVR hardware during their execution.
//
// If you want to use the code in an application that has to fit harder
// real time requirements, i.e. shorter interrupt respond times for
// other IRQs, then you can implement the ISRs as INTERRUPTs or doing
// things like yield().
//
// However, there are some caveats. One of them is that you must care
// for and handle frequent transient errors because otherwise
// `rc5_xint_isr' will begin to recurse. One approach could be to
// disable only the external interrupt during execution of that routine.
// An other approath could be to lock the routine...

// The timer overflow interrupt service routine
SIGNAL (SIG_OVERFLOW0)
{
    rc5_timer_isr();
}

// The external interrupt's ISR
// This might also be a PCINT, a pin change interrupt on newer AVRs.
// If you use multiple pin change interrupts, then you have to add a test
// wether or not the pin change event comes from the IR receiver's pin.
// You can also configure the Analog Comparator to work as a
// source of pin change events.
SIGNAL (SIG_INTERRUPT0)
{
    rc5_xint_isr();
}

/*
  Here we go. What we have to implement:

  void rc5_timer_run (void)         Run (start) the overflow timer
  void rc5_timer_enable_irq (void)  Enable timer overflow IRQs
  void rc5_xint_enable (void)       Enable edge triggered external interrupt IRQs
*/

/*
  `RC5_TIMER_RUN' is the value that must be written to the
  timer configuration register in order to start it.

  The values below fit the controllers below.

  For other AVR derivatives this may look completely different.
  If so, take care for it in your own derivative's section below.
*/
#if (RC5_PRESCALE==1024)
#   define      RC5_TIMER_RUN ((1 << CS02) | (1 << CS00))
#elif   (RC5_PRESCALE==256)
#   define      RC5_TIMER_RUN (1 << CS02)
#elif   (RC5_PRESCALE==64)
#   define      RC5_TIMER_RUN ((1 << CS01) | (1 << CS00))
#else
#   error This RC5_PRESCALE is not supported
#endif // RC5_PRESCALE

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#if defined (__AVR_ATmega8__) || defined (__AVR_AT90S2313__) \
    || defined (__AVR_ATmega32__) || defined (__AVR_ATmega16__)
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void rc5_timer_run (void)
{
    TCCR0 = RC5_TIMER_RUN;
}

void rc5_timer_enable_irq (void)
{
    TIMSK |= (1 << TOIE0);
}

void rc5_xint_enable (void)
{
    MCUCR = (MCUCR | (1 << ISC00)) & ~ (1 << ISC01);

#if defined (__AVR_AT90S2313__)
    GIMSK |= (1 << INT0);
#else
    GICR |= (1 << INT0);
#endif // AT90S2313

//  MCUCR = (MCUCR | (1 << ISC10)) & ~ (1 << ISC11);
//  GIMSK |= (1 << INT1); // S2313
//  GICR |= (1 << INT1);  // M8
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#elif defined (__AVR_ATtiny2313__)
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void rc5_timer_run (void)
{
    TCCR0B = RC5_TIMER_RUN;
}

void rc5_timer_enable_irq (void)
{
    TIMSK |= (1 << TOIE0);
}

void rc5_xint_enable (void)
{
    // INTx on both edges
    MCUCR = (MCUCR | (1 << ISC00)) & ~ (1 << ISC01);
    GIMSK |= (1 << INT0);

//  MCUCR = (MCUCR | (1 << ISC10)) & ~ (1 << ISC11);
//  GIMSK |= (1 << INT1);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#elif defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) \
    || defined (__AVR_ATmega168__)
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void rc5_timer_run (void)
{
    TCCR0B = RC5_TIMER_RUN;
}

void rc5_timer_enable_irq (void)
{
    TIMSK0 |= (1 << TOIE0);
}

void rc5_xint_enable (void)
{
    // INTx on both edges
    EICRA = (EICRA | (1 << ISC00)) & ~ (1 << ISC01);
    EIMSK |= (1 << INT0);

//  EICRA = (EICRA | (1 << ISC10)) & ~ (1 << ISC11);
//  EIMSK |= (1 << INT1);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#else
#   error This AVR derivative is (not yet) supported.
#   error Feel free to add the required code.
#endif // AVR derivatives
////////////////////////////////////////////////////////////////////////////
//////////