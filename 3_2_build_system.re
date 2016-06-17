
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
@<tt>{PROJECT_SOURCEFILES}と@<tt>{PROJECTDIRS}が指定できる。



@<tt>{PROJECT_SOURCEFILES}は追加のソースファイル、
@<tt>{PROJECTDIRS}は追加のインクルードディレクトリを指定する。



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
@<tt>{$(CONTIKI)/cpu}配下から読み込む。

