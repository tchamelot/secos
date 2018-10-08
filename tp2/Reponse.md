# Réponse TP2

# 1
# 2
# 3
L'IDT pour breakpoint est la numéro 3.

Lorsque l'on remplace la fonction trampoline de base du breakpoint par notre *bp_handler*, il y a une fatal exception après l'affichage du message de notre handler.
L'exception est une GP.

Pour supprimer cette exception, il faut corriger le fonctionnement de la pile. Pour cela, il faut supprimer le mécanisme de la pile mise en place par la fonction C du handler.
