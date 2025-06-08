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
//  ' = ':Show total and request payment (Calculates total, waits for input, gives change) -- E:Exit (End or Reboot program)  --

const byte PRODUCTS=3;
const byte DATA=3;

String products[PRODUCTS][DATA]={
  {"001","CocaCola","20"},
  {"002","Papas Fritas","25"},
  {"003","Mostaza","10"},
};
//After, change the data

String inserted_products[100];

byte row_pins[ROWS]={12,11,8,7};
byte col_pins[COLS]={6,5,4,2};

Keypad cust_keypad=Keypad(makeKeymap(keys), row_pins, col_pins, ROWS, COLS);

String inserted_product;
char char_code[3];
char cust_key;
byte digits=3,i=0,total_codes=100;
byte productIndex;
int inserted_products_lenght=0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  while(inserted_products_lenght<total_codes){
    Serial.print("Ingrese el codigo de producto [xxx]: ");
    while(i<digits){
      cust_key=cust_keypad.getKey();
      if(cust_key && isNumber(cust_key)){
        char_code[i]=cust_key;
        Serial.print(cust_key);
        i++;
      }
    }
    Serial.println();
    char_code[3] = '\0';
    char newcust_key = NO_KEY;
    while(newcust_key!='E'){
      newcust_key=cust_keypad.getKey();
      if (newcust_key){
        switch(newcust_key){
          case 'A':
            addProduct(char_code,inserted_products_lenght);
            break;
          case 'B':
            applyDiscount();
            break;
          case 'C':
            cancelPurchase();
            break;
          case 'D':
            deleteLastProduct();
            break;
          case '=':
            showTotalAndProcessThePayment();
            break;
          default:
            Serial.print("switch def");
            break;
        }
        if(newcust_key=='A'){
          break;
        }
      }
    }
    inserted_products_lenght++;
    char_code[0] = '\0';
    char_code[1] = '\0';
    char_code[2] = '\0';
  }
}

bool isNumber(char key_pressed){
  bool notIsA = key_pressed!='A',
       notIsB = key_pressed!='B',
       notIsC = key_pressed!='C',
       notIsD = key_pressed!='D',
       notIsE = key_pressed!='E',
       notIsEqual = key_pressed!='=';

  return notIsA && notIsB && notIsC && notIsD && notIsE && notIsEqual;
}

String intToString(int number){
  String new_string="";
  if(number>=9){
    new_string="000" + number;
  }
  if(number>=99){
    new_string="00" + number;
  }
  if(number>=999){
    new_string="0" + number;
  }
  if(number>=9999){
    new_string="0" + number;
  }
  return new_string;
}

void addProduct(String new_code,int index){
    for(int j=0;j<=PRODUCTS;j++){
      if(products[j][0].equals(new_code)){
        inserted_products[index]=String(new_code);
        break;
      }
    }
    Serial.println();
    Serial.print("--------------------------------------------------------");

    Serial.println();
    for(int k=0;k<=inserted_products_lenght;k++){
      for(int l=0;l<=PRODUCTS;l++){
        if(products[l][0].equals(inserted_products[k])){
          Serial.println();
          Serial.print("-- Codigo: "); Serial.print(products[l][0]);Serial.print(" --- Nombre: ");Serial.print(products[l][1]);Serial.print(" --->  $");Serial.print(products[l][2]);
        }
      }
    }
    Serial.println();
    Serial.println();
    Serial.println("--------------------------------------------------------");
    Serial.println();
}
void applyDiscount(){
  // codigo para aplicar descuento al total (al final de la compra)
}
void cancelPurchase(){
  // codigo para cancelar la compra y limpiar la lista (seguir trabajando)
}
void deleteLastProduct(){
  // codigo para borrar ultimo producto de la lista
}
void showTotalAndProcessThePayment(){
  // codigo para ver el total, realizar el pago y mostrar el vuelto
}