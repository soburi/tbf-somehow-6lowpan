
== ContikiOSのフォルダ構成


ContikiOSのソースはかなり@<b>{散らかっている}。
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



contikiを使ってアプリ開発をする場合は@<tt>{examples}、contikiへのアーキテクチャ移植は
@<tt>{platform}, @<tt>{cpu}、contikiのOS部分、ネットワークを触るには@<tt>{core}あたりを見て行けばよい。


== Makefileの構造
 * @<tt>{$(CONTIKI_PROJECT)/Makefile}


プロジェクト固有のMakefile。



守らなければならないお約束として、変数 @<tt>{CONTIKI} にcontikiのソースが
格納されているディレクトリを設定すること、
Makefileの最後に 


//emlist{
include $(CONTIKI)/Makefile
//}


として、Contikiの共通設定を読み込むようにすること。
この2点を設定すれば、最小のMakefileとなる。
この場合、@<tt>{$(CONTIKI_PROJECT)/$(CONTIKI_PROJECT).c} の1ファイルのみがコンパイル対象となる。



プロジェクトのオプションとして、



: PROJECT_SOURCEFILES
  追加のソースファイルを指定
: PROJECTDIRS
  追加のインクルードディレクトリを指定



が設定可能である。



Makefileは特にアーキテクチャ依存がなければ、全アーキテクチャ共用となる。
ビルド時にmakeコマンド実行時に引数で、変数 @<tt>{TARGET} にビルド対象のボードを指定することで、
ビルド対象を切り替える。

 * @<tt>{contiki/Makefile.include}


基本定義のMakefile
各アプリのMakefileでアプリ固有の定義を書いて、
最後でこれをincludeする。
外部から指定可能なオプションとして、

 * @<tt>{CONTIKI_WITH_IPV4}
 * @<tt>{CONTIKI_WITH_IPV6}
 * @<tt>{CONTIKI_WITH_RIME}


がある。これで使用するネットワークプロトコルを指定する。
IPv6の場合はさらに

 * @<tt>{CONTIKI_WITH_RPL}


で RPLの要否を設定できる。



@<tt>{CFLAGS}に @<tt>{'-D'} を追加するための仕組みとして @<tt>{DEFINES} が使用できる。
@<tt>{-D}に続くマクロ定義を記載することで、 コンパイル時に@<tt>{-D}オプションとして@<tt>{DEFINES}で
指定したマクロ定義が反映される。

 * @<tt>{$(CONTIKI)/platform/$(TARGET)/Makefile.$(TARGET)}
 * @<tt>{$(CONTIKI)/cpu/$(CONTIKI_CPU)/Makefile.}@<strong>{CPUNAME}


ボード・CPU固有の設定が定義されたMakefile。



@<tt>{contiki/Makefile.include}から@<tt>{TARGET}変数に応じてplatform配下の
ターゲットボードのMakefileが呼ばれる。



ターゲットボードのMakefileの中からさらに、
搭載されているCPUに対応するMakefileを
@<tt>{$(CONTIKI)/cpu}配下から読み込んでいる。



JN516xの場合もプラットフォーム固有の設定がある。



: CHIP
  JN516xシリーズのうちのどれかを指定する。TWE-Liteの場合はJN5164を指定する
: TARGET_WITH_UART0, TARGET_WITH_UART1
  それぞれUART0,1を使う場合は1に。
: JENNIC_PCB, JENNIC_STACK , JENNIC_MAC
  NXPのSDKのMakefileに由来するパラメータ。触らなくてよい。
: DISABLE_LTO
  Link Time Optimizationの有効無効の切り替え。これもNXPのSDKのMakefileに由来
: DEBUG
  SWを指定するとUARTにデバッグメッセージを出す。HWはJTAGらしいが... これもNXPのSDKのMakefileに由来
: JN516x_WITH_DR1174, JN516x_WITH_DR1175, JN516x_WITH_DR1199
   NXPの販売している評価ボードの名前。原則使うことはないが、
   ボタンの設定が必要なサンプルがあるので、そういった場合に、
   @<tt>{JN516x\_WITH\_DR1174} を指定する

