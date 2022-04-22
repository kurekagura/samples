1.MyWindow1

QThreadを使ったサンプル。

2.MyWindow2

std::threadを使ったサンプル。

3.MyWindow3

条件変数，キュー（QueueChannel）を使ったサンプル。
ImageCaptureThreadはひたすらキューに画像を投入。
ImageProcessorThreadはひたすらキューから画像を取り出し、GUIに投げる。

[Memo]
Qt Creator 4.11.0 (Community)で、ウィンドウを自動フィットに設定する方法
window.uiというxmlファイルであるが、直接編集するのではなく、デザイナからエディットする。
MyWindowのgeometryを100*100など、表示画像より小さく設定しておく。
デフォルトで作成されているmenubarとstatusbarを削除
Label(QLabel)を配置、objectNameをimageLabelへ、scaledContentsをONへ変更する。
centralwidgetのレイアウトを「垂直に並べる」（横棒アイコン）へ変更する。

Build Instruction

1. CMakeLists.txtをQt Creator(IDE)で開く
2. OpenCV_DIR(e.x,,c:/lib/opencv-4.5.5)を追加。
　CMakeCache変数。.userに保存される。
3. TOOLCHAIN


実行時の環境変数
　→「ビルド時の環境変数」から「環境変数なし」へ変更
　→PATH環境変数が例えば以下のようになるよう変更

C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\bin;C:\lib\opencv-4.5.5a\x64\vc16\bin;C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.5\bin;C:\gstreamer\1.0\msvc_x86_64\bin

※Qtのパスは既定で存在する。