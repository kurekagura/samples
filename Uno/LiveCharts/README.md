# 環境

```
*******************************************************
** Visual Studio 2022 Developer Command Prompt v17.3.3
** Copyright (c) 2022 Microsoft Corporation
*******************************************************

C:\Program Files\Microsoft Visual Studio\2022\Community>wsl -l -v
  NAME                   STATE           VERSION
* Ubuntu-22.04           Running         2
```

以下をWindowsにインストール。
- vcxsrv-64.1.20.14.0.installer.exe
- gtk3-runtime-3.24.31-2022-01-04-ts-win64.exe(不要だったかも)

# VS2022プロジェクト作成手順

プロジェクトテンプレート：[Uno Platform App]


以下、３つを選択。

- WinUI (Windows App SDK)
- Gtk (Linux, macOS, Windows 7 or later)
- WPF (Windows7 or later)

# launchSettings.json

```
{
  "profiles": {
    "App1.Skia.Gtk": {
      "commandName": "Project"
    },
    "WSL": {
      "commandName": "WSL2",
      "distributionName": "Ubuntu-22.04",
      "environmentVariables": {
        "DISPLAY": "xxx.xxx.x.x:0"
      }
    }
  }
}
```

# 参考
