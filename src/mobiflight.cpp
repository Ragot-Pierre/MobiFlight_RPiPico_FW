#define MF_NAME      "MCDU"
#define MF_TYPE      "A320 HomePit"
#define MF_VERSION   "3.0.0"

#include <Arduino.h>

#include <CmdMessenger.h>
#include <HomePitBus.h>

#include "mobiflight.h"

// Other defines.
static constexpr unsigned long POWER_SAVING_TIME_SECS = 60 * 60; // One hour (60 minutes * 60 seconds).

unsigned long lastButtonPress = 0;
bool powerSavingMode = false;

static char unique_serial_str[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1 + 3] = "SN-"; // The +1 is for the null terminator and +3 is for "ID-"

CmdMessenger cmdMessenger = CmdMessenger(Serial);

HomePitBus auxBus = HomePitBus(Serial1);
HomePitBus mainBus = HomePitBus(Serial2);

/**
 * @brief Registers callbacks for all supported MobiFlight commands.
 *
 */
void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(MFMessage::kSetPin, OnSetPin);
  cmdMessenger.attach(MFMessage::kGetInfo, OnGetInfo);
  cmdMessenger.attach(MFMessage::kGetConfig, OnGetConfig);
  cmdMessenger.attach(MFMessage::kSetConfig, OnSetConfig);
  cmdMessenger.attach(MFMessage::kResetConfig, SendOk);
  cmdMessenger.attach(MFMessage::kSaveConfig, OnSaveConfig);
  cmdMessenger.attach(MFMessage::kActivateConfig, OnActivateConfig);
  cmdMessenger.attach(MFMessage::kSetName, OnSetName);
  cmdMessenger.attach(MFMessage::kGenNewSerial, OnGenNewSerial);
  cmdMessenger.attach(MFMessage::kTrigger, SendOk);
  cmdMessenger.attach(MFMessage::kResetBoard, OnResetBoard);
}

/**
 * @brief General callback to simply respond OK to the desktop app for unsupported commands.
 *
 */
void SendOk()
{
  cmdMessenger.sendCmd(MFMessage::kStatus, F("OK"));
}

/**
 * @brief Callback for the MobiFlight event. This doesn't have to do anything so just report success.
 *
 */
void OnSaveConfig()
{
  cmdMessenger.sendCmd(MFMessage::kConfigSaved, F("OK"));
}

/**
 * @brief Callback for the MobiFlight event. This doesn't have to do anything so just report success.
 *
 */
void OnActivateConfig()
{
  cmdMessenger.sendCmd(MFMessage::kConfigActivated, F("OK"));
}

/**
 * @brief Loads or generates a new board serial number. Sends a kConfigActivated
 * message to MobiFlight for compatibility purposes.
 *
 */
void OnResetBoard()
{
  // This is required to maintain compatibility with the standard Mobiflight firmware
  // which eventually activates the config when resetting the board.
  OnActivateConfig();
}

/**
 * @brief Callback for handling an button type input from the cockpit.
 *
 * @param keyId The ID of the key that triggered the event
 * @param state State of the button (pressed or released)
 */
void SendButtonEvent(const int keyId, const bool state)
{
  lastButtonPress = millis();

  // if (keyId > ButtonNames::ButtonCount)
  // {
  //   cmdMessenger.sendCmd(MFMessage::kStatus, F("Keypress isn't valid"));
  //   return;
  // }

  // Send the button name and state to MobiFlight.
  cmdMessenger.sendCmdStart(MFMessage::kButtonChange);
  cmdMessenger.sendCmdArg(keyId);
  cmdMessenger.sendCmdArg(state);
  cmdMessenger.sendCmdEnd();
}

/**
 * @brief Callback for handling an analogue type input from the cockpit.
 *
 * @param keyId The ID of the key that triggered the event
 * @param value State of the button (pressed or released)
 */
void SendAnalogEvent(const int keyId, const int value)
{
  lastButtonPress = millis();

  // Send the button name and state to MobiFlight.
  cmdMessenger.sendCmdStart(MFMessage::kAnalogChange);
  cmdMessenger.sendCmdArg(keyId);
  cmdMessenger.sendCmdArg(value);
  cmdMessenger.sendCmdEnd();
}

/**
 * @brief Callback for setting the board configuration. Since the board configuration is fixed
 * any requests from MobiFlight to set the config are simply ignored and a remaining byte count of
 * 512 is sent back to keep the desktop app happy.
 *
 */
void OnSetConfig()
{
  cmdMessenger.sendCmd(MFMessage::kStatus, 512);
}

/**
 * @brief Callback for unknown commands.
 *
 */
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(MFMessage::kStatus, F("n/a"));
}

/**
 * @brief Callback for sending the board information to MobiFlight.
 *
 */
void OnGetInfo()
{
  cmdMessenger.sendCmdStart(MFMessage::kInfo);
  cmdMessenger.sendCmdArg(MF_TYPE);
  cmdMessenger.sendCmdArg(MF_NAME);
  cmdMessenger.sendCmdArg(unique_serial_str);
  cmdMessenger.sendCmdArg(MF_VERSION);
  cmdMessenger.sendCmdEnd();
}

