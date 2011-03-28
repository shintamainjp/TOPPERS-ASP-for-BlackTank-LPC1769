/**
 * @mainpage
 *
 * @section about
 *
 * この文章はdoxygen.cに記述されているが、実際にコンパイル対象ではない。
 * Doxygenの処理の便宜上付けたファイル名である。
 *
 * @section sec1 BlackTank LPC1769
 *
 *  BlackTank LPC1769はARM Cortex-M3とRTOSを楽しむ事を目的に設計
 *  した基板です。
 *  有機ELディスプレイ、４系統ユーザ入力、オーディオコーデック、
 *  マイクロSDカードなどが１枚の基板に搭載されています。
 *  ARM Cortex-M3にはNXP SemiconductorsのLPC1769を採用しています。
 *
 *  また、基板サイズはユニバーサル基板(Sunhayato ICB-293)と同じです。
 *  実験回路をユニバーサル基板に実装して、ドッキングして使用する
 *  こともできます。
 *
 * @section sec2 タスク設計
 *
 * @subsection sec21 ユーザインプットタスク(task_userinput)
 *  ユーザインプットタスクは４系統のユーザ入力の変化を観察する
 *  タスクです。
 *  変化があればデータキューを介してシステムコントロールタスクに
 *  通知されます。
 *
 * @subsection sec22 メニュータスク(task_menu)
 *  メニュータスクはユーザにシステムのサービスを提示し、
 *  ユーザからの要求をシステムに伝達する役目を果たします。
 *
 *  ユーザは各タスクから次のマクロのみで指示することができます。
 *
 *  @code
 *  USERMSG(device, value);
 *  @endcode
 *
 *  - deviceは以下から選択できます。
 *    - スイッチ: SW0, SW1, SW2, SW3
 *    - ボリューム: VOL0, VOL1, VOL2, VOL3
 *    .
 *  .
 *
 *  - Typeはビットマッピングです。
 *    - 長押し状態フラグ：SW_LONG_PUSH
 *    .
 *  .
 *
 *  このタスクへの指示はデータキューを介して行われます。
 *  データキュー内部データ構造は以下のようになっています。
 *
 *  <pre>
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+------------------+
 *  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0| Description      |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+------------------+
 *  |                 |<-          [9:0]          ->| Value            |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+------------------+
 *  |           |11:10|                             | Type             |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+------------------+
 *  |<-[15:12]->|                                   | Device           |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+------------------+
 *  </pre>
 *
 * @subsection sec23 LEDタスク(task_led)
 *  LEDタスクはLEDの点灯を制御するタスクです。
 *
 *  点灯は各タスクから次のマクロのみで指示することができます。
 *
 *  @code
 *  LEDMSG(target, control);
 *  @endcode
 *
 *  - targetは以下から選択できます。
 *    - デバッグ用LED: DBLED0, DBLED1, DBLED2, DBLED3
 *    - スイッチ用LED: SWLED0, SWLED1, SWLED2, SWLED3
 *    .
 *  .
 *
 *  - controlは以下から選択できます。
 *    - LED点灯：LEDON
 *    - LED消灯：LEDOFF
 *    .
 *  .
 *
 *  このタスクへの指示はデータキューを介して行われます。
 *  データキュー内部データ構造は以下のようになっています。
 *
 *  <pre>
 *  +---+---+---+---+---+---+---+---+-------------+
 *  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | Description |
 *  +---+---+---+---+---+---+---+---+-------------+
 *  |               |<-   [3:0]   ->| Target      |
 *  +---+---+---+---+---+---+---+---+-------------+
 *  |   | x   x   x |               | Reserved    |
 *  +---+---+---+---+---+---+---+---+-------------+
 *  |[7]|                           | Control     |
 *  +---+---+---+---+---+---+---+---+-------------+
 *  </pre>
 *
 * @subsection sec24 ディスプレイタスク(task_display)
 *  ディスプレイタスクは有機ELディスプレイを制御するタスクです。
 *
 *  ディスプレイの制御は以下のマクロを使って行うことができます。
 *
 *  @code
 *  DISP_CLEAR(R, G, B);
 *  DISP_LINE(X1, Y1, X2, Y2, R, G, B);
 *  DISP_BOX(X1, Y1, X2, Y2, R, G, B);
 *  DISP_FILLBOX(X1, Y1, X2, Y2, R1, G1, B1, R2, G2, B2);
 *  DISP_TEXT(X, Y, R, G, B, "TEXT");
 *  DISP_BMPFILE("0:FILENAME");
 *  DISP_AUDIO_LEVELMETER(L, R);
 *  @endcode
 *
 *  タスク間はメールボックスでやりとりされます。
 *  マクロはこれを隠蔽した実装としました。
 *  メールボックスで陥りがちな同期問題を未然に防ぐ対策として
 *  メモリプール管理機能を使ってデータ領域に対するアクセスを
 *  管理しています。
 *
 *  今回の実装では受信側が処理を完了するまで送信側が次の処理に
 *  遷移しない実装にしてあります。
 *  今回のタスク設計ではメニュータスクがブロックの対象となります。
 *
 * @subsection sec25 ナチュラルタイニーシェルタスク(task_ntshell)
 *  システムをコンソールから制御することのできるインターフェース
 *  タスクです。
 *
 * @subsection sec26 オーディオ初期化タスク(task_audio_init)
 *  オーディオを初期化するためのタスクです。
 *
 * @subsection sec27 オーディオタスク(task_audio)
 *  オーディオを処理するためのタスクです。
 *
 *  このタスクにはパラメータを指定するためのマクロがあります。
 *
 *  @code
 *  AUDIO_PARAM(TARGET,VALUE);
 *  @endcode
 *
 *  - TARGETとVALUEは以下から選択します。
 *    - TARGET: VAR0, VAR1, VAR2, VAR3
 *    - VALUE : The value of the target.
 *    .
 *  .
 *
 *  <pre>
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+------------------+
 *  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0| Description      |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+------------------+
 *  |           |<-            [11:0]             ->| Value            |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+------------------+
 *  |<-[15:12]->|                                   | Target           |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+------------------+
 *  </pre>
 *
 *  VAR0, VAR1, VAR2, VAR3の各値をどのように使うのかは実装される
 *  オーディオエフェクトに依存します。
 *  詳しくはaudio_effect.cを御参照下さい。
 *
 * @section sec3 アプリケーション例
 *
 * - オーディオエフェクタ
 * - メトロノーム
 * - 多機能時計
 * - ピンボールゲーム
 *
 * @section sec4 プライオリティ
 *
 * ユーザ入力タスクはディスプレイタスクより優先度が高い必要がある。
 * 表示処理中にユーザ入力があっても処理できる必要があるためである。
 *
 * @section sec5 使用上の注意
 *
 * OLEDは同じ内容を表示し続けると焼き付きが発生します。
 * 表示内容を時折変更するようにして下さい。
 */
