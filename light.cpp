#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
UartHandler uartNeopixel;


// NEOPIXEL CODES
#define NUM_PIXELS  3

uint8_t brightness = 1;
uint32_t prior_micros = 0;
uint8_t drawMem[NUM_PIXELS * 3];
uint8_t DMA_BUFFER_MEM_SECTION displayMem[NUM_PIXELS * 12];


void UpdateNeopixel(void *state, UartHandler::Result res)
{
    if(res != UartHandler::Result::OK) return;

    // TODO: CHECK IF BUSY

    const uint8_t *p = drawMem;
    const uint8_t *end = p + (NUM_PIXELS * 3);
    uint8_t *fb = displayMem;

    while(p < end)
    {
        uint8_t b = *p++;
        uint8_t g = *p++;
        uint8_t r = *p++;
        uint32_t mult = brightness + 1;
        b = (b * mult) >> 8;
        g = (g * mult) >> 8;
        r = (r * mult) >> 8;

        uint32_t n = 0;
        n = (g << 16) | (r << 8) | b;       // WS2812_GRB

        const uint8_t *stop = fb + 12;
        do {
            uint8_t x = 0x08;
            if (!(n & 0x00800000)) x |= 0x07;
            if (!(n & 0x00400000)) x |= 0xE0;
            n <<= 2;
            *fb++ = x;
        } while (fb < stop);
    }

    uint32_t microseconds_per_led = 30;

    // reset function
    uint32_t min_elapsed = (NUM_PIXELS * microseconds_per_led) + 300;
    if (min_elapsed < 2500) min_elapsed = 2500;
    uint32_t m;
    while (1){
        m = System::GetUs();
        if ((m - prior_micros) > min_elapsed) break;
    }
    prior_micros = m;

    // transmit...
    uartNeopixel.DmaTransmit(displayMem, NUM_PIXELS * 12, NULL, UpdateNeopixel, NULL);
    hw.SetLed(0);
}

void setPixel(uint32_t num, uint32_t color)
{
    if(num >= NUM_PIXELS) return;
    num *= 3;
    drawMem[num+0] = (color)       & 255;       // RED
    drawMem[num+1] = (color >> 8)  & 255;       // GREEN
    drawMem[num+2] = (color >> 16) & 255;       // BLUE
}


void AudioCallback(AudioHandle::InterleavingInputBuffer in, AudioHandle::InterleavingOutputBuffer out, size_t size)
{
    
}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(48);

    UartHandler::Config uart_conf;
    uart_conf.periph        = UartHandler::Config::Peripheral::USART_1;
    uart_conf.mode          = UartHandler::Config::Mode::TX;
    uart_conf.pin_config.tx = Pin(PORTB, 6);
    uart_conf.pin_config.rx = Pin(PORTB, 7);
    uart_conf.baudrate      = 4000000;
    uartNeopixel.Init(uart_conf);

    hw.SetLed(1);
    System::Delay(1000);

    setPixel(0, 0xff00ff);      // purple
    setPixel(1, 0x00ff00);      // green
    setPixel(2, 0xff0000);      // red
    // UpdateNeopixel();

    uartNeopixel.DmaTransmit(displayMem, NUM_PIXELS * 12, NULL, UpdateNeopixel, NULL);

    // hw.StartAudio(AudioCallback);

    for(;;) {
    }
}