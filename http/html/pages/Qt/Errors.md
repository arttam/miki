## Errors and solutions for Qt (Linux)

### Could not find XCB

Typiceal error message
```bash
qt.qpa.plugin: Could not find the Qt platform plugin "xcb" in ""
This application failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix this problem.
```

Remedy
```bash
# 1. Find where Qt plugins are
#    In my case, with Qt as Conan dependency

╭─art@pop-os ~/Projects/ZoneSense/release/bin
╰─➤  find /home/art/Archive/.conan/data/qt/5.12.3/mirriad/stable-9.3/package -name plugins
/home/art/Archive/.conan/data/qt/5.12.3/mirriad/stable-9.3/package/8173599d590ab2d0c81f09d49cdcaa23eb15945a/plugins

# 2. Export that path as QT_PLUGIN_PATH
╭─art@pop-os ~/Projects/ZoneSense/release/bin
╰─➤  export QT_PLUGIN_PATH=/home/art/Archive/.conan/data/qt/5.12.3/mirriad/stable-9.3/package/8173599d590ab2d0c81f09d49cdcaa23eb15945a/plugins

# 3. All works from now on
...
```

