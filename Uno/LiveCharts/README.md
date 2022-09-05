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

# LiveCharts

## LiveChartsのインストール

「ソリューションのNugetパッケージの管理」から３つプロジェクトに対して `LiveChartsCore.SkiaSharpView.Uno.WinUI 2.0.0-beta.400`をNuget。

GtkとWpfでビルドエラー。
```
重大度レベル	コード	説明	プロジェクト	ファイル	行	抑制状態
エラー	MSB3025	ソース ファイル
 "C:\Users\hoge\.nuget\packages\skiasharp.nativeassets.webassembly\2.88.1\build\netstandard1.0\libSkiaSharp.a" はディレクトリです。
 "Copy" タスクはディレクトリのコピーをサポートしません。
 	App1.Skia.Gtk	C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\amd64\Microsoft.Common.CurrentVersion.targets	5128	
```

WORKAROUND:

GtkとWpfに対して個別に `LiveChartsCore.SkiaSharpView.Uno.WinUI 2.0.0-beta.361` へバージョンダウン。

## WorkAround for Gtk

WSL実行時にエラー。
```
InvalidOperationException:
The version of the native libSkiaSharp library (88.0) is incompatible with this version of SkiaSharp.
Supported versions of the native libSkiaSharp library are in the range [88.1, 89.0).
```

`SkiaSharp.NativeAssets.Linux 2.88.1` をNuget（以下の情報が表示されるので確認後にOK）。
```
Platforms\App1.Skia.Gtk
更新:
SkiaSharp.2.88.1-preview.91 -> SkiaSharp.2.88.1
SkiaSharp.NativeAssets.Linux.2.88.1-preview.79 -> SkiaSharp.NativeAssets.Linux.2.88.1
SkiaSharp.NativeAssets.macOS.2.88.1-preview.91 -> SkiaSharp.NativeAssets.macOS.2.88.1
SkiaSharp.NativeAssets.Win32.2.88.1-preview.91 -> SkiaSharp.NativeAssets.Win32.2.88.1
```
⇒ 動作OK

## WorkAround for Wpf

ビルド・実行はできるがウィンドウに何も表示されない。起動したウィンドウをリサイズすると表示される。

# 参考

- [2022/5/12 - LiveCharts Announces Support for Uno Platform](https://platform.uno/blog/livecharts-announces-support-for-uno-platform/)
