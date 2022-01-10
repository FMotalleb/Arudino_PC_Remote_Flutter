mixin StringCaller implements _StringCallerBase {
  Future<void> callWith(String data) async {
    await caller(data);
  }
}

abstract class _StringCallerBase {
  _StringCallerBase._(this.caller);
  final Future<void> Function(String) caller;
}
