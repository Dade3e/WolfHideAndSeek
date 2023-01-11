void attesa_msg_wererabbit(){
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    Serial.println("");
    Serial.println("Received packet!");
    // read the packet
    message_recv = "";
    while (LoRa.available()) {
      message_recv += (char)LoRa.read();
    }
    
    rssi = LoRa.packetRssi();
    
    // print the Packet and RSSI
    Serial.print("message_recv:\t");
    Serial.println(message_recv);
    Serial.print("rssi:\t\t");
    Serial.println(rssi);
    int id = (message_recv[0] - '0')*10+(message_recv[1] - '0');
    int value = message_recv[2] - '0';
    senders[id] = value;
    if(senders_rssi[id] == 0)
      senders_rssi[id] = distanza(rssi);
    senders_rssi[id] = int((senders_rssi[id] + distanza(rssi))/2);
    //senders_rssi[id] = rssi;
    check_value_wererabbit(value);
    
    lastRecvTime = millis();
  }
}

void attesa_msg_infected(){
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    Serial.println("");
    Serial.println("Received packet!");
    // read the packet
    message_recv = "";
    while (LoRa.available()) {
      message_recv += (char)LoRa.read();
    }
    
    rssi = LoRa.packetRssi();
    
    // print the Packet and RSSI
    Serial.print("message_recv:\t");
    Serial.println(message_recv);
    Serial.print("rssi:\t\t");
    Serial.println(rssi);
    int id = (message_recv[0] - '0')*10+(message_recv[1] - '0');
    int value = message_recv[2] - '0';
    senders[id] = value;
    //if(senders_rssi[id] == 0)
    //  senders_rssi[id] = distanza(rssi);
    //senders_rssi[id] = int((senders_rssi[id] + distanza(rssi))/2);
    senders_rssi[id] = rssi;
    check_value_infected(value);
    
    lastRecvTime = millis();
  }
}

void sendMessage() {
  LoRa.beginPacket();                   // start packet
  String outgoing = "";
  if(myPlayerID < 10){
    outgoing += "0";    
  }
  outgoing += String(myPlayerID) + String(myValue);
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
  Serial.print("message sent :");
  Serial.println(outgoing);
}

int distanza(int tmp_rssi){
  //Distance = 10 ^ ((Measured Power -RSSI)/(10 * N))
  //Measured Power =  rssi ad 1m; N is the constant for the environmental factor. It takes a value between 2-4.
  float tmp = (-58) - (tmp_rssi);
  int dist = 1;
  if(tmp != 0){
    tmp = tmp/24;
    dist = int(pow(10,(tmp)));
  }
  return dist;
}

void check_value_wererabbit(int value){
  if(value == 2){
    // RIMANGO WERERABBIT in quanto ho colpito un ex wererabbit, ma invio al wererabbit che ha sbagliato preda
    stato = 2;
    if(displayOnOff == 1)
      schermata_ex_wererabbit();
    lastCleanTime = millis();
  }

  if(value == 3){
    //INDICO HIT e DIVENTO PREDA
    stato = 3;
    if(displayOnOff == 1)
      schermata_HIT();
    lastCleanTime = millis();
  }
}

void check_value_infected(int value){
  if(value == 3){
    //INDICO HIT 
    stato = 3;
    if(displayOnOff == 1)
      schermata_HIT();
    lastCleanTime = millis();
  }
}
