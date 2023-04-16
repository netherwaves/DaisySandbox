#include <daisy_seed.h>
#include <daisysp.h>

using namespace daisy;
using namespace daisy::seed;
using namespace std;


struct PrintCue
{
    enum Type {
        COMMAND = 0,
        DATA
    };
    Type type;

    uint8_t cmd;        // if type == COMMAND
    wstring str;        // if type == DATA
};

/**
 * @brief Add string to print queue
 * @param str input string
 */
void QueueStr(wstring &str, vector<PrintCue*> &queue)
{
    PrintCue* cue = new PrintCue;

    cue->type = PrintCue::Type::DATA;
    cue->str  = str;

    queue.push_back(cue);
}

/**
 * @brief Add SetCursor command to print queue
 * 
 * @param row row
 * @param col column
 */
void QueueCursor(uint8_t row, uint8_t col, vector<PrintCue*> &queue)
{
    uint8_t row_offset = 0;
    if(row % 2 == 1) row_offset += 0x40;
    if(row > 1)      row_offset += 0x14;

    PrintCue* cue = new PrintCue;
    cue->type = PrintCue::Type::COMMAND;
    cue->cmd  = 0x80 + row_offset + col;         // 0x80 = SET_DDRAM_ADDR

    queue.push_back(cue);
}