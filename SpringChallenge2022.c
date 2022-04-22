#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

// DEFINE CONSTANT DECLARATIONS 
#define MAP_HEIGHT 9000
#define MAP_WIDTH 17630
#define HEROES_COUNT 3
#define BASE_MAX_HP 3

// DEFINE FUNCTIONS DECLARATIONS
#define ABS(x) ((x < 0) ? -(x) : (x))

// TYPEDEF ALL STRUCTURES
typedef struct s_player     t_player;
typedef struct s_base       t_base;
typedef struct s_hero       t_hero;
typedef struct s_monster    t_monster;
typedef struct s_entity     t_entity;
typedef struct s_pos        t_pos;
typedef struct s_game       t_game;

// ALL STRUCTURES DECLARATIONS
struct s_pos {
    int x;
    int y;
};

struct s_entity {
    t_pos   *pos;
    int     id;
};

struct s_monster {
    t_entity    *entity;
    t_pos       *direction;
    int         hp;
    int         nearBase;
    int         threatFor;
};

struct s_hero {
    t_entity    *entity;
    int         owner;
    int         damage;
};

struct s_base {
    t_pos   *pos;
    int     hp;
};

struct s_player {
    int     id;
    t_base  *base;
    t_hero  **heroes;
    int     heroCount;
};

struct s_game {
    t_player    *me;
    t_player    *opponent;
};

// FUNCTIONS DECLARATIONS
void        debug(char *s);
void        printPlayer(t_player *p);
void        printGame(t_game *g);
void        printHero(t_hero *h);
void        printPos(t_pos *pos);
void        printBase(t_base *b);
void        printMonster(t_monster *monster);
void        printMonsters(t_monster **monsters, int monsterCount);
t_pos       *initPos();
t_pos       *initPosValues(int x, int y);
t_entity    *initEntity(int id, int x, int y);
t_monster   *initMonster(int id, int x, int y, int hp, int vx, int vy, int nearBase, int threatFor);
t_base      *initBase(int baseX, int baseY);
t_player    *initPlayer(int id, int heroCount, int baseX, int baseY);
t_game      *initGame();


// TOOLS FUNCTIONS
void        debug(char *s)
{
    fprintf(stderr, s);
}

void        printPlayer(t_player *p)
{
    fprintf(stderr, "Player %d\n", p->id);
    printBase(p->base);
}

void        printGame(t_game *g)
{
    printPlayer(g->me);
    printPlayer(g->opponent);
}

void        printHero(t_hero *h)
{
    return ; // TODO
}

void        printPos(t_pos *pos)
{
    fprintf(stderr, "x: %d\ny: %d\n", pos->x, pos->y);
}

void        printEntity(t_entity *e)
{
    fprintf(stderr, "Entity :\n");
    printPos(e->pos);
    fprintf(stderr, "Id : %d\n", e->id);
}

void        printBase(t_base *b)
{
    fprintf(stderr, "Base :\n");
    printPos(b->pos);
    fprintf(stderr, "HP: %d\n", b->hp);
}

void        printMonster(t_monster *monster)
{
    fprintf(stderr, "Monster :\n");
    printEntity(monster->entity);
    printPos(monster->direction);
    fprintf(stderr, "HP : %d\nNear : %s\nThreat for : %s\n", monster->hp, (monster->nearBase ? "Someone" : "No one"), (monster->threatFor == 1 ? "Me" : "Not me"));
}

void        printMonsters(t_monster **monsters, int monsterCount)
{
    for (int i = 0; i < monsterCount; i++)
    {
        printMonster(monsters[i]);
    }
}


// PROCESS FUNCTIONS
t_pos   *initPos()
{
    t_pos *pos;

    pos = (t_pos *)malloc(sizeof(*pos));

    return (pos);
}

t_pos   *initPosValues(int x, int y)
{
    t_pos *pos;

    pos = (t_pos *)malloc(sizeof(*pos));
    pos->x = x;
    pos->y = y;

    return (pos);
}

t_entity *initEntity(int id, int x, int y)
{
    t_entity *e;

    e = (t_entity *)malloc(sizeof(*e));
    e->id = id;
    e->pos = initPosValues(x, y);

    return (e);
}

