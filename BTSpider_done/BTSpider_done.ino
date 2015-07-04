#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

SoftwareSerial I2CBT(10,11);//定義PIN10及PIN11分別為RX及TX腳位
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150
#define SERVOMAX  560

uint8_t servonum = 0;
int i,j,k;

int RB1 = 0,RB2 = 1,RB3 = 2;
int LB1 = 3,LB2 = 4,LB3 = 5;
int LF1 = 6,LF2 = 7,LF3 = 8;
int RF1 = 9,RF2 = 10,RF3 = 11;
int Mfirepin=6;
int Mstatuspin=5;

void setup() {
  Serial.begin(9600); //Arduino起始鮑率：9600
  I2CBT.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);

  pinMode(13, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(5, INPUT);
  digitalWrite(3,LOW);
  digitalWrite(4,HIGH);

  start();
  delay(1000);
  stand();
  delay(1000);
}

void setServoPulse(uint8_t n, double pulse) {
  double pulselength;

  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); 
  Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); 
  Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}
//========角度換算========
void deg(int k,int a){
  a = map(a,0,180,150,560);
  pwm.setPWM(k,0,a);
}
//========右半邊========
void degr(int k,int a){
  a = map(a,0,180,150,560);
  pwm.setPWM(k,0,a);
}
//========左半邊========
void degl(int k,int a){
  a = map(a,180,0,150,560);
  pwm.setPWM(k,0,a);
}
//========end========

void loop() {
  byte cmmd[20];
  int insize;	
  while(1){
    digitalWrite(3,LOW);
    if ((insize=(I2CBT.available()))>0){  //讀取藍牙訊息
      Serial.print("input size = ");
      Serial.println(insize);
      for (int i=0; i<insize; i++){
        Serial.print(cmmd[i]=char(I2CBT.read()));
        Serial.print(" ");
      }
    }
    switch (cmmd[0]) { //讀取第一個字

    case 97: //97為"a"的ASCII CODE  此為測試用
      digitalWrite(13,HIGH);  //點亮LED
      break;

    case 98://98為"b"的ASCII CODE
      Serial.println("Get b");
      digitalWrite(13,LOW);   //熄滅LED此為測試用
      break;
    case 102://102為"f"的ASCII CODE
      FWD();
      break;
    case 115://115為"s"的ASCII CODE
      stand();
      break;
    case 107://107為"k"的ASCII CODE b:98
      BWD();
      break;
    case 114://114為"r"的ASCII CODE
      right();
      break;
    case 108://108為"l"的ASCII CODE
      left();
      break;
    case 101://101為"e"的ASCII CODE  //單發
      fire3();
      break;
    case 100://100為"d"的ASCII CODE  //連發
      fire();
      break;
    case 119://119為"w"的ASCII CODE 　順
      rr();
      break;
    case 120://120為"x"的ASCII CODE 　逆
      rl();
      break;
    case 106://120為"j"的ASCII CODE
      WEST();
      break;
    case 117://120為"u"的ASCII CODE
      EAST();
      break;
    case 105://105為"i"的ASCII CODE
       nofire();
    break;
    } //Switch
  } //while
}//loop

//========火箭炮系列========//
//連發
void fire(){
  digitalWrite(Mfirepin,HIGH);
  delay(1000);
  digitalWrite(Mfirepin,LOW);
}
//================//
void fire2(){
  digitalWrite(3,HIGH);
  delay(90);
  digitalWrite(3,LOW);
  delay(1000);
}
//=====================//
//單發
void fire3(){
  // 發射導彈
  int ms;
  digitalWrite(Mfirepin, HIGH);
  // 讀取導彈發射狀態
  do{
    delay(100);
    ms=digitalRead(Mstatuspin);
  }
  while(ms==1);
  
  digitalWrite(Mfirepin, LOW);
  delay(500);
  }
void nofire(){
  digitalWrite(Mfirepin, LOW);
}
//========火箭炮系列========//

void lay(){
  for(i = 0;i < 12;i++)
    deg(i,90);
}

