<?php
/**
 * Save humans, destroy zombies!
 **/

/**
 * This file is for the project "Code vs Zombies" on Codingame (https://www.codingame.com/multiplayer/optimization/code-vs-zombies)
**/

class		pos {
	public	$x;
	public	$y;
}

$player = new pos ();

function	get_distance($pos1, $pos2) {

	$dist1 = ($pos1->x - $pos2->x) * ($pos1->x - $pos2->x);
	$dist2 = ($pos1->y - $pos2->y) * ($pos1->y - $pos2->y);

	$dist = sqrt($dist1 + $dist2);

	return ($dist);
}

function	can_reach($human, $distz, $player) {
	$distp = get_distance($human, $player);

	if ($distz / 400 > $distp / 1000 + 2000)
		return (false);
	return (true);
}

function	find_closer($humans, $zombies, $player) {
	$minAll = 99999;
	$dest = new pos ();
	$dest->x = 8000;
	$dest->y = 4500;
	foreach ($humans as $idh => $human) {
		$min = 99999;
		foreach ($zombies as $idz => $zombie) {
			$dist = get_distance($human, $zombie);
			if ($dist <= $min) {
				$min = $dist;
			}
		}
		if (can_reach($human, $min, $player) && $minAll > $min) {
			$minAll = $min;
			$dest = $human;
		error_log ("I'm watching over $idh.");
		if (can_reach($human, $min, $player)) {
			error_log ("I think I can reach him in time");
			}
		}
	}
	return ($dest);
}

// game loop
while (TRUE)
{
	unset($humans);
	unset($zombies);
	$humans = [];
	$zombies = [];
    fscanf(STDIN, "%d %d",
        $player->x,
        $player->y
    );
    fscanf(STDIN, "%d",
        $humanCount
    );
    for ($i = 0; $i < $humanCount; $i++)
    {
        fscanf(STDIN, "%d %d %d",
            $humanId,
            $x,
            $y
        );
		$humans[$humanId] = new pos ();
		$humans[$humanId]->x = $x;
		$humans[$humanId]->y = $y;
    }
    fscanf(STDIN, "%d",
        $zombieCount
    );
    for ($i = 0; $i < $zombieCount; $i++)
    {
        fscanf(STDIN, "%d %d %d %d %d",
            $zombieId,
            $zombieX,
            $zombieY,
            $nextX,
            $nextY
        );
		$zombies[$zombieId] = new pos ();
		$zombies[$zombieId]->x = $nextX;
		$zombies[$zombieId]->y = $nextY;
    }

	if (count($humans) != $humanCount) {
		error_log ("Human count is wrong");
		error_log (var_export(count($humans), true));
		error_log (var_export($humanCount, true));
	}

	if (count($zombies) != $zombieCount) {
		error_log ("Zombie count is wrong");
		error_log (var_export(count($zombies), true));
		error_log (var_export($zombieCount, true));
	}

	$dest = find_closer($humans, $zombies, $player);

    // Write an action using echo(). DON'T FORGET THE TRAILING \n
    // To debug (equivalent to var_dump): error_log(var_export($var, true));

    echo("$dest->x $dest->y\n"); // Your destination coordinates
}
?>
