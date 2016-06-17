
== 'hello-world'の動作確認


'hello-world'のサンプルをTWE-Lite向けにビルドして、ContikiOSの動作確認を行う。


=== JN516xのビルド環境


TWE-Lite向けに@<b>{JN516xのビルド環境}をセットアップする。



NXPのサイトからJN516x向けの開発環境、SDKなどをダウンロードしてインストールする。
ここではインストール先はデフォルトのままとする。

 * @<href>{http://cache.nxp.com/documents/other/JN-SW-4141.zip,BeyondStudio for NXP}
 * @<href>{http://cache.nxp.com/documents/other/JN-SW-4163.zip,JN516x IEEE 802.15.4 SDK}
 * @<href>{http://cache.nxp.com/documents/other/JN-SW-4107.zip,JN51xx Producton Flash Programmer}


コマンドプロンプトでパスを設定する。
contikiのビルドにはmakeコマンドが必要なので、開発環境に入っているmsysのフォルダにパスを通しておく。


//emlist{
set PATH=%PATH%;C:\NXP\bstudio_nxp\msys\bin
set PATH=%PATH%;C:\NXP\bstudio_nxp\sdk\Tools\ba-elf-ba2-r36379\bin
set PATH=%PATH%;C:\NXP\ProductionFlashProgrammer
//}


Makefileの内部でリリース日を取得するためにgitコマンドを呼んでいるが、
インストールした開発環境にこれは含まれていない。
@<tt>{RELSTR}の環境変数が設定されていれば、gitコマンドを呼ばずにこちらを使うので、
環境変数を設定する。バージョン表示の文字列なので、設定しなくとも特に影響はない。


//emlist{
set RELSTR="localbuild"
//}

=== ビルド、書き込み


contikiのディレクトリ配下の @<tt>{examples/hello-world} で、


//emlist{
make TARGET=jn516x
//}


を実行する。ビルドが成功すれば、@<tt>{hello-world.jn516x.bin}が生成される。


=== 動作確認


シリアルポートにつないで、


//emlist{
Hello, world
//}


のお約束の表示が出てくれば成功である。

