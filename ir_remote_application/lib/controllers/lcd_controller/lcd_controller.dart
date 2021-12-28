import 'package:ir_remote_application/controllers/serial_port_controller/serial_port_controller.dart';

class LcdController {
  SerialPortController serialPortController;
  void printToLcd(String text) {
    bool isRaw = text.startsWith('raw:');
    if (isRaw) {
      serialPortController.write(text.replaceFirst('raw:', ''));
    } else {
      serialPortController.write("lcd:$text");
    }
  }

  LcdController(
    this.serialPortController,
  );
}
