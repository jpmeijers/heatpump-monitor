#include "reason.hpp"

#ifdef ESP_IDF_VERSION_MAJOR // IDF 4+
#if CONFIG_IDF_TARGET_ESP32 // ESP32/PICO-D4
#include "esp32/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/rom/rtc.h"
#else 
#error Target CONFIG_IDF_TARGET is not supported
#endif
#else // ESP32 Before IDF 4.0
#include "rom/rtc.h"
#endif

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

int get_reset_reason_code(int core) {
    return rtc_get_reset_reason(core);
}

String get_reset_reason(int reason)
{
  switch ( reason)
  {
    case 1 : return "POWERON_RESET";break;          /**<1,  Vbat power on reset*/
    case 3 : return "SW_RESET";break;               /**<3,  Software reset digital core*/
    case 4 : return "OWDT_RESET";break;             /**<4,  Legacy watch dog reset digital core*/
    case 5 : return "DEEPSLEEP_RESET";break;        /**<5,  Deep Sleep reset digital core*/
    case 6 : return "SDIO_RESET";break;             /**<6,  Reset by SLC module, reset digital core*/
    case 7 : return "TG0WDT_SYS_RESET";break;       /**<7,  Timer Group0 Watch dog reset digital core*/
    case 8 : return "TG1WDT_SYS_RESET";break;       /**<8,  Timer Group1 Watch dog reset digital core*/
    case 9 : return "RTCWDT_SYS_RESET";break;       /**<9,  RTC Watch dog Reset digital core*/
    case 10 : return "INTRUSION_RESET";break;       /**<10, Instrusion tested to reset CPU*/
    case 11 : return "TGWDT_CPU_RESET";break;       /**<11, Time Group reset CPU*/
    case 12 : return "SW_CPU_RESET";break;          /**<12, Software reset CPU*/
    case 13 : return "RTCWDT_CPU_RESET";break;      /**<13, RTC Watch dog Reset CPU*/
    case 14 : return "EXT_CPU_RESET";break;         /**<14, for APP CPU, reseted by PRO CPU*/
    case 15 : return "RTCWDT_BROWN_OUT_RESET";break;/**<15, Reset when the vdd voltage is not stable*/
    case 16 : return "RTCWDT_RTC_RESET";break;      /**<16, RTC Watch dog reset digital core and rtc module*/
    default : return "NO_MEAN";
  }
}

String get_reset_reason_verbose(int reason)
{
  switch ( reason)
  {
    case 1  : return "Vbat power on reset"; break;
    case 3  : return "Software reset digital core";break;
    case 4  : return "Legacy watch dog reset digital core";break;
    case 5  : return "Deep Sleep reset digital core";break;
    case 6  : return "Reset by SLC module, reset digital core";break;
    case 7  : return "Timer Group0 Watch dog reset digital core";break;
    case 8  : return "Timer Group1 Watch dog reset digital core";break;
    case 9  : return "RTC Watch dog Reset digital core";break;
    case 10 : return "Instrusion tested to reset CPU";break;
    case 11 : return "Time Group reset CPU";break;
    case 12 : return "Software reset CPU";break;
    case 13 : return "RTC Watch dog Reset CPU";break;
    case 14 : return "for APP CPU, reseted by PRO CPU";break;
    case 15 : return "Reset when the vdd voltage is not stable";break;
    case 16 : return "RTC Watch dog reset digital core and rtc module";break;
    default : return "NO_MEAN";
  }
}