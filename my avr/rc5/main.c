  #include <avr/io.h>
  #include <avr/interrupt.h>
  #include "rc5.h"
  #include "rc5.c"

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
//              ...

              // Tell receiver software to get more RC5 frames
              rc5.flip = -1;
          } // done RC5
      } // main loop

      // We never come here, thus no return needed.
      // It would be dead, anyway...
  }