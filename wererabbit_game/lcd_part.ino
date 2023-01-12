void wererabbit_first_frame(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, wererabbit64, 64, 64, WHITE);
  display.setTextSize(1);
  display.setCursor(66,8);
  display.print("WERE");
  display.setCursor(90,14);
  display.print("RABBIT");
  display.setCursor(80,34);
  display.print("HIDE &");
  display.setCursor(86,46);
  display.print("SEEK");
  display.display();
}

void schermata_recv_draws(){
  display.clearDisplay();
  display.drawBitmap(0, 16, rabbit32_05, 32, 32, WHITE);
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  int j=1;
  int k=0;
  int w=0;
  for(int i=0;i<100;i++){
    if(senders[i] == 1){
      display.drawBitmap(42+(18*w), 29*k, rabbit16, 16, 16, WHITE);
      display.setTextSize(1);
      if(senders_rssi[i] < 100){
        display.setCursor(45+(18*w), 29*k+18);
        display.print(senders_rssi[i]);
        //Serial.println(senders_rssi[i]);
        //display.fillRect(43+(18*w), 29*k+18, senders_rssi[i], 3, WHITE);
        //display.drawBitmap(42+(19*w), 29*k+18, little_infin, 16, 5, WHITE);
      }
      else{
        display.drawBitmap(42+(19*w), 29*k+18, little_infin, 16, 5, WHITE);
      }
        
      if(j%5 == 0)
        k++;
      if(j%5 == 0)
        w=-1;
      j++;
      w++;
    }
      
  }
  
  display.setCursor(0,0);
  display.print("Id:");
  display.setCursor(20,0);
  display.print(myPlayerID);
  display.setCursor(56,56);
  display.print(gameTimer);
  display.setTextSize(1);
  display.setCursor(105,56);
  display.print((millis()-lastRecvTime)/1000);
  display.display();
     
}

void schermata_ex_wererabbit(){
  display.clearDisplay();
  display.drawBitmap(6, 24, rabbit16, 16, 16, WHITE);
  display.drawBitmap(50, 12, rabbit32_05, 32, 32, WHITE);
  display.setTextSize(1);
  display.setCursor(90,16);
  display.print("YOU");
  display.setCursor(90,28);
  display.print("HIT");
  display.setCursor(90,40);
  display.print("LAST");
  display.setCursor(50,52);
  display.print("WERE-RABBIT");
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Id:");
  display.setCursor(20,0);
  display.print(myPlayerID);
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
  display.drawBitmap(57, 24, rabbit16, 16, 16, WHITE);
  display.setCursor(102,20);
  display.print("msg:");
  display.setCursor(102,32);
  display.print(msgCount);
  display.setCursor(56,56);
  display.print(gameTimer);
  if(circle_size<60)
    display.drawCircle(64,32,circle_size,WHITE);
  display.setTextSize(1);
  display.setCursor(96,56);
  //display.print((millis()-lastSendTime));
  display.print(interval);
  display.display();
  circle_size += 2;
}

void schermata_send_wererabbit(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setCursor(0,0);
  display.print("Sending ex wererabbit");
  display.setCursor(0,20);
  display.print("id:");
  display.setCursor(4,32);
  display.print(myPlayerID);
  display.setCursor(100,20);
  display.print("msg:");
  display.setCursor(105,32);
  display.print(msgCount);
  display.setCursor(96,56);
  display.print(interval);
  
  if(circle_size<60)
    display.drawCircle(64,32,circle_size,WHITE);

  switch(rabbit_draw){
    case 0:
    case 1:
    case 2:
    case 3:
      display.drawBitmap(48, 16, rabbit32_05, 32, 32, WHITE);
    break;
    case 4:
      display.drawBitmap(48, 16, rabbit32_04, 32, 32, WHITE);
    break;
    case 5:
      display.drawBitmap(48, 16, rabbit32_03, 32, 32, WHITE);
    break;
    case 6:
      display.drawBitmap(48, 16, rabbit32_02, 32, 32, WHITE);
    break;
    default:
      display.drawBitmap(48, 16, rabbit32_01, 32, 32, WHITE);
    break;
  }

  rabbit_draw++;
  display.display();
  circle_size += 2;
}

void schermata_colpito(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Id:");
  display.setCursor(20,0);
  display.print(myPlayerID);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setTextSize(1);
  display.setCursor(96,6);
  display.print("YOU");
  display.setCursor(96,18);
  display.print("ARE");
  display.setCursor(96,30);
  display.print("THE");
  display.setCursor(96,42);
  display.print("NEW");
  display.setCursor(48,54);
  display.print("WERE-RABBIT");
  switch(rabbit_draw){
    case 0:
      display.drawBitmap(48, 12, rabbit32_01, 32, 32, WHITE);
    break;
    case 1:
      display.drawBitmap(48, 12, rabbit32_02, 32, 32, WHITE);
    break;
    case 2:
      display.drawBitmap(48, 12, rabbit32_03, 32, 32, WHITE);
    break;
    case 3:
      display.drawBitmap(48, 12, rabbit32_04, 32, 32, WHITE);
    break;
    default:
      display.drawBitmap(48, 12, rabbit32_05, 32, 32, WHITE);
    break;
  }

  rabbit_draw++;
  if(rabbit_draw > 7)
    rabbit_draw=0; 

  display.display();
}

void schermata_nera(){
  display.clearDisplay();
  display.display();
}

void schermata_HIT(){
  display.clearDisplay();
  display.drawBitmap(0, 16, rabbit32_05, 32, 32, WHITE);
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setCursor(0,0);
  display.print("Id:");
  display.setCursor(20,0);
  display.print(myPlayerID);

  display.drawBitmap(62, 24, rabbit16, 16, 16, WHITE);
  display.setTextSize(1);
  display.setCursor(90,10);
  display.print("YOU");
  display.setCursor(90,22);
  display.print("HIT");
  display.setCursor(90,34);
  display.print("A");
  display.setCursor(90,46);
  display.print("RABBIT");
  display.display();
}
void schermata_shoot(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setTextSize(1);
  display.drawBitmap(0, 16, rabbit32_05, 32, 32, WHITE);
  // bullets
  for(int x = 0; x<ammo; x++){
    display.drawBitmap(40+(10*x/2), 8+((x%2)*24), bullet8X20, 8, 20, WHITE);
  }
  display.display();
}

void schermata_win(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, wererabbit64, 64, 64, WHITE);
  display.setTextSize(1);
  display.setCursor(66,8);
  display.print("WERE");
  display.setCursor(90,14);
  display.print("RABBIT");
  display.setCursor(80,34);
  display.print("YOU");
  display.setCursor(86,46);
  display.print("WIN");
  display.display();
}
void schermata_lose(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, wererabbit64, 64, 64, WHITE);
  display.setTextSize(1);
  display.setCursor(66,8);
  display.print("WERE");
  display.setCursor(90,14);
  display.print("RABBIT");
  display.setCursor(80,34);
  display.print("YOU");
  display.setCursor(86,46);
  display.print("LOSE");
  display.display();
}