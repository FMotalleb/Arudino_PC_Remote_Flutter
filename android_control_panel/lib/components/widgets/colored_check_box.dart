import 'package:flutter/material.dart';

class ColoredCheckBox extends StatefulWidget {
  const ColoredCheckBox(
      {Key? key,
      this.size = 40,
      this.activeIcon = Icons.check,
      this.deActiveIcon = Icons.close,
      this.activeBoxColor = Colors.black,
      this.deActiveBoxColor = Colors.black,
      this.activeIconColor = Colors.white,
      this.deActiveIconColor = Colors.white,
      required this.onChanged,
      this.initialValue = false,
      this.boxRadius = const Radius.circular(3)})
      : super(key: key);
  final Radius boxRadius;
  final bool initialValue;
  final IconData activeIcon;
  final IconData deActiveIcon;
  final Color activeBoxColor;
  final Color deActiveBoxColor;
  final Color activeIconColor;
  final Color deActiveIconColor;
  final double size;
  final void Function(bool) onChanged;
  @override
  _ColoredCheckBoxState createState() => _ColoredCheckBoxState();
}

class _ColoredCheckBoxState extends State<ColoredCheckBox>
    with SingleTickerProviderStateMixin {
  late final Animation<double> progress;
  late final AnimationController controller;
  @override
  void initState() {
    super.initState();
    controller = AnimationController(
        vsync: this,
        duration: const Duration(milliseconds: 300),
        reverseDuration: const Duration(milliseconds: 300));
    progress = Tween<double>(begin: 0.0, end: 1.0).animate(controller);
  }

  late bool _value = widget.initialValue;

  bool get value => _value;

  set value(bool value) {
    setState(() => _value = value);
    if (value) {
      controller.forward();
    } else {
      controller.reverse();
    }
    widget.onChanged(value);
  }

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: () => value = !value,
      child: AnimatedContainer(
          alignment: Alignment.center,
          duration: const Duration(milliseconds: 300),
          height: widget.size,
          width: widget.size,
          padding: EdgeInsets.all(widget.size / 20),
          decoration: BoxDecoration(
            color: value ? widget.activeBoxColor : widget.deActiveBoxColor,
            borderRadius: BorderRadius.all(widget.boxRadius),
          ),
          child:
              AnimatedIcon(icon: AnimatedIcons.pause_play, progress: progress)
          // Icon(
          //   value ? widget.activeIcon : widget.deActiveIcon,
          //   color: value ? widget.activeIconColor : widget.deActiveIconColor,
          //   size: widget.size / 10 * 9,
          // ),
          ),
    );
  }
}
