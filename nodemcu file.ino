#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include <Adafruit_Sensor.h> //for temp
#include <DHT.h> //for temp
#include <DHT_U.h> //for temp
#include <Wire.h> //for pressure i2c com
#include <Adafruit_BMP280.h> //for pressure

DHT dht(D3, DHT11); //for temp
Adafruit_BMP280 bmp; //for pressure



SoftwareSerial nodemcu(D6, D5); //D6 = Rx & D5 = Tx

float b = 0.0;
float c = 0.0;
float q=0.00;
float Current = 0.0;
float lpg = 0.0000;
float co = 0.0000;
float smoke = 0.0000;
int counter = 0;
String e="";
String a="";
String d="";
String f="";
String g="";
String h="";
String i="";
String j="";
int ptime=0;
int ptime2=0;
int ptime3=0;
int crtime=0;
//int led1=D0 ;
//int led2= D4;
//int led3= D7;


//char auth[] = "bIgSnIE_jSBC4-Xflrtc4mt7Vv51peLo";
const char* ssid = "Your wifi name";
const char* password = "your wifi password";

AsyncWebServer server(80);

IPAddress local_IP(192, 168, 0,27);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);


String getPressure() {
  float p = q;
  Serial.println(p);
  return String(p);
}

String readtemperature() {
  // Read temperature as Celsius (the default)
  float t = b;
  return String(t);
}

String readhumidity() {
  float h = c;
  return String(h);
}
String getcurrent() {
  float cr = Current;
  return String(cr);
}
String getlpg() {
  float l = lpg;
  return String(l);
}
String getco() {
  float cm = co;
  return String(cm);
}
String getsmoke() {
  float s = smoke;
  return String(s);
}
String getobject() {
  int o = counter;
  return String(o);
}
String gettempchng() {
  if(b>45)
  {
    e="WARNING! TEMPERATURE EXCEEDED THE LIMIT! Thermal Conditioning On";
  }
  else if(b<20)
  {
    e="WARNING! TEMPERATURE IS FALLING! Thermal conditioning Off";
  }
  else
  {
    e="";
  }
  return String(e); 
}
String gethumchng() {
  if(c>90)
  {
    a="WARNING! RELATIVE HUMIDITY EXCEEDED THE LIMIT! Use low temperature chilled water/glycol mixture or refrigerant for dehumidification";
  }
  else if(c<45)
  {
    a="WARNING! RELATIVE HUMIDITY IS FALLING! Use steam humidifiers for humidification";
  }
  else
  {
    a="";
  }
  return String(a); 
}
String getpressurechng() {
  if(q>30)
  {
    d="WARNING! PRESSURE IS RISING! Air Ventilation On(Fresh air Out)";
  }
  else if(q<12)
  {
    d="WARNING! PRESSURE IS FALLING! Air Ventilation On(Fresh Air In)";
  }
  else
  {
    d="";
  }
  return String(d); 
}

String getlpgchng() {
  if(lpg>=10)
  {
    i="WARNING! Rise in LPG Detected! Fire Alarm On!";
  }
  else
  {
    i="";
  }
  return String(i); 
}
String getsmokechng() {
  if(smoke>=70)
  {
    h="WARNING! Smoke Detected! Fire Alarm On!";
  }
  else
  {
    h="";
  }
  return String(h); 
}

String getcochng() {
  if(co>=70)
  {
    f="WARNING! CO Poisoning Alert!";
  }
  else
  {
    f="";
  }
  return String(f); 
}

String getobjectchng() {
  if(counter>10)
  {
    g="WARNING! Production Count Exceeded";
  }
  else
  {
    g="";
  }
  return String(g); 
}


void setup() {
  crtime=millis();
  Serial.begin(9600);
  nodemcu.begin(9600);
  dht.begin();//for temp
  pinMode(D0,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D7,OUTPUT);
  while (!Serial) continue;
  //for pressure
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
      if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());


  

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/webpage.html");
   });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getPressure().c_str());
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readtemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", readhumidity().c_str());
  });
  server.on("/current", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getcurrent().c_str());
  });
  server.on("/lpg", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getlpg().c_str());
  });
  server.on("/co", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getco().c_str());
  });
  server.on("/smoke", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getsmoke().c_str());
  });
  server.on("/object", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getobject().c_str());
  });
    server.on("/tempchng", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", gettempchng().c_str());
  });
  server.on("/humchng", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", gethumchng().c_str());
  });
    server.on("/pressurechng", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getpressurechng().c_str());
  });
      server.on("/cochng", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getcochng().c_str());
  });
      server.on("/objectchng", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getobjectchng().c_str());
  });
  server.on("/lpgchng", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getlpgchng().c_str());
  });
  server.on("/smokechng", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", getsmokechng().c_str());
  });



  server.begin();
}

void loop() {
  crtime=millis();

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  if (data == JsonObject::invalid()) {
    //Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }
  counter = data["counter"];
  Current = data["Current"];
  int x = data["lpg"];
  int y = data["co"];
  int z = data["smoke"];
   lpg=x;
  co=y;
  smoke=z;
  
  // temp_________________________________________________________________________
  delay(100);
  b = dht.readTemperature();
  delay(100);

  c = dht.readHumidity();
//for pressure_________________________________________________________
  float r = bmp.readPressure();
    q=(r-100500)/3;
    digitalWrite(D0,HIGH);

}
