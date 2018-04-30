#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PROD

#define NONE -1
#define MINE 0
#define TOWER 1
#define BARRACK 2

#define QUEEN -1
#define KNIGHT 0
#define ARCHER 1
#define GIANT 2

#define ANY -2
#define NEUTRAL -1
#define ALLY 0
#define ENEMY 1

#define KNIGHT_COST 80
#define ARCHER_COST 100
#define GIANT_COST 140

#define ABS(x) x < 0 ? -x : x

typedef struct s_pos	t_pos;
typedef struct s_site	t_site;
typedef struct s_unit	t_unit;

struct		s_pos
{
	int		x;
	int		y;
};

struct		s_site
{
	int		id;
	t_pos	pos;
	int		radius;
	int		owner;
	int 	gold;
	int 	maxSize;
	int		type;
	int		param1;
	int		param2;
};

struct		s_unit
{
	t_pos	pos;
	int		owner;
	int		type;
	int		health;
};

int	getDistance(t_pos obj1, t_pos obj2);
int findNearestObject(t_pos from, t_site *all, int size, int target, int team);
int findSpecificUnit(t_unit *all, int size, int target, int team);
int countUnit(t_unit *all, int size, int target, int team);
int countObject(t_site *all, int size, int target, int team);
int estimateTimeBeforeCollision(t_pos from, t_pos to, int speed, int range, int radius1, int radius2);
int findNearestUnit(t_pos from, t_unit *all, int size, int target, int team);

