# IoT-project-on-industrial-operation-and-room-condition
**Main purpose of this project is to monitor various data of a running operation and room condition.**
<br>
**It also shows warning if any value crosses the preset threshold value.**
### This project consists of two part- arduino and nodemcu
Data is first collected in arduino through sensors and then sent to nodemcu through i2c protocol
<br>
Sensors used-
- Temperature and Humidity sensor (DHT11)
- Current sensor (ACS712-30A)
- Pressure sensor (BMP280)
- MQ2 gas sensor
- IR sensor for counter

Nodemcu uses SPIFFS protocol to host a dynamic webpage and present the respective sensor data.<br>
Be sure to organize your files in the same manner as shown on layout.jpg
