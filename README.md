# xkbconv

Bit of a strange, albeit useful tool.

This is likely only useful if your software-based keyboard layout is something
other than QWERTY, and want to configure Yubikey's [static password features].

## Building

Install dependencies:

* libxkbcommon

Run this command:

```
make
```

There will be an executable in the current working directory.

## Usage

You supply three things:

* Source keyboard context
* Destination keyboard context
* Text

The source, and destination keyboard contexts, are virtual keyboards. They
represent the model, layout, variant, and options belonging to a keyboard. If
you're unsure about what these mean, please refer to the [xkbcommon]
documentation. The text should be limited to the ASCII character set, and
passed on STDIN.

```
xkbconv \
  --src-model pc105 \
  --src-layout us \
  --src-variant dvorak \
  --src-options '' \
  --dst-model pc105 \
  --dst-layout us \
  --dst-variant basic \
  --dst-options ''
  < file.txt
```

For each character in the input text, the program will find the corresponding
[scancode] in the source keyboard context, and output the ASCII character with
the same keycode in the destination keyboard context.

[static password features]: https://support.yubico.com/support/solutions/articles/15000006480-understanding-core-static-password-features
[scancodes]: https://en.wikipedia.org/wiki/Scancode
[xkbcommon]: https://xkbcommon.org/doc/current/