int main()
{
	// Output declaration
	char	*queenMove;
	char	*trainMove;
	int 	*trainMoveStore;

	// Input declaration
	t_site	*sites;
	t_unit	*units;
	int 	numSites;
	int 	numUnits;
	int 	touchedSite;
	int 	gold;

	// Index declaration
	int 	i, j;

	// Utils declaration
	int 	mineCount;
	int 	kBarrackCount;
	int 	ourQueen;
	int 	othQueen;
	int 	nearestSite;
	int 	enemyKCount;
	int 	towerCount;
	int 	boostingMines;
	int 	boostingTower;
	int 	count;
	int 	closeKnight;
	int 	estimatedTime;

	// Game initialization

    scanf("%d", &numSites);
	sites = (t_site *)malloc(sizeof(*sites) * numSites);

    for (i = 0; i < numSites; i++) {
        scanf("%d%d%d%d", &(sites[i].id), &(sites[i].pos.x), &(sites[i].pos.y), &(sites[i].radius));
    }

    // game loop
    while (1) {

		// Turn initialization

		scanf("%d%d", &gold, &touchedSite);

		for (i = 0; i < numSites; i++) {
            scanf("%d%d%d%d%d%d%d", &(sites[i].id), &(sites[i].gold), &(sites[i].maxSize), &(sites[i].type), &(sites[i].owner), &(sites[i].param1), &(sites[i].param2));
		}

        scanf("%d", &numUnits);
		units = (t_unit *)malloc(sizeof(*units) * numUnits);

		for (i = 0; i < numUnits; i++) {
            scanf("%d%d%d%d%d", &(units[i].pos.x), &(units[i].pos.y), &(units[i].owner), &(units[i].type), &(units[i].health));
        }

#ifdef DEBUG

		fprintf(stderr, "DEBUG START\n");

		for (i = 0; i < numSites; i++)
		{
			fprintf(stderr, "For Site %d , pos x = %d & pos y = %d\n", i, sites[i].pos.x, sites[i].pos.y);
		}

		for (i = 0; i < numUnits; i++)
		{
			fprintf(stderr, "For Unit %d , pos x = %d & pos y = %d\n", i, units[i].pos.x, units[i].pos.y);
		}

		fprintf(stderr, "DEBUG END\n");

#endif

		// Output initialization

		queenMove = (char *)malloc(sizeof(*queenMove) * 100);
		trainMoveStore = (int *)malloc(sizeof(*trainMoveStore) * 5);
		trainMoveStore[0] = 0;
		trainMoveStore[1] = 0;
		trainMoveStore[2] = 0;
		trainMoveStore[3] = 0;
		trainMoveStore[4] = 0;
		sprintf(queenMove, "WAIT");

		// Processing

		ourQueen = findSpecificUnit(units, numUnits, QUEEN, ALLY);
		othQueen = findSpecificUnit(units, numUnits, QUEEN, ENEMY);

		boostingMines = 0;
		boostingTower = 0;

		mineCount = countObject(sites, numSites, MINE, ALLY);
		towerCount = countObject(sites, numSites, TOWER, ALLY);
		kBarrackCount = countObject(sites, numSites, BARRACK, ALLY);
		enemyKCount = countUnit(units, numUnits, KNIGHT, ENEMY);

		for (i = 0; i < numSites; i++)
		{
			if (touchedSite == sites[i].id && sites[i].type == MINE && sites[i].param1 < sites[i].maxSize)
			{
				boostingMines = 1;
				sprintf(queenMove, "BUILD %d MINE", touchedSite);
			}
			if (touchedSite == sites[i].id && sites[i].type == TOWER && sites[i].param2 < 500)
			{
				boostingTower = 1;
				sprintf(queenMove, "BUILD %d TOWER", touchedSite);
			}
		}

		if (enemyKCount && !boostingMines && !boostingTower)
		{
			if (towerCount < 4)
			{
				nearestSite = findNearestObject(units[ourQueen].pos, sites, numSites, NONE, NEUTRAL);
				sprintf(queenMove, "BUILD %d TOWER", sites[nearestSite].id);
			}
			else if (kBarrackCount < 2)
			{
				nearestSite = findNearestObject(units[ourQueen].pos, sites, numSites, NONE, NEUTRAL);
				sprintf(queenMove, "BUILD %d BARRACKS-KNIGHT", sites[nearestSite].id);
			}
			else
			{
				nearestSite = findNearestObject(units[ourQueen].pos, sites, numSites, NONE, NEUTRAL);
				if (sites[nearestSite].gold == 0)
					sprintf(queenMove, "BUILD %d TOWER", sites[nearestSite].id);
				else
					sprintf(queenMove, "BUILD %d MINE", sites[nearestSite].id);
			}
		}
		else if (mineCount < 3 && !boostingMines && !boostingTower)
		{
				nearestSite = findNearestObject(units[ourQueen].pos, sites, numSites, NONE, NEUTRAL);
				if (sites[nearestSite].gold == 0)
					sprintf(queenMove, "BUILD %d TOWER", sites[nearestSite].id);
				else
					sprintf(queenMove, "BUILD %d MINE", sites[nearestSite].id);
		}
		else if (kBarrackCount < 1 && !boostingMines && !boostingTower)
		{
				nearestSite = findNearestObject(units[ourQueen].pos, sites, numSites, NONE, NEUTRAL);
				sprintf(queenMove, "BUILD %d BARRACKS-KNIGHT", sites[nearestSite].id);
		}
		else if (towerCount == 0 && !boostingMines && !boostingTower)
		{
			nearestSite = findNearestObject(units[ourQueen].pos, sites, numSites, NONE, NEUTRAL);
			sprintf(queenMove, "BUILD %d TOWER", sites[nearestSite].id);
		}
		else if (mineCount == 3 && !boostingMines && !boostingTower)
		{
				nearestSite = findNearestObject(units[ourQueen].pos, sites, numSites, NONE, NEUTRAL);
				if (sites[nearestSite].gold == 0)
					sprintf(queenMove, "BUILD %d TOWER", sites[nearestSite].id);
				else
					sprintf(queenMove, "BUILD %d MINE", sites[nearestSite].id);
		}
		else if (kBarrackCount == 1 && !boostingMines && !boostingTower)
		{
				nearestSite = findNearestObject(units[ourQueen].pos, sites, numSites, NONE, NEUTRAL);
				sprintf(queenMove, "BUILD %d BARRACKS-KNIGHT", sites[nearestSite].id);
		}
		else if (!boostingMines && !boostingTower)
		{
				nearestSite = findNearestObject(units[ourQueen].pos, sites, numSites, NONE, NEUTRAL);
				sprintf(queenMove, "BUILD %d TOWER", sites[nearestSite].id);
		}

		if (mineCount >= 4 && towerCount >= 4 && kBarrackCount >= 2)
		{
			int x, y;
			nearestSite = findNearestObject(units[ourQueen].pos, sites, numSites, BARRACK, ENEMY);
			if (units[ourQueen].pos.x > sites[nearestSite].pos.x)
				y = 0;
			if (units[ourQueen].pos.x < sites[nearestSite].pos.x)
				y = 1000;
			if (units[ourQueen].pos.y > sites[nearestSite].pos.y)
				x = 0;
			if (units[ourQueen].pos.y < sites[nearestSite].pos.y)
				x = 1920;
			sprintf(queenMove, "MOVE %d %d", x, y);
		}

		closeKnight = findNearestUnit(units[ourQueen].pos, units, numUnits, KNIGHT, ENEMY);
		if (closeKnight != -1 && towerCount > 2)
		{
			estimatedTime = estimateTimeBeforeCollision(units[ourQueen].pos, units[closeKnight].pos, 100, 0, 20, 30);
			if (estimatedTime <= 3)
			{
				t_pos pos;
				int maxDist;
				t_pos tmpPos;
				int dist;
				maxDist = -1;
				nearestSite = findNearestObject(units[ourQueen].pos, sites, numSites, TOWER, ALLY);
				for (i = 0; i < 180; i++)
				{
					tmpPos.x = sites[nearestSite].pos.x + (float)(sites[nearestSite].radius) * cos(i);
					tmpPos.y = sites[nearestSite].pos.y + (float)(sites[nearestSite].radius) * cos(i);
					dist = getDistance(units[closeKnight].pos, tmpPos);
					if (dist > maxDist)
					{
						pos.x = tmpPos.x;
						pos.y = tmpPos.y;
						maxDist = dist;
					}
				}
				sprintf(queenMove, "MOVE %d %d", pos.x, pos.y);
			}
		}

		for (i = 1; i * KNIGHT_COST < gold; i++);
		i--;
		if (kBarrackCount > i)
			count = i;
		else
			count = kBarrackCount;
		if (kBarrackCount >= 1 && gold >= 1 * KNIGHT_COST)
		{
			trainMoveStore[0] = findNearestObject(units[othQueen].pos, sites, numSites, BARRACK, ALLY);
			j = 1;
			for (i = 0; i < numSites && j < count; i++)
			{
				if (sites[i].type == BARRACK && sites[i].owner == ALLY && sites[i].param1 == 0)
				{
					trainMoveStore[j] = sites[i].id;
					j++;
				}
			}
		}

		trainMove = (char *)malloc(sizeof(*trainMove) * 100);

		switch (count)
		{
			case 1 :
				sprintf(trainMove, "TRAIN %d", trainMoveStore[0]);
				break ;
			case 2 :
				sprintf(trainMove, "TRAIN %d %d", trainMoveStore[0], trainMoveStore[1]);
				break ;
			case 3 :
				sprintf(trainMove, "TRAIN %d %d %d", trainMoveStore[0], trainMoveStore[1], trainMoveStore[2]);
				break ;
			case 4 :
				sprintf(trainMove, "TRAIN %d %d %d %d", trainMoveStore[0], trainMoveStore[1], trainMoveStore[2], trainMoveStore[3]);
				break ;
			case 5 :
				sprintf(trainMove, "TRAIN %d %d %d %d %d", trainMoveStore[0]), trainMoveStore[1], trainMoveStore[2], trainMoveStore[3], trainMoveStore[4];
				break ;
			default :
				sprintf(trainMove, "TRAIN");
				break ;
		}

		// Printing our answer
        printf("%s\n", queenMove);
        printf("%s\n", trainMove);
    }

    return 0;
}

