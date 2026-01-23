volatile int gArrayX[10];
volatile int gArrayY[10];
volatile bool filterDone = false;
const int redLED         = 5;
const int greenLED       = 4;
//const int blueLED      = 6;      
const int button1        = 2;      
const int button2        = 3;    
volatile unsigned long lastInterruptTime = 0;
const unsigned long debounceDelay = 200; // ms


void setup() {

//create array
  Serial.begin(9600);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(button1), isr_button, FALLING);

  for(int i=0; i<10;  i++) {
    gArrayX[i] = i;
    Serial.println(gArrayX[i]);

    Serial.println("Initial X:");
  for (int i = 0; i < 10; i++) {
    Serial.println(gArrayX[i]);
    }
  }
}

void isr_button() {
  unsigned long now = millis();

  // Debounce 
  if (now - lastInterruptTime < debounceDelay) {
    return;   
  }
  lastInterruptTime = now;

  // Compute y[n] = 2*x[n] - x[n-1]
  // Handle n=0 
  gArrayY[0] = 2 * gArrayX[0] - gArrayX[0];  // = x[0]

  for (int n = 1; n < 10; n++) {
    gArrayY[n] = 2 * gArrayX[n] - gArrayX[n - 1];
  }

  filterDone = true; // tell loop to print results
}


void loop() {
  if (filterDone) {
    noInterrupts();          // make sure it doesn't stop here
    filterDone = false;
    interrupts();

    Serial.println("Filtered Y:");
    for (int i = 0; i < 10; i++) {
      Serial.println(gArrayY[i]);
    }
  }
}

