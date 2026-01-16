#define LED_GREEN 4
#define LED_RED   5

// Data array 
int data[] = {4, 5, 6, 7, 4, 3, 5, 6, 9, 1, 2, 3, 4, 8, 7};
int dataLen = sizeof(data) / sizeof(data[0]);

// Pattern array 
int pattern[3] = {1, 2, 3};

void setup() {
  Serial.begin(9600);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);

  int count = 0;

  // Slide over data
  for (int i = 0; i <= dataLen - 3; i++) {

    // Check 3 entries in a row
    if (data[i]     == pattern[0] &&
        data[i + 1] == pattern[1] &&
        data[i + 2] == pattern[2]) {

      count++;                         
      digitalWrite(LED_GREEN, HIGH);   // turn green ON when detected
      delay(500);                      
      
    }
  }

  // End of program behavior:
  if (count == 0) {
    digitalWrite(LED_RED, HIGH);       // pattern NOT found at all
  } else {
    digitalWrite(LED_GREEN, HIGH);     // pattern found // leave green on
  }

  Serial.print("Pattern occurrences: ");
  Serial.println(count);
}

void loop() {
  
}
