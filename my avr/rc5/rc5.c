/*
 * RC5 decoder
 *
 * Contributed by Georg-Johann Lay <georgjohann@web.de>
 *
 * @Author  : Georg-Johann Lay
 * @Date    : 2007-02-02
 * @File    : rc5.c
 * @Language: ISO C 99
 * @Purpose : Implementation of RC5 receiver, target independent part.
 *            Do not compile this file.
 *            It is just included by the target dependent part `rc5-foo.h'.
 */

/*
  To implement an RC5 receiver there are basically two approaches:

  -1-
  Look at the signal level in time gaps of a bit's or half bit's
  duration. This gives a straight forward and simple implementation.
  However, the time intervals must be followed very strictly.
  Otherwise, synchronization will get lost and the receiver won't
  work. So one needs a quite exact timer. Moreover, not all RC5
  transmitters follow exactly the RC5 signal standard and introduce
  tolerances in bit durations that make this approach obsolete.

  -2-
  Synchronize with the signal at each signal edge and evaluate
  the time that elapsed since the last edge. This approch is much
  more powerful: It does not need sophisticated timer capabilities,
  allows both integrity checks of the received data and allowing
  tolerances in the transmitted protocol.

  We take way -2-
*/


// The rc5 data struct to communicate with the application
// typedef'ed in `rc5.h'
rc5_t rc5;

// Some tweaks...
#ifdef __GNUC__
#   define inline __attribute__((always_inline))
#endif // GCC

// Target dependent stuff that is needed in `rc5_init'.
// The implementation can be found in the target dependent source file.
static inline void rc5_timer_run (void);
static inline void rc5_timer_enable_irq (void);
static inline void rc5_xint_enable (void);

// (Mostly) target independent RC5 algorithms, implemented in this file
static inline void rc5_timer_isr (void);
static inline void rc5_xint_isr (void);

//////////////////////////////////////////////////////////////////////////////

// An RC5 frame has 14 bits:
// G1 G2[~C.6] F A4 A3 A2 A1 A0 C5 C4 C3 C2 C1 C0
// G: automatic Gain control bits
// F: flip bit, aka. toggle bit
// A: address
// C: command, complement if G2 may serve as seventh command bit
#define RC5_BITS_PER_FRAME 14

// Time in µs for a whole bit of RC5 (first & second half bit)
#define RC5_BIT_US   (64*27)

// Number of timer ticks for the duration of one RC5 bit.
// This can be used in a C source or by the preprocessor for
// a range check, i.e. a valid F_CPU/RC5_PRESCALE ratio.
#define RC5_TICKS_VAL \
    F_CPU / 1000 * RC5_BIT_US / 1000 / RC5_PRESCALE

#define RC5_TICKS \
    ((uint8_t) ((uint32_t) (RC5_TICKS_VAL)))

#define RC5_DELTA \
    ((RC5_TICKS_VAL) / 6)

// Test RC5_TICKS_VAL for a reasonable value
#if (RC5_TICKS_VAL > 200) || (RC5_TICKS_VAL < 12)
#error This is a bad combination of F_CPU and RC5_PRESCALE!
#endif

// This union allows to access 16 bits as word or as two bytes.
// This approach is (probably) more efficient than shifting.
union data16
{
    uint16_t asWord;
    uint8_t  asByte[2];
};

// Local information that we need just in this file
struct
{
    // The very RC5 data buffer
    union data16 data;
    // The address we shall listen to:
    // If the MSB is set all addresses are welcome
    uint8_t      addr;
    // Number of edges (external interrupts) occured so far
    uint8_t      nedge;
    // Number of half bits bits received so far
    uint8_t      hbits;
} rc5_info;


//////////////////////////////////////////////////////////////////////////////

void rc5_init (uint8_t addr)
{
    // Save RC5 address that we listen to
    rc5_info.addr = addr;

    // Reset half bit count
    rc5_info.hbits = 0;

    // .flip = -1: no RC5 data available, listen
    rc5.flip = -1;

    // Initialize the hardware
    rc5_timer_run ();
    rc5_timer_enable_irq ();
    rc5_xint_enable ();
}

//////////////////////////////////////////////////////////////////////////////