/**
 * @brief Callback for sending module configuration to MobiFlight.
 * The module configuration is stored as a fixed string in EEPROM since it never changes.
 *
 */
void OnGetConfig()
{
  Serial.println(configString);
    // "10,
    // 1.0.L1:
    // 1.1.L2:
    // 1.2.L3:
    // 1.3.L4:
    // 1.4.L5:
    // 1.5.L6:
    // 1.6.MSG:
    // 1.7.DIR:
    // 1.8.IDX:
    // 1.9.TUN:
    // 1.10.A:
    // 1.11.H:
    // 1.12.O:
    // 1.13.V:
    // 1.14.FPLN:
    // 1.15.B:
    // 1.16.I:
    // 1.17.P:
    // 1.18.W:
    // 1.19.LEGS:
    // 1.20.C:
    // 1.21.J:
    // 1.22.Q:
    // 1.23.X:
    // 1.24.DEP_ARR:
    // 1.25.D:
    // 1.26.K:
    // 1.27.R:
    // 1.28.Y:
    // 1.29.PERF:
    // 1.30.E:
    // 1.31.L:
    // 1.32.S:
    // 1.33.Z:
    // 1.34.DSPL_MENU:
    // 1.35.F:
    // 1.36.M:
    // 1.37.T:
    // 1.38.SP:
    // 1.39.MFD_ADV:
    // 1.40.G:
    // 1.41.N:
    // 1.42.U:
    // 1.43.DIV:
    // 1.44.MFD_DATA:
    // 1.45.1:
    // 1.46.4:
    // 1.47.7:
    // 1.48.DOT:
    // 1.49.PREV:
    // 1.50.2:
    // 1.51.5:
    // 1.52.8:
    // 1.53.0:
    // 1.54.3:
    // 1.55.6:
    // 1.56.9:
    // 1.57.PLUSMINUS:
    // 1.58.R1:
    // 1.59.R2:
    // 1.60.R3:
    // 1.61.R4:
    // 1.62.R5:
    // 1.63.R6:
    // 1.64.EXEC:
    // 1.65.NEXT:
    // 1.66.CLR:
    // 1.67.BRT:
    // 1.68.DIM:
    // 1.69.DEL:
    // 3.70.Brightness:
    // ;"
    // ));
}

/**
 * @brief Callback for MobiFlight LED output commands.
 *
 */
void OnSetPin()
{
  // Read led state argument, interpret string as boolean
  int pin = cmdMessenger.readInt16Arg();
  int state = cmdMessenger.readInt16Arg();

  if (pin == 0)
  {
    cmdMessenger.sendCmd(kStatus, "OK");
    analogWrite(LED_BUILTIN, state);
  }
}

/**
 * @brief Serial numbers are hardcoded on Picos in the flash memory so this just
 * returns that value.
 *
 */
void OnGenNewSerial()
{
  cmdMessenger.sendCmdStart(MFMessage::kInfo);
  cmdMessenger.sendCmdArg(unique_serial_str);
  cmdMessenger.sendCmdEnd();
}

/**
 * @brief Stubbed out method to accept the name argument then discard it. The name
 * is actually hardcoded in the firmware.
 *
 */
void OnSetName()
{
  cmdMessenger.readStringArg();

  cmdMessenger.sendCmdStart(MFMessage::kStatus);
  cmdMessenger.sendCmdArg(MF_NAME);
  cmdMessenger.sendCmdEnd();
}

/**
 * @brief Checks to see if power saving mode should be enabled or disabled
 * based on the last time a key was pressed.
 *
 */
void CheckForPowerSave()
{
  if (!powerSavingMode && ((millis() - lastButtonPress) > (POWER_SAVING_TIME_SECS * 1000)))
  {
    powerSavingMode = true;
    // ledMatrix.SetPowerSaveMode(true);
  }
  else if (powerSavingMode && ((millis() - lastButtonPress) < (POWER_SAVING_TIME_SECS * 1000)))
  {
    // ledMatrix.SetPowerSaveMode(false);
    powerSavingMode = false;
  }
}

/**
 * @brief Arduino initialization method for the Core 1.
 *
 */
void setup()
{
  pico_get_unique_board_id_string(&unique_serial_str[3], sizeof(unique_serial_str) - 3);

  /* USB Serial connection - Baudrate doesn't matter here */
  Serial.begin(115200);

  // Serial1.begin(921600);
  Serial1.begin(460800);

  attachCommandCallbacks();
  cmdMessenger.printLfCr();

  OnResetBoard();

  lastButtonPress = millis();
}

/**
 * @brief Arduino initialization method for the Core 2.
 *
 */
void setup1()
{
  // Serial2.begin(921600);
  Serial2.begin(460800);
}

/**
 * @brief Arduino application loop for Core 1.
 *
 */
void loop()
{
  // while (rp2040.fifo.available())
  // {
  //   /* code */
  // }
  

  cmdMessenger.feedinSerialData();

  CheckForPowerSave();
}

/**
 * @brief Arduino application loop for Core 2.
 *
 */
void loop1()
{
  while (rp2040.fifo.available())
  {
    /* code */
  }
}
