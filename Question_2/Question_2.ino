#include <math.h>
float array[361];

void setup() {
  
Serial.begin(9600);
for(int i = 0; i < 361; i++){
  float radians = i * (PI/ 180.0);
 array[i] = cos(radians);
 Serial.println(array[i], 6);
}
}

void loop() {
  // put your main code here, to run repeatedly:

}


