#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

// Uncomment to display setup debug.

// #define DEBUG

#define ABS(x) x > 0 ? x : -x

typedef struct s_give   t_give;
typedef struct s_road   t_road;
typedef struct s_troop  t_troop;
typedef struct s_usine  t_usine;

struct          s_give {
   int         from;
   int         to;
   int         nb;
};

struct          s_road {
   int         from;
   int         to;
   int         dist;
};

struct          s_troop {
   int         owner;
   int         from;
   int         to;
   int         dist;
   int         nb;
};

struct          s_usine {
   int         owner;
   int         cyb;
   int         prod;
   int         total_troop;
   int         priority;
   t_road      *road;
};

/*
   NOTES SUR LES STRUCTURES :
   La structure road : Sert a determiner chaque route, avec l'usine d'origine (from), l'usine d'arrivée (to), et la distance (dist). Ils sont biderectionnels.
   La structure troop : Sert a sauvegarder chaque troupe actuellement non dans une usine.
       Leurs temps avant arrivée est indiqué par dist, leur cible par to, leur nombre par nb, et elles appartiennent au joueur owner.
   La structure usine : Sert a sauvegarder l'état de chaque usine.
       Son possesseur est indiqué par owner, sa prod par prod, sa protection par cyb, son interet par priority, et son état si le jeu se finit sans que personne ne joue plus de coup par total_troop.
       Egalement, il est important de noter que la structure usine possede un attribut road, qui est la liste de toute les routes possibles pour cette usine.
           La route n partant de l'usine i pointe vers l'usine n, ainsi usine[usine[i].road[n].to] == usine[n] est vrai.
           Si la route n'existe pas, ou que l'usine veut savoir si une route la relie a elle meme, la distance entre les deux usines vaut -1. Sinon, elle vaut entre 0 et 20.
*/

static t_usine  *usine;

/* TOOLS */

int     *insert(int *tab, int value, int pos, int size)
{
   int i;

   i = size;

   while (--i > pos)
       tab[i] = tab[i - 1];
   tab[pos] = value;
   return (tab);
}

void    ft_strclr(char *s, int size)
{
   int		index;

   index = 0;
   while (index < size)
   {
       s[index] = '\0';
       index++;
   }
   return ;
}

/* PROCESSING THE AI */

int     *sort(t_usine *usine, int fct_count)
{
   int         *tab;
   int         i, j;

   tab = (int *)malloc(sizeof(*tab) * fct_count);
   i = -1;
   while (++i < fct_count)
       tab[i] = -1;

   i = -1;
   while (++i < fct_count)
   {
       j = -1;
       while (++j < fct_count)
       {
           if (tab[j] == -1 || usine[i].priority >= usine[tab[j]].priority)
           {
               insert(tab, i, j, fct_count);
               break ;
           }
       }
   }

   return (tab);
}

char    *check_given(t_give *given, char *str)
{
   char    *tmp;
   int     i;

   tmp = (char *)malloc(sizeof(*tmp) * 3);

   i = -1;
   while (given[++i].nb != -1)
   {
       strcat(str, "MOVE ");
       sprintf(tmp, "%d", given[i].from);
       strcat(str, tmp);
       ft_strclr(tmp, 3);
       strcat(str, " ");
       sprintf(tmp, "%d", given[i].to);
       strcat(str, tmp);
       ft_strclr(tmp, 3);
       strcat(str, " ");
       sprintf(tmp, "%d", given[i].nb);
       strcat(str, tmp);
       ft_strclr(tmp, 3);
       strcat(str, ";");
   }
   return (str);
}

t_give  *add_troop(int from, int to, int nb, t_give *given)
{
   int     i;

   i = 0;
   while (given[i].nb != -1)
       i++;
   given[i + 1].nb = -1;
   given[i].from = from;
   given[i].to = to;
   given[i].nb = nb;
   return (given);
}

int     can_send(t_usine *usine, int from, int to, int fct_count)
{
   int     nb;
   int     needed;

   needed = 0;
   if (usine[to].owner == -1)
       needed = usine[to].prod * (usine[to].road[from].dist + 1) + usine[to].total_troop;
   if (usine[to].owner == 0)
       needed = usine[to].total_troop;
   if (needed <= 0)
       needed = 0;
   else if (needed > usine[from].cyb)
       needed = 0;
   else
       nb = needed + 1;
   if (usine[from].total_troop - nb < 0)
       nb = 0;
   return (nb);
}

