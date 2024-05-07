/*
   Para ESP32

   Modulo
   RFID RC522     ESP32

   3.3v           3.3v
   RST            27
   GND            GND
   IRQ            no se usa
   MISO           19
   MOSI           23
   SCK            18
   SDA            15

*/


#include "SPI.h"
#include "MFRC522.h"

#define RST_PIN  27
#define SS_PIN   15

MFRC522 mfrc522(SS_PIN, RST_PIN); // Inicializamos el lector RFID

#include <LiquidCrystal_I2C.h>

int lcdColumns = 16; // Definimos el número de columnas del LCD
int lcdRows = 2; // Definimos el número de filas del LCD

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); // Creamos un objeto LiquidCrystal_I2C para controlar el LCD

void setup() {
  Serial.begin(115200); // Inicializamos la comunicación serial para imprimir mensajes de depuración
  while (!Serial); // Esperamos hasta que se abra el monitor serial
  SPI.begin(); // Inicializamos la comunicación SPI
  mfrc522.PCD_Init(); // Inicializamos el lector RFID
  lcd.init(); // Inicializamos el LCD
  lcd.backlight(); // Encendemos la retroiluminación del LCD
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) { // Verificamos si hay una tarjeta RFID presente
    Serial.println("Contenido de la tarjeta:"); // Imprimimos un mensaje en el monitor serial
    String uuid = ""; // Creamos una cadena para almacenar el UUID
    for (byte i = 0; i < mfrc522.uid.size; i++) { // Recorremos cada byte del UUID
      if (mfrc522.uid.uidByte[i] < 0x10) uuid += "0"; // Si el byte es menor que 0x10, agregamos un 0 a la cadena (formato hexadecimal)
      uuid += String(mfrc522.uid.uidByte[i], HEX); // Convertimos el byte a hexadecimal y lo agregamos a la cadena UUID
    }
    Serial.println(uuid); // Imprimimos el UUID en el monitor serial

    lcd.clear(); // Limpiamos el contenido del LCD
    lcd.setCursor(0, 0); // Colocamos el cursor en la primera fila del LCD
    lcd.print("UUID:"); // Mostramos la etiqueta "UUID:"
    lcd.setCursor(0, 1); // Colocamos el cursor en la segunda fila del LCD
    lcd.print(uuid); // Mostramos el UUID en el LCD

    mfrc522.PICC_HaltA(); // Detenemos la comunicación con la tarjeta RFID
    mfrc522.PCD_StopCrypto1(); // Detenemos la encriptación
  }
  delay(1000); // Esperamos 1 segundo antes de realizar la próxima lectura
}
