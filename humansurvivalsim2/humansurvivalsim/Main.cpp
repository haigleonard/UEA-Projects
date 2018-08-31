//includes areas for keyboard control, mouse control, resizing the window
//and draws a spinning rectangle
#include <windows.h>		// Header File For Windows
#include "Image_Loading/nvImage.h"
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "math.h"
#include <stdlib.h>
#include <time.h>
#include <MMSystem.h>
#include "Model/Map.h"
#include "Model/Survivor.h"
#include "Model/Health.h"
#include "Model/Prey.h"
#include "Model/PathFinder.h"
#include "Model/Node.h"
#include "Model/Fire.h"
#include "Model/Water.h"
#include "Model/console.h"
#include "Model/logFile.h"
SimulationInfo stateInfo;
Map myMap;
Health health;
Survivor mySurvivor;
PathFinder finder;
Fire myFire;
Shelter myShelter;
Water myWater;
std::vector<Prey> prey;
logFile myLog;
float scroll = 0, mouse_x = 0, mouse_y = 0, screenWidth = 1500, screenHeight = 1000, zoom = 30, cameraX = 30, cameraY = 80;
float rainvar = 0, normalT = 1.0, tropicT = 0.5, graphs[8][1000], camera = 0.5f, lighting = 0.0, firelight = 1.0;
float offsetX = 0.167, offsetY = 0.0; //Offset values for texturing purposes
int dayUnits = 0, dayTens = 0, dayHundreds = 0, dayCount = 0, task = 0, frames = 12, mapSize = 50, deerNo = 5, treeNo = 10, previousWood = 0, fireCount = 0;

//WazAdded renamed LeftPressed to leftPressed
bool keys[256], leftPressed = false, island = false, survivor = false, fireToken = false, raining = false, resources = true, graphLock = false, clickToken = false, drySeasonBool = true; //WazAdded bool to control what texture is used
bool hoverD = false, hoverI = false, hoverP = false, hoverBack = false, newWoodCollected = false; //Flags for change of colour for buttons WazAdded

GLuint grassTex, waterTex, mountainTex, treeSpriteSheet, islandBut, survivorBut, islandBut2, survivorBut2, day0, day1, day2, day3, day4, day5, day6, day7, day8, day9, previous, nextNum, waterHuman, food, energyTex, semi, woodSprite, degrees;
GLuint pause, pause2, play, play2, fireTex, titleTex, moonTex, sunTex, deerTex, rip, shelter, rain, whiteT, logTex;
GLuint running, waterCol, woodforFire, woodforeShel, zzz, makingShel, makingFire, maintainingFire, hunting, filterWater, cookingFood, calcTask, mapTex, go, normalTemp, tropicTemp, percent, rudy, graphT, blur, weightgraph, tempGraph, treeGraph, preyGraph;
GLuint dryTex, monsoonTex, preyText, treeText, mapText, whiteBorder, yellowBorder, go2, dayMoon, whiteBorderH, decreaseFPS, decreaseFPS2, increaseFPS, increaseFPS2, paused, fps, back, back2, fireSS, healthSS; 
GLuint battery_SS, blur2, cals, deci, hungerlev, hydrTex, hydr, litres, preyCount, temptex, weightTex, woodHead, treeCountH, levels_background, filteredTex, heatmap,decide, returnHome, checkMet, filteredT, wander;
//WazAdded text GLuints + whiteBorder + yellowBorder+ go2 + dayMoon + whiteBorderH + previous - prewind + nextNum - forw + decreaseFPS + increaseFPS + paused + decreaseFPS2 + increaseFPS2 + play2 + pause2 + fps
//WazAdded back
void moveConsoleWindow(int w, int h);
void Graphs();

GLuint loadPNG(char* name)
{
	// Texture loading object
	nv::Image img;

	GLuint myTextureID;

	// Return true on success
	if (img.loadImageFromFile(name))
	{
		glGenTextures(1, &myTextureID);
		glBindTexture(GL_TEXTURE_2D, myTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}

	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);

	return myTextureID;

}

//TEXTURE LOADER
void init()
{
	grassTex = loadPNG("tex/grass.png");
	waterTex = loadPNG("tex/water.png");
	mountainTex = loadPNG("tex/mountain.png");
	treeSpriteSheet = loadPNG("tex/treeSpriteSheet.png"); 
	treeText = loadPNG("tex/treeText.png"); 
	islandBut = loadPNG("tex/island.png");
	survivorBut = loadPNG("tex/survivor.png"); 
	islandBut2 = loadPNG("tex/island2.png");
	survivorBut2 = loadPNG("tex/survivor2.png");
	titleTex = loadPNG("tex/title.png");
	sunTex = loadPNG("tex/sun.png");
	moonTex = loadPNG("tex/moon.png");
	pause = loadPNG("tex/pause.png");
	pause2 = loadPNG("tex/hoverPause.png"); 
	paused = loadPNG("tex/paused.png"); 
	play = loadPNG("tex/play.png");
	play2 = loadPNG("tex/hoverPlay.png"); 
	fireTex = loadPNG("tex/fire.png");
	fireSS = loadPNG("tex/campfire_spritesheet.png"); 
	deerTex = loadPNG("tex/turkey.png");
	preyText = loadPNG("tex/preyText.png"); 
	rip = loadPNG("tex/Headstone.png");
	nextNum = loadPNG("tex/next.png"); 
	previous = loadPNG("tex/previous.png");  
	increaseFPS = loadPNG("tex/increaseFPS.png");  
	increaseFPS2 = loadPNG("tex/hoverIncreaseFPS.png");
	decreaseFPS = loadPNG("tex/decreaseFPS.png");
	decreaseFPS2 = loadPNG("tex/hoverDecreaseFPS.png"); 
	fps = loadPNG("tex/fps.png");
	shelter = loadPNG("tex/shelter.png");
	rain = loadPNG("tex/rain.png");
	food = loadPNG("tex/food.png");
	logTex = loadPNG("tex/book.png");
	back = loadPNG("tex/back.png");
	back2 = loadPNG("tex/back2.png");
	semi = loadPNG("tex/semi.png");
	woodSprite = loadPNG("tex/wood.png");
	degrees = loadPNG("tex/deg.png");
	mapTex = loadPNG("tex/map.png");
	weightgraph = loadPNG("tex/weightgraph.png");
	mapText = loadPNG("tex/mapText.png");
	go = loadPNG("tex/simulate.png");
	blur = loadPNG("tex/blur.png");
	go2 = loadPNG("tex/simulate2.png");
	dryTex = loadPNG("tex/drySeason.png");
	monsoonTex = loadPNG("tex/monsoonSeason.png");
	tropicTemp = loadPNG("tex/monsoon.png"); 
	normalTemp = loadPNG("tex/dry.png");
	rudy = loadPNG("tex/survivorB.png");
	graphT = loadPNG("tex/Zgraph.png");
	treeGraph = loadPNG("tex/treegraph.png");
	preyGraph = loadPNG("tex/preyGraph.png");
	tempGraph = loadPNG("tex/tempgraph.png");
	dayMoon = loadPNG("tex/day_moon.png"); 
	healthSS = loadPNG("tex/health_spritesheet.png");
	day0 = loadPNG("tex/0.png");
	day1 = loadPNG("tex/1.png");
	day2 = loadPNG("tex/2.png");
	day3 = loadPNG("tex/3.png");
	day4 = loadPNG("tex/4.png");
	day5 = loadPNG("tex/5.png");
	day6 = loadPNG("tex/6.png");
	day7 = loadPNG("tex/7.png");
	day8 = loadPNG("tex/8.png");
	day9 = loadPNG("tex/9.png");
	
	battery_SS = loadPNG("tex/battery_SS.png");
	blur2 = loadPNG("tex/blur2.png");
	cals = loadPNG("tex/cals.png");
	deci = loadPNG("tex/dec.png");
	energyTex = loadPNG("tex/energyTex.png");
	hungerlev = loadPNG("tex/hungerlev.png");
	hydrTex = loadPNG("tex/hydrTex.png");
	hydr = loadPNG("tex/hydr.png");
	litres = loadPNG("tex/litres.png");
	preyCount = loadPNG("tex/preyCount.png");
	temptex = loadPNG("tex/temptex.png");
	treeCountH = loadPNG("tex/treeCountH.png");
	weightTex = loadPNG("tex/weightTex.png");
	woodHead = loadPNG("tex/woodHead.png");
	levels_background = loadPNG("tex/levels_background.png");
	filteredTex = loadPNG("tex/filteredTex.png");
	filteredT = loadPNG("tex/filteringWaterT.png");
	heatmap = loadPNG("tex/heatmap.png");
	
	decide = loadPNG("tex/deciding.png");
	returnHome = loadPNG("tex/returnHome.png");
	checkMet = loadPNG("tex/checkMetrics.png");
	returnHome = loadPNG("tex/returnHome.png");
	waterCol = loadPNG("tex/watercollecting.png");
	woodforFire = loadPNG("tex/woodforfire.png");
	woodforeShel = loadPNG("tex/woodforshelter.png");
	zzz = loadPNG("tex/zzz.png");
	makingShel = loadPNG("tex/makingshelter.png");
	makingFire = loadPNG("tex/makingfire.png");
	maintainingFire = loadPNG("tex/maintainfire.png");
	hunting = loadPNG("tex/huntingforfood.png");
	filterWater = loadPNG("tex/filteringWater.png");
	cookingFood = loadPNG("tex/cookingFood.png");
	whiteBorder = loadPNG("tex/whiteBorder.png"); 
	whiteBorderH = loadPNG("tex/whiteBorderHorizontal.png");
	yellowBorder = loadPNG("tex/yellowBorder.png"); //WazAdded
	calcTask = loadPNG("tex/noTask.png"); //WazAdded
	wander = loadPNG("tex/wander.png");
}

