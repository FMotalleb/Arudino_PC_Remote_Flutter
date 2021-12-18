import 'dart:typed_data';

import 'package:flutter_libserialport/flutter_libserialport.dart';
import 'package:ir_remote_application/controllers/ir_remote_controller/ir_remote_controller.dart';
import 'package:ir_remote_application/models/bindable_string.dart';

class SerialPortController {
  final IRRemoteController irRemoteController = IRRemoteController();
  BindableString lastInput = BindableString();
  final SerialPort _port;
  void write(String text) {
    _port.write(Uint8List.fromList(text.codeUnits));
  }

  SerialPortController([String serialPort = 'COM3'])
      : _port = SerialPort(serialPort) {
    irRemoteController.addListener(
        () => irRemoteController.handleRemoteAction(irRemoteController));

    if (!_port.openReadWrite()) {}
    final reader = SerialPortReader(_port);
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
