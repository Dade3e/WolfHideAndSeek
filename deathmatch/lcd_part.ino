void first_frame(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 16, skull32, 32, 32, WHITE);
  display.setTextSize(2);
  display.setCursor(50,12);
  display.print("DEATH");
  display.setCursor(50,36);
  display.print("MATCH");
  display.display();
}

void schermata_ammo(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("ID");
  display.setCursor(0,10);
  display.print(myPlayerID);
  display.setCursor(0,28);
  display.print("TEAM");
  display.setCursor(0,38);
  display.print("ID ");
  //display.setCursor(0,20);
  display.print(myTeamID);
  // bullets
  if(ammo > 0){
    for(int x = 0; x<ammo; x++){
      display.drawBitmap(40+(10*x/2), ((x%2)*24), bullet8X20, 8, 20, WHITE);
    }    
  }else{
    display.setCursor(60,28);
    display.print("END AMMO");
  }
  
  display.setCursor(0,56);
  display.print("Kills ");
  display.print(kills);
  display.setCursor(64,56);
  display.print(gameTimer);
  display.setCursor(100,56);
  display.print(VBAT);
  display.display();
}


void schermata_wait(){
  display.clearDisplay();
  display.drawBitmap(0, 8, skull32, 32, 32, WHITE);
  display.setTextSize(1);
  display.setCursor(56,8);
  display.print("WAIT FOR ");
  display.setCursor(56,20);
  display.print("THE END");
  display.setCursor(56,32);
  display.print("OF THE GAME");
  display.setCursor(0,56);
  display.print("Id:");
  display.print(myPlayerID);
  display.setCursor(56,56);
  display.print(gameTimer);
  display.setCursor(100,56);
  display.print(VBAT);
  display.display();
}

void schermata_nera(){
  display.clearDisplay();
  display.display();
}

void schermata_HIT(){
  display.clearDisplay();
  display.drawBitmap(0, 16, skull32, 32, 32, WHITE);
  display.setTextSize(2);
  display.setCursor(64,24);
  display.print("HIT");
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
  display.drawBitmap(0, 16, skull32, 32, 32, WHITE);
  display.setTextSize(1);
  display.setCursor(40,8);
  display.print("DEATH");
  display.setCursor(40,20);
  display.print("MATCH");
  display.setTextSize(2);
  display.setCursor(40,42);
  display.print("YOU WIN");
  display.display();
}
void schermata_lose(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(16, 0, skull32, 32, 32, WHITE);
  display.setTextSize(1);
  display.setCursor(64,8);
  display.print("DEATH");
  display.setCursor(64,20);
  display.print("MATCH");
  display.setTextSize(2);
  display.setCursor(16,42);
  display.print("YOU LOSE");
  display.display();
}