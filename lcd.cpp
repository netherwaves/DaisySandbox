#include <daisy_seed.h>
#include "lcd_helpers.h"

using namespace daisy;
using namespace daisy::seed;
using namespace std;

#define LCD_PIN_RS          D28
#define LCD_PIN_EN          D27
#define LCD_PIN_D4          D26
#define LCD_PIN_D5          D25
#define LCD_PIN_D6          D24
#define LCD_PIN_D7          D23

DaisySeed hw;
LcdHD44780 lcd;
TimerHandle screen_timer;

vector<PrintCue*> lcd_queue;
bool nibble = 0;
size_t lcd_charidx = 0;


/**
 * @brief Callback for the LCD timer;
 * will print any and all uint8_t data in the lcd_queue
 * at a rate of 1kHz (1ms / output)
 * 
 * @param data 
 */
void LCDCallback(void* data)
{
    lcd.ResetEnablePin();                                   // write low enable pin (was set in PrintAsync / WriteAsync)
    if(lcd_queue.size() == 0) return;                       // check size of queue
    
    // queue is full; proceed
    uint8_t out_char = 0;                                   // the output here !

    // retrieve data
    PrintCue* cue = lcd_queue.at(0);
    wstring   curr;

    if(cue->type == PrintCue::Type::COMMAND)
    {
        out_char = cue->cmd;                                 // get command data
        lcd.SetCommandRegister();                           // set command register
        lcd.PrintAsync(out_char, nibble);                   // print command
    }
    else
    {
        wchar_t curr_char = cue->str[lcd_charidx];              // get character to write

        auto search = custom_chars.find(curr_char);
        // if we have to render one of the diacritic characters, print it
        // else, print normal character
        out_char = search != custom_chars.end()
                    ? search->second.pos
                    : wctob(curr_char);

        lcd.SetDataRegister();                              // set data register
        lcd.PrintAsync(out_char, nibble);                   // print character nibble
    }
    
    nibble = !nibble;                                       // switch nibble
    if(!nibble)                                             // data done printing (2 callback cycles)
    {
        if(cue->type == PrintCue::Type::COMMAND)             // check if we are done with command
        {
            lcd_queue.erase(lcd_queue.begin());             // always done after 1 byte (sory)
            delete cue;
            lcd_charidx = 0;
        }

        if(cue->type == PrintCue::Type::DATA                 // check if we are done with string
           && ++lcd_charidx >= cue->str.length())
        {
            lcd_queue.erase(lcd_queue.begin());             // pop element
            delete cue;
            lcd_charidx = 0;                                // reset char index
        }
    }
}

int main(void)
{
    hw.Init();

    LcdHD44780::Config lcd_cfg;

    lcd_cfg.cursor_blink  = false;
    lcd_cfg.cursor_on     = false;

    lcd_cfg.rs = LCD_PIN_RS;
    lcd_cfg.en = LCD_PIN_EN;
    lcd_cfg.d4 = LCD_PIN_D4;
    lcd_cfg.d5 = LCD_PIN_D5;
    lcd_cfg.d6 = LCD_PIN_D6;
    lcd_cfg.d7 = LCD_PIN_D7;

    System::Delay(500);
    screen.Init(lcd_cfg);
    System::Delay(500);

    // configure TIM4
    TimerHandle::Config tim_cfg;
    
    tim_cfg.periph     = TimerHandle::Config::Peripheral::TIM_4;
    tim_cfg.enable_irq = true;
    tim_cfg.period     = 10000 * 2;         // 10kHz

    screen_timer.Init(tim_cfg);
    screen_timer.SetPrescaler(10 - 1);      // scaled to 1kHz (this works)
    screen_timer.SetCallback(LCDCallback);
    screen_timer.Start();

    screen.SetCursor(0,0);
    screen.Print("abcdefghijklmnopqrst");
    screen.SetCursor(1, 0);
    screen.Print("____________________");
    screen.SetCursor(2, 0);
    screen.Print("--------------------");
    screen.SetCursor(3, 0);
    screen.Print("////////////////////");

    for(;;) {}
}