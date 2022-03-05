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

centralwidgetのsizePolicyを[Expanding,Expanding,0,0]とする。
MyWindowとimageLabelのsizePolicyを[Preferred,Preferred,0,0]へ設定したいのだが、
