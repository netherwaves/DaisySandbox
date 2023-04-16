#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

DaisySeed hw;

void AudioCallback(AudioHandle::InterleavingInputBuffer in, AudioHandle::InterleavingOutputBuffer out, size_t size)
{
  for (size_t i = 0; i < size; i += 2)
  {
    out[i] = out[i+1] = in[i];
  }
}

int main(void)
{
  hw.Init();
  hw.SetAudioBlockSize(48);
  float sample_rate = hw.AudioSampleRate();

  hw.StartAudio(AudioCallback);

  for(;;) {}
}