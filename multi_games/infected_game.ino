void infectedGame(){
  gestioneDisplayOnOff();
  if(stato == 0){
    //RECV and SHOOT
    attesa_msg_infected();
    if((millis()-lastSendTime)>1000){
      lastSendTime = millis();
      VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
      if(displayOnOff == 1 && triggerState == 0)
        schermata_recv_draws();
    }
    if((millis()-lastCleanTime)>60000){
      lastCleanTime = millis();
      memset(senders, 0, sizeof(senders));
    }
    if(triggerState == 0 && digitalRead(triggerPin) == LOW){
      shoot_infected();
      if(displayOnOff == 1)
        schermata_shoot();
      triggerState = 1;
      ammo--;
      if(ammo <= 0)
        ammo = 16;
    }
    gestioneTrigger();
    
  }

  if(stato == 1){
    //SEND
    receiveIR_wererabbit();
    if (millis() - lastSendTime > interval) {
      sendMessage();
      VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
      lastSendTime = millis();            // timestamp the message
      interval = random(3000) + 9000;    // 9 - 12 seconds
      circle_size = 12;
    }
    if (millis() - lastCircleTime > 300) {
        if(displayOnOff == 1)
          schermata_send();
      lastCircleTime = millis();
    }
  }

  //colpito 
  if(stato == 3){
    //Attesa per schermata HIT
    if((millis()-lastCleanTime)>3000){
      lastCleanTime = millis();
      stato = 0; // torno WERERABBIT
    }
  }


  //Sono stato colpito
  if(stato == 5){
    stato = 0; //DIVENTO WERERABBIT
    myValue = 3;
    int j = 0;
    while(j < 2){
      if (millis() - lastSendTime > interval) {
        sendMessage();
        VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
        lastSendTime = millis();            // timestamp the message
        interval = random(3000) + 9000;    // 9 - 12 seconds
        circle_size = 12;
        j++;
      }
      if (millis() - lastCircleTime > 1000) {
        schermata_colpito();
        VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
        
        lastCircleTime = millis();
      }
    }
    myValue = 1; 
  }
}