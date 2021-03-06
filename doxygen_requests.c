/**
 * @file doxygen_requests.c
 * @author Shinichiro Nakamura
 * @brief Doxygen用のファイル。改善要求が記述してある。
 * @details
 *
 * @page requests 改善要求
 *
 * @section requests_hardware ハードウェアに関する改善要求
 *
 * - NIKKAI HB2-15の感触やクリック音が個体によって異なるので他のスイッチにしたい。
 *   - FUJISOKU CFPB-1CC-4W9W が結構良いのでそちらにしたい。
 *
 * - 隣り合うパッドで接続するパターンが一部良くない。(ショートチェックがやりにくい。)
 *   - パッドの横から直接横に伸びている。
 *   - OLEDコネクタ
 *   - TLV320AIC23Bの一部の端子
 *
 * - ボリュームのセンス値がフラフラしすぎのような気がするけど？
 *
 * - 通信確認LEDが暗い。
 *   - デバッグ用LEDに採用したLEDと同じ物を使う。(3号機、4号機は改善済み。)
 *
 * - RESET, ISPのシルクが入っていない。
 *   - ガーバーデータ作成時にレイヤー指定を間違えたかも。
 *   - さらにチェックが甘かった。
 *
 * - オーディオコネクタのどれが何かわからない。シルクを入れたい。
 *   - 場所がない。
 *
 * - デバッグ用LEDが眩しすぎる。意図して高輝度LEDだが、眩しすぎ！
 *   - 抵抗値を変更する。
 *
 * - スイッチが微妙な基板端に配置されていてカバーアクリルを作りづらい。
 *   - 基板端にするならする。しないなら離す。
 *
 * - UART2がネジ穴に近すぎる。
 *   - もう少し離して配置したい。
 *
 * - JTAGコネクタがネジ穴に近い。
 *   - もう少し離して配置したい。
 *
 * - 発振器のパッケージを見直したい。
 *   - 小さいパッケージの中ではどれが一般的？
 *
 * - ボリュームではなく、ロータリーエンコーダにしたい。
 *   - ボリュームの初期状態に設定値が縛られるのを避けたい。
 *   - クルクル回したい。
 *
 * - TP1, TP2の位置が使いづらい。
 *   - 配線の都合上？
 *
 * @section requests_firmware ファームウェアに関する改善要求
 *
 * - サンプルデザインをもっと魅力的にしたい。
 */
