
void schermata_recv(){
  int offset = 7;
  display.clearDisplay();
  //display.drawBitmap(0, 0, wolf32, 32, 32, WHITE);
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("prey:");
  display.setCursor(0,offset+10);
  display.print("id");
  display.setCursor(60,offset);
  display.print("distance");
  int j=1;
  for(int i=0;i<10;i++){
    if(senders[i] == 1){
      display.setTextSize(1);
      display.setCursor(36,j*10+offset);
      //display.print(char(248));
      display.print(i);
      display.setCursor(60,j*10+offset);
      display.print(senders_rssi[i]);
      j++;
    }
      
  }
  display.setTextSize(1);
  display.setCursor(120,56);
  display.print((millis()-lastRecvTime)/1000);
  display.display();
}

void schermata_recv_draws(){
  display.clearDisplay();
  display.drawBitmap(0, 16, wolf32, 32, 32, WHITE);
  display.setTextSize(1);
  
  display.setCursor(0,56);
  display.print(VBAT);
  int j=1;
  int k=0;
  int w=0;
  for(int i=0;i<10;i++){
    if(senders[i] == 1){      
      display.drawBitmap(42+(18*w), 34*k, rabbit16, 16, 16, WHITE);
      display.setTextSize(1);
      //display.setCursor(56+(16*j), 10*k+16);
      //display.print(char(248));
      //display.print(i);
      if(senders_rssi[i] < 100){
        display.setCursor(45+(18*w), 34*k+18);
        display.print(senders_rssi[i]);
      }
      else{
        display.drawBitmap(42+(18*w), 34*k+18, little_infin, 16, 5, WHITE);
      }
        
      if(j%5 == 0)
        k++;
      if(j%5 == 0)
        w=-1;
      j++;
      w++;
    }
      
  }
  /*display.setCursor(0,0);
  display.print("prey:");
  display.setCursor(30,0);
  display.print(j);*/
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print((millis()-lastRecvTime)/1000);
  display.display();
}

void schermata_ex_lupo(){
  display.clearDisplay();
  display.drawBitmap(0, 16, wolf32, 32, 32, WHITE);
  display.drawBitmap(64, 8, wolf40, 40, 40, WHITE);
  display.setTextSize(1);
  display.setCursor(58,56);
  display.print("LAST WOLF");
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print((millis()-lastRecvTime)/1000);
  display.setCursor(0,56);
  display.print(VBAT);
  display.display();
}

void schermata_send(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("HIDE");
  display.setCursor(0,20);
  display.print("id:");
  display.setCursor(0,32);
  display.print(myPlayerID);
  display.drawBitmap(56, 24, rabbit16, 16, 16, WHITE);
  display.setCursor(102,20);
  display.print("msg:");
  display.setCursor(102,32);
  display.print(msgCount);
  if(circle_size<60)
    display.drawCircle(64,32,circle_size,WHITE);
  display.setTextSize(1);
  display.setCursor(96,56);
  //display.print((millis()-lastSendTime));
  display.print(interval);
  display.display();
  circle_size += 2;
}

void schermata_send_wolf(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("HIDE");
  display.setCursor(0,20);
  display.print("id:");
  display.setCursor(5,32);
  display.print(myPlayerID);
  display.drawBitmap(49, 18, wolf32, 32, 32, WHITE);
  display.setCursor(100,20);
  display.print("msg:");
  display.setCursor(105,32);
  display.print(msgCount);
  if(circle_size<60)
    display.drawCircle(64,32,circle_size,WHITE);
  display.setTextSize(1);
  display.setCursor(96,56);
  //display.print((millis()-lastSendTime));
  display.print(interval);
  display.display();
  circle_size += 2;
}

void schermata_colpito(int swap){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setTextSize(2);
  display.setCursor(5,10);
  display.print("NEW");
  display.setCursor(0,32);
  display.print("WOLF");
  if(swap%2==0)
    display.drawBitmap(64, 0, wolf64, 64, 64, WHITE);
  display.display();
}

void schermata_nera(){
  display.clearDisplay();
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setCursor(0,20);
  display.print("id:");
  display.setCursor(5,32);
  display.print(myPlayerID);
  display.setTextColor(WHITE);
  display.display();
}

void schermata_HIT(){
  display.clearDisplay();
  display.drawBitmap(0, 8, wolf40, 40, 40, WHITE);
  display.setTextSize(1);
  display.drawBitmap(70, 16, rabbit16, 16, 16, WHITE);
  display.setCursor(70,40);
  display.print("HIT");
  display.setCursor(0,56);
  display.print(VBAT);
  display.display();
}
void schermata_shoot(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setTextSize(1);
  display.drawBitmap(0, 16, wolf32, 32, 32, WHITE);
  // bullets
  for(int x = 0; x<ammo; x++){
    display.drawBitmap(40+(10*x/2), 8+((x%2)*24), bullet8X20, 8, 20, WHITE);
  }
  display.display();
}