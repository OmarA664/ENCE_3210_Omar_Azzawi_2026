int i = 0;
int array[511];

void setup() {
  
Serial.begin(9600);
for (i=0; i < 512; i++){
array[i] = 2*(i*i);
Serial.println(array[i]);
  }
}


void loop() {
  

}
