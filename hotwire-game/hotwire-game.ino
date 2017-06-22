/*
 ESP8266 Blink by Simon Peter
 Blink the blue LED on the ESP-01 module
 This example code is in the public domain
 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
uint8_t ACTIVATE_A = 2;
uint8_t ACTIVATE_B = 5;
uint8_t WIRE_CONTACT = 4;
uint8_t BUZZER = 8;
uint8_t RED   = 11;
uint8_t GREEN = 10;
uint8_t BLUE  =  9;


const uint8_t PLAYER_A  = 1;
const uint8_t PLAYER_B = 2;
uint8_t owner = PLAYER_A;


uint8_t game_over = false;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(ACTIVATE_A, INPUT);
  pinMode(ACTIVATE_B, INPUT);
  pinMode(WIRE_CONTACT, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  // make alarm if hot wire is touched
  digitalWrite(BUZZER, hotwire_touched());
  
  //state_machine();
  //show_state();
  // debug code
  led(hotwire_touched(), activate_A_touched(), activate_B_touched());
}

void state_machine() {
  if(hotwire_touched()) {
    game_over = true;
  } 
  if(activate_A_touched()) {
    if(game_over) {
      // reset game
      if(owner == PLAYER_A) game_over = false;
    }
    else {
      // change ownership
      owner = PLAYER_A;
    }
  }

  if(activate_B_touched()) {
    if(game_over) {
      // reset game
      if(owner == PLAYER_B) game_over = false;
    }
    else {
      // change ownership
      owner = PLAYER_B;
    }
  }  
}

void show_state() {
  if(game_over) {
    led(1, 0, 0);
  }
  else if(owner == PLAYER_A) {
    led(0, 1, 0);
  }
  else if(owner == PLAYER_B) {
    led(0, 0, 1);
  }
}

bool hotwire_touched() {
  return !digitalRead(WIRE_CONTACT); 
}

bool activate_A_touched() {
  return !digitalRead(ACTIVATE_A); 
}

bool activate_B_touched() {
  return !digitalRead(ACTIVATE_B);
}

void led(uint8_t red, uint8_t green, uint8_t blue) {
  digitalWrite(RED, !red);
  digitalWrite(GREEN, !green);
  digitalWrite(BLUE, !blue);
}

