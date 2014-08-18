Arduino-based weather station
==============================

One arduino board(SensorNode) measure Temperature(DS18B20), Humidity(DHT22), Pressure(BMP180) and send it via FS1000A(315Mhz) RF-module to another board.
Another arduino board(DisplayNode) receive data and show it on screen. 


Libraries Needed
-------------------

* RadioHead Packet Radio library (http://www.airspayce.com/mikem/arduino/RadioHead/)  
* DHT sensor library *https://github.com/adafruit/DHT-sensor-library)
* OneWire library (http://www.pjrc.com/teensy/td_libs_OneWire.html)
* DallasTemperature library (http://download.milesburton.com/Arduino/MaximTemperature/)
* Adafruit BMP085/BMP180 library (https://github.com/adafruit/Adafruit-BMP085-Library)
* Library for our Monochrome Nokia 5110 LCD Displays (https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library)
* Adafruit GFX graphics core library (https://github.com/adafruit/Adafruit-GFX-Library)

