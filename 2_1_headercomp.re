
== 6LoWPANを繙く


6LoWPANに関連するRFCは



: RFC4944
  @<b>{Transmission of IPv6 Packets over IEEE 802.15.4 Networks}
  いわば6LoWPANの原典. パケット分割と第一世代のヘッダ圧縮を規定している。



: RFC6282
  @<b>{Compression Format for IPv6 Datagrams over IEEE 802.15.4-Based Networks}
  第二世代のヘッダ圧縮IPHCを規定する。IPHCの制定でRFC4944のヘッダ圧縮が非推奨となった。



: RFC6774
  @<b>{Neighbor Discovery Optimization for IPv6 over Low-Power Wireless Personal Area Networks (6LoWPANs)}
  近傍発見についてのアップデート。



本稿では、IPv6ヘッダ圧縮を中心に6LoWPANの概要をなぞっていく。


=== ディスパッチ


6LoWPANのパケットの先頭はディスパッチと呼ばれる領域である。

//table[tbl1][]{
@<b>{Pattern}	@<b>{Header Type}
-----------------
00  xxxxxx	NALP       - Not a LoWPAN frame
01  000001	IPv6       - Uncompressed IPv6 Addresses
01  000010	LOWPAN_HC1 - LOWPAN_HC1 compressed IPv6
01  000011	reserved   - Reserved for future use
....	reserved   - Reserved for future use
01  001111	reserved   - Reserved for future use
01  010000	LOWPAN_BC0 - LOWPAN_BC0 broadcast
01  010001	reserved   - Reserved for future use
....	reserved   - Reserved for future use
01  111110	reserved   - Reserved for future use
01  111111	ESC        - Additional Dispatch byte follows
10  xxxxxx	MESH       - Mesh Header
11  000xxx	FRAG1      - Fragmentation Header (first)
11  001000	reserved   - Reserved for future use
....	reserved   - Reserved for future use
11  011111	reserved   - Reserved for future use
11  100xxx	FRAGN      - Fragmentation Header (subsequent)
11  101000	reserved   - Reserved for future use
....	reserved   - Reserved for future use
11  111111	reserved   - Reserved for future use
//}


名前の通り、この領域が示す情報で後続のデータの読み方が全く変わるので、
それぞれの処理の振り分けを行うため情報である。



01000001(0x41) がヘッダ圧縮を示すHC1、
11000xxx(0xC0～0xC7)と11100xxx(0xE0～0xE7)でパケット分割のデータ構造を
示している。
後にRFC6282のヘッダ圧縮の改定で、011xxxxx(0x60～0x7F)がIPHCとして規定された。


=== ヘッダ圧縮


IPv6と6LoWPANを比較して、その際立った特徴はヘッダ圧縮だろう。
802.15.4の最大127byteという極めて小さいサイズのペイロードでは、
ヘッダだけで1/3を使い潰してしまう。



このため、6LoWPANではあの手この手で情報の圧縮を行っている。



RFC4944 で規定されているヘッダ圧縮はHC1(IPヘッダ圧縮)とHC2(UDPヘッダの圧縮)の
２つが規定されている。



IPv6のヘッダで大部分を占めているのが16byteのアドレス情報である。送信元と送信先で
計32byte、IPv6ヘッダの実に8割がこれで占められている。



このアドレスを如何に効率よく格納・復元するかというのがヘッダ圧縮の主要な課題である。


=== HC1


HC1のアドレス圧縮の戦略は、「MACアドレスからわかるものはそれを使う」



IPv6のSLAAC(StateLess Address Auto Configuration)では、128bitのアドレスのうち、
下位64bitをMACアドレスから算出するようになっている。



であれば、128bitのうち下位64bitについては、MAC層のアドレスと情報が重複している
ということになる。



また、上位64bitも同一セグメント内同士のリンクローカルアドレスであれば、
fe80:0:0:0の固定となる。



纏めると、

//table[tbl2][]{
圧縮可能	上位64bit(prefix)	下位64bit(address)
-----------------
可	Link Local (fe80)	MACアドレスから算出
不可	通常のprefix	　通常のアドレス
//}


のようになる。



HC1では2bitの領域を使って、prefixがfe80であるか否か、addressがMAC派生であるか否か、
を示している。
理想的なケースは、HC1の場合、LinkLocalでのピアツーピア的な通信で、
この場合はLink Localの固定prefixとMACアドレスからIPv6アドレスの算出が可能となり、
6LoWPAN層で持つアドレス情報は4bitのアドレス圧縮フラグのみとなる。
SLAACのアドレス決定方式に従う場合でLink Local以外の通信はprefixの8byte、
Link LocalでSLAACのアドレス決定方式でない場合(多くはなさそう…)も8byteのアドレスが
必要となる。



逆説的には、Link Localでなければprefix部の8byteは必ず必要となるし、
MACアドレスと一致しないノードに対しての通信に対してはアドレス部は必ず8byte必要とある。
これが意味するところは、「ルーターの外に出るパケットについては全くアドレスが圧縮されない」ということになる。


=== IPHC


HC1のアドレス圧縮では、最小で4bitまで圧縮可能であることは示せたが、
適応ケースが極めて少ない仕様であり、だいたい送信元送信先あわせて16bitぐらい
必要なケースが大半、ルーターを超えるような場合は全く小さくならず、
実用的なケースを十分カバーできているとはいいがたい状況だった。



いわば第二世代にあたるIPHCではこの問題に対処すべく、
「コンテクストベースの圧縮」を規定している。



これは、言ってしまえば「短縮ダイアル(若い子にはもう解らない?!)」方式で、事前に対応テーブルを作って
受信側送信側双方で展開を行う仕組みである。



では、それはどうやって対応テーブルを共有するのか？、という疑問が発生するが、
これについてはrfc6775の近傍探索の拡張で規定している。
@<href>{https://tools.ietf.org/html/rfc6775#section-4.2,6LoWPAN Context Option https://tools.ietf.org/html/rfc6775#section-4.2}
つまり、IPv4におけるARPの役割に近いところでこれを定義している。



IPHCは4ビットで事前に共有したコンテクストを指定できる。
すなわち、16個の短縮アドレスがあるということである。



IPHCのヘッダが16ビット、コンテクストが4ビット+4ビットで、
これで24ビットのヘッドになった。



さらに、



: CIDフラグ
  1に設定されている場合、送信元、送信先ともに0番のコンテクストとする。



という裏技が規定されていて、コンテクスト指定部を省略できる形式がある。



これによって、事前に定義された「最も頻度の高い経路」については、
16ビットまで短縮できる。



HC1でも、最短16ビットとなったが、HC1で「MACが見えるドメインでのリンクローカル通信のみ」という
限定から、コンテクストの内容によってこの制限が緩和されたこと、
よく使うアドレスが16以内であれば、24bitまで詰められるので、
比較的現実的なシーンに対応できるようになった、といえるであろう。