//====初始動作====//
void start(){
  degr(RB1,90); degr(RB2,0); degr(RB3,90);

  degl(LB1,90); degl(LB2,0); degl(LB3,90);

  degl(LF1,90); degl(LF2, 0); degl(LF3,90);

  degr(RF1,90); degr(RF2,0); degr(RF3,90);  
}
void stand(){
  //RB腳
  degr(RB1,145); degr(RB2,35); degr(RB3,45);
  //LB腳
  degl(LB1,135); degl(LB2,25); degl(LB3,45);
  //LF腳
  degl(LF1,145); degl(LF2,35); degl(LF3,135);
  //RF腳
  degr(RF1,145); degr(RF2,35); degr(RF3,135);
}

//====RB腳前進系列1====//
void RBFWD_1(){
  degr(RB1,165);
  degr(RB2,65);
  degr(RB3,70);
}
//====RB腳前進系列2====//
void RBFWD_2(){
  degr(RB1,140);
  degr(RB2,35);
}
//====RB腳前進系列3====//
void RBFWD_3(){
  degr(RB3,20);
}
//====RB腳前進整合====//
void RBFWD(){     
  RBFWD_1();
  delay(250);
  //-------------------------------------------------------
  RBFWD_2();
  delay(250);
  //-------------------------------------------------------------
  RBFWD_3();
  delay(250);
}
//====LF腳前進系列1====//
void LFFWD_1(){
  degl(LF1,165);  //45->27
  degl(LF2,65);  //45->15
  degl(LF3,160);   //45->27
}
//====LF腳前進系列2====//
void LFFWD_2(){
  degl(LF1,140);  //27->45
  degl(LF2,35);  //117->135
}
//====LF腳前進系列3====//
void LFFWD_3(){
  degl(LF3,110);//27->63
}
//====LF腳前進整合====//
void LFFWD(){
  LFFWD_1();
  delay(150);
  LFFWD_2();
  delay(150);
  LFFWD_3();
  delay(150);
}
//====LB腳前進系列1====//
void LBFWD_1(){
  degl(LB3,20);//117->153
}
//====LB腳前進系列2====//
void LBFWD_2(){
  degl(LB1,155 );//125 -> 155
  degl(LB2,60);//35 -> 65
  degl(LB3,70);//45 -> 
}
//====LB腳前進系列3====//
void LBFWD_3(){
  degl(LB1,130); //27->45
  degl(LB2,25); //65->35
}
//====LB腳前進整合====//
void LBFWD(){
  LBFWD_1();
  delay(150);
  LBFWD_2();
  delay(150);
  LBFWD_3();
  delay(150);
}
//====RF腳前進系列1====//
void RFFWD_1(){
  degr(RF3,110);  // 153->117
}
//====RF腳前進系列2====//
void RFFWD_2(){
  degr(RF1,165);//135 -> 165
  degr(RF2,65);//35->65
  degr(RF3,160);//135->153
}
//====RF腳前進系列3====//
void RFFWD_3(){
  degr(RF1,140); //165->135
  degr(RF2,35); //63->45
}
//====RF腳前進整合====//
void RFFWD(){
  RFFWD_1();
  delay(150);
  RFFWD_2();
  delay(150);
  RFFWD_3();
  delay(150);
}
//====機器人前進!!====//
void FWD(){  
  RBFWD_1(); LFFWD_1(); LBFWD_1(); RFFWD_1();
  delay(250);

  RBFWD_2(); LFFWD_2(); LBFWD_2(); RFFWD_2();
  delay(250);

  RBFWD_3(); LFFWD_3(); LBFWD_3(); RFFWD_3();
  delay(250);
}
//====機器人前進結束====//
//-------------------------------------------------------

