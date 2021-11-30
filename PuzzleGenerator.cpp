#include "PuzzleGenerator.h"
#include <limits>
#include <ctime>
#include <cmath>

using namespace std;

PuzzleGenerator::PuzzleGenerator(int _nRows, int _nColumns, int _minVal, int _maxVal)
	:nRows(_nRows), nColumns(_nColumns), minVal(_minVal), maxVal(_maxVal)
{}

Puzzle PuzzleGenerator::GeneratePuzzle()
{
	srand(time(0));
	timer.StartTimer();
	maxtime = 59.9;	// To make sure we don't exceed a minute
	
	double random_walk_time = 59.9;	// 5 seconds.
	
	//return RandomWalk(random_walk_time);	// Do a random walk for some time and return the solution.
	return SimulatedAnnealingSolver(random_walk_time);
	//return HillClimbingSolver(random_walk_time);
}

Puzzle PuzzleGenerator::RandomWalk(double timelimit)
{
	// A very simple function that start at a random configuration and keeps randomly modifying it
	// until t hits the time limit. Returns the best solution found so far.

	Puzzle p(nRows, nColumns, minVal, maxVal);	// Generate a random puzzle with the specified values.
	
	// Keep track of the best puzzle found so far (and its value).
	Puzzle bestPuzzle = p;			
	int bestValue = p.GetValue();
	
	// Keep track of the time so we don't exceed it.
	Timer t;
	t.StartTimer();
	
	// Loop until we hit the time limit.
	while (t.GetElapsedTime() < timelimit-0.1)	// To make sure that we don't exceed the time limit, we stop just before we hit the time limit.
	{
		// Generate a successor of p by randomly changing the value of a random cell 
		// (since we are doing a random walk, we just replace p with its successor).
		p = p.GetRandomSuccessor();	
		int value = p.GetValue();
		
		// Update the current best solution.
		if (value > bestValue)	// Calling GetValue() for the first time is costly
										// since the puzzle has to be evaluated first.
		{
			bestValue = value;	// Calling it a second time simply returns the value that was computed before.
			bestPuzzle = p;
		}
	}
	
	return bestPuzzle;
	
	// The following code is not executed in this function. It exists just as an example for getting all the successors of a puzzle.
	vector<Puzzle> successors;
	bestPuzzle.GetAllSuccessors(successors);
}


//first attempt at a local search algorithm, not as effective as simulate annealing
Puzzle PuzzleGenerator::HillClimbingSolver(double timelimit){
	

	Puzzle p(nRows, nColumns, minVal, maxVal);	// Generate a random puzzle with the specified values.
	// Keep track of the best puzzle found so far (and its value).

	//ensure p has a solution
	while(!p.HasSolution()){
			p = p.GetRandomSuccessor();
		}

	//set p to best puzzle
	Puzzle bestPuzzle = p;	

	//initialize a vector of successors
	vector<Puzzle> successors;

	//start the timer
	Timer t;
	t.StartTimer();

	//while it took less than 5 seconds
	while(t.GetElapsedTime() < timelimit-0.1){

		//get all successors
		bestPuzzle.GetAllSuccessors(successors);
		//set end to true for now and change it to false if we find a better puzzle
		bool end = true;

		//go through all successors and if you find a better puzzle make it the best and move on
		for(int i=0; i<successors.size(); i++){
			if(successors[i].GetValue()>bestPuzzle.GetValue()){
				bestPuzzle = successors[i];
				end = false;
			}
		}
		//if you don't find a better puzzle end now cuz we are at a local max
		// if(end){
		// 	return bestPuzzle;
		// }
	}

	return bestPuzzle;

}

Puzzle PuzzleGenerator::SimulatedAnnealingSolver(double timelimit){

	Puzzle p(nRows, nColumns, minVal, maxVal);	// Generate a random puzzle with the specified values.

	// Keep track of the best puzzle found so far (and its value).
	Puzzle bestPuzzle = p;	
	int bestValue = p.GetValue();

	cout<<"initial value of puzzle "<<bestValue<<endl<<endl;
	
	//start the timer
	Timer t;
	t.StartTimer();

	//continue looking for new puzzles while we are within the timelimit
	while(t.GetElapsedTime() < timelimit-0.1){
		
		//choose a random successor 
		Puzzle proposal = bestPuzzle.GetRandomSuccessor();

		//calculate probability that we take the puzzle
		//temperature schedule is total time - elapsed time
		//-delta E = propsal value - best value
		//probability is e^(-deltaE/Temp)
		double probability = exp( (proposal.GetValue()-bestValue) / (60-t.GetElapsedTime()) );

		//compare that to a random number between 0 and 1
		double random = rand()/( (double) RAND_MAX);

		// if it is a good choice take it right away
		if(proposal.GetValue()>bestValue){
			bestPuzzle = proposal;
			bestValue = proposal.GetValue();
		}
		//if it is a "bad choice" we might still take it depending on the probability
		//if that probability is better than the random number take the puzzle next
		if(probability>random){
			bestPuzzle = proposal;
			bestValue = proposal.GetValue();
		}

	
	}

		return bestPuzzle;
	}


