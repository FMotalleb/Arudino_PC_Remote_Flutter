import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';

import 'package:android_control_panel/components/models/delayed_sync/delayed_sync.dart';
import 'package:android_control_panel/components/widgets/colored_check_box.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:hemend/ui_related/pre_built_widgets/message_box.dart';

class ChatPage extends StatefulWidget {
  final BluetoothDevice server;

  const ChatPage({required this.server});

  @override
  _ChatPage createState() => new _ChatPage();
}

class _Message {
  int whom;
  String text;

  _Message(this.whom, this.text);
}

class _ChatPage extends State<ChatPage> {
  static final clientID = 0;
  BluetoothConnection? connection;

  List<_Message> messages = List<_Message>.empty(growable: true);
  String _messageBuffer = '';

  final TextEditingController textEditingController =
      new TextEditingController();
  final ScrollController listScrollController = new ScrollController();

  bool isConnecting = true;
  bool get isConnected => (connection?.isConnected ?? false);

  bool isDisconnecting = false;

  @override
  void initState() {
    super.initState();

    BluetoothConnection.toAddress(widget.server.address).then((_connection) {
      print('Connected to the device');
      connection = _connection;
      setState(() {
        isConnecting = false;
        isDisconnecting = false;
      });

      connection!.input!.listen(_onDataReceived).onDone(() {
        // Example: Detect which side closed the connection
        // There should be `isDisconnecting` flag to show are we are (locally)
        // in middle of disconnecting process, should be set before calling
        // `dispose`, `finish` or `close`, which all causes to disconnect.
        // If we except the disconnection, `onDone` should be fired as result.
        // If we didn't except this (no flag set), it means closing by remote.
        if (isDisconnecting) {
          print('Disconnecting locally!');
        } else {
          print('Disconnected remotely!');
        }
        if (this.mounted) {
          setState(() {});
        }
      });
    }).catchError((error) {
      print('Cannot connect, exception occured');
      print(error);
    });
  }

  @override
  void dispose() {
    // Avoid memory leak (`setState` after dispose) and disconnect
    if (isConnected) {
      isDisconnecting = true;
      connection?.dispose();
      connection = null;
    }

    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final List<Row> list = messages.map((_message) {
      final isSender = _message.whom == clientID;
      return Row(
        children: <Widget>[
          CustomPaint(
            painter: MessageBoxSegment(
                const Radius.circular(15), const Size(17, 17),
                painter: Paint()..color = const Color(0xff447a9c),
                sharpPointOffset: const Offset(2, 5),
                sharpPointAtEnd: isSender),
            child: Container(
              child: Text(
                  (text) {
                    return text == '/shrug' ? '¯\\_(ツ)_/¯' : text;
                  }(_message.text.trim()),
                  style: const TextStyle(color: Colors.white)),
              padding: const EdgeInsets.all(12.0),
              margin: const EdgeInsets.only(bottom: 8.0, left: 8.0, right: 8.0),
              width: 222.0,
              // decoration: BoxDecoration(
              //     color: _message.whom == clientID
              //         ? Colors.blueAccent
              //         : Colors.grey,
              //     borderRadius: BorderRadius.circular(7.0)),
            ),
          ),
        ],
        mainAxisAlignment:
            isSender ? MainAxisAlignment.end : MainAxisAlignment.start,
      );
    }).toList();

    final serverName = widget.server.name ?? "Unknown";
    return Scaffold(
      appBar: AppBar(
          title: (isConnecting
              ? Text('Connecting chat to ' + serverName + '...')
              : isConnected
                  ? Text('Live chat with ' + serverName)
                  : Text('Chat log with ' + serverName))),
      body: SafeArea(
        child: Column(
          children: <Widget>[
            Flexible(
              child: ListView.separated(
                  padding: const EdgeInsets.all(12.0),
                  controller: listScrollController,
                  itemBuilder: (_, i) => list[i],
                  separatorBuilder: (_, __) => Container(
                      height: 2,
                      width: 100,
                      margin: const EdgeInsets.all(3.0),
                      decoration: const BoxDecoration(
                          borderRadius: BorderRadius.all(Radius.circular(25)),
                          color: Color(0x66858585))),
                  itemCount: messages.length),
            ),
            const SizedBox(height: 5, width: 1),
            RGBHandler(
              messageHandler: _sendMessage,
            ),
            const SizedBox(height: 5, width: 1),
            SizedBox(
              width: 50,
              height: 50,
              child: LightKey(
                messageHandler: _sendMessage,
              ),
            ),
            Row(
              children: <Widget>[
                Flexible(
                  child: Container(
                    margin: const EdgeInsets.only(left: 16.0),
                    child: TextField(
                      style: const TextStyle(fontSize: 15.0),
                      controller: textEditingController,
                      decoration: InputDecoration.collapsed(
                        hintText: isConnecting
                            ? 'Wait until connected...'
                            : isConnected
                                ? 'Type your message...'
                                : 'Chat got disconnected',
                        hintStyle: const TextStyle(color: Colors.grey),
                      ),
                      enabled: isConnected,
                    ),
                  ),
                ),
                Container(
                  margin: const EdgeInsets.all(8.0),
                  child: IconButton(
                      icon: const Icon(Icons.send),
                      onPressed: isConnected
                          ? () => _sendMessage(textEditingController.text)
                          : null),
                ),
              ],
            )
          ],
        ),
      ),
    );
  }

