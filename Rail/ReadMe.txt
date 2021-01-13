Sistema di segnalazione ferroviaria

Per la gestione, abbiamo pensato di risolvere il problema in modo statico,
ossia di stampare in ogni momento lo stato degli treni e delle stazioni.
Abbiamo creato 3 classi principali:
-ReadFile: una classe che si occupa dei dati.
-Station: una classe che descrive lo stato delle stazioni.
-Train: una classe che rappresenta i treni.

Dopodiché nella main, abbiamo collegato le classi attraverso due funzioni
della classe Station (ArrivalRequest e DepartuRerequest) e una funzioni
della classe Train (update). Aggiorna i treni per un tempo massimo di 1 giono e mezzo.

Come output del terminale:
1)lo stato di ogni treno in ogni istante
2)arrivo e/o partenza dei treni
l'output si trova nella cartella build


Difficoltà riscontrata:
-collegamento tra Station e Train non funziona sempre, questo ha portato nella
main la gestione dell'output in modo non ottimale.
