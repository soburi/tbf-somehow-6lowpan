Arduino interoperable layer on Contiki
==================

本章では、拙作の Arduino *In*teroperable layer on Con*tiki*
(以下Intiki)と、
それを使って6LoWPANを使った通信をArduinoのAPIベースで扱う
手法を紹介する。


Intikiの概要
------------

Intikiは、Contikiのシステム上で動作するArduino APIの互換実装である。
また、Arduino互換APIとContikiの本体、ArduinoのIDEで記述されたスケッチとを
あわせてビルドするためのMakefile生成ツールを含んで、Arduinoのパッケージとして
提供する。
現状では開発中であるため、完成度はまだまだ、ということを了承いただきたい。


Intikiのインストール
--------------------

IntikiはArduinoのボードマネージャからインストールできる
パッケージとして配布している。
Unofficialなパッケージなので、環境設定からURLを指定する。
`ファイル` → `環境設定` のダイアログの、
`追加のボードマネージャのURL`の欄に、以下のURLを追加する。

```
http://soburi.github.io/JN516x-arduino-package/package_soburi_intiki_index.json
```

設定を行い、`ツール` → `ボード:...` → `ボードマネージャ...` を
選択すると、ダイアログのリストの下の方に `JN516x Boards` の項目が現れる。
この項目を選択し、インストールを実行する。

初回のコンパイル時にパッケージの一部をダウンロードする仕掛けになっている
ので、初回のビルド時には若干時間がかかる。




