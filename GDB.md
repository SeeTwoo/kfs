#LAUNCH GDB

gdb -x <filename>
lance gdb et gdb execute les commandes dans le fichier, ici on s'en sert pour automatiser la jonction avec qemu

notre fichier c'est cmd.gdb donc on fait

gdb -x cmd.gdb --tui


#GDB commands

print (ou p) : affiche une variable ou une valeur en general une fois

p/x : affiche au format hexadecimal

display : affiche une variable ou une valeur a chaque nouvelle commande, utile pour voir l'evolution sans avoir a taper print 1000 fois

n (ou next) : avance a la ligne suivante

s (ou step) : rentre dans la fonction de la ligne actuelle si possible

c (ou continue) : continue l'execution jusqu'au prochain breakpoint

b <nom de fonction ou numero de ligne> (ou break) : place un breakpoint
