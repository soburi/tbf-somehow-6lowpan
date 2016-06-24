//raw[|latex| \\clearpage ]

やっぱりクソ仕様じゃないか！
--------------------------

ここまで見てきた流れで、6LoWPANが如何に無茶な要求をすり合わせて
生まれてきたものかは何となく察することはできよう。
しかしながら、それでも「為さねばならぬ」となった背景がある。
それが、 2005年に米国行政管理予算局から発行されたIPv6移行計画に関わる調達指針である。

: M-05-22, Transition Planning for Internet Protocol Version 6 \(IPv6\)
  [https://www.whitehouse.gov/sites/default/files/omb/assets/omb/memoranda/fy2005/m05-22.pdf](https://www.whitehouse.gov/sites/default/files/omb/assets/omb/memoranda/fy2005/m05-22.pdf)

政府がIPv6移行に本腰を入れることを示したこの調達方針によって、
米国のIPv6が推進されることになる。

6LoWPANの仕様は、2004年にIETFのworking groupが発足、RFC4919の初版は2007/08の制定となっている。
ちょうどこの頃の動きとリンクしているようなタイミングである。
背景は色々推し量るところも多いが、ともかくこの仕様により、
のちにIoTと呼ばれる機器もIPv6移行への道筋がつけられることになる。

見るからに妥協の産物で、IPv6が目指した理想とは全く違う場所に着地することになった
6LoWPANであるが、政治的要請も含めて必要に迫られて作られた「醜くも現実な解」であることは
知っておくべきであろう。

LoRaWANや日本発のローカル規格のWiSUNなど、IPv6がそのまま載せられて6LoWPANの仕組みが
不要な新たに登場しているので、徐々に不要になっていってほしい規格ではあるが、
この経緯を見るに「妥協案であるが故になかなか捨てられない」状況も透けて見えてきそうで、
なかなかしぶとく生き延びてしまいそうな印象も受ける。


ちなみに、日本の公共系のIPv6調達は、米国に9年遅れて2014年に総務省から

: IPv6対応調達仕様書モデル
  [http://www.soumu.go.jp/main\_content/000301467.pdf](http://www.soumu.go.jp/main_content/000301467.pdf)

が発行されている。
