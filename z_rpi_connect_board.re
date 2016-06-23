
= RaspberryPi 用 TWE-Lite接続ボード

== RaspberryPi と TWE-Liteの接続


Contikiおよぼ6LBRでは、ノードとはUART経由で接続する。
RaspberryPiからは、USB-Serial変換(FT232)を介して接続することも
できるが、拡張コネクタから出ているUARTを利用する
HAT(RaspberryPiの拡張ボード)を作成することで、、ケーブルを
引きまわさない、コンパクトな接続形態にできる。



最低限結線しなければならない端子として、UARTのTX,RX,
イメージ焼き込みのため、MISOとRESET端子をRPi側のGPIOに結線する。



この基板では、利便性のためDIO2,3をLEDに、DIO8をスイッチに接続している。
これは、NXPのDR1174評価ボードと同じ配置となっている。



スイッチを必要とするサンプルのビルド時に、@<tt>{JN516x_WITH_DR1174=1}
を指定することで、対応することができる。



//image[twe_on_rpi_sch_part][RaspberryPi TWE-Lite 接続基板 回路図(抜粋)]{
//}


//table[tbl1][]{
番号	部品名	スペック
-----------------
C1	コンデンサ	0.1μF 0603サイズ
D1	LED	0603サイズ
D2	LED	0603サイズ
D3	ショットキーバリアダイオード	BAT43XV2
D4	ショットキーバリアダイオード	BAT43XV2
P1	ピンソケット	2x13
R1	抵抗	10KΩ 0603サイズ
R2	抵抗	10KΩ 0603サイズ
R3	抵抗	10KΩ 0603サイズ
R4	抵抗	680Ω 0603サイズ
R5	抵抗	680Ω 0603サイズ
SW1	タクトスイッチ	ALPS SKRPACE010
SW2	タクトスイッチ	ALPS SKRPACE010
SW3	タクトスイッチ	ALPS SKRPACE010
U1	表面実装 TWE-Lite	SMD uFLアンテナタイプ
//}

== 焼き込みプログラム


@<href>{https://github.com/soburi/jenprog,私家版jenprog}を使うことで、
RPiのGPIOを使って焼き込みモードへ遷移させて、プログラムの焼き込みを行うことができる。


//emlist{
sudo jenprog/jenprog -c serial -t /dev/ttyS0 --rpi --spimiso-pin=9 \
--reset-pin=23 --verbose -p slip-radio.jn516x.bin
//}

== Raspbianのシリアルコンソール機能の抑止


Raspbianの初期設定では、拡張コネクタのUARTから、シリアルコンソールとして
接続できるようになっている。
この設定が有効だと、シリアルコンソール(getty)にUARTを占有されるので、
無効にする。@<tt>{raspi-config} でこの設定が行えるので、


//emlist{
sudo raspi-config
//}


としてraspi-configを起動して、@<tt>{9 Advanced Options} より @<tt>{A7 Serial} を選択する。


//emlist{
Would you like a login shell to be accessible over serial?
//}


と聞かれるので、@<tt>{No}を選択する。

