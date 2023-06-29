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

※類似の[Uno Platform App(Xamarin|UWP)]との違いは？

以下、２つを選択。
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

# vcXsrv設定

XLaunch起動時に、以下のx を選択。
```
x Multiple windows
x Display number -1

x Start no client

x Clipboard
 x Primary Selection
- Native opengl（チェックしたらダメ） 
x Disable access control
```

# トラブル1

## Gtk版の画面が起動できない

エラー
```
(App1.Skia.Gtk:70): Gtk-WARNING **: 01:24:12.596: cannot open display: :0
```

1. xxx.xxx.x.x:0"のことろは":0"ではNG。必ず自ホストのIPアドレスを指定する必要がある。
1. vcXsrvの設定で、[Native opengl]のチェックを外すとFixできた。

参考4のF/W設定は不要だったかもしれない。vcXsrv (XLaunch) 起動時の警告時にプライベートのみ許可すればOKと思われる。いずれにしてもポート番号 6000 が通っておけば良いと思われる。

## トラブル2

WSLを再起したら壊れた。但し、PC再起動で直った。
```
wsl -l
wsl -t Ubuntu-22.04
```

# コマンドメモ

```
sudo apt update
sudo apt install libgtk-3-dev mesa-utils libgl1-mesa-glx

sudo apt-get update
sudo apt-get install -y apt-transport-https

sudo apt-get install -y dotnet-sdk-6.0

wget https://packages.microsoft.com/config/ubuntu/22.04/packages-microsoft-prod.deb -O packages-microsoft-prod.deb
sudo dpkg -i packages-microsoft-prod.deb
rm packages-microsoft-prod.deb
```

# 参考

1. [Get Started on Visual Studio 2022](https://platform.uno/docs/articles/get-started-vs-2022.html?tabs=windows)
2. [Additional setup for Linux or WSL](https://platform.uno/docs/articles/get-started-with-linux.html?tabs=ubuntu1804%2Cwindows)
3. [Ubuntu に .NET SDK または .NET ランタイムをインストールする](https://docs.microsoft.com/ja-jp/dotnet/core/install/linux-ubuntu#2204)
4. [Running an X server with WSL2](https://skeptric.com/wsl2-xserver/)

# Nugetしたもの

Uno.WinUI

```xml
<Page.Resources>
  <Style TargetType="Button">
    <Setter Property="HorizontalAlignment" Value="Stretch" />
    <Setter Property="VerticalAlignment" Value="Stretch" />
  </Style>
</Page.Resources>
```