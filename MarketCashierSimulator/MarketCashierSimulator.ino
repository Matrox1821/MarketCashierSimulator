#include <Keypad.h>

const byte ROWS=4;
const byte COLS=4;

char keys[ROWS][COLS]={
  {'7','8','9','A'},
  {'4','5','6','B'},
  {'1','2','3','C'},
  {'E','0','=','D'} 
};
//  A:Add (Add to end of list)  --  B:Apply discount (Apply the discount to the total purchase.)  --  C:Cancel purchase (clears the product list)  --  D:Delete (Remove last product) --
//  " = ":Show total and request payment (Calculates total, waits for input, gives change) -- E:Exit (End or Reboot program)  --

const byte PRODUCTS=2;
const byte DATA=3;

string keys[ROWS][COLS]={
  {'001','CocaCola','20'},
  {'002','Papas Fritas','25'},
  {'003','Mostaza','10'},
};
//After, change the data

string inserted_code;

byte row_pins[ROWS]={12,11,8,7};
byte col_pins[COLS]={6,5,4,2};

Keypad cust_keypad=Keypad(makeKeymap(keys), row_pins, col_pins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char cust_key;

  /*  
    string soyString="hola";
    int soyInt=soyString.toInt() 
  */

  do{
    cust_key=cust_keypad.getKey();

    if(cust_key){

      inserted_code+=cust_key;

      if(cust_key=='A'){
        AddProduct();
      }
      if(cust_key=='B'){
        ApplyDiscount();
      }
      if(cust_key=='C'){
        CancelPurchase();
      }
      if(cust_key=='D'){
        DeleteLastProduct();
      }
      if(cust_key=='='){
        ShowTotalAndProcessThePayment();
      }

    }

  }while(cust_key!='E');
}

void AddProduct(){
  // codigo para ingersar un codigo de producto a la lista
}
void ApplyDiscount(){
  // codigo para aplicar descuento al total
}
void CancelPurchase(){
  // codigo para cancelar la compra y limpiar la lista (seguir trabajando)
}
void DeleteLastProduct(){
  // codigo para borrar ultimo producto
}
void ShowTotalAndProcessThePayment(){
  // codigo para ver el total, realizar el pago y mostrar el vuelto
}