// This routine evaluates the RC5 frame and stores it in the
// `rc5' struct, provided it is ok and the address matches.
void rc5_timer_isr (void)
{
    // The collected bits.
    union data16 data = rc5_info.data;

    // Number of bits
    uint8_t nbits = (uint8_t) (1+rc5_info.hbits) / 2;

    // An RC5 frame consists of 14 bits.
    if (RC5_BITS_PER_FRAME == nbits
#ifdef RC5_RC6
        // check for the first AGC bit to be 1
        && data.asByte[1] >= 0x20
#else
        // check for the two AGC bits to be 1
        && data.asByte[1] >= 0x30
#endif // RC6
        // Did the application allow reception of new RC5 data?
        && rc5.flip < 0)
    {
        uint8_t rc5_code;
        uint8_t rc5_addr;

        // We do the bit manipulation stuff by hand because of code size.
        rc5_code = data.asByte[0] & 0x3f; // 0b00111111 : Bits #0..#5
#ifdef RC5_RC6
        // Second AGC is abused as ~code.6
        if (0 == (data.asByte[1] & 0x10))
            rc5_code |= (1 << 6);
#endif // RC6

        // Gather all address bits in data.asByte[1]
        data.asWord <<= 2;
        rc5_addr = data.asByte[1] & 0x1f; // 0b00011111 : Bits #6..#10

        // Are we addressee?
        if (rc5_info.addr == rc5_addr
            || rc5_info.addr >= 128)
        {
            // Yes. Store the RC5 frame: code (command), address and flip.
            rc5.addr = rc5_addr;
            rc5.code = rc5_code;

            int8_t flip = 0;
            if (data.asByte[1] & 0x20) // 0b00100000 : Bit #11
                flip = 1;
            rc5.flip = flip;
        }
    }
    // If we land here and `hbits' is not as expected,
    // we did not store anything and do just a reset.

    // Reset and wait for the next RC5 frame
    rc5_info.hbits = 0;
}

//////////////////////////////////////////////////////////////////////////////

// This routine collects the bits
void rc5_xint_isr (void)
{
    // The number of half bits.
    uint8_t hbits = rc5_info.hbits;

    // Either we are done with this frame or this is no RC5.
    // ...maybe some other protocol, maybe a transient
    // error or the like. Just run into the timer overflow and clean up
    // the mess there. Disabling XINT is not the best idea if we have
    // code size in mind. Errors will not occur very often, anyway.
    if (hbits >= 2*RC5_BITS_PER_FRAME -1)
        return;

    // Read the timer value.
    uint8_t timer = RC5_TIMER_REG;

    // Reset the timer to 0.
    RC5_TIMER_REG = 0;

    // The number of edges (interrupts) occured so far.
    uint8_t nedge = rc5_info.nedge;

    // Start of a new RC5 frame?
    if (0 == hbits)
    {
        // Yes:
        // We are only interested in falling edges
        if (1 == (RC5_PIN_REG >> RC5_PAD) % 2)
            return;

        // Set all bits of RC5 frame and edge count to 0
        rc5_info.data.asWord = 0;
        nedge = 0;

        // Mimic the first AGC's first half bit
        timer = RC5_TICKS/2;
    }

    // Number of half bits of the just elapsed period
    // is 1 or 2 or invalid.
    // As RC5 toggles in the middle of each bit one interval of
    // constant signal level lasts RC5_TICKS or RC5_TICKS/2.
    // In the first case, a bit's second part has the same level
    // as the subsequent bit's first part.

    // We received at least one half bit.
    hbits++;

    // Test interval's length
    if (timer > RC5_TICKS - RC5_DELTA)
    {
        // Two half bits: one more...
        hbits++;

        // reduce the validity check to the case of one half bit.
        timer -= RC5_TICKS/2;
    }

    // Test if a half bit's length is invalid:
    // |timer - RC5_TICKS/2| > RC5_DELTA ?
    if (timer < RC5_TICKS/2 - RC5_DELTA
        || timer > RC5_TICKS/2 + RC5_DELTA)
    {
        // Invalid:
        // Setting `nbits' to some invalid value will make
        // this ISR and the timer ISR ignore the rubbish.
        hbits = 0xff;

        // Force a soon timer overflow
        RC5_TIMER_REG = -RC5_TICKS;
    }
    else
    {
        // Valid:
        // Test if the edge occured in the middle of a bit, i.e.
        // if the number of half bits is odd.
        // If so, this edge tells us the bit's value.
        if (hbits % 2 == 1)
        {
            // We shift data left by one and then store the
            // received bit in the LSB. This is best because
            // RC5 is big endian (MSB first).
            union data16 data = rc5_info.data;
            data.asWord <<= 1;

            // Store the bit. Edge polarity toggles; due to signal
            // inversion, a falling edge (even) is a ONE and
            // a raising edge (odd) is a ZERO.
            // Note that edge count starts at 0.
            if (nedge % 2 == 0)
                data.asByte[0] |= 1;

            // Store data
            rc5_info.data = data;

            if (hbits >= 2*RC5_BITS_PER_FRAME -1)
                // We have collected all bits.
                // Maybe there will be one more IRQ (hbit) but we do not care.
                // Frame is complete: force a soon timer overflow.
                RC5_TIMER_REG = -RC5_TICKS;
        }
    }

    // write back the local copies
    rc5_info.hbits = hbits;
    rc5_info.nedge = 1+nedge;
}


