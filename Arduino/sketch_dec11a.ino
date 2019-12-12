#include <SPI.h>
#define SS 8

// Set up our SPI settings (2MHz, MSB, MODE0)
SPISettings settingsA(1000000, MSBFIRST, SPI_MODE0);

// variables to hold data (should be structs or better to organize data)
uint8_t data0, data1, data2, data3 = 0;
uint16_t thermocouple_temp, ic_temp = 0;
uint8_t fault, fault_scv, fault_scg, fault_oc = 0;
char buff[40];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     // Start Serial Monitor
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);   // Subordinate select HIGH disables shift register on MAX31855
  SPI.begin ();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(SS, LOW);    // Subordinate select LOW enables shift register on MAX31855
  SPI.beginTransaction(settingsA);  // Sets the parameters for SPI and disables interrupts

  // Transfer four bytes (MAX31855 is read-only so 0 is a dummy value)
  data0 = SPI.transfer(0);
  data1 = SPI.transfer(0);
  data2 = SPI.transfer(0);
  data3 = SPI.transfer(0);

  SPI.endTransaction();     // Turn interrupts back on
  digitalWrite(SS, HIGH);   // Subordinate select HIGH disables shift register on MAX31855

  // Clean up data
  // Thermocouple temperature
  thermocouple_temp = ((data0 << 6) | (data1 >> 2));
  if (thermocouple_temp >> 13)  // thermocouple temp is negative
  {
    thermocouple_temp &= 0x1FFF;  // Drop the 1 in the 14th bit
    thermocouple_temp = -1 * thermocouple_temp; // turn value negative
  }
  thermocouple_temp /= 4;   // temperature is 4 * temp in Celcius

  // IC internal temperature
  ic_temp = ((data2 << 4) | (data3 >> 4));
  if (ic_temp >> 13)  // thermocouple temp is negative
  {
    ic_temp &= 0x1FFF;  // Drop the 1 in the 14th bit
    ic_temp = -1 * thermocouple_temp; // turn value negative
  }
  ic_temp = ic_temp / 16;

  Serial.println("Thermocouple:");
  sprintf(buff, "Celcius: %d,  Farenheit: %d", thermocouple_temp, (thermocouple_temp * 9 + 5 * 32) / 5);
  Serial.println(buff);
  Serial.println("IC:");
  sprintf(buff, "Celcius: %d,  Farenheit: %d", ic_temp, (ic_temp * 9 + 5 * 32) / 5);
  Serial.println(buff);
  Serial.println("");

  delay(1000);              // Wait a second
}
