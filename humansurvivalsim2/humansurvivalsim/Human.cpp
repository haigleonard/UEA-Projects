#include "Model/Human.h"
#include <iostream>


/********************************************************************************
 * Default Constructor - setting fields of a Human object to default values:
 * default health, 176lbs (80kg) in weight, basic requirement of water and zero
 * millilitres/litres consumed so far.
 *******************************************************************************/
Human::Human()
{
	Human::health = new Health();
	Human::weight = 176;
	Human::calculateWaterRequired();
	Human::waterConsumed = 0;
}

/********************************************************************************
* Deconstructor
********************************************************************************/
Human::~Human()
{
}

/********************************************************************************
 * Assigning an initial/new weight for a human.
 * @param weight - an integer parameter that should be the new weight in kilograms
 * for a Human object.
 ********************************************************************************/
void Human::setWeight(int weight) 
{
	Human::weight = weight * 0.453592;
}

/********************************************************************************
* A method that retrieves the current assigned weight to the Human object.
* @return Returns an integer value that refers to the previously assigned weight
* in kilograms.
********************************************************************************/
 int Human::getWeight()
 {
	 return Human::weight;
 }

/********************************************************************************
*
*******************************************************************************/
void Human::calculateWaterRequired()
{

	float waterInLitres = (Human::weight * 0.5) * 0.0295735;
	float formattedResult = round(waterInLitres * 10.0) / 10.0;
	
	Human::waterRequired = 0;
}

/********************************************************************************
*
*******************************************************************************/
int Human::getWaterRequired()
{
	return Human::waterRequired;
}

/********************************************************************************
* A method that will update the amount of water consumed by the Human object
(survivior), it will be a set portion every time of 230 (ml) that was determined
* by research.
*******************************************************************************/
void Human::updateWaterConsumed()
{
	Human::waterConsumed += 230;
}

/********************************************************************************
*
*******************************************************************************/
int Human::getWaterConsumed()
{
	return Human::waterConsumed;
}

Health* Human::getHealth()
{
	return Human::health;
}


