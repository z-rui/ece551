#include <stdio.h>
#include <stdlib.h>

struct _retire_info {
	int months;
	double contribution;
	double rate_of_return;
};

typedef struct _retire_info retire_info;

double calculations(int startAge, double balance, retire_info info)
{
	int year, month;

	year = startAge / 12;
	month = startAge % 12;

	while (info.months--) {
		printf("Age %3d month %2d you have $%.2lf\n", year, month, balance);
		balance = (1.0 + info.rate_of_return) * balance + info.contribution;
		if (++month == 12) {
			++year;
			month = 0;
		}
	}
	return balance;
}

void retirement (int startAge, double initial, retire_info working, retire_info retired)
{
	// working
	initial = calculations(startAge, initial, working);
	// retired
	calculations(startAge + working.months, initial, retired);
}

int main()
{
	retire_info working = {489, 1000, 0.045/12};
	retire_info retired = {384, -4000, 0.01/12};

	retirement(327, 21345, working, retired);

	return 0;
}
