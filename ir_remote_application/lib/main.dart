import 'dart:io';

import 'package:flutter/material.dart';
import 'package:ir_remote_application/components/log/log_last_item_view.dart';
import 'package:ir_remote_application/components/serial_output/serial_output.dart';
import 'package:ir_remote_application/controllers/lcd_controller/lcd_controller.dart';
import 'package:ir_remote_application/controllers/native_methods_controller/keyboard_controller.dart';
import 'components/value_bar/value_bar.dart';
import 'controllers/serial_port_controller/serial_port_controller.dart';

SerialPortController serialController = SerialPortController();
late LcdController lcdController = LcdController(serialController);
void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key, required this.title}) : super(key: key);
  final String title;
  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  int _counter = 0;
  bool _backlight = true;

  bool get backlight => _backlight;

  set backlight(bool backlight) {
    _backlight = backlight;
    serialController.write('lcd:backLight:${_backlight ? "on" : "off"}');
  }

  void _incrementCounter() {
    KeyboardController.sendKey(VirtualKeyboardKeys.VOLUME_MUTE);
    setState(() {
      _counter++;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            LastItemView(boundedItem: serialController.lastInput),
            FittedBox(
              child: SerialOutput(
                lcdController: lcdController,
              ),
            ),
            ValueBar(
              receiver: serialController.valueController,
            )
          ],
        ),
      ),
      floatingActionButtonLocation: FloatingActionButtonLocation.endFloat,
      floatingActionButton: FloatingActionButton(
        foregroundColor:
            (!backlight) ? const Color(0xfffefefe) : const Color(0xff1e1e1e),
        backgroundColor:
            backlight ? const Color(0xfffefefe) : const Color(0xff1e1e1e),
        onPressed: () => setState(() => backlight = !backlight),
        tooltip: 'set LCD backlight to ${_backlight ? "off" : "on"}',
        child: Icon(backlight ? Icons.lightbulb : Icons.lightbulb_outline),
      ), // This trailing comma makes auto-formatting nicer for build methods.
    );
  }
}
