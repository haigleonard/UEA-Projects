#include "Model/Survivor.h"

const double convertKG = 0.453592;

SimState *Info_S;

Survivor::Survivor() {}

Survivor::~Survivor() {}

Survivor::Survivor(SimulationInfo *info) {
	//Setting miscalenous
	Survivor::timer = 0;
	Survivor::shelterTask = false;
	Survivor::preyStock = 0;
	Survivor::woodTask = false;
	Survivor::preyTask = false;
	Survivor::dayEnded = true;
	Survivor::calorieIn = 0.0;
	Survivor::caloriesMul = 0.0;
	//Setting dehydration
	Survivor::waterLevel = 0;
	Survivor::dehydration = 0;
	Survivor::curr_dehydration = 0;
	Survivor::task = 0;
	//Setting wood count
	Survivor::wood = 0;
	//Setting energy
	Survivor::energy = 100;
	//Setting survivor's position
	std::vector<int> validPositions = info->getMap()->findValidPos();
	this->setlocationX(validPositions[0]);
	this->setlocationY(validPositions[1]);
	//Setting survivor's speed
	Survivor::speed = 0.5;
	//Setting metrics	
	Survivor::weight = 78.6;
	Survivor::initialWeight = 78.6;
	Survivor::height = 178;
	Survivor::age = 37;
	Survivor::bmr = calculateBMR();
	//Water info
	info->getWater()->setFoundWater(false);
	info->getWater()->setUWater(0);
	info->getWater()->setFWater(0);
	//Creating FSM and setting the initial state
	survivorFSM = new FSM;
	Info_S = info;
	this->survivorFSM->setState(&adapter, this, info, this->wander);
	currentState = decision;
}


