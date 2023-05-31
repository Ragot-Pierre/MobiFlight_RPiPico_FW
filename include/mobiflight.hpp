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
3.0.MCDU-Debug LED:\
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
1.20.MCDU-RAD NAV:\
1.21.MCDU-FUEL PRED:\
1.22.MCDU-SEC F-PLN:\
1.23.MCDU-ATC COMM:\
1.24.MCDU-MCDU MENU:\
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
1.55.MCDU-MCDU MENU:\
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
3.75.MCDU-BACKLIGHT:\
1.76.FCU-B A/THR:\
3.77.FCU-L A/THR:\
1.78.FCU-B AP 1:\
3.79.FCU-L AP 1:\
1.80.FCU-B AP 2:\
3.81.FCU-L AP 2:\
1.82.FCU-HTVS:\
1.83.FCU-SPD/MACH:\
1.84.FCU-METRIC ALT:\
1.85.FCU-B LOC:\
3.86.FCU-L LOC:\
1.87.FCU-B EXPED:\
3.88.FCU-L EXPED:\
1.89.FCU-B APPR:\
3.90.FCU-L APPR:\
1.91.FCU-SPD +:\
1.92.FCU-SPD -:\
1.93.FCU-SPD PULL:\
1.94.FCU-SPD PUSH:\
1.95.FCU-HDG +:\
1.96.FCU-HDG -:\
1.97.FCU-HDG PULL:\
1.98.FCU-HDG PUSH:\
1.99.FCU-VS +:\
1.100.FCU-VS -:\
1.101.FCU-VS PULL:\
1.102.FCU-VS PUSH:\
1.103.FCU-ALT +:\
1.104.FCU-ALT -:\
1.105.FCU-ALT PULL:\
1.106.FCU-ALT PUSH:\
1.107.FCU-ALT 100:\
1.108.FCU-ALT 1000:\
3.109.FCU-SPD 1:\
3.110.FCU-SPD 2:\
3.111.FCU-SPD 3:\
3.112.FCU-SPD DOT:\
3.113.FCU-HDG 1:\
3.114.FCU-HDG 2:\
3.115.FCU-HDG 3:\
3.116.FCU-HDG DOT:\
3.117.FCU-ALT 1:\
3.118.FCU-ALT 2:\
3.119.FCU-ALT 3:\
3.120.FCU-ALT 4:\
3.121.FCU-ALT 5:\
3.122.FCU-ALT DOT:\
3.123.FCU-VS 1:\
3.124.FCU-VS 2:\
3.125.FCU-VS 3:\
3.126.FCU-VS 4:\
3.127.FCU-VS 5:\
3.128.FCU-BACKLIGHT:\
1.129.CT EFIS-B FD:\
3.130.CT EFIS-L FD:\
1.131.CT EFIS-B LS:\
3.132.CT EFIS-L LS:\
1.133.CT EFIS-B CSTR:\
3.134.CT EFIS-L CSTR:\
1.135.CT EFIS-B WPT:\
3.136.CT EFIS-L WPT:\
1.137.CT EFIS-B VOR D:\
3.138.CT EFIS-L VOR D:\
1.139.CT EFIS-B NDB:\
3.140.CT EFIS-L NDB:\
1.141.CT EFIS-B ARPT:\
3.142.CT EFIS-L ARPT:\
1.143.CT EFIS-NA1 ADF:\
1.144.CT EFIS-NA1 OFF:\
1.145.CT EFIS-NA1 VOR:\
1.146.CT EFIS-NA2 ADF:\
1.147.CT EFIS-NA2 OFF:\
1.148.CT EFIS-NA2 VOR:\
1.149.CT EFIS-MLS:\
1.150.CT EFIS-MVOR:\
1.151.CT EFIS-MNAV:\
1.152.CT EFIS-MARC:\
1.153.CT EFIS-MPLAN:\
1.154.CT EFIS-R10:\
1.155.CT EFIS-R20:\
1.156.CT EFIS-R40:\
1.157.CT EFIS-R80:\
1.158.CT EFIS-R160:\
1.159.CT EFIS-R320:\
1.160.CT EFIS-BARO +:\
1.161.CT EFIS-BARO -:\
1.162.CT EFIS-BARO PULL:\
1.163.CT EFIS-BARO PUSH:\
1.164.CT EFIS-BARO HG:\
1.165.CT EFIS-BARO HPA:\
3.166.CT EFIS-BARO QFE:\
3.167.CT EFIS-BARO QNH:\
3.168.CT EFIS-BARO 1:\
3.169.CT EFIS-BARO 2:\
3.170.CT EFIS-BARO 3:\
3.171.CT EFIS-BARO 4:\
3.172.CT EFIS-BARO DOT:\
3.173.CT EFIS-BACKLIGHT:\
1.174.FO EFIS-B FD:\
3.175.FO EFIS-L FD:\
1.176.FO EFIS-B LS:\
3.177.FO EFIS-L LS:\
1.178.FO EFIS-B CSTR:\
3.179.FO EFIS-L CSTR:\
1.180.FO EFIS-B WPT:\
3.181.FO EFIS-L WPT:\
1.182.FO EFIS-B VOR D:\
3.183.FO EFIS-L VOR D:\
1.184.FO EFIS-B NDB:\
3.185.FO EFIS-L NDB:\
1.186.FO EFIS-B ARPT:\
3.187.FO EFIS-L ARPT:\
1.188.FO EFIS-NA1 ADF:\
1.189.FO EFIS-NA1 OFF:\
1.190.FO EFIS-NA1 VOR:\
1.191.FO EFIS-NA2 ADF:\
1.192.FO EFIS-NA2 OFF:\
1.193.FO EFIS-NA2 VOR:\
1.194.FO EFIS-MLS:\
1.195.FO EFIS-MVOR:\
1.196.FO EFIS-MNAV:\
1.197.FO EFIS-MARC:\
1.198.FO EFIS-MPLAN:\
1.199.FO EFIS-R10:\
1.200.FO EFIS-R20:\
1.201.FO EFIS-R40:\
1.202.FO EFIS-R80:\
1.203.FO EFIS-R160:\
1.204.FO EFIS-R320:\
1.205.FO EFIS-BARO +:\
1.206.FO EFIS-BARO -:\
1.207.FO EFIS-BARO PULL:\
1.208.FO EFIS-BARO PUSH:\
1.209.FO EFIS-BARO HG:\
1.210.FO EFIS-BARO HPA:\
3.211.FO EFIS-BARO QFE:\
3.212.FO EFIS-BARO QNH:\
3.213.FO EFIS-BARO 1:\
3.214.FO EFIS-BARO 2:\
3.215.FO EFIS-BARO 3:\
3.216.FO EFIS-BARO 4:\
3.217.FO EFIS-BARO DOT:\
3.218.FO EFIS-BACKLIGHT:\
11.219.5.CT RMACP-VHF1 VOL:\
1.220.CT RMACP-VHF1 CALL B:\
3.221.CT RMACP-VHF1 CALL L:\
11.222.5.CT RMACP-VHF2 VOL:\
1.223.CT RMACP-VHF2 CALL B:\
3.224.CT RMACP-VHF2 CALL L:\
11.225.5.CT RMACP-VHF3 VOL:\
1.226.CT RMACP-VHF3 CALL B:\
3.227.CT RMACP-VHF3 CALL L:\
11.228.5.CT RMACP-HF1 VOL:\
1.229.CT RMACP-HF1 CALL B:\
3.230.CT RMACP-HF1 CALL L:\
11.231.5.CT RMACP-HF2 VOL:\
1.232.CT RMACP-HF2 CALL B:\
3.233.CT RMACP-HF2 CALL L:\
11.234.5.CT RMACP-PA VOL:\
1.235.CT RMACP-PA B:\
3.236.CT RMACP-PA L:\
11.237.5.CT RMACP-INT VOL:\
11.238.5.CT RMACP-CAB VOL:\
11.239.5.CT RMACP-VOR1 VOL:\
11.240.5.CT RMACP-VOR2 VOL:\
11.241.5.CT RMACP-MKR VOL:\
11.242.5.CT RMACP-ILS VOL:\
11.243.5.CT RMACP-MLS VOL:\
11.244.5.CT RMACP-ADF1 VOL:\
11.245.5.CT RMACP-ADF2 VOL:\
1.246.CT RMACP-VOICE B:\
3.247.CT RMACP-VOICE L:\
1.248.CT RMACP-RESET B:\
1.249.CT RMACP-INT B:\
1.250.CT RMACP-RAD B:\
1.251.CT RMACP-MECH B:\
3.252.CT RMACP-MECH L:\
1.253.CT RMACP-ATT B:\
3.254.CT RMACP-ATT L:\
3.255.CT RMACP-BACKLIGHT:\
1.256.CT RMACP-ON/OFF:\
1.257.CT RMACP-FREQ+:\
1.258.CT RMACP-FREQ-:\
1.259.CT RMACP-TRANSFER B:\
3.260.CT RMACP-TRANSFER L:\
3.261.CT RMACP-ACTIVE 1:\
3.262.CT RMACP-ACTIVE 2:\
3.263.CT RMACP-ACTIVE 3:\
3.264.CT RMACP-ACTIVE 4:\
3.265.CT RMACP-ACTIVE 5:\
3.266.CT RMACP-ACTIVE 6:\
3.267.CT RMACP-ACTIVE DOT:\
3.268.CT RMACP-STBY 1:\
3.269.CT RMACP-STBY 2:\
3.270.CT RMACP-STBY 3:\
3.271.CT RMACP-STBY 4:\
3.272.CT RMACP-STBY 5:\
3.273.CT RMACP-STBY 6:\
3.274.CT RMACP-STBY DOT:\
1.275.CT RMACP-VHF1 B:\
3.276.CT RMACP-VHF1 L:\
1.277.CT RMACP-VHF2 B:\
3.278.CT RMACP-VHF2 L:\
1.279.CT RMACP-VHF3 B:\
3.280.CT RMACP-VHF3 L:\
1.281.CT RMACP-HF1 B:\
3.282.CT RMACP-HF1 L:\
1.283.CT RMACP-HF2 B:\
3.284.CT RMACP-HF2 L:\
3.285.CT RMACP-SEL L:\
1.286.CT RMACP-AM B:\
3.287.CT RMACP-AM L:\
1.288.CT RMACP-NAV B:\
3.289.CT RMACP-NAV L:\
1.290.CT RMACP-VOR B:\
3.291.CT RMACP-VOR L:\
1.292.CT RMACP-ILS B:\
3.293.CT RMACP-ILS L:\
1.294.CT RMACP-MLS B:\
3.295.CT RMACP-MLS L:\
1.296.CT RMACP-ADF B:\
3.297.CT RMACP-ADF L:\
1.298.CT RMACP-BFO B:\
3.299.CT RMACP-BFO L:\
3.300.CT RMACP-BACKLIGHT:\
11.301.5.FO RMACP-VHF1 VOL:\
1.302.FO RMACP-VHF1 CALL B:\
3.303.FO RMACP-VHF1 CALL L:\
11.304.5.FO RMACP-VHF2 VOL:\
1.305.FO RMACP-VHF2 CALL B:\
3.306.FO RMACP-VHF2 CALL L:\
11.307.5.FO RMACP-VHF3 VOL:\
1.308.FO RMACP-VHF3 CALL B:\
3.309.FO RMACP-VHF3 CALL L:\
11.310.5.FO RMACP-HF1 VOL:\
1.311.FO RMACP-HF1 CALL B:\
3.312.FO RMACP-HF1 CALL L:\
11.313.5.FO RMACP-HF2 VOL:\
1.314.FO RMACP-HF2 CALL B:\
3.315.FO RMACP-HF2 CALL L:\
11.316.5.FO RMACP-PA VOL:\
1.317.FO RMACP-PA B:\
3.318.FO RMACP-PA L:\
11.319.5.FO RMACP-INT VOL:\
11.320.5.FO RMACP-CAB VOL:\
11.321.5.FO RMACP-VOR1 VOL:\
11.322.5.FO RMACP-VOR2 VOL:\
11.323.5.FO RMACP-MKR VOL:\
11.324.5.FO RMACP-ILS VOL:\
11.325.5.FO RMACP-MLS VOL:\
11.326.5.FO RMACP-ADF1 VOL:\
11.327.5.FO RMACP-ADF2 VOL:\
1.328.FO RMACP-VOICE B:\
3.329.FO RMACP-VOICE L:\
1.330.FO RMACP-RESET B:\
1.331.FO RMACP-INT B:\
1.332.FO RMACP-RAD B:\
1.333.FO RMACP-MECH B:\
3.334.FO RMACP-MECH L:\
1.335.FO RMACP-ATT B:\
3.336.FO RMACP-ATT L:\
3.337.FO RMACP-BACKLIGHT:\
1.338.FO RMACP-ON/OFF:\
1.339.FO RMACP-FREQ+:\
1.340.FO RMACP-FREQ-:\
1.341.FO RMACP-TRANSFER B:\
3.342.FO RMACP-TRANSFER L:\
3.343.FO RMACP-ACTIVE 1:\
3.344.FO RMACP-ACTIVE 2:\
3.345.FO RMACP-ACTIVE 3:\
3.346.FO RMACP-ACTIVE 4:\
3.347.FO RMACP-ACTIVE 5:\
3.348.FO RMACP-ACTIVE 6:\
3.349.FO RMACP-ACTIVE DOT:\
3.350.FO RMACP-STBY 1:\
3.351.FO RMACP-STBY 2:\
3.352.FO RMACP-STBY 3:\
3.353.FO RMACP-STBY 4:\
3.354.FO RMACP-STBY 5:\
3.355.FO RMACP-STBY 6:\
3.356.FO RMACP-STBY DOT:\
1.357.FO RMACP-VHF1 B:\
3.358.FO RMACP-VHF1 L:\
1.359.FO RMACP-VHF2 B:\
3.360.FO RMACP-VHF2 L:\
1.361.FO RMACP-VHF3 B:\
3.362.FO RMACP-VHF3 L:\
1.363.FO RMACP-HF1 B:\
3.364.FO RMACP-HF1 L:\
1.365.FO RMACP-HF2 B:\
3.366.FO RMACP-HF2 L:\
3.367.FO RMACP-SEL L:\
1.368.FO RMACP-AM B:\
3.369.FO RMACP-AM L:\
1.370.FO RMACP-NAV B:\
3.371.FO RMACP-NAV L:\
1.372.FO RMACP-VOR B:\
3.373.FO RMACP-VOR L:\
1.374.FO RMACP-ILS B:\
3.375.FO RMACP-ILS L:\
1.376.FO RMACP-MLS B:\
3.377.FO RMACP-MLS L:\
1.378.FO RMACP-ADF B:\
3.379.FO RMACP-ADF L:\
1.380.FO RMACP-BFO B:\
3.381.FO RMACP-BFO L:\
3.382.FO RMACP-BACKLIGHT:\
;";
