#include <unistd.h>

void rot(char *str)
{
	int a = 0;
	while ( str[a] != '\0')
	{
		if ((str[a] >= 'a' && str[a] <= 'y') || (str[a] >= 'A' && str[a] <= 'Y'))
		
			str[a] +=  1;	
		
		else if (str[a] == 'z' || str[a] == 'Z')
		
			str[a] -= 25;

			write(1, &str[a], 1);
			a++;
	
	}
}
int main(int ac, char **av)
{
	if (ac == 2)
	{
		rot(av[1]);
	}
	
}
