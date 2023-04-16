#include "daisy_seed.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;

// Declare a DaisySeed object called hardware
DaisySeed hw;

uint32_t t;

int main(void)
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hw.Init();
    hw.StartLog(true);

    t = System::GetNow();
}
