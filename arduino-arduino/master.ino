#include <SPI.h>

char sentData = 0;
char receivedData = 0;

void setup() {
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8); // Set clock speed to 1MHz
  pinMode(10, OUTPUT); // Set SS pin as output
  Serial.begin(9600); // Start serial communication
}

void loop() {
  digitalWrite(10, LOW); // Select the slave
  
  sentData = 'H';
  receivedData = SPI.transfer(sentData); // Send 'H' to the slave and receive response
  Serial.print("Sent: ");
  Serial.println(sentData);
  Serial.print("Received: ");
  Serial.println(receivedData);
  
  delay(1000);
  
  sentData = 'L';
  receivedData = SPI.transfer(sentData); // Send 'L' to the slave and receive response
  Serial.print("Sent: ");
  Serial.println(sentData);
  Serial.print("Received: ");
  Serial.println(receivedData);
  
  delay(1000);
  
  digitalWrite(10, HIGH); // Deselect the slave
}
