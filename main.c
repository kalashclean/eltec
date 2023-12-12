#include <furi.h>
#include <furi-hal-spi.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
typedef struct {
    // Define your model for the CAN message data
    char message[32]; // Buffer to hold the message to be displayed
} CanMessageViewModel;
void can_message_view_render(Canvas* canvas, void* model) {
    CanMessageViewModel* view_model = model;
    // Clear the canvas
    canvas_clear(canvas);
    // Set the font to use
    canvas_set_font(canvas, FontSecondary);
    // Add text to the canvas with the CAN message data
    canvas_draw_str(canvas, 0, 0, view_model->message);
}
void on_can_message_received(uint8_t* data, size_t size) {
    CanMessageViewModel view_model;
    // Convert the received CAN message data to a string (assuming ASCII data for simplicity)
    snprintf(view_model.message, sizeof(view_model.message), "CAN Data: %s", data);
    // Request the GUI to redraw the view with the new data
    // You would typically post an event or message to the main loop of your application to do this
    // This is a placeholder to illustrate the process
    gui_redraw(); // This function will differ based on how your application's main loop is structured
}

void mcp2515_spi_init() {
    SpiBusConfig spi_config = {
        .spi_dev = SpiBusDev1,
        .speed = 10000000, // Example speed
        .mode = SpiBusMode0
        //.sclk_gpio = <SCLK_PIN>,
        //.mosi_gpio = <MOSI_PIN>,
        //.miso_gpio = <MISO_PIN>,
        
    };
    spi_bus_init(&spi_config);
}

// Define the READ instruction opcode for MCP2515
#define MCP2515_READ_INSTRUCTION 0x03

// Function to send the READ instruction set to the MCP2515 and read the answer

uint8_t mcp2515_read(uint8_t address) {
    uint8_t read_command = 0x03; // Read command for MCP2515
    uint8_t data;
    const GpioPin CS_PIN = {.port = GPIOA, .pin = LL_GPIO_PIN_4};

    
    spi_bus_acquire_sync();
    spi_bus_cs_select(CS_PIN, true);
    spi_bus_xfer(SpiBusDev1, &read_command, NULL, 1, SpiBusWidth1);
    spi_bus_xfer(SpiBusDev1, &address, NULL, 1, SpiBusWidth1);
    spi_bus_xfer(SpiBusDev1, NULL, &data, 1, SpiBusWidth1);
    spi_bus_cs_select(CS_PIN, false);
    spi_bus_release();

    return data;
}
uint8_t* readRXB0() {
    uint8_t rxb0ctrl = mcp2515_read(0x60); // Read RXB0CTRL
    if(rxb0ctrl & 0x01) { // Check if a message is received
        // Read CAN ID, DLC, Data, etc.
        uint8_t id_high = mcp2515_read(0x61); // First byte of ID
        uint8_t id_low = mcp2515_read(0x62); // Second byte of ID
        uint8_t dlc = mcp2515_read(0x65); // Data Length Code
        size_t size = (size_t)(dlc & 0x0F); // Data Length Code
        uint8_t data[8]; // CAN data bytes (up to 8 bytes)
        for(uint8_t i = 0; i < size; i++) { // Read data bytes based on DLC
            data[i] = mcp2515_read(0x66 + i);
        }
        on_can_message_received(data, size);
        // Process the CAN message...
    }
}

uint8_t* readRXB1() {
    uint8_t rxb1ctrl = mcp2515_read(0x70); // Read RXB1CTRL
    if(rxb1ctrl & 0x01) { // Check if a message is received
        // Similar process as RXB0
    }
    uint8_t id_high = mcp2515_read(0x71); // First byte of ID
        uint8_t id_low = mcp2515_read(0x72); // Second byte of ID
        uint8_t dlc = mcp2515_read(0x75); // Data Length Code
        size_t size = (size_t)(dlc & 0x0F); // Data Length Code
        uint8_t data[8]; // CAN data bytes (up to 8 bytes)
        for(uint8_t i = 0; i < size; i++) { // Read data bytes based on DLC
            data[i] = mcp2515_read(0x76 + i);
        }
        on_can_message_received(data, size);
}

int main() {
    mcp2515_spi_init();
    // Additional MCP2515 setup if necessary

    // Main loop
    while(true) {
        readRXB0()
        readRXB1()
        // Your application logic here
        // Example: send/receive CAN messages
    }
}
