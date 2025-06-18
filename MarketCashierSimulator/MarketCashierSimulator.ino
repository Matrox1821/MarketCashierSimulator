#include <Keypad.h>
#include <avr/pgmspace.h>

const byte ROWS = 4;
const byte COLS = 4;

const char keys[ROWS][COLS] = {
  { '7', '8', '9', 'A' },
  { '4', '5', '6', 'B' },
  { '1', '2', '3', 'C' },
  { 'E', '0', '=', 'D' }
};

//  A:Add (Add to end of list)  --  B:Apply discount (Apply the discount to the total purchase.)  --  C:Cancel purchase (clears the product list)  --  D:Delete (Remove last product) --
//  ' = ':Show total and request payment (Calculates total, waits for input, gives change) -- E:Exit (End or Reboot program)  --

const byte PRODUCTS = 20;
const byte DATA = 3;

// Producto 0
const char p00_0[] PROGMEM = "001";
const char p00_1[] PROGMEM = "CocaCola 2.25L";
const char p00_2[] PROGMEM = "4000";

// Producto 1
const char p01_0[] PROGMEM = "002";
const char p01_1[] PROGMEM = "Papas Fritas Pehuamar 450gr";
const char p01_2[] PROGMEM = "8500";

// Producto 2
const char p02_0[] PROGMEM = "003";
const char p02_1[] PROGMEM = "Mostaza Savora 500gr";
const char p02_2[] PROGMEM = "2250";

// Producto 3
const char p03_0[] PROGMEM = "004";
const char p03_1[] PROGMEM = "Pepas Delicias de la Nonna 200gr";
const char p03_2[] PROGMEM = "1200";

// Producto 4
const char p04_0[] PROGMEM = "005";
const char p04_1[] PROGMEM = "Galletitas 5 semillas 500gr";
const char p04_2[] PROGMEM = "1200";

// Producto 5
const char p05_0[] PROGMEM = "006";
const char p05_1[] PROGMEM = "Arroz Gallo Oro 1Kg";
const char p05_2[] PROGMEM = "2300";

// Producto 6
const char p06_0[] PROGMEM = "007";
const char p06_1[] PROGMEM = "Cafe Cabrales Molido 250gr";
const char p06_2[] PROGMEM = "13300";

// Producto 7
const char p07_0[] PROGMEM = "008";
const char p07_1[] PROGMEM = "Speed 500ml";
const char p07_2[] PROGMEM = "2500";

// Producto 8
const char p08_0[] PROGMEM = "009";
const char p08_1[] PROGMEM = "Leche Serenisima 1L";
const char p08_2[] PROGMEM = "2500";

// Producto 9
const char p09_0[] PROGMEM = "010";
const char p09_1[] PROGMEM = "Yerba Mate Taragui 1Kg";
const char p09_2[] PROGMEM = "4200";

// Producto 10
const char p10_0[] PROGMEM = "011";
const char p10_1[] PROGMEM = "Fideos Don Vicente 500gr";
const char p10_2[] PROGMEM = "2300";

// Producto 11
const char p11_0[] PROGMEM = "012";
const char p11_1[] PROGMEM = "Pure de Tomate Arcor 530gr";
const char p11_2[] PROGMEM = "1000";

// Producto 12
const char p12_0[] PROGMEM = "013";
const char p12_1[] PROGMEM = "Queso Rallado La Paulina 40gr";
const char p12_2[] PROGMEM = "1000";

// Producto 13
const char p13_0[] PROGMEM = "014";
const char p13_1[] PROGMEM = "Pan Lactal Bimbo 600gr";
const char p13_2[] PROGMEM = "5600";

// Producto 14
const char p14_0[] PROGMEM = "015";
const char p14_1[] PROGMEM = "Harina Pureza 0000 1Kg";
const char p14_2[] PROGMEM = "900";

// Producto 15
const char p15_0[] PROGMEM = "016";
const char p15_1[] PROGMEM = "Sprite 2.25L";
const char p15_2[] PROGMEM = "3800";

// Producto 16
const char p16_0[] PROGMEM = "017";
const char p16_1[] PROGMEM = "Mermelada BC Frutilla 390gr";
const char p16_2[] PROGMEM = "3000";

