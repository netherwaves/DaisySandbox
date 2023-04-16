#include "daisy_seed.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;
using namespace daisy::seed;

// Declare a DaisySeed object called hardware
DaisySeed hw;
Switch btn;


int main(void)
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hw.Init();
    btn.Init(D14, 10);

    // Loop forever
    for(;;) {
        btn.Debounce();
        if(btn.Pressed())
        {
            System::Reset();
        }
    }
}
