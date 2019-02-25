#pragma NEXMON targetregion "patch"

#include <firmware_version.h>   // definition of firmware version macros
#include <debug.h>              // contains macros to access the debug hardware
#include <wrapper.h>            // wrapper definitions for functions that already exist in the firmware
#include <structs.h>            // structures that are used by the code in the firmware
#include <helper.h>             // useful helper functions
#include <patcher.h>            // macros used to craete patches such as BLPatch, BPatch, ...
#include <rates.h>              // rates used to build the ratespec for frame injection
#include <nexioctls.h>          // ioctls added in the nexmon patch
#include <capabilities.h>       // capabilities included in a nexmon patch

#include <local_wrapper.h>

static unsigned short lfsr_rand = 0xDEADu;

unsigned short prng_rand()
{
  unsigned int bit_rand  =
    ((lfsr_rand) ^ (lfsr_rand >> 2) ^ (lfsr_rand >> 3) ^ (lfsr_rand >> 5) ) & 1;
  lfsr_rand =  (lfsr_rand >> 1) | (bit_rand << 15);
  return lfsr_rand;
}

void prng_seed(unsigned int seed)
{
  if (seed != 0) {
    lfsr_rand = seed;
  }
}
