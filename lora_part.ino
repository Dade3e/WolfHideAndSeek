void attesa_msg(){
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
    Serial.println(message_recv);
    Serial.println(rssi);
    int id = (message_recv[0] - '0')+(message_recv[1] - '0');
    int value = message_recv[2] - '0';
    senders[id] = value;
    senders_rssi[id] = distanza(rssi);

    check_value(value);
    
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
}

int distanza(int tmp_rssi){
  //Distance = 10 ^ ((Measured Power -RSSI)/(10 * N))
  //Measured Power = -12 rssi ad 1m; N is the constant for the environmental factor. It takes a value between 2-4.
  float tmp = (-64) - (tmp_rssi);
  int dist = 1;
  if(tmp != 0){
    tmp = tmp/22;
    dist = int(pow(10,(tmp)));
  }
  return dist;
}

void check_value(int value){
  if(value == 2){
    // RIMANGO LUPO in quanto ho colpito un ex lupo ma invio al lupo che ha sbagliato preda
    stato = 2;
    if(displayOnOff == 1)
      schermata_ex_lupo();
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
