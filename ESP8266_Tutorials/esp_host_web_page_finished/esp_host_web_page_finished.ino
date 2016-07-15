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
 
void espInit()
{
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  espWrite("AT+RST\r\n", NULL); // reset module
  delay(1000);
  espSerial.flush();
  espWrite("AT+CWMODE=2\r\n", "OK"); // configure as access point
  espWrite("AT+CWSAP=\"ESPWIFI_NATE1234\",\"TEST\",1,0,4\r\n", "OK");
  espWrite("AT+CIFSR\r\n", "OK"); // get ip address
  espWrite("AT+CIPMUX=1\r\n", "OK"); // configure for multiple connections
  espWrite("AT+CIPSERVER=1,80\r\n", "OK"); // turn on server on port 80
}

void setup()
{
  Serial.begin(9600);  //Setup serial to host computer
  espSerial.begin(espBaudRate);  //setup the serial to esp
  
  //init the esp
  espInit();
}

void loop()
{
  //look for the receive data packet from ESP
  if(espSerial.find("+IPD,"))
  { 
    
    while(espSerial.available() == 0);
    int connectionId = espSerial.read()-48; // subtract 48 because the read() function returns 
                      // the ASCII decimal value and 0 (the first decimal number) starts at 48

    String webPage = "<!DOCTYPE html>\n<html><body>";
    webPage += "Run Time (M:S): ";
    webPage += (millis()/60000);
    webPage += ":";
    webPage += ((millis() % 60000)/1000);
    webPage += "\n</body></html>";
      //send the web page
      const int dataLength = 2048;
      for(unsigned int i = 0; i < webPage.length(); i+= dataLength)
      {
        String chunk = "";
        for(unsigned int j = 0; (j+i < webPage.length() && j < dataLength); j++)
        {
          chunk += webPage[i+j];
        }
        
        String cipSend = "AT+CIPSEND=";
        cipSend += connectionId;
        cipSend += ",";
        cipSend +=chunk.length();
        cipSend +="\r\n";
        String response;
        response = espWrite(cipSend, "OK");
        response = espWrite(chunk, "OK");
      }
   
      String closeCommand = "AT+CIPCLOSE="; 
      closeCommand+=connectionId; // append connection id
      closeCommand+="\r\n";
     
      String response = espWrite(closeCommand, "OK");
     //192.168.4.1 
    }
}