//These functions will dictate how and what happens with the FSM via funciton pointers
void Survivor::Wander(SimulationInfo * info)
{
	//This function should call the pathfinder and randomly move around until a resource or a predator triggers its collision detection.
	//Depending on the trigger then we should either change to run or collect mode.
	//While wandering we should possibly deplete energy levels and hunger levels.
	task = 1;
	//Dehydration unit
	this->dehydration = this->dehydration + 0.55;
	info->setCalouriesUsed(info->getCaloriesUsed() + this->bmrIncrease);// + this->bmr * this->caloriesMul));
																		//std::cout << (info->getCaloriesUsed() + this->bmrIncrease + (this->bmr * this->caloriesMul / 1050)) << std::endl;


																		//Looking for water has a bigger "hit detection"
	for (int x = -3; x < 4; x++) {
		for (int y = -3; y < 4; y++) {

			bool check = (this->xPos + x >= 0 && this->yPos + y >= 0) && (this->xPos + x < info->getMap()->getNumRows() && this->yPos + y < info->getMap()->getNumCols());

			if (check) {
				int currX = this->xPos + x;

				int currY = this->yPos + y;

				if (info->getMap()->getNode(currX, currY)->getValue() == 0) {
					if (!info->getWater()->checkFoundWater()) {

						//Finding the closest valid node to collect water
						for (int i = -1; i < 2; i++) {
							for (int j = -1; j < 2; j++) {
								if (info->getMap()->getNode(currX + i, currY + j)->getValue() == 1) {

									//Clearing current path
									this->optimal_path.clear();
									info->getMap()->clear();
									info->setNumMoves(1);
									info->setMove(false);

									currX = currX + i;
									currY = currY + j;

									//Going to water
									this->pathToWater = this->finder.path(info->getMap()->getNode(this->xPos, this->yPos),
										info->getMap()->getNode(currX, currY), info->getMap());

									//std::cout << "Found water at distance..." << std::endl;
									this->sTask = "Found water at distance...";

									//Setting water parameters
									info->getWater()->setFoundWater(true);
									info->getWater()->setX(currX);
									info->getWater()->setY(currY);

									goto endloop; //feel like this is a hack but it works!
								}
							}
						}

					endloop:

						currentState = go_water; //going to water

						return;

					}
				}

			}
		}
	}


	//Collision detection sort of a thing 
	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {

			bool check = (this->xPos + x >= 0 && this->yPos + y >= 0) && (this->xPos + x < info->getMap()->getNumRows() && this->yPos + y < info->getMap()->getNumCols());

			if (check) {
				int currX = this->xPos + x;

				int currY = this->yPos + y;

				if ((this->xPos - currX > 0 && this->yPos - currY == 0) ||
					(this->xPos - currX < 0 && this->yPos - currY == 0) ||
					(this->xPos - currX == 0 && this->yPos - currY > 0) ||
					(this->xPos - currX == 0 && this->yPos - currY < 0)) {

					if (info->getMap()->getNode(currX, currY)->getValue() == 3 && this->preyTask) {

						//std::cout << "prey found" << std::endl;

						info->getMap()->getNode(currX, currY)->setValue(1);

						int len = info->getPrey()->size();
						for (int i = 0; i < len; i++) {
							Prey p = info->getPrey()->at(i);
							if (p.getX() == currX && p.getY() == currY) {
								info->setPreyCount(info->getPreyCount() - 1);
								info->getPrey()->erase(info->getPrey()->begin() + i);
								break;
							}
						}

						this->preyStock = this->preyStock + 1;

						this->caloriesMul = this->caloriesMul + 0.001;

						//std::cout << "prey killed" << std::endl;

						currentState = decision;

						return;
					}

					if (info->getMap()->getNode(currX, currY)->getValue() == -2 && this->wood < 10 && this->woodTask) {

						this->setlocationX(currX);
						this->setlocationY(currY);

						//std::cout << "Wood found" << std::endl;
						this->sTask = "Wood found.";

						currentState = collect_wood;

						return;

					}
				}
			}
		}
	}


	if (info->getMove() && !info->getPause()) {



		int distHome = 0;

		if (info->getShelter()->getCamp()) {
			distHome = finder.manhattan_distance(this->xPos, this->yPos, info->getShelter()->getX(), info->getShelter()->getY());
		}

		if ((this->energy < distHome * 2 || !info->checkNightBool()) && info->getShelter()->getCamp()) {
			this->optimal_path.clear();
			info->getMap()->clear();
			info->setNumMoves(1);
			this->pathToHome = this->finder.path(info->getMap()->getNode(this->xPos, this->yPos),
				info->getMap()->getNode(info->getShelter()->getX(), info->getShelter()->getY()), info->getMap());

			//std::cout << this->energy << std::endl;

			currentState = returning_home;

			return; //go straight to returning home

		}
		else if (energy < 3) {

			currentState = rest; //case where he rests in day light

			return; //go straight to rest

		}
		else if (!info->checkNightBool()) {

			currentState = rest;

			return;

		}
		else { //okay to move

			int pathX = this->optimal_path[this->optimal_path.size() - info->getNumMoves()].getX();
			int pathY = this->optimal_path[this->optimal_path.size() - info->getNumMoves()].getY();
			if (pathX > this->xPos) { this->setlocationX(this->xPos + this->speed); }
			if (pathX < this->xPos) { this->setlocationX(this->xPos - this->speed); }
			if (pathY > this->yPos) { this->setlocationY(this->yPos + this->speed); }
			if (pathY < this->yPos) { this->setlocationY(this->yPos - this->speed); }
			info->getMap()->getNode(this->xPos, this->yPos)->heatTile();

			float pathXT = pathX;
			float pathYT = pathY;

			if (this->xPos < (pathXT + speed) && this->xPos >(pathXT - speed) &&
				this->yPos < (pathYT + speed) && this->yPos >(pathYT - speed))

			{
				info->setNumMoves(info->getNumMoves() + 1);
				this->energy = this->energy - 2; // each move depletes 2 energy units.
				this->caloriesMul = this->caloriesMul + 0.0007;
				this->setlocationX(pathX);
				this->setlocationY(pathY);
			}
			if (info->getNumMoves() == this->optimal_path.size() + 1)
			{
				info->setNumMoves(1);
				info->setMove(false);
			}
		}
	}
	else {

		this->startNode = Node(this->xPos, this->yPos);
		//std::vector<int> goalValid = info->getMap()->findValidPos();


		bool validGoal = false;
		int range = 5;
		int xLowerBound = this->xPos - range;
		int yLowerBound = this->yPos - range;
		int xUpperBound = this->xPos + range;
		int yUpperBound = this->yPos + range;
		int xG;
		int yG;

		while (!validGoal) {
			xG = xLowerBound + (std::rand() % (xUpperBound - xLowerBound + 1));
			yG = yLowerBound + (std::rand() % (yUpperBound - yLowerBound + 1));
			bool check = xG >= 0 && yG >= 0 && xG < info->getMap()->getNumRows() && yG < info->getMap()->getNumCols();
			if (check) {
				if (info->getMap()->getNode(xG, yG)->getValue() == 1 && this->xPos != xG && this->yPos != yG) {
					this->goalNode = Node(xG, yG);
					validGoal = true;
				}
			}
		}

		info->getMap()->clear();
		this->optimal_path = this->finder.path(this->startNode, this->goalNode, info->getMap());
		if (this->optimal_path.size() > 0) {
			info->setMove(true);
		}

	}


}