//=====RB腳後退系列1====//
void RBBWD_1(){
  degr(RB1,165);
  degr(RB2,65);
  degr(RB3,20);
}
//=====RB腳後退系列2====//
void RBBWD_2(){
  degr(RB1,140);
  degr(RB2,35);
}
//=====RB腳後退系列3====//
void RBBWD_3(){
  degr(RB3,70);
}
//=====RB腳後退整合====//
void RBBWD(){     //3畾萄???
  RBBWD_1();
  delay(250);
  //-------------------------------------------------------
  RBBWD_2();
  delay(250);
  //-------------------------------------------------------------
  RBBWD_3();
  delay(250);
}
//=====LF腳後退系列1====//
void LFBWD_1(){
  degl(LF1,165);  //45->27
  degl(LF2,65);  //45->15
  degl(LF3,110);   //45->27
}
//=====LF腳後退系列2====//
void LFBWD_2(){
  degl(LF1,140);  //27->45
  degl(LF2,35);  //117->135
}
//=====LF腳後退系列3====//
void LFBWD_3(){
  degl(LF3,160);//27->63
}
//=====LF腳後退整合====//
void LFBWD(){
  LFBWD_1();
  delay(150);
  LFBWD_2();
  delay(150);
  LFBWD_3();
  delay(150);
}
//=====LB腳後退系列1====//
void LBBWD_1(){
  degl(LB3,70);//117->153
}
//=====LB腳後退系列2====//
void LBBWD_2(){
  degl(LB1,155 );//125 -> 155
  degl(LB2,60);//35 -> 65
  degl(LB3,20);//45 -> 
}
//=====LB腳後退系列3====//
void LBBWD_3(){
  degl(LB1,130); //27->45
  degl(LB2,25); //65->35
}
//=====LB腳後退整合====//
void LBBWD(){
  LBBWD_1();
  delay(150);
  LBBWD_2();
  delay(150);
  LBBWD_3();
  delay(150);
}

//=====RF腳後退系列1====//
void RFBWD_1(){
  degr(RF3,160);  // 153->117
}
//=====RF腳後退系列2====//
void RFBWD_2(){
  degr(RF1,165);//135 -> 165
  degr(RF2,65);//35->65
  degr(RF3,110);//135->153
}
//=====RF腳後退系列3====//
void RFBWD_3(){
  degr(RF1,140); //165->135
  degr(RF2,35); //63->45
}
//=====RF腳後退整合====//
void RFBWD(){
  RFBWD_1();
  delay(150);
  RFBWD_2();
  delay(150);
  RFBWD_3();
  delay(150);
}
//====機器人後退====//
void BWD(){  
  RBBWD_1(); LFBWD_1(); LBBWD_1(); RFBWD_1();
  delay(250);

  RBBWD_2(); LFBWD_2(); LBBWD_2(); RFBWD_2();
  delay(250);

  RBBWD_3(); LFBWD_3();  LBBWD_3(); RFBWD_3();
  delay(250);
}
//====機器人後退結束====//

//====機器人向左移動====//
void left(){
  RBFWD_1(); LFLFT_1(); LBLFT_1(); RFFWD_1();
  delay(250);

  RBFWD_2(); LFLFT_2(); LBLFT_2(); RFFWD_2();
  delay(250);

  //RBBWD_3(); LFBWD_3();  LBBWD_3(); RFBWD_3();
  RBFWD_3(); LFLFT_3();  LBLFT_3(); RFFWD_3();
  delay(250);
}
//====LF腳左前進系列1====//
void LFLFT_1(){
  degl(LF1,165);  //45->27
  degl(LF2,65);  //45->15
  degl(LF3,180);   //45->27
}
//====LF腳左前進系列2====//
void LFLFT_2(){
  degl(LF1,140);  //27->45
  degl(LF2,35);  //117->135
}
//====LF腳左前進系列3====//
void LFLFT_3(){
  degl(LF3,90);//27->63
}
//====LF腳左前進整合====//
void LFLFT(){
  LFLFT_1();
  delay(150);
  LFLFT_2();
  delay(150);
  LFLFT_3();
  delay(150);
}
//====LB腳左前進系列1====//
void LBLFT_1(){
  degl(LB3,0);//117->153
}
//====LB腳左前進系列2====//
void LBLFT_2(){
  degl(LB1,155 );//125 -> 155
  degl(LB2,60);//35 -> 65
  degl(LB3,90);//45 -> 
}
//====LB腳左前進系列3====//
void LBLFT_3(){
  degl(LB1,130); //27->45
  degl(LB2,25); //65->35
}
//====LB腳左前進整合====//
void LBLFT(){
  LBLFT_1();
  delay(150);
  LBLFT_2();
  delay(150);
  LBLFT_3();
  delay(150);
}
//====機器人向左移動結束====//

