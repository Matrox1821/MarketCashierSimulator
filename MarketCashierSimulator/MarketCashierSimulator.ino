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

const byte PRODUCTS = 9;
const byte DATA = 3;

// Producto 0
const char p00_0[] PROGMEM = "001";
const char p00_1[] PROGMEM = "CocaCola";
const char p00_2[] PROGMEM = "3000";

// Producto 1
const char p01_0[] PROGMEM = "002";
const char p01_1[] PROGMEM = "Papas Fritas 500g";
const char p01_2[] PROGMEM = "1500";

// Producto 2
const char p02_0[] PROGMEM = "003";
const char p02_1[] PROGMEM = "Mostaza";
const char p02_2[] PROGMEM = "100";

// Producto 3
const char p03_0[] PROGMEM = "004";
const char p03_1[] PROGMEM = "Pepas 1kg";
const char p03_2[] PROGMEM = "2300";

// Producto 4
const char p04_0[] PROGMEM = "005";
const char p04_1[] PROGMEM = "Galle. 5 semillas";
const char p04_2[] PROGMEM = "1000";

// Producto 5
const char p05_0[] PROGMEM = "006";
const char p05_1[] PROGMEM = "Arroz 1kg";
const char p05_2[] PROGMEM = "2200";

// Producto 6
const char p06_0[] PROGMEM = "007";
const char p06_1[] PROGMEM = "Jugo Tang";
const char p06_2[] PROGMEM = "600";

// Producto 7
const char p07_0[] PROGMEM = "008";
const char p07_1[] PROGMEM = "Speed";
const char p07_2[] PROGMEM = "1900";

// Producto 8
const char p08_0[] PROGMEM = "009";
const char p08_1[] PROGMEM = "Leche Serenisima";
const char p08_2[] PROGMEM = "1800";

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
};


byte row_pins[ROWS] = { 12, 11, 8, 7 };
byte col_pins[COLS] = { 6, 5, 4, 2 };

Keypad cust_keypad = Keypad(makeKeymap(keys), row_pins, col_pins, ROWS, COLS);

enum Estado {
  ESPERANDO_CODIGO,
  ESPERANDO_ACCION,
  AGREGAR_O_CANCELAR,
  FASE_PAGO
};

Estado estado_actual = ESPERANDO_CODIGO;

byte total_code_products=10, code_digits=4;

char inserted_products[10][4];

char char_code[4];
byte i=0, inserted_products_lenght=0;

int total_price=0;

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
      AddOrCancel(key, char_code);
      break;
    case ESPERANDO_ACCION:
      handleAction(key, char_code);
      break;
    case FASE_PAGO:
      showTotalAndProcessThePayment(key, char_code);
      break;
  }
}


