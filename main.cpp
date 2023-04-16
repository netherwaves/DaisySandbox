#include "daisy_seed.h"
#include "daisysp.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed hardware;
Oscillator osc;

void AudioCallback(AudioHandle::InterleavingInputBuffer in, AudioHandle::InterleavingOutputBuffer out, size_t size)
{
    for(size_t i = 0; i < size; i += 2)
    {
        // send to buffer
        out[i] = out[i+1] = in[i+1];
    }
}

int main(void)
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hardware.Init();
    
    hardware.SetAudioBlockSize(48);
    hardware.StartAudio(AudioCallback);

    // Loop forever
    for(;;) {
    }
}
