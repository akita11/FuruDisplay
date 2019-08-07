#include <EEPROM.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

// ATtiny861
// TILT : PB0 = D9 / PCINT8
// SW   : PB1 = D8 / PCINT9
// LED0 : PB6 = A9
// LED1 : PB5 = A8
// LED2 : PB4 = A7
// LED3 : PA5 = A4
// LED4 : PA4 = A3
// LED5 : PA0 = A0
// LED6 : PA1 = A1
// LED7 : PA2 = A2
// TXD  : PA6 = AIN0
// RXD  : PA7 = AIN1
// unused:
// PB2 (D7)
// PB3 (D6)
#define N_PAT 32 // number of pattern columns
#define N_AVG 4
#define N_AVG_TH 1
#define N_PATTERNS 8

#define SCAN_DELAY 10

uint8_t LEDpin[] = {A9, A8, A7, A4, A3, A0, A1, A2};
#define pinTILT 9
#define pinSW   8

uint16_t value[8]; // intensity
uint16_t threshold[8]; // intensity threshold
uint16_t thresholdW[8]; // intensity threshold (work)
uint8_t pat[N_PAT]; // display patterns
uint8_t ip = 0; // index of displaying pattern

void init_pattern()
{
  uint8_t i = 0;
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x10);
  EEPROM.write(i++, 0x10);
  EEPROM.write(i++, 0x10);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x80);
  EEPROM.write(i++, 0x80);
  EEPROM.write(i++, 0x80);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x80);
  EEPROM.write(i++, 0x80);
  EEPROM.write(i++, 0x80);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x82);
  EEPROM.write(i++, 0x82);
  EEPROM.write(i++, 0x82);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xBF);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x12);
  EEPROM.write(i++, 0x12);
  EEPROM.write(i++, 0x12);
  EEPROM.write(i++, 0x02);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x04);
  EEPROM.write(i++, 0x08);
  EEPROM.write(i++, 0x10);
  EEPROM.write(i++, 0x20);
  EEPROM.write(i++, 0x40);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xBF);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x3C);
  EEPROM.write(i++, 0x42);
  EEPROM.write(i++, 0x95);
  EEPROM.write(i++, 0xA1);
  EEPROM.write(i++, 0xA1);
  EEPROM.write(i++, 0x95);
  EEPROM.write(i++, 0x42);
  EEPROM.write(i++, 0x3C);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x3C);
  EEPROM.write(i++, 0x42);
  EEPROM.write(i++, 0x95);
  EEPROM.write(i++, 0xA1);
  EEPROM.write(i++, 0xA1);
  EEPROM.write(i++, 0x95);
  EEPROM.write(i++, 0x42);
  EEPROM.write(i++, 0x3C);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x3C);
  EEPROM.write(i++, 0x42);
  EEPROM.write(i++, 0x95);
  EEPROM.write(i++, 0xA1);
  EEPROM.write(i++, 0xA1);
  EEPROM.write(i++, 0x95);
  EEPROM.write(i++, 0x42);
  EEPROM.write(i++, 0x3C);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x80);
  EEPROM.write(i++, 0x80);
  EEPROM.write(i++, 0x80);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x82);
  EEPROM.write(i++, 0x82);
  EEPROM.write(i++, 0x82);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x06);
  EEPROM.write(i++, 0x18);
  EEPROM.write(i++, 0x60);
  EEPROM.write(i++, 0x80);
  EEPROM.write(i++, 0x60);
  EEPROM.write(i++, 0x18);
  EEPROM.write(i++, 0x06);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x9E);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0xF2);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x9E);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0xF2);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x9E);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0xF2);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x9E);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0xF2);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0xF2);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0xF2);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0xF2);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0xF2);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x1E);
  EEPROM.write(i++, 0x02);
  EEPROM.write(i++, 0x02);
  EEPROM.write(i++, 0x02);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x1E);
  EEPROM.write(i++, 0x02);
  EEPROM.write(i++, 0x02);
  EEPROM.write(i++, 0x02);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x1E);
  EEPROM.write(i++, 0x02);
  EEPROM.write(i++, 0x20);
  EEPROM.write(i++, 0x02);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x1E);
  EEPROM.write(i++, 0x02);
  EEPROM.write(i++, 0x02);
  EEPROM.write(i++, 0x02);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0x00);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0x92);
  EEPROM.write(i++, 0xFE);
  EEPROM.write(i++, 0x00);
}
// EEPROM map (0x200 = 512 bytes) / 32 * 8 = 256
//   Pattern 0-5 : pre-defined
//   Pattern 6-7 : scanned
// address 0x000 - 0x01f : Pattern0
// address 0x020 - 0x03f : Pattern1
// ...
// address 0x0e0 - 0x0ff : Pattern7

