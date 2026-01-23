//used chat GPT to learn how to use millis and had it comment heavily

// ---------------- PIN DEFINITIONS ----------------
// Pin for RED LED
#define red      5

// Pin for GREEN LED
#define green    4

// Button 1 input pin
#define button_1 2

// Button 2 input pin
#define button_2 3


// ---------------- GLOBAL VARIABLES ----------------

// These store how many times each button has been pressed
// Marked volatile because they are modified inside an ISR
volatile int button_1_presses = 0;
volatile int button_2_presses = 0;

// These store the last time (in milliseconds) each button was accepted
// Used for debouncing inside the ISR
volatile unsigned long last_b1_time = 0;
volatile unsigned long last_b2_time = 0;

// Minimum time between valid button presses (debounce time)
const unsigned long debounceDelay = 50; // milliseconds


void setup() {
  // Start serial communication so we can print values
  Serial.begin(9600);

  // Set LED pins as outputs
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

  // Set button pins as inputs with internal pull-up resistors
  // Button reads HIGH normally, LOW when pressed
  pinMode(button_1, INPUT_PULLUP);
  pinMode(button_2, INPUT_PULLUP);

  // Attach interrupts to both buttons
  // FALLING means HIGH → LOW (button press)
  attachInterrupt(digitalPinToInterrupt(button_1), isr_button, FALLING);
  attachInterrupt(digitalPinToInterrupt(button_2), isr_button, FALLING);
}

void loop() {
  // ---------------- LED LOGIC ----------------
  // Compare button press counts and turn LEDs on accordingly

  // If button 1 has been pressed more times than button 2
  if (button_1_presses > button_2_presses) {
    digitalWrite(red, HIGH);     // Turn RED LED ON
    digitalWrite(green, LOW);    // Turn GREEN LED OFF
  }
  // If button 2 has been pressed more times than button 1
  else if (button_2_presses > button_1_presses) {
    digitalWrite(green, HIGH);   // Turn GREEN LED ON
    digitalWrite(red, LOW);      // Turn RED LED OFF
  }
  // If both buttons have been pressed the same number of times
  else {
    digitalWrite(red, LOW);      // Turn both LEDs OFF
    digitalWrite(green, LOW);
  }

  // ---------------- SERIAL OUTPUT ----------------
  // Print button press counts to Serial Monitor

  Serial.print("Button 1 push count: ");
  Serial.println(button_1_presses);

  Serial.print("Button 2 push count: ");
  Serial.println(button_2_presses);

  // Slow down serial output so it is readable
  delay(500);
}


// ---------------- INTERRUPT SERVICE ROUTINE ----------------
// This function runs automatically when EITHER button is pressed
// Keep it short and fast

void isr_button() {

  // Get the current time in milliseconds
  unsigned long now = millis();

  // ---------------- BUTTON 1 DEBOUNCE ----------------
  // Check if button 1 is currently pressed (LOW with INPUT_PULLUP)
  if (digitalRead(button_1) == LOW) {

    // Only count the press if enough time has passed
    // This filters out button bounce
    if (now - last_b1_time > debounceDelay) {
      button_1_presses++;        // Increment button 1 press count
      last_b1_time = now;        // Save time of this valid press
    }
  }

  // ---------------- BUTTON 2 DEBOUNCE ----------------
  // Check if button 2 is currently pressed
  if (digitalRead(button_2) == LOW) {

    // Apply debounce timing
    if (now - last_b2_time > debounceDelay) {
      button_2_presses++;        // Increment button 2 press count
      last_b2_time = now;        // Save time of this valid press
    }
  }
}
