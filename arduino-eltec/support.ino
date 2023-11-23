//in this codei try to read the value from two different ways, by using SPI.transfer and by using the MCP2515 library, without success
 
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
  digitalWrite(10, HIGH);  // Deselect the slave
}
 
void loop() {
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(10, LOW);  // Select the slave
  delay(100);
  uint8_t dummyReceive;
  dummyReceive = SPI.transfer(0x03);  // Read Register
  delay(100);
  dummyReceive = SPI.transfer(0x0F);  // Addr CANCTRL
  delay(100);
  receivedData = SPI.transfer(0x00);  // Read Byte (should be 0x87)
  delay(100);
  digitalWrite(10, HIGH);  // Deselect the slave
  SPI.endTransaction();
  Serial.print(receivedData,HEX);

}
