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

## 構成の変更

プロジェクトテンプレートをベースに、SPAを任意のURLへの配置したり、アセンブリ名を変更したりするには、幾つかの変更が必要と思われます。その際にポイントとなるであろう点を列挙します（具体的には未検証）。

- アセンブリ名や既定の名前空間は、`$(MSBuildProjectName)`、`{$(MSBuildProjectName.Replace(" ", "_"))`と設定。

- Razorコンポーネントの先頭は大文字である必要がある。

### App.razor（ルートテンプレート）

Routerコンポーネントが使われています。コンパイルされ`<ProjectNamespace>.dll`内に、FQCN：`<ProjectNamespace>.App`として実装されます。

```xml
<Router AppAssembly="@typeof(App).Assembly">
```

アプリが起動すると、AppAssemblyに指定されたアセンブリがスキャンされ、RouteAttribute （`@page`指定により付与）を持つアプリのコンポーネントのルート情報が収集されます。

- [ルートテンプレート](https://learn.microsoft.com/ja-jp/aspnet/core/blazor/fundamentals/routing?view=aspnetcore-7.0#route-templates)

### index.html

wwwroot/index.htmlになります。生成コードでは、`wwwroot/`が`アプリケーションルート：http://localhost/`にマップされます。

さらに以下のようにhrefにアプリのベースパスを指定する必要があります。

```html
<base href="/" />
```

- [アプリのベースパス](https://learn.microsoft.com/ja-jp/aspnet/core/blazor/host-and-deploy/?view=aspnetcore-7.0&tabs=visual-studio#app-base-path)
- 末部に/が必須
- app.UsePathBase

### ルーティング

生成コードではPages/*.razorファイルの中で`@page`ディレクティブで設定されています。

```razor
# In Pages/Index.razor
@page "/"

# In Pages/FetchData.razor
@page "/fetchdata"

#その他も同様
```

但し、上記全てのPages/*.razorは一つのSPAとなるので、ナビゲーションにある/fetchdataをクリックして画面遷移しても/fetchdataに対するHTTPリクエストは発行されません。

## NavLink

Shared/NavMenu.razor内でも利用しているNavLink要素はURL生成に関係しています。

```html
<NavLink class="nav-link" href="/" Match="NavLinkMatch.All">
    <span class="oi oi-home" aria-hidden="true"></span> Home
</NavLink>
```

- [NavLink および NavMenu コンポーネント](https://learn.microsoft.com/ja-jp/aspnet/core/blazor/fundamentals/routing?view=aspnetcore-7.0#navlink-and-navmenu-components)