//====機器人向右移動====//
void right(){
  RBRIT_1(); LFFWD_1(); LBFWD_1(); RFRIT_1();
  delay(250);

  RBRIT_2(); LFFWD_2(); LBFWD_2(); RFRIT_2();
  delay(250);
  
  RBRIT_3(); LFFWD_3();  LBFWD_3(); RFRIT_3();
  delay(250);
}
//====RF腳右前進系列1====//
void RFRIT_1(){
  degr(RF3,90);  // 153->117
}
//====RF腳右前進系列2====//
void RFRIT_2(){
  degr(RF1,165);//135 -> 165
  degr(RF2,65);//35->65
  degr(RF3,170);//135->153
}
//====RF腳右前進系列3====//
void RFRIT_3(){
  degr(RF1,140); //165->135
  degr(RF2,35); //63->45
}
//====RF腳右前進整合====//
void RFRIT(){
  RFRIT_1();
  delay(150);
  RFRIT_2();
  delay(150);
  RFRIT_3();
  delay(150);
}
//====RB腳右前進系列1====//
void RBRIT_1(){
  degr(RB1,165);
  degr(RB2,65);
  degr(RB3,90);
}
//====RB腳右前進系列2====//
void RBRIT_2(){
  degr(RB1,140);
  degr(RB2,35);
}
//====RB腳右前進系列3====//
void RBRIT_3(){
  degr(RB3,0);
}
//====RB腳右前進整合====//
void RBRIT(){    
  RBRIT_1();
  delay(250);
  //-------------------------------------------------------
  RBRIT_2();
  delay(250);
  //-------------------------------------------------------------
  RBRIT_3();
  delay(250);
}
//====機器人向右移動結束====//


//-----------------------------------------------------------------
//自轉回正函數，歸位用
void comeback(){
  degr(RB3,45);
  degr(RF3,135);
  degl(LB3,45);
  degl(LF3,135);
}

