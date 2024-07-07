#include "UART_Software.h"
void setup() {

  uart_init();
  uart_write("Hello World!\n");
}

void loop() {
  uint8_t receivedData = uart_readByte();
  uart_write("\nReceived: ");
  uart_writeByte(receivedData);
}
