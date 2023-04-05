void sendMessage() {

  String myid = "";
  String for_who = "";
  if(myPlayerID < 10){
    myid = "0";
  }
  if(per_chi < 10){
    for_who = "0";
  }
  String outgoing = myid + String(myPlayerID) + for_who +  String(per_chi) + String(gameState);

  LoRa.beginPacket();
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  
  Serial.print("message sent :");
  Serial.println(outgoing);
}

void lora_recv(){
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    Serial.println("");
    Serial.println("Received packet!");
    // read the packet
    message_recv = "";
    while (LoRa.available()) {
      message_recv += (char)LoRa.read();
    }
    int rssi = LoRa.packetRssi();
    // print the Packet and RSSI
    Serial.print("message_recv:\t");
    Serial.println(message_recv);
    Serial.print("rssi:\t\t");
    Serial.println(rssi);
    int da_chi = (message_recv[0] - '0')*10+(message_recv[1] - '0');
    int for_who = (message_recv[2] - '0')*10+(message_recv[3] - '0');
    int suo_valore = message_recv[4] - '0';
    check_message(da_chi, for_who, suo_valore, rssi);
    
    lastRecvTime = millis();
  }
}

void check_message(int da_chi, int for_who, int suo_valore, int rssi){
  //se sono Sano, controllo se qualcuno se sono vicino ad un infetto (gameState 1):
  if(gameState == 1){
    if(for_who == myPlayerID && suo_valore == 2){
      //ho la conferma di aver colpito
      schermata_HIT();
      per_chi = da_chi;
      sendMessage();
      lastCleanTime = millis();
    }
  }

  //sono in stato appena infettato, ho inviato, aspetto risposta
  else if(gameState == 2){
    if(for_who == myPlayerID){
      gameState = 3;
      per_chi = da_chi;
      lastSendTime = millis()-6000;
    }
  }
}