#include <SPI.h>
#include <Wire.h>
#include <string.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  //屏幕
  pinMode(11, INPUT_PULLUP);                  //摇杆上的按钮，接在引脚D11

  display.setTextSize(1);                     //文本大小
  display.setTextColor(SSD1306_WHITE);        //文本颜色（只有白色）
}


String InputStr(String msg = "Input"){
  char Val = 65, TmpVal;              //Val表示选定的字符
  int Mode = 1;
  String InStr = "";                  //用于存储已输入的字符串
  
  while(1){
    display.clearDisplay();

    int valueX = int(analogRead(A1)); //摇杆X轴
    int valueY = int(analogRead(A0)); //摇杆Y轴
    int valueZ = int(digitalRead(11));//摇杆按钮

    if(valueY >= 600){                //摇杆向上拨动时，选择上一个字符
      Val += 1;
    } else if(valueY <= 50){          //摇杆向下拨动时，选择下一个字符
      Val -= 1;
    } else if(valueZ == 0){           //点击摇杆时，将选定的字符添加到已输入的字符串
      InStr += Val;
    } else if(valueX >= 600){         //左滑摇杆时，提交
      break;
    } else if(valueX <= 50){          //快捷跳转到字符
      Mode += 1;
      if(Mode == 1) Val = 97;         //跳转到a
      else if(Mode == 2) Val = 65;    //跳转到A
      else if(Mode == 3) Val = 48;    //跳转到1
      else if(Mode == 4) Val = 33;    //跳转到!
      else if(Mode == 5) Val = 1;     //跳转到特殊符号
      else if(Mode == 6) Mode = 0;    //避免超出可选模式范围
    }

    display.setCursor(0, 0);
    display.print(msg);
    display.print(InStr);

    display.drawLine(0, 50, 128, 50, WHITE);

    display.setCursor(0, 52);
    display.print("[ ");
    display.print(Val);
    display.print(" ]");
    display.display();
    delay(70);
  }
  return InStr;
}

void run(String code, String inputs = "A") {
  //display.clearDisplay();
  
  //String inputs = InputStr("Input [INPUT]");

  display.clearDisplay();
  display.setCursor(0, 0);
  int pointer, cnt = 0;
  int str[105];
  int i;

  for(int i = 0; i < 105; i ++) str[i] = 0;

  for(i = 0; i < code.length(); i ++) {
    switch(code[i]){
      case '>':
        pointer ++;
        break;
      case '<':
        pointer ++;
        break;
      case '+':
        str[pointer] += 1;
        break;
      case '-':
        str[pointer] -= 1;
        break;
      case '.':
        display.print((char)str[pointer]);
        break;
      case ',':
        str[pointer] = int(inputs[cnt]);
        cnt ++;
        break;
    }
    display.display();
  }
  display.display();
}

void loop() {
  String a, b = "";
  
  display.clearDisplay();
  display.setCursor(0, 0);
  a = InputStr("> ");
  
  int space_flag = 0;
  for(int i = 0; i <= sizeof(a) + 1; i ++){
    if(a[i] == ' ' && space_flag == 0){
      space_flag = 1;
      continue;
    }
    else if(space_flag != 0){
      b += a[i];
    }
  }
  
  run(",+.>,+.>,+.>,+.>,+.>,+.", b);
  delay(2000);
  display.display();
  delay(5000);
}
/*
输入样例：
+----------------+
|> aaa 114514    |
+----------------+
aaa指的是文件名（目前没有用）
114514指的是向程序输入的字符串

样例输出：
+----------------+
|225625          |
+----------------+
*/
