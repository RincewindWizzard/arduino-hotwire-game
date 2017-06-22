#include<avr/sleep.h> 
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

  // enable pin change interrupt for input pins
  cli();
  PCICR |= _BV(PCIE2);
  PCMSK2 |= _BV(PCINT18) | _BV(PCINT20) | _BV(PCINT21);
  sei();

  show_state();
}


ISR(PCINT2_vect) {
  // make alarm if hot wire is touched
  digitalWrite(BUZZER, hotwire_touched());
  state_machine();
  show_state();
}
// the loop function runs over and over again forever
void loop() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // den tiefsten Schlaf auswählen PWR_DOWN
  sleep_enable(); // sleep mode einschalten
  sleep_mode(); // in den sleep mode gehen
  // debug code
  //led(hotwire_touched(), activate_A_touched(), activate_B_touched());
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