/********************************************************************************
* A function that when passed an existing, preloaded image will draw it to the
* screen where intended by the width and height parameters.
* @param textureName A GLuint that refers to the texture that will be used for the
*					 image.
* @param w1 An integer that refers to how far from the left of the window the image
*			needs to be from (-15 - 15).
* @param w2 An integer that refers to how far from the right of the window the image
*			needs to be from (15 - -15).
* @param h1 An integer that refers to how far from the bottom of the window the image
*			needs to be from (-15 - 15).
* @param h2 An integer that refers to how far from the top of the window the image
*			needs to be from (15 - -15).
********************************************************************************/
//DRAW FUNCTIONS
void drawImg(GLuint textureName, float w1, float w2, float h1, float h2, float alpha) {
	float newZoom1 = (zoom / 15);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textureName);
	glColor4f(1.0, 1.0, 1.0, alpha);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex2f(w1 * newZoom1, h1 * newZoom1);
	glTexCoord2f(0.0, 1.0); glVertex2f(w1 * newZoom1, h2 * newZoom1);
	glTexCoord2f(1.0, 1.0); glVertex2f(w2 * newZoom1, h2 * newZoom1);
	glTexCoord2f(1.0, 0.0); glVertex2f(w2 * newZoom1, h1 * newZoom1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

//WazAdded TODO rename variables appropriately and finish comment
/********************************************************************************
* A function that when passed an existing, preloaded image will draw it to the
* screen where intended by the width and height parameters.
* @param textureName A GLuint that refers to the texture that will be used for the
*					 image.
* @param w1 An integer that refers to how far from the left of the window the image
*			needs to be from (-15 - 15).
* @param w2 An integer that refers to how far from the right of the window the image
*			needs to be from (15 - -15).
* @param h1 An integer that refers to how far from the bottom of the window the image
*			needs to be from (-15 - 15).
* @param h2 An integer that refers to how far from the top of the window the image
*			needs to be from (15 - -15).
* @param blX A float that refers to bottom left
* @param blY A float that refers to bottom left
* @param tlX A float that refers to top left
* @param tlY A float that refers to top left
* @param trX A float that refers to top right
* @param trY A float that refers to top right
* @param brX A float that refers to bottom right
* @param brY A float that refers to bottom right
********************************************************************************/
void drawImg(GLuint textureName, float w1, float w2, float h1, float h2, float alpha, float blX, float blY, float tlX, float tlY, float trX, float trY, float brX, float brY){

	float newZoom1 = (zoom / 15);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textureName);
	glColor4f(1.0, 1.0, 1.0, alpha);
	glBegin(GL_POLYGON);
	glTexCoord2f(blX, blY); glVertex2f(w1 * newZoom1, h1 * newZoom1);
	glTexCoord2f(tlX, tlY); glVertex2f(w1 * newZoom1, h2 * newZoom1);
	glTexCoord2f(trX, trY); glVertex2f(w2 * newZoom1, h2 * newZoom1);
	glTexCoord2f(brX, brY); glVertex2f(w2 * newZoom1, h1 * newZoom1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

//WazAdded TODO rename variables appropriately and finish comment
/********************************************************************************
* A function that when passed an existing, preloaded image will draw it to the
* screen where intended by the width and height parameters.
* @param textureName A GLuint that refers to the texture that will be used for the
*					 image.
* @param w1 An integer that refers to how far from the left of the window the image
*			needs to be from (-15 - 15).
* @param w2 An integer that refers to how far from the right of the window the image
*			needs to be from (15 - -15).
* @param h1 An integer that refers to how far from the bottom of the window the image
*			needs to be from (-15 - 15).
* @param h2 An integer that refers to how far from the top of the window the image
*			needs to be from (15 - -15).
* @param blX A float that refers to bottom left
* @param blY A float that refers to bottom left
* @param tlX A float that refers to top left
* @param tlY A float that refers to top left
* @param trX A float that refers to top right
* @param trY A float that refers to top right
* @param brX A float that refers to bottom right
* @param brY A float that refers to bottom right
********************************************************************************/
void drawIslandObj(GLuint textureName, float w1, float w2, float h1, float h2, float colx, float coly, float colz, float cola, float blX, float blY, float tlX, float tlY, float trX, float trY, float brX, float brY) {

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textureName);
	glColor4f(colx, coly, colz, cola);
	glBegin(GL_POLYGON);
	glTexCoord2f(blX, blY); glVertex2f(w1, h1);
	glTexCoord2f(tlX, tlY); glVertex2f(w1, h2);
	glTexCoord2f(trX, trY); glVertex2f(w2, h2);
	glTexCoord2f(brX, brY); glVertex2f(w2, h1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void number(float w1, float w2, float h1, float h2, int day) {
	if (day == 0) {
		drawImg(day0, w1, w2, h1, h2, 1.0);
	}
	if (day == 1) {
		drawImg(day1, w1, w2, h1, h2, 1.0);
	}
	if (day == 2) {
		drawImg(day2, w1, w2, h1, h2, 1.0);
	}
	if (day == 3) {
		drawImg(day3, w1, w2, h1, h2, 1.0);
	}
	if (day == 4) {
		drawImg(day4, w1, w2, h1, h2, 1.0);
	}
	if (day == 5) {
		drawImg(day5, w1, w2, h1, h2, 1.0);
	}
	if (day == 6) {
		drawImg(day6, w1, w2, h1, h2, 1.0);
	}
	if (day == 7) {
		drawImg(day7, w1, w2, h1, h2, 1.0);
	}
	if (day == 8) {
		drawImg(day8, w1, w2, h1, h2, 1.0);
	}
	if (day == 9) {
		drawImg(day9, w1, w2, h1, h2, 1.0);
	}
}

void drawGraphs(GLuint graphType, float graphID, int G, GLuint statName) {
	float newZoom1 = (zoom / 15);
	float newZoom2 = (zoom / 30);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, graphType);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex2f(-29.5 * newZoom2, (5.3f * newZoom1) + (scroll * newZoom1) - (graphID* newZoom1) + (14 * newZoom2));
	glTexCoord2f(0.0, 1.0); glVertex2f(-14.5 * newZoom2, (5.3f * newZoom1 + (scroll * newZoom1) - (graphID* newZoom1)) + (14 * newZoom2));
	glTexCoord2f(1.0, 1.0); glVertex2f(-14.5 * newZoom2, (4.3f * newZoom1) + (scroll * newZoom1) - (graphID* newZoom1));
	glTexCoord2f(1.0, 0.0); glVertex2f(-29.5 * newZoom2, (4.3f * newZoom1) + (scroll * newZoom1) - (graphID* newZoom1));
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	drawImg(statName, -14.75, -7.24, 5.3 + scroll - graphID + 7, 5.3 + scroll - graphID + 8.5, 1.0);

	float e = 0;
	if (dayCount < 31) {
		for (int i = 1; i <= dayCount; i++) {
			glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f((e* newZoom1) - (0.22 *newZoom1) + (-14 * newZoom1), (4.8 * newZoom1) + (graphs[G][i - 1] / 20) * newZoom1 + (scroll * newZoom1) - (graphID* newZoom1));
			glVertex2f((((e* newZoom1) + (-14 * newZoom1))), (4.8 * newZoom1) + (graphs[G][i] / 20) * newZoom1 + (scroll * newZoom1) - (graphID* newZoom1));
			e += 0.22;
			glEnd();
		}
	}
	else {
		for (int i = dayCount - 30; i < dayCount; i++) {
			glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f((e* newZoom1) - (0.22 *newZoom1) + (-14 * newZoom1), (4.8 * newZoom1) + (graphs[G][i - 1] / 20) * newZoom1 + (scroll * newZoom1) - (graphID* newZoom1));
			glVertex2f((((e* newZoom1) + (-14 * newZoom1))), (4.8 * newZoom1) + (graphs[G][i] / 20) * newZoom1 + (scroll * newZoom1) - (graphID* newZoom1));
			e += 0.22;
			glEnd();
		}
	}
}

/********************************************************************************
* A function that
********************************************************************************/
//FRAMES AND RUNNING
void deltaTime() {
	const float FRAMES_PER_SECOND = frames;
	const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

	DWORD next_game_tick = GetTickCount();

	int sleep_time = 0;

	bool game_is_running = true;



	next_game_tick += SKIP_TICKS;
	sleep_time = next_game_tick - GetTickCount();
	if (sleep_time >= 0) {
		Sleep(sleep_time);
	}
	else {
	}
}

//WazAdded
/********************************************************************************
* A function that draws the relevant emoji sprite in relations to the survivors
* health condition.
********************************************************************************/
void drawHealthStatus() {

	Survivor* survivor = stateInfo.getSurvivor();

	if(stateInfo.getSurvivor()->getWeight() <= stateInfo.getSurvivor()->getInitialWeight() - stateInfo.getSurvivor()->weigthLoss()) drawImg(healthSS, 12.35, 14.45, -13.2, -11.1, 1.0, 0.8, 0.0, 0.8, 1.0, 1.0, 1.0, 1.0, 0.0); //Death
	else if (survivor->currentState == Survivor::rest) drawImg(healthSS, 12.35, 14.45, -13.2, -11.1, 1.0, 0.2, 0.0, 0.2, 1.0, 0.4, 1.0, 0.4, 0.0); //Sleep
	else drawImg(healthSS, 12.35, 14.45, -13.2, -11.1, 1.0, 0.4, 0.0, 0.4, 1.0, 0.6, 1.0, 0.6, 0.0); //indifferent(mood that relates to being stuck on an island trying to survive lol

	//Implement once illness and body temperature is running
	//drawImg(healthSS, 12.3, 14.7, -13.2, -10.8, 1.0, 0.0, 0.0, 0.0, 1.0, 0.2, 1.0, 0.2, 0.0); //Sick
	//drawImg(healthSS, 12.35, 14.45, -13.2, -11.1, 1.0, 0.6, 0.0, 0.6, 1.0, 0.8, 1.0, 0.8, 0.0); //Cold

}

//WazAdded
/********************************************************************************
* A function that draws highlighted or non-highlighted buttons to increase/decrease
* the fps of the simulation and also draws either a play or pause button depending
* on the simulation state.
********************************************************************************/
void drawSimulationControls() {

	int tens, units; //Default fps value split into tens and units.

	//WazAdded Controls for increasing frames and pausing/playing simulation.
	if (!stateInfo.getPause()) {

		if (hoverP) drawImg(pause2, 12.85, 13.85, -8.85, -7.85, 1.0); else drawImg(pause, 12.85, 13.85, -8.85, -7.85, 1.0);

	}
	else {

		drawImg(paused, -7.1, 15.0, -8.9, 13.6, 1.0); //WazAdded paused overlay
		if (hoverP) drawImg(play2, 12.85, 13.85, -8.85, -7.85, 1.0); else drawImg(play, 12.85, 13.85, -8.85, -7.85, 1.0);

	}

	//Draws fps icon and numbers relative to the current fps of simulation
	drawImg(fps, 12.86, 13.83, -7.8, -7.3, 1.0);
	tens = frames / 10; units = frames % 10;
	number(13.86, 14.36, -7.8, -7.3, tens);
	number(14.4, 14.9, -7.8, -7.3, units);

	if (hoverD) drawImg(decreaseFPS2, 11.8, 12.8, -8.85, -7.85, 1.0); else drawImg(decreaseFPS, 11.8, 12.8, -8.85, -7.85, 1.0);
	if (hoverI) drawImg(increaseFPS2, 13.9, 14.9, -8.85, -7.85, 1.0); else drawImg(increaseFPS, 13.9, 14.9, -8.85, -7.85, 1.0);

	hoverD = false; hoverI = false; hoverP = false; //Resetting flags
}

void pauseSim() {

	float newSWMult = (screenWidth / 1500); //New screen width multiplier
	float newSHMult; //TODO implement screenheight changes

	//WazAdded
	//DecreaseFPS button - newSWMult used for when window size is adjusted.
	if (mouse_x < (newSWMult * 1390) && mouse_x > (newSWMult * 1339) && mouse_y < 240 && mouse_y > 209) {
		
		hoverD = true;
		if (leftPressed && !clickToken) {
			if (frames > 11) frames--;
		}

	//Play/Pause Button
	} else if (mouse_x < (newSWMult * 1442) && mouse_x >(newSWMult * 1392) && mouse_y < 240 && mouse_y > 209) {

		hoverP = true;
		if (leftPressed && !clickToken) {
			
			if (!stateInfo.getPause()) stateInfo.setPause(true);
			else stateInfo.setPause(false);

			clickToken = true;
			
		}
		
	//IncreaseFPS button
	} else if (mouse_x < (newSWMult * 1494) && mouse_x >(newSWMult * 1445) && mouse_y < 240 && mouse_y > 209) {
	
		hoverI = true;
		if (leftPressed && !clickToken) {
			if (frames < 60) frames++;
		}

	}

	if (!leftPressed) clickToken = false;
	
	
}

//CHARACTER AND ANIMALS
void drawPrey() {
	
	int numOfPrey = stateInfo.getPreyCount(); //WazAdded additional condition when no prey is present

	if (!stateInfo.getPause() && numOfPrey > 0) {
		int preyIndex = rand() % prey.size();
		
		//Loop to change a random amount of randomly selected preys X & Y values		
		prey[preyIndex].preyFSM->update(&prey[preyIndex], &stateInfo, prey[preyIndex].currentState);

		// Retrieving information from simulation info due to the updates from FSM below
		stateInfo.updatePrey(stateInfo.getPrey(), stateInfo.getMap(), stateInfo.getPause(), true);
	}

	for (int i = 0; i < stateInfo.getPreyCount(); i++) {
		drawIslandObj(deerTex, -myMap.getNumRows() + prey[i].getX() + cameraX, -myMap.getNumRows() + 1 + prey[i].getX() + cameraX, -myMap.getNumCols() - prey[i].getY() + cameraY, -myMap.getNumCols() + 1 - prey[i].getY() + cameraY, lighting, lighting, lighting, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
	}
	
}

void character() {

	myLog.taskUpdate(stateInfo.getSurvivor()->getSTask());

	stateInfo.getMap()->calculateAvailableNodes();
	
	if (stateInfo.getSurvivor()->getEnergy() / 200 > 0.1) {
		if (myMap.getDegrees() > 25) {
			stateInfo.getSurvivor()->setSpeed((stateInfo.getSurvivor()->getEnergy() / 225));
		}
		else {
			stateInfo.getSurvivor()->setSpeed((stateInfo.getSurvivor()->getEnergy() / 200));
		}
	}
	
	if (stateInfo.getSurvivor()->getWaterLevel() <= -1200 || stateInfo.getSurvivor()->getWeight() <= stateInfo.getSurvivor()->getInitialWeight() - stateInfo.getSurvivor()->weigthLoss()) {
		drawIslandObj(rip, -stateInfo.getMap()->getNumRows() + mySurvivor.getlocationX() + cameraX, -stateInfo.getMap()->getNumRows() + 1 + mySurvivor.getlocationX() + cameraX, -stateInfo.getMap()->getNumCols() - mySurvivor.getlocationY() + cameraY, -stateInfo.getMap()->getNumCols() + 1 - mySurvivor.getlocationY() + cameraY, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
	}
	else {
		
			drawIslandObj(rudy, -stateInfo.getMap()->getNumRows() + mySurvivor.getlocationX() + cameraX, -stateInfo.getMap()->getNumRows() + 1 + mySurvivor.getlocationX() + cameraX, -stateInfo.getMap()->getNumCols() - mySurvivor.getlocationY() + cameraY, -stateInfo.getMap()->getNumCols() + 1 - mySurvivor.getlocationY() + cameraY, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
			if (!stateInfo.getPause()) {
			//Updating info
			stateInfo.update(stateInfo.getSurvivor(), stateInfo.getMap(), stateInfo.getFire(), stateInfo.getShelter(), stateInfo.getWater(), stateInfo.getHealth(),  stateInfo.checkNightBool(),
				stateInfo.getMove(), stateInfo.getPause(), stateInfo.getNumMoves(), stateInfo.getHours(), stateInfo.getMinutes());

			//Use updatted info into FSM
			mySurvivor.survivorFSM->update(&mySurvivor, &stateInfo, mySurvivor.currentState);
		}
	}
}

//MAP
void drawMap() {

	if (stateInfo.checkNightBool()) { if ((stateInfo.getHours() - 7) < 9) { lighting = 0.4 + ((stateInfo.getHours() - 7) / 14); } else { lighting = 0.9714 - (((stateInfo.getHours() - 7) - 8) / 14); } } // 0.9714285714
	else { lighting = 0.4; }
	if (firelight >= 1.0) { fireToken = true; }
	if (firelight <= 0.3) { fireToken = false; }

	if ((rand() % 10) == 5 && !stateInfo.getPause()) { firelight += 0.2; }
	if (fireToken) {
		firelight -= 0.05;
	}
	else {
		if (!stateInfo.getPause())firelight += 0.05;
	}

	fireCount++;

	for (int i = 0; i < stateInfo.getMap()->getNumCols(); i++) {
		for (int e = 0; e < stateInfo.getMap()->getNumRows(); e++) {
			if (stateInfo.getMap()->getNode(e, i)->getValue() != -9) {
				drawIslandObj(grassTex, -stateInfo.getMap()->getNumCols() + e + cameraX, -stateInfo.getMap()->getNumCols() + 1 + e + cameraX, -stateInfo.getMap()->getNumCols() - i + cameraY, -stateInfo.getMap()->getNumCols() + 1 - i + cameraY, lighting, lighting, lighting, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
			}
			if (stateInfo.getMap()->getNode(e, i)->getValue() == 0) {
				drawIslandObj(waterTex, -stateInfo.getMap()->getNumCols() + e + cameraX, -stateInfo.getMap()->getNumCols() + 1 + e + cameraX, -stateInfo.getMap()->getNumCols() - i + cameraY, -stateInfo.getMap()->getNumCols() + 1 - i + cameraY, lighting, lighting, lighting, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
			}
			if (stateInfo.getMap()->getNode(e, i)->getValue() == -1) {

				drawIslandObj(mountainTex, -stateInfo.getMap()->getNumCols() + e + cameraX, -stateInfo.getMap()->getNumCols() + 1 + e + cameraX, -stateInfo.getMap()->getNumCols() - i + cameraY, -stateInfo.getMap()->getNumCols() + 1 - i + cameraY, lighting, lighting, lighting, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);

			}
			if (stateInfo.getMap()->getNode(e, i)->getValue() == -2) {
				drawIslandObj(treeSpriteSheet, -stateInfo.getMap()->getNumCols() + e + cameraX, -stateInfo.getMap()->getNumCols() + 1 + e + cameraX, -stateInfo.getMap()->getNumCols() - i + cameraY, -stateInfo.getMap()->getNumCols() + 1 - i + cameraY, lighting, lighting, lighting, 1.0, 0.75, 0.0, 0.75, 0.5, 1.0, 0.5, 1.0, 0.0);
			}
			if (stateInfo.getMap()->getNode(e, i)->getValue() == -3) {

				//WazAdded for fire animation

				if (fireCount > 3) {

					fireCount = 0;
					if (offsetX > 1) offsetX = 0.167; else offsetX += 0.167;

				}
				if (!stateInfo.checkNightBool() && myFire.checkFireMade()) {
					drawIslandObj(grassTex, -stateInfo.getMap()->getNumCols() + e + cameraX, -stateInfo.getMap()->getNumCols() + 1 + e + cameraX, -stateInfo.getMap()->getNumCols() - i + cameraY, -stateInfo.getMap()->getNumCols() + 1 - i + cameraY, firelight, 0.3, 0.3, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
				}drawIslandObj(fireSS, -stateInfo.getMap()->getNumCols() + e + cameraX, -stateInfo.getMap()->getNumCols() + 1 + e + cameraX, -stateInfo.getMap()->getNumCols() - i + cameraY, -stateInfo.getMap()->getNumCols() + 1 - i + cameraY, firelight, firelight - lighting, firelight - lighting, 1.0, -0.167 + offsetX, 0.0, -0.167 + offsetX, 1.0, 0.0 + offsetX, 1.0, 0.0 + offsetX, 0.0);

			}
			if (stateInfo.getMap()->getNode(e, i)->getValue() == -4) {
				drawIslandObj(shelter, -stateInfo.getMap()->getNumCols() + e + cameraX, -stateInfo.getMap()->getNumCols() + 1 + e + cameraX, -stateInfo.getMap()->getNumCols() - i + cameraY, -stateInfo.getMap()->getNumCols() + 1 - i + cameraY, lighting, lighting, lighting, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
			}

		}


	}
	if (!stateInfo.checkNightBool() && myFire.checkFireMade()) {
		if (stateInfo.getMap()->getNode(stateInfo.getFire()->getX()-1, stateInfo.getFire()->getY())->getValue() == 1) { drawIslandObj(grassTex, -stateInfo.getMap()->getNumCols() + (stateInfo.getFire()->getX() - 1) + cameraX, -stateInfo.getMap()->getNumCols() + 1 + (stateInfo.getFire()->getX() - 1) + cameraX, -stateInfo.getMap()->getNumCols() - stateInfo.getFire()->getY() + cameraY, -stateInfo.getMap()->getNumCols() + 1 - stateInfo.getFire()->getY() + cameraY, firelight, 0.3, 0.3, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0); }
		if (stateInfo.getMap()->getNode(stateInfo.getFire()->getX()+1, stateInfo.getFire()->getY())->getValue() == 1) { drawIslandObj(grassTex, -stateInfo.getMap()->getNumCols() + (stateInfo.getFire()->getX() + 1) + cameraX, -stateInfo.getMap()->getNumCols() + 1 + (stateInfo.getFire()->getX() + 1) + cameraX, -stateInfo.getMap()->getNumCols() - stateInfo.getFire()->getY() + cameraY, -stateInfo.getMap()->getNumCols() + 1 - stateInfo.getFire()->getY() + cameraY, firelight, 0.3, 0.3, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);}
		if (stateInfo.getMap()->getNode(stateInfo.getFire()->getX(), stateInfo.getFire()->getY()+1)->getValue() == 1) { drawIslandObj(grassTex, -stateInfo.getMap()->getNumCols() + stateInfo.getFire()->getX() + cameraX, -stateInfo.getMap()->getNumCols() + 1 + stateInfo.getFire()->getX() + cameraX, -stateInfo.getMap()->getNumCols() - (stateInfo.getFire()->getY() + 1) + cameraY, -stateInfo.getMap()->getNumCols() + 1 - (stateInfo.getFire()->getY() + 1) + cameraY, firelight, 0.3, 0.3, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);}
		if (stateInfo.getMap()->getNode(stateInfo.getFire()->getX(), stateInfo.getFire()->getY()-1)->getValue() == 1) { drawIslandObj(grassTex, -stateInfo.getMap()->getNumCols() + stateInfo.getFire()->getX() + cameraX, -stateInfo.getMap()->getNumCols() + 1 + stateInfo.getFire()->getX() + cameraX, -stateInfo.getMap()->getNumCols() - (stateInfo.getFire()->getY() - 1) + cameraY, -stateInfo.getMap()->getNumCols() + 1 - (stateInfo.getFire()->getY() - 1) + cameraY, firelight, 0.3, 0.3, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);}
	
		if (stateInfo.getMap()->getNode(stateInfo.getFire()->getX()-1, stateInfo.getFire()->getY()+1)->getValue() == 1) { drawIslandObj(grassTex, -stateInfo.getMap()->getNumCols() + (stateInfo.getFire()->getX() - 1) + cameraX, -stateInfo.getMap()->getNumCols() + 1 + (stateInfo.getFire()->getX() - 1) + cameraX, -stateInfo.getMap()->getNumCols() - (stateInfo.getFire()->getY() + 1) + cameraY, -stateInfo.getMap()->getNumCols() + 1 - (stateInfo.getFire()->getY() + 1) + cameraY, firelight, 0.3, 0.3, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);}
		if (stateInfo.getMap()->getNode(stateInfo.getFire()->getX()+1, stateInfo.getFire()->getY()+1)->getValue() == 1) { drawIslandObj(grassTex, -stateInfo.getMap()->getNumCols() + (stateInfo.getFire()->getX() + 1) + cameraX, -stateInfo.getMap()->getNumCols() + 1 + (stateInfo.getFire()->getX() + 1) + cameraX, -stateInfo.getMap()->getNumCols() - (stateInfo.getFire()->getY() + 1) + cameraY, -stateInfo.getMap()->getNumCols() + 1 - (stateInfo.getFire()->getY() + 1) + cameraY, firelight, 0.3, 0.3, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);}
		if (stateInfo.getMap()->getNode(stateInfo.getFire()->getX()-1, stateInfo.getFire()->getY()-1)->getValue() == 1) { drawIslandObj(grassTex, -stateInfo.getMap()->getNumCols() + (stateInfo.getFire()->getX() - 1) + cameraX, -stateInfo.getMap()->getNumCols() + 1 + (stateInfo.getFire()->getX() - 1) + cameraX, -stateInfo.getMap()->getNumCols() - (stateInfo.getFire()->getY() - 1) + cameraY, -stateInfo.getMap()->getNumCols() + 1 - (stateInfo.getFire()->getY() - 1) + cameraY, firelight, 0.3, 0.3, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);}
		if (stateInfo.getMap()->getNode(stateInfo.getFire()->getX()+1, stateInfo.getFire()->getY()-1)->getValue() == 1) { drawIslandObj(grassTex, -stateInfo.getMap()->getNumCols() + (stateInfo.getFire()->getX() + 1) + cameraX, -stateInfo.getMap()->getNumCols() + 1 + (stateInfo.getFire()->getX() + 1) + cameraX, -stateInfo.getMap()->getNumCols() - (stateInfo.getFire()->getY() - 1) + cameraY, -stateInfo.getMap()->getNumCols() + 1 - (stateInfo.getFire()->getY() - 1) + cameraY, firelight, 0.3, 0.3, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);}
	}

	

	
	
	

	if (raining) {
		for (int i = -50; i < 100; i += 10) {
			for (int e = -50; e < 100; e += 10) {
				drawIslandObj(rain, e, e + 10, i - rainvar, i + 10 - rainvar, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);
			}
		}
		rainvar += 0.5;

		if (rainvar > 10) {
			rainvar = 0;
		}
	}
}

//WazAdded //TODO maybe rework this
/********************************************************************************
* A function that increments the time
* @author Haig
********************************************************************************/
void calculateTime() {

	if (!stateInfo.getPause()) {
		stateInfo.setHours(stateInfo.getHours() + 0.02);
		myLog.logAverage(float(stateInfo.getSurvivor()->getWeight()), (float(stateInfo.getSurvivor()->getWaterLevel())), (float(stateInfo.getSurvivor()->getEnergy())), 
			(float(stateInfo.getSurvivor()->getWood())), (float(stateInfo.getSurvivor()->getPreyKilled())), (float(myMap.getTreeCount())));
		myMap.updateTemperature(int(stateInfo.getHours()), int(stateInfo.getMinutes()));
		if (stateInfo.getHours() >= (12 * 2)) {
			dayUnits++;
			for (int i = 0; i < stateInfo.getMap()->getNumCols(); i++) {
				for (int e = 0; e < stateInfo.getMap()->getNumRows(); e++) {
					stateInfo.getMap()->getNode(e, i)->reduceHeat();
				}
			}

			myMap.plantTrees(); //spawning more trees
			int v1 = rand() % 3;
			if (v1 == 1) {
				raining = true;
			}
			else {
				raining = false;
			}
			
			if (stateInfo.getPreyCount() < 10) {
				//Spawning a new prey
				stateInfo.getMap()->calculateAvailableNodes();
				std::vector<Node*> availableNodes = stateInfo.getMap()->getAvailableNodes(); //finding new available nodes 

				std::vector<Node> validGoalNodes;
				Node temp, goal;
				std::string gender;

				gender = Animal::chooseGender();

				//Creating new Prey object
				stateInfo.setPreyCount(stateInfo.getPreyCount() + 1);
				Prey p = Prey(&stateInfo);
				prey.push_back(p);

				//Finding a valid start and goal node for this prey to be added
				temp = availableNodes[rand() % availableNodes.size()];
				prey[prey.size() - 1].setX(temp.getX());
				prey[prey.size() - 1].setY(temp.getY());
				validGoalNodes = prey[prey.size() - 1].getValidGoalNodes(&stateInfo);
				goal = validGoalNodes.at(rand() % validGoalNodes.size());
				prey[prey.size() - 1].setPath(finder.path(prey[prey.size() - 1], goal, stateInfo.getMap()));

				//Updating the vector
				stateInfo.setPrey(&prey);
			}
			
			myLog.logUpdate(dayUnits);
			mySurvivor.setInjuryStatus();
			//myHealth.setHunger(myHealth.getHunger() - 25);
			if (dayUnits == 10) {
				dayUnits = 0;
				dayTens++;
			}
			if (dayTens == 10) {
				dayTens = 0;
				dayHundreds++;

			}
			stateInfo.setHours(0);
		}
	}

	stateInfo.setMinutes((stateInfo.getHours() - int(stateInfo.getHours())) * 60);

	// Night = 20:01 - 5:59
	if (int(stateInfo.getHours()) <= 20 && int(stateInfo.getHours()) >= 7) {
		stateInfo.setNightBool(true);
	}
	else {
		stateInfo.setNightBool(false);

	}

}

//WazAdded
/********************************************************************************
* A function that draws the number referring to how many days the
* survivior has survived this is achieved by the iteration of the 24 hour clock.
* The time is also drawn with respect of the current simulation time. It also
* draws the current temperature of the simulation which is in respect to the time
* of day.
* @author Wazzy
********************************************************************************/
void drawSimulationDayTimeTemp() {

	int remainder; //Remainder of hours or minutes

	calculateTime();

	//WazAdded Checking resources are required to be drawn (Logbook feature)
	if (resources) {

		//WazAdded Drawing temperature
		number(10.7, 12.0, -10.4, -9.1, int(myMap.getDegrees() / 10));
		number(12.05, 13.35, -10.4, -9.1, myMap.getDegrees() % 10);
		drawImg(degrees, 13.4, 14.7, -10.4, -9.1, 1.0);

		//WazAdded Drawing time
		remainder = int(stateInfo.getHours()) % 10; //TODO Does hours and minutes in simInfo need to be floats
		number(8.78, 10.08, -14.7, -13.4, int(stateInfo.getHours() / 10));
		number(10.13, 11.43, -14.7, -13.4, remainder);

		remainder = int(stateInfo.getMinutes()) % 10;
		drawImg(semi, 11.48, 12.0, -14.7, -13.4, 1.0);
		number(12.05, 13.35, -14.7, -13.4, int(stateInfo.getMinutes() / 10));
		number(13.4, 14.7, -14.7, -13.4, remainder);
	}
	
	//WazAdded Drawing day numbers
	drawImg(dayMoon, 7.1, 10.85, 13.6, 14.9, 1.0);

	//WazAdded Moon or Sun next to day text depending on the time of day
	if(!stateInfo.checkNightBool()) drawImg(moonTex, 7.35, 8.45, 13.7, 14.8, 1.0); else drawImg(sunTex, 7.35, 8.45, 13.7, 14.8, 1.0);

	number(13.6, 14.9, 13.6, 14.9, dayUnits);
	number(12.25, 13.55, 13.6, 14.9, dayTens);
	number(10.9, 12.2, 13.6, 14.9, dayHundreds);

}

//HUD
void drawHUD() {

	
	float newSWMult = (screenWidth / 1500);
	float mouseXPosition = mouse_x * (screenWidth / 1500); //TODO Change this like the others
														   //WazAdded Drawing title image
	drawImg(titleTex, -7.0, 7.0, 13.6, 14.9, 1.0);
	drawImg(whiteBorder, -14.9, -7.1, -14.9, 13.5, 1.0);
	drawImg(whiteBorderH, -7, 14.9, -8.9, -14.9, 1.0);

	drawSimulationControls();

	//WazAdded Checks if resources is needed to be displayed
	if (resources) drawImg(logTex, 8.88, 10.6, -10.4, -9.1, 1.0);
	else {


		if (mouse_x < (newSWMult * 1484) && mouse_x >(newSWMult * 1435) && mouse_y < 42 && mouse_y > 11) {

			if (leftPressed && !clickToken) {

				clickToken = true;
				resources = true;

			}

			hoverBack = true; //Setting flag to highlight back button

		}
		else hoverBack = false; //Setting flag to not highlight back button

		if (hoverBack) drawImg(back2, 13.7, 14.7, -14.7, -13.7, 1.0); else drawImg(back, 13.7, 14.7, -14.7, -13.7, 1.0);

	}

	//WazAdded Logbook clickable to remove all other resources and display logged events
	if (mouse_x < (newSWMult * 1276) && mouse_x >(newSWMult * 1198) && mouse_y < 194 && mouse_y > 153 && leftPressed && !clickToken && resources) {

		clickToken = true;
		resources = false;

	}
	else { if (!leftPressed) clickToken = false; }
	Graphs();
	//WazAdded
	//TODO dynamic screenheight, coded for graphic lab comps
	//Is mouse is in top region of window, checked which button it's on.

	if (mouse_y < 1003 && mouse_y > 957 && mouseXPosition < 172 && mouseXPosition > 0) {
	
		
		drawImg(islandBut2, -14.9, -11.56, 13.6, 14.9, 1.0);
		if (survivor) drawImg(survivorBut2, -11.51, -7.1, 13.6, 14.9, 1.0); else  drawImg(survivorBut, -11.51, -7.1, 13.6, 14.9, 1.0);

		if (leftPressed) {
			island = true;
			survivor = false;
			scroll = 0; 
		}
		

	} else if (mouse_y < 1003 && mouse_y > 957 && mouseXPosition > 174 && mouseXPosition < 394){

		if (island) drawImg(islandBut2, -14.9, -11.56, 13.6, 14.9, 1.0); else drawImg(islandBut, -14.9, -11.56, 13.6, 14.9, 1.0);
		drawImg(survivorBut2, -11.51, -7.1, 13.6, 14.9, 1.0);

		if (leftPressed) {
			survivor = true;
			island = false;
			scroll = 0; //Resetting scroll point for data HUD.
		}
			
	} else {

		if(island) drawImg(islandBut2, -14.9, -11.56, 13.6, 14.9, 1.0); else drawImg(islandBut, -14.9, -11.56, 13.6, 14.9, 1.0);
		if(survivor) drawImg(survivorBut2, -11.51, -7.1, 13.6, 14.9, 1.0); else drawImg(survivorBut, -11.51, -7.1, 13.6, 14.9, 1.0);

	}
	
	

	
	

}
/********************************************************************************
* //TODO comment this
* A function that
* @author Wazzy
********************************************************************************/
void drawHungerLevel() {
	
	drawImg(hungerlev, -5.51, -4.41, -14.7, -9.1, 1.0);
	double hungerLevel = stateInfo.getCaloriesUsed();
	
	if (hungerLevel > 7999) {
		drawImg(blur2, -5.4, -4.5, -14.1, -9.2, 0.5);
	}
	else if (hungerLevel > 5999) {
		drawImg(blur2, -5.4, -4.5, -14.57, -14.14, 0.5);
		drawImg(blur2, -5.4, -4.5, -13.49, -9.2, 0.5);
	}
	else if (hungerLevel > 3999) {
		drawImg(blur2, -5.4, -4.5, -14.57, -13.58, 0.5);
		drawImg(blur2, -5.4, -4.5, -12.93, -9.2, 0.5);
	}
	else if (hungerLevel > 1999) {
		drawImg(blur2, -5.4, -4.5, -14.57, -13.05, 0.5);
		drawImg(blur2, -5.4, -4.5, -12.39, -9.2, 0.5);
	}
	else {
		drawImg(blur2, -5.4, -4.5, -14.57, -12.5, 0.5);
		drawImg(blur2, -5.4, -4.5, -11.85, -9.2, 0.5);
	}
		
		
	


}

/********************************************************************************
* //TODO comment this
* A function that
* @author Wazzy
********************************************************************************/
void drawEnergyLevel() {
	auto energyLevel = stateInfo.getSurvivor()->getEnergy();

	if (energyLevel < 101 && energyLevel > 80) drawImg(battery_SS, -4.31, -3.21, -14.7, -9.1, 1.0, 0.0, 0.0, 0.0, 1.0, 0.167, 1.0, 0.167, 0.0);
	else if (energyLevel < 81 && energyLevel > 60) drawImg(battery_SS, -4.31, -3.21, -14.7, -9.1, 1.0, 0.167, 0.0, 0.167, 1.0, 0.334, 1.0, 0.334, 0.0);
	else if (energyLevel < 61 && energyLevel > 40) drawImg(battery_SS, -4.31, -3.21, -14.7, -9.1, 1.0, 0.334, 0.0, 0.334, 1.0, 0.5, 1.0, 0.5, 0.0);
	else if (energyLevel < 41 && energyLevel > 20)drawImg(battery_SS, -4.31, -3.21, -14.7, -9.1, 1.0, 0.5, 0.0, 0.5, 1.0, 0.667, 1.0, 0.667, 0.0);
	else if (energyLevel < 21 && energyLevel > 0)drawImg(battery_SS, -4.31, -3.21, -14.7, -9.1, 1.0, 0.667, 0.0, 0.667, 1.0, 0.834, 1.0, 0.834, 0.0);
	else drawImg(battery_SS, -4.31, -3.21, -14.7, -9.1, 1.0, 0.834, 0.0, 0.834, 1.0, 1.0, 1.0, 1.0, 0.0);
}

/********************************************************************************
* A function that
* @author Wazzy
********************************************************************************/
/********************************************************************************
* //TODO comment this
* A function that
* @author Wazzy
********************************************************************************/
void drawHydrationLevel() {
	double  waterLevel = stateInfo.getSurvivor()->getWaterLevel();
	float hydrationLevel = 100.0, os = 0.0; //100% hydrTex

	drawImg(levels_background, -6.71, -5.61, -14.7, -9.1, 1.0);
	if (waterLevel < 0) {
		hydrationLevel = ceil(-waterLevel / 12.0); //Working out how much of hydrText to draw 
		os = 0.053 * hydrationLevel;
		drawImg(hydrTex, -6.55, -5.77, -14.55, -9.25 - os, 1.0, 0.0, 0.0, 0.0, 1.0 - (hydrationLevel / 100), 1.0, 1.0 - (hydrationLevel / 100), 1.0, 0.0);
	}
	else drawImg(hydrTex, -6.55, -5.77, -14.55, -9.25, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0);

}
void drawWaterSupply() {

	int tens, units, tenths, hundreds;
	double filteredWater = stateInfo.getWater()->getFWater(), unFilteredWater = stateInfo.getWater()->getUWater();
	int secondDecimalPoint = (int)filteredWater % 10;
	int firstDecimalPoint = ((int)filteredWater % 100 - secondDecimalPoint) / 10;


	//Drawing filtered water 
	drawImg(filteredTex, -2.25, -0.25, -11.75, -9.1, 1.0, 0.0, 0.0, 0.0, 1.0, 0.5, 1.0, 0.5, 0.0);
	tens = filteredWater / 1000;
	number(-2.71, -2.21, -11.9, -11.4, tens);
	units = filteredWater / 100;
	number(-2.21, -1.71, -11.9, -11.4, units);
	drawImg(deci, -1.70, -1.51, -11.9, -11.5, 1.0);
	number(-1.5, -1.0, -11.9, -11.4, 0);
	number(-1.0, -0.5, -11.9, -11.4, 0);
	drawImg(litres, -0.5, 0.0, -11.9, -11.4, 1.0);

	secondDecimalPoint = (int)unFilteredWater % 10;
	firstDecimalPoint = ((int)unFilteredWater % 100 - secondDecimalPoint) / 10;

	////Drawing unfiltered water 
	drawImg(filteredTex, -2.25, -0.25, -14.5, -12.05, 1.0, 0.5, 0.0, 0.5, 1.0, 1.0, 1.0, 1.0, 0.0);
	tens = unFilteredWater / 1000;
	number(-2.71, -2.21, -14.7, -14.2, tens);
	units = unFilteredWater / 100;
	number(-2.21, -1.71, -14.7, -14.2, units);
	drawImg(deci, -1.70, -1.51, -14.7, -14.2, 1.0);
	number(-1.5, -1.0, -14.7, -14.2, 0);
	number(-1.0, -0.5, -14.7, -14.2, 0);
	drawImg(litres, -0.5, 0.0, -14.7, -14.2, 1.0);
}
//WazAdded
/********************************************************************************
* A function that 
* @author Wazzy
********************************************************************************/
void resourceHUD() {

		float offsetX = 0.0, offsetY = 0.0, currentWood = stateInfo.getSurvivor()->getWood(); //Offsets to position wood sprites
		float alpha = 0.0;
		double currentFireLevel = stateInfo.getFire()->getFireLevel();
		int foodCount = stateInfo.getSurvivor()->getPreyKilled();

		if (resources) {


			//Drawing hydration level
			drawHydrationLevel();

			//Drawing hunger level
			drawHungerLevel();

			//Drawing energy level
			drawEnergyLevel();

			//Drawing unfiltered and filtered metrics
			drawWaterSupply();

			//Drawing food stock
			for (int i = 0; i < 5; i++) {

				if (i < foodCount) {
					drawImg(food, 0.53, 2.03, -14.7 + offsetY, -13.5 + offsetY, 1.0);

				}
				else drawImg(food, 0.53, 2.03, -14.7 + offsetY, -13.5 + offsetY, 0.4);
				offsetY += 1.1;
			}
			offsetY = 0.0;

			//Drawing wood stock in pyramid formation when max wood stock (10 max)
			for (int i = 0; i < 10; i++) {

				if (currentWood > 0) drawImg(woodSprite, 6.88 - offsetX, 8.28 - offsetX, -14.7 + offsetY, -13.3 + offsetY, 1.0, 0.0, 0.5, 0.0, 1.0, 0.5, 1.0, 0.5, 0.5);
				else drawImg(woodSprite, 6.88 - offsetX, 8.28 - offsetX, -14.7 + offsetY, -13.3 + offsetY, 0.4, 0.0, 0.5, 0.0, 1.0, 0.5, 1.0, 0.5, 0.5);
				currentWood--; //ADDED

				switch (i) {
				case 3:
					offsetX = 0.7;
					offsetY = 1.25;
					break;
				case 6:
					offsetX = 1.4;
					offsetY = 2.5;
					break;
				case 8:
					offsetX = 2.1;
					offsetY = 3.75;
					break;
				default:
					offsetX += 1.45;
					break;
				}


			}

			//drawing fire level to indicate the status of the campfire (Roaring - dowsed)
			if (currentFireLevel < 101 && currentFireLevel > 74) offsetX = 0.25;
			else if (currentFireLevel < 76 && currentFireLevel > 50)  offsetX = 0.5;
			else if (currentFireLevel < 51 && currentFireLevel > 24)  offsetX = 0.75;
			else offsetX = 0.25;
			if(!myFire.checkFireMade()){ offsetX = 1.0; }
			drawImg(fireTex, 8.99, 11.27, -13.2, -10.8, 1.0, -0.25 + offsetX, 0.0, -0.25 + offsetX, 1.0, -0.25 + (offsetX + 0.25), 1.0, -0.25 + (offsetX + 0.25), 0.0);

			drawHealthStatus();

		}
	else {

		//Determines what the survivor is currently doing and draws appropriately to the screen 
		task = mySurvivor.getTask();
		switch (task) {
		
		case 1:
			drawImg(wander, -6.85, 14.7, -13.1, -10.1, 1.0);
			break;
		case 2:
			drawImg(zzz, -6.85, 14.7, -13.1, -10.1, 1.0);
			break;
		case 3:
			drawImg(woodforFire, -6.85, 14.7, -13.1, -10.1, 1.0);
			break;
		case 4:
			drawImg(waterCol, -6.85, 14.7, -13.1, -10.1, 1.0);
			break;
		case 5:
			drawImg(makingShel, -6.85, 14.7, -13.1, -10.1, 1.0);
			break;
		case 6:
			drawImg(makingFire, -6.85, 14.7, -13.1, -10.1, 1.0);
			break;
		case 7:
			drawImg(returnHome, -6.85, 14.7, -13.1, -10.1, 1.0); // returning home
			break;
		case 8:
			drawImg(decide, -6.85, 14.7, -13.1, -10.1, 1.0); //decision
			break;
		case 9:
			drawImg(checkMet, -6.85, 14.7, -13.1, -10.1, 1.0); // check metrics
			break;
		case 10:
			drawImg(filteredT, -6.85, 14.7, -13.1, -10.1, 1.0); //filtered water
			break;
		case 11:
			drawImg(waterCol, -6.85, 14.7, -13.1, -10.1, 1.0);
			break;
		case 12:
			drawImg(cookingFood, -6.85, 14.7, -13.1, -10.1, 1.0);
			break;
		default:
			break;
		}
	}

}

//WINDOW AND KEYS
void setUpSim() {

	srand(time(NULL));
	myMap.iniMap(mapSize, mapSize, normalT, treeNo);
	stateInfo.setMap(&myMap);
	stateInfo.setFire(&myFire);
	stateInfo.setWater(&myWater);
	stateInfo.setShelter(&myShelter);
	stateInfo.setHealth(&health);
	mySurvivor = Survivor(&stateInfo); //initially set to wander state
	stateInfo.setSurvivor(&mySurvivor);
	stateInfo.setHours(6.0);
	stateInfo.setMinutes(0.0);
	stateInfo.setPreyCount(deerNo);
	
	// Initialising Prey objects & setting position of Prey.
	stateInfo.getMap()->calculateAvailableNodes();
	std::vector<Node*> availableNodes = stateInfo.getMap()->getAvailableNodes();
	std::vector<Node> validGoalNodes;
	Node temp, goal;
	std::string gender;
	for (int i = 0; i < stateInfo.getPreyCount(); i++) {
		gender = Animal::chooseGender();
		Prey preyi = Prey(&stateInfo);		
		prey.push_back(preyi);
		temp = availableNodes[rand() % availableNodes.size()];
		prey[i].setX(temp.getX());
		prey[i].setY(temp.getY());
		validGoalNodes = prey[i].getValidGoalNodes(&stateInfo);
		goal = validGoalNodes.at(rand() % validGoalNodes.size());
		prey[i].setPath(finder.path(prey[i], goal, stateInfo.getMap()));
	}
	stateInfo.setPrey(&prey);
	
	// Retrieving information from simulation info due to the updates from FSM below
	//TESTTING IF THIS MAKES A DIFFERENCE
	//updatePrey(stateInfo.getPrey(), stateInfo.getMap(), stateInfo.getPause());
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width; screenHeight = height;           // to ensure the mouse coordinates match 
														  // we will use these values to set the coordinate system

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	gluOrtho2D(-zoom, zoom, -zoom, zoom);           // set the coordinate system for the window


	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void processKeys()
{
	if (keys[VK_UP] && myMap.getStart()) {
		cameraY -= camera;
	}
	if (keys[VK_DOWN] && myMap.getStart()) {
		cameraY += camera;
	}
	if (keys[VK_LEFT] && myMap.getStart()) {
		cameraX += camera;
	}
	if (keys[VK_RIGHT] && myMap.getStart()) {
		cameraX -= camera;
	}
	if (keys['X'] && zoom < 50 && myMap.getStart()) {
		zoom += camera;
		reshape(1500, 1000);
	}
	if (keys['Z'] && zoom >10 && myMap.getStart()) {
		zoom -= camera;
		reshape(1500, 1000);
	}
	if (keys['C'] && myMap.getStart()) {
		if (scroll > 0 && survivor || island) {
			scroll--;
		}
	}
	if (keys['D'] && myMap.getStart()) {
		if (scroll < 91 && survivor || island) {
			scroll++;
		}
	}
}


void splash() {
	//WazAdded new position values  for lots of images
	float newSW = (screenWidth / 1500);
	drawImg(blur, -15, 15, -15, 15, 1.0);
	drawImg(yellowBorder, -14.5, 14.5, -14.5, 14.5, 1);
	drawImg(titleTex, -9, 9, 9, 11, 1.0);

	drawImg(deerTex, -7.3, -4.8, 5.5, 8, 1.0);
	drawImg(preyText, -4.1, -1.1, 6.5, 7.5, 1.0);
	drawImg(previous, -0.8, 1.2, 6, 8, 1.0);
	number(1.2, 3.2, 6, 8, (deerNo / 10));
	number(3.3, 5.3, 6, 8, deerNo - (int(deerNo / 10) * 10));
	drawImg(nextNum, 5.3, 7.3, 6, 8, 1.0);

	drawImg(mapTex, -7.4, -4.7, 2.4, 4.6, 1.0);
	drawImg(mapText, -4.1, -1.1, 2.9, 4.1, 1.0);
	drawImg(previous, -0.8, 1.2, 2.5, 4.5, 1.0);
	number(1.2, 3.2, 2.5, 4.5, (mapSize / 10));
	number(3.3, 5.3, 2.5, 4.5, mapSize - (int(mapSize / 10) * 10));
	drawImg(nextNum, 5.3, 7.3, 2.5, 4.5, 1.0);

	drawImg(treeSpriteSheet, -7.3, -4.8, -1.0, 1.5, 1.0, 0.0, 0.5, 0.0, 1.0, 0.25, 1.0, 0.25, 0.5);
	drawImg(treeText, -4.4, -0.9, -0.5, 0.7, 1.0);
	drawImg(previous, -0.8, 1.2, -1.0, 1, 1.0);
	number(1.2, 3.2, -1.0, 1, (treeNo / 10));
	number(3.3, 5.3, -1.0, 1, treeNo - (int(treeNo / 10) * 10));
	drawImg(nextNum, 5.3, 7.3, -1.0, 1, 1.0);
	
	//WazAdded to determine what texture to draw
	if(drySeasonBool) drawImg(dryTex, -7.3, -4.8, -4.5, -2.0, 1.0);
	else drawImg(monsoonTex, -7.3, -4.8, -4.5, -2.0, 1.0);

	drawImg(normalTemp, -3.8, 0.2, -4.3, -2.2, normalT);
	drawImg(tropicTemp, 1.2, 7.1, -4.3, -2.2, tropicT);

	if(newSW * mouse_x > 335 && newSW * mouse_x < 1104 && mouse_y > 125 && mouse_y < 250) drawImg(go2, -7.3, 7.1, -11.25, -7.75, 1.0);
	else drawImg(go, -7.3, 7.1, -11.25, -7.75, 1.0);

	//WazAdded validation on deerNo increasing to above 99 and below 0 (Loops)
	//WazAdded validation on ratios needs to be done
	// HAIGS if (mouse_x < 500 * newSW && mouse_x >  464 * newSW && mouse_y > 724 && mouse_y < 757 && LeftPressed && mapSize > 10)
	if (newSW * mouse_x > 729 && newSW * mouse_x < 781 && mouse_y > 717 && mouse_y < 752 && leftPressed) { if (!clickToken) { clickToken = true; if (--deerNo > 0) {} else deerNo = 99; } } 
	else if (newSW * mouse_x > 1043 && newSW * mouse_x < 1096 && mouse_y > 717 && mouse_y < 752 && leftPressed) { if (!clickToken) { clickToken = true; if (++deerNo < 11) {} else deerNo = 1; } } 
	else if (newSW * mouse_x > 729 && newSW * mouse_x < 781 && mouse_y > 600 && mouse_y < 635 && leftPressed) { if (!clickToken) { clickToken = true; if ((mapSize -= 10) >= 30) {} else mapSize = 90; } }
	else if (newSW * mouse_x > 1043 && newSW * mouse_x < 1096 && mouse_y > 600 && mouse_y < 635 && leftPressed) { if (!clickToken) { clickToken = true; if ((mapSize += 10) < 91) {} else mapSize = 30; } }
	else if (newSW * mouse_x > 729 && newSW * mouse_x < 781 && mouse_y > 484 && mouse_y < 517 && leftPressed) { if (!clickToken) { clickToken = true; if (--treeNo > 0) {} else treeNo = 10; } }
	else if (newSW * mouse_x > 1043 && newSW * mouse_x < 1096 && mouse_y > 484 && mouse_y < 517 && leftPressed) { if (!clickToken) { clickToken = true; if (++treeNo < 11) {} else treeNo = 1; } }

	else if (newSW * mouse_x > 560 && newSW * mouse_x < 759 && mouse_y > 364 && mouse_y < 415 && leftPressed) { if (!clickToken) { clickToken = true; // Normal clicked, tropic/monsoon season more transparent
		drySeasonBool = true;
		normalT = 1.0; 
		tropicT = 0.5;	
		} } 
	else if (newSW * mouse_x > 810 && newSW * mouse_x < 1110 && mouse_y > 375 && mouse_y < 415 && leftPressed) { if (!clickToken) { clickToken = true; // Tropic clicked, normal/dry season more transparent
			drySeasonBool = false;
			normalT = 0.5; 
			tropicT = 1.0;
		} } 
	else if (newSW * mouse_x > 335 && newSW * mouse_x < 1104 && mouse_y > 125 && mouse_y < 250 && leftPressed) { if (!clickToken) { setUpSim(); } }
	
	if (!leftPressed) { clickToken = false; }

}

//GRAPHS
void calculateGraph(int G, int val, float div) {
	if (graphs[G][dayCount] == 0 && !stateInfo.getPause()) {
		int graphValue = val;
		graphs[G][dayCount] = ceil(graphValue / div);
	}
}

void HeatMap(float XG, float YG, float colourVal1, float colourVal2, float colourVal3) {
	float newZoom1 = (zoom / 15);
	float newZoom2 = (zoom / 30);
	glBegin(GL_POLYGON);
	glColor3f(colourVal1, colourVal2, colourVal3);
	glVertex2f((XG - 29.5) * newZoom2, -60 * newZoom2 + (YG) * newZoom2 + scroll * newZoom1);
	glVertex2f((XG - 29.5) * newZoom2, -60 * newZoom2 + ((YG + 0.4515)) * newZoom2 + scroll * newZoom1);
	glVertex2f(((XG + 0.3005) - 29.5) * newZoom2, -60 * newZoom2 + (YG + 0.4515) * newZoom2 + scroll * newZoom1);
	glVertex2f(((XG + 0.3005) - 29.5) * newZoom2, -60 * newZoom2 + YG * newZoom2 + scroll * newZoom1);
	glEnd();
	//drawImg(movementHeatMap, -14, -8.5, 121 + scroll - 20 + 4, 121 + scroll - 20 + 5, 1.0);
}

void Graphs() {

	calculateGraph(0, stateInfo.getHealth()->getHunger(), 30);
	calculateGraph(1, myMap.getTreeCount(), 1.5); //100% workarno
	calculateGraph(2, myMap.getDegrees(), 0.35); //100% done
	calculateGraph(3, mySurvivor.getWeight(), 0.77);
	calculateGraph(4, stateInfo.getPreyCount(), 0.069);
	calculateGraph(5, mySurvivor.getWaterLevel() - mySurvivor.getDehydration()+1360,11);
	calculateGraph(6, mySurvivor.getWood(), 0.069);
	
	calculateGraph(7, stateInfo.getHealth()->getHunger(), 50);

	if (island) {
		drawGraphs(treeGraph, 1, 1, treeCountH); //100% working treeC
		drawGraphs(tempGraph, 11, 2, temptex); // temperature
		drawGraphs(preyGraph, 21, 4, preyCount); // 50% Prey
	}
	if (survivor) {
		drawGraphs(weightgraph, 1, 3, weightTex); //Weight
		drawGraphs(graphT, 11, 5, hydr); //water
		drawGraphs(preyGraph, 21, 6, woodHead); //Wood

		drawImg(heatmap, -14.75, -7.24, 5.3 + scroll - 31 + 7, 5.3 + scroll - 31 + 8.5, 1.0);
		int count1 = 0;
		int count2 = 0;
		float H = 0.4515;
		float W = 0.3005;
		for (float i = H*(myMap.getNumCols()-1); i > 0; i -= 0.4515) {
			for (float e = 0; e < W*(myMap.getNumRows() - 1); e += 0.3005) {
				float colValr = 1.0 - myMap.getNode(count1, count2)->getHeatR();
				float colValg = 1.0 - myMap.getNode(count1, count2)->getHeatG();
				float colValb = 1.0 - myMap.getNode(count1, count2)->getHeatB();
				HeatMap(e, i, colValg, colValr, colValb);
				count1++;
			}
			count1 = 0;
			count2++;
		}
	}

	if ((int(stateInfo.getHours()) % 2) == 0) {
		if (graphLock) {
			dayCount++;
			//myHealth.setHunger(myHealth.getHunger() - 250);
			graphLock = false;
		}
	}
	else {
		graphLock = true;
	}
}

//UPDATE FUNCTION, CALLED EVERY FRAME
void display() {
	glClearColor(0.0f, 0.0f, lighting, 1.0f); // Clear the background of our window to red  
	glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on) 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!myMap.getStart()) {
		//setUpSim(); //Remove when uncommenting splash();
		
		
		splash();
		
		//std::cout << mouse_x << "   " << mouse_y << "      " << ((screenWidth / 1500) * mouse_x) << endl;
	}
	else {
		//Update
		stateInfo.update(stateInfo.getSurvivor(), stateInfo.getMap(), stateInfo.getFire(), stateInfo.getShelter(), stateInfo.getWater(), stateInfo.getHealth(), stateInfo.checkNightBool(),
			stateInfo.getMove(), stateInfo.getPause(), stateInfo.getNumMoves(), stateInfo.getHours(), stateInfo.getMinutes());

		drawMap();
		character();
		drawPrey();
		
		
		
		drawHUD(); //WazAdded Swapped this before Graphs();
		
		pauseSim(); //WazAdded swapped this before drawHUD();
		resourceHUD(); //WazAdded Swapped this after drawHUD();
		drawSimulationDayTimeTemp(); //WazAdded renamed
		deltaTime();
		

		//X and Y Coordinants
		//std::cout << mouse_x << "   " << mouse_y << "      " << ((screenWidth / 1500) * mouse_x) << endl;
		// Mouse window Coordinants
		//std::cout << (mouse_x / 33.33333) - 15 << "   " << (mouse_y / 33.33333) - 15 << endl;
	}
}


/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

													   //win32 global variabless
HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


							/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done = false;								// Bool Variable To Exit Loop

	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
	moveConsoleWindow((GetSystemMetrics(SM_CXSCREEN) - screenWidth - 35) / 8, (GetSystemMetrics(SM_CYSCREEN) - screenHeight - 100) / 7);

	// Create Our OpenGL Window
	if (!CreateGLWindow("Island Survivor Simulator", screenWidth, screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if (keys[VK_ESCAPE])
				done = true;

			processKeys();

			display();					// Draw The Scene
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}
	break;

	case WM_SIZE:								// Resize The OpenGL Window
	{
		reshape(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	break;

	case WM_LBUTTONDOWN:
	{
		mouse_x = LOWORD(lParam);
		mouse_y = screenHeight - HIWORD(lParam);
		leftPressed = true;
	}
	break;

	case WM_LBUTTONUP:
	{
		leftPressed = false;
	}
	break;

	case WM_MOUSEMOVE:
	{
		mouse_x = LOWORD(lParam);
		mouse_y = screenHeight - HIWORD(lParam);
	}
	break;
	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		keys[wParam] = true;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}
	break;
	case WM_KEYUP:								// Has A Key Been Released?
	{
		keys[wParam] = false;					// If So, Mark It As FALSE
		return 0;								// Jump Back
	}
	break;

	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*/

bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

																	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();

	return true;									// Success
}
void moveConsoleWindow(int w, int h)
{
	_COORD coord;
	coord.X = w;
	coord.Y = h;
	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = h - 1;
	Rect.Right = w - 1;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, coord);
	SetConsoleWindowInfo(Handle, TRUE, &Rect);
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, screenWidth + 1, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}