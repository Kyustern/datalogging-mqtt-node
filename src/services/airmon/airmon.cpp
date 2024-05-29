#include <DFRobot_ENS160.h>
#include <airmon.h>
#include <Adafruit_NeoPixel.h>

#define I2C_COMMUNICATION  //I2C communication. Comment out this line of code if you want to use SPI communication.

#ifdef  I2C_COMMUNICATION
  /**
   *   For Fermion version, the default I2C address is 0x53, connect SDO pin to GND and I2C address will be 0x52
   */
  DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ 0x53);
#else
  /**
   * Set up digital pin according to the on-board pin connected with SPI chip-select pin
   * csPin Available Pins. For example: ESP32&ESP8266(D3), m0(6)
   */
  uint8_t csPin = D3;
  DFRobot_ENS160_SPI ENS160(&SPI, csPin);
#endif

  /**
   * Get the sensor operating status
   * Return value: 0-Normal operation, 
   *         1-Warm-Up phase, first 3 minutes after power-on.
   *         2-Initial Start-Up phase, first full hour of operation after initial power-on. Only once in the sensor’s lifetime.
   * note: Note that the status will only be stored in the non-volatile memory after an initial 24h of continuous
   *       operation. If unpowered before conclusion of said period, the ENS160 will resume "Initial Start-up" mode
   *       after re-powering.
   */

void initAirmon()
{
  // Init the sensor
  while( NO_ERR != ENS160.begin() ){
    Serial.println("Communication with ENS160 sensor failed, trying again in 3 seconds");
    delay(3000);
  }
  
  Serial.println("Airmon sensor started");

  /**
   * Set power mode
   * mode Configurable power mode:
   *   ENS160_SLEEP_MODE: DEEP SLEEP mode (low power standby)
   *   ENS160_IDLE_MODE: IDLE mode (low-power)
   *   ENS160_STANDARD_MODE: STANDARD Gas Sensing Modes
   */
  ENS160.setPWRMode(ENS160_STANDARD_MODE);

  /**
   * Users write ambient temperature and relative humidity into ENS160 for calibration and compensation of the measured gas data.
   * ambientTemp Compensate the current ambient temperature, float type, unit: C
   * relativeHumidity Compensate the current ambient temperature, float type, unit: %rH
   */

  //Here settoing some placeholder values
  ENS160.setTempAndHum(/*temperature=*/25.0, /*humidity=*/50.0);

}

void setTemperatureCompensation(AHT_Reading* readingPointer) {
  if (readingPointer->humidity != 0 && readingPointer->temperature != 0) {
    ENS160.setTempAndHum(readingPointer->temperature, readingPointer->humidity);
  }
  
}

//ppr.imagerie@chu-toulouse.fr

// uint32_t getAqiColorCode(int aqi = 0) {
  
// }

//TODO: Define thresholds for the returned values that will trigger an alert on the oled screen or turn on the status LED to indicate that something is not good
//TODO: Or drive a neo pixel type LED
void getAirmonReading(ENS_Reading* readingPointer)
{
  uint8_t Status = ENS160.getENS160Status();
  Serial.print("Sensor operating status : ");
  Serial.println(Status);

  readingPointer->aqi = ENS160.getAQI();
  readingPointer->eco2 = ENS160.getECO2();
  readingPointer->tvoc = ENS160.getTVOC();
}