//define the peripheral to use to interface with the ESP8266
#define espSerial Serial1

//specify the baud rate to the ESP
const int espBaudRate = 115200;//19200;
const int espEnablePin = 2;

void setup()
{
  Serial.begin(9600);  //Setup serial to host computer
  espSerial.begin(espBaudRate);  //setup the serial to esp
  pinMode(espEnablePin, OUTPUT);
  digitalWrite(espEnablePin, HIGH);
}

void loop()
{
  while(Serial.available() > 0)
  {
    char c = Serial.read();
    espSerial.write(c);
    //if it is a carriage return, we have to also append newline for ESP command set
    if(c == '\r')
    {
      espSerial.write('\n');
    }
  }
  
  while(espSerial.available() > 0)
  {
    Serial.write(espSerial.read());
  }
}
