
	TOPPERS新世代カーネル用コンフィギュレータ（Release 1.6.0）


TOPPERS新世代カーネル用コンフィギュレータは、カーネルやソフトウェア部品
の構成や初期状態を定義したシステムコンフィギュレーションファイルを解釈し、
システムを構築する上で必要なファイル類を生成するTOPPERS新世代カーネル用
のツールです。

コンフィギュレータは、コンパイラやアセンブラを初めとする開発ツールと同様、
PC等の開発用コンピュータ上で動作するコマンドラインプログラムです。したが
って、コンフィギュレータそのものが最終的な製品に組み込まれることは、原則
としてありません。コンフィギュレータはコマンドラインツールなので、コンパ
イラを初めとした他のコマンドラインツールと組み合わせて使用することを前提
としています。しかし、ユーザーが適切に設定を行うことで、多くの統合開発環
境から呼び出せます。 


【コンフィギュレータの構築方法】

カーネルを構築する前に、まず、コンフィギュレータをコンパイルする必要が
あります（コンフィギュレータをバイナリで入手した場合には、このステップ
は必要はありません）。
コンフィギュレータの構築にはBoost C++ Libraries 1.42.0以上が必要です。
Boost C++ Librariesは、下記URLから入手することができます。

http://www.boost.org/

Windowsの場合、Visual C++用のバイナリが下記URLのサイトから入手可能です。

http://www.boostpro.com/

Boost C++ Librariesをバイナリで入手できない場合には、ソースからビルドし
てください。

=== GNU開発環境を用いる場合 ===

コンフィギュレータ（cfgプログラム）を構築するには、cfgディレクトリに移動
し、configureおよびmakeコマンドを実行します。

	% cd cfg
	% ./configure
	% make

ただし、Boostをインストールしたディレクトリおよび名称が、標準で想定して
いるものと違う場合には、configureに--with-headersおよび--with-libraries
オプションによりインクルードおよびライブラリのディレクトリを指定してくだ
さい。
また、ホストシステムによっては、最適化レベルを上げると正しくコンパイルで
きないことが知られています。そのような場合には、最適化レベルを下げるか、
最適化を抑止するように、Makefileを修正する必要があります。

=== Microsoft Visual Studioを用いる場合 ===

コンフィギュレータ（cfgプログラム）を構築するには、cfgディレクトリ中の
cfg.slnをVisual Studio（またはVisual C++ Express Edition）で開いてくだ
さい。cfg.slnはVisual Studio.NET 2003用のものですが、上位のバージョンを
使用する場合にはソリューションを変換して使用してください。
ソリューションを開いた後、Release版としてビルドを実行してださい。
cfg/cfg/Releaseディレクトリにcfg.exeが生成されますので、必要なフォルダ
にコピーまたは移動してください。

・注意事項

Boost C++ Librariesをインストールした後、インクルードおよびライブラリの
ディレクトリをVisual Studioに登録してからビルドを実行してください。
Visual C++ 2005 Express Editionを使用する場合、別途Platform SDKをインス
トールする必要があります。


【コンフィギュレータの使い方】

コンフィギュレータ（cfg）は、TOPPERS新世代カーネル統合仕様書の2.12.5節
の記述の通り、3つ（または4つ）のパスで構成されます。

コンフィギュレータは、システムコンフィギュレーションファイル名をパラメー
タに取り、以下のオプションを持ちます（重要なもののみ）。

	--help
		オプションのリストと説明を表示する。

	-v［--version］
		コンフィギュレータのバージョンを表示する。

	-k［--kernel］<カーネル名>
		カーネルの名称を指定する。デフォルトはasp。

	-p［--pass］<パス番号>
		パス番号を指定する。1〜3のいずれか。

	-I［--include-path］<インクルードパス名>
		INCLUDEディレクティブにおいて、インクルードするコンフィギュレー
		ションファイルを探すパス名を指定する。

	-T［--template-file］<テンプレートファイル名>
		テンプレートファイルの名称を指定する。パス2とパス3で有効。

	--api-table <静的APIテーブルファイル名>
		静的APIテーブルのファイル名を指定する。

	--cfg1-def-table <値取得シンボルテーブルファイル名>
		値取得シンボルテーブルのファイル名を指定する。

	-r［--rom-image］<ロードモジュール名>
		ロードモジュールをSレコードフォーマットの形に変換したファイルの
		名称を指定する。パス3でのみ有効。

	-s [--symbol-table] <シンボルファイル名>
		ロードモジュール中の各シンボルとアドレスの対応表を含むシンボル
		ファイルの名称を指定する。パス3でのみ有効。

	--id-output-file <ID番号出力ファイル名>
		オブジェクトのID番号の割付け結果を、指定した名前のファイルに書
		き出す。パス2でのみ有効。

	--id-input-file <ID番号入力ファイル名>
		オブジェクトのID番号の割付けを、指定した名前のファイルから取り
		込む。パス2でのみ有効。

	--external-id
		オブジェクトのID番号を保持する変数の定義を生成する。パス2でのみ
		有効。

	-M［--print-dependencies］<ターゲットシンボル>
		システムコンフィギュレーションファイルの依存関係を出力する。
		Makefile中の依存関係を生成する際に用いる。

コンフィギュレータの詳細仕様については、別途PDFファイルの形で配布してい
る「TOPPERS新世代カーネル用コンフィギュレータ仕様」を参照してください。
テンプレートファイルの作成にあたっては、「TOPPERS新世代カーネル用コンフ
ィギュレータ内蔵マクロプロセッサ仕様書」を参照してください。

