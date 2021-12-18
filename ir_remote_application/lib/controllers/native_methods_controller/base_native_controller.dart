import 'package:flutter/services.dart';

abstract class MethodChannelBase {
  MethodChannelBase._();
  static const MethodChannel _channel = MethodChannel('native_channel');
  static const String methodName = '';
}