// Producto 17
const char p17_0[] PROGMEM = "018";
const char p17_1[] PROGMEM = "Galletas Surtido Bagley 400gr";
const char p17_2[] PROGMEM = "2300";

// Producto 18
const char p18_0[] PROGMEM = "019";
const char p18_1[] PROGMEM = "Detergente Magistral 750ml";
const char p18_2[] PROGMEM = "5200";

// Producto 19
const char p19_0[] PROGMEM = "020";
const char p19_1[] PROGMEM = "Aceite de girasol Natura 1.5L";
const char p19_2[] PROGMEM = "3600";

// Matriz de punteros a los textos anteriores
const char* const products[PRODUCTS][DATA] PROGMEM = {
  { p00_0, p00_1, p00_2 },
  { p01_0, p01_1, p01_2 },
  { p02_0, p02_1, p02_2 },
  { p03_0, p03_1, p03_2 },
  { p04_0, p04_1, p04_2 },
  { p05_0, p05_1, p05_2 },
  { p06_0, p06_1, p06_2 },
  { p07_0, p07_1, p07_2 },
  { p08_0, p08_1, p08_2 },
  { p09_0, p09_1, p09_2 },
  { p10_0, p10_1, p10_2 },
  { p11_0, p11_1, p11_2 },
  { p12_0, p12_1, p12_2 },
  { p13_0, p13_1, p13_2 },
  { p14_0, p14_1, p14_2 },
  { p15_0, p15_1, p15_2 },
  { p16_0, p16_1, p16_2 },
  { p17_0, p17_1, p17_2 },
  { p18_0, p18_1, p18_2 },
  { p19_0, p19_1, p19_2 },
};


byte row_pins[ROWS] = { 12, 11, 8, 7 };
byte col_pins[COLS] = { 6, 5, 4, 2 };

Keypad cust_keypad = Keypad(makeKeymap(keys), row_pins, col_pins, ROWS, COLS);

enum Estado {
  ESPERANDO_CODIGO,
  SELECCIONE_CANTIDAD,
  ESPERANDO_ACCION,
  AGREGAR_O_CANCELAR,
  FASE_PAGO
};

Estado estado_actual = ESPERANDO_CODIGO;

byte total_code_products=20, code_digits=4;

char inserted_products[20][4];
byte amount_products[20];

char char_code[4];

bool esperandoCantidad = false;
char amount[4] = {'\0'};
byte cantidad_index = 0;

byte i=0, inserted_products_lenght=0;

unsigned long total_price=0;

void setup() {
  Serial.begin(9600);
  Serial.print(F("Ingrese el codigo de producto [xxx]:"));
  Serial.println(F(""));
}

void loop() {
  char key = cust_keypad.getKey();
  if (!key) return;
  switch (estado_actual) {
    case ESPERANDO_CODIGO:
      WriteCode(key);
      break;
    case AGREGAR_O_CANCELAR:
      AddOrCancel(key);
      break;
    case ESPERANDO_ACCION:
      handleAction(key);
      break;
    case FASE_PAGO:
      showTotalAndProcessThePayment(key);
      break;
  }
}

//messages

void MessageToWriteCode(){
  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(F("Cantidad ingresada."));
  Serial.println(F(""));
  Serial.println(F("------------------------------------------------------------------------------"));
  Serial.println(F(""));
  Serial.println(F("Presione 'A' para agregar el producto."));
  Serial.println(F("Para cancelar la compra presione 'C'."));
  Serial.println(F("Para borrar el ultimo codigo ingresado y seguir agregando productos presione 'D'."));
  Serial.println(F("Para dejar de agregar productos y ejecutar otras acciones presione 'E'."));
}

void MessageToReloadPurchase(){
  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(F("Reiniciando compra..."));
  Serial.println(F(""));
  Serial.println(F(""));
}

void MessageToDeleteCode(){
  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(F("[  Codigo borrado.  ]"));
  Serial.println(F(""));
  Serial.println(F(""));
}

void MessageCodeNotEntered(){
  Serial.println(F(""));
  Serial.println(F("[  Debe ingresar un codigo de producto primero.  ]"));
  Serial.println(F(""));
}