t_give  *send_troop(t_usine *usine, int to, int fct_count, t_give *given)
{
   int     nb;
   int     i;

   i = -1;
   while (++i < fct_count)
   {
       if (i != to && usine[to].road[i].dist != 1 && usine[i].owner == 1 && usine[to].prod != 0)
       {
           nb = 0;
           nb = can_send(usine, i, to, fct_count);
           if (nb > usine[i].cyb || nb <= usine[to].cyb)
               nb = 0;
           if (nb)
               given = add_troop(i, to, nb, given);
       }
   }
   return (given);
}

char    *check_bomb(t_usine *usine, t_troop *troop, int fct_count, int troop_count)
{
    char    *str;
    char    *tmp;
   int     i;

   str = (char *)malloc(sizeof(*str) * 11);
   ft_strclr(str, 11);

   tmp = (char *)malloc(sizeof(*tmp) * 3);
   ft_strclr(tmp, 3);

   i = -1;
   while (++i < fct_count)
   {

   }
   return ("");
}

char    *check_inc(t_usine *usine, t_troop *troop, int fct_count, int troop_count)
{
   int     i;

   i = -1;
   while (++i < fct_count)
   {

   }
   return ("");
}

void    process(t_usine *usine, t_troop *troop, int fct_count, int troop_count)
{
   t_give      *given;
   char        *str;
   int         *tab;
   int         i;

   str = (char *)malloc(sizeof(*str) * 501);
   ft_strclr(str, 501);

   given = (t_give *)malloc(sizeof(*given) * 501);
   given[0].nb = -1;

   fprintf(stderr, "Salut ! Ceci est un test :)\n");

   i = -1;
   while (++i < fct_count)
       if (usine[i].owner != 1)
           given = send_troop(usine, i, fct_count, given);

   strcat(str, check_bomb(usine, troop, fct_count, troop_count));
   strcat(str, check_inc(usine, troop, fct_count, troop_count));
   str = check_given(given, str);

#ifdef DEBUG
   i = -1;

   fprintf(stderr, "LISTES DES PRIORITES :\n");
   while (++i < fct_count)
       fprintf(stderr, "Prioritée pour l'usine %d : %d\n", i, usine[i].priority);
   fprintf(stderr, "FIN DE LA LISTE DES PRIORITES.\n");

   i = -1;

   fprintf(stderr, "LISTES DES PRIORITES :\n");
   while (++i < fct_count)
       fprintf(stderr, "Prioritée numéro %d : Usine %d\n", i + 1, tab[i]);
   fprintf(stderr, "FIN DE LA LISTE DES PRIORITES.\n");
#endif

   printf("%sMSG Can't touch this !\n", str);

   return ;
}

/* PART GETTING INFORMATIONS */

/*

   I think this one need a bit of an explanation :
   road is the list of all roads / links in the map.
   usine is the list of all usines in the map.

   First, for each factories in the map, we create a "road" part that can contain up to the number of factories of road / link.
   Next, we initialize each road to the NONE value (wich is -1 for this).
   Then we check for each link in the road list if we have a link for this factory.
   If yes, we then fill the value with the one for the link.
   Links are two ways so we don't care if it's from or to.

*/

t_usine *link_road(t_road *road, t_usine *usine, int fct_count, int lk_count)
{
   int     i;
   int     j;

   i = -1;
   while (++i < fct_count)
   {

#ifdef  DEBUG
       fprintf(stderr, "Initalisation pour l'usine %d : START\n", i);
#endif

       usine[i].road = (t_road *)malloc(sizeof(*road) * fct_count);

#ifdef DEBUG
       fprintf(stderr, "Initalisation pour l'usine %d : CREATION COMPLETE\n", i);
#endif

       j = -1;
       while (++j < fct_count)
       {
           usine[i].road[j].from = i;
           usine[i].road[j].to = j;
           usine[i].road[j].dist = -1;
       }

#ifdef DEBUG
       fprintf(stderr, "Initalisation pour l'usine %d : DEFAULT SETUP COMPLETE\n", i);
#endif

       j = -1;
       while (++j < lk_count)
       {
           if (road[j].from == i)
           {
               usine[i].road[road[j].to].from = road[j].from;
               usine[i].road[road[j].to].to = road[j].to;
               usine[i].road[road[j].to].dist = road[j].dist;
           }
           else if (road[j].to == i)
           {
               usine[i].road[road[j].from].to = road[j].from;
               usine[i].road[road[j].from].from = road[j].to;
               usine[i].road[road[j].from].dist = road[j].dist;
           }
       }

#ifdef DEBUG
       fprintf(stderr, "Initalisation pour l'usine %d : FIN\n", i);
#endif

   }
   i = -1;
   return (usine);
}