#define check_tilt() (digitalRead(pinTILT)==0?1:0) // ON=1, OFF=0

#define T_LONGPRESS 200

uint8_t check_SW() // 0=no press, 1=short press, 2=long press
{
  uint8_t t = 0;
  if (digitalRead(pinSW) == 1) return (0);
  while (digitalRead(pinSW) == 0 && t++ < T_LONGPRESS) delay(10);
  if (t < T_LONGPRESS) return (1); // short press
  else return (2); // long press
}

uint8_t check_SW_raw()
{
  if (digitalRead(pinSW) == 1) return (0);
  else return (1);
}

// Functions
// void SetMode(int pos, int mode)
// void SetLED(uint8_t p)
// uint16_t get_value()
// uint8_t scan()

// set LED pin mode at "pos", (all if "pos"=-1)
void SetMode(int pos, int mode)
{
  uint8_t i;
  if (mode == INPUT) {
    // disable pullup for input pin
    if (pos == -1) for (i = 0; i < 8; i++) digitalWrite(LEDpin[i], 0);
    else digitalWrite(LEDpin[pos], 0);
  }
  if (pos == -1) for (i = 0; i < 8; i++) pinMode(LEDpin[i], mode);
  else pinMode(LEDpin[pos], mode);
}

// set LED pattern based on "p"
void SetLED(uint8_t p)
{
  uint8_t i, b = 1;
  for (i = 0; i < 8; i++) {
    if ((p & b) != 0) digitalWrite(LEDpin[i], 1);
    else digitalWrite(LEDpin[i], 0);
    b = b << 1;
  }
}

uint16_t get_value()
{
  // Time method; measure time to reach V_TH, large value for dark
  uint16_t s;
#define V_TH 30
#define T_MAX 30
  uint16_t t = 0;
  uint8_t f = 0, b, j, i;
  uint16_t v;
  uint8_t p;
  for (j = 0; j < 8; j++) {
    p = LEDpin[j];
    SetMode(-1, OUTPUT); SetLED(0x00);
    SetMode(-1, INPUT);
    t = 0; v = 0;
    while (v < V_TH && t < T_MAX) {
      t++;
      s = 0;
      for (i = 0; i < N_AVG; i++) {
        s += analogRead(p);
        delay(SCAN_DELAY);
      }
      v = s / N_AVG;
    }
    value[j] = t;
  }
}

void get_threshold()
{
  uint16_t s[8];
  uint8_t i, j;
  for (i = 0; i < 8; i++) s[i] = 0;
  for (j = 0; j < N_AVG_TH; j++) {
    get_value();
    for (i = 0; i < 8; i++) s[i] += value[i];
  }
  for (i = 0; i < 8; i++) threshold[i] = s[i] / N_AVG_TH;
}

void ShowPatternIndex(uint8_t n)
{
  SetMode(-1, OUTPUT);
  SetLED(0x00);
  uint8_t v = 1, i;
  while (n-- > 0) v = v << 1;
  for (i = 0; i < 5; i++) {
    SetLED(v); delay(50);
    SetLED(0); delay(50);
  }

}

void FlashLED(uint8_t p1, uint8_t p2, uint8_t t1, uint8_t t2, uint8_t n)
{
  SetMode(-1, OUTPUT);
  while (n-- > 0) {
    SetLED(p1); delay(t1);
    SetLED(p2); delay(t2);
  }
  SetLED(0x00);
}

