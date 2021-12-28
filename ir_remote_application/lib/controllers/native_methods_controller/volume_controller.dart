import 'package:flutter/services.dart';

import 'base_native_controller.dart';

abstract class VolumeController implements MethodChannelBase {
  VolumeController._();
  static const MethodChannel _channel = MethodChannel('native_channel');
  static const String methodName = 'volume';
  static Future<dynamic> setTo(int volume) async {
    return await _channel.invokeMethod('$methodName:$volume');
  }
}
