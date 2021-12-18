import 'package:flutter/services.dart';

import 'base_native_controller.dart';

abstract class Terminal implements MethodChannelBase {
  Terminal._();
  static const MethodChannel _channel = MethodChannel('native_channel');
  static const String methodName = 'terminal';
  static Future<dynamic> run(String cmd) async {
    return await _channel.invokeMethod('$methodName:$cmd');
  }
}
