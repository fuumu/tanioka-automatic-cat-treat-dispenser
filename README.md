# automatic-cat-treat-dispenser

# 概要
卒業制作で自動おやつディスペンサーを作成

# 機能
・タッチセンサで触れたらおやつを排出する

・タッチセンサの出力がHIGHになればサーボモーターでゲートを開閉する

・おやつが排出されたら、LINEでタッチされたことと残りのおやつ排出回数を通知させる

・残りのおやつ排出回数が０になればLINEで補充通知と補充＆マイコンでリセットボタンを押すまで停止

# 使用モジュール
|部品|個数|用途|接続ピン|
|:---:|:---:|:---:|:---:|
|Arduino UNO R4 WiFi|1|通信・制御|USBケーブル|
|タッチセンサ（TTP223B）|1|手動給餌トリガー|D2|
|サーボモータ(9G Servo)|1|- フラップゲートの開閉制御|D9|
|Breadboard Power Module with Battery|1|電源供給の安定化|ブレッドボード|

# 使用ライブラリ（ソフトウェア）
・WiFiS3.h

　→Arduino UNO R4 WiFi向けのWi-Fi制御ライブラリ（追加ライブラリ）

・ArduinoHttpClient.h

　→HTTP/HTTPS 通信を行うためのクライアントライブラリ（追加ライブラリ）
 
・Servo.h

 　→サーボモータを制御するためのライブラリ（標準ライブラリ）
  
・arduino_secrets.h

　→WiFi SSIDやパスワードなど秘密情報を記述するためのヘッダーファイル

# 配線図
※Breadboard Power Module with Batteryを実機には取り付けている
  <img width="869" height="760" alt="image" src="https://github.com/user-attachments/assets/db1d06ef-2cb0-4e73-b557-7a47ea7ae2fe" />

# 回路図
<img width="817" height="573" alt="image" src="https://github.com/user-attachments/assets/2ce914e1-487e-42d6-9dd5-871a02f01f54" />

# 動作仕様書

```mermaid
graph TD
    開始 --> 初期化[setup & サーボ原点復帰]
    初期化 --> WiFi接続
    WiFi接続 -->|失敗| 停止
    WiFi接続 -->|成功| loop

    loop --> センサ判定[タッチセンサ状態判定]
    センサ判定 -->|条件成立| サーボ動作[開閉 & LINE通知]
    センサ判定 -->|条件不成立| loop

    サーボ動作 --> おやつ判定{おやつ残量チェック}
    おやつ判定 -->|残量あり| loop
    おやつ判定 -->|残量なし| 停止1[補充待ち & リセット]
    停止1 --> 初期化
```

# 参考サイト

[基本プロジェクト：サーボ](https://docs.sunfounder.com/projects/elite-explorer-kit/ja/latest/basic_projects/27_basic_servo.html)

[WiFi接続方法参考](https://docs.sunfounder.com/projects/elite-explorer-kit/ja/latest/iot_projects/00_iot_project.html)
