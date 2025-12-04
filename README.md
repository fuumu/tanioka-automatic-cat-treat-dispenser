# automatic-cat-treat-dispenser

# 概要
タッチセンサに触れたら、サーボモーターでゲートを開閉し、自動でおやつを排出してLINEで排出したことを通知する

# 機能
・タッチセンサで触れたらおやつを排出する

・タッチセンサの出力がHIGHになればサーボモーターでゲートを開閉する

・おやつが排出されたら、LINEで時刻とメッセージを通知させる

※余裕があれば機能を追加する

# 使用モジュール
|部品|個数|用途|接続ピン|
|:---:|:---:|:---:|:---:|
|Arduino UNO R4 WiFi|1|通信・制御|USBケーブル|
|タッチセンサ（TTP223B）|1|手動給餌トリガー|D2|
|サーボモータ(9G Servo)|1|- フラップゲートの開閉制御|D9|

# 使用ライブラリ（ソフトウェア）
・WiFiS3.h

　→Arduino UNO R4 WiFi向けのWi-Fi制御ライブラリ

・ArduinoHttpClient.h

　→‣HTTP/HTTPSリクエストを簡単に送るためのライブラリ
 
・Servo.h

 　→Arduino標準ライブラリのひとつで、サーボモータを簡単に制御するためのヘッダファイル
  
・arduino_secrets.h

　→WiFi接続

# 配線図
  <img width="463" height="730" alt="image" src="https://github.com/user-attachments/assets/51c0345f-d02d-4cca-8499-4d4f401e82a4" />

# 回路図
<img width="817" height="573" alt="image" src="https://github.com/user-attachments/assets/2ce914e1-487e-42d6-9dd5-871a02f01f54" />

# 動作仕様書

```mermaid
graph TD
    開始 --> 初期化[setup]
    初期化 --> サーボ原点復帰
    サーボ原点復帰 --> WiFi接続
    WiFi接続 --> 接続失敗
    接続失敗 --> 停止
    WiFi接続 --> 成功
    成功 --> loop

    loop --> センサ読み取り[タッチセンサの状態読み取り]
    センサ読み取り --> LOW
    センサ読み取り --> HIGH

    LOW --> 判定["触られた瞬間（前回LOW→今回HIGH）かつ10秒以上経過"]
    HIGH --> 判定

    判定 --> False[条件不成立]
    False --> 保存[今回のセンサ状態を保存]
    保存 --> loop

    判定 --> True[条件成立]
    True --> サーボ開く
    サーボ開く --> 待機[1秒待機]
    待機 --> サーボ閉じる

    サーボ閉じる --> LINE通知呼び出し[LINE通知関数を呼び出す]
    LINE通知呼び出し --> WiFi再チェック{WiFi接続再チェック}
    WiFi再チェック --> 失敗!
    失敗! --> WiFi再チェック
    WiFi再チェック --> 成功!

    成功! --> SSLClient[SSL/TLS対応Wi-Fiクライアント作成]
    SSLClient --> JSON作成[JSON本文作成]
    JSON作成 --> HTTP準備[HTTPリクエスト準備]
    HTTP準備 --> POST送信[POSTリクエスト]
    POST送信 --> JSON送信[JSON本文送信]
    JSON送信 --> レスポンス待ち
    レスポンス待ち --> ステータスコード[ステータスコード取得]
    ステータスコード --> セッション終了[セッション終了]
    セッション終了 --> loop
```
