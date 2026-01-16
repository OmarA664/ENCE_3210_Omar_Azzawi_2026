const int redLED   = 5;
const int greenLED = 4;
//const int blueLED  = 6;      
const int button1  = 2;      
const int button2  = 3;      


#define TIME 500  // ms per step

volatile bool toggleRequest = false;  
bool running = false;

unsigned long lastStepTime = 0;
int stepIndex = 0;


void allOff() {
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  //digitalWrite(blueLED, LOW);   
}


void isr_button() {
  toggleRequest = true;
}

void setup() {
  Serial.begin(9600);

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  //pinMode(blueLED, OUTPUT);       

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(button1), isr_button, FALLING);

  allOff();
}

void loop() {
  // ----- button toggle -----
  if (toggleRequest) {
    toggleRequest = false;

    delay(25); // debounce
    if (digitalRead(button1) == LOW) {      
      running = !running;

      if (!running) {
        allOff();           // stop 
        stepIndex = 0;      // reset cases
      } else {
        lastStepTime = millis(); // start timing
      }
    }
  }

  //------ do nothing ---------
  if (!running) return;

  // ----- timing -------
  unsigned long now = millis();
  if (now - lastStepTime < TIME) return;
  lastStepTime = now;

  // ------- light cases -----
  switch (stepIndex) {
    case 0: // R
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      //digitalWrite(blueLED, LOW);
      break;

    case 1: // RG
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, HIGH);
      //digitalWrite(blueLED, LOW);
      break;

    case 2: // RGB
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, HIGH);
      //digitalWrite(blueLED, HIGH);
      break;

    case 3: // GB
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, HIGH);
      //digitalWrite(blueLED, HIGH);
      break;

    case 4: // B
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, LOW);
      //digitalWrite(blueLED, HIGH);
      break;

    case 5: // RB
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      //digitalWrite(blueLED, HIGH);
      break;
  }

  stepIndex = (stepIndex + 1) % 6; 
}