void setup() {
  int i;
  uint8_t sw;
  pinMode(pinTILT, INPUT); digitalWrite(pinTILT, 1); // TILT pin, enable pullup
  pinMode(pinSW, INPUT); digitalWrite(pinSW, 1); // SW pin, enable pullup
  Serial.begin(9600); // for debug
  sw = 1; i = 0;
  while(i < 256 && sw == 1){
    byte d = EEPROM.read(i++);
    if (EEPROM.read(i++) != 0xff) sw = 0;
//    Serial.print(sw); Serial.print(' ');
//    Serial.println(i);
  }
  if (sw == 1){
    Serial.print("initial pattern");
    init_pattern();    
  }
  SetMode(-1, OUTPUT);
  // unused pins
  pinMode(6, OUTPUT); digitalWrite(6, LOW);
  pinMode(7, OUTPUT); digitalWrite(7, LOW);

  GIMSK |= 0x10; // PCIE0=1, enable PCIE0
  PCMSK1 = 0x03; // enable PCINT8 & PCINT9
  sei();

  Serial.println("start");

  i = 0;
  while (digitalRead(pinSW) == 0 && i++ < 500) delay(10);
  if (i > 495) {
    i = 0;
    while (digitalRead(pinSW) == 0) {
      if (i == 0) SetLED(0x55); else SetLED(0xaa);
      delay(500);
      i = 1 - i;
    }
    SetLED(0x00);
    while ((sw = check_SW()) == 0);
    if (sw == 2) {
      Serial.println("intitialize pattern");
      i = 0;
      init_pattern();
    }
  }

  // dump EEPROM content for debug
  for (i = 0; i < 32 * 8; i++) {
    if ((i % 16) == 0) {
      Serial.print(i, HEX);
      Serial.print(':');
    }
    Serial.print(EEPROM.read(i), HEX); Serial.print(' ');
    if ((i % 16) == 15)Serial.println(' ');
  }
  // load initial pattern
  for (i = 0; i < N_PAT; i++) pat[i] = EEPROM.read(i);
}

ISR (PCINT_vect) { }

// mode = 0 : no scanned pattern exists
// mode = 1 : scanned pattern exists
uint8_t mode = 0;

void ShowValue(uint8_t n)
{
  uint8_t i;
  get_value();
  for (i = 0; i < n; i++) {
    Serial.print(value[i]);
    Serial.print(' ');
  }
  Serial.println();
}

uint8_t f_triggered = 0;
uint32_t t_wake = 0;

