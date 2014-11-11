/*
  最終課題プログラム用
  （かなり無理ゲー仕様）
  344973 西井　洵之介
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<eggx.h>

#define nfontsize 20//基本的な文字サイズを統一
#define cury 360.0//カーソルのy座標。基本的に変更しないため定数で。
#define countreset 30//主人公（敵）の移動用。

int win,xstar[10],ystar[10];//星の位置指定。
int prest;//自分の残り用
int enerest,penerest;//敵の残り用
float xpla,ypla,curx;//主人公（敵）のx、y座標とカーソルのx座標。いろいろ使う
int enecheck[3];float xene[3],yene[3];//敵の座標や表示されているか等
int  bombcheck[10];float bombtime[10],xbomb[10],ybomb[10];//爆発演出用
int enefirecheck[5];float xenefire[5],yenefire[5];//弾が表示されているかとその座標
float xplafire[3];float yplafire[3];//自分の弾用
int pfirecheck[10];float xpfirehit[10][3],ypfirehit[10][3];//強い敵（自分）の出す大きい弾の判定用

void gameover(void);//ゲームオーバー用プロトタイプ。
void clear(void);//ゲームクリア用プロトタイプ。正直に言うとプロトタイプの使い道がなかった

int title(void){//タイトル画面の処理
  int x,y;//文字の位置指定用
  int key;//キー入力用
  int back;//戻り値用

  newpen(win,1);
  x=150;y=310;//位置指定
  drawstr(win,x,y,30,0.0,"VS.PLAYER");//文字描画
  x=135;y=210;//位置指定
  drawstr(win,x,y,nfontsize,0.0,"cursor key:move");//文字描画
  x=160;y=190;//位置指定
  drawstr(win,x,y,nfontsize,0.0,"A:enemy(A)");//文字描画
  x=160;y=170;//位置指定
  drawstr(win,x,y,nfontsize,0.0,"S:enemy(B)");//文字描画
  x=170;y=120;//位置指定
  drawstr(win,x,y,nfontsize,0.0,"1=START");//文字描画
  x=170;y=70;//位置指定
  drawstr(win,x,y,nfontsize,0.0,"3=EXIT");//文字描画
  while(1){//入力用の繰り返し。特定のキーのみ入力できるようにしたら少し面倒な感じになってしまった。
    key=ggetch();
    if(key==49){
      back=0;
      break;
    }
    if(key==50){
      back=1;
      break;
    }
    if(key==51){
      back=2;
      break;
    }
  }//キーボードからの入力を待つ。
  
  return back ;
}

void starreset(void){//星の位置リセット
  int a;//繰り返しに使用

  newpen(win,1);
  
  for(a=0;a<=9;a++){//星の位置完全リセット
    xstar[a]=(rand() % 400);
    ystar[a]=(rand() % 400);
  }
  return;//戻り値なし
}

void star(void){//星を降らせる処理
  int a,b=2;//aは繰り返し用、bはサイズ指定
  newpen(win,1);
  for(a=0;a<=9;a++){//星を１０個流す
    fillrect(win,xstar[a],ystar[a],b,b);
    ystar[a]=ystar[a]-2;
    if(ystar[a]<0){//星が画面端に移動した時の処理。ちょっと変則的に次の星が流れるようにする
      xstar[a]=(rand() % 400);
      ystar[a]=((rand() % 100)+400);
    }
  }
  return;
}
void monku(void){//ゲーム開始時に文字を表示させるあたりの処理
  newpen(win,2);
  drawstr(win,95,200,nfontsize,0.0,"PLAYER 1 STRIKES BACK!");//文字表記
  if(ypla<=40.0){
    ypla++;
  }//ゲーム開始時は主人公（敵）が下からやってくるようにした。
  return;
}
void rest(void){//敵（自分）の残り表示用
  static float xsample[3],ysample[3];//サイズ指定用

  xsample[0]=2.0;ysample[0]=393.0;
  xsample[1]=12.0;ysample[1]=375.6795;
  xsample[2]=22.0;ysample[2]=393.0;
  
  newpen(win,7);
  fillpoly(win,xsample,ysample,3,0);
  newpen(win,1);
  drawstr(win,24.0,380.0,nfontsize,0.0,"=%d",enerest);
  
  xsample[0]=80.0;
  xsample[1]=85.0;ysample[1]=384.3398;
  xsample[2]=90.0;
  newpen(win,6);

  fillpoly(win,xsample,ysample,3,0);

  ysample[0]=384.3398;
  ysample[1]=375.6796;
  ysample[2]=384.3398;
  
  fillpoly(win,xsample,ysample,3,0);
  newpen(win,1);
  drawstr(win,92.0,380.0,nfontsize,0.0,"=%d",penerest);
}//場所指定して正三角形と文字を描いているだけ

void plarest(void){//主人公（敵）の残り表示用
  static float xsample[3],ysample[3];//サイズ指定用
  
  newpen(win,2);
  xsample[0]=2.0;ysample[0]=2.0;
  xsample[1]=6.0;ysample[1]=6.9282;
  xsample[2]=10.0;ysample[2]=2.0;

  fillpoly(win,xsample,ysample,3,0);

  xsample[0]=10.0;ysample[0]=2.0;
  xsample[1]=20.0;ysample[1]=17.3205;
  xsample[2]=30.0;ysample[2]=2.0;

  fillpoly(win,xsample,ysample,3,0);

  xsample[0]=30.0;ysample[0]=2.0;
  xsample[1]=34.0;ysample[1]=6.9282;
  xsample[2]=38.0;ysample[2]=2.0;

  fillpoly(win,xsample,ysample,3,0);

  newpen(win,1);
  drawstr(win,40.0,2.0,nfontsize,0.0,"=%d",prest);

  return;
}//restと同じく場所指定して正三角形と文字描いているだけ

void player(int mnk){//主人公（敵）に関する処理
  static int count;//ある程度移動したり、停止してから次ランダムでまた動くか止まるかを抽選するのを実現するための変数
  static int random=0;//乱数を代入
  float xl[3],yl[3];
  float xc[3],yc[3];
  float xr[3],yr[3];//xl~yrは主人公（敵）描画用変数
  if(mnk<=0){//完全に開始してからこの機能を使う
    if(count<=0){
      random=(rand() % 3);
      count=countreset;
    }
    if(random==0 && count>=0 && xpla>=5.0){
      xpla=xpla-2.0;
    }
    if(random==2 && count>=0 && xpla<=335.0){
      xpla=xpla+2.0;
    }
    count--;
  }//主人公（敵）はランダムに動く
  newpen(win,2);
  xl[0]=xpla;yl[0]=ypla;
  xl[1]=(xpla+7.5);yl[1]=(ypla+12.9903);
  xl[2]=(xpla+15);yl[2]=ypla;//左の小さい三角

  xc[0]=(xpla+15);yc[0]=ypla;
  xc[1]=(xpla+30);yc[1]=(ypla+25.9807);
  xc[2]=(xpla+45);yc[2]=ypla;
  
  xr[0]=(xpla+45);yr[0]=ypla;
  xr[1]=(xpla+52.5);yr[1]=(ypla+12.9903);
  xr[2]=(xpla+60);yr[2]=ypla;

  fillpoly(win,xl,yl,3,0);
  fillpoly(win,xc,yc,3,0);
  fillpoly(win,xr,yr,3,0);//主人公描画

  return;
}

void cur(void){//カーソルに関する処理
  int i;//繰り返し用
  float key,cx[3],cy[3];//keyはキー入力用、あとはカーソル描画用
  newpen(win,4);
  key=ggetch();
  if(key==28 && curx<=380.0){//左キーが押された時
    curx=curx+15.0;//カーソル左に移動
  }
  if(key==29 && curx>=5.0){//右キーが押された時
    curx=curx-15.0;//カーソル右に移動
  }
  if((key==97) || (key==115)){//AもしくはSキーが押された時
    for(i=0;i<=2;i++){//敵（自分）が表示されるのは３体までというのを実現するためのfor
      if((enecheck[i]==0) && (key==97) && (enerest>0)){//Aキーの場合の処理
	enerest=enerest-1;
	enecheck[i]=1;
	xene[i]=curx-5.0;
	yene[i]=420.0;
	break;
      }
	 if((enecheck[i]==0) && (key==115) && (penerest>0)){//Sキーの場合の処理
	penerest=penerest-1;
	enecheck[i]=2;
	xene[i]=curx-5.0;
	yene[i]=436.0;
	break;
      }
    }
  }
      
  cx[0]=curx;cy[0]=cury;
  cx[1]=(curx+5.0),cy[1]=(cury-8.6602);
  cx[2]=(curx+10.0),cy[2]=cury;
  
  fillpoly(win,cx,cy,3,0);
  return;
  }

void pfirehit(int back,float x,float y){//強攻撃の当たり判定を作る関数
  if(pfirecheck[back]==0){
    pfirecheck[back]=1;
    xpfirehit[back][0]=x-42.6;ypfirehit[back][0]=y-42.6;
    xpfirehit[back][1]=x;ypfirehit[back][1]=y-60.0;
    xpfirehit[back][2]=x+42.6;ypfirehit[back][2]=y-42.6;//当たり判定は3つあればどれか引っかかると思ったため３つだけ作った
  }
  return;
}

  
void enefireche(int type,float x,float y){//攻撃の種類によって動作を振り分ける関数
  int a;//繰り返し用
  for(a=0;a<=4;a++){
    if(enefirecheck[a]==0 && type==1){//弱攻撃用
      enefirecheck[a]=1;
      xenefire[a]=x+10;yenefire[a]=y-17.0;
      break;
    }
    if(enefirecheck[a]==0 && type==2){//強攻撃用
      enefirecheck[a]=2;
      xenefire[a]=x+10;yenefire[a]=y-34.0;
      pfirehit(a,xenefire[a],yenefire[a]);
      break;
    }
  }
  return; 
}

void enemy(void){//敵（自分）に関する処理
  static int a;//繰り返し用
  static float xenedraw[3],yenedraw[3];//敵（自分）を描画する際のx位置とy位置を設定する用
  static float ypenedraw[3];//強い方を出した時のy座標を設定する用
  for(a=0;a<=2;a++){//敵（自分
    if(enecheck[a]==1){
      xenedraw[0]=xene[a];yenedraw[0]=yene[a];
      xenedraw[1]=xene[a]+10.0;yenedraw[1]=yene[a]-17.3205;
      xenedraw[2]=xene[a]+20.0;yenedraw[2]=yene[a];
      
      newpen(win,7);
      fillpoly(win,xenedraw,yenedraw,3,0);//弱敵（自分）描画
      
      yene[a]=yene[a]-0.8;
      if(((yene[a]<=340.5) && (yene[a]>=339.5)) || ((yene[a]<=200.5) && (yene[a]>=199.5))){//弾は一つの敵から最大2つ出る
	enefireche(1,xene[a],yene[a]);
      }
      if(yene[a]<=0.0){
	enecheck[a]=0;
      }
    }
    if(enecheck[a]==2){
      xenedraw[0]=xene[a];yenedraw[0]=yene[a];
      xenedraw[1]=xene[a]+10.0;yenedraw[1]=yene[a]-17.3205;
      xenedraw[2]=xene[a]+20.0;yenedraw[2]=yene[a];
      
      ypenedraw[0]=yene[a]-17.3205;
      ypenedraw[1]=yene[a]-34.6410;
      ypenedraw[2]=yene[a]-17.3205;
      
      newpen(win,6);
      fillpoly(win, xenedraw,yenedraw,3,0);
      fillpoly(win,xenedraw,ypenedraw,3,0);//強敵（自分）描画
      
      if(((yene[a]<=340.5) && (yene[a]>=339.5)) || ((yene[a]<=200.5) && (yene[a]>=199.5))){//同じく一つの敵から最大2つ
	enefireche(2,xene[a],yene[a]);
      }
      yene[a]=yene[a]-0.8;
      if(yene[a]<=0.0){//画面外で消える
	enecheck[a]=0;
      } 
    }
  }
  
  return;
}

void bombdraw(void){//爆発を表示させる関数
  int a;//繰り返し用
  newpen(win,1);
  for(a=0;a<=9;a++){
    if(bombcheck[a]==1){
      fillcirc(win,xbomb[a],ybomb[a],40,40);
      bombtime[a]--;//時間を減らしていく
      if(bombtime[a]<=0){//時間が0になると表示が消える
	bombcheck[a]=0;
      }
    }
  }
  return;
}
  

void bombcount(float x,float y){//爆発する座標を設定する関数
  int a;//繰り返し用
  for(a=0;a<=9;a++){
    if(bombcheck[a]==0){
      bombcheck[a]=1;
      bombtime[a]=50;//爆発を表示させる時間を設定
      xbomb[a]=x;ybomb[a]=y;
      break;
    }
  }
  return;
}

void phitcheck(void){//主人公（敵）用当たり判定
  int a,b;//繰り返し用
  int xplacenter,yplacenter;//プレイヤー（敵）のほぼ真ん中を代入する
  for(a=0;a<=2;a++){
    if((xene[a]+10.0)>=(xpla+7.5) && (xene[a]+10.0)<=(xpla+52.5) && (yene[a]-8.6602)>=(ypla) && (yene[a]-8.6602)<=(ypla+12.9903) && enecheck[a]==1){//if内で当たり判定を決めている。敵とプレイヤーがぶつかった時用
      enecheck[a]=0;
      prest--;
      xplacenter=xpla+30.0;
      yplacenter=ypla+12.9803;
      if(prest>1){
	xpla=170.0;//主人公（敵）がミスした時は中心に戻るようにした。
      }     
      bombcount(xene[a],yene[a]);
      bombcount(xplacenter,yplacenter);
    }
    if((xene[a]+10.0)>=(xpla+7.5) && (xene[a]+10.0)<=(xpla+52.5) && (yene[a]-8.6602)>=(ypla) && (yene[a]-8.6602)<=(ypla+25.9808) && enecheck[a]==2){//上のと少し似ているが、敵の種類で当たり判定の範囲が違うようにしたかったので別にした。
      enecheck[a]=0;
      prest--;
      xplacenter=xpla+30.0;
      yplacenter=ypla+12.9803;
      xpla=170.0;
      bombcount(xene[a],yene[a]);
      bombcount(xplacenter,yplacenter);
    }
  }
  for(a=0;a<=4;a++){
    if(xenefire[a]>=(xpla+7.5) && xenefire[a]<=(xpla+52.5) && yenefire[a]>=ypla && yenefire[a]<=(ypla+25.9808) && enefirecheck[a]==1){//敵（自分）の出した弾と主人公（敵）との当たり判定
      enefirecheck[a]=0;
      prest--;
      xplacenter=xpla+30.0;
      yplacenter=ypla+12.9803;
      if(prest>1){
	xpla=170.0;
      }
      bombcount(xplacenter,yplacenter);
    }
  }
  for(a=0;a<=9;a++){
    for(b=0;b<=2;b++){
      if(xpfirehit[a][b]>=(xpla+7.5) && xpfirehit[a][b]<=(xpla+52.5) && ypfirehit[a][b]>=ypla && ypfirehit[a][b]<=(ypla+25.9808) && enefirecheck[a]==2 && pfirecheck[a]==1){//複雑だが、敵（自分）が出した弾に存在する当たり判定と主人公（敵）の当たり判定
	enefirecheck[a]=0;
	pfirecheck[a]=0;
	prest--;
	xplacenter=xpla+30.0;
	yplacenter=ypla+12.9803;
	if(prest>1){
	  xpla=170.0;
	}
	bombcount(xplacenter,yplacenter);
      }
    }      
    
  }
  return;
}

void enefire(void){//敵（自分）が発射した弾の表示用の関数
  int a;//繰り返し
  float nsize=3.0;//弱弾のサイズ指定
  float psize=60.0;//強弾のサイズ指定
  for(a=0;a<=4;a++){
    if(enefirecheck[a]==1){
      newpen(win,4);
      fillcirc(win,xenefire[a],yenefire[a],nsize,nsize);
      yenefire[a]=yenefire[a]-1.4;
      if(yenefire[a]<=0.0){//画面外に出ると消える
	enefirecheck[a]=0;
      }
    }
    if(enefirecheck[a]==2){
      newpen(win,5);
      fillarc(win,xenefire[a],yenefire[a],psize,psize,225.0,315.0,1);
      yenefire[a]=yenefire[a]-2.0;
      if(yenefire[a]<=0.0){//強い弾も画面外に出ると消える
	enefirecheck[a]=0;
	pfirecheck[a]=0;
      }
    }
  }
  
  return;
}

void pfirehitmove(void){//強い敵（自分）が出した弾の当たり判定を動かす用の関数
  int a,b;//繰り返し用
  for(a=0;a<=9;a++){
    if(pfirecheck[a]==1){
      for(b=0;b<=2;b++){//forを使って全部同じように移動
	ypfirehit[a][b]=ypfirehit[a][b]-2.0;
      }
    }
  }
  return;
}
void plafire(void){//プレイヤー（敵）から弾を発射する関数
  int a;//繰り返し用
  float size=3.0;//弾のサイズ

  newpen(win,2);
  for(a=0;a<=2;a++){
    fillcirc(win,xplafire[a],yplafire[a],size,size);
  }
   

  xplafire[0]=xplafire[0]-1.5;yplafire[0]=yplafire[0]+8.0;
  yplafire[1]=yplafire[1]+8.0;
  xplafire[2]=xplafire[2]+1.5;yplafire[2]=yplafire[2]+8.0;//主人公補正　自分だけ３方向発射　しかも速い

  if(yplafire[1]>=400){
    xplafire[0]=xpla+7.5;yplafire[0]=ypla+12.0;
    xplafire[1]=xpla+30.0;yplafire[1]=ypla+12.0;
    xplafire[2]=xpla+52.5;yplafire[2]=ypla+12.0;
  }
  return;
}

void enehitcheck(void){//敵（自分）の当たり判定用関数
  int a,b;//繰り返し用
  float xenecenter,yenecenter;//敵（自分）が表示されている大体の中心を出す。爆発の描画に使う
  
  for(a=0;a<=2;a++){
    for(b=0;b<=2;b++){
      if((enecheck[a]==1) && (xplafire[b]>=(xene[a]+2.5)) && (xplafire[b]<=(xene[a]+17.5)) && (yplafire[b]>=(yene[a]-16.0)) && (yplafire[b]<=yene[a])){//弱敵（自分）用当たり判定
	enecheck[a]=0;
	xenecenter=xene[a]+10.0;yenecenter=yene[a]-8.5;
	bombcount(xenecenter,yenecenter);
      }
      if((enecheck[a]==2) && (xplafire[b]>=(xene[a]+2.5)) && (xplafire[b]<=(xene[a]+17.5)) && (yplafire[b]>=(yene[a]-32.0)) && (yplafire[b]<=yene[a])){//強敵（自分）用当たり判定。こちらの方が大きいので当たり判定もでかい。
	enecheck[a]=0;
	xenecenter=xene[a]+10.0;yenecenter=yene[a]-17.3205;
	bombcount(xenecenter,yenecenter);   
      }
    }
  }
  return;
}
	  
  

int main(void){//メインルーチン　個別の様々な処理は繰り返しを分かりやすくするため基本的に全てサブルーチンで行う。
  int start,mnk;//タイトル、オープニング用
  int a;//繰り返し
  int out;//タイトルに戻るかを判定するための変数
  
  gsetnonblock(ENABLE);
  win=gopen(400,400);
  winname(win,"main");
  while(1){
    gclr(win);
    start=title();//タイトル画面を表示、かつ
    if(start==2){//タイトルで「EXIT」を選んだ場合は即終了する
      return 0;
    }

    if(start==0){
      starreset();
      for(a=0;a<=2;a++){
	enecheck[a]=0;
      }
      mnk=200;
      xpla=170.0;ypla=-25.0;
      curx=195.0;
      prest=15;//主人公（敵）の残機。正直15でも難易度はきつい（と僕がやった限りでは。実際はどうだろうか。意図しない攻略法があるかもしれない。）
      enerest=30;penerest=10;
      xplafire[0]=xpla+7.5;yplafire[0]=52.0;
      xplafire[1]=xpla+30.0;yplafire[1]=52.0;
      xplafire[2]=xpla+52.5;yplafire[2]=52.0;
      out=0;//この辺で数値などを一度リセットしている。タイトルに戻ったときそのまま次のゲームを遊べるようにするため。
      while(out<=0){
	gclr(win);
	star();
	if(mnk>=0){//オープニング（PLAYER 1 STRIKES BACK!のところ）表示用
	  monku();//表示
	  mnk--;
	}
	player(mnk);//主人公に関する処理
	if(mnk<=0){
	  bombdraw();//爆発描画処理
	  cur();//カーソル処理
	  enemy();//敵（自分）処理
	  plafire();//主人公の発射する弾に関する処理
	  pfirehitmove();//敵（自分）が発射した強い弾の当たり判定を動かす処理
	  if(prest==0 && out==0){
	    out=1;
	    clear();//ゲームクリア処理
	  }
	  if(enerest==0 && penerest==0 && enecheck[0]==0 && enecheck[1]==0 && enecheck[2]==0 && out==0){
	    out=1;
	    gameover();//ゲームオーバー処理
	  }
	  enefire();//敵（自分）の発射する弾に関する処理
	  phitcheck();//主人公（敵）の当たり判定処理
	  enehitcheck();//敵（自分）の当たり判定処理
	}
	rest();//敵（自分）の残機の処理
	plarest();//主人公（敵）の残機の処理
	
	msleep(8);//きれいに動かしたかったためmsleepは少なめ
      }
    }
    msleep(5);
  }
}
  
void gameover(void){//ゲームオーバー用関数
  int key,a;
  newpen(win,4);
  drawstr(win,120,200,nfontsize,0.0,"NOT DEFENDED...");
  drawstr(win,160,100,nfontsize,0.0,"1= BACK");
  for(a=0;a<=2;a++){
    enecheck[a]=0;
  }
  for(a=0;a<=4;a++){
    enefirecheck[a]=0;
  }
  for(a=0;a<=9;a++){
    bombcheck[a]=0;
  }//ゲームの情報は全て一度消去しています。
  while(1){
    key=ggetch();
    if(key==49){
      break;
    }
  }
  
  return;
}

void clear(void){//ゲームクリア用関数
  int key,a;
  newpen(win,2);
  drawstr(win,80,200,nfontsize,0.0,"PLAYER 1 GAME OVER!!!!!");
  drawstr(win,160,100,nfontsize,0.0,"1= BACK");
  for(a=0;a<=2;a++){
    enecheck[a]=0;
  }
  for(a=0;a<=4;a++){
    enefirecheck[a]=0;
  }
  for(a=0;a<=9;a++){
    bombcheck[a]=0;
  }//ゲームオーバーの時と同じく、情報は一度削除
  while(1){
    key=ggetch();
    if(key==49){
      break;
    }
  }
  return;
}
      
//このゲームを作った後に「普通のシューティングの方が面白いのでは？」と考えた。以上。   
    
    
  
