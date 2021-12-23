import 'package:flutter/material.dart';
import 'package:ir_remote_application/controllers/lcd_controller/lcd_controller.dart';

class SerialOutput extends StatelessWidget {
  const SerialOutput({Key? key, required this.lcdController}) : super(key: key);
  final LcdController lcdController;
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
          ),
        ),
        ElevatedButton(
            onPressed: () {
              lcdController.printToLcd(controller.text);
            },
            child: const Text('emit'))
      ],
    );
  }
}
