#include <daisy_seed.h>
#include <daisysp.h>

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

DaisySeed hw;
Oscillator osc;

void AudioCallback(AudioHandle::InterleavingInputBuffer in, AudioHandle::InterleavingOutputBuffer out, size_t size)
{
    for(size_t i = 0; i < size; i += 2)
    {
        out[i] = out[i+1] = osc.Process() * 0.5;
    }
}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(48);
    float sample_rate = hw.AudioSampleRate();

    osc.Init(sample_rate);
    osc.SetFreq(1000);
    osc.SetAmp(0.1);
    osc.SetWaveform(Oscillator::WAVE_SIN);

    hw.StartAudio(AudioCallback);

    for(;;) {}
}