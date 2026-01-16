#define LED_RED   5
#define LED_GREEN 4
//#define LED_BLUE  6

char letters[] = {'R', 'G', 'B', 'R', 'G', 'R', 'B', 'B', 'R', 'G', 'R', 'B', 'G', 'R'
};
int size = sizeof(letters) / sizeof(letters[0]);

int countR = 0;
int countG = 0;
int countB = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  //pinMode(LED_BLUE, OUTPUT);

  
  for (int i = 0; i < size; i++) {

    // Turn all lights OFF before checking the next letter
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    //digitalWrite(LED_BLUE, LOW);

    // Check current letter
    if (letters[i] == 'R') {
      digitalWrite(LED_RED, HIGH);
      countR++;
      Serial.println("R detected");
    }
    else if (letters[i] == 'G') {
      digitalWrite(LED_GREEN, HIGH);
      countG++;
      Serial.println("G detected");
    }
    // else if (letters[i] == 'B') {
    //   digitalWrite(LED_BLUE, HIGH);
    //   countB++;
    //   Serial.println("B detected");
    // }

    delay(500);  // delay to show each light
  }

  
  Serial.print("R count: ");
  Serial.println(countR);

  Serial.print("G count: ");
  Serial.println(countG);

  Serial.print("B count: ");
  Serial.println(countB);
}

void loop() {
  // nothing here
}
