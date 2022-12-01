#include <WiFi.h>
#include <WiFiClientSecure.h>

//請修改以下參數--------------------------------------------
char SSID[] = "WTY";
char PASSWORD[] = "A940812A";
String Linetoken = "rOHJ8EqnoiDD1fYXOdRDGyWvwGYbxmlwc8IvZigPLOk";
//---------------------------------------------------------
WiFiClientSecure client;//網路連線物件
char host[] = "notify-api.line.me";//LINE Notify API網址
//---------------------------------------------------------
int value, old_value;
int LED = 36;
//---------------------------------------------------------
void setup() {
  pinMode(LED, OUTPUT);  //設定 DIO Pin 13 為 LED 輸出
  Serial.begin(115200);
  //連線到指定的WiFi SSID
  Serial.print("Connecting Wifi: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  //連線成功，顯示取得的IP
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  client.setInsecure();//ESP32核心 1.0.6以上
}

void loop() {
  value = analogRead(LED); //參數也可用 0
  String message;
  if (value > 512 && old_value <= 512) {
    message = "天亮了";
    PP(message);
  }
  if (value <= 512 && old_value > 512) {
    message = "天黑了";
    PP(message);
  }
  old_value = value;
}
void PP(String message) {
  if (client.connect(host, 443)) {
    int LEN = message.length();
    //傳遞POST表頭
    String url = "/api/notify";
    client.println("POST " + url + " HTTP/1.1");
    client.print("Host: "); client.println(host);
    //權杖
    client.print("Authorization: Bearer "); client.println(Linetoken);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: "); client.println( String((LEN + 8)) );
    client.println();
    client.print("message="); client.println(message);
    client.println();
    //等候回應
    delay(2000);
    String response = client.readString();
    //顯示傳遞結果
    Serial.println(response);
    client.stop(); //斷線，否則只能傳5次
  } else {
    //傳送失敗
    Serial.println("connected fail");
  }
}
