void shoot() {
  irsend.sendSony(440000+(myPlayerID*100)+myTeamID, 20, 2);  // 20 bits & 2 repeats
}


void receiveIR() {
  if (irrecv.decode(&results)) {
      int ir_value = results.value;
      Serial.println(ir_value);
      //head 101000
      if(ir_value/10000 == 44){
        int idshotme = (ir_value%440000)/100;
        int teamshotme = (ir_value%100);
        controllo_colpito(idshotme, teamshotme);
      }
      irrecv.resume(); // Receive the next value
  }
}


void controllo_colpito(int idshotme, int teamshotme){
  // 99 è tutt contro tutti
  if((teamshotme == 99 || teamshotme != myTeamID) && idshotme != myPlayerID){
    per_chi = idshotme;
    gameState = 2; //Stato colpito
    digitalWrite(speakerPin, LOW);
  }// altrimenti fuoco amico
  
}
