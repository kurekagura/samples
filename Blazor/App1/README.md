# App1.sln（App1.Client+App1.Server）

VS2022で作成したが以下と等価と思われます。但し、生成コードのnamespace等が微妙な差異が生じていました。

```dos
dotnet new blazorwasm -ho -au None --use-program-main --no-https -o App2
```

[blazorwasm](https://learn.microsoft.com/ja-jp/dotnet/core/tools/dotnet-new-sdk-templates#blazorwasm)

## 概要

bootstrap.cssが利用されている

クライアントの実装[App1.Client](../App1/Client/App1.Client.csproj)はコンパイルされ、DLL（App1.Client.dll）として生成される。

クライアントの[Program.cs](./Client/Program.cs)の`builder.RootComponents.Add<App1.Client.App>("#app");`の型パラメータが[App.razor](./Client/App.razor)ビルド後のFQCNです。`#app`は[wwwroot/index.html](./Client/wwwroot/index.html)のdiv要素で、このUI領域がSPA（App1.Client）に置き換わり、実行されるイメージです。

## MSBuildファイル

```xml
<Project Sdk="Microsoft.NET.Sdk.BlazorWebAssembly">
```

VSの出力種類は「コンソールアプリケーション」として表示されていますが、関係がないようです。

## WebServerとの関係と「発行」

App1.Server.csprojからApp1.Client.csprojが参照されています。これによって、App1.Serverから「発行」を行うとApp1.Client.dllやwwwrootフォルダなどのクライアント（SPA関連）モジュールも含めて指定の発行フォルダへコピーできます。

また、App1.Clientプロジェクトから「発行」を行うとApp1.Client単体の発行も行うことができます。

【メモ】以上は、VSプロジェクトテンプレートのケースです。複数クライアントのケースでプロジェクトをどのように構成するかについて、要検討かもしれません。

## LibMAN

jsライブラリをコマンドラインから管理できます（CDNからのインストールや構成ファイル`libman.json`からの復元など）。

libman.jsonが存在するディレクトリで以下のコマンドで復元できます。

```dos
libman restore
```

Nugetパッケージ：`Microsoft.Web.LibraryManager.Build`を利用することで、ビルド時に復元を実行することもできます。

- [ライブラリ ファイルを復元する](https://learn.microsoft.com/ja-jp/aspnet/core/client-side/libman/libman-cli?view=aspnetcore-7.0#restore-library-files)
- [
ビルド時にファイルを復元する](https://learn.microsoft.com/ja-jp/aspnet/core/client-side/libman/libman-vs?view=aspnetcore-7.0#restore-files-during-build)

