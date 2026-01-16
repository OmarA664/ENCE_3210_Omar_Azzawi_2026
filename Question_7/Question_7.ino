const int LED_R = 5;
const int LED_G = 4;
const int LED_B = 6;

const int BTN_OP   = 2;  // button 1: choose operation
const int BTN_EXEC = 3;  // button 2: execute


int A[10] = { 1,2,3,4,5,6,7,8,9,10 };
int B[10] = { 10,9,8,7,6,5,4,3,2,1 };
int C[10]; // results


int opCount = 0;                 // 0=none yet, 1=add, 2=sub, 3=mul
bool lastOpBtnState   = HIGH;    // for edge detection
bool lastExecBtnState = HIGH;


void setLED(bool r, bool g, bool b) {
  digitalWrite(LED_R, r);
  digitalWrite(LED_G, g);
  digitalWrite(LED_B, b);
}

void allOff() { setLED(LOW, LOW, LOW); }
void allOn()  { setLED(HIGH, HIGH, HIGH); }

void showSelectedOperation() {
  
  if (opCount == 1)      setLED(HIGH, LOW,  LOW);  // add = red
  else if (opCount == 2) setLED(LOW,  HIGH, LOW);  // sub = green
  else if (opCount == 3) setLED(LOW,  LOW,  HIGH); // mul = blue
  else                   allOff();                 // none selected
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  pinMode(BTN_OP, INPUT_PULLUP);
  pinMode(BTN_EXEC, INPUT_PULLUP);

  allOff();
}

void loop() {
  
  bool opBtnNow = digitalRead(BTN_OP);

  
  if (lastOpBtnState == HIGH && opBtnNow == LOW) {
    opCount++;                 // increment counter once per press
    if (opCount > 3) opCount = 1; // wrap 1..3

    showSelectedOperation();   // show LED for selected op
    delay(25);                 // debounce
  }
  lastOpBtnState = opBtnNow;

  
  bool execBtnNow = digitalRead(BTN_EXEC);

  if (lastExecBtnState == HIGH && execBtnNow == LOW) {
    
    allOff();

    // Apply selected operation to ALL elements
    for (int i = 0; i < 10; i++) {
      if (opCount == 1) {
        C[i] = A[i] + B[i];
      } else if (opCount == 2) {
        C[i] = A[i] - B[i];
      } else if (opCount == 3) {
        C[i] = A[i] * B[i];
      } else {
        C[i] = 0; // if no op selected yet
      }
    }

    
    Serial.println("Results (C array):");
    for (int i = 0; i < 10; i++) {
      Serial.print("C[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.println(C[i]);
    }

    
    allOn();

    delay(25); // debounce
  }
  lastExecBtnState = execBtnNow;
}
