

//raw[|latex| \clearpage ]


== ビルド

=== arduino-builder


Arduino1.6.8以降、スケッチのビルドは@<tt>{arudino-builder} のコマンドに
委譲して行われる。



この、arduino-builderはarduinoのビルド設定である@<tt>{platform.txt}の
設定からコンパイル、リンクのコマンドライン引数を決定して、
ビルドを行う機能を持つ。
従来、IDEで抱えていた機能を単体プログラムに分離したものでもあり、



コマンドラインからスケッチのビルドが可能となった


=== Contikiのビルドシステム


Contikiのビルドシステムは既に紹介したように、
Makefileをベースに作られている。
arduino-builderはarduinoの管理下のソースのビルドに特化しているため、
Contikiの部分のビルドには使えない、Contikiのビルドシステムで
Arduino部分のソースをビルドする必要がある。



arduinoとしてビルド対象となるファイルはarduino-builderが知っているが、
これを何らかの方法でMakefileに反映する必要がある。


=== contiki-makehelper


この目的のために、arduino-builderから実行されるコンパイル実行の情報を
収集する、contiki-makehelperを導入する。
方法としては、gccの代わりにコンパイル時にこのプログラムを実行し、
ビルド時に与えられるコンパイルオプションを収集する。
情報が全て収集できたところで、Makefileを生成して、makeコマンドを
実行する。


=== ライブラリのスキャン


arduino-builderはスケッチのビルドの際、ヘッダでインクルードしているライブラリを
コンパイル対象に含める動作を行っている。
が、この動作、実はかなりの力技である。



すなわち、インクルードが解決するまで、ライブラリのフォルダを
片っ端からインクルード対象にして探し回って、エラーが無くなったところで
探索を終了する、所謂総当たり方式で検索を行っている。

