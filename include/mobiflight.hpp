#pragma once

#define MCDU_START                    0
#define MCDU_RANGE                    75
#define MCDU_ADDR                     0x00
#define FCU_START                     76
#define FCU_RANGE                     128
#define FCU_ADDR                      0x01
#define CAPT_EFIS_START               129
#define CAPT_EFIS_RANGE               173
#define CAPT_EFIS_ADDR                0x02
#define FO_EFIS_START                 174
#define FO_EFIS_RANGE                 218
#define FO_EFIS_ADDR                  0x03


#define OVH_START                     231
#define OVH_RANGE                     233
#define OVH_ADDR                      0x04

enum MFDevice
{
    kTypeNotSet,              // 0
    kTypeButton,              // 1
    kTypeEncoderSingleDetent, // 2 (retained for backwards compatibility, use kTypeEncoder for new configs)
    kTypeOutput,              // 3
    kTypeLedSegment,          // 4
    kTypeStepperDeprecated1,  // 5 (keep for backwards compatibility, doesn't support autohome)
    kTypeServo,               // 6
    kTypeLcdDisplayI2C,       // 7
    kTypeEncoder,             // 8
    kTypeStepperDeprecated2,  // 9 (keep for backwards compatibility, stepper type with auto zero support if btnPin is > 0)
    kTypeOutputShifter,       // 10 Shift register support (example: 74HC595, TLC592X)
    kTypeAnalogInput,         // 11 Analog Device with 1 pin
    kTypeInputShifter,        // 12 Input shift register support (example: 74HC165)
    kTypeMuxDriver,           // 13 Multiplexer selector support (generates select outputs)
    kTypeDigInMux,            // 14 Digital input multiplexer support (example: 74HCT4067, 74HCT4051)
    kTypeStepper              // new stepper type with settings for backlash and deactivate output
};

// This is the list of recognized commands. These can be commands that can either be sent or received.
// In order to receive, attach a callback function to these events
//
// If you increase this list, make sure to check that the MAXCALLBACKS value
// in CmdMessenger.h is set apropriately

enum MFMessage
{
    kInitModule,           // 0
    kSetModule,            // 1
    kSetPin,               // 2
    kSetStepper,           // 3
    kSetServo,             // 4
    kStatus,               // 5, Command to report status
    kEncoderChange,        // 6
    kButtonChange,         // 7
    kStepperChange,        // 8
    kGetInfo,              // 9
    kInfo,                 // 10
    kSetConfig,            // 11
    kGetConfig,            // 12
    kResetConfig,          // 13
    kSaveConfig,           // 14
    kConfigSaved,          // 15
    kActivateConfig,       // 16
    kConfigActivated,      // 17
    kSetPowerSavingMode,   // 18
    kSetName,              // 19
    kGenNewSerial,         // 20
    kResetStepper,         // 21
    kSetZeroStepper,       // 22
    kTrigger,              // 23
    kResetBoard,           // 24
    kSetLcdDisplayI2C,     // 25
    kSetModuleBrightness,  // 26
    kSetShiftRegisterPins, // 27
    kAnalogChange,         // 28
    kInputShifterChange,   // 29
    kDigInMuxChange,       // 30
    kSetStepperSpeedAccel, // 31
    kDebug = 0xFF          // 255
};

void attachCommandCallbacks();

void OnActivateConfig();
void OnGenNewSerial();
void OnGetConfig();
void OnGetInfo();
void OnResetBoard();
void OnSaveConfig();
void OnSetConfig();
void OnSetName();
void OnSetPin();
void OnTrigger();

void OnUnknownCommand();

void SendOk();

void SendEvent(const uint keyId, const uint value);

bool InList(const uint value, const uint* list);
void TranslateToTuple(const uint keyId, uint8_t& addr, uint8_t& data);
uint TranslateFromTuple(const uint8_t addr, const uint8_t data);

const uint analogPins[] = {654,655,656,657}; // To be filled with known analog pin numbers, mostly on audio panels and light brightness potentiometers.

const char* configString =
"10,\
3.0.MCDU-Debug_LED:\
1.1.MCDU-L1:\
1.2.MCDU-L2:\
1.3.MCDU-L3:\
1.4.MCDU-L4:\
1.5.MCDU-L5:\
1.6.MCDU-L6:\
1.7.MCDU-R1:\
1.8.MCDU-R2:\
1.9.MCDU-R3:\
1.10.MCDU-R4:\
1.11.MCDU-R5:\
1.12.MCDU-R6:\
1.13.MCDU-DIR:\
1.14.MCDU-PROG:\
1.15.MCDU-PERF:\
1.16.MCDU-INIT:\
1.17.MCDU-DATA:\
1.18.MCDU-VIDE_1:\
1.19.MCDU-F-PLN:\
1.20.MCDU-RAD_NAV:\
1.21.MCDU-FUEL_PRED:\
1.22.MCDU-SEC_F-PLN:\
1.23.MCDU-ATC_COMM:\
1.24.MCDU-MCDU_MENU:\
1.25.MCDU-AIRPORT:\
1.26.MCDU-VIDE_2:\
1.27.MCDU-LEFT:\
1.28.MCDU-UP:\
1.29.MCDU-RIGHT:\
1.30.MCDU-DOWN:\
1.31.MCDU-1:\
1.32.MCDU-2:\
1.33.MCDU-3:\
1.34.MCDU-4:\
1.35.MCDU-5:\
1.36.MCDU-6:\
1.37.MCDU-7:\
1.38.MCDU-8:\
1.39.MCDU-9:\
1.40.MCDU-COMMA:\
1.41.MCDU-0:\
1.42.MCDU-+/-:\
1.43.MCDU-A:\
1.44.MCDU-B:\
1.45.MCDU-C:\
1.46.MCDU-D:\
1.47.MCDU-E:\
1.48.MCDU-F:\
1.49.MCDU-G:\
1.50.MCDU-H:\
1.51.MCDU-I:\
1.52.MCDU-J:\
1.53.MCDU-K:\
1.54.MCDU-L:\
1.55.MCDU-MCDU_MENU:\
1.56.MCDU-N:\
1.57.MCDU-O:\
1.58.MCDU-P:\
1.59.MCDU-Q:\
1.60.MCDU-R:\
1.61.MCDU-S:\
1.62.MCDU-T:\
1.63.MCDU-U:\
1.64.MCDU-V:\
1.65.MCDU-W:\
1.66.MCDU-X:\
1.67.MCDU-Y:\
1.68.MCDU-Z:\
1.69.MCDU-/:\
1.70.MCDU-SP:\
1.71.MCDU-OVFY:\
1.72.MCDU-CLR:\
1.73.MCDU-BRT:\
1.74.MCDU-DIM:\
3.75.MCDU-FM1:\
3.76.MCDU-IND:\
3.77.MCDU-RDY:\
3.78.MCDU-XXX:\
3.79.MCDU-FM2:\
3.80.MCDU-FAIL_FMGC:\
3.81.MCDU-MCDU_MENU_ANC:\
3.82.MCDU-BACKLIGHT:\
1.231.231:\
3.232.OVH-ELEC_AV:\
3.233.OVH-ELEC_ON:\
;";
