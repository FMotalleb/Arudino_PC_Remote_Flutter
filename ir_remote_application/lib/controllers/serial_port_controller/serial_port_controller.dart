import 'package:flutter_libserialport/flutter_libserialport.dart';
import 'package:ir_remote_application/controllers/ir_remote_controller/ir_remote_controller.dart';
import 'package:ir_remote_application/models/bindable_string.dart';

class SerialPortController {
  final IRRemoteController irRemoteController = IRRemoteController();
  BindableString lastInput = BindableString();
  SerialPortController() {
    irRemoteController.addListener(
        () => irRemoteController.handleRemoteAction(irRemoteController));

    final port = SerialPort('COM3');
    if (!port.openReadWrite()) {}
    final reader = SerialPortReader(port);
    reader.stream.listen(
      (data) {
        String serialData = String.fromCharCodes(data);
        lastInput.text = serialData;
        if (serialData.startsWith('IRRemote')) {
          irRemoteController
              .handleReceiver(serialData.replaceFirst('IRRemote:', ''));
        }
      },
    );
  }
}
