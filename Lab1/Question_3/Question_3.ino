int array[51] ;

void setup() {

  Serial.begin(9600);
  int max1 = array[0]; //max
  int max2 = array[0]; //second highest

  //create array
  for(int i=0; i<51;  i++) {
    array[i] = i;
    Serial.println(array[i]);
  }

  //find Max
  for (int i = 1; i < 51; i++) {
    if (array[i] > max1) {
    max1 = array[i];
    
    }
  }

  // find max value < max1
  bool found = false;
  for (int i = 0; i < 51; i++) {
    if (array[i] < max1) {
      if (!found || array[i] > max2) {
        max2 = array[i];
        found = true;
      }
    }
  }

  
  Serial.print("Max is: ")
  Serial.println(max1);
  Serial.print("Second highest is: ")
  Serial.print(max2);
}

void loop() {}
