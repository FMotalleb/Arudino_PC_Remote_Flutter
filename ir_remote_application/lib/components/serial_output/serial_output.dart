import 'package:flutter/material.dart';
import 'package:ir_remote_application/controllers/lcd_controller/lcd_controller.dart';

extension stringTool on Iterable<String> {
  String asString() {
    final StringBuffer buffer = StringBuffer();
    forEach((element) {
      buffer.write(element);
    });
    return buffer.toString();
  }
}

class SerialOutput extends StatelessWidget {
  const SerialOutput({Key? key, required this.lcdController}) : super(key: key);
  final LcdController lcdController;

  void emit(String data) {
    if (!data.contains(":") && data.length > 15) {
      final splintedData = data.split('');
      lcdController.printToLcd(splintedData.getRange(0, 16).asString() +
          ':' +
          splintedData.getRange(16, splintedData.length - 1).asString());
    } else {
      lcdController.printToLcd(data);
    }
  }

  @override
  Widget build(BuildContext context) {
    final TextEditingController controller = TextEditingController();

    return Row(
      children: [
        SizedBox(
          width: 450,
          height: 50,
          child: TextField(
            controller: controller,
            onSubmitted: emit,
          ),
        ),
        ElevatedButton(
            onPressed: () => emit(controller.text), child: const Text('emit'))
      ],
    );
  }
}
