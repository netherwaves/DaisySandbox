#include <daisy_seed.h>
#include <daisysp.h>

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

int main(void)
{
    hw.Init();
    hw.StartLog(true);

    System::Delay(1000);

    // -1109   340     4169    11946   157     -4
    uint16_t h = 11946;
    uint16_t a = 6, b = 9;
    float p = 0;
    p += fastpower((int32_t)(a - b)  * 1e-3, 2);

    FixedCapStr<10, char> str;
    str.AppendFloat(fastpower((int32_t)(a - b), 2));
    hw.PrintLine("%s", str.Cstr());

    for(;;) {}
}