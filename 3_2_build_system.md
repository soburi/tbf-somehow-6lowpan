
ContikiOSのフォルダ構成
-----------------------

ContikiOSのソースはかなり*散らかっている*。
主要なフォルダ構成を理解すると扱いやすい。

: apps
  examplesから参照される。単体ではあまり使わない。
: core
  OSのソース
: core/net
  プロトコルスタックのソース
: core/sys
  ContikiOSのプロセス管理やタイマー機能のソース
: cpu
  CPU依存のソース
: examples
  サンプル類。
: platform
  ボード依存のソース
: platform/jn516x
  JN516x(TWE-Lite)関連のソース。JN516xはcpuのフォルダにソースを入れていない
: tools
  ホストPC側で使うツール類。SLIP接続のツールなどが入っている


Makefileの構造
------------

* `$(CONTIKI_PROJECT)/Makefile`

プロジェクト固有のMakefile。

守らなければならないお約束として、変数 `CONTIKI` にcontikiのソースが
格納されているディレクトリを設定すること、
Makefileの最後に 

```
include $(CONTIKI)/Makefile
```

として、Contikiの共通設定を読み込むようにすること。
この2点を設定すれば、最小のMakefileとなる。
この場合、`$(CONTIKI_PROJECT)/$(CONTIKI_PROJECT).c` の1ファイルのみがコンパイル対象となる。

プロジェクトのオプションとして、
`PROJECT_SOURCEFILES`と`PROJECTDIRS`が指定できる。

`PROJECT_SOURCEFILES`は追加のソースファイル、
`PROJECTDIRS`は追加のインクルードディレクトリを指定する。

Makefileは特にアーキテクチャ依存がなければ、全アーキテクチャ共用となる。
ビルド時にmakeコマンド実行時に引数で、変数 `TARGET` にビルド対象のボードを指定することで、
ビルド対象を切り替える。


* `contiki/Makefile.include`

基本定義のMakefile
各アプリのMakefileでアプリ固有の定義を書いて、
最後でこれをincludeする。
外部から指定可能なオプションとして、

  - `CONTIKI_WITH_IPV4`
  - `CONTIKI_WITH_IPV6`
  - `CONTIKI_WITH_RIME`

がある。これで使用するネットワークプロトコルを指定する。
IPv6の場合はさらに

  - `CONTIKI_WITH_RPL`
 
で RPLの要否を設定できる。

`CFLAGS`に `'-D'` を追加するための仕組みとして `DEFINES` が使用できる。
`-D`に続くマクロ定義を記載することで、 コンパイル時に`-D`オプションとして`DEFINES`で
指定したマクロ定義が反映される。




* `$(CONTIKI)/platform/$(TARGET)/Makefile.$(TARGET)`
* `$(CONTIKI)/cpu/$(CONTIKI_CPU)/Makefile.`**CPUNAME**

ボード・CPU固有の設定が定義されたMakefile。

`contiki/Makefile.include`から`TARGET`変数に応じてplatform配下の
ターゲットボードのMakefileが呼ばれる。

ターゲットボードのMakefileの中からさらに、
搭載されているCPUに対応するMakefileを
`$(CONTIKI)/cpu`配下から読み込む。






