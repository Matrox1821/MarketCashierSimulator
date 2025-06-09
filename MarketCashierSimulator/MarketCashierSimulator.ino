#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  { '7', '8', '9', 'A' },
  { '4', '5', '6', 'B' },
  { '1', '2', '3', 'C' },
  { 'E', '0', '=', 'D' }
};

//  A:Add (Add to end of list)  --  B:Apply discount (Apply the discount to the total purchase.)  --  C:Cancel purchase (clears the product list)  --  D:Delete (Remove last product) --
//  ' = ':Show total and request payment (Calculates total, waits for input, gives change) -- E:Exit (End or Reboot program)  --

const byte PRODUCTS = 3;
const byte DATA = 3;

String products[PRODUCTS][DATA] = {
  { "001", "CocaCola", "20" },
  { "002", "Papas Fritas", "25" },
  { "003", "Mostaza", "10" },
};
//After, change the data

char inserted_products[100][4];

byte row_pins[ROWS] = { 12, 11, 8, 7 };
byte col_pins[COLS] = { 6, 5, 4, 2 };

Keypad cust_keypad = Keypad(makeKeymap(keys), row_pins, col_pins, ROWS, COLS);

String inserted_product;
char char_code[4];
char cust_key;
byte digits = 3, total_codes = 100,i=0;
byte productIndex;
int inserted_products_lenght = 0;
int price = 0;

enum Estado {
  ESPERANDO_CODIGO,
  ESPERANDO_ACCION
};

Estado estado_actual = ESPERANDO_CODIGO;

String inserted_product;
char char_code[3];
char cust_key;
byte digits=3,i=0,total_codes=100;
byte productIndex;
int inserted_products_lenght=0;
void setup() {
  Serial.begin(9600);
  Serial.println("Ingrese el código de producto [xxx]: ");
}

void loop() {
 char key = cust_keypad.getKey();
  if (key) {
    switch (estado_actual) {
      case ESPERANDO_CODIGO:
        if (isNumber(key)) {
          char_code[i++] = key;
          Serial.print(key);

          if (i == 3) {
            char_code[3] = '\0';  // Finaliza el string
            Serial.println();
            Serial.println("Código ingresado. Presione 'A' para agregar el producto o 'C' para cancelar.");
            estado_actual = ESPERANDO_ACCION;
          }
        } else {
          estado_actual = ESPERANDO_ACCION;
        }
        break;

      case ESPERANDO_ACCION:
        handleAction(key,char_code);
        break;
    }
  }
  /*  while(newcust_key!='E'){
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
    } */
}


void handleAction(char action,char char_code[4]) {
  static unsigned long lastEPress = 0;
  bool codeReady = char_code[0] != '\0';

  if (action == 'A') {
    if (codeReady) {
      addProduct(char_code);
      resetCode();
    } else {
      Serial.println("Debe ingresar un código de producto primero.");
    }
    return;
  }

  if (action == 'C') {
    if (codeReady) {
      Serial.println("Código cancelado.");
      resetCode();
    } else {
      cancelPurchase();
    }
    return;
  }

  // Acciones que no requieren código
  switch (action) {
    case 'B':
      if (inserted_products_lenght > 0) applyDiscount();
      else Serial.println("Debe agregar al menos un producto.");
      break;

    case 'D':
      if (inserted_products_lenght > 0) deleteLastProduct();
      else Serial.println("No hay productos para eliminar.");
      break;

    case '=':
      if (inserted_products_lenght > 0) showTotalAndProcessThePayment();
      else Serial.println("No hay productos agregados.");
      break;

    case 'E':
      if (millis() - lastEPress < 2000) {
        Serial.println("Finalizando programa...");
        while (true)
          ;  // detener
      } else {
        Serial.println("Reiniciando compra...");
        cancelPurchase();
        lastEPress = millis();
      }
      break;

    default:
      Serial.println("Tecla no reconocida.");
  }
}

void resetCode() {
  for (int j = 0; j < 4; j++) char_code[j] = '\0';
  i = 0;
  estado_actual = ESPERANDO_CODIGO;
  Serial.println();
  Serial.println("Ingrese el código de producto [xxx]: ");
}

bool isNumber(char key_pressed) {
  bool notIsA = key_pressed!='A',
       notIsB = key_pressed!='B',
       notIsC = key_pressed!='C',
       notIsD = key_pressed!='D',
       notIsE = key_pressed!='E',
       notIsEqual = key_pressed!='=';

  return notIsA && notIsB && notIsC && notIsD && notIsE && notIsEqual;
}

void addProduct(char new_code[4]) {
  bool found = false;
  for (int j = 0; j < PRODUCTS; j++) {
    if (products[j][0].equals(new_code)) {
       found = true;
      price += products[j][2].toInt();

      strncpy(inserted_products[inserted_products_lenght], new_code, 4);
      Serial.print("Asignado a inserted_products[");
      Serial.print(inserted_products_lenght);
      Serial.print("]: ");
      Serial.println(inserted_products[inserted_products_lenght]);  // ¿Se ve el código?
      
      inserted_products_lenght++;

      break;
    }
  }
  if(found){
    Serial.println();
    Serial.print("----------------------[Producto agregado]-----------------------");
    Serial.println();
    for (int k = 0; k < inserted_products_lenght; k++) {
      for (int l = 0; l < PRODUCTS; l++) {
        if (products[l][0].equals(inserted_products[k])) {
          Serial.println();
          Serial.print("-- Codigo: ");
          Serial.print(products[l][0]);
          Serial.print(" --- Nombre: ");
          Serial.print(products[l][1]);
          Serial.print(" --->  $");
          Serial.print(products[l][2]);
        }
      }
    }
    Serial.println();
    Serial.println();
    Serial.println("----------------------[Producto agregado]-----------------------");
    Serial.println();
    
  }
  if (!found) {
    Serial.println("Código no encontrado.");
    return;
  }
}

void applyDiscount() {
  // codigo para aplicar descuento al total (al final de la compra)
}
void cancelPurchase() {
  // codigo para cancelar la compra y limpiar la lista (seguir trabajando)
}
void deleteLastProduct() {
  if (inserted_products_lenght <= 0) return;
  for (int j = PRODUCTS - 1; j >= 0; j--) {
    if (products[j][0].equals(inserted_products[inserted_products_lenght - 1])) {
      price -= products[j][2].toInt();
      break;
    }
  }
  strcpy(inserted_products[inserted_products_lenght - 1], "");
  inserted_products_lenght--;
  Serial.print("Ultimo producto eliminado");
  delay(300);
  for (int k = 0; k <= inserted_products_lenght; k++) {
    for (int l = 0; l <= PRODUCTS; l++) {
      if (products[l][0].equals(inserted_products[k])) {
        Serial.println();
        Serial.print("-- Codigo: ");
        Serial.print(products[l][0]);
        Serial.print(" --- Nombre: ");
        Serial.print(products[l][1]);
        Serial.print(" --->  $");
        Serial.print(products[l][2]);
      }
    }
  }
}
void showTotalAndProcessThePayment() {
  // codigo para ver el total, realizar el pago y mostrar el vuelto
}