  void _onDataReceived(Uint8List data) {
    // Allocate buffer for parsed data
    int backspacesCounter = 0;
    data.forEach((byte) {
      if (byte == 8 || byte == 127) {
        backspacesCounter++;
      }
    });
    Uint8List buffer = Uint8List(data.length - backspacesCounter);
    int bufferIndex = buffer.length;

    // Apply backspace control character
    backspacesCounter = 0;
    for (int i = data.length - 1; i >= 0; i--) {
      if (data[i] == 8 || data[i] == 127) {
        backspacesCounter++;
      } else {
        if (backspacesCounter > 0) {
          backspacesCounter--;
        } else {
          buffer[--bufferIndex] = data[i];
        }
      }
    }

    // Create message if there is new line character
    String dataString = String.fromCharCodes(buffer);
    int index = buffer.indexOf(13);
    if (~index != 0) {
      setState(() {
        messages.add(
          _Message(
            1,
            backspacesCounter > 0
                ? _messageBuffer.substring(
                    0, _messageBuffer.length - backspacesCounter)
                : _messageBuffer + dataString.substring(0, index),
          ),
        );
        _messageBuffer = dataString.substring(index);
      });
    } else {
      _messageBuffer = (backspacesCounter > 0
          ? _messageBuffer.substring(
              0, _messageBuffer.length - backspacesCounter)
          : _messageBuffer + dataString);
    }
  }

  void _sendMessage(String text) async {
    text = text.trim();
    textEditingController.clear();

    if (text.length > 0) {
      try {
        connection!.output.add(Uint8List.fromList(utf8.encode(text + "\r\n")));
        await connection!.output.allSent;

        setState(() {
          messages.add(_Message(clientID, text));
        });

        Future.delayed(const Duration(milliseconds: 333)).then((_) {
          listScrollController.animateTo(
              listScrollController.position.maxScrollExtent,
              duration: const Duration(milliseconds: 333),
              curve: Curves.easeOut);
        });
      } catch (e) {
        // Ignore error, but notify state
        setState(() {});
      }
    }
  }
}

enum RelayLightState { on, off }

extension RelayLightStateParser on RelayLightState {
  String getCommandFor(String relayName) {
    String result = "";
    switch (this) {
      case RelayLightState.on:
        result = "on";
        break;
      case RelayLightState.off:
        result = "off";
        break;
    }
    return "$relayName:$result";
  }

  RelayLightState get nextState {
    switch (this) {
      case RelayLightState.on:
        return RelayLightState.off;

      case RelayLightState.off:
        return RelayLightState.on;
    }
  }

  Color get mainThemeColor {
    switch (this) {
      case RelayLightState.on:
        return const Color(0xFFFFF240);

      case RelayLightState.off:
        return const Color(0xFF131313);
    }
  }

