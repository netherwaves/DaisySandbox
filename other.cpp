#include "daisy_seed.h"
#include "sd.h"

using namespace daisy;

DaisySeed      hw;

int main(void)
{
    hw.Init();
    hw.StartLog(true);

    CreateSDAndRead(hw);
}