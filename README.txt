2016-08-03
Seul le sample 4_callback semble être intéressant dans le cadre du projet LI3DS et la communication avec la CamLight via SerialPort.
Via ce sample, on arrive à établir une connection vers le shell du linux embarqué sur la CamLight.
On peut envoyer des commandes et récupérer les résultats (le print out) de la commande.
C'est une implémentation assez minimaliste d'un terminal serial connection.
Faudrait faire un filtre sur les encodages de couleurs fournit par le shell de la CamLight.
Un (exemple) de Filtre (ou interprêteur) est utilisé dans le miniterm.py dans la lib python pyserial. 
