#include <Keypad.h>

const byte ROWS=4;
const byte COLS=4;

char keys[ROWS][COLS]={
  {'7','8','9','A'},
  {'4','5','6','B'},
  {'1','2','3','C'},
  {'on/c','0','=','D'}
};

byte row_pins[ROWS]={12,11,8,7};
byte col_pins[COLS]={6,5,4,2};

Keypad cust_keypad= Keypad(makeKeymap(keys), row_pins, col_pins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char cust_key=cust_keypad.getKey();
  if(cust_key)Serial.println(cust_key);
}
