#include "daisy_seed.h"

using namespace daisy;

SdmmcHandler   sdcard;
FatFSInterface fsi;
FIL            file;

void CreateSDAndRead(DaisySeed &hw)
{
    SdmmcHandler::Config sd_config;
    sd_config.Defaults();
    sd_config.speed = SdmmcHandler::Speed::VERY_FAST;
    sdcard.Init(sd_config);
    fsi.Init(FatFSInterface::Config::MEDIA_SD);

    System::Delay(1000);

    FRESULT r = f_mount(&fsi.GetSDFileSystem(), fsi.GetSDPath(), 0);
    hw.PrintLine("f_mount %ld", r);

    System::Delay(1000);

    hw.PrintLine("opening...");
    if(int result = f_open(&file, "0:/registry.json", (FA_OPEN_EXISTING | FA_READ)))
    {
        hw.PrintLine("COULD NOT OPEN - %ld", result);
    }
    else
    {
        hw.PrintLine("OPENED OK");
    }
}