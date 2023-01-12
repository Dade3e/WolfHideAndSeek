void shoot() {
  irsend.sendSony(440000+(myPlayerID*100)+id_wolf, 20, 2);  // 20 bits & 2 repeats
}


void receiveIR() {
  if (irrecv.decode(&results)) {
      int ir_value = results.value;
      Serial.println(ir_value);
      //head 101000
      if(ir_value/10000 == 44){
        id_wolf = (ir_value%440000)/100;
        int id_last_wolf = (ir_value%(440000 + id_wolf*100));
        controllo_colpito(id_last_wolf);
      }
      Serial.println(id_wolf);
      irrecv.resume(); // Receive the next value
  }
}


void controllo_colpito(int id_last_wolf){
  if(id_last_wolf == 0){
    if(id_wolf== myPlayerID){
      stato = 0;
    }
  }
  else if(id_last_wolf == myPlayerID){
    lastSendTime = millis()-8000;
    myValue = 2; //invio segnale che ero ex wererabbit
    stato = 4; //stato ex wererabbit
  }
  else{
    lastSendTime = millis()-8000;
    myValue = 3;
    stato = 5; //Stato colpito
  }  
}