void Survivor::Run(SimulationInfo * info)
{
	//This function should be triggered when a predator is within close proximity of the survivor
	//PathFinder should possibly call for a point in direct opposition of the predator
	//Once completed the pathfinder is complete then check again for the predator.
	//Quickly depletes energy levels.
	//Should change back to wander once safe. Or continue in run if not safe.

}

void Survivor::Rest(SimulationInfo * info)
{
	//This function should be triggered when energy levels are low.
	//Remain idle until energy levels are a good level.
	//Should move to back to wander if both energy levels are high and resource levels are low.
	//Different levels of resting depending on shelter and fire being made.
	task = 2;
	this->dehydration = this->dehydration + 0.25;

	info->setCalouriesUsed(info->getCaloriesUsed() + this->bmrIncrease);// + this->bmr * this->caloriesMul));
																										//std::cout << (info->getCaloriesUsed() + this->bmrIncrease + (this->bmr * this->caloriesMul / 1050)) << std::endl;

	int a = floor(info->getHours());
	int b = floor(info->getMinutes());

	if (a == 23 && (b >= 57 && b <= 59)) {
		currentState = check_metrics;
		return;
	}

	if (!info->checkNightBool()) { //fire and shelter		
								   //Since fire is only made at night time and make_fire returns to rest this state is only ever called at night time.

		if (info->getShelter()->getCamp() && info->getFire()->checkFireMade()) {
			info->getFire()->setFireLevel(info->getFire()->getFireLevel() - 1);
			if (info->getFire()->getFireLevel() == 0) {
				info->getMap()->getNode(info->getFire()->getX(), info->getFire()->getY())->setValue(1);
				info->getFire()->setFireMade(false);
			}
			if (this->energy <= 100) {
				this->energy = this->energy + 1; //best case scenario for the survivor.
			}
		}
		else if (info->getShelter()->getCamp() && !(info->getFire()->checkFireMade())) { // shelter only						
			if (this->wood > 0) {
				this->MakeFire(info); //should hit the condition above after returning from make_fire
				return;
			}
			else {
				if (this->energy <= 100) {
					this->energy = this->energy + 0.75;
				}
			}
		}
		else if (!(info->getShelter()->getCamp()) && info->getFire()->checkFireMade()) { // fire only				
			info->getFire()->setFireLevel(info->getFire()->getFireLevel() - 1);
			if (info->getFire()->getFireLevel() == 0) {
				info->getMap()->getNode(info->getFire()->getX(), info->getFire()->getY())->setValue(1);
				info->getFire()->setFireMade(false);
			}
			if (this->energy <= 100) {
				this->energy = this->energy + 0.5;
			}
		}
		else {
			if (this->wood > 0) {
				this->MakeFire(info); //make fire for the night
			}
			// gain nothing. worst case scenario for the survivor
		}

	}
	else {
		if (this->energy <= 100) {
			this->energy = this->energy + 1;
		}
		else { //good to go
			if (info->getFire()->checkFireMade()) {
				//Once he leaves camp put fire out.
				info->getFire()->setFireLevel(0);
				info->getFire()->setFireMade(false);
				info->getMap()->getNode(info->getFire()->getX(), info->getFire()->getY())->setValue(1);

				currentState = decision;
			}
			else {
				currentState = decision;
			}
		}
	}

}

void Survivor::CollectWood(SimulationInfo * info)
{
	//This function should be triggered when resources are within close proximity of the survivor.
	//Should move back to wander if not all resources are found.
	//If all resources are found. Call pathfinder to the camp and move to the Rest state. Depleting resouces and increasing energy.
	task = 3;
	this->wood = this->wood + 1;
	info->getMap()->getNode(this->xPos, this->yPos)->setValue(1);

	this->caloriesMul = this->caloriesMul + 0.00075;

	std::cout << this->wood << std::endl;

	currentState = decision;

}