void WriteCode(char key) {
  if (isNumber(key)) {
    char_code[i++] = key;
    Serial.print(key);

    if (i == 3) {
      char_code[3] = '\0';  // Finaliza el string
      Serial.println(F(""));
      Serial.println(F(""));
      Serial.println(F("Codigo ingresado."));
      Serial.println(F(""));
      Serial.println(F(""));
      Serial.println(F("------------------------------------------------------------------------------"));
      Serial.println(F(""));
      Serial.println(F(""));
      Serial.println(F("Presione 'A' para agregar el producto."));
      Serial.println(F("Para cancelar la compra presione 'C'."));
      Serial.println(F("Para borrar el ultimo codigo ingresado y seguir agregando productos presione 'D'."));
      Serial.println(F("Para dejar de agregar productos y ejecutar otras acciones presione 'E'."));

      estado_actual = AGREGAR_O_CANCELAR;
    }
  } else {
    switch (key) {
      case 'C':
        estado_actual = ESPERANDO_CODIGO;
        Serial.println(F(""));
        Serial.println(F("Reiniciando compra..."));
        Serial.println(F(""));
        cancelPurchase();
        break;
      case 'D':
        estado_actual = ESPERANDO_CODIGO;
        Serial.println(F(""));
        Serial.println(F(""));
        Serial.println(F("[  Codigo borrado.  ]"));
        deleteLastProduct();
        Serial.println(F(""));
        Serial.println(F(""));
        break;
      case 'E':
        estado_actual = ESPERANDO_ACCION;
        break;
    }
    resetCode();
  }
}
void AddOrCancel(char action, char code[4]) {
  bool codeReady = strlen(code) == 3;

  switch (action) {
    case 'A':
      if (!codeReady) {
        Serial.println(F(""));
        Serial.println(F("[  Debe ingresar un codigo de producto primero.  ]"));
        Serial.println(F(""));
        estado_actual = ESPERANDO_CODIGO;
        break;
      }
      addProduct(code);
      estado_actual = ESPERANDO_CODIGO;
      break;

    case 'C':
      estado_actual = ESPERANDO_CODIGO;
      Serial.println(F(""));
      Serial.println(F("Reiniciando compra..."));
      Serial.println(F(""));
      cancelPurchase();
      break;

    case 'D':
      estado_actual = ESPERANDO_CODIGO;
      Serial.println(F(""));
      Serial.println(F(""));
      Serial.println(F("[  Codigo borrado.  ]"));
      deleteLastProduct();
      Serial.println(F(""));
      Serial.println(F(""));
      break;

    case 'E':
      estado_actual = ESPERANDO_ACCION;
      break;
  }

  resetCode();
}