int main()
{
   int         factory_count; // the number of factories
   t_usine     *usine; // the list of factories
   int         link_count; // the number of links between factories
   t_road      *road; // the list of links between factories
   int         entity_count; // the number of entities (e.g. factories and troops)
   int         troop_count; // the number of troops in movement (e.g. entities - factories)
   t_troop     *troop; // the list of troops in movement
   int         tour;
   int         i;
   int         j;

   scanf("%d", &factory_count);
   scanf("%d", &link_count);

   // setup list of usine
   usine = (t_usine *)malloc(sizeof(*usine) * (factory_count));

   // setup list of roads
   road = (t_road *)malloc(sizeof(*road) * (link_count));

   i = -1;
   while (++i < link_count) {
       scanf("%d%d%d", &(road[i].from), &(road[i].to), &(road[i].dist));
   }

   // give to each factory the list of road reaching it
   usine = link_road(road, usine, factory_count, link_count);

   tour = 0;

#ifdef DEBUG
   i = -1;
   while (++i < factory_count)
   {
       fprintf(stderr, "LISTES DES ROUTES POUR L'USINE %d :\n", i);
       j = -1;
       while (++j < factory_count)
       {
           if (j != i && usine[i].road[j].dist != -1)
           {
               fprintf(stderr, "Vers l'usine %d avec un temps de trajet de %d tours.\n", usine[i].road[j].to, usine[i].road[j].dist);
           }
       }
   }
#endif

   // game loop
   while (1) {
       tour++;
       scanf("%d", &entity_count); //get the number of entity each turn

       troop_count = entity_count - factory_count;
       troop = (t_troop *)malloc(sizeof(*troop) * troop_count);

       j = 0;

       i = -1;
       while (++i < entity_count)
       {
           int entityId;
           char entityType[11];
           int arg1;
           int arg2;
           int arg3;
           int arg4;
           int arg5;

           scanf("%d%s%d%d%d%d%d", &entityId, entityType, &arg1, &arg2, &arg3, &arg4, &arg5);

           if (strcmp(entityType, "FACTORY") == 0)
           {
               usine[entityId].owner = arg1;
               usine[entityId].cyb = arg2;
               usine[entityId].prod = arg3;
               usine[entityId].total_troop = arg2;
           }
           else if (strcmp(entityType, "TROOP") == 0)
           {
               troop[j].owner = arg1;
               troop[j].from = arg2;
               troop[j].to = arg3;
               troop[j].dist = arg5;
               troop[j].nb = arg4;
               usine[arg3].total_troop += usine[arg3].owner == arg1 ? arg4 : -arg4;
               j++;
           }

       }

#ifdef DEBUG
       i = -1;
       while (++i < troop_count)
       {
           fprintf(stderr, "PROPRIETES DE LA TROUPE %d :\n", i);
           fprintf(stderr, "Propiétaire : %s\n", troop[i].owner == 0 ? "NEUTRE" : usine[i].owner == 1 ? "Vous" : "Ennemie");
           fprintf(stderr, "Nombre de cyborgs : %d\n", troop[i].nb);
           fprintf(stderr, "Temps avant arrivée : %d\n", troop[i].dist);
           fprintf(stderr, "Origine de l'armée : %d\n", troop[i].from);
           fprintf(stderr, "Destination de l'armée : %d\n", troop[i].to);
       }

       i = -1;
       while (++i < factory_count)
       {
           fprintf(stderr, "PROPRIETES DE L'USINE %d :\n", i);
           fprintf(stderr, "Propiétaire : %s\n", usine[i].owner == 0 ? "NEUTRE" : usine[i].owner == 1 ? "Vous" : "Ennemie");
           fprintf(stderr, "Nombre de cyborgs : %d\n", usine[i].cyb);
           fprintf(stderr, "Production par tour : %d\n", usine[i].prod);
       }
#endif

       // Write an action using printf(). DON'T FORGET THE TRAILING \n
       // To debug: fprintf(stderr, "Debug messages...\n");

       // This is the part where the "IA" is created, with all the informations gathered beforehand
       process(usine, troop, factory_count, troop_count);

       free(troop);
       troop = NULL;
   }

   return 0;
}