void Survivor::CollectWater(SimulationInfo * info)
{
	//State to collect water, once water is found. Water in this simulation is measured in 
	//centilitres. Therefore, 200 amounts to 2 litres of water to be carried.
	//Water levels are depleted "realistically".
	task = 4;
	//std::cout << "Water collected" << std::endl;
	this->sTask = "Wood collected.";

	info->getWater()->setUWater(info->getWater()->getUWater() + 200);

	this->caloriesMul = this->caloriesMul + 0.00075;

	currentState = decision;

}

void Survivor::MakeShelter(SimulationInfo * info)
{
	task = 5;
	//std::cout << "Making shelter..." << std::endl;
	this->sTask = "Making shelter...";

	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			int currX = info->getWater()->getX() + x;
			int currY = info->getWater()->getY() + y;

			bool check = (currX >= 0 && currY >= 0) && (currX < info->getMap()->getNumRows() && currY < info->getMap()->getNumCols());

			if (check && info->getMap()->getNode(currX, currY)->getValue() == 1 && currX != info->getWater()->getX() && currY != info->getWater()->getY()) {
				this->wood = this->wood - 5; //cost of making shelter.
				info->getShelter()->setX(currX);
				info->getShelter()->setY(currY);
				info->getMap()->getNode(currX, currY)->setValue(-4);
				info->getShelter()->setMadeCamp(true);
				goto endloop;
			}

		}
	}

endloop:

	this->shelterTask = false;

	currentState = decision;

	//std::cout << "Shelter made." << std::endl;

	return;

}

void Survivor::MakeFire(SimulationInfo * info)
{
	task = 6;
	this->wood = this->wood - 1; //cost of making fire.

	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			if (info->getMap()->getNode(this->xPos + x, this->yPos + y)->getValue() == 1 && xPos != xPos + x && yPos != yPos + y) {
				info->getFire()->setX(this->xPos + x);
				info->getFire()->setY(this->yPos + y);
				info->getMap()->getNode(this->xPos + x, this->yPos + y)->setValue(-3);
				info->getFire()->setFireMade(true);
				info->getFire()->setFireLevel(100);
				goto foundValid;
			}
		}
	}

foundValid:

	return;

}

void Survivor::ReturningHome(SimulationInfo * info)
{
	task = 7;
	this->dehydration = this->dehydration + 0.55;

	info->setCalouriesUsed(info->getCaloriesUsed() + this->bmrIncrease);// + this->bmr * this->caloriesMul));
																		//std::cout << (info->getCaloriesUsed() + this->bmrIncrease + (this->bmr * this->caloriesMul / 1050)) << std::endl;

																		//Collision detection sort of a thing 
	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {

			bool check = (this->xPos + x >= 0 && this->yPos + y >= 0) && (this->xPos + x < info->getMap()->getNumRows() && this->yPos + y < info->getMap()->getNumCols());

			if (check) {
				int currX = this->xPos + x;

				int currY = this->yPos + y;

				if ((this->xPos - currX > 0 && this->yPos - currY == 0) ||
					(this->xPos - currX < 0 && this->yPos - currY == 0) ||
					(this->xPos - currX == 0 && this->yPos - currY > 0) ||
					(this->xPos - currX == 0 && this->yPos - currY < 0)) {


					if (info->getMap()->getNode(currX, currY)->getValue() == -2 && this->wood < 10 && this->woodTask) {

						this->setlocationX(currX);
						this->setlocationY(currY);

						//std::cout << "Wood found" << std::endl;
						this->sTask = "Wood found.";

						currentState = collect_wood;

						return;

					}

					if (info->getMap()->getNode(currX, currY)->getValue() == 3 && this->preyTask) {

						//std::cout << "prey found" << std::endl;

						info->getMap()->getNode(currX, currY)->setValue(1);

						int len = info->getPrey()->size();
						for (int i = 0; i < len; i++) {
							Prey p = info->getPrey()->at(i);
							if (p.getX() == currX && p.getY() == currY) {
								info->setPreyCount(info->getPreyCount() - 1);
								info->getPrey()->erase(info->getPrey()->begin() + i);
								break;
							}
						}

						this->preyStock = this->preyStock + 1;

						this->caloriesMul = this->caloriesMul + 0.001;

						//std::cout << "prey killed" << std::endl;

						currentState = decision;

						return;

					}

				}
			}
		}
	}

	int pathX = this->pathToHome[this->pathToHome.size() - info->getNumMoves()].getX();
	int pathY = this->pathToHome[this->pathToHome.size() - info->getNumMoves()].getY();

	if (pathX > this->xPos) { this->setlocationX(this->xPos + this->speed); }
	if (pathX < this->xPos) { this->setlocationX(this->xPos - this->speed); }
	if (pathY > this->yPos) { this->setlocationY(this->yPos + this->speed); }
	if (pathY < this->yPos) { this->setlocationY(this->yPos - this->speed); }
	info->getMap()->getNode(this->xPos, this->yPos)->heatTile();

	float pathXT = pathX;
	float pathYT = pathY;

	if (this->xPos < (pathXT + speed) && this->xPos >(pathXT - speed) &&
		this->yPos < (pathYT + speed) && this->yPos >(pathYT - speed))

	{
		info->setNumMoves(info->getNumMoves() + 1);
		this->energy = this->energy - 2; // each move depletes 2 energy units.
		this->caloriesMul = this->caloriesMul + 0.0007;
		this->setlocationX(pathX);
		this->setlocationY(pathY);
	}
	if (info->getNumMoves() == this->pathToHome.size() + 1)
	{
		info->setNumMoves(1);
		info->setMove(false);
		currentState = rest;
	}
}