void MessageToDeleteCodeNonExistent(){
  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(F("No hay productos para eliminar."));
  Serial.println(F(""));
  Serial.println(F(""));
}
void MessagePayment(){
  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(F("---------------------------[ Fase de pago ]-----------------------------------"));
  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(F("Presione 'A' para pagar."));
  Serial.println(F("Presione 'B' para agregar descuento."));
  Serial.print(F("El precio a pagar es de $"));
  Serial.print(total_price);
  Serial.println(F(""));
}
void MessageProductAdded(){
  Serial.println();
  Serial.println();
  Serial.println(F("----------------------[Producto agregado]-----------------------"));
  Serial.println();
}
void MessageOfDiiscounts(byte percent){
  switch (percent) {
    case 5:
      Serial.println();
      Serial.print(F("Descuento seleccionado 5%"));
      Serial.println();
      break;
    case 10:
      Serial.println();
      Serial.print(F("Descuento seleccionado 10%"));
      Serial.println();
      break;
    case 15:
      Serial.println();
      Serial.print(F("Descuento seleccionado 15%"));
      Serial.println();
      break;
    case 20:
      Serial.println();
      Serial.print(F("Descuento seleccionado 20%"));
      Serial.println();
      break;
  }
}

void showList() {
  for (byte k = 0; k < inserted_products_lenght; k++) {
    for (byte l = 0; l < PRODUCTS; l++) {
      char code_buffer[4];
      strcpy_P(code_buffer, (PGM_P)pgm_read_word(&products[l][0]));
      if (strcmp(code_buffer, inserted_products[k]) == 0) {
        char name_buffer[64];
        char price_buffer[64];
        strcpy_P(name_buffer, (PGM_P)pgm_read_word(&products[l][1]));
        strcpy_P(price_buffer, (PGM_P)pgm_read_word(&products[l][2]));
        unsigned int precio = atoi(price_buffer);
        unsigned long total = (unsigned long)precio * amount_products[k];
        Serial.println();
        Serial.print(F("-- Codigo: "));
        Serial.print(code_buffer);
        Serial.print(F(" --- Nombre: "));
        Serial.print(name_buffer);
        Serial.print(F(" ---> x"));
        Serial.print(amount_products[k]);
        Serial.print(F(" ---> $"));
        Serial.print(total);
        Serial.println();
      }
    }
  }
}


void WriteCode(char key) {
   if (esperandoCantidad) {
    if (isNumber(key) && cantidad_index < 3) {
      amount[cantidad_index++] = key;
      Serial.print(key);
    } else if (cantidad_index==3||key == 'B') {
      amount[cantidad_index] = '\0';
      esperandoCantidad = false;
      

      MessageToWriteCode();

      estado_actual = AGREGAR_O_CANCELAR;
    }
    return;
  }
  if (isNumber(key)) {
    char_code[i++] = key;
    Serial.print(key);

    if (i == 3) {
      char_code[3] = '\0';
      i = 0;
      esperandoCantidad = true;
      Serial.println(F(""));
      Serial.println(F(""));
      Serial.println(F("Ingrese cantidad (max 3 digitos). Presione 'B' para confirmar:"));
    }
  } else {
    switch (key) {
      case 'C':
        estado_actual = ESPERANDO_CODIGO;
        MessageToReloadPurchase();
        cancelPurchase();
        break;
      case 'D':
        estado_actual = ESPERANDO_CODIGO;
        MessageToDeleteCode();
        deleteLastProduct();
        break;
      case 'E':
        estado_actual = ESPERANDO_ACCION;

        break;
    }
    resetCode();
  }
}
void AddOrCancel(char action) {
  byte cantidad = atoi(amount); 
  switch (action) {
    case 'A':
      if (cantidad == 0) cantidad = 1;
      addProduct(char_code,cantidad);
      estado_actual = ESPERANDO_CODIGO;
      break;
    case 'C':
      MessageToReloadPurchase();
      cancelPurchase();
      estado_actual = ESPERANDO_CODIGO;
      break;
    case 'D':
      MessageToDeleteCode();
      deleteLastProduct();
      estado_actual = ESPERANDO_CODIGO;
      break;
    case 'E':
      estado_actual = ESPERANDO_ACCION;
      break;
  }
  resetCode();
}