t_monster *initMonster(int id, int x, int y, int hp, int vx, int vy, int nearBase, int threatFor)
{
    t_monster *m;

    m = (t_monster *)malloc(sizeof(*m));
    m->entity = initEntity(id, x, y);
    m->hp = hp;
    m->direction = initPosValues(vx, vy);
    m->nearBase = nearBase;
    m->threatFor = threatFor;

    return (m);
}

t_base  *initBase(int baseX, int baseY)
{
    t_base *b;

    b = (t_base *)malloc(sizeof(*(b)));
    b->pos = initPosValues(baseX, baseY);
    b->pos->x = baseX;
    b->pos->y = baseY;
    b->hp = BASE_MAX_HP;

    return (b);
}

t_player *initPlayer(int id, int heroCount, int baseX, int baseY)
{
    t_player *p;

    p = (t_player *)malloc(sizeof(*p));
    p->id = id;
    p->base = initBase(baseX, baseY);
    p->heroes = (t_hero **)malloc(sizeof(*(p->heroes)) * heroCount);
    p->heroCount = heroCount;

    return (p);
}

t_game  *initGame()
{
    t_game *game;
    int myBaseX;
    int myBaseY;
    int heroCount;

    game = (t_game *)malloc(sizeof(*game));

    scanf("%d%d", &myBaseX, &myBaseY);
    scanf("%d", &heroCount);

    game->me = initPlayer(0, heroCount, myBaseX, myBaseY);
    game->opponent = initPlayer(1, heroCount, ABS(myBaseX - MAP_WIDTH), ABS(myBaseY - MAP_HEIGHT));

    return game;
}

int     getDistance(t_pos *a, t_pos *b)
{
    return sqrt((b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y));
}

t_pos   *findClosestThreat(t_player *me, t_monster **monsters, int monsterCount)
{
    int minDistance = 700000;
    t_pos *pos = initPosValues(me->base->pos->x, me->base->pos->y);

    for (int i = 0; i < monsterCount; i++)
    {
        if (monsters[i]->threatFor == 1)
        {
            int monsterDistanceToBase = getDistance(me->base->pos, monsters[i]->entity->pos);
            if (monsterDistanceToBase < minDistance)
            {
                pos = monsters[i]->entity->pos;
                minDistance = monsterDistanceToBase;
            }
        }
    }

    return (pos);
}


// MAIN
int main()
{
    t_monster **monsters;
    t_game *game = initGame();
    printGame(game);

    // game loop
    while (1) {
        for (int i = 0; i < 2; i++) {
            // Your base health
            int health;
            // Ignore in the first league; Spend ten mana to cast a spell
            int mana;
            scanf("%d%d", &health, &mana);
        }
        // Amount of heros and monsters you can see
        int entity_count;
        scanf("%d", &entity_count);
        monsters = (t_monster **)malloc(sizeof(*monsters) * entity_count - 6);
        int monsterCount = 0;
        for (int i = 0; i < entity_count; i++) {
            // Unique identifier
            int id;
            // 0=monster, 1=your hero, 2=opponent hero
            int type;
            // Position of this entity
            int x;
            int y;
            // Ignore for this league; Count down until shield spell fades
            int shield_life;
            // Ignore for this league; Equals 1 when this entity is under a control spell
            int is_controlled;
            // Remaining health of this monster
            int health;
            // Trajectory of this monster
            int vx;
            int vy;
            // 0=monster with no target yet, 1=monster targeting a base
            int near_base;
            // Given this monster's trajectory, is it a threat to 1=your base, 2=your opponent's base, 0=neither
            int threat_for;
            scanf("%d%d%d%d%d%d%d%d%d%d%d", &id, &type, &x, &y, &shield_life, &is_controlled, &health, &vx, &vy, &near_base, &threat_for);
            if (type == 0) {
                monsters[monsterCount] = initMonster(id, x, y, health, vx, vy, near_base, threat_for);
                monsterCount++;
            }
        }
        printMonsters(monsters, monsterCount);
        t_pos *target = findClosestThreat(game->me, monsters, monsterCount);
        for (int i = 0; i < game->me->heroCount; i++) {

            // Write an action using printf(). DON'T FORGET THE TRAILING \n
            // To debug: fprintf(stderr, "Debug messages...\n");


            // In the first league: MOVE <x> <y> | WAIT; In later leagues: | SPELL <spellParams>;
            printf("MOVE %d %d\n", target->x, target->y);
        }
    }

    return 0;
}