//======機器人自轉右=====//
//RB腳自轉右動作1
void RBrr_1(){
  degr(RB1,165);
  degr(RB2,65);
  degr(RB3,90);
}
//RB腳自轉右動作2
void RBrr_2(){
  degr(RB1,140);
  degr(RB2,35); 
}
//RB腳自轉右動作整合
void RBrr(){
  RBrr_1();
  delay(300);
  RBrr_2();
  delay(300);
  comeback();
  delay(300);
}
//LB腳自轉右動作1
void LBrr_1(){
  degl(LB1,155);
  degl(LB2,60);
  degl(LB3,0);
}
//LB腳自轉右動作2
void LBrr_2(){
  degl(LB1,130); 
  degl(LB2,25); 
}
//LB腳自轉右動作整合
void LBrr(){
  LBrr_1();
  delay(300);
  LBrr_2();
  delay(300);
  comeback();
  delay(300);
}
//LB腳自轉右動作1
void LFrr_1(){
  degl(LF1,165); 
  degl(LF2,65); 
  degl(LF3,90);
}
//LB腳自轉右動作2
void LFrr_2(){
  degl(LF1,140); 
  degl(LF2,35);
}
//LF腳自轉右動作整合
void LFrr(){
  LFrr_1();
  delay(300);
  LFrr_2();
  delay(300);
  comeback();
  delay(300);
}
//
void RFrr_1(){
  degr(RF1,165);
  degr(RF2,65);
  degr(RF3,180);
}
//
void RFrr_2(){
  degr(RF1,140); 
  degr(RF2,35); 
}
//
void RFrr(){
  RFrr_1();
  delay(300);
  RFrr_2();
  delay(300);
  comeback();
  delay(300);
}
//機器人自轉右動作
void rr(){
  RBrr_1();
  delay(300);
  RBrr_2();
  delay(300);
  RFrr_1();
  delay(300);
  RFrr_2();
  delay(300);
  LFrr_1();
  delay(300);
  LFrr_2();
  delay(300);
  LBrr_1();
  delay(300);
  LBrr_2();
  delay(300);
  comeback();
  delay(300);
}
//自轉右結束
//------------------------------
//======機器人自轉左=====//
//RB腳自轉左動作1
void RBrl_1(){
  degr(RB1,165);
  degr(RB2,65);
  degr(RB3,0);
}
//RB腳自轉左動作2
void RBrl_2(){
  degr(RB1,140);
  degr(RB2,35); 
}
//RB腳自轉左動作整合
void RBrl(){
  RBrl_1();
  delay(300);
  RBrl_2();
  delay(300);
  comeback();
  delay(300);
}
//LB腳自轉左動作1
void LBrl_1(){
  degl(LB1,155);
  degl(LB2,60);
  degl(LB3,90);
}
//LB腳自轉左動作2
void LBrl_2(){
  degl(LB1,130); 
  degl(LB2,25); 
}
//LB腳自轉左動作整合
void LBrl(){
  LBrr_1();
  delay(300);
  LBrr_2();
  delay(300);
  comeback();
  delay(300);
}
//LB腳自轉左動作1
void LFrl_1(){
  degl(LF1,165); 
  degl(LF2,65); 
  degl(LF3,180);
}
//LB腳自轉左動作2
void LFrl_2(){
  degl(LF1,140); 
  degl(LF2,35);
}
//LF腳自轉左動作整合
void LFrl(){
  LFrl_1();
  delay(300);
  LFrl_2();
  delay(300);
  comeback();
  delay(300);
}
//
void RFrl_1(){
  degr(RF1,165);
  degl(LB2,90);
  degl(LB1,0);
  degr(RF2,85);
  degr(RF3,90);
  degl(LB1,155);
  degl(LB2,60);
}
//
void RFrl_2(){
  degr(RF1,140); 
  degr(RF2,35); 
}
//
void RFrl(){
  RFrl_1();
  delay(300);
  RFrl_2();
  delay(300);
  comeback();
  delay(300);
}
//機器人自轉左動作
void rl(){
  RBrl_1();
  delay(300);
  RBrl_2();
  delay(300);
  LBrl_1();
  delay(300);
  LBrl_2();
  delay(300);
  LFrl_1();
  delay(300);
  LFrl_2();
  delay(300);
  RFrl_1();
  delay(300);
  RFrl_2();
  delay(300);
  comeback();
  delay(300);
}
//自轉左結束
//------------------------------------
//====RB腳平移右系列1====//
void RB_E_1(){
  degr(RB1,165);
  degr(RB2,65);
  degr(RB3,10);
}
//====RB腳平移右系列2====//
void RB_E_2(){
  degr(RB1,140);
  degr(RB2,35);
}
//====RB腳平移右系列3====//
void RB_E_3(){
  degr(RB3,60);
}
//====RB腳平移右整合====//
void RBEST(){     
  RB_E_1();
  delay(250);
  //-----------------------------------------
  RB_E_2();
  delay(250);
  //-----------------------------------------
  RB_E_3();
  delay(250);
}

//====RF腳平移右系列1====//
void RF_E_1(){
  degr(RF3,120);  
}
//====RF腳平移右系列2====//
void RF_E_2(){
  degr(RF1,165);
  degr(RF2,65);
  degr(RF3,170);
}
//====RF腳平移右系列3====//
void RF_E_3(){
  degr(RF1,140); 
  degr(RF2,35); 
}
//====RF腳平移右整合====//
void RFEST(){
  RF_E_1;
  delay(150);
  RF_E_2();
  delay(150);
  RF_E_3();
  delay(150);
}

//====LF腳平移右系列1====//
void LF_E_1(){
  degl(LF1,165);  //45->27
  degl(LF2,65);  //45->15
  degl(LF3,170);   //45->27
}
//====LF腳平移右系列2====//
void LF_E_2(){
  degl(LF1,140);  //27->45
  degl(LF2,35);  //117->135
}
//====LF腳平移右系列3====//
void LF_E_3(){
  degl(LF3,120);//27->63
}
//====LF腳平移右整合====//
void LFEST(){
  LF_E_1();
  delay(150);
  LF_E_2();
  delay(150);
  LF_E_3();
  delay(150);
}