void handleAction(char action) {
  switch (action) {
    case 'A':
      estado_actual = ESPERANDO_CODIGO;
      resetCode();
      break;
    case 'C':
      MessageToReloadPurchase();
      cancelPurchase();
      estado_actual = ESPERANDO_CODIGO;
      resetCode();
      break;

    case 'D':
      if (inserted_products_lenght > 0) {
        MessageToDeleteCode();
        deleteLastProduct();
      } else {
        MessageToDeleteCodeNonExistent();
        resetCode();
      }
      break;

    case 'E':
      Serial.println(F("Finalizando turno..."));
      cancelPurchase();
      Serial.end();
      break;

    case '=':
      if (inserted_products_lenght > 0) {
        MessagePayment();
        estado_actual = FASE_PAGO;
      } else {
        Serial.println(F(""));
        Serial.println(F("No hay productos agregados."));
        resetCode();
      }
      break;

    default:
      Serial.println(F("Tecla no reconocida."));
  }
}

void resetCode() {
  for (byte j = 0; j < 4; j++) char_code[j] = '\0';
  i = 0;
  for (byte j = 0; j < 4; j++) amount[j] = '\0';
  cantidad_index = 0;
  esperandoCantidad = false;
  if (estado_actual == ESPERANDO_ACCION) {
    Serial.println(F(""));
    Serial.println(F(""));
    Serial.println(F("------------------------------------------------------------------------------"));
    Serial.println(F(""));
    Serial.println(F(""));
    Serial.println(F("Para seguir agregando productos presione 'A'"));
    Serial.println(F("Para cancelar compra presione 'C'"));
    Serial.println(F("Para eliminar el ultimo producto presione 'D'"));
    Serial.println(F("Para terminar turno presione 'E'"));
    Serial.println(F("Para realizar el pago presione '='"));
    Serial.print(F("Seleccione una opcion: "));
  }

  if (estado_actual == ESPERANDO_CODIGO) {
    Serial.println(F(""));
    Serial.println(F(""));
    Serial.println(F("------------------------------------------------------------------------------"));
    Serial.println(F(""));
    Serial.println(F(""));
    Serial.println(F("Para borrar el ultimo codigo ingresado y seguir agregando productos presione 'D'."));
    Serial.println(F("Para cancelar la compra presione 'C'."));
    Serial.println(F("Para dejar de agregar productos y ejecutar otras acciones presione 'E'."));
    Serial.println(F("Ingrese el codigo de producto [xxx]: "));
    Serial.println(F(""));
  }
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

void addProduct(char new_code[4], byte cantidad) {
  bool found = false;
  for (byte j = 0; j < PRODUCTS; j++) {
    char buffer[4]; 
    strcpy_P(buffer, (PGM_P)pgm_read_word(&(products[j][0])));

    if (strcmp(buffer, new_code) == 0) {
      found = true;
      strncpy(inserted_products[inserted_products_lenght], new_code, 3);
      inserted_products[inserted_products_lenght][3] = '\0';
      amount_products[inserted_products_lenght] = cantidad;

      char price_buffer[64];
      strcpy_P(price_buffer, (PGM_P)pgm_read_word(&products[j][2]));
      unsigned int precio = atoi(price_buffer);
      unsigned long total = (unsigned long)precio * cantidad;

      total_price += total ;
      inserted_products_lenght++;
      break;
    }
  }
  if (found) {
    MessageProductAdded();
    showList();
    MessageProductAdded();
  } else {
    Serial.println();
    Serial.println(F("Codigo no encontrado."));
    Serial.println();
  }
}

void cancelPurchase() {
  estado_actual = ESPERANDO_CODIGO;
  for (byte j = 0; j < 4; j++) char_code[j] = '\0';
  for (byte i = 0; i < total_code_products; i++) {
    for (byte j = 0; j < code_digits; j++) {
      inserted_products[i][j] = '\0';
    }
  }
  inserted_products_lenght = 0;
  return;
}

void deleteLastProduct() {
  if (inserted_products_lenght <= 0) return;

  char last_code[4];

  byte index = inserted_products_lenght - 1;

  int cantidad = amount_products[index]; 

  strncpy(last_code, inserted_products[index], 4);

  for (int j = PRODUCTS - 1; j >= 0; j--) {
    char code_buffer[8];
    char price_buffer[64];
    strcpy_P(code_buffer, (PGM_P)pgm_read_word(&products[j][0]));
    if (strcmp(code_buffer, last_code) == 0) {
      strcpy_P(price_buffer, (PGM_P)pgm_read_word(&products[j][2]));
      unsigned int precio = atoi(price_buffer);
      total_price -= precio * cantidad;
      break;
    }
  }

  inserted_products_lenght--;
  inserted_products[inserted_products_lenght][0] = '\0';
  amount_products[inserted_products_lenght] = 0;

  Serial.println();
  Serial.println(F("Ultimo producto eliminado"));
  Serial.println();
  delay(300);

  showList();
}

void showTotalAndProcessThePayment(char action) {

  switch (action) {
    case 'A':
      paymentProcess();
      resetCode();
      break;

    case 'B':
      Serial.println();
      Serial.print(F("Seleccione el porcentaje."));
      Serial.println();
      Serial.print(F("A) 5%"));
      Serial.println();
      Serial.print(F("B) 10%"));
      Serial.println();
      Serial.print(F("C) 15%"));
      Serial.println();
      Serial.print(F("D) 20%"));
      Serial.println();
      selectDiscount();
      break;

    default:
      Serial.println(F("Tecla no reconocida."));
  }
}
void paymentProcess() {
  Serial.println();
  Serial.print(F("El precio a pagar es de $"));
  Serial.print(total_price);
  Serial.println();
  Serial.println();
  Serial.println(F("Para dejar de escribir numeros presione 'E'."));
  Serial.print(F("Ingrese el monto a pagar: $"));

  bool payCompleted = false;
  byte payIndex = 0;
  char pay[20] = {0}; 

  while (!payCompleted) {
    char key = cust_keypad.getKey();
    if (key == NO_KEY) continue;
    if (isNumber(key)) {
      pay[payIndex++] = key;
      Serial.print(key);
    }
    else if (key == 'E') {
      pay[payIndex] = '\0';
      int payValue = atoi(pay);
      if (payValue < total_price) {
        memset(pay, 0, sizeof(pay));
        payIndex = 0;
        Serial.println();
        Serial.println(F("Pago insuficiente"));
        Serial.println();
        Serial.println();
        Serial.println(F("Para dejar de escribir numeros presione 'E'."));
        Serial.print(F("Ingrese un monto mayor: $"));
      }
      else {
        Serial.println();
        Serial.println();
        Serial.println(F("-----------------------[ Pago aceptado ]---------------------------"));
        Serial.println();
        Serial.println();
        if (payValue == total_price) {
          Serial.println(F("Pago justo, no hay vuelto."));
          Serial.println();
        }
        else {
          Serial.print(F("El vuelto es de $"));
          Serial.print(payValue - total_price);
          Serial.println();
        }
        showList();
        Serial.println();
        Serial.println(F("-----------------------[ Nueva compra ]---------------------------"));
        Serial.println();
        resetCode();
        payCompleted = true;
      }
    }
  }
}

void selectDiscount() {
  bool added_discount = false;
  while (!added_discount) {
    char discount_key = cust_keypad.getKey();
    if (discount_key == NO_KEY) continue;
    switch (discount_key) {
      case 'A':
        MessageOfDiiscounts(5);
        applyDiscount(5);
        added_discount = true;
        break;
      case 'B':
        MessageOfDiiscounts(10);
        applyDiscount(10);
        added_discount = true;
        break;
      case 'C':
        MessageOfDiiscounts(15);
        applyDiscount(15);
        added_discount = true;
        break;
      case 'D':
        MessageOfDiiscounts(20);
        applyDiscount(20);
        added_discount = true;
        break;

      default:
        Serial.println(F("Descuento inexistente."));
    }
  }
  paymentProcess();
}

void applyDiscount(byte discount){
  unsigned long descuento = total_price * discount / 100.0;
  total_price -= descuento;
}