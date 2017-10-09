// LoRa chip library RN2XX3
#include <rn2xx3.h>
#include <SoftwareSerial.h>

// Temperature/Humidity sensor SHT21 libaries 
#include <Wire.h>
#include <Sodaq_SHT2x.h>

SoftwareSerial mySerial(7, 8); // RX, TX
#define RST  2

// Copy the following lines from TTN Console -> Devices -> Overview tab -> "EXAMPLE CODE"
const char *devAddr = "XXX";
const char *nwkSKey = "XXX";
const char *appSKey = "XXX";

rn2xx3 myLora(mySerial);

float humidity = 0.0;
float temperature = 0.0;

// Setup routine runs once when you press reset
void setup() {
  pinMode(13, OUTPUT);
  led_on();

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Startup");

  // Reset rn2xx3
  pinMode(RST, OUTPUT);
  digitalWrite(RST, HIGH);
  digitalWrite(RST, LOW);
  delay(500);
  digitalWrite(RST, HIGH);

  // Initialise the rn2xx3 module
  myLora.autobaud();

  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(myLora.hweui());
  Serial.print("Microchip RN2xx3 version number: ");
  Serial.println(myLora.sysver());

  myLora.initABP(devAddr, appSKey, nwkSKey);
  Wire.begin();
  // Initialize the SHT21

  led_off();
  delay(2000);
}

// the loop routine runs over and over again forever:
void loop() {
  led_on();
  Serial.println("TXing");

  // Retrieving the sensors data
  humidity= SHT2x.GetHumidity();
  temperature= SHT2x.GetTemperature();
  
  // Encode floats as ints (20.98 becomes 2098)
  int16_t humidityInt = round(humidity * 100);
  int16_t temperatureInt = round(temperature * 100);

  Serial.println(humidityInt);
  Serial.println(temperatureInt);

   int sensorValue = analogRead(A0);
   float outvoltage = sensorValue * (5.0 / 1023.0);
   Serial.print("outvoltage = ");
   Serial.print(outvoltage);
   Serial.println("V");
   int16_t windSpeed = round(6*outvoltage * 100);
   Serial.print("wind speed is ");
   Serial.print(windSpeed /100);
   Serial.println(" level now");

  byte payload[6];
  payload[0] = highByte(temperatureInt);
  payload[1] = lowByte(temperatureInt);

  payload[2] = highByte(humidityInt);
  payload[3] = lowByte(humidityInt);

  
  payload[4] = highByte(windSpeed);
  payload[5] = lowByte(windSpeed);
  
  //myLora.txUncnf(String(humidity) + ' ' + String(temperature));
  myLora.txBytes(payload, sizeof(payload));
  led_off();

  delay(20000);
}

void led_on()
{
  digitalWrite(13, 1);
}

void led_off()
{
  digitalWrite(13, 0);
}