void handleAction(char action, char code[4]) {
  static unsigned long lastEPress = 0;
  bool codeReady = strlen(code) == 3;

  switch (action) {
    case 'A':
      estado_actual = ESPERANDO_CODIGO;
      resetCode();
      break;

    case 'C':
      Serial.println(F(""));
      Serial.println(F(""));
      Serial.println(F("Reiniciando compra..."));
      Serial.println(F(""));
      Serial.println(F(""));
      cancelPurchase();
      estado_actual = ESPERANDO_CODIGO;
      resetCode();
      break;

    case 'D':
      if (inserted_products_lenght > 0) {
        deleteLastProduct();
      } else {
        Serial.println(F(""));
        Serial.println(F(""));
        Serial.println(F("No hay productos para eliminar."));
        Serial.println(F(""));
        Serial.println(F(""));
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

void addProduct(char new_code[4]) {
  bool found = false;
  for (byte j = 0; j < PRODUCTS; j++) {
    char buffer[4];  // Solo para código de 3 chars + null
    strcpy_P(buffer, (PGM_P)pgm_read_word(&(products[j][0])));
    if (strcmp(buffer, new_code) == 0) {
      found = true;

      // Copiamos solo 3 caracteres, luego agregamos terminador
      strncpy(inserted_products[inserted_products_lenght], new_code, 3);
      inserted_products[inserted_products_lenght][3] = '\0';

      char price_buffer[8];
      strcpy_P(price_buffer, (PGM_P)pgm_read_word(&products[j][2]));
      total_price += atoi(price_buffer);

      inserted_products_lenght++;

      break;
    }
  }
  if (found) {
    Serial.println();
    Serial.println();
    Serial.println(F("----------------------[Producto agregado]-----------------------"));
    Serial.println();
    showList();
    Serial.println();
    Serial.println();
    Serial.println(F("----------------------[Producto agregado]-----------------------"));
    Serial.println();
  } else {
    Serial.println();
    Serial.println(F("Codigo no encontrado."));
    Serial.println();
  }
}

void cancelPurchase() {
  estado_actual = ESPERANDO_CODIGO; //reset state
  for (byte j = 0; j < 4; j++) char_code[j] = '\0';
  for (byte i = 0; i < total_code_products; i++) {
    for (byte j = 0; j < code_digits; j++) {
      inserted_products[i][j] = '\0';
    }
  }
  return;
}

void deleteLastProduct() {
  if (inserted_products_lenght == 0) return;

  // Buscar producto para restar precio
  char code_buffer[8];
  char* last_code = inserted_products[inserted_products_lenght - 1];
  for (int j = PRODUCTS - 1; j >= 0; j--) {
    char price_buffer[16];
    strcpy_P(code_buffer, (PGM_P)pgm_read_word(&products[j][0]));
    if (strcmp(code_buffer, last_code) == 0) {
      strcpy_P(price_buffer, (PGM_P)pgm_read_word(&products[j][2]));
      total_price -= atoi(price_buffer);
      break;
    }
  }

  // Eliminar producto
  inserted_products_lenght--;
  inserted_products[inserted_products_lenght][0] = '\0';

  Serial.println();
  Serial.println(F("Ultimo producto eliminado"));
  Serial.println();
  delay(300);

  // Mostrar productos restantes
  for (byte k = 0; k < inserted_products_lenght; k++) {
    for (byte l = 0; l < PRODUCTS; l++) {
      strcpy_P(code_buffer, (PGM_P)pgm_read_word(&products[l][0]));
      if (strcmp(code_buffer, inserted_products[k]) == 0) {
        char name_buffer[32];
        char price_buffer[16];

        strcpy_P(name_buffer, (PGM_P)pgm_read_word(&products[l][1]));
        strcpy_P(price_buffer, (PGM_P)pgm_read_word(&products[l][2]));

        Serial.println();
        Serial.print(F("-- Codigo: "));
        Serial.print(code_buffer);
        Serial.print(F(" --- Nombre: "));
        Serial.print(name_buffer);
        Serial.print(F(" --->  $"));
        Serial.print(price_buffer);
        Serial.println();
      }
    }
  }
}

void showTotalAndProcessThePayment(char action, char code[4]) {
  static unsigned long lastEPress = 0;
  bool codeReady = strlen(code) == 3;

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

void selectDiscount() {
  bool added_discount = false;
  while (!added_discount) {
    char discount_key = cust_keypad.getKey();
    if (discount_key == NO_KEY) continue;
    switch (discount_key) {
      case 'A':
        Serial.println();
        Serial.print(F("Descuento seleccionado 5%"));
        Serial.println();
        applyDiscount(5);
        added_discount = true;
        break;
      case 'B':
        Serial.println();
        Serial.print(F("Descuento seleccionado 10%"));
        Serial.println();
        applyDiscount(10);
        added_discount = true;
        break;
      case 'C':
        Serial.println();
        Serial.print(F("Descuento seleccionado 15%"));
        Serial.println();
        applyDiscount(15);
        added_discount = true;
        break;
      case 'D':
        Serial.println();
        Serial.print(F("Descuento seleccionado 20%"));
        Serial.println();
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
  total_price-=(total_price*discount)/100;
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
  char pay[20] = {0};  // inicializar a cero

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
        memset(pay, 0, sizeof(pay)); // limpiar buffer
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
        payCompleted = true; // para salir del while
      }
    }
  }
}
void showList() {
  for (byte k = 0; k < inserted_products_lenght; k++) {
    for (byte l = 0; l < PRODUCTS; l++) {
      char code_buffer[4];
      strcpy_P(code_buffer, (PGM_P)pgm_read_word(&products[l][0]));
      if (strcmp(code_buffer, inserted_products[k]) == 0) {
        char name_buffer[32];
        char price_buffer[16];
        strcpy_P(name_buffer, (PGM_P)pgm_read_word(&products[l][1]));
        strcpy_P(price_buffer, (PGM_P)pgm_read_word(&products[l][2]));
        Serial.println();
        Serial.print(F("-- Codigo: "));
        Serial.print(code_buffer);
        Serial.print(F(" --- Nombre: "));
        Serial.print(name_buffer);
        Serial.print(F(" --->  $"));
        Serial.print(price_buffer);
        Serial.println();
      }
    }
  }
}  