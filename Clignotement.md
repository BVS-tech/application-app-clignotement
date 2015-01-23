Reconnaissance clignotement pour appairage
==========================================

Le but est de trouver la zone couverte par une lampe.
50 FPS
var_sensitivity par défaut

Test lampe  5 Hz (100ms par état)
-----------------------------------------------------

**partielement masquée**

Classifier 0-3
confirmTracking_th = 60;
~300 pixel

Si classifier 3-9
très erratique

si classifier 0-2
Fonctionne mais avec moins de points

**Non masquée**

Même paramètres 
Fonctionne aussi mais attention aux reflexions !!

Test lampe 3.3 Hz (150ms par état)
----------------------------------

**partielement masquée**

Même paramètres

Ne fonctionne pas

**Non masquée**

Même paramètres 
Erratique

+++++++++++++++++++++++++

Il faut maintenant faire les tests en conditions réelles.
Puis il faudra l'integrer aux applications.
