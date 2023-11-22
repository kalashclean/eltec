#include <mcp2515.h>

#include <SPI.h>
struct can_frame canMsg;
char sentData = 0;
char receivedData = 0;
MCP2515 mcp2515(10);
void setup() {
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS);
  mcp2515.setNormalMode();
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);  // Set clock speed to 1MHz
  pinMode(10, OUTPUT);                  // Set SS pin as output
  Serial.begin(9600);                   // Start serial communication
}

void loop() {
  digitalWrite(10, LOW);  // Select the slave
uint8_t readStatus = mcp2515.readMessage(&canMsg);
Serial.print(readStatus+":");
Serial.print(canMsg.can_id,HEX);
Serial.print("_");

Serial.print(canMsg.can_dlc,HEX);

  receivedData = SPI.transfer(0x00);  // Send 0 to the slave and receive response
  Serial.println();
  Serial.print("Received: ");
  Serial.print(receivedData, HEX);
  Serial.println();

  delay(1000);


  digitalWrite(10, HIGH);  // Deselect the slave
}
