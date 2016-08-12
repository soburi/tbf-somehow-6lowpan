
LinuxにおけるIEEE802.15.4ドライバ・インターフェース
=====================================================
C90に向けた追補としての個人的なメモ


IEEE802.15.4概況
----------------

LinuxにおけるIEEE802.15.4の実装はメインラインにすでにマージが完了している。
6LoWPANに関連するいくつかのRFCについての未実装部分も順次対応が進められている。

http://events.linuxfoundation.org/sites/events/files/slides/ELC-US-2015.pdf
(https://github.com/linux-wpan/linux-wpan-next)

RPLのルーティング実装も linux-rpl、unstrung, simpleRPLなどの実装が存在している。

wpan-tools
----------

wpan-toolsに含まれるiwpanでIEEE802.15.4のドライバ層に対して設定の書き込み・読み出しを行う
ことができる。
IEEE802.15.4に固有なツールはこれだけである。


ネットワークデバイスの起動
-------------------------

デバイス自体はwpan0として見えてくる。
6LoWPANの中間層として、`ip link` でlowpan0と関連付ける。

```
ifconfig wpan0 up 
ip link add link wpan0 name lowpan0 type lowpan
```


ドライバ・インターフェース
--------------------------




