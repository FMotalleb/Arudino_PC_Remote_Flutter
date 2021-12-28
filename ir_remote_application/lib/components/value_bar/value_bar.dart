import 'package:flutter/material.dart';
import 'package:ir_remote_application/controllers/native_methods_controller/volume_controller.dart';
import 'package:ir_remote_application/controllers/serial_port_controller/serial_port_controller.dart';

class ValueBar extends StatefulWidget {
  const ValueBar({Key? key, required this.receiver}) : super(key: key);
  final ValueReceiver<int> receiver;
  @override
  _ValueBarState createState() => _ValueBarState();
}

class _ValueBarState extends State<ValueBar> {
  ValueReceiver<int> get receiver => widget.receiver;
  Future<void> update() async {
    setState(() {});

    final test = await VolumeController.setTo(receiver.value);

    return;
  }

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    receiver.addListener(update);
  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();
    receiver.removeListener(update);
  }

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      width: 200,
      child: LinearProgressIndicator(
        value: receiver.value / 100,
      ),
    );
  }
}
