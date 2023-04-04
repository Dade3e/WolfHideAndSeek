void infected_first_frame(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, infected_rabbit64, 64, 64, WHITE);
  display.setTextSize(1);
  display.setCursor(64,6);
  display.print("INFECTED");
  display.setCursor(92,16);
  display.print("RABBIT");
  display.setCursor(80,34);
  display.print("HIDE &");
  display.setCursor(86,46);
  display.print("SEEK");
  display.display();
}

void schermata_recv_draws(){
  display.clearDisplay();
  display.drawBitmap(0, 16, infected_rabbit32, 32, 32, WHITE);
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  int j=1;
  int k=0;
  int w=0;
  for(int i=0;i<100;i++){
    if(senders[i] == 2){    // 2: rabbits
      display.drawBitmap(42+(18*w), 29*k, angry_rabbit16, 16, 16, WHITE);
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


void schermata_send(){
  display.clearDisplay();
  display.setTextSize(1);
  //display.setCursor(0,0);
  //display.print("FIND");
  display.setCursor(0,0);
  display.print("Id:");
  display.setCursor(20,0);
  display.print(myPlayerID);
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setTextSize(1);
  display.drawBitmap(0, 16, angry_rabbit32, 32, 32, WHITE);
  // bullets
  if(ammo > 0){
    for(int x = 0; x<ammo; x++){
      display.drawBitmap(40+(10*x/2), 8+((x%2)*24), bullet8X20, 8, 20, WHITE);
    }    
  }else{
    display.setCursor(60,28);
    display.print("END AMMO");
  }
  
  
  display.setTextSize(1);
  display.setCursor(56,56);
  display.print(gameTimer);
  display.setCursor(96,56);
  //display.print(interval-(millis() - lastSendTime));
  display.print(interval);
  display.display();
}

void schermata_send_wererabbit(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setCursor(0,0);
  display.print("Sending new infected");
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

  display.drawBitmap(48, 16, infected_rabbit32, 32, 32, WHITE);
  display.display();
  circle_size += 2;
}


void schermata_wait(){
  display.clearDisplay();
  display.drawBitmap(0, 16, skull32, 32, 32, WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Id:");
  display.setCursor(20,0);
  display.print(myPlayerID);
  display.setCursor(0,56);
  display.print(VBAT);
  display.setTextSize(1);
  display.setCursor(56,10);
  display.print("WAIT FOR ");
  display.setCursor(56,22);
  display.print("THE END");
  display.setCursor(56,34);
  display.print("OF THE GAME");
  display.setCursor(56,56);
  display.print(gameTimer);
  display.setCursor(96,56);
  display.print(interval);
  display.display();
}

void schermata_nera(){
  display.clearDisplay();
  display.display();
}

void schermata_HIT(){
  display.clearDisplay();
  display.drawBitmap(16, 16, infected_rabbit32, 32, 32, WHITE);
  display.setTextSize(1);
  display.setCursor(74,12);
  display.print("YOU");
  display.setCursor(74,24);
  display.print("HIT");
  display.setCursor(74,36);
  display.print("AN");
  display.setCursor(74,48);
  display.print("INFECTED");
  display.display();
}

void schermata_infettato(){
  display.clearDisplay();
  display.drawBitmap(0, 0, infected_rabbit64, 64, 64, WHITE);
  display.setTextSize(1);
  display.setCursor(74,12);
  display.print("YOU");
  display.setCursor(74,24);
  display.print("ARE");
  display.setCursor(74,36);
  display.print("BEEN");
  display.setCursor(74,48);
  display.print("INFECTED");
  display.display();
}

void schermata_send_infected(){
  display.clearDisplay();
  display.drawBitmap(16, 16, infected_rabbit32, 32, 32, WHITE);
  display.setTextSize(1);
  display.setCursor(74,12);
  display.print("YOU");
  display.setCursor(74,24);
  display.print("INFECTED");
  display.setCursor(74,36);
  display.print("A");
  display.setCursor(74,48);
  display.print("RABBIT");

  if(circle_size<60)
    display.drawCircle(32,32,circle_size,WHITE);    
  //display.drawBitmap(48, 16, infected_rabbit32, 32, 32, WHITE);

  display.setCursor(96,56);
  display.print(interval);
  display.display();
  circle_size += 2;
  display.display();
}

void schermata_colpito(){
  display.clearDisplay();
  display.drawBitmap(16, 16, skull32, 32, 32, WHITE);
  display.setTextSize(1);
  display.setCursor(74,16);
  display.print("YOU");
  display.setCursor(74,28);
  display.print("ARE");
  display.setCursor(74,40);
  display.print("DEATH");
  display.display();
}

void schermata_win(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, infected_rabbit64, 64, 64, WHITE);
  display.setTextSize(1);
  display.setCursor(64,6);
  display.print("INFECTED");
  display.setCursor(92,16);
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
  display.drawBitmap(0, 0, infected_rabbit64, 64, 64, WHITE);
  display.setTextSize(1);
  display.setCursor(64,6);
  display.print("INFECTED");
  display.setCursor(92,16);
  display.print("RABBIT");
  display.setCursor(80,34);
  display.print("YOU");
  display.setCursor(86,46);
  display.print("LOSE");
  display.display();
}