#include <SoftwareSerial.h>

//핀 설정
#define BT_TXD 2
#define BT_RXD 3

//소프트웨어 시리얼 포트 설정
SoftwareSerial BTSerial(BT_TXD, BT_RXD);

void setup() {
//모니터 프로그램 셋팅 및 시작
  Serial.begin(9600);

  //블루투스 시리얼 셋팅 및 시작
  BTSerial.begin(9600);

  Serial.println("Start The Bluetooth App");
}

void loop() {
//블루투스로 데이터가 들어온 것이 있다면
if(BTSerial.available()){
//모니터프로그램으로 다시 전송
    Serial.write(BTSerial.read());
  }

//모니터 프로그램에서 입력 받은 데이터가 있다면
if(Serial.available()){
//블루투스로 전송
    BTSerial.write(Serial.read());
  }
}
