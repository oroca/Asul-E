#include <SoftwareSerial.h>

//블루투스 핀
const int btTXD = 2;
const int btRXD = 3;
//LED 핀
const int ledPin = 13;

//블루투스용 소프트웨어 시리얼 포트 설정
SoftwareSerial btSerial(btTXD, btRXD);

// 시리얼 통신 프로토콜 사용도리 변수
String serString="";

//블루투스 시리얼 통신 처리 함수
void btProc()
{
  //시리얼 데이터가 있는지 체크
  //없으면 그냥 빠져나감.
  if(btSerial.available() <= 0)
    return;

  //데이터 있으면 1바이트 읽어옴.
  char ch = btSerial.read();

// 변수에 추가함.
  serString += ch;

  // 입력 받은 데이터가 '\n' newline 이면...
  if(ch == '\n')
  {
    //여지까지 입력 받은 데이터가 있는지 체크
    if(serString.length()>0)
    {
      //명령어 시작('$') 위치 찾기
      int startCMD = serString.indexOf("$");

      //명령어 시작위치를 찾았다면
      if(startCMD != -1)
      {
        //Serial.print(serString);

        String serArray[10];
        int tmpcnt=0;
        int idx;

        //명령어 시작 값('$') 제거
        String tmpString=serString.substring(startCMD+1);

        //공백 제거
        tmpString.trim();

        // 데이터 파싱
        while(tmpString.length() > 0)
        {
          // ','를 기준으로 짤라서 serArray에 저장.
          idx = tmpString.indexOf(",");
          if(idx == -1)
          {
            //','없다면 마지막 데이터 저장 후 빠져나감.
            serArray[tmpcnt] = tmpString;
            serArray[tmpcnt].trim();
            tmpcnt++;
            break;
          }

          //데이터 배열에 저장
          serArray[tmpcnt] = tmpString.substring(0, idx);

          //
          tmpString = tmpString.substring(idx+1);
          //
          tmpString.trim();

          //
          serArray[tmpcnt].trim();

          tmpcnt++;
        }

//명령 serArray[0]이 led이면 LED 제어
        if(serArray[0].equalsIgnoreCase("led"))
        {
          //문자열을 숫자로 변환
          int onoff = serArray[1].toInt();

          //onoff가 1이면 LED ON
          if(onoff == 1) {
            digitalWrite(ledPin, HIGH);
          }else{
//onoff가 1이 아니면  LED OFF
            digitalWrite(ledPin, LOW);
          }
        }
      }
    }
    //입력 받은 변수 데이터 초기화
    serString = "";
  }
}

void setup() {
  //모니터 프로그램 셋팅 및 시작
  Serial.begin(9600);

  //블루투스 시리얼 셋팅 및 시작
  btSerial.begin(9600);

  //LED 포트 설정
  pinMode(ledPin, OUTPUT);

  //시작을 알림.
  Serial.println("Start The Bluetooth App");
}

void loop() {
  btProc();
}