void Survivor::Decision(SimulationInfo * info)
{
	task = 8;
	if (dayEnded) {
		dayEnded = false;
		this->bmr = calculateBMR();
		this->bmrIncrease = this->bmr / 1100; //no clue how this step works		
	}

	//Water is the highest priority so check if water has been found and if water is needed.
	if (info->getWater()->getFWater() > 0) { //drinks water			

		if (this->waterLevel - this->dehydration <= 0) {

			int amountToDrink = this->dehydration - this->curr_dehydration;

			if (info->getWater()->getFWater() > amountToDrink) {
				this->curr_dehydration = this->dehydration;
				this->waterLevel = this->waterLevel + amountToDrink;
				info->getWater()->setFWater(info->getWater()->getFWater() - amountToDrink);
			}
			else {
				this->waterLevel = this->waterLevel + info->getWater()->getFWater();
				info->getWater()->setFWater(0);
			}

			//std::cout << "Water drank" << std::endl;
			this->sTask = "Water drank.";
		}

		//Shelter should be second highest priority.
		if (!info->getShelter()->getCamp()) {

			if (this->wood >= 5) {

				// Clear path as it is about to change to close proximity of the water source.
				this->optimal_path.clear();
				info->getMap()->clear();
				info->setNumMoves(1);
				info->setMove(false);

				//std::cout << "Going close to water source for shelter..." << std::endl;
				this->sTask = "Going close to water source for shelter...";

				this->pathToWater = this->finder.path(info->getMap()->getNode(this->xPos, this->yPos),
					info->getMap()->getNode(info->getWater()->getX(), info->getWater()->getY()), info->getMap());

				this->woodTask = false;
				this->shelterTask = true;

				currentState = go_water;

				return;

			}
			else {

				this->optimal_path.clear();
				info->getMap()->clear();
				info->setNumMoves(1);
				info->setMove(false);

				this->woodTask = true;
				currentState = wander;

				//std::cout << "Need wood for shelter." << std::endl;
				this->sTask = "Need wood for shelter.";

				return;
			}
		}
		else { //shelter made need to prioritise food now since water is attained.			
			if (this->preyStock > 0 && this->wood > 0) {

				this->preyTask = false;
				this->woodTask = false;

				if (this->calorieIn == 0) {
					this->MakeFire(info);

					//std::cout << "Cooking" << std::endl;
					this->sTask = "Cooking.";

					currentState = cooking;

					info->getFire()->setFireLevel(0);
					info->getFire()->setFireMade(false);
					info->getMap()->getNode(info->getFire()->getX(), info->getFire()->getY())->setValue(1);

					return;
				}
				else {
					if (this->xPos == info->getShelter()->getX() && this->yPos == info->getShelter()->getY()) {
						int a = floor(info->getHours());

						if (a >= 16 && a < 22 && this->wood == 0) { //needs to get wood for night time

							this->optimal_path.clear();
							info->getMap()->clear();
							info->setNumMoves(1);
							info->setMove(false);

							this->woodTask = true;
							currentState = wander;

							//std::cout << "Getting dark, need wood..." << std::endl;
							this->sTask = "Getting dark, need wood...";

							return;

						}
						else {
							currentState = rest;
							std::cout << "Already eaten, nothing else to do." << std::endl;
							this->sTask = "Already eaten, nothing else to do.";
							return;
						}
					}
					else {
						this->optimal_path.clear();
						info->getMap()->clear();
						info->setNumMoves(1);
						this->pathToHome = this->finder.path(info->getMap()->getNode(this->xPos, this->yPos),
							info->getMap()->getNode(info->getShelter()->getX(), info->getShelter()->getY()), info->getMap());

						std::cout << this->energy << std::endl;

						currentState = returning_home;

						return; //go straight to returning home
					}
				}


			}
			else if (this->preyStock > 0 && wood == 0) {
				//Just to make sure that wander is really clear before a move is attempted.

				this->optimal_path.clear();
				info->getMap()->clear();
				info->setNumMoves(1);
				info->setMove(false);

				this->preyTask = false;
				this->woodTask = true;

				currentState = wander;

				//std::cout << "Need wood for food." << std::endl;
				this->sTask = "Need wood for food.";

				return;

			}
			else {
				//Just to make sure that wander is really clear before a move is attempted.
				this->optimal_path.clear();
				info->getMap()->clear();
				info->setNumMoves(1);
				info->setMove(false);

				this->preyTask = true;
				currentState = wander;
				//std::cout << "Hunting..." << std::endl;
				this->sTask = "Hunting";

				return;
			}
		}

	}
	else if (info->getWater()->getUWater() > 0) { //filters water

		if (this->wood > 0) { //current wood is enough to filter

			this->MakeFire(info);

			//std::cout << "Filtering..." << std::endl;
			this->sTask = "Filtering...";

			currentState = filter_water;

			info->getFire()->setFireLevel(0);
			info->getFire()->setFireMade(false);
			info->getMap()->getNode(info->getFire()->getX(), info->getFire()->getY())->setValue(1);

			woodTask = false; //no longer need to look for wood as water is about to be filtered.

		}
		else if (this->woodStock > 0) { //wood stock at the shelter is enough to filter
										//need to go to shelter and deplete woodStock to filter water.
										//check if the survivor is at the shelter then decide if the survivor needs to return home.
		}
		else { // no wood for filtering
			   //Just to make sure that wander is really clear before a move is attempted.
			this->optimal_path.clear();
			info->getMap()->clear();
			info->setNumMoves(1);
			info->setMove(false);

			woodTask = true;
			currentState = wander;

			//std::cout << "Need wood for filtering." << std::endl;
			this->sTask = "Need wood for filtering.";

			return;
		}

		return;

	}
	else {
		if (info->getWater()->getUWater() == 0 && info->getWater()->checkFoundWater()) {

			this->optimal_path.clear();
			info->getMap()->clear();
			info->setNumMoves(1);
			info->setMove(false);
			this->pathToWater = this->finder.path(info->getMap()->getNode(this->xPos, this->yPos),
				info->getMap()->getNode(info->getWater()->getX(), info->getWater()->getY()), info->getMap());

			//std::cout << "Fetching water..." << std::endl;
			this->sTask = "Fetching water...";

			currentState = go_water; //going to water
			return;
		}
		else {

			//std::cout << "Need water." << std::endl;
			this->sTask = "Need water.";
			this->optimal_path.clear();
			info->getMap()->clear();
			info->setNumMoves(1);
			info->setMove(false);

			int a = floor(info->getHours());

			std::cout << a << std::endl;

			if (a >= 12 && a < 22) { //needs to get wood for night time

				this->woodTask = true;

				//std::cout << "Getting dark, need wood..." << std::endl;
				this->sTask = "Getting dark, need wood...";

			}
			else {
				this->woodTask = false;
			}

			currentState = wander;
			return;
		}
	}
}



