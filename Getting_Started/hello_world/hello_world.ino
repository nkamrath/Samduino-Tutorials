void setup() {
  Serial.begin(9600);  //initialize the serial connection to the computer
}

void loop() {
  Serial.println("Hello World!");  //print the text "Hello World" to the computer via USB Serial
  delay(1000);  //delay one second
}
