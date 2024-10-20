#include <unistd.h>

void fizz(void)
{
	int s = 1;
	while (s >= 0 && s <= 100)
	{
		write(1, &s, 1);
		s++;
		if ( s % 5 == 0)
		{
			write(1, "fizz", 4);

			if ( s % 7 == 0)
			{
				write(1, "buzz", 4);
					
					if( s % 5 == 0 && s % 7 == 0 )
					{
						write(1, "fizzbuzz", 8);
					}
					s = '0';
					s++;
			}
		}
	}
}
int main(void)
{
	fizz();
}
