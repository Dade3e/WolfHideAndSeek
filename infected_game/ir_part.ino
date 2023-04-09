void shoot() {
  irsend.sendSony(440000+(myPlayerID*100), 20, 2);  // 20 bits & 2 repeats
}


void receiveIR() {
  if (irrecv.decode(&results)) {
      int ir_value = results.value;
      Serial.println(ir_value);
      //head 101000
      if(ir_value/10000 == 44){
        controllo_colpito((ir_value%440000)/100);
      }
      irrecv.resume(); // Receive the next value
  }
}


void controllo_colpito(int idshotme){
  lastSendTime = millis();
  per_chi = idshotme;
  gameState = 4; //Stato colpito
  digitalWrite(speakerPin, HIGH);
}
