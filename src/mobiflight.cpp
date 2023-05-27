#define MF_NAME      "MCDU"
#define MF_TYPE      "A320 HomePit"
#define MF_VERSION   "3.0.0"

#include <Arduino.h>

#include <CmdMessenger.hpp>
#include <HomePitBus.hpp>

#include "mobiflight.hpp"

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

  auxBus.Init(460800);

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
  // mainBus.Init(460800);
}

/**
 * @brief Arduino application loop for Core 1.
 *
 */
void loop()
{
  while (rp2040.fifo.available())
  {
    uint32_t tmpData = rp2040.fifo.pop();
    
    uint8_t cmd = (tmpData >> 24) & 0xFF;
    uint8_t addr = (tmpData >> 16) & 0xFF;
    uint8_t data = (tmpData >> 8) & 0xFF;
    uint8_t value = tmpData & 0xFF;
  }
  
  auxBus.ProcessData();

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
    uint32_t tmpData = rp2040.fifo.pop();
    
    uint8_t cmd = (tmpData >> 24) & 0xFF;
    uint8_t addr = (tmpData >> 16) & 0xFF;
    uint8_t data = (tmpData >> 8) & 0xFF;
    uint8_t value = tmpData & 0xFF;

    mainBus.Send(cmd, addr, data, value);
  }

  mainBus.ProcessData();

  while (mainBus.index)
  {
    uint8_t addr = mainBus.FIFO[mainBus.index][0];
    uint8_t data = mainBus.FIFO[mainBus.index][1];
    uint8_t value = mainBus.FIFO[mainBus.index][2];

    uint32_t tmpData = (static_cast<uint32_t>('U') << 24) | (static_cast<uint32_t>(addr) << 16) | (static_cast<uint32_t>(data) << 8) | static_cast<uint32_t>(value);

    rp2040.fifo.push(tmpData);

    mainBus.index--;
  }
}
