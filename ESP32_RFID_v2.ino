// Usar este programa con la libreria RFID_MFRC522v2 by GithubCommunity
// que se instala desde la IDE de Arduino

// el esquema de conexion esta en ESP32_RFID522.png


#include <MFRC522v2.h>                // Librería principal para el manejo del lector RFID MFRC522.
#include <MFRC522DriverSPI.h>         // Librería para el manejo del lector RFID a través del protocolo SPI.
//#include <MFRC522DriverI2C.h>       // Librería para manejar el lector RFID a través de I2C (comentada porque se usa SPI en este ejemplo).
#include <MFRC522DriverPinSimple.h>   // Librería para manejar el pin de selección del lector RFID.
#include <MFRC522Debug.h>             // Librería para imprimir información de depuración.

MFRC522DriverPinSimple ss_pin(5);     // Configura el pin de selección del lector RFID en el pin 5.
MFRC522DriverSPI driver{ss_pin};      // Crea un controlador SPI usando el pin de selección anterior.
// MFRC522DriverI2C driver{};         // Crea un controlador I2C, comentado porque usamos SPI.
MFRC522 mfrc522{driver};              // Crea una instancia del lector MFRC522 usando el controlador SPI.

String tagContent = "";               // Variable para almacenar el contenido del ID de la tarjeta.

void setup()
{
  Serial.begin(115200); // Inicializa la comunicación serial con la PC para depuración.
  
  while (!Serial) {
    ; // Espera hasta que el puerto serial esté disponible (solo necesario en algunos Arduinos como los basados en ATMEGA32U4).
  }
  
  mfrc522.PCD_Init(); // Inicializa el lector RFID.
  
  // Muestra los detalles del lector RFID en la consola serial.
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial); 
  
  // Mensaje de bienvenida indicando que el lector está listo para escanear tarjetas.
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop()
{
  // Verifica si hay una nueva tarjeta presente en el lector.
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return; // Si no hay tarjeta, sale del loop y vuelve a comprobar en la siguiente iteración.
  }

  // Intenta leer el serial de la tarjeta seleccionada.
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return; // Si no se puede leer la tarjeta, sale del loop y vuelve a intentar en la siguiente iteración.
  }

  // Muestra información detallada de la tarjeta en la consola serial.
  MFRC522Debug::PICC_DumpToSerial(mfrc522, Serial, &(mfrc522.uid));

  // Imprime el UID de la tarjeta en formato hexadecimal.
  Serial.println("Card ID:");
  Serial.println("--------------------------");
  
  // Recorre cada byte del UID de la tarjeta y lo convierte a formato hexadecimal.
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    tagContent.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ")); // Agrega un "0" adicional si el byte es menor de 16 (0x10) para formato.
    tagContent.concat(String(mfrc522.uid.uidByte[i], HEX)); // Agrega el byte actual en formato hexadecimal.
  }

  tagContent.toUpperCase(); // Convierte el contenido del UID a mayúsculas para legibilidad.

  Serial.println(tagContent); // Imprime el UID completo de la tarjeta en la consola serial.
}
