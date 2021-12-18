// ignore_for_file: constant_identifier_names

import 'package:flutter/services.dart';

import 'base_native_controller.dart';

abstract class KeyboardController implements MethodChannelBase {
  KeyboardController._();
  static const MethodChannel _channel = MethodChannel('native_channel');
  static const String methodName = 'virtualKeyboard';
  static Future<dynamic> sendKey(int key) async {
    return await _channel.invokeMethod('$methodName:$key');
  }
}

abstract class VirtualKeyboardKeys {
  VirtualKeyboardKeys._();
  static const int VOLUME_MUTE = 0xAD;
  static const int VOLUME_DOWN = 0xAE;
  static const int VOLUME_UP = 0xAF;
  static const int MEDIA_NEXT_TRACK = 0xB0;
  static const int MEDIA_PREV_TRACK = 0xB1;
  static const int MEDIA_STOP = 0xB2;
  static const int MEDIA_PLAY_PAUSE = 0xB3;
  static const int LAUNCH_MAIL = 0xB4;
  static const int LAUNCH_MEDIA_SELECT = 0xB5;
  static const int LAUNCH_APP1 = 0xB6;
  static const int LAUNCH_APP2 = 0xB7;

  static const int LBUTTON = 0x01;
  static const int RBUTTON = 0x02;
  static const int CANCEL = 0x03;
  static const int MBUTTON = 0x04;
  static const int XBUTTON1 = 0x05;
  static const int XBUTTON2 = 0x06;
  static const int BACK = 0x08;
  static const int TAB = 0x09;
  static const int CLEAR = 0x0C;
  static const int RETURN = 0x0D;
  static const int SHIFT = 0x10;
  static const int CONTROL = 0x11;
  static const int MENU = 0x12;
  static const int PAUSE = 0x13;
  static const int CAPITAL = 0x14;
  static const int KANA = 0x15;
  static const int HANGEUL = 0x15;
  static const int HANGUL = 0x15;
  static const int JUNJA = 0x17;
  static const int FINAL = 0x18;
  static const int HANJA = 0x19;
  static const int KANJI = 0x19;
  static const int ESCAPE = 0x1B;
  static const int CONVERT = 0x1C;
  static const int NONCONVERT = 0x1D;
  static const int ACCEPT = 0x1E;
  static const int MODECHANGE = 0x1F;
  static const int SPACE = 0x20;
  static const int PRIOR = 0x21;
  static const int NEXT = 0x22;
  static const int END = 0x23;
  static const int HOME = 0x24;
  static const int LEFT = 0x25;
  static const int UP = 0x26;
  static const int RIGHT = 0x27;
  static const int DOWN = 0x28;
  static const int SELECT = 0x29;
  static const int PRINT = 0x2A;
  static const int EXECUTE = 0x2B;
  static const int SNAPSHOT = 0x2C;
  static const int INSERT = 0x2D;
  static const int DELETE = 0x2E;
  static const int HELP = 0x2F;
  static const int LWIN = 0x5B;
  static const int RWIN = 0x5C;
  static const int APPS = 0x5D;
  static const int SLEEP = 0x5F;
  static const int NUMPAD0 = 0x60;
  static const int NUMPAD1 = 0x61;
  static const int NUMPAD2 = 0x62;
  static const int NUMPAD3 = 0x63;
  static const int NUMPAD4 = 0x64;
  static const int NUMPAD5 = 0x65;
  static const int NUMPAD6 = 0x66;
  static const int NUMPAD7 = 0x67;
  static const int NUMPAD8 = 0x68;
  static const int NUMPAD9 = 0x69;
  static const int MULTIPLY = 0x6A;
  static const int ADD = 0x6B;
  static const int SEPARATOR = 0x6C;
  static const int SUBTRACT = 0x6D;
  static const int DECIMAL = 0x6E;
  static const int DIVIDE = 0x6F;
  static const int F1 = 0x70;
  static const int F2 = 0x71;
  static const int F3 = 0x72;
  static const int F4 = 0x73;
  static const int F5 = 0x74;
  static const int F6 = 0x75;
  static const int F7 = 0x76;
  static const int F8 = 0x77;
  static const int F9 = 0x78;
  static const int F10 = 0x79;
  static const int F11 = 0x7A;
  static const int F12 = 0x7B;
  static const int F13 = 0x7C;
  static const int F14 = 0x7D;
  static const int F15 = 0x7E;
  static const int F16 = 0x7F;
  static const int F17 = 0x80;
  static const int F18 = 0x81;
  static const int F19 = 0x82;
  static const int F20 = 0x83;
  static const int F21 = 0x84;
  static const int F22 = 0x85;
  static const int F23 = 0x86;
  static const int F24 = 0x87;
}
