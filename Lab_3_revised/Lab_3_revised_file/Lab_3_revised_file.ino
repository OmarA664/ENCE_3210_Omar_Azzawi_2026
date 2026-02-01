#include <Arduino.h>

// ---------------- Shared Variables ----------------

// Incremented once per second inside Timer1 ISR
volatile uint32_t gTimer = 0;

// Flags set by interrupts, handled in loop()
volatile bool task1Flag = false;
volatile bool task2Flag = false;

// ---------------- Pin Definitions ----------------

const int gLED_g  = 4;   // Green LED
const int gLED_r  = 5;   // Red LED
const int gButton = 2;   // Button (INT0)

// ---------------- Task Definitions ----------------

// Task 1: runs at 1 Hz
void task1() {
  digitalWrite(gLED_g, !digitalRead(gLED_g));
  gTimer++;
}

// Task 2: runs on button interrupt
void task2() {
  bool pressed = (digitalRead(gButton) == LOW);
  digitalWrite(gLED_r, pressed ? HIGH : LOW);
}

// ---------------- Timer1 Interrupt (1 Hz) ----------------
// 16 MHz / 1024 prescaler = 15625 counts/sec
ISR(TIMER1_COMPA_vect) {
  task1Flag = true;
}

// ---------------- Button Interrupt ----------------
void buttonISR() {
  task2Flag = true;
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(9600);

  pinMode(gLED_g, OUTPUT);
  pinMode(gLED_r, OUTPUT);
  pinMode(gButton, INPUT_PULLUP);

  digitalWrite(gLED_g, LOW);
  digitalWrite(gLED_r, LOW);

  // ---------- Timer1 Configuration ----------
  cli();                // Disable global interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 15624;        // Compare match for 1 Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 interrupt
  sei();                // Enable global interrupts

  // ---------- External Interrupt (Button) ----------
  attachInterrupt(digitalPinToInterrupt(gButton), buttonISR, CHANGE);
}

// ---------------- Main Loop ----------------
void loop() {

  // ---------- Handle Task 1 ----------
  if (task1Flag) {
    task1Flag = false;
    task1();
  }

  // ---------- Handle Task 2 ----------
  if (task2Flag) {
    task2Flag = false;
    task2();
  }

  // ---------- Foreground Task ----------
  if (gTimer >= 3) {
    Serial.println("3 seconds elapsed");
    gTimer = 0;
  }
}
