L'idée est bonne. Les deux approches que tu décris sont viables dans ce kernel. 
1) Allocation dynamique pour agrandir le ring
L'idée c'est de remplacer le tableau fixe u8 buffer[RING_BUFFER_SIZE] par un buffer alloué dynamiquement (pointeur + capacity).
- Avantage : transparent pour kprintf/kputs. kwrite détecte qu'il ne reste plus de place, alloue un plus gros buffer, recopie l'existant, met à jour read/write/count/capacity.
- Inconvénient : il faut un allocateur. J'ai regardé rapidement le codebase, je ne vois pas de kmalloc/kfree évident pour l'instant. Si aucun heap n'existe, il faudrait en implémenter un minimum (bump allocator) juste pour ça, ce qui est faisable mais ça grossit le scope.
- Détails à surveiller : struct ring est passé partout (ft_stdout, ft_stdin, events, kbd_ring). Passer d'un tableau inline à un pointeur est un petit changement structurel mais ça touche pas mal de code. Aussi, recopier quand on grandit est chiant mais nécessaire pour garder la logique circulaire telle qu'elle est.
2) "Coopératif" / garder le contexte et vider au fur et à mesure (ma préférence ici)
C'est, je pense, la plus adaptée à l'architecture actuelle. 
Le principe : au lieu d'imprimer 200+ lignes d'un coup dans print_stack, on garde l'état du dump (pointeur current, top, esp, ebp, un compteur/phase) dans une variable globale. On imprime un petit batch (ex: 16-32 entrées) à chaque "tick", on se rend compte qu'il reste du boulot et on laisse kloop tourner un tour complet.
Comme ft_console est déjà appelé à chaque itération de la boucle principale, le ft_stdout ring ne débordera jamais s'on ne pousse que quelques caractères à la fois entre chaque vidage.
Comment ça s'intègre
1. Avoir un job global  
Créer un truc du genre :
struct stackdump_job {
    int active;
    uint32_t *cur;
    uint32_t *top;
    uint32_t esp, ebp;
};
Stocké en global (comme kbd_ring).
2. Transformer print_stack en "démarreur"  
Quand on tape print stack, execute_line ne fait plus le gros while bloquant. Il initialise juste le job avec esp/ebp/cur/top et le marque active = 1. print_stack retourne immédiatement.
3. Ajouter un tick dans kloop  
Dans kloop(), entre shell() et ft_console() (ou avant/après, peu importe tant que ft_console tourne quand même), on fait :
if (stackdump_job.active)
    stackdump_tick(&stackdump_job, &ft_stdout);
stackdump_tick pousse N lignes max dans ft_stdout (kprintf), incrémente cur, et quand cur >= top met active = 0.
4. Laisser le consumer faire son travail  
Directement après, ft_console(&console, &ft_stdout) vide ces quelques lignes vers le VGA. Au prochain tour de while(1), on refait ça jusqu'à la fin. Résultat : tout s'affiche, ring reste < 256 en permanence.
Comparaison
Critère	Dynamique (grow)	Coopératif (step + yield)
Changement API	Faible si fait bien (kprintf inchangé)	print_stack change de sémantique (devient asynchrone visuellement)
Besoin allocateur	Oui (sinon compliqué)	Non
Impact global	Toucher struct ring, ring_push/pop, init partout	Ajouter un job global + 1 appel dans kloop
Risque	Plus facile de se planter sur la recopie/circulaire	Très localisé, facile à débugguer, s'intègre pile au modèle "polling" existant
Extensible	Utile pour n'importe quoi	Utile pour n'importe quel dump long (backtrace futur, hexdump, etc)
Mon avis
Je pencherais fortement pour l'approche coopérative.
Ta description "garde le context du dump de la stack en mémoire mais fais des tours de boucle principale pour vider stdout au fur et a mesure" est exactement ça. C'est propre avec l'architecture déjà en place : le "scheduler" existe déjà sous forme de cette boucle while(1) qui tourne en boucle. On n'invente rien, on exploite juste le fait que ft_console est déjà appelée systématiquement.
Le côté "allocation dynamique minimum" est tentant, mais sans allocateur ça devient vite bancal. Le coopératif évite complètement le problème de saturation du ring au lieu de le contourner en le rendant plus grand. 
Si on veut vraiment aller plus loin plus tard, on pourrait généraliser ça en un mini "job queue" (pending_jobs[]) et avoir un jobs_tick() unique dans kloop. Mais pour juste print stack, un simple flag + state suffit amplement.
