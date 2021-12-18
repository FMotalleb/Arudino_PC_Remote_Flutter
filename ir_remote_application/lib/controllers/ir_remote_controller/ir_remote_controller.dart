import 'package:hemend/object_controllers/listenables/listenable_object.dart';
import 'package:ir_remote_application/controllers/native_methods_controller/keyboard_controller.dart';
import 'package:ir_remote_application/controllers/native_methods_controller/terminal_controller.dart';

enum RemoteKey {
  plus200,
  plus100,
  num9,
  num8,
  num7,
  num6,
  num5,
  num4,
  num3,
  num2,
  num1,
  num0,
  equalizer,
  volUp,
  volDown,
  mediaPlay,
  forward,
  backward,
  channelUp,
  channel,
  channelDown,
  repeat,
  none,
}

class IRRemoteController with ListenableObject {
  RemoteKey _lastCalledKey = RemoteKey.none;
  RemoteKey get currentAction => _lastCalledKey;

  static const Map<int, RemoteKey> keyMapping = {
    62209: RemoteKey.plus200,
    59137: RemoteKey.plus100,
    46593: RemoteKey.num9,
    44545: RemoteKey.num8,
    48641: RemoteKey.num7,
    42497: RemoteKey.num6,
    58369: RemoteKey.num5,
    63489: RemoteKey.num4,
    41473: RemoteKey.num3,
    59393: RemoteKey.num2,
    62465: RemoteKey.num1,
    59905: RemoteKey.num0,
    63233: RemoteKey.equalizer,
    60161: RemoteKey.volUp,
    63745: RemoteKey.volDown,
    48385: RemoteKey.mediaPlay,
    49153: RemoteKey.forward,
    48129: RemoteKey.backward,
    47361: RemoteKey.channelUp,
    47617: RemoteKey.channel,
    47873: RemoteKey.channelDown,
    0: RemoteKey.repeat,
    -1: RemoteKey.none,
  };

  void handleReceiver(String code) {
    final keyCode = int.parse(code);
    if (!keyMapping.keys.contains(keyCode)) {
      return;
    }
    RemoteKey action = keyMapping[keyCode]!;
    if (action != RemoteKey.repeat && action != RemoteKey.none) {
      _lastCalledKey = action;
    }
    callThemAll();
  }

  void handleRemoteAction(IRRemoteController controller) {
    switch (controller.currentAction) {
      case RemoteKey.plus200:
        // TODO: Handle this case.
        break;
      case RemoteKey.plus100:
        // TODO: Handle this case.
        break;
      case RemoteKey.num9:
        // TODO: Handle this case.
        break;
      case RemoteKey.num8:
        // TODO: Handle this case.
        break;
      case RemoteKey.num7:
        // TODO: Handle this case.
        break;
      case RemoteKey.num6:
        // TODO: Handle this case.
        break;
      case RemoteKey.num5:
        // TODO: Handle this case.
        break;
      case RemoteKey.num4:
        // TODO: Handle this case.
        break;
      case RemoteKey.num3:
        // TODO: Handle this case.
        break;
      case RemoteKey.num2:
        // TODO: Handle this case.
        break;
      case RemoteKey.num1:
        // TODO: Handle this case.
        break;
      case RemoteKey.num0:
        break;
      case RemoteKey.equalizer:
        Terminal.run('shutdown /a');
        break;
      case RemoteKey.volUp:
        KeyboardController.sendKey(VirtualKeyboardKeys.VOLUME_UP);
        break;
      case RemoteKey.volDown:
        KeyboardController.sendKey(VirtualKeyboardKeys.VOLUME_DOWN);
        break;
      case RemoteKey.mediaPlay:
        KeyboardController.sendKey(VirtualKeyboardKeys.MEDIA_PLAY_PAUSE);
        break;
      case RemoteKey.forward:
        KeyboardController.sendKey(VirtualKeyboardKeys.MEDIA_NEXT_TRACK);
        break;
      case RemoteKey.backward:
        KeyboardController.sendKey(VirtualKeyboardKeys.MEDIA_PREV_TRACK);
        break;
      case RemoteKey.channelUp:
        Terminal.run(
            'powercfg /setactive 9e681d4a-08ae-4329-8af9-91f69c1cd0f7');
        break;
      case RemoteKey.channel:
        Terminal.run('shutdown -s -t 60');
        break;
      case RemoteKey.channelDown:
        Terminal.run(
            'powercfg.exe /setactive fc5f74eb-21c9-41b4-8e1d-db27be202c5f');
        break;
      case RemoteKey.repeat:
        break;
      case RemoteKey.none:
        break;
    }
  }
}
