#include <iostream>
#include <string>
#include <chrono>
#include <locale>
#include <iomanip>
#include <iostream>

using namespace std::chrono;
using std::endl;

// settings
const int width = 3;
const int height = 3;
const int start = 1;
const int limit = 11;

// init
int x;
int y;
int prx;
int pry;
int loc = 0;
int wins = 0;
int tosolve;
int range = limit + start;
unsigned long long int iterations = 0;
int count = (width * height);
int square[width * height] = { start };
int unique;

class comma_numpunct : public std::numpunct<char>
{protected:
	virtual char do_thousands_sep() const
	{ return ','; }
	virtual std::string do_grouping() const
	{ return "\03"; } };

int countDistinct(int arr[], int n)
{	int res = 1;
	for (int i = 1; i < n; i++) {
		int j = 0;
		for (j = 0; j < i; j++)
			if (arr[i] == arr[j])
				break;
		if (i == j)
			res++; }
	return res; }

// set locale hack to use commas in iostream
std::locale comma_locale(std::locale(), new comma_numpunct());

int main()
{
	// calculate how many solves to find, thanks Zaslavsky :)
	int t = range;
	int tmod = t % 12;
	switch(tmod)
	{
		case 0:
		case 2:
		case 6:
		case 8: 
			tosolve = ((t * t * t) - 16 * t * t + 76 * t - 96) / 6;
			break;
		case 1: 
			tosolve = ((t * t * t) - 16 * t * t + 73 * t - 58) / 6;
			break;
		case 3:
		case 11: 
			tosolve = ((t * t * t) - 16 * t * t + 73 * t - 102) / 6;
			break;
		case 4:
		case 10: 
			tosolve = ((t * t * t) - 16 * t * t + 76 * t - 112) / 6;
			break;
		case 5:
		case 9: 
			tosolve = ((t * t * t) - 16 * t * t + 73 * t - 90) / 6;
			break;
		case 7: 
			tosolve = ((t * t * t) - 16 * t * t + 73 * t - 70) / 6;
			break;
		default: 0;
			break;
	};

	// start timer
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	// main loop
	while (square[8] != limit) {
		int sums[width + height + 2] = { 0 };
		square[0]++;
		iterations++;
		// check if square has equal columns before continuing
		if ((square[0] + square[1] + square[2] == square[3] + square[4] + square[5]) &&
			(square[0] + square[1] + square[2] == square[6] + square[7] + square[8]) &&
			(square[0] + square[3] + square[6] == square[2] + square[5] + square[8]) 
			)
		{
			//int n = sizeof(square) / sizeof(square[0]);
			//unique = countDistinct(square, n);
			//if (unique == count)
			//{
				// columns
				x = 0;
				y = 0;
				while (x < width)
				{
					while (y < count)
					{
						sums[x] += square[y];
						y += width;
					}
					x++;
					y = x;
				}
				// rows
				x = 0;
				y = 0;
				while (y < height)
				{
					while (x < width)
					{
						sums[y + width] += square[x + (width * y)];
						x++;
					}
					y++;
					x = 0;
				}
				// attempt at breaking before full sums[] is complete [successful]
				//if (sums[1] == sums[2])

				// check if square has unique values
				int n = sizeof(square) / sizeof(square[0]);
				unique = countDistinct(square, n);

				if (unique == count)
				{
					// diagonal right to left
					y = 0;
					while (y < height)
					{
						sums[width + height] += square[(width * (y + 1)) - (y + 1)];
						y++;
					}

					// diagonal left to right
					y = 0;
					while (y < height)
					{
						sums[width + height + 1] += square[(width * y) + y];
						y++;
					}

					// test sums for unique
					int n = sizeof(sums) / sizeof(sums[0]);
					unique = countDistinct(sums, n);

					if (unique == 1)
					{
						// WIN! print square, n, progress
						prx = 0;
						pry = 0;
						while (pry < height)
						{
							while (prx < width)
							{
								std::cout << square[prx + (pry * height)] << " ";
								prx++;
							}
							std::cout << endl;
							pry++;
							prx = 0;
						}
						std::cout << "n=" << sums[0] << endl;
						wins++;
						float percentComplete = ((float)wins / (float)tosolve) * 100;
						std::cout << "Solved " << wins << " of " << tosolve << endl 
							<< (int)percentComplete << "%" << endl << endl;
					}
				}
			// reset sums
			memset(sums, 0, sizeof(sums));
			//for (int i = 0; i < (width + height + 2); i++) { sums[i] = 0; }
		}
		// cascade the square
		while (loc < count)
		{
			if (square[loc] >= limit)
			{
				square[loc] = start;
				loc++;
			}
			else
			{
				if (loc != 0) { square[loc]++; }
				break;
			}
		}
		loc = 0;
		// loop, increment, continue
	}
	// stop timer, print diagnostics
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> total_time = duration_cast<duration<double>>(t2 - t1);

	std::cout << width << "x" << height << " square from " << start << " to " << limit << endl;
	std::cout << "Total solved: " << wins << endl;
	std::cout << "Total time: " << total_time.count() << " s" << endl;
	std::cout.imbue(comma_locale);
	std::cout << "Finished with ";
	std::cout << std::setprecision(2) << std::fixed << iterations << " cycles" << endl;
	std::cout << iterations / total_time.count() << " cycles per second" << endl;

}

