#define LED_COUNT 4
int led[LED_COUNT] = {15, 16, 12, 13};

void displayCounter(int value) {
   for(int i=0; i < LED_COUNT; i++) {
    //1010
    if (bitRead(value, i) == 1) {
      digitalWrite(led[i], HIGH);
    } else {
      digitalWrite(led[i], LOW);
    }
   }
}

void setup() {
  // put your setup code here, to run once:
  for(int i=0; i < LED_COUNT; i++) {
    pinMode(led[i], OUTPUT);    
  }
  for(int i=0; i<16;i++) {
    displayCounter(i);
    delay(1000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  return;
}