void Survivor::CheckMetrics(SimulationInfo * info) {
	task = 9;
	//Calculating water consumption
	this->waterLevel = this->waterLevel - this->dehydration;
	//Calculating Basic Metabolic Rate
	this->bmr = calculateBMR();
	//Amount to be added on top of the BMR
	double addCal = this->caloriesMul * this->bmr;
	this->bmr = this->bmr + addCal;
	//Calories spent for the day
	double todayCal = this->calorieIn - this->bmr;
	//Weigth calculation
	double addWeight = (todayCal / 1000) * 0.20;
	this->weight = this->weight + addWeight;

	std::cout << this->bmr << " " << info->getCaloriesUsed() << std::endl;

	//Setting parameters
	this->dehydration = 0;
	this->curr_dehydration = 0;
	this->caloriesMul = 0.0;
	this->calorieIn = 0.0;	
	this->dayEnded = true;
	this->woodTask = false;
	todayCal = 0;
	addWeight = 0;

	currentState = rest;
}


void Survivor::FilterWater(SimulationInfo * info)
{
	task = 10;
	if (timer < 25) {
		this->timer++;
		info->getMap()->getNode(info->getFire()->getX(), info->getFire()->getY())->setValue(-3);
		currentState = filter_water;
	}
	else {
		info->getFire()->setFireLevel(0);
		info->getFire()->setFireMade(false);
		info->getMap()->getNode(info->getFire()->getX(), info->getFire()->getY())->setValue(1);
		info->getWater()->setFWater(info->getWater()->getUWater());
		info->getWater()->setUWater(0);
		this->timer = 0;
		this->caloriesMul = this->caloriesMul + 0.00075;
		currentState = decision;
	}




}

