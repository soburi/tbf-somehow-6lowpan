
= 6LBR


 @<b>{本稿執筆の時点で、本章の内容に不備があることをお詫び申し上げます}


== IPと6LoWPAN


Internetとは、Internet Protocolの名のとおり、IPで通信している
ネットワークである。
Internet(=The Net)と他のネットワークを接続するには、
IPの通信と相互に中継する必要がある。



注意しなければならないのは、6LoWPANのプロトコルはそもそもIPv6ではなく、
Border-Routerの内側、つまりIPv6との
ギャップが少なくなるように設計されたプロトコルであるということだ。



すなわち、6LoWPANはIPv6との相互運用運用性が高くなるように設計
されており、透過的に扱える局面も多い。が、本質的に別の体系の
通信であり、相互に接続するには何等かの変換が必要となる。



それを行うのが@<b>{Border-Router}である。


== 6LBRの概要


ContikiOSの派生プロジェクトで6LBRがある。
@<b>{6L}oWPAN @<b>{B}order @<b>{R}outer で、6LoWPANとIPの中継を行う
Border Routerのパッケージである。



本章では、6LBRを使って、6LoWPANとIPネットワークの相互接続の実験を行う。


== ルーター接続


6LBRは設定でいくつかのネットワーク構成に対応するが、ここでは代表的な
ルーター接続を取り上げる。ルーター接続はLayer3、すなわちIPv6の層で通信を
中継する。IPv6の層の通信を中継するので、中継する元と先は異なるサブネット
となる。


=== LAN側


LAN側はIPv6の通信が可能であればOKである。ネットワークの形態も特に問わないが、
たいていの場合はEthernetかWiFiになるだろう。IPv6の設定を行う必要がある


=== PAN側の接続


6LBRは6LoWPAN側へのインターフェースとしてContikiに含まれるslip-radioの
ソフトウェアを組み込んだ802.15.4の端末を使い、シリアル接続で通信を仲介する。
slip-radioは802.15.4の通信をほぼそのままSLIP(Serial Line IP)に回送する
モジュールである。
ノード(TWE-Lite)側でSLIPのパケットに乗せられたIPv6の通信を6LoWPANに
変換して、中継する。

//table[tbl1][]{
@<b>{LAN側}	@<b>{6LBR}	.	@<b>{Node(TWE-Lite)}
-----------------
IPv6	.	IPv6	6LoWPAN
802.3 MAC or 802.11 MAC	.	SLIP	802.15.4 MAC
Ethernet   or WiFi	.	UART	802.15.4 PHY
//}


6LBRそのものはContikiのnativeターゲット向けにビルドしたバイナリ、
すなわちLinuxのアプリである。
これとslip-radioが通信して、6LoWPANのスタックをLinux上で動作させている。


== 6LBRのインストール


https://github.com/cetic/6lbr/wiki



6LBRのソースはContikiのフォークとなっている。
本体は、@<tt>{examples/6lbr} にある cetic-6lbr であるが、
6LoWPANのプロトコルスタックにも手が加えられている。



Contikiのビルドの仕組みに乗っかっているので、
インストール手順



https://github.com/cetic/6lbr/wiki/Other-Linux-Software-Configuration



に従ってLinux上で普通にmakeを実行すればよい。


//emlist{
# ビルド依存パッケージ
apt-get install libncurses5-dev bridge-utils git

# submoduleを更新
git clone https://github.com/cetic/6lbr
cd 6lbr
git submodule update --init --recursive

# build
cd examples/6lbr
make all
make plugins
make tools

# install
sudo make install
sudo make plugins-install
//}


最近のdebianはsystemdの設定が必要なので、
@<tt>{/etc/systemd/system/6lbr.service} として
設定ファイルを作る。


//emlist{
[unit]
Description = 6lbr daemon

[Service]
ExecStart = /usr/bin/6lbr
Restart = always
Type = simple

[Install]
wantedBy = multi-user.target
//}

== 6LBRの設定


6LBRは、@<tt>{/etc/6lbr/}配下の設定ファイルと、
WEB-UIで設定を行う。



メインの設定ファイルは @<tt>{/etc/6lbr/6lbr.conf}である。
このファイルで、使用するネットワークインターフェースや、
SLIP-Radioの接続先を指定する。
いくつか設定例が入っているが、ここではRaspberry Pi向けの設定例 
@<tt>{/etc/6lbr/6lbr.conf.rpi} の内容で設定されているものとして話を進める。
この6lbr.confとは別に、バイナリ形式の設定ファイルがある。
こちらは主にWEB-UIでの設定の保存に使われる。
これの中身は、


//emlist{
/usr/lib/6lbr/bin/nvm_tool --print /etc/6lbr/nvm.dat
//}


のように補助コマンドの@<tt>{nvm_tool}で見ることが出来る。
このコマンドで出力で、Ethernet側のIPアドレスを


//emlist{
Eth default router : bbbb::1
//}


のように指定している。これに合わせて、Ethernet側にv6アドレスを設定する。



@<tt>{/etc/6lbr/6lbr.conf} に、


//emlist{
BRIDGE=1
//}


のようにブリッジ有効の設定があると、仮想ブリッジを作るので、
br0がv6アドレスの設定の対象である。ipコマンドでnvm.datに設定されている
v6アドレスを設定する。


//emlist{
sudo ip -6 addr bbbb::1/64 dev br0
//}


ここまでできたら、


//emlist{
sudo service 6lbr start
//}


として、6LBRを立ち上げる。
httpサーバーが動いているので、 @<tt>{http://[bbbb::1]/} にアクセスすると、
WEB-UIから状態の確認と設定が出来る。



WSN側とEthernet側が認識されていることを確認する。

