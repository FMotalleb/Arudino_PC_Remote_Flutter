import 'dart:async';

import 'package:android_control_panel/components/mixins/socket_caller/string_caller.dart';

class DelayedSync with StringCaller {
  DelayedSync({required this.caller, required this.delay}) {
    _timer = Timer(delay, () => this(_fallBackString));
  }
  void pushString(String data) {
    _fallBackString = data;
  }

  String _fallBackString = '';
  late Timer _timer;
  Future<void> call(String data) async {
    if (_fallBackString != '') {
      await caller(_fallBackString);
      _fallBackString = '';
    }

    _timer = Timer(delay, () => this(_fallBackString));
  }

  @override
  final Future<void> Function(String p1) caller;
  final Duration delay;
}
