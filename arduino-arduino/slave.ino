#include <SPI.h>

char receivedData = 0;
char responseData = 0;

void setup() {
  pinMode(MISO, OUTPUT); // Set MISO as OUTPUT
  SPCR |= _BV(SPE); // Enable SPI
}

void loop() {
  if (SPSR & _BV(SPIF)) { // Check if SPI data has been received
    receivedData = SPDR; // Get the received data
    
    // Respond based on received data
    if (receivedData == 'H') {
      responseData = 'A'; // Respond with 'A' when 'H' is received
    } else if (receivedData == 'L') {
      responseData = 'B'; // Respond with 'B' when 'L' is received
    }
    
    SPDR = responseData; // Send the response data
  }
}
