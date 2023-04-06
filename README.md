# WERERABBIT game
## Regole
### Wererabbit
Il gioco è metà tra nascondino e "ce l'hai":
Ogni giocatore ha un arma e i sensori ir,
Minimo di giocatori 3,
Il gioco si divide in wererabbit e rabbits.
Il wererabbit può sparare per passare il suo stato di wererabbit,
i rabbit non possono sparate, solo nascondersi o scappare.

A inizio gioco, il giocatore wererabbit conta per un tempo scelto dal gruppo, gli altri si nascondono.
Ogni giocatore rabbit, ogni circa 10 secondi invia un impulso a 866mhz, il quale è ricevuto dal wererabbit,
che lo visualizza sullo schermo come intensita di segnale trasformata circa in metri e lo aiuterà a cercare i rabbit.

Quando un wererabbit trova e colpisce un rabbit, il colpito diventerà in 60 sec il nuovo wererabbit e il vecchio wererabbit diventerà un rabbit.
Inoltre il vecchio wererabbit è immune al nuovo, ma solo a quello, non ai successivi.

Allo scadere del tempo mostrato a display, perde chi è il wererabbit o lo sta diventando.


### Infected
Ogni giocatore ha un arma e i ricevitori ir.
Il gioco si divide in infetto e sani.

Il gioco inizia con l'infetto che si nasconde.
L'infetto ha il sensore di prossimità il quale vede la distanza dai sani.
Se un sano entra nel raggio dei 5 metri da un infetto, l'infetto può premere il grilletto per infettare il sano.
Se un sano gli spara prima, l'infetto muore.

Il gioco finisce con lo scadere del tempo.
Vincono i più numerosi tra sani e infetti.

### Deathmatch
Classico deathmatch a scquadre o tutti contro tutti,
Se si imposta l'id team a 99, non ci sono squadre e sarà tutti contro tutti.
Una volta colpiti anche solo una volta si muore.
Il gioco finisce allo scadere del tempo.
TODO: vincita?
TODO: tornare alla base per riprendere vita?

### Materiali utilizzati:
- Presa Jack verticale 4 poli: https://www.amazon.it/dp/B089222S84?psc=1&ref=ppx_yo2ov_dt_b_product_details
- EDISION 05-00-0017 Cavo Jack a IR: https://www.amazon.it/dp/B08612KY32?psc=1&ref=ppx_yo2ov_dt_b_product_details
- Esp LoRa32: https://www.amazon.it/dp/B07RWYJQPT?psc=1&ref=ppx_yo2ov_dt_b_product_details
- Led IR SFH 4544: https://it.rs-online.com/web/p/led-a-infrarossi/8108247?sra=pstk
- Transistor 2N2222
- Resistenza da 510/470 Ohm
- Resistenza da 33/27 Ohm
- Pulsante 7 mm: https://www.amazon.it/Gebildet-Precablato-Momentaneo-Temporanei-Ferroviario/dp/B083K1YCZK/ref=sr_1_7?keywords=pulsante&qid=1673539022&sr=8-7&th=1
- Finecorsa da usare come grilletto, forse: https://www.amazon.it/InduSKY-Interruttore-Finecorsa-interruttore-Momentaneo/dp/B08736V215/ref=sr_1_21?keywords=finecorsa&qid=1673539142&sprefix=finecorsa%2Caps%2C115&sr=8-21 
- Morsettiera a vite 2.54: https://www.amazon.it/dp/B08VRD69B6?psc=1&smid=A1LEBA88GLK1VX&ref_=chk_typ_imgToDp
