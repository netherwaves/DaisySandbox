#include <daisy_seed.h>
#include <daisysp.h>

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

#define INL  in[i]
#define INR  in[i+1]
#define OUTL out[i]
#define OUTR out[i+1]

#define NUM_BUZZES          7

DaisySeed hw;
Compressor comp;
Svf voice_filter, buzzes[NUM_BUZZES];
Led indicator;

float sample_rate;
float env = 0;
float sqsum = 0;
float rms = 0;
float thresh = 0;

uint32_t dsp_counter = 0;

/**
 * @brief Calculate RMS amplitude based on input samples.
 * Implements onepole low pass filter
 * 
 * @param samp sample value [-1.0, 1.0]
 * @param win window time (in ms)
 * @param att attack time (in ms)
 * @param rel release time (in ms)
 * @return the smoothed RMS value
 */
float CalculateRMS(float& out, float samp, size_t win = 1, size_t att = 10, size_t rel = 500)
{
    // times in seconds
    float tatt  = exp(-1.0 / (sample_rate * (att * 1e-3)));
    float trel  = exp(-1.0 / (sample_rate * (rel * 1e-3)));
    float coeff = exp(-1.0 / (sample_rate * (win * 1e-3)));

    // mean squares !! :3c
    sqsum = (sqsum * (1 - coeff)) + (coeff * samp * samp);
    
    // rooted
    float rms = fclamp(fastroot(sqsum, 2.0), 0, 1);

    // attack/release smoothing
    float theta = rms > env ? tatt : trel;
    env = ((1.0 - theta) * rms) + (theta * env);
        
    // write to  rms out ,,.,,., mew
    out = env;
    return env;
}

/**
 * @brief Computes audio signals at callback rate defined by application.
 * 
 * @param in the interleaved input buffer
 * @param out the interleaved output buffer
 * @param size the size of the buffer
 */
void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    for(size_t i = 0; i < size; i += 2)
    {
        // compress dual source signal a little
        float post_limiter = comp.Process((INL + INR) * 0.5);

        // filter out higher frequencies (we don't need them)
        // plus it just introduces noise anyways
        float in_filt = post_limiter;

        for(int j = 0; j < 4; j++)
        {
            voice_filter.Process(in_filt);
            in_filt = voice_filter.Low();
        }

        // 1000Hz band stop
        float base_freq = 1000;
        for(int j = 0; j < NUM_BUZZES; j++)
        {
            buzzes[j].Process(in_filt);
            in_filt = buzzes[j].Notch();
        }

        // calculate RMS + pitch
        CalculateRMS(rms, in_filt, 1, 5, 200);     // compute RMS amplitude

        OUTL = OUTR = in_filt * 0.05;
    }

    indicator.Set(rms > thresh);
    indicator.Update();

    dsp_counter++;
    uint32_t cb_rate = hw.AudioCallbackRate();
    if(dsp_counter % (cb_rate / 10) == 0)
    {
        FixedCapStr<6> str;
        str.AppendFloat(rms);
        hw.PrintLine("RMS - %s", str.Cstr());
    }
}



int main(void)
{
    hw.Init();
    hw.StartLog(true);

    hw.SetAudioBlockSize(48);

    indicator.Init(D8, false);

    sample_rate = hw.AudioSampleRate();

    // voice lowpass filter
    voice_filter.Init(sample_rate);
    voice_filter.SetFreq(1300);
    voice_filter.SetRes(0.45);

    // digital buzz notch filter
    for(int i = 0; i < NUM_BUZZES; i++)
    {
        buzzes[i].Init(sample_rate);
        buzzes[i].SetFreq(i * 1000);
        buzzes[i].SetRes(0.15);
    }

    /**
     *  COMPRESSOR
     */
    comp.Init(sample_rate);
    comp.SetThreshold(-40);
    comp.SetAttack(0.05);
    comp.SetRelease(0.35);
    comp.SetRatio(40);
    comp.SetMakeup(36);

    AdcChannelConfig cfg;
    cfg.InitSingle(D19);
    hw.adc.Init(&cfg, 1);
    hw.adc.Start();

    hw.StartAudio(AudioCallback);

    for(;;)
    {
        thresh = 1.0 - hw.adc.GetFloat(0);
    }
}
