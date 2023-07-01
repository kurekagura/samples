# 「.NET MAUI Blazor アプリ」

VS2022で作成しました。「.NET MAUI アプリ」とは別のプロジェクトテンプレートです。仕組み的には「.NET MAUI アプリ」をベースに自分でBlazorアプリの構成を組むこともできます。

## 概要

`.NET MAUI アプリ`のContentPageコントロール上（MainPage.xaml）に`BlazorWebView`コントロールをホストしたHybrid形態のアプリです。`BlazorWebView`は所謂ウェブビュー（ウェブブラウザ）で、この中ではHTML/CSS/JavaScript、またBlzaor技術が利用できます（ホスト側はXAMLですが、中味はXAMLではなくHTMLです）。以下の`blazor.webview.js`のおかげでBlazorが利用できるというだけのものです（最終的にはJavaScript/WebAssemblyということになります）。

```xml
<script src="_framework/blazor.webview.js" autostart="false"></script>
```

Windows/MacCatalyst/Android/iOS/Tizenに対してクロスプラットフォームビルドすることができます（Platformsフォルダ参照）。

## csproj

```xml
<Project Sdk="Microsoft.NET.Sdk.Razor">
```

[csproj](./MauiBlazorApp1/MauiBlazorApp1.csproj)

## 発行

配布パッケージは .msix、.appxのため、ASP.NET Core等の通常のアプリとは異なるウィザードが起動します。
