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
 * @brief Check if value is present in list
 * @param value value to find
 * @param list list to search in
 * @return bool
*/
bool InList(const uint value, const uint *list)
{
  for (size_t i = 0; i < sizeof(list); i++) {
    if (list[i] == value) { return true; }
  }
  return false;
}

/**
 * @brief Translate the (addr, data) tuple to a uint pin number
 * @param addr address of the remote device
 * @param data remote "pin" number for this device
 * @return uint pin number as defined in the MobiFlight board definition
*/
uint TranslateFromTuple(uint8_t addr, uint8_t data)
{
  uint keyId = 0;

  switch (addr)
  {
  case MCDU_ADDR:
    keyId = data + MCDU_START;
    break;
    
  case FCU_ADDR:
    keyId = data + FCU_START;
    break;
    
  case CAPT_EFIS_ADDR:
    keyId = data + CAPT_EFIS_START;
    break;
    
  case FO_EFIS_ADDR:
    keyId = data + FO_EFIS_START;
    break;
  
  default:
    break;
  }

  return keyId;
}

/**
 * @brief Translate the uint pin number format to a (addr, data) tuple
 * @param keyId uint pin number to translate from
 * @param addr address of the remote device
 * @param data remote "pin" number for this device
*/
void TranslateToTuple(uint keyId, uint8_t& addr, uint8_t& data)
{
  if (keyId <= MCDU_RANGE)
  {
    addr = MCDU_ADDR;
    data = keyId - MCDU_START;
  }
  else if (keyId <= FCU_RANGE)
  {
    addr = FCU_ADDR;
    data = keyId - FCU_START;
  }
  else if (keyId <= CAPT_EFIS_RANGE)
  {
    addr = CAPT_EFIS_ADDR;
    data = keyId - CAPT_EFIS_START;
  }
  else if (keyId <= FO_EFIS_RANGE)
  {
    addr = FO_EFIS_ADDR;
    data = keyId - FO_EFIS_START;
  }
}

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
  cmdMessenger.attach(MFMessage::kTrigger, OnTrigger);
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
 * @brief Callback for handling an input from the cockpit.
 *
 * @param message The MFMessage type to announce if input is digital or analog
 * @param keyId The ID of the key that triggered the event
 * @param value State of the button (pressed or released)
 */
void SendEvent(const uint keyId, const uint value)
{
  MFMessage message = MFMessage::kButtonChange;

  if (InList(keyId, analogPins)) { message = MFMessage::kAnalogChange; }

  cmdMessenger.sendCmdStart(message);
  cmdMessenger.sendCmdArg(keyId);
  cmdMessenger.sendCmdArg(value);
  cmdMessenger.sendCmdEnd();

  lastButtonPress = millis();
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
  uint pin = cmdMessenger.readInt16Arg();
  uint value = cmdMessenger.readInt16Arg();

  cmdMessenger.sendCmd(kStatus, "OK");
  
  uint8_t addr = 0;
  uint8_t data = 0;

  TranslateToTuple(pin, addr, data);

  uint tmpData = (static_cast<uint>('W') << 24) | (static_cast<uint>(addr) << 16) | (static_cast<uint>(data) << 8) | static_cast<uint>(value);

  rp2040.fifo.push(tmpData);
}

/**
 * @brief Callback for MobiFlight trigger command.
 * Assumed to be used for fetching input states.
 */
void OnTrigger()
{
  uint pin = cmdMessenger.readInt16Arg();

  cmdMessenger.sendCmd(kStatus, "OK");
  
  uint8_t addr = 0;
  uint8_t data = 0;

  TranslateToTuple(pin, addr, data);

  uint tmpData = (static_cast<uint>('R') << 24) | (static_cast<uint>(addr) << 16) | (static_cast<uint>(data) << 8) | static_cast<uint>(0x00);

  rp2040.fifo.push(tmpData);
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
  mainBus.Init(460800);
}

/**
 * @brief Arduino application loop for Core 1.
 *
 */
void loop()
{
  uint keyId = 0;

  while (rp2040.fifo.available())
  {
    uint tmpData = rp2040.fifo.pop();
    
    uint8_t cmd = (tmpData >> 24) & 0xFF;
    uint8_t addr = (tmpData >> 16) & 0xFF;
    uint8_t data = (tmpData >> 8) & 0xFF;
    uint8_t value = tmpData & 0xFF;

    SendEvent(TranslateFromTuple(addr, data), value);
  }
  
  auxBus.ProcessData();

  for (uint8_t i = 0; i < auxBus.index; i++)
  {
    uint8_t addr = auxBus.FIFO[i][0];
    uint8_t data = auxBus.FIFO[i][1];
    uint8_t value = auxBus.FIFO[i][2];
    
    SendEvent(TranslateFromTuple(addr, data), value);
  }

  auxBus.index = 0;

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
    uint tmpData = rp2040.fifo.pop();
    
    uint8_t cmd = (tmpData >> 24) & 0xFF;
    uint8_t addr = (tmpData >> 16) & 0xFF;
    uint8_t data = (tmpData >> 8) & 0xFF;
    uint8_t value = tmpData & 0xFF;

    mainBus.Send(cmd, addr, data, value);

    if (!addr & !data)
    {
      analogWrite(LED_BUILTIN, value);
    }    
  }

  mainBus.ProcessData();

  for (uint8_t i = 0; i < mainBus.index; i++)
  {
    // uint8_t cmd = MFMessage::kButtonChange;
    uint8_t addr = mainBus.FIFO[i][0];
    uint8_t data = mainBus.FIFO[i][1];
    uint8_t value = mainBus.FIFO[i][2];

    // if (InList((uint)data, analogPins))
    // {
    //   cmd = MFMessage::kAnalogChange;
    // }

    uint tmpData = /*(static_cast<uint>(cmd) << 24) |*/ (static_cast<uint>(addr) << 16) | (static_cast<uint>(data) << 8) | static_cast<uint>(value);

    rp2040.fifo.push(tmpData);
  }

  mainBus.index = 0;
}
