

//raw[|latex| \clearpage ]


== Protothreads


ContikiOSの最大の特徴は @<b>{Protothreads} の仕組みによって、
単一の @<b>{main()} 関数実行の中で、マルチタスクのプログラミングが
可能としていることだ。



この動作を理解すると、ContikiOSのとても歴史的な実装の下での
マルチタスクプログラミングを行ううえで



@<b>{Protothreads} は、端的に言ってしまえば個々のタスクを状態機械として
実行状態を制御し、それを @<b>{main()} のループの中で逐一呼び出す
一連の仕組みのことある。
