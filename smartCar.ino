#include <ESP8266WiFi.h>

#define Echo D2
#define Tring D8
#define Dong1 D6
#define Dong2 D7
#define Zhuan1 D5
#define Zhuan2 D4


char* ssid = "CMCC-****";
char* password = "1***********";

int port = 8888; // 端口号
WiFiServer server(port);

long getTime()
{
  digitalWrite(Tring, HIGH);
  delayMicroseconds(10);
  digitalWrite(Tring, LOW);

  return pulseIn(Echo, HIGH);
}

void initUltrasonic()
{
  pinMode(Echo, INPUT);
  pinMode(Tring, OUTPUT);
}

void initWiFiSta()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED ) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(WiFi.localIP());
}

void initL9110s()
{
  pinMode(Dong1, OUTPUT);
  pinMode(Dong2, OUTPUT);
  pinMode(Zhuan1, OUTPUT);
  pinMode(Zhuan2, OUTPUT);
}

void carZhuan()
{
  digitalWrite(Zhuan1, LOW);
  digitalWrite(Zhuan2, LOW);
}

void carPark()
{
  digitalWrite(Dong1, LOW);
  digitalWrite(Dong2, LOW);
}
void carUp()
{
  digitalWrite(Dong1, HIGH);
  digitalWrite(Dong2, LOW);
}
void carDown()
{
  digitalWrite(Dong1, LOW);
  digitalWrite(Dong2, HIGH);
}

void carRight()
{
  digitalWrite(Zhuan1, LOW);
  digitalWrite(Zhuan2, HIGH);
}

void carLeft()
{
  digitalWrite(Zhuan1, HIGH);
  digitalWrite(Zhuan2, LOW);
}

void setup() {
  initL9110s();
  initUltrasonic();
  Serial.begin(115200);
  initWiFiSta();
  server.begin();
}

void loop() {
  char cmd;
  int mark = 0;
  long dis;


  WiFiClient client = server.available();//服务初始化
  while (client.connected()) {
    while (client.available() > 0) {
      cmd = client.read();
      dis = getTime() / 58;
      if (dis < 10) {
        carDown();
        delay(200);
        carPark();
        mark = 1;
      } else {
        mark = 0;
      }
      if (mark == 0) {
        switch (cmd) {
          case 'p':
            carPark();
            break;
          case 'z':
            carZhuan();
            break;
          case 'u':
            carUp();
            break;
          case 'd':
            carDown();
            break;
          case 'r':
            carRight();
            break;
          case 'l':
            carLeft();
            break;
        }
      }
    }
  }
}
