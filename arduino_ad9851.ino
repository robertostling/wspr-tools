/* AD9851 module WSPR transmitter
 * Robert Östling SM0YSR 2017-08-29
 *
 * Use the encode.py script to generate the data to transmit, then paste it
 * into msg = { ... } below.
 *
 * You may also want to tweak the frequency:
 */

// Set TX frequency
#define F_BASE 3594086.0
//#define F_BASE 10140186.0

byte msg[162] = { /* output of encode.py */ };


byte pin_button = 2;    // button that pulls LOW to start transmitting
                        // internal pull-up is enabled, so just short to GND
                        // This is just for timing, after the button is
                        // pressed the system will go into an infinite loop
                        // transmitting every 10 minutes.

byte pin_key = 3;       // "keyer" output, HIGH = enable TX
byte pin_reset = 4;     // DDS reset pin
byte pin_load = 5;      // DDS load pin
byte pin_clock = 6;     // DDS clock pin
byte pin_data = 7;      // DDS data pin
byte pin_led = 13;      // Arduino LED

/*
Left side of module (with LED facing up, at upper right corner):

VCC
W_CLK (clock)
FU_UD (load)
DATA (data)
RESET (reset)
GND
QOUT1
QOUT2
ZOUT1
ZOUT2
*/

// Encoded frequency table for the 4 FSK tones
// This is pre-computed so that it can be sent directly to the DDS
unsigned long fsk[4] = {
  (unsigned long)((F_BASE * 4294967296.0) / 180e6),
  (unsigned long)(((F_BASE + 1.0*1.4648) * 4294967296.0) / 180e6),
  (unsigned long)(((F_BASE + 2.0*1.4648) * 4294967296.0) / 180e6),
  (unsigned long)(((F_BASE + 3.0*1.4648) * 4294967296.0) / 180e6)
};

void write_bit(byte x) {
  if(x) {
    digitalWrite(pin_clock, LOW);
    digitalWrite(pin_data, HIGH);
    digitalWrite(pin_clock, HIGH);
    digitalWrite(pin_data, LOW);
  } else {
    digitalWrite(pin_clock, LOW);
    digitalWrite(pin_data, LOW);
    digitalWrite(pin_clock, HIGH);
  }
}

void write_byte(byte x) {
  for(byte i=0; i<8; i++) {
    write_bit(x&1);
    x >>= 1;
  }
}

void set_frequency_word(unsigned long w) {
  digitalWrite(pin_load, LOW);
  for(byte i=0; i<32; i++) {
    write_bit(w&1);
    w >>= 1;
  }
  write_byte(0x09);
  digitalWrite(pin_load, HIGH);
}

void setup() {
  pinMode(pin_reset, OUTPUT);
  pinMode(pin_load, OUTPUT);
  pinMode(pin_clock, OUTPUT);
  pinMode(pin_data, OUTPUT);
  pinMode(pin_led, OUTPUT);
  pinMode(pin_key, OUTPUT);
  pinMode(pin_button, INPUT_PULLUP);

  digitalWrite(pin_reset, LOW);
  digitalWrite(pin_clock, LOW);
  digitalWrite(pin_data, LOW);
  digitalWrite(pin_load, LOW);
  digitalWrite(pin_led, LOW);
  digitalWrite(pin_key, LOW);

  delay(2000);

  digitalWrite(pin_reset, HIGH);
  delay(5);
  digitalWrite(pin_reset, LOW);
  delay(5);

  digitalWrite(pin_clock, HIGH);
  delay(5);
  digitalWrite(pin_clock, LOW);
  delay(5);

  digitalWrite(pin_load, HIGH);
  delay(5);
  digitalWrite(pin_load, LOW);
  delay(5);

  // Arbitrary
  set_frequency_word(fsk[0]);
}

// Infinite transmit loop, one message every 10 minutes
void tx() {
  int i;
  unsigned long t0 = millis();

  while(1) {
    digitalWrite(pin_key, HIGH);
    for (i=0; i<162; i++) {
      set_frequency_word(fsk[msg[i]]);
      double target =
        t0 + (unsigned long)(((double)(i+1))*(1000.0*8192.0/12000.0));
      digitalWrite(pin_led, (i&1));
      while(millis() < target);
    }
    digitalWrite(pin_key, LOW);
    digitalWrite(pin_led, LOW);
    t0 += 600000UL;
    while(millis() < t0);
  }
}

void loop() {
  // Wait for button press, then start infinite loop
  if (digitalRead(pin_button) == LOW) {
    tx();
    // this will never be reached
  }
}

