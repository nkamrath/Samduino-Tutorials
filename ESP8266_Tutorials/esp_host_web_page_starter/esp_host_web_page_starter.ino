//define the serial interface port
#define espSerial Serial1

//specify the baud rate to the ESP
const int espBaudRate = 115200;

uint8_t string_contains(String s, String pattern)
{
  uint8_t matched_length = 0;
  for(int i = 0; i < s.length(); i++)
  {
    if(s[i] == pattern[matched_length])
    {
      matched_length++;
    }
    else
    {
      matched_length = 0;
    }
    if(matched_length == pattern.length())
    {
      return 1;
    }
  }
  return 0;
}

String espWrite(String command, char* okString)
{
  String response = "";
  espSerial.print(command); // send the read character to the esp8266

  long int time = millis();
  long int waitUntil = time + 200;
   
  if(okString != NULL)
  {
    while(millis() < waitUntil && !string_contains(response, "busy") && !string_contains(response, okString))
    {
      while(espSerial.available())
      {
        response += (char)espSerial.read();
      }
    }
   
    //validate the response from the esp8266
    if(string_contains(response, "busy"))
    {
       
      if(!string_contains(response, okString))
      {
        Serial.print("got busy--------------");
        int searchOkTimeout = 1;

        while(!espSerial.find(okString) && searchOkTimeout != 0)
        {
          searchOkTimeout--;
        }
      }
      //retry after esp is ready
      espSerial.print(command);
    }
  }
  return response;
}



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
