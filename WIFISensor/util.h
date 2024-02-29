// 중복 import 방지용 코드
#ifndef UTIL
#define UTIL


bool isValidate(String data);
void swap(String* a, String* b);


//빈문자열인지 이상한 문자열인지 null인지 empty인지 검사
//멀쩡하면 true, 쬐끔이라도 이상하면 false 반환
bool isValidate(String data)
{
  return (data != NULL) //null은 절대안됨
  && (!data.isEmpty()) //제공되는 함수
  && (data != " ") //공백문자 안됨
  && (data != "")  //비어있어도 안됨
  && (data != "0")
  ;
}

//전치 전도 함수
void swap(String* a, String* b)
{
  String temp = *a;
  *a = *b;
  *b = temp;
}

#endif