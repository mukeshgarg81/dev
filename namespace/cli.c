#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main (int argc, char *argv[])
{
	char input[] = " ";
	char *argument;
	while(strcmp(input, "quit")!= 0) {
		printf("$");
		scanf ("%s", input);

		if(strcmp(input,"clear") == 0) {
			printf("\e[1;1H\e[2J");
		}

		else if(strcmp(argv[1],"cd") == 0) {
			if(chdir(argv[2]) == -1) {
				printf("\n directory does not exists");
			}
		}

		else if(strcmp(input, "echo") == 0) {
			char str[50];
			scanf("%[^\n]+", str);
			printf(" %s", str);
		}
	}
}
