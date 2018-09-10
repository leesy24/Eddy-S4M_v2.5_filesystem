#include <stdio.h>

int main()
{
	char dps;
	dps = 0x00;
	dps |= 0x18;
	printf("%X\n", dps);
	
	dps = dps | 0x18;
	printf("%X\n", dps);

	switch (dps & 0x18)
	{
		case 0x18:
			printf("yes\n");
		break;
		default:
			printf("no\n");
	}

	char test[16];
	printf("%d\n", sizeof(test));
}

