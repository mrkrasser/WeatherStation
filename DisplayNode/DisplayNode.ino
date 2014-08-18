// DisplayNode - part of arduino-based weather station
// 
// Receive data over RF(315Mhz) and show in on display(PCD8544-based).
//

boolean debug=0;   //Set to 1 for console debugging

#include <RH_ASK.h>
#include <SPI.h>   
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

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

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

void setup()
{
    if (debug==1)
    { 
		Serial.begin(9600);	
		Serial.println("DisplayNode start.");
    }
        
	// init radio
    radio.init();
    radio.setModeRx();
    
    // led light
    pinMode(10, OUTPUT);
    digitalWrite(10, LOW);  
    
	// init display
    display.begin();
    display.setContrast(38);
    display.clearDisplay(); 
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.print("Waiting data!"); 
    display.display();
}

void loop()
{ 
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
    
    if (radio.recv(buf, &buflen)) // Non-blocking
    {
        memcpy(&sensor, &buf, sizeof(sensor)); 
        
		// show data on screen
        display.clearDisplay();
        display.setCursor(0,0);
        display.print("T:"); 
        display.print(sensor.ds18b20_t);
        display.println("*C ");
        display.print("H:"); 
        display.print(sensor.dht22_h);
        display.println("%");
        display.print("P:");
        display.print(sensor.bmp180_p*0.0075006375541921);
        display.println("mmHg");
        display.print("A:");
        display.print(sensor.bmp180_a);
        display.println("m");
        display.print("Bat.:");
        display.print(sensor.battery);
        display.println("mV");
        display.display();
           
		// enable backlight for 5 sec
        digitalWrite(10, LOW);   
        delay(5000);               
        digitalWrite(10, HIGH);  
     
		// print data for debugging
        if (debug==1)
        { 
			Serial.print("DHT22    T:"); 
			Serial.print(sensor.dht22_t);
			Serial.print("*C ");
			Serial.print("H:"); 
			Serial.print(sensor.dht22_h);
			Serial.println("%");
			  
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
			Serial.print("Bat.:");
			Serial.print(sensor.battery);
			Serial.println("mV");
        }
    }
}

