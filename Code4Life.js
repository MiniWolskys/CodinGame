
/**
 * Bring data on patient samples from the diagnosis machine to the laboratory with enough molecules to produce medicine!
 **/

function sortByPriority(a, b)
{
	return (b.priority - a.priority);
}

var Sample = function (){

	this.id = -1;
	this.carriedBy = -1;
	this.rank = -1;
	this.expertiseGain = -1;
	this.health = -1;
	this.costA = -1;
	this.costB = -1;
	this.costC = -1;
	this.costD = -1;
	this.costE = -1;

	this.priority = -1;
	this.still = false;

	this.renew = function (a, b, c, d, e, f, g, h, i, j)
	{
		this.id = a;
		this.carriedBy = b;
		this.rank = c;
		this.expertiseGain = d;
		this.health = e;
		this.costA = f;
		this.costB = g;
		this.costC = h;
		this.costD = i;
		this.costE = j;

		this.still = true;

		if (this.carriedBy != -1 && playerList[this.carriedBy].holdList.indexOf(this.id) == -1)
		{
			playerList[this.carriedBy].holdList.push(this);
			if (this.rank == 3)
				playerList[this.carriedBy].haveRank3 += 1;
			if (playerList[this.carriedBy].holdList.length > 3)
				print("FATAL : Player " + this.carriedBy + " is holding more than 3 samples.");
		}
	}
}

var Project = function () {
	this.costA = -1;
	this.costB = -1;
	this.costC = -1;
	this.costD = -1;
	this.costE = -1;

	this.renew = function (a, b, c, d, e)
	{
		this.costA = a;
		this.costB = b;
		this.costC = c;
		this.costD = d;
		this.costE = e;
	}
}

var Player = function () {

	this.position = -1;
	this.eta = -1;
	this.score = -1;
	this.storageA = -1;
	this.storageB = -1;
	this.storageC = -1;
	this.storageD = -1;
	this.storageE = -1;
	this.expertiseA = -1;
	this.expertiseB = -1;
	this.expertiseC = -1;
	this.expertiseD = -1;
	this.expertiseE = -1;

	this.haveRank3 = 0;

	this.holdList = [];

	this.renew = function (a, b, c, d, e, f, g, h, i, j, k, l, m) {
		this.position = a;
		this.eta = b;
		this.score = c;
		this.storageA = d;
		this.storageB = e;
		this.storageC = f;
		this.storageD = g;
		this.storageE = h;
		this.expertiseA = i;
		this.expertiseB = j;
		this.expertiseC = k;
		this.expertiseD = l;
		this.expertiseE = m;

		this.haveRank3 = 0;

		this.holdList = [];
	}
}

var sampleList = [];
var playerList = [];
var projectList = [];
playerList[0] = new Player();
playerList[1] = new Player();
var turn = 0;

var projectCount = parseInt(readline());
for (var i = 0; i < projectCount; i++) {
    var inputs = readline().split(' ');
	projectList[i] = new Project ();
	projectList[i].renew(parseInt(inputs[0]), parseInt(inputs[1]), parseInt(inputs[2]), parseInt(inputs[3]), parseInt(inputs[4]))
}

