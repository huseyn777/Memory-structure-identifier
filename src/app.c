#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int recursive(int number)
{
	if(number == 0)
	   return 0;
	else
	   recursive(number - 1000);
}




int main()
{
	printf("Pid : %d\n", getpid());
	int *ptr;
	int number = 1000000;
	ptr = (int*) malloc(number * sizeof(int));
		if(ptr == NULL)
		printf("Error1\n");
	 //for(int i = 0; i < number; ++i)
         //printf("First address 0x%lx\n",ptr + i);



	
	while(1)
	{
		sleep(10);
		printf("10 sec passed\n");
      		number = number + 1000000;
		ptr = realloc(ptr,number * sizeof(int));
		if(ptr == NULL)
			printf("Error2\n");
		//for(int i = 0; i < number; ++i)
         	//printf("Second address 0x%lx, number: %d\n",ptr + i, number);
		recursive(number);
		if(number == 10000000)
			break; 
	}
	free(ptr);
	return 0;
}
