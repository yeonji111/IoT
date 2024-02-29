// 중복 import 방지용 코드
#ifndef HTML_PAGE
#define HTML_PAGE

/* 
HTML 소스 코드

HTML/CSS/JS 코드가 맞으나
html파일 지원이 안됨으로 String 객체형태로 취급
*/

class HTMLPage{

  private:
    //항상 쓰이는 헤더 부분
    String header = String("<!DOCTYPE HTML>") +
                "<html> <head>" + 
                "<title>Jimin's IOT</title>\r\n"+
                "<style>\r\n"+
                ".a:hover{cursor:pointer;}\r\n"+
                "div {margin-top:3%;}\r\n"+
                "input {margin-top:3%; font-size: 1.2rem;}\r\n"+
                "li {margin-top:3%; font-size: 1.2rem;}\r\n"+
                "</style></head>\r\n";
  public:

    //body 부분
    String getReturn = header + 
    "<body><div style='text-align:center; margin-top:10% font-size:2rem;'><a href='/' style='font-size:2rem;'> 설정완료됨 돌아가기 </a></div></body>";

    String getWIFIList(String strList)
    {
      String htmlPage = header +
                "<body>\r\n"+
                "<div id='wifiList'>"+ strList + "</div>\r\n"
                "<div id='selected'></div>\r\n"
                "<div id='_pw'>\r\n"+
                "<form method='get' action='/action_page'>\r\n"+
                "<input type='text' id='ap' name='apName'> 을 선택하셨습니다. 아래에 비밀번호를 입력하세요. <br>\r\n"+
                "<input type='password' id='pw' name='apPw'> <input type='submit' value='connect'>\r\n"+
                "</form>\r\n"+
                "</div>\r\n"+
                "</body>\r\n"+
                "<script>\r\n"+
                "document.getElementById('_pw').style.visibility = 'hidden';\r\n"+
                "function select(apName){\r\n"+
                "document.getElementById('ap').value = apName;\r\n"+
                "document.getElementById('_pw').style.visibility = 'visible';\r\n"+
                "document.getElementById('pw').value = ''}\r\n"+
                "function connection(){ \r\n"+
                "var pw = document.getElementById('pw').value;\r\n"+
                "} \r\n"+ 
                "</script> \r\n"+
                "</html> \r\n";

      return htmlPage;
    };
};



#endif