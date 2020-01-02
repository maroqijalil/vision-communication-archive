void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  while(Serial.available()==0);

  String val = String(Serial.read());
 
  digitalWrite(LED_BUILTIN, LOW);
  
  delay(1000);
  
  if(Serial.available()>0)
  {
    Serial.println("Testing...");
    if(val=='1')
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }

    Serial.println(val);
  }
}
