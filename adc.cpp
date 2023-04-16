#include "daisy_seed.h"
#include "daisysp.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed hw;

int main(void)
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hw.Init();
    hw.StartLog(true);

    AdcChannelConfig adc_config[2];
    adc_config[0].InitSingle(D15);
    adc_config[1].InitSingle(D19);

    hw.adc.Init(adc_config, 2);
    hw.adc.Start();

    // Loop forever
    for(;;) {
        float val = pow(1.0 - hw.adc.GetFloat(0), 2) * 0.15;
        float val2 = 1.0 - hw.adc.GetFloat(1);

        FixedCapStr<5> str, str1;
        str.AppendFloat(val);
        str1.AppendFloat(val2);
        hw.PrintLine("%s - %s", str.Cstr(), str1.Cstr());

        System::Delay(100);
    }
}