void loop() {
  uint8_t i, sw;
  uint8_t addr_pat;

  sw = check_SW();
  if (sw == 1) {
    // short press -> pattern change
    ip = (ip + 1) % N_PATTERNS;
    ShowPatternIndex(ip);
    addr_pat  = ip << 5;
    for (i = 0; i < N_PAT; i++) {
      pat[i] = EEPROM.read(addr_pat++);
    }
  }
  else if (sw == 2) {
    Serial.print("mode="); Serial.println(mode);
    if (mode == 1) {
      // scanned, but non-stored pattern exist -> write
      addr_pat  = ip << 5;
      SetLED(0x00);
      for (i = 0; i < N_PAT; i++) {
        SetLED(0x55); delay(10);
        SetLED(0xaa); delay(20);
        EEPROM.write(addr_pat++, pat[i]);
      }
      SetLED(0x00);
      while (digitalRead(pinSW) == 0) delay(10); // wait for SW released
      mode = 0;
    }
    else {
      FlashLED(0xff, 0x00, 150, 150, 5);
      while ((sw = check_SW()) == 0) delay(10);
      if (sw == 1) {
        Serial.println("Scan White #1...");
        get_threshold();
        for (i = 0; i < 8; i++) {
          Serial.print(threshold[i]);
          Serial.print(' ');
        } Serial.println(' ');
        for (i = 0; i < 8; i++) thresholdW[i] = threshold[i];
        FlashLED(0xff, 0x00, 100, 100, 1); // flash after scan
        Serial.println("done, waiting for release");
        while (digitalRead(pinSW) == 0) delay(10); // wait for SW released
        Serial.println("Waiting for scan black");
        while ((sw = check_SW()) == 0) delay(10);
        if (sw == 1) {
          Serial.println("Scan White #2...");
          get_threshold();
          for (i = 0; i < 8; i++) {
            Serial.print(threshold[i]);
            Serial.print(' ');
          } Serial.println(' ');
          for (i = 0; i < 8; i++) thresholdW[i] += threshold[i];
          FlashLED(0xff, 0x00, 100, 100, 1); // flash after scan
          Serial.println("done, waiting for release");
          while ((sw = check_SW()) == 0) delay(10); // wait for SW released
          if (sw == 1) {
            Serial.println("Scan Black #1...");
            get_threshold();
            for (i = 0; i < 8; i++) {
              Serial.print(threshold[i]);
              Serial.print(' ');
            } Serial.println(' ');
            for (i = 0; i < 8; i++) thresholdW[i] += threshold[i];
            FlashLED(0xff, 0x00, 100, 100, 1); // flash after scan
            Serial.println("done, waiting for release");
            while (digitalRead(pinSW) == 0) delay(10); // wait for SW released
            Serial.println("Waiting for scan black");
            while ((sw = check_SW()) == 0) delay(10);
            if (sw == 1) {
              Serial.println("Scan Black #2...");
              get_threshold();
              for (i = 0; i < 8; i++) {
                Serial.print(threshold[i]);
                Serial.print(' ');
              } Serial.println(' ');
              for (i = 0; i < 8; i++) threshold[i] = (threshold[i] + thresholdW[i]) / 4;
              FlashLED(0xff, 0x00, 100, 100, 3); // flash after scan
              Serial.println("done, waiting for release");
              while (digitalRead(pinSW) == 0) delay(10); // wait for SW released

              i = 0;
              while (i < N_PAT) {
                // scan column at short press, break if long pressed
                Serial.print("Scanning "); Serial.println(i);
                while ((sw = check_SW()) == 0) delay(10);
                if (sw == 1) { // scan one colunmn if short press
                  get_value();
                  uint8_t k, b = 1, p = 0;
                  for (k = 0; k < 8; k++) {
                    Serial.print(value[k]);
                    Serial.print(' ');
                    Serial.print(threshold[k]);
                    Serial.print('|');
                    if (value[k] > threshold[k]) p |= b;
                    b = b << 1;
                  }
                  pat[i++] = p;
                  Serial.println(' ');
                  Serial.println(pat[i - 1], HEX);
                  FlashLED(0xff, 0x00, 100, 100, 2);
                  FlashLED(p, 0x00, 500, 200, 2);
                }
                else if (sw == 2) { // break if long pressed
                  Serial.println("Break");
                  while (i < N_PAT) {
                    pat[i] = 0x00; i++;
                  }
                }
              }
              mode = 1;
            }
          }
        }
      }
      Serial.print("Scan complete");
      FlashLED(0xff, 0x00, 200, 200, 5);
      for (i = 0; i < 10; i++) Serial.println(pat[i], HEX);
    }
  }

#define POV_WAIT 2
  // wait for shake trigger, and display POV patterns
  if (f_triggered == 0 && check_tilt() == 1) { // avoid continous trigger (body tilted)
    delay(40);
    for (i = 0; i < N_PAT; i++) {
      SetLED(pat[i]); delay(POV_WAIT);
    }
    SetLED(0);
    t_wake = 0;
    f_triggered = 1;
  }
  if (f_triggered == 1 & check_tilt() == 0) f_triggered = 0;

#define TIME_TO_SLEEP 10000 // [x1ms] 10sec
  if (t_wake > TIME_TO_SLEEP) {
    // Go to sleep after no operation of 60sec
    Serial.println("Go to sleep");

    // disable IO pin
    SetMode(-1, OUTPUT); SetLED(0x00);
    ACSRA |= 0x80; // disable comparator
    ADCSRA &= ~0x80; // disbale ADC
    WDTCR &= ~0x08; // disable WDT
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    asm("sleep");

    t_wake = 0;
  }
  t_wake++; delay(1);
}

