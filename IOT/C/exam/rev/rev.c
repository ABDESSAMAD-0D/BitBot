
#include <unistd.h>

void rev(char *fu)
{
	int	a = 0;
	while (fu[a] != 0)
	{
		a++;
	}
	a--;

	while (fu[a] > 0)
	{
		write(1, &fu[a], 1);
		a--;
	}
	write(1, "\n", 1);
}
int main(int ac, char **av)
{
	if ( ac >= 2)
	{
		rev(av[1]);
	}
}
