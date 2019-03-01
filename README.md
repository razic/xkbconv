# xkbconv

## requirements

```bash
apt-get install -y  libxkbcommon-dev libxkbcommon-x11-dev
```

## build from source

```
make
```

## usage

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
