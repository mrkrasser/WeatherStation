// SensorNode - part of arduino-based weather station
// 
// Measure Temperature(DS18B20), Humidity(DHT22), Pressure(BMP180) and send it via FS1000A(315Mhz) RF-module
//

boolean debug=0;   //Set to 1 for console debugging

const int ONE_WIRE_PWR=8;
const int ONE_WIRE_BUS=3;
const int DHT22_PWR=9;
const int DHT22_PIN=2;

#include <RH_ASK.h>
#include <SPI.h>   // from RH_ASK: not actually used but needed to compile
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

DHT dht(DHT22_PIN, DHT22);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);
Adafruit_BMP085 bmp;
RH_ASK radio;

struct SensorData { 
  float dht22_t;
  float dht22_h;
  float ds18b20_t;
  float bmp180_t;
  float bmp180_p;
  float bmp180_a;
  long battery;
};

SensorData sensor;

// https://code.google.com/p/tinkerit/wiki/SecretVoltmeter
// reading power voltage using internal chip 1.1 reference
long readVcc() {
	long result;
	// Read 1.1V reference against AVcc
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	delay(2); // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Convert
	while (bit_is_set(ADCSRA,ADSC));
	result = ADCL;
	result |= ADCH<<8;
	result = 1126400L / result; // Back-calculate AVcc in mV
	return result;
}

void setup()
{
    if (debug==1)
    { 
      Serial.begin(9600);	  // Debugging only
      Serial.println("SensorNode start.");
    }
  
	// set power pins
    pinMode(DHT22_PWR,OUTPUT);
    pinMode(ONE_WIRE_PWR,OUTPUT);
   
	// init DHT22 and switch off power
    dht.begin();
    digitalWrite(DHT22_PWR,LOW);
     
	// init OneWire bus and switch off power
    ds18b20.begin();  
    digitalWrite(ONE_WIRE_PWR,LOW);
  
	// init BMP 180
    bmp.begin();
    
	// init radio
    radio.init();
    radio.setModeIdle();
}

void loop()
{
	// Read data from DHT22
	digitalWrite(DHT22_PWR,HIGH);
	delay(2000);   // time for wake up
	sensor.dht22_h = dht.readHumidity();
	sensor.dht22_t = dht.readTemperature();   // Read temperature as Celsius
	digitalWrite(DHT22_PWR,LOW);

	// Read data from OneWire
	digitalWrite(ONE_WIRE_PWR,HIGH);
	delay(4000);   // time for wake up
	ds18b20.requestTemperatures(); // Send the command to get temperatures
	sensor.ds18b20_t = ds18b20.getTempCByIndex(0);
	digitalWrite(ONE_WIRE_PWR,LOW);

	// Read data from BMP180
	sensor.bmp180_t = bmp.readTemperature();
	sensor.bmp180_p = bmp.readPressure();
	sensor.bmp180_a = bmp.readAltitude();
	
	// Read Voltage value
	sensor.battery = readVcc();

	// Enable radio and send data
	radio.setModeTx();
	radio.send((uint8_t*)&sensor, sizeof(sensor));
	radio.waitPacketSent();
	radio.setModeIdle();

	// Debugging output
	if (debug==1)
	{ 
	  Serial.print("DHT22    T:"); 
	  Serial.print(sensor.dht22_t);
	  Serial.print("*C ");
	  Serial.print("H:"); 
	  Serial.print(sensor.dht22_h);
	  Serial.println("%\t");
		  
	  Serial.print("DS18B20  T:");
	  Serial.print(sensor.ds18b20_t);  
	  Serial.println("*C ");
	  
	  Serial.print("BMP180   T:");
	  Serial.print(sensor.bmp180_t);
	  Serial.print("*C ");
	  Serial.print("P:");
	  Serial.print(sensor.bmp180_p);
	  Serial.print("Pa ");
	  Serial.print("A:");
	  Serial.print(sensor.bmp180_a);
	  Serial.println("m");
	  Serial.println(readVcc());
	}

	// Sleep for next reading
	delay(60000);
}