// Simple name for simple Rick. Return the distance between arg1 and arg2.

int	getDistance(t_pos obj1, t_pos obj2)
{
	return (sqrt((obj1.x - obj2.x) * (obj1.x - obj2.x) + (obj1.y - obj2.y) *(obj1.y - obj2.y)));
}

// Return the first unit of type TARGET and team TEAM found.
// Unit possibilities are QUEEN, KNIGHT, ARCHER and GIANT. Team possibilities are ANY, NEUTRAL, ALLY or ENEMY.
// If there is no such object of said team, return -1.

int findSpecificUnit(t_unit *all, int size, int target, int team)
{
	int 	i;

	for (i = 0; i < size; i++)
	{
		if (all[i].type == target && (all[i].owner == team || team == -2))
		{
			return (i);
		}
	}

	return (-1);
}

// Return the index of nearest UNIT from our t_unit array with team specified.
// Object possibilities are MINE, TOWER or BARRACK. Team possibilities are ANY, NEUTRAL, ALLY or ENEMY.
// If there is no such object of said team, return -1.

int findNearestObject(t_pos from, t_site *all, int size, int target, int team)
{
	int i;
	int dist;
	int maxDist, maxIndex;

	maxIndex = -1;
	maxDist = 2000000000;

	for (i = 0; i < size; i++)
	{
		if (all[i].type == target && (all[i].owner == team || team == -2))
		{
			dist = getDistance(from, all[i].pos);
			if (dist < maxDist)
			{
				maxDist = dist;
				maxIndex = i;
			}
		}
	}

	return (maxIndex);
}

// Return the count of UNIT from our t_unit array with specified team.
// Unit possibilities are QUEEN, KNIGHT, ARCHER and GIANT. Team possibilities are ANY, NEUTRAL, ALLY or ENEMY.

int countUnit(t_unit *all, int size, int target, int team)
{
	int count;
	int i;

	count = 0;

	for (i = 0; i < size; i++)
	{
		if (all[i].type == target && (all[i].owner == team || team == -2))
		{
			count += 1;
		}
	}

	return (count);
}

// Return the count of OBJECT from our t_site array with specified team.
// Object possibilities are MINE, TOWER or BARRACK. Team possibilities are ANY, NEUTRAL, ALLY or ENEMY.

int countObject(t_site *all, int size, int target, int team)
{
	int count;
	int i;

	count = 0;

	for (i = 0; i < size; i++)
	{
		if (all[i].type == target && (all[i].owner == team || team == -2))
		{
			count += 1;
		}
	}

	return (count);
}

int findNearestUnit(t_pos from, t_unit *all, int size, int target, int team)
{
	int i;
	int dist;
	int maxDist, maxIndex;

	maxIndex = -1;
	maxDist = 2000000000;

	for (i = 0; i < size; i++)
	{
		if (all[i].type == target && (all[i].owner == team || team == -2))
		{
			dist = getDistance(from, all[i].pos);
			if (dist < maxDist)
			{
				maxDist = dist;
				maxIndex = i;
			}
		}
	}

	return (maxIndex);
}

int estimateTimeBeforeCollision(t_pos from, t_pos to, int speed, int range, int radius1, int radius2)
{
	int dist = getDistance(from, to) - radius1 - radius2 - range;
	return (dist / speed);
}
