#include "daisy_seed.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;

// Declare a DaisySeed object called hardware
DaisySeed hw;
TimerHandle tim;

uint8_t state = 0;
uint32_t last_time = 0;
uint32_t counter = 0;

void Callback(void* data)
{
    TimerHandle* tim = (TimerHandle*)data;

    state = 1 - state;
    hw.SetLed(state);
    hw.PrintLine("%ld", tim->GetTick());
}

int main(void)
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hw.Init();
    hw.StartLog();
    
    TimerHandle::Config cfg;
    
    cfg.periph     = TimerHandle::Config::Peripheral::TIM_3;
    cfg.enable_irq = true;
    cfg.period     = 10000 * 2;

    tim.Init(cfg);
    tim.SetPrescaler(10 - 1);                   // adjusted to 1kHz
    tim.SetCallback(Callback, (void*)&tim);
    tim.Start();
}
