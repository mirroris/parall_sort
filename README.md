# parallel_sort
This new algorithm is original designed by Fuma Suenaga(github:mirroris).

It can sort any array in O(Nlogk)⋍O(N) time.

I came up with this algorithm in December 2022

I wrote this readme for the first time 2023/01/05

I am Japanese, so plz forgive describing this algorithm in Japanese. Let me get more time to translate it to English.

## 新たなソーティングアルゴリズムの考案

私のオリジナルアルゴリズムとして主張できるようにとりあえず先に文字を起こして公開しています。
つまりGithubにソースコードだけ置いて、いくらでも盗用できる状態から抜け出したかっただけです。
これから、より詳しく書き換えていくよう努めます。

##　背景
ゼミの研究で並列ソーティングの可能性について考えていたところ、O(Nlogk)≃O(N)でソーティングできるアルゴリズムを思いついたので公開します。O(N
)と聞いてO(NlogN)よりも早いことに違和感を感じる方もいるかと思いますが、key比較しないソーティングの限界はO(NlogN)ではありません。RadixSortなどがその主な例です。


##　アルゴリズムの詳細
1. Building Tree

まず、ソート対象を木構造に落とし込みます。この木構造は名前が無いのでF-Treeとします。

F-Treeは2分木で、葉だけがソート対象の要素を持つ。

F-Treeに要素xを挿入するとき、2進数xのi桁目が0のとき、左の子へ、1のとき右の子へたどるようにしてノードを作りあげる。
既にノードがある場合は再度生成しない。同じ要素に対してはその数を葉が持っているので
、葉への辺が存在するかどうかで値の存在を確かめることができる。なお、この木構造を導入した利点はF-Treeに落とし込むだけで半ばソート完了と
言っても過言ではないところである。

全ての要素をF-Treeに挿入したら２の操作に移ります。

2. DFS

次に、DFSでF-Treeをたどればソーティングされていることになります。

DFSの計算量はO(N)です
