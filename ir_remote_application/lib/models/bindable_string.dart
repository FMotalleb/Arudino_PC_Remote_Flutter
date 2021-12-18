import 'package:hemend/object_controllers/binder/bindable_object.dart';

class BindableString extends MultipleBindableObject {
  String _text = '';

  String get text => _text;

  set text(String text) {
    _text = text;
    update();
  }
}
