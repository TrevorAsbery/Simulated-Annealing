#include <stdio.h>
#include <stdlib.h>
#include <climits>
#include "Puzzle.h"
#include "Timer.h"

class PuzzleGenerator{
public:
	PuzzleGenerator(int nRows, int nColumns, int minVal, int maxVal);
	Puzzle GeneratePuzzle(); 
	Puzzle RandomWalk(double timelimit);	// Time limit given in seconds.
	Puzzle SimulatedAnnealingSolver(double timelimit);
	Puzzle HillClimbingSolver(double timelimit);
	
private:
	int nRows, nColumns, minVal, maxVal;
	Timer timer;
	double maxtime;
};
