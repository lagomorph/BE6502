/*
 Based on code from Ben Eater with some modifications by John L. Scarfone
 with this license: https://creativecommons.org/licenses/by/4.0/
*/

const char ADDR[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const char DATA[] = {39, 41, 43, 45, 47, 49, 51, 53};
const char RAM_ENABLE[] = {23, 25};  // CS, OE
const char ROM_ENABLE = 27;  // CS
const char IO_ENABLE[] = {29, 31};  // CS1, CS2
const char *ENABLESTR[] = {
  "** RAM ROM IO",
  "** RAM ROM",
  "** RAM IO",
  "RAM",
  "** ROM IO",
  "ROM",
  "IO",
  "** NONE"
};
#define CLOCK 2
#define READ_WRITE 3

void setup() {
  for (int n = 0; n < 16; n += 1) {
    pinMode(ADDR[n], INPUT);
  }
  for (int n = 0; n < 8; n += 1) {
    pinMode(DATA[n], INPUT);
  }
  pinMode(ROM_ENABLE, INPUT);
  for (int n = 0; n < 2; n += 1) {
    pinMode(RAM_ENABLE[n], INPUT);
    pinMode(IO_ENABLE[n], INPUT);
  }
  pinMode(CLOCK, INPUT);
  pinMode(READ_WRITE, INPUT);

  attachInterrupt(digitalPinToInterrupt(CLOCK), onClock, RISING);
  
  Serial.begin(57600);
}

void onClock() {
  char output[15];

  unsigned int address = 0;
  for (int n = 0; n < 16; n += 1) {
    int bit = digitalRead(ADDR[n]) ? 1 : 0;
    Serial.print(bit);
    address = (address << 1) + bit;
  }
  
  Serial.print("   ");
  
  unsigned int data = 0;
  for (int n = 0; n < 8; n += 1) {
    int bit = digitalRead(DATA[n]) ? 1 : 0;
    Serial.print(bit);
    data = (data << 1) + bit;
  }

  unsigned int enable = digitalRead(RAM_ENABLE[0]) | digitalRead(RAM_ENABLE[1]);
  enable = (enable << 1) + digitalRead(ROM_ENABLE);
  enable = (enable << 1) + ((digitalRead(IO_ENABLE[0]) ? 0 : 1) | digitalRead(IO_ENABLE[1]));

  sprintf(output, "   %04x  %c %02x  %s", address, digitalRead(READ_WRITE) ? 'r' : 'W', data, ENABLESTR[enable]);
  Serial.println(output);  
}

void loop() {
}
