# Flusso di gioco e protocollo

## Se sei infetto:
Stato 0: Init
idShotMe = myIdPlayer
myValue = stato +1 = 1
Ascolto rivecitore ir e rssi degli altri
TODO: quando premo il grilletto invio il mio stato per infettare

Se vengo colpito con ir,
vado in stato 5
metto myValue in 3
idShotMe = id di chi mi ha colpito

Stato 5:
Stato in cui sto morendo e invio di essere morto
myValue = 3;
idShotMe != mio
2 tentativi invio messaggio:
> invio: mio id 2 char, id di chi mi ha colpito 2 char, mio valore: 3
e mostro schermata colpito ogni secondo
Vado in stato 6, myValue 5

Stato 6:
stato in cui sono morto e aspetto il fine partita
Ricezione stato con lora
invio il mio stato


## Se sei cacciatore
