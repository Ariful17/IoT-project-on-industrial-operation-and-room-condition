

#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <MQ2.h> //for gas

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(5, 6);

int pin = A0; //for gas
float lpg, co, smoke; //for gas
MQ2 mq2(pin); //for gas

double Vout = 0; //for current
double Current = 0; //for current
const double scale_factor = 0.185; //for current
const double vRef = 5.00;  //for current
const double resConvert = 1024; //for current
double resADC = vRef/resConvert; //for current
double zeroPoint = vRef/2; //for current

 int counter;
 int object=0;
 boolean state=0;
//float hum;

void setup() {
  Serial.begin(9600);

  nodemcu.begin(9600);
  mq2.begin(); //for gas
  delay(1000);

  Serial.println("Program started");
}

void loop() {

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  //Obtain Temp and Hum data
  dht11_func();


  //Assign collected data to JSON Object
  data["counter"] = object;
  data["Current"]= Current;
  data["lpg"]= lpg;
  data["co"]= co;
  data["smoke"]= smoke;
  //Send data to NodeMCU
  data.printTo(nodemcu);
  jsonBuffer.clear();

  delay(100);
}

void dht11_func() {
  // counter______________________________________
int a= analogRead(A2);
Serial.print("object count=");
Serial.print(" "); 
if(a>100 && state==0)
{
  object++;
  state=1;
}
if(a<100 && state==1)
{
  state=0;
}
Serial.print(object); 
Serial.print("  ");
delay(100);
//current_________________________________________________________________________

  for(int i = 0; i < 1000; i++) {
    Vout = (Vout + (resADC * analogRead(A1)));   
  }
  Vout = Vout /1000;
  Current = (Vout - zeroPoint)/ scale_factor;                                    
                            
  Serial.print("Current = ");                  
  Serial.print(Current,2);
  Serial.println(" Amps  ");
  delay(100);

//gas_________________________________________________________________________________
  lpg = mq2.readLPG();
  Serial.print("lpg: ");
  Serial.print(lpg);
  Serial.print(" ppm ");
  delay(100);
  co = mq2.readCO();
  Serial.print("co: ");
  Serial.print(co);
  Serial.print(" ppm ");
  delay(100);
  smoke = mq2.readSmoke();
  Serial.print("smoke: ");
  Serial.print(smoke);
  Serial.println(" ppm");
  delay(100);

}