// game loop
while (true) {
	// Compte le nombre de tour
	turn++;

	sampleList = [];

	// Recupere les inputs
    for (var i = 0; i < 2; i++) {
        var inputs = readline().split(' ');
		playerList[i].renew(inputs[0], parseInt(inputs[1]), parseInt(inputs[2]), parseInt(inputs[3]), parseInt(inputs[4]), parseInt(inputs[5]), parseInt(inputs[6]), parseInt(inputs[7]), parseInt(inputs[8]), parseInt(inputs[9]), parseInt(inputs[10]), parseInt(inputs[11]), parseInt(inputs[12]));
    }
    var inputs = readline().split(' ');
    var availableA = parseInt(inputs[0]);
    var availableB = parseInt(inputs[1]);
    var availableC = parseInt(inputs[2]);
    var availableD = parseInt(inputs[3]);
    var availableE = parseInt(inputs[4]);
    var sampleCount = parseInt(readline());
    for (var i = 0; i < sampleCount; i++) {
        var inputs = readline().split(' ');
        var sampleId = parseInt(inputs[0]);
		sampleList[sampleId] = new Sample ();
        sampleList[sampleId].renew(sampleId, parseInt(inputs[1]), parseInt(inputs[2]), inputs[3], parseInt(inputs[4]), parseInt(inputs[5]), parseInt(inputs[6]), parseInt(inputs[7]), parseInt(inputs[8]), parseInt(inputs[9]));
	}

	// Classe les samples par ordre de priorité, selon plusieurs criteres : Le ratio vie / tour et la possibilité de les créer.
	for (i in sampleList)
	{
		if (sampleList[i].still == true)
		{
			var neededA = sampleList[i].costA - playerList[0].storageA - playerList[0].expertiseA < 0 ? 0 : sampleList[i].costA - playerList[0].storageA - playerList[0].expertiseA;
			var neededB = sampleList[i].costB - playerList[0].storageB - playerList[0].expertiseB < 0 ? 0 : sampleList[i].costB - playerList[0].storageB - playerList[0].expertiseB;
			var neededC = sampleList[i].costC - playerList[0].storageC - playerList[0].expertiseC < 0 ? 0 : sampleList[i].costC - playerList[0].storageC - playerList[0].expertiseC;
			var neededD = sampleList[i].costD - playerList[0].storageD - playerList[0].expertiseD < 0 ? 0 : sampleList[i].costD - playerList[0].storageD - playerList[0].expertiseD;
			var neededE = sampleList[i].costE - playerList[0].storageE - playerList[0].expertiseE < 0 ? 0 : sampleList[i].costE - playerList[0].storageE - playerList[0].expertiseE;

			sampleList[i].priority = sampleList[i].health / (neededA + neededB + neededC + neededD + neededE);

			var already = false;

			for (j in projectList)
			{
				if (sampleList[i].expertiseGain == "A" && projectList[j].costA > playerList[0].expertiseA && !already)
				{
					already = true;
					sampleList[i].priority *= 2;
				}
				if (sampleList[i].expertiseGain == "B" && projectList[j].costB > playerList[0].expertiseB && !already)
				{
					already = true;
					sampleList[i].priority *= 2;
				}
				if (sampleList[i].expertiseGain == "C" && projectList[j].costC > playerList[0].expertiseC && !already)
				{
					already = true;
					sampleList[i].priority *= 2;
				}
				if (sampleList[i].expertiseGain == "D" && projectList[j].costD > playerList[0].expertiseD && !already)
				{
					already = true;
					sampleList[i].priority *= 2;
				}
				if (sampleList[i].expertiseGain == "E" && projectList[j].costE > playerList[0].expertiseE && !already)
				{
					already = true;
					sampleList[i].priority *= 2;
				}
			}

			if (sampleList[i].costA > availableA + playerList[0].storageA + playerList[0].expertiseA || sampleList[i].costB > availableB + playerList[0].storageB + playerList[0].expertiseB || sampleList[i].costC > availableC + playerList[0].storageC + playerList[0].expertiseC || sampleList[i].costD > availableD + playerList[0].storageD + playerList[0].expertiseD || sampleList[i].costE > availableE + playerList[0].storageE + playerList[0].expertiseE)
				sampleList[i].priority = 0;
			if (sampleList[i].carriedBy == 1)
				sampleList[i].priority = 0;
		}
	}
	sampleList.sort(sortByPriority);
	playerList[0].holdList.sort(sortByPriority);

	var answer = "GOTO SAMPLES";

	if (playerList[0].eta == 0 && playerList[0].position == "SAMPLES")
	{
		if (playerList[0].holdList.length == 3)
		{
			answer = "GOTO DIAGNOSIS";
		}
		else
		{
			if (playerList[0].expertiseA + playerList[0].expertiseB + playerList[0].expertiseC + playerList[0].expertiseD + playerList[0].expertiseE > 12 && playerList[0].haveRank3 < 2)
				answer = "CONNECT 3"
			else if (playerList[0].expertiseA + playerList[0].expertiseB + playerList[0].expertiseC + playerList[0].expertiseD + playerList[0].expertiseE > 15)
				answer = "CONNECT 3"
			else if (playerList[0].expertiseA + playerList[0].expertiseB + playerList[0].expertiseC + playerList[0].expertiseD + playerList[0].expertiseE > 9)
				answer = "CONNECT 2";
			else
				answer = "CONNECT 1";
		}
	}
	else if (playerList[0].eta == 0 && playerList[0].position == "DIAGNOSIS")
	{
		var find = false;
		for (i in playerList[0].holdList)
		{
			if (playerList[0].holdList[i].health == -1)
			{
				find = true;
				answer = "CONNECT " + playerList[0].holdList[i].id;
			}
		}

		if (!find)
		{
			find = false;
			for (i in playerList[0].holdList)
			{
				if (playerList[0].holdList[i].priority <= 0)
				{
					find = true;
					answer = "CONNECT " + playerList[0].holdList[i].id;
				}
			}

			if (!find)
			{
				answer = "GOTO MOLECULES";
				if (playerList[0].holdList.length < 2)
					answer = "GOTO SAMPLES";
			}
		}
	}
	else if (playerList[0].eta == 0 && playerList[0].position == "MOLECULES")
	{
		var neededA = 0;
		var neededB = 0;
		var neededC = 0;
		var neededD = 0;;
		var neededE = 0;

		playerList[0].holdList.reverse();

		answer = "GOTO LABORATORY";

		for (i in playerList[0].holdList)
		{
			var neededA = playerList[0].holdList[i].costA - (playerList[0].storageA + playerList[0].expertiseA);
			var neededB = playerList[0].holdList[i].costB - (playerList[0].storageB + playerList[0].expertiseB);
			var neededC = playerList[0].holdList[i].costC - (playerList[0].storageC + playerList[0].expertiseC);
			var neededD = playerList[0].holdList[i].costD - (playerList[0].storageD + playerList[0].expertiseD);
			var neededE = playerList[0].holdList[i].costE - (playerList[0].storageE + playerList[0].expertiseE);
			if (neededA <= availableA && neededB <= availableB && neededC <= availableC && neededD <= availableD && neededE <= availableE)
			{
				if (playerList[0].storageA + playerList[0].storageB + playerList[0].storageC + playerList[0].storageD + playerList[0].storageE >= 10)
					answer = "GOTO LABORATORY";
				else if (neededA > 0 && availableA > 0)
					answer = "CONNECT A";
				else if (neededB > 0 && availableB > 0)
					answer = "CONNECT B";
				else if (neededC > 0 && availableC > 0)
					answer = "CONNECT C";
				else if (neededD > 0 && availableD > 0)
					answer = "CONNECT D";
				else if (neededE > 0 && availableE > 0)
					answer = "CONNECT E";
			}
		}

		for (i in playerList[1].holdList)
		{

		}
	}
	else if (playerList[0].eta == 0 && playerList[0].position == "LABORATORY")
	{
		if (playerList[0].holdList.length > 0 && playerList[0].holdList[0].priority > 0)
			answer = "GOTO MOLECULES";
		for (i in playerList[0].holdList)
		{
			if (playerList[0].holdList[i].costA <= playerList[0].storageA + playerList[0].expertiseA && playerList[0].holdList[i].costB <= playerList[0].storageB + playerList[0].expertiseB && playerList[0].holdList[i].costC <= playerList[0].storageC + playerList[0].expertiseC && playerList[0].holdList[i].costD <= playerList[0].storageD + playerList[0].expertiseD && playerList[0].holdList[i].costE <= playerList[0].storageE + playerList[0].expertiseE)
			{
				answer = "CONNECT " + playerList[0].holdList[i].id;
			}
		}
	}

	if (playerList[0].eta > 0)
	{
		answer = "Oblivion Awaits !";
	}

	// Modifie la valeur still de chaque sample pour savoir si il est encore en jeu. Lors du prochain tour, tous les samples encore présent seront renew et verront leur valeur revenir a true.
	// Derniere chose a faire avant la fin du tour et l'affichage de la réponse.
	for (i in sampleList)
	{
		sampleList[i].still = false;
	}

    // Write an action using print()
    // To debug: printErr('Debug messages...');

    print(answer);
}
