

//raw[|latex| \clearpage ]


== intikiの実装について

=== sketchのコンパイル


Arduinoのスケッチは、C++の文法に従っている。
実際にコンパイルが行われる場合は、


//emlist{
#include <Arduino.h>
#line 1 "Blink.ino"
void setup();
#line 6 "Blink.ino"
void loop();
#line 1 "Blink.ino"
//}


のような形で、@<tt>{Arduino.h}のインクルードと、エラーメッセージの行番号調整のための @<tt>{#line}
マクロが埋め込まれている。
スケッチのコンパイルが行われる場合、このc++のソースと、
Arduinoのインストールフォルダの配下にある、@<tt>{hardware}の下のソース
(AVRの場合は @<tt>{hardware/arduino/avr/cores})、必要であれば、ライブラリのソースも対象にして、
コンパイル、リンク、hex化の処理が行われる。



@<tt>{main()}関数はmain.cppの中で定義されており、
スケッチの @<tt>{setup()} と @<tt>{loop()} はここから呼び出されている。



Arduinoの処理の流れ自体は非常にシンプルで、所謂ベアメタルと呼ばれる、
OS的な仕組みのない、エントリポイントのアドレスから処理を逐一実行していく、
素朴な組み込みプログラミングである。


=== Protothreads


Contikiの最大の特徴は @<b>{Protothreads} の仕組みによって、
単一の @<b>{main()} 関数実行の中で、マルチタスクのプログラミングが
可能としていることだ。



@<b>{Protothreads} は、端的に言ってしまえば個々のタスクを状態機械として
実行状態を制御し、それを @<b>{main()} のループの中で逐一呼び出す
一連の仕組みのことある。



具体的な実装はマクロの集合で行われている。



Contikiの場合もArduinoと同じように、@<b>{main()}のエントリポイントから処理を
実行していく処理となっている。
ただ、そこに@<b>{Protothreads}の仕組みが入ることによって、
疑似マルチスレッド的な処理が行え、OSのシステムコールに相当するような動きが
実現できている。



この機構によって、ネットワークも含めて非同期処理が可能になっており、OSと呼べる様々な機能が実現されている。
ただ、プログラムの制御構造の観点から見ればArduinoとほぼ同様で、
エントリポイントからメインループの処理を逐一実行するという構造は変わらないのである。


=== multithread (あるいはCoroutine)


Contikiには、multithread APIというものが定義されている。
これは、@<b>{main}とは別にスタックフレームを用意して、処理を切り替える際に
スタックを切り替える、所謂コンテクストスイッチのためのAPIである。
実行の切り替えはOS側で行わず、処理の中で明示的に他のタスクに処理を譲る(@<tt>{yield})必要があるから、
multithreadというよりも、むしろCoroutineと呼ぶべき機能ではあるのだが、
ContikiのAPIとして定義されている。
ただ、ContikiではProtothreadsの機能で非同期処理が実現できてしまうため、
このAPIを実装しているプラットフォームがほとんど存在しないのである。



今回intikiでは、このmultithread機能を使って、
Arduino互換レイヤを実装している。
ContikiのProtothreadsはマクロ展開をうまく使うことで実装されており、
マクロ展開が行えるよう文法に制約が発生する。
一方、ArduinoのスケッチはC++の文法以上の制限は課していないため、
Protothreadsの仕組みの上に、Arduinoのソースは乗せられないということになる。



multithreadであればArduino部分の実装は
Protothreadsのマクロ展開の文法に縛られることなく行うことができる。
multithreadの実行制御は関数の呼び出しで行われるので、
文法としては、C/C++に準拠するためだ。


=== JN516xのコンテクストスイッチの実装


intikiに含まれるContiki(の改造版)には、


//emlist{
static void
sw()
{
  register unsigned int*  r7 __asm__("r7") = running->sp;
  register unsigned int** r8 __asm__("r8") = &(running->sp);

  __asm__ volatile(
    "b.di                   \n\t"

    "b.addi  r1, r1, -0x40  \n\t"
    "b.sw    0x00(r1), r1   \n\t"
    "b.sw    0x04(r1), r2   \n\t"
    ～～～～～
    "b.sw    0x3c(r1), r16  \n\t"

    "b.sw    0x00(%1), r1   \n\t"
    "b.mov   r1, %0         \n\t"

    /* r1: stack pointer */
    "b.lwz   r2,  0x04(r1)  \n\t"
    ～～～～～
    "b.lwz   r6,  0x18(r1)  \n\t"
    /* r7: local variable */
    /* r8: local variable */
    ～～～～～
    "b.lwz   r16, 0x3c(r1)  \n\t"
    "b.addi  r1, r1, 0x40   \n\t"

    "b.ei                   \n\t"
    :
    : "r" (r7), "r"(r8)
   );
}
//}


こんな感じで実装されているが、実のところ、TWE-Liteのアセンブラ命令の
詳細は不明であるため、なんとなく動いているというのが実情で、
検証も十分にできないのである…


=== Arduinoスレッドの設計


Arduinoスレッドの基本戦略は、「Arduinoのメインループを回す」、
「OS機能が必要な処理はAPIの中でyieldと復帰を管理する」。
これが意味するところは、スレッドの実行制御に関する処理は
全てAPIに埋め込むんで、Arduinoのスケッチからは行わないということである。
ArduinoのAPIで割り込みによる非同期実行を行うAPIは @<tt>{attachInterrupt} のみで、
それ以外はブロック実行で処理を行い、戻り値によってその結果を通知する、
普通の関数呼び出しとなっている。
コンテクストスイッチの場合、yieldした時点でスレッドの動作が停止するので、
実行スレッドの側からみるとブロック実行される関数の呼び出しである。
Arduino APIのブロック動作と同等の動きにするためには、実行制御をAPI実装の
中に埋める必要がある。
Arduinoスレッドが止まってる間にmainスレッド(すなわち、Protothreadsの世界で動いている方)
で処理を行い、完了したら再度Arduinoスレッドに実行を移す。
CPUを握って回り続けるのは


=== delay


ここで、最も基本的で、かつ最も重要でな実装となる @<tt>{delay} を取り上げる。
非同期の待ち合わせ、ということでいえば、この時間待ち合わせができれば
他の処理も同様に実装できるからだ。



処理のシーケンスを提示する。



//image[delay_sequence][delay シーケンス]{
//}


 1. ContikiのProtothreadの立ち上げ。@<tt>{main()}関数で @<tt>{process_run()} を呼ぶ無限ループを動かしている。システム側なのでユーザーは触らない処理である。
 1. Contiki側の@<tt>{main_thread}から、コルーチンとして@<tt>{arduino_thread}を立ち上げる。実行権は@<tt>{arduino_thread}に移る。
 1. @<tt>{arduino_thread}は、arduinoのスケッチを実行する。@<tt>{delay()}のような非同期処理が必要なAPI(contikiのプロセス間通信に依存する 機能)を実行するときに、実行権を開放し、@<tt>{main_thread}に実行コンテクストを移す。
 1. @<tt>{main_thread}は、@<tt>{arduino_thread}が要求する、contiki側の処理を実行する。@<tt>{delay()} であれば、contikiのタイマーAPIである、@<tt>{etimer_expired()}を呼び出す
 1. contiki側の応答があったら、その処理結果を格納して、@<tt>{arduino_thread}に再び実行権を移す
 1. 基本動作として、@<tt>{loop()}が終わったら、一旦@<tt>{main_thread}に実行権を戻す。 これをやらないと、contiki側の処理が動かなくなる。
 1. @<tt>{main_thread}は「即時復帰」のイベントを要求し、一旦OS側に制御を移す
 1. 9, contikiは即時に@<tt>{main_thread}に実行権を渡す
 1. 再び、@<tt>{arduino_thread}に実行コンテクストを移して、スケッチの実行 を再開する。@<tt>{loop()}を呼び終えたところで止まってるので、 復帰とともに、次の@<tt>{loop()}の呼び出しの処理が行われる。


@<tt>{arduino_thread}と@<tt>{main_thread}の間で行われる、処理の委譲は
共有変数を介して、関数ポインタと変数のやりとりをしている。
この2つの処理が同時に動くことはないので、競合などは発生しない。



arduinoのスケッチから見ると、APIの挙動が変わっていないので、
システムの挙動として目に見える変更はない。



contikiから見ると、@<tt>{arduino_thread}の処理は、@<tt>{main_thread}を介して
「多少戻りの遅い、一つのProtothread」として見える。
基本的にシステムとしての整合性はあるが、@<tt>{loop()}の構造によっては
それが破壊されることもある。
たとえば、@<tt>{loop()}の中で無限ループを行って@<tt>{loop()}を抜けないような
スケッチはcontiki側に制御が全く移らなくなるので、動作できない。



そのような制約はあるにしても、スケジューラのディスパッチによらない
本実装は、arduinoの自然な拡張として、contikiを利用した非同期実装は
十分有用と考える。

