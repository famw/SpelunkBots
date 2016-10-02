# Troubleshooting

In case of any problems running it locally, try to do the things described here.

## 1. Create a xvfb display

Make sure you have `xvfb` installed, then, create a new screen:

```bash
Xvfb :1 -screen 0 1024x720x24
```

## 2. Create a vnc server for xvfb display

Make sure you have `x11vnc` installed, then, run:

```bash
x11vnc -display :1 -localhost
```

## 3. Run wine in the virtual display

```bash
DISPLAY=:1 wine /path/to/app.exe
```

## 4. View results

Open a remote connection to `vnc://localhost:5900` and watch the application
running.

> See: https://help.ubuntu.com/community/VNC/Clients. But I think that using
> the one that comes with the distro is enough.
