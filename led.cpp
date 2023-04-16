#include <daisy_seed.h>
#include <daisysp.h>

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

DaisySeed hw;
Led led;
Oscillator osc;
TimerHandle hndl;

float val;

void AudioCallback(AudioHandle::InterleavingInputBuffer in, AudioHandle::InterleavingOutputBuffer out, size_t size)
{
    for(size_t i = 0; i < size; i += 2)
    {
        val = osc.Process();
    }
}

void LEDCallback(void* data)
{
    float val = fmap(osc.Process() * 0.5 + 0.5, 0.5, 1);
    led.Set(val);
    led.Update();
}

int main(void)
{
    hw.Init();
    
    float sample_rate = hw.AudioSampleRate();
    TimerHandle::Config timer_cfg;
    timer_cfg.periph = TimerHandle::Config::Peripheral::TIM_3;
    timer_cfg.enable_irq = true;

    auto tim_target_freq = 2500;
    auto tim_base_freq = System::GetPClk2Freq();
    timer_cfg.period = tim_base_freq / tim_target_freq / 2;

    hndl.Init(timer_cfg);
    hndl.SetCallback(LEDCallback);
    
    led.Init(D8, false, tim_target_freq);

    osc.Init(tim_target_freq);
    osc.SetAmp(1);
    osc.SetFreq(0.5);
    osc.SetWaveform(Oscillator::WAVE_TRI);

    hndl.Start();

    for(;;) {}
}