  Color get secondaryThemeColor {
    switch (this) {
      case RelayLightState.on:
        return const Color(0xFF131313);

      case RelayLightState.off:
        return const Color(0xFFDBDBDB);
    }
  }

  IconData get icon {
    switch (this) {
      case RelayLightState.on:
        return Icons.lightbulb;
      case RelayLightState.off:
        return Icons.lightbulb_outline;
    }
  }
}

class LightKey extends StatefulWidget {
  const LightKey(
      {Key? key,
      required this.messageHandler,
      this.initialState = RelayLightState.off})
      : super(key: key);
  final void Function(String) messageHandler;
  final RelayLightState initialState;

  @override
  _LightKeyState createState() => _LightKeyState();
}

class _LightKeyState extends State<LightKey> {
  RelayLightState _state = RelayLightState.off;

  RelayLightState get state => _state;

  set state(RelayLightState state) {
    setState(() {
      _state = state;
    });
    widget.messageHandler(_state.getCommandFor('relay'));
  }

  @override
  void initState() {
    super.initState();
    state = widget.initialState;
  }

  @override
  Widget build(BuildContext context) {
    return InkWell(
      borderRadius: BorderRadius.circular(9999999),
      onTap: () => state = state.nextState,
      child: AnimatedContainer(
        duration: const Duration(milliseconds: 450),
        decoration: BoxDecoration(
          shape: BoxShape.circle,
          color: state.secondaryThemeColor,
        ),
        child: Icon(state.icon, color: state.mainThemeColor),
      ),
    );
  }
}

class RGBHandler extends StatefulWidget {
  const RGBHandler({Key? key, required this.messageHandler}) : super(key: key);

  final void Function(String) messageHandler;
  @override
  _RGBHandlerState createState() => _RGBHandlerState();
}

class _RGBHandlerState extends State<RGBHandler> {
  Color _color = const Color(0xff000000);
  late DelayedSync syncer = DelayedSync(
      caller: (data) async => widget.messageHandler(data),
      delay: const Duration(seconds: 1));
  Color get color => _color;

  set color(Color color) {
    setState(() => _color = color);
    syncer.pushString(
        "rgb:${_color.red * 4},${_color.green * 4},${_color.blue * 4}");
  }

  void update() {
    widget.messageHandler(
        'rgb:${red ? activationValue : 0},${green ? activationValue : 0},${blue ? activationValue : 0}');
  }

  bool _red = false;

  bool get red => _red;

  set red(bool red) {
    _red = red;
    update();
  }

  bool _blue = false;

  bool get blue => _blue;

  set blue(bool blue) {
    _blue = blue;
    update();
  }

  bool _green = false;

  bool get green => _green;

  set green(bool green) {
    _green = green;
    update();
  }

  static const activationValue = 750;
  @override
  Widget build(BuildContext context) {
    return SizedBox(
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: [
          const SizedBox(width: 50),
          ColoredCheckBox(
            activeBoxColor: Colors.red,
            deActiveBoxColor: Colors.red,
            onChanged: (value) => red = value,
          ),
          const SizedBox(),
          ColoredCheckBox(
            activeBoxColor: Colors.green,
            deActiveBoxColor: Colors.green,
            onChanged: (value) => green = value,
          ),
          const SizedBox(),
          ColoredCheckBox(
            activeBoxColor: Colors.blue,
            deActiveBoxColor: Colors.blue,
            onChanged: (value) => blue = value,
          ),
          const SizedBox(width: 50),
          // Slider(
          //     activeColor: Colors.red,
          //     value: _color.red.toDouble(),
          //     max: 255,
          //     onChanged: (value) => color = _color.withRed(value.toInt())),
          // Slider(
          //     activeColor: Colors.green,
          //     value: _color.green.toDouble(),
          //     max: 255,
          //     onChanged: (value) => color = _color.withGreen(value.toInt())),
          // Slider(
          //     activeColor: Colors.blue,
          //     value: _color.blue.toDouble(),
          //     max: 255,
          //     onChanged: (value) => color = _color.withBlue(value.toInt())),
        ],
      ),
    );
  }
}
