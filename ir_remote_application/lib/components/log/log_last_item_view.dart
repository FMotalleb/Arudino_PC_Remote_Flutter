import 'package:flutter/material.dart';
import 'package:hemend/object_controllers/binder/bindable_object.dart';
import 'package:ir_remote_application/models/bindable_string.dart';

class LastItemView extends StatefulWidget {
  const LastItemView({
    Key? key,
    required this.boundedItem,
  }) : super(key: key);
  final BindableString boundedItem;
  @override
  _LastItemViewState createState() => _LastItemViewState();
}

class _LastItemViewState extends BindableState<LastItemView, BindableString> {
  @override
  Widget build(BuildContext context) {
    return Container(
      child: Text("last serial input : ${bondedObject.text}"),
    );
  }

  @override
  BindableString get bondedObject => widget.boundedItem;
}
