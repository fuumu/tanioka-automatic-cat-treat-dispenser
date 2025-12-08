#include <WiFiS3.h>  ///Arduino UNO R4 WiFi向けのWi-Fi制御ライブラリ

#include <ArduinoHttpClient.h>/// HTTP/HTTPSリクエストを簡単に送るためのライブラリ

#include <Servo.h> ///サーボを動かす為の関数を使用できるようになるライブラリ

#include"arduino_secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;  // WiFi接続状態の初期値

// LINE Messaging API設定
const char serverAddress[] = "api.line.me";///LINEのMessaging APIサーバのホスト名

const int port = 443; ////HTTPS用のポート番号
const String channelToken = "o72tG0EFTF/ZdGxDrkwuzDmbdW14r4hq/V/XSOYOJiTEusf3nA3cm/mwrniJUIfAZcp16wJeIEP2l1AuIDEiOchyGiwdGp47NeJxbqdQdaUo8Cwl4k/I68V96+JMo4CZKK//n0L4bvVjh7AnC1koYQdB04t89/1O/w1cDnyilFU="; // Messaging APIのチャネルアクセストークン
                                                         //認証に使うチャネルアクセストークン

// ピン設定
const int touchPin = 2;       // TTP223タッチセンサ
const int servoPin = 9;       // SG90サーボ
Servo gateServo;              //gateServoというサーボ制御用のオブジェクトを宣言

bool lastTouchState = LOW;   /// 前回のセンサ状態を記録する変数

unsigned long lastTriggerTime = 0; ///最後に動作した時間を保存する変数

const unsigned long cooldown = 10000; // 10秒クールダウン重複開閉を防ぐため

int count = 0;///おやつを排出回数を保存する変数
#define MAX_SNACK 3///最大おやつ排出回数

WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);

void setup() {
  delay(2000);
  Serial.begin(115200);

  pinMode(touchPin, INPUT);   ///タッチセンサのピンを入力モードに設定

  gateServo.attach(servoPin);  ///サーボをD9に接続

  gateServo.write(0); // 初期状態：閉じる　0度にする

///// USB接続が完了するまで待機
  while (!Serial) {
    ;  
  }

  //- WiFiモジュールが見つからない場合は停止
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFiモジュールとの通信が確立できませんでした");
    while (true)
      ;
  }
///ファームウェアが古い場合は警告
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("ファイアウォールをグレードアップしてください");
  }

  //- WiFiに接続されるまで繰り返し接続を試みる
  while (status != WL_CONNECTED) {
    Serial.print("接続を試みていますSSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    delay(10000);
  }

  //- 接続成功
  Serial.println("ネットワーク接続が完了しました");

}

void loop() {
  bool touchState = digitalRead(touchPin);/// タッチセンサの状態を読み取る。触れるとHIGHになる

  /// 「触られた瞬間（前回LOW→今回HIGH）」かつ「最後の動作から10秒以上経過」したら動作開始
  if (touchState == HIGH && lastTouchState == LOW && millis() - lastTriggerTime > cooldown) {
    Serial.println("猫が触れました！おやつ排出します");

    count++;

    for(int pos=0; pos<=90; pos+=5){
    gateServo.write(pos);
    delay(50);
   }

    delay(2000);  ///開いた状態で待機（秒数は実機を作ってから決める）

    for(int pos=90; pos>=0; pos-=5){
    gateServo.write(pos);
    delay(50);
    }

    char msg[512];  // メッセージ用バッファ
    snprintf(msg, sizeof(msg),"The cat activated the touch sensor! Remaining treat dispenses: %d times!",MAX_SNACK - count);
    ///snprintf:書式付き文字列を安全に msg に書き込む関数
    ///第2引数 sizeof(msg) により、最大 512 バイトまでしか書き込まれないので バッファオーバーフロー防止

    sendLineMessage(msg);//直前に組み立てた文字列 msg を LINE送信関数に渡します
    
    Serial.println("送信完了");

    lastTriggerTime = millis();///今回の動作時間を記録。次のクールダウン判定に使う
  }
  if(count==MAX_SNACK)
  {
    char msg1[512];
    snprintf(msg1,sizeof(msg1),"Add Snack!");
    sendLineMessage(msg1);

    while(1)
    {
      ;
    }
  }
  lastTouchState = touchState; ///今回のセンサ状態を保存。次回の判定に使う

}

// LINE Messaging APIでメッセージ送信関数
void sendLineMessage(const char* message){///与えられた文字列messageをLINEに送る関数の開始

//WiFi接続が切れていたら、試みる
  while(WiFi.status() != WL_CONNECTED){
  status = WiFi.begin(ssid, pass);
  delay(10000);
}

  // JSON本文を char[] + snprintf で組み立て
  // バッファサイズは十分に大きく確保（例: 512）
  char body[512];
  snprintf(body, sizeof(body),
           "{\"to\":\"U76585dd698c2e9b1f679a9daa6c849c5\",\"messages\":[{\"type\":\"text\",\"text\":\"%s\"}]}",message);////snprintf:書式付き文字列を安全にバッファへ書き込む関数
  ///sizeof(body) を指定しているので、最大 512 バイトまでしか書き込まれず、バッファオーバーフローを防げます
  ///LINE Messaging API が要求する JSON 形式
 ///"to" に送信先ユーザーIDを指定
 ///"messages" 配列にテキストメッセージを入れます
 ///%s は プレースホルダで、ここに message の文字列が埋め込まれます
///引数として渡された文字列（const char*）が %s の位置に展開

  client.beginRequest();/// 新しいHTTPリクエストの準備開始

  client.post("/v2/bot/message/push");///プッシュメッセージ送信用エンドポイントにPOSTリクエスト

  client.sendHeader("Authorization", "Bearer " + channelToken);///-  認証ヘッダを追加（Bearer認証）

  client.sendHeader("Content-Type", "application/json");///本文がJSONであることをサーバに伝える

  client.sendHeader("Content-Length", strlen(body));///送る本文の長さ（バイト数）を明示

  client.beginBody();/// ここからリクエスト本文の送信開始を宣言

  client.print(body);///実際のJSON本文を送信

  client.endRequest();///リクエストを終了し、サーバのレスポンス待ちへ

  int statusCode = client.responseStatusCode();///サーバから返ったHTTPステータスコードを取得
                                               ///目安: 200が成功。401/403は認証・権限問題、400はJSON不備など

  String response = client.responseBody();///サーバのレスポンス本文（詳細メッセージ）を取得

  Serial.print("LINE status: ");

  Serial.println(statusCode);///実際のステータスコードを出力

  Serial.println(response);///レスポンス本文を出力（エラー原因の確認に役立つ)

 client.stop(); //HttpClient client が持っている HTTPリクエスト／レスポンスのセッションを終了
                //呼ばないと、次回の送信時に「前の接続が残っている」と誤解されて失敗することがある

 wifi.stop(); //WiFiSSLClient wifi が持っている暗号化されたソケット（TLS/SSLセッション）を終了
              //HTTPS通信では「暗号化トンネル」を張っているので、これを閉じないと次回の接続が不安定になる

}