//====LB腳平移右系列1====//
void LB_E_1(){
  degl(LB3,10);//117->153
}
//====LB腳平移右系列2====//
void LB_E_2(){
  degl(LB1,155 );//125 -> 155
  degl(LB2,60);//35 -> 65
  degl(LB3,60);//45 -> 
}
//====LB腳平移右系列3====//
void LB_E_3(){
  degl(LB1,130); //27->45
  degl(LB2,25); //65->35
}
//====LB腳平移右整合====//
void LBEST(){
  LB_E_1();
  delay(150);
  LB_E_2();
  delay(150);
  LB_E_3();
  delay(150);
}
//平移右開始
void EAST(){
  RB_E_1();RB_E_2();RB_E_3();delay(250);
  RF_E_1();RF_E_2();RF_E_3();delay(250);
  LF_E_1();LF_E_2();LF_E_3();delay(250);
  LB_E_3();LB_E_3();LB_E_3();delay(250);
}
//平移右結束
//---------------------------------------
//====RB腳平移左系列1====//
void RB_W_1(){
  degr(RB1,165);
  degr(RB2,65);
  degr(RB3,80);
}
//====RB腳平移左系列2====//
void RB_W_2(){
  degr(RB1,140);
  degr(RB2,35);
}
//====RB腳平移左系列3====//
void RB_W_3(){
  degr(RB3,30);
}
//====RB腳平移左整合====//
void RBWST(){
  RB_W_1();
  delay(250);
  //-----------------------------------------
  RB_W_2();
  delay(250);
  //-----------------------------------------
  RB_W_3();
  delay(250);
}

//====RF腳平移左系列1====//
void RF_W_1(){
  degr(RF3,170);  // 153->117
}
//====RF腳平移左系列2====//
void RF_W_2(){
  degr(RF1,165);//135 -> 165
  degr(RF2,65);//35->65
  degr(RF3,120);//135->153
}
//====RF腳平移左系列3====//
void RF_W_3(){
  degr(RF1,140); //165->135
  degr(RF2,35); //63->45
}
//====RF腳平移左整合====//
void RFWST(){
  RF_W_1();
  delay(150);
  RF_W_2();
  delay(150);
  RF_W_3();
  delay(150);
}

//====LF腳平移左系列1====//
void LF_W_1(){
  degl(LF1,165);  //45->27
  degl(LF2,65);  //45->15
  degl(LF3,80);   //45->27
}
//====LF腳平移左系列2====//
void LF_W_2(){
  degl(LF1,140);  //27->45
  degl(LF2,35);  //117->135
}
//====LF腳平移左系列3====//
void LF_W_3(){
  degl(LF3,30);//27->63
}
//====LF腳平移左整合====//
void LFWST(){
  LF_W_1();
  delay(150);
  LF_W_2();
  delay(150);
  LF_W_3();
  delay(150);
}

//====LB腳前進系列1====//
void LB_W_1(){
  degl(LB3,80);//117->153
}
//====LB腳前進系列2====//
void LB_W_2(){
  degl(LB1,155 );//125 -> 155
  degl(LB2,60);//35 -> 65
  degl(LB3,10);//45 -> 
}
//====LB腳前進系列3====//
void LB_W_3(){
  degl(LB1,130); //27->45
  degl(LB2,25); //65->35
}
//====LB腳前進整合====//
void LBWST(){
  LB_W_1();
  delay(150);
  LB_W_2();
  delay(150);
  LB_W_3();
  delay(150);
}
//平移左開始
void WEST(){
  LF_W_1();LF_W_2();LF_W_3();delay(250);
  LB_W_3();LB_W_3();LB_W_3();delay(250);
  RB_W_1();RB_W_2();RB_W_3();delay(250);
  RF_W_1();RF_W_2();RF_W_3();delay(250);
}
//平移左結束