void Survivor::GoWater(SimulationInfo * info)
{
	task = 11;
	this->dehydration = this->dehydration + 0.35;

	info->setCalouriesUsed(info->getCaloriesUsed() + this->bmrIncrease);// + this->bmr * this->caloriesMul));
																		//std::cout << (info->getCaloriesUsed() + this->bmrIncrease + (this->bmr * this->caloriesMul / 1050)) << std::endl;

	int pathX = this->pathToWater[this->pathToWater.size() - info->getNumMoves()].getX();
	int pathY = this->pathToWater[this->pathToWater.size() - info->getNumMoves()].getY();

	if (pathX > this->xPos) { this->setlocationX(this->xPos + this->speed); }
	if (pathX < this->xPos) { this->setlocationX(this->xPos - this->speed); }
	if (pathY > this->yPos) { this->setlocationY(this->yPos + this->speed); }
	if (pathY < this->yPos) { this->setlocationY(this->yPos - this->speed); }
	info->getMap()->getNode(this->xPos, this->yPos)->heatTile();

	float pathXT = pathX;
	float pathYT = pathY;

	if (this->xPos < (pathXT + speed) && this->xPos >(pathXT - speed) &&
		this->yPos < (pathYT + speed) && this->yPos >(pathYT - speed))

	{
		info->setNumMoves(info->getNumMoves() + 1);
		this->energy = this->energy - 2; // each move depletes 2 energy units.
		this->caloriesMul = this->caloriesMul + 0.0007;
		this->setlocationX(pathX);
		this->setlocationY(pathY);
	}
	if (info->getNumMoves() == this->pathToWater.size() + 1)
	{
		info->setNumMoves(1);
		info->setMove(false);

		if (this->shelterTask) {
			this->MakeShelter(info);
		}
		else {
			currentState = collect_water;
		}
	}
}

void Survivor::Cooking(SimulationInfo * info)
{
	task = 12;
	if (timer < 50) {
		this->timer++;
		info->getMap()->getNode(info->getFire()->getX(), info->getFire()->getY())->setValue(-3);
		currentState = cooking;
	}
	else {
		info->getFire()->setFireLevel(0);
		info->getFire()->setFireMade(false);
		info->getMap()->getNode(info->getFire()->getX(), info->getFire()->getY())->setValue(1);
		this->timer = 0;
		this->preyStock = this->preyStock - 1;
		currentState = decision;
		info->setCalouriesUsed(0);
		this->caloriesMul = this->caloriesMul + 0.00075;
		this->calorieIn = this->calorieIn + 2000;
	}
}

