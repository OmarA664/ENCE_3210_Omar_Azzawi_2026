#include <Arduino.h> 
// Used Chat gpt to comment every line
// Includes the Arduino core library so we can use Arduino functions
// like pinMode(), digitalWrite(), micros(), Serial, etc.

// ---------------- Shared Variables ----------------

// Volatile because this variable is modified in a timed task
// and read by the foreground (main) task
volatile uint32_t gTimer = 0;

// ---------------- Pin Definitions ----------------

// Green LED pin (used by Task 1)
const int gLED_g  = 4;

// Red LED pin (used by Task 2)
const int gLED_r  = 5;

// Button input pin 
const int gButton = 2;   

// ---------------- Task Frequencies ----------------

// Task 1 runs at 1 Hz (once per second)
const uint32_t gTask1_HZ = 1;   

// Task 2 runs at 10 Hz (ten times per second)
const uint32_t gTask2_HZ = 10;  

// ---------------- Task Periods ----------------

// Convert Task 1 frequency to period in microseconds
// 1 Hz -> 1,000,000 microseconds
const uint32_t gTask1_PERIOD = 1000000UL / gTask1_HZ;

// Convert Task 2 frequency to period in microseconds
// 10 Hz -> 100,000 microseconds
const uint32_t gTask2_PERIOD = 1000000UL / gTask2_HZ;

// ---------------- Scheduler Timing Variables ----------------

// Stores the last execution time of Task 1
uint32_t lastTask1 = 0;

// Stores the last execution time of Task 2
uint32_t lastTask2 = 0;

// ---------------- Task 1 Definition ----------------
void task1() {
  
  // Toggle the green LED state
  // This produces a blinking effect at 1 Hz
  digitalWrite(gLED_g, !digitalRead(gLED_g));

  // Increment the shared timer once per second
  // This value is used by the main (foreground) task
  gTimer++;
}

// ---------------- Task 2 Definition ----------------
void task2() {
  
  // Read the button input
  bool pressed = (digitalRead(gButton) == LOW);

  // Turn the red LED on if button is HIGH, otherwise turn it off
  digitalWrite(gLED_r, pressed ? HIGH : LOW);
}

// ---------------- Setup Function ----------------
void setup() {
  
  // Initialize serial communication for debugging/output
  Serial.begin(9600);

  // Configure LED pins as outputs
  pinMode(gLED_g, OUTPUT);
  pinMode(gLED_r, OUTPUT);

  // Configure button pin as input with internal pull-up resistor
  pinMode(gButton, INPUT_PULLUP);

  // Capture the current time so tasks start cleanly
  // This prevents tasks from "catching up" at startup
  uint32_t now = micros();
  lastTask1 = now;
  lastTask2 = now;

  // Ensure LEDs start in the OFF state
  digitalWrite(gLED_g, LOW);
  digitalWrite(gLED_r, LOW);
}

// ---------------- Main Loop (Foreground Task) ----------------
void loop() {
  
  // Read the current time in microseconds
  uint32_t now = micros();

  // ---------- Task 1 Scheduler (1 Hz) ----------
  // Check if enough time has elapsed to run Task 1
  if ((uint32_t)(now - lastTask1) >= gTask1_PERIOD) {

    // Advance the last execution time by exactly one period
    // This keeps the timing stable and avoids drift
    lastTask1 += gTask1_PERIOD;

    // Execute Task 1
    task1();
  }

  // ---------- Task 2 Scheduler (10 Hz) ----------
  // Check if enough time has elapsed to run Task 2
  if ((uint32_t)(now - lastTask2) >= gTask2_PERIOD) {

    // Advance the last execution time by exactly one period
    lastTask2 += gTask2_PERIOD;

    // Execute Task 2
    task2();
  }

  // ---------- Foreground (Main) Task ----------
  // Use the counter incremented by Task 1 as a timer
  // When 3 seconds have elapsed, print a message
  if (gTimer >= 3) {

    // Print message over the serial port
    Serial.println("3 seconds elapsed");

    // Reset the timer counter
    gTimer = 0;
  }
}
