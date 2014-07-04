#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class Cab
{
	bool running_;
	int cows_,
		bulls_;

	void start()
	{
		running_ = true;
	}

	void stop()
	{
		running_ = false;
	}

	int genNum()
	{
		int min,
			max,
			output;

		min = 1023;
		max = 9876;
		output = 0;

		
		while(hasDupes(output))
		{
			srand(time(NULL));
			output = min + ( rand() % ( max - min + 1 ) );
		}

		return output;

	}

	bool hasDupes ( int n) {
	    int i,
	    	j, 
	    	used[4];

	   	for (i = 0; i < 4; i++)
	   	{
	   		used[i] = n % 10;
	   		n /= 10;
	   	}

	   	j = 1;
	   	for (i = 0; i < 4; i++)
	   	{
	   		while(j < 4)
	   		{
	   			if(used[i] == used[j])
	   			{
	   				return true;
	   			}
	   		j++;  
	   		}
	   		j = i+2;
	   	}
	   	
	    return false;
	}

	void compare(int firstNum, int input)
	{
		int firstNumArr[4],
			inputArr[4],
			i,j;

		cows_ = 0;
		bulls_ = 0;

		for (i = 0; i < 4; i++)
		{
			firstNumArr[i] = firstNum % 10;
			inputArr[i] = input % 10;
			firstNum /= 10;
			input /= 10;
		}

		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (i == j && firstNumArr[i] == inputArr[j])
					bulls_++;
				else if(firstNumArr[i] == inputArr[j])
					cows_ ++;
			}
		}
	}

	int getCows()
	{
		return cows_;
	}

	int getBulls()
	{
		return bulls_;
	}

public:
	Cab()
	{
		start();
	}


	void run()
	{	
		int firstNum,
			input,
			turns;

		firstNum = genNum();
		turns = 0;

		cout << endl << endl;
		cout << ",-*'^'~*-.,_,.-*~ ç¤Ψ§ åñď βɥȽȽ§ ~*-.,_,.-*~'^'*-," << endl << endl;
		while(running_)
		{
			cout << endl;
			cout << "Enter your guess: ";
			cin >> input;
			if (cin.fail())
			{
				cout << "Input must be an integer" << endl;
				return ;			
			}
			while(input < 1023 || input > 9876 || hasDupes(input) || input < 0)
			{
				cout << endl;
				cout << "Input should be a positive four digit integer with no duplicate digits." << endl;
				cout << "Please try again." << endl;
				cout << "Enter your guess: ";
				cin >> input;

			}

			turns++;
			if(firstNum == input)
			{
				cout << "YOU WIN in " << turns << " turns!!" << endl;
				stop();
			}
			else
			{
				compare(firstNum, input);
				if(getCows())
					cout << "Cows found: " <<  getCows() << endl;
				if(getBulls())
					cout << "Bulls found: " << getBulls() << endl << endl;
			}
		}
	}
};

int main(int argc, char const *argv[])
{
	Cab cab;

	cab.run();
	 	
 	return 0;
} 