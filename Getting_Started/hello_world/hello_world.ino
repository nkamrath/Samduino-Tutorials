void setup() {
  //initialize the serial connection to the computer
  Serial.begin(9600);
}

void loop() {
  //print the text "Hello World" to the computer via USB Serial
  Serial.println("Hello World!");
  delay(1000);  //delay one second
}
