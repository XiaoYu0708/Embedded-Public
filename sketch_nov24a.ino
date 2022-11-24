#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <analogWrite.h>
#include <CString>
#include <iostream>

#define button 14

#define LEDB 17
#define LEDR 16
#define LEDG 4

#define USE_SERIAL Serial

char ssid[] = "WTY";
char password[] = "A940812A";
char url[] = "https://data.epa.gov.tw/api/v2/aqx_p_02?api_key=e8dd42e6-9b8b-43f8-991e-b3dee723a52d&limit=1000&sort=datacreationdate%20desc&format=JSON";
String saveName[77];
String saveSide[77];
int savePM[77];
int new_view, f = 0, z = 0;

void ISR() {
  P(z);
  z++;
  if (z >= 77) {
    z = 0;
  }
}
void setup() {
  pinMode(button, INPUT);
  attachInterrupt(button, ISR, FALLING);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("開始連線到SSID：");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("連線完成");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("啟動網頁連線");
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  //Serial.print("httpCode=");
  //Serial.println(httpCode);

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    //Serial.print("payload=");
    //Serial.println(payload.length());
    //USE_SERIAL.println(payload);
    // String input;

    DynamicJsonDocument doc(16384);

    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    for (JsonObject field : doc["fields"].as<JsonArray>()) {

      const char* field_id = field["id"];      // "site", "county", "pm25", "datacreationdate", "itemunit"
      const char* field_type = field["type"];  // "text", "text", "text", "text", "text"

      const char* field_info_label = field["info"]["label"];  // "測站名稱", "縣市名稱", "細懸浮微粒濃度", "資料建置日期", "測項單位"
    }

    const char* resource_id = doc["resource_id"];  // "c1f31192-babd-4105-b880-a4c2e23a3276"

    const char* extras_api_key = doc["__extras"]["api_key"];  // "e8dd42e6-9b8b-43f8-991e-b3dee723a52d"

    bool include_total = doc["include_total"];             // true
    const char* total = doc["total"];                      // "78"
    const char* resource_format = doc["resource_format"];  // "object"
    const char* limit = doc["limit"];                      // "1000"
    const char* offset = doc["offset"];                    // "0"

    const char* links_start = doc["_links"]["start"];
    const char* links_next = doc["_links"]["next"];

    int i = 0;
    for (JsonObject record : doc["records"].as<JsonArray>()) {

      const char* record_site = record["site"];                          // "大城", "富貴角", "麥寮", "關山", "馬公", "金門", "馬祖", "埔里", "復興", ...
      const char* record_county = record["county"];                      // "彰化縣", "新北市", "雲林縣", "臺東縣", "澎湖縣", "金門縣", "連江縣", "南投縣", ...
      const char* record_pm25 = record["pm25"];                          // "9", "10", "14", "4", "9", "7", "8", "13", "18", "21", ...
      const char* record_datacreationdate = record["datacreationdate"];  // "2022-11-17 17:00", "2022-11-17 ...
      const char* record_itemunit = record["itemunit"];                  // "μg/m3", "μg/m3", "μg/m3", "μg/m3", "μg/m3", ...
      int PM = atoi(record_pm25);

      saveName[i] = record_site;
      saveSide[i] = record_county;
      savePM[i] = atoi(record_pm25);
      i++;
    }
    http.end();
    Serial.println("資料存取完成");
  }
}
void Color(int R, int G, int B) {
  analogWrite(LEDR, R);
  analogWrite(LEDG, G);
  analogWrite(LEDB, B);
}
void P(int i) {
  Serial.print("第");
  Serial.print(i + 1);
  Serial.println("筆資料");
  Serial.print("地區為：");
  Serial.print(saveName[i]);
  Serial.print("，");
  Serial.println(saveSide[i]);
  Serial.print("PM2.5：");
  Serial.println(savePM[i]);
  int PM = savePM[i];
  if (PM >= 251) {
    Serial.println("等級為紫色");
    Color(128, 0, 128);
  } else if (PM >= 55) {
    Serial.println("等級為紅色");
    Color(128, 0, 0);
  } else if (PM >= 36) {
    Serial.println("等級為橘色");
    Color(210, 100, 0);
  } else if (PM >= 16) {
    Serial.println("等級為黃色");
    Color(200, 200, 20);
  } else if (PM >= 0) {
    Serial.println("等級為綠色");
    Color(0, 128, 0);
  }
  Serial.println();
}

