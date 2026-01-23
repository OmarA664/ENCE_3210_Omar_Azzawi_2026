//used chat GPT to comment heavily 

// ======================= PIN DEFINITIONS =======================

// Red LED output pin
#define LED_R 5

// Green LED output pin
#define LED_G 4

// Blue LED output pin
#define LED_B 6

// Button 1: selects the arithmetic operation
#define BTN_OP   2

// Button 2: executes the selected operation
#define BTN_EXEC 3


// ======================= DATA ARRAYS =======================

// First input array (10 integers)
int A[10] = { 1,2,3,4,5,6,7,8,9,10 };

// Second input array (10 integers)
int B[10] = { 10,9,8,7,6,5,4,3,2,1 };

// Result array (empty at start)
int C[10];


// ======================= INTERRUPT FLAGS =======================
// These variables are modified inside ISRs, so they MUST be volatile

// Set to true when the operation-select button is pressed
volatile bool opPressed   = false;

// Set to true when the execute button is pressed
volatile bool execPressed = false;


// ======================= PROGRAM STATE =======================

// Tracks which operation is selected
// 1 = addition, 2 = subtraction, 3 = multiplication
int opCount = 0;


// ======================= LED HELPER FUNCTIONS =======================

// Turn LEDs on/off using a single function
void setLED(bool r, bool g, bool b) {
  digitalWrite(LED_R, r);   // control red LED
  digitalWrite(LED_G, g);   // control green LED
  digitalWrite(LED_B, b);   // control blue LED
}

// Turn all LEDs OFF
void allOff() {
  setLED(LOW, LOW, LOW);
}

// Turn all LEDs ON
void allOn() {
  setLED(HIGH, HIGH, HIGH);
}

// Show which operation is currently selected using LEDs
void showSelectedOperation() {

  // Addition selected → RED LED ON
  if (opCount == 1)
    setLED(HIGH, LOW, LOW);

  // Subtraction selected → GREEN LED ON
  else if (opCount == 2)
    setLED(LOW, HIGH, LOW);

  // Multiplication selected → BLUE LED ON
  else if (opCount == 3)
    setLED(LOW, LOW, HIGH);

  // No operation selected → all LEDs OFF
  else
    allOff();
}


// ======================= SETUP =======================

void setup() {
  // Start serial communication for debugging / output
  Serial.begin(9600);

  // Configure LED pins as outputs
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  // Configure button pins with internal pull-up resistors
  // Buttons read HIGH when idle, LOW when pressed
  pinMode(BTN_OP, INPUT_PULLUP);
  pinMode(BTN_EXEC, INPUT_PULLUP);

  // Attach interrupts:
  // When BTN_OP is pressed, call isr_op()
  // When BTN_EXEC is pressed, call isr_exec()
  attachInterrupt(digitalPinToInterrupt(BTN_OP),   isr_op,   FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_EXEC), isr_exec, FALLING);

  // Start with all LEDs OFF
  allOff();
}


// ======================= MAIN LOOP =======================

void loop() {

  // ---------- HANDLE OPERATION-SELECT BUTTON ----------
  // If ISR set the opPressed flag
  if (opPressed) {

    // Clear the flag so we only handle this press once
    opPressed = false;

    // Increment operation counter
    opCount++;

    // Wrap around after 3
    if (opCount > 3)
      opCount = 1;

    // Display selected operation using LEDs
    showSelectedOperation();
  }

  // ---------- HANDLE EXECUTE BUTTON ----------
  // If ISR set the execPressed flag
  if (execPressed) {

    // Clear the flag
    execPressed = false;

    // Spec: turn all LEDs OFF while calculating
    allOff();

    // Apply selected operation to all array elements
    for (int i = 0; i < 10; i++) {

      // Addition
      if (opCount == 1)
        C[i] = A[i] + B[i];

      // Subtraction
      else if (opCount == 2)
        C[i] = A[i] - B[i];

      // Multiplication
      else if (opCount == 3)
        C[i] = A[i] * B[i];

      // No operation selected
      else
        C[i] = 0;
    }

    // Print results to Serial Monitor
    Serial.println("Results:");
    for (int i = 0; i < 10; i++) {
      Serial.print("C[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.println(C[i]);
    }

    // Spec: when all results are saved, turn ALL LEDs ON
    allOn();
  }
}


// ======================= INTERRUPT SERVICE ROUTINES =======================
// These run immediately when buttons are pressed
// Keep them VERY short and fast

// ISR for operation-select button
void isr_op() {
  opPressed = true;   // just set a flag
}

// ISR for execute button
void isr_exec() {
  execPressed = true; // just set a flag
}
