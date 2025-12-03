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
|Arduino UNO R4 WiFi|1|マイコン||
|タッチセンサ（TTP223B）|1|手動給餌トリガー||
|サーボモータ(9G Servo)|1|- フラップゲートの開閉制御||

# 使用ライブラリ（ソフトウェア）
・WiFiS3.h

　→Arduino UNO R4 WiFi向けのWi-Fi制御ライブラリ

・ArduinoHttpClient.h

　→‣HTTP/HTTPSリクエストを簡単に送るためのライブラリ
 
・Servo.h

 　→Arduino標準ライブラリのひとつで、サーボモータを簡単に制御するためのヘッダファイル

# 配線図
  <img width="463" height="730" alt="image" src="https://github.com/user-attachments/assets/51c0345f-d02d-4cca-8499-4d4f401e82a4" />

# 回路図
<img width="817" height="573" alt="image" src="https://github.com/user-attachments/assets/2ce914e1-487e-42d6-9dd5-871a02f01f54" />



