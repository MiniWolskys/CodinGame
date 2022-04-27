#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

// SYSTEM DEFINE
#define FALSE 0
#define TRUE 1

// DEFINE CONSTANT DECLARATIONS 
#define MAP_HEIGHT 9000
#define MAP_WIDTH 17630
#define HEROES_COUNT 3
#define BASE_MAX_HP 3
#define BASE_VISION_RANGE 5000
#define MONSTER_MOVEMENT_SPEED 400
#define HERO_MOVEMENT_SPEED 800
#define HERO_ATTACK_RANGE 800
#define HERO_VISION_RANGE 2200

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
typedef enum e_role         t_role;


// ALL STRUCTURES DECLARATIONS
enum e_role {
    NO_ROLE,
    GOALKEEPER,
    SCOUT,
    DEFENDER,
    ATTACKER
};


struct s_pos {
    int x;
    int y;
};

struct s_entity {
    int     id;
    t_pos   *direction;
    t_pos   *pos;
    int     movementSpeed;
};

struct s_monster {
    t_entity    *entity;
    int         hp;
    int         nearBase;
    int         threatFor;
    int         isTargeted;
};

struct s_hero {
    t_entity    *entity;
    t_role      role;
    int         mine;
    int         isControlled;
    int         shield;
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
    int     mana;
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

int         getDistance(t_pos *a, t_pos *b);

void        playGoalKeeper(t_game *game, t_monster **monsters, int monsterCount);

t_monster   *findClosestThreat(t_pos *pos, t_monster **monsters, int monsterCount);
t_pos       *intercept(t_entity *from, t_entity *target, t_base *base);
t_pos       *moveByPos(t_pos *a, t_pos *b);
t_pos       *moveByValues(t_pos *a, int vx, int vy);

t_pos       *initPos();
t_pos       *initPosValues(int x, int y);
t_entity    *initEntity(int id, int x, int y, int vx, int vy, int movementSpeed);
t_monster   *initMonster(int id, int x, int y, int hp, int vx, int vy, int nearBase, int threatFor);
t_hero      *initHero(int id, int x, int y, int shield, int isControlled, int owner);
t_base      *initBase(int baseX, int baseY);
t_player    *initPlayer(int id, int heroCount, int baseX, int baseY);
t_game      *initGame();


// TOOLS FUNCTIONS
void        debug(char *s)
{
    fprintf(stderr, "%s", s);
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
    printPos(e->direction);
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
    fprintf(stderr, "HP : %d\nNear : %s\nThreat for : %s\n", monster->hp, (monster->nearBase ? "Someone" : "No one"), (monster->threatFor == 1 ? "Me" : "Not me"));
}

void        printMonsters(t_monster **monsters, int monsterCount)
{
    for (int i = 0; i < monsterCount; i++)
    {
        printMonster(monsters[i]);
    }
}

int     getDistance(t_pos *a, t_pos *b)
{
    return sqrt((b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y));
}

t_pos   *moveByPos(t_pos *a, t_pos *b)
{
    return (moveByValues(a, b->x, b->y));
}

t_pos   *moveByValues(t_pos *a, int vx, int vy)
{
    return (initPosValues(a->x + vx, a->y + vy));
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

t_entity *initEntity(int id, int x, int y, int vx, int vy, int movementSpeed)
{
    t_entity *e;

    e = (t_entity *)malloc(sizeof(*e));
    e->id = id;
    e->pos = initPosValues(x, y);
    e->direction = initPosValues(vx, vy);
    e->movementSpeed = movementSpeed;

    return (e);
}

t_monster *initMonster(int id, int x, int y, int hp, int vx, int vy, int nearBase, int threatFor)
{
    t_monster *m;

    m = (t_monster *)malloc(sizeof(*m));
    m->entity = initEntity(id, x, y, vx, vy, MONSTER_MOVEMENT_SPEED);
    m->hp = hp;
    m->nearBase = nearBase;
    m->threatFor = threatFor;
    m->isTargeted = false;

    return (m);
}

t_hero  *initHero(int id, int x, int y, int shield, int isControlled, int owner)
{
    t_hero *h;

    h = (t_hero *)malloc(sizeof(*h));
    h->mine = owner;
    h->entity = initEntity(id, x, y, 0, 0, HERO_MOVEMENT_SPEED);
    h->role = NO_ROLE;
    h->isControlled = isControlled;
    h->shield = shield;

    return (h);
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
    p->heroes = (t_hero **)malloc(sizeof(t_hero *) * heroCount);
    p->heroCount = heroCount;
    p->mana = 0;

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

t_pos   *calculateNewDirection(t_pos *A, t_pos *B)
{
    int distance;
    int x;
    int y;

    distance = getDistance(A, B);
    x = ABS(A->x - B->x) * (MONSTER_MOVEMENT_SPEED / distance);
    y = ABS(A->y - B->y) * (MONSTER_MOVEMENT_SPEED / distance);

    return (initPosValues(x, y));
}

t_pos   *calculateCollisionPosition(t_entity *from, t_entity *target, t_base *base)
{
    t_pos *whereToGo;
    t_pos *newMonsterPosition;
    t_pos *monsterDirection;
    int previousDistance;
    int distance;
    int i;

    newMonsterPosition = target->pos;
    previousDistance = getDistance(from->pos, newMonsterPosition);
    monsterDirection = target->direction;
    i = 0;
    while (distance - HERO_ATTACK_RANGE > i * HERO_MOVEMENT_SPEED)
    {
        previousDistance = distance;
        distance = getDistance(from->pos, newMonsterPosition);
        if (distance > previousDistance) {
            break;
        }
        newMonsterPosition = moveByPos(newMonsterPosition, monsterDirection);
        if (getDistance(newMonsterPosition, base->pos) < 5000)
        {
            monsterDirection = calculateNewDirection(base->pos, newMonsterPosition);
        }
        i++;
    }
    whereToGo = newMonsterPosition;

    return (whereToGo);
}

t_pos   *intercept(t_entity *from, t_entity *target, t_base *base)
{
    if (getDistance(from->pos, target->pos) < HERO_ATTACK_RANGE)
    {
        return (initPosValues(target->pos->x + target->direction->x, target->pos->y + target->direction->y));
    }
    return (calculateCollisionPosition(from, target, base));
}

t_monster   *findClosestEnemy(t_pos *pos, t_monster **monsters, int monsterCount)
{
    int minDistance = 700000;
    t_monster *targetMonster = NULL;

    for (int i = 0; i < monsterCount; i++)
    {
        if (monsters[i]->threatFor != 2) {
            int monsterDistanceToBase = getDistance(pos, monsters[i]->entity->pos);
            if (monsterDistanceToBase < minDistance)
            {
                if (targetMonster) {
                    targetMonster->isTargeted = FALSE;
                }
                targetMonster = monsters[i];
                targetMonster->isTargeted = TRUE;
                minDistance = monsterDistanceToBase;
            }
        }
    }

    return (targetMonster);
}

t_monster   *findClosestThreat(t_pos *pos, t_monster **monsters, int monsterCount)
{
    int minDistance = 700000;
    t_monster *targetMonster = NULL;

    for (int i = 0; i < monsterCount; i++)
    {
        if (monsters[i]->isTargeted == FALSE && monsters[i]->threatFor == 1)
        {
            int monsterDistanceToBase = getDistance(pos, monsters[i]->entity->pos);
            if (monsterDistanceToBase < minDistance)
            {
                if (targetMonster) {
                    targetMonster->isTargeted = FALSE;
                }
                targetMonster = monsters[i];
                targetMonster->isTargeted = TRUE;
                minDistance = monsterDistanceToBase;
            }
        }
    }

    return (targetMonster);
}

t_monster **findNearbyMonsters(t_pos *pos, t_monster **monsters, int monsterCount, int maxDistance)
{
    t_monster **nearbyMonsters;
    int nearbyMonsterCount;
    int i;

    nearbyMonsterCount = 0;
    for (i = 0; i < monsterCount; i++)
    {
        if (getDistance(pos, monsters[i]->entity->pos) < maxDistance)
        {
            nearbyMonsterCount++;
        }
    }

    nearbyMonsters = (t_monster **)malloc(sizeof(t_monster *) * nearbyMonsterCount + 1);
    nearbyMonsters[nearbyMonsterCount] = NULL;

    nearbyMonsterCount = 0;
    for (i = 0; i < monsterCount; i++)
    {
        if (getDistance(pos, monsters[i]->entity->pos) < maxDistance)
        {
            nearbyMonsters[nearbyMonsterCount] = monsters[i];
            nearbyMonsterCount++;
        }
    }
    return (nearbyMonsters);
}

int     getTotalMonsterHp(t_monster **monsters)
{
    int totalHp;
    int i;

    i = 0;
    totalHp = 0;
    while (monsters[i])
    {
        totalHp += monsters[i]->hp;
        i++;
    }

    return (totalHp);
}

int     getMonsterNumber(t_monster **monsters)
{
    int i;

    i = 0;
    while (monsters[i])
    {
        i++;
    }
    return (i);
}

void    playDefenser(t_hero *hero, t_game *game, t_monster **monsters, int monsterCount)
{
    t_monster **monstersNearBase;
    t_monster **monstersNearHero;
    t_monster *closestThreat;
    int monsterNearBaseCount;

    fprintf(stderr, "DEFENSER: Let's see what I'll do...\n");
//    monstersNearBase = findNearbyMonsters(game->me->base->pos, monsters, monsterCount, BASE_VISION_RANGE);
//    if (getTotalMonsterHp(monstersNearBase) > 40)
//    {
//        fprintf(stderr, "DEFENSER: I'll help the goalkeeper !\n");
//        playGoalKeeper(game, monsters, monsterCount);
//        return ;
//    }
    monstersNearHero = findNearbyMonsters(hero->entity->pos, monsters, monsterCount, HERO_VISION_RANGE);
    if (getMonsterNumber(monstersNearHero) > 0)
    {
        closestThreat = findClosestEnemy(hero->entity->pos, monstersNearHero, getMonsterNumber(monstersNearHero));
        if (closestThreat)
        {
            fprintf(stderr, "DEFENSER: I'll attack the closest guy !\n");
            printf("MOVE %d %d\n", closestThreat->entity->pos->x, closestThreat->entity->pos->y);
            return ;
        }
    }
    fprintf(stderr, "DEFENSER: Nothing to do...\n");
    printf("MOVE %d %d\n", ABS(game->me->base->pos->x - 4500), ABS(game->me->base->pos->y - 4500));
}

void    playGoalKeeper(t_game *game, t_monster **monsters, int monsterCount)
{
    t_monster   *targetMonster;
    t_pos       *interceptedTarget;

    targetMonster = findClosestThreat(game->me->base->pos, monsters, monsterCount);
    if (targetMonster) {
        interceptedTarget = intercept(game->me->heroes[0]->entity, targetMonster->entity, game->me->base);
    }
    if (targetMonster) {
        if (getDistance(targetMonster->entity->pos, game->me->base->pos) < 800 && game->me->mana > 9 && targetMonster->hp > 2) {
            printf("SPELL WIND 1500 1500\n");
        }
        else {
            printf("MOVE %d %d\n", interceptedTarget->x, interceptedTarget->y);
        }
    }
    else {
        printf("MOVE %d %d\n", ABS(game->me->base->pos->x - 1500), ABS(game->me->base->pos->y - 1500));
    }
}


// MAIN
int main()
{    
    t_monster **monsters;
    t_monster **targets;
    t_pos *interceptedTarget;
    t_monster *targetMonster;
    int monsterCount;
    int myHeroCount;
    int oppHeroCount;
    int entity_count;
    int i;

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
    
    t_game *game = initGame();
    printGame(game);

    // game loop
    while (1) {
        scanf("%d%d", &game->me->base->hp, &game->me->mana);
        scanf("%d%d", &game->opponent->base->hp, &game->opponent->mana);
        // Amount of heros and monsters you can see
        scanf("%d", &entity_count);
        monsters = (t_monster **)malloc(sizeof(t_monster *) * entity_count - 6);
        monsterCount = 0;
        myHeroCount = 0;
        oppHeroCount = 0;
        for (i = 0; i < entity_count; i++) {
            scanf("%d%d%d%d%d%d%d%d%d%d%d", &id, &type, &x, &y, &shield_life, &is_controlled, &health, &vx, &vy, &near_base, &threat_for);
            if (type == 0) {
                monsters[monsterCount] = initMonster(id, x, y, health, vx, vy, near_base, threat_for);
                monsterCount++;
            }
            if (type == 1) {
                game->me->heroes[myHeroCount] = initHero(id, x, y, shield_life, is_controlled, 1);
                myHeroCount++;
            }
            if (type == 2) {
                game->opponent->heroes[oppHeroCount] = initHero(id, x, y, shield_life, is_controlled, 0);
                oppHeroCount++;
            }
        }
        printMonsters(monsters, monsterCount);
        playGoalKeeper(game, monsters, monsterCount);
        playDefenser(game->me->heroes[1], game, monsters, monsterCount);
        playGoalKeeper(game, monsters, monsterCount);
       // printf("MOVE %d %d\n", ABS(game->opponent->base->pos->x - 4500), ABS(game->opponent->base->pos->y - 4500));
    }

    return 0;
}
