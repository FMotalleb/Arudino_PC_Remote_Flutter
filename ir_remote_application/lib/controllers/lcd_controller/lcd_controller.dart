import 'package:ir_remote_application/controllers/serial_port_controller/serial_port_controller.dart';

class LcdController {
  SerialPortController serialPortController;
  void printToLcd(String text) {
    serialPortController.write("lcd:$text");
  }

  LcdController(
    this.serialPortController,
  );
}
