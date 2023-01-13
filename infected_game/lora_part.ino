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
    int rssi = LoRa.packetRssi();
    // print the Packet and RSSI
    Serial.print("message_recv:\t");
    Serial.println(message_recv);
    Serial.print("rssi:\t\t");
    Serial.println(rssi);
    int id = (message_recv[0] - '0')*10+(message_recv[1] - '0');
    int idshootme = (message_recv[2] - '0')*10+(message_recv[3] - '0');
    int value = message_recv[4] - '0';
    senders[id] = value;
    
    lastRecvTime = millis();
  }
}

void attesa_msg_infected(){
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    Serial.println("");
    Serial.println("attesa_msg_infected, Received packet!");
    // read the packet
    message_recv = "";
    while (LoRa.available()) {
      message_recv += (char)LoRa.read();
    }
    if((millis()-lastCleanTime)>60000){
      lastCleanTime = millis();
      memset(senders, 0, sizeof(senders));
    }
    int rssi = LoRa.packetRssi();
    // print the Packet and RSSI
    Serial.print("message_recv:\t");
    Serial.println(message_recv);
    Serial.print("rssi:\t\t");
    Serial.println(rssi);
    int id = (message_recv[0] - '0')*10+(message_recv[1] - '0');
    int idshootme = (message_recv[2] - '0')*10+(message_recv[3] - '0');
    int value = message_recv[4] - '0';
    senders[id] = value;
    //if(senders_rssi[id] == 0)
    senders_rssi[id] = distanza(rssi);
    //senders_rssi[id] = int((senders_rssi[id] + distanza(rssi))/2);
    //senders_rssi[id] = rssi;
    //senders_rssi[id] = disegno_forza(rssi);
    check_value_infected(value, id, rssi);
    
    lastRecvTime = millis();
    lastRecvTime_ctl = millis();
  }
}

void attesa_msg_hit(){
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    Serial.println("");
    Serial.println("attesa_msg_hit, Received packet!");
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
    int id = (message_recv[0] - '0')*10+(message_recv[1] - '0');
    int idshootme = (message_recv[2] - '0')*10+(message_recv[3] - '0');
    int value = message_recv[4] - '0';
    check_value_hit(value, idshootme, rssi);
    
    lastRecvTime = millis();
  }
}

void sendMessage() {
  LoRa.beginPacket();                   // start packet
  String myid = "";
  String myshotid = "";
  if(myPlayerID < 10){
    String myid = "0";
  }
  if(idShotMe < 10){
    String myshotid = "0";
  }
  String outgoing = myid + String(myPlayerID) + myshotid +  String(idShotMe) + String(myValue);
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

int disegno_forza(int tmp_rssi){
  //Distance = 10 ^ ((Measured Power -RSSI)/(10 * N))
  //Measured Power =  rssi ad 1m; N is the constant for the environmental factor. It takes a value between 2-4.
  int init_rssi = 60;
  if(-tmp_rssi < init_rssi )
    return 14;
  return 14 - double((-tmp_rssi - init_rssi)/4.28);
}

void check_value_infected(int value, int idplayer, int tmp_rssi){
  if(tmp_rssi >= sogliaRSSI && value == 2){
    Serial.print("Ho infettato: ");
    Serial.println(idplayer);
    stato = 4;
    idShotMe = idplayer;
    myValue = 4;
    lastSendTime = millis()-8000;
  }
}

void check_value_hit(int value, int idshootme, int tmp_rssi){
  if(value == 3 && idshootme == myPlayerID){
    if(displayOnOff == 1)
      schermata_HIT();
    lastCleanTime = millis() + 3000; // attesa di 1 sec in piu per mostrare la schermata hit
    //ammo++??
  }
  if(value == 4 && idshootme == myPlayerID){
    if(displayOnOff == 1)
      schermata_infettato(); // creare stato per inviare che ho infettato
    lastCleanTime = millis() + 3000; // attesa di 1 sec in piu per mostrare la schermata infettato
    //ammo++??
    stato = 0;
    myValue = 1;
  }
}