void Survivor::adapter(void * context, SimState * info, int state)
{
	switch ((State)state) {
	case wander: static_cast<Survivor *>(context)->Wander((SimulationInfo *)info); break;
	case collect_wood: static_cast<Survivor *>(context)->CollectWood((SimulationInfo *)info); break;
	case run: static_cast<Survivor *>(context)->Run((SimulationInfo *)info); break;
	case rest: static_cast<Survivor *>(context)->Rest((SimulationInfo *)info); break;
	case returning_home: static_cast<Survivor *>(context)->ReturningHome((SimulationInfo *)info); break;
	case collect_water: static_cast<Survivor *>(context)->CollectWater((SimulationInfo *)info); break;
	case check_metrics: static_cast<Survivor *>(context)->CheckMetrics((SimulationInfo *)info); break;
	case filter_water: static_cast<Survivor *>(context)->FilterWater((SimulationInfo *)info); break;
	case go_water: static_cast<Survivor *>(context)->GoWater((SimulationInfo *)info); break;
	case decision: static_cast<Survivor *>(context)->Decision((SimulationInfo *)info); break;
	case cooking: static_cast<Survivor *>(context)->Cooking((SimulationInfo *)info); break;
	}
}

void Survivor::setlocationX(float X)
{
	Survivor::xPos = X;
}

void Survivor::setlocationY(float Y)
{
	Survivor::yPos = Y;
}

float Survivor::getlocationX()
{
	return Survivor::xPos;
}

float Survivor::getlocationY()
{
	return Survivor::yPos;
}

/********************************************************************************
* Assigning an initial/new weight for a Survivor.
* @param weight - an integer parameter that should be the new weight in kilograms
Add a comment to this line
* for a Survivor object.
********************************************************************************/
void Survivor::setWeight(double weight)
{
	Survivor::weight = weight * convertKG;
}

/********************************************************************************
* A method that retrieves the current assigned weight to the Survivor object.
* @return Returns an integer value that refers to the previously assigned weight
* in kilograms.
********************************************************************************/
double Survivor::getWeight()
{
	return Survivor::weight;
}


double Survivor::getEnergy()
{
	return Survivor::energy;
}

void Survivor::setEnergy(double newEnergy)
{
	Survivor::energy = newEnergy;
}

int Survivor::getWood()
{
	return Survivor::wood;
}

void Survivor::setWood(int newWood)
{
	Survivor::wood = newWood;
}

void Survivor::setPreyKilled(int newPreyKilled)
{
	Survivor::preyStock = newPreyKilled;
}

int Survivor::getPreyKilled()
{
	return Survivor::preyStock;
}


void Survivor::setSpeed(double nS)
{
	Survivor::speed = nS;
}

double Survivor::getSpeed()
{
	return Survivor::speed;;
}

void Survivor::setDehydration(double nDehy)
{
	Survivor::dehydration = nDehy;
}

double Survivor::getDehydration()
{
	return Survivor::dehydration;
}

void Survivor::setTemperature(double nTemperature)
{
	Survivor::temperature = nTemperature;
}

double Survivor::getTemperature()
{
	return Survivor::temperature;
}

void Survivor::setWaterLevel(double nWaterLevel)
{
	Survivor::waterLevel = nWaterLevel;
}

double Survivor::getWaterLevel()
{
	return Survivor::waterLevel;
}

void Survivor::setCurrDehyd(double nCurrDehyd)
{
	Survivor::curr_dehydration = nCurrDehyd;
}

double Survivor::getCurrDehyd()
{
	return curr_dehydration;
}

double Survivor::calculateBMR()
{
	bmr = height * 6.25 + weight * 9.99 - age * 4.92 + 5;
	return bmr;
}

Health* Survivor::getHealth()
{
	return Survivor::health;
}

bool Survivor::getInjuryStatus()
{
	return Survivor::injuryStatus;
}

void Survivor::setInjuryStatus() {
	injuryStatus = false;
}

void Survivor::setSTask(std::string nSTask)
{
	Survivor::sTask = nSTask;
}

std::string Survivor::getSTask()
{
	return Survivor::sTask;
}

double Survivor::weigthLoss()
{	
	return (60 * Survivor::bmr / 1000) * 0.20;
}

void Survivor::setInitialWeight(double nInitialWeight)
{
	Survivor::initialWeight = nInitialWeight;
}

double Survivor::getInitialWeight()
{
	return Survivor::initialWeight;
}

int Survivor::getTask() {
	return task;
}