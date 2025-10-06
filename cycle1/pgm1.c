#include <stdio.h>
#include <string.h>

char result[20][20];
char copy[20];
char states[20][20];

void add_state(char a[20], int i){
	strcpy(result[i], a);
}

void display(int n){
	int k = 0;
	printf("\nEpsilon closure of %s = { ", copy);
	while(k < n){
		printf(" %s", result[k]);
		if (k < n-1) printf(",");
		k++;
	}
	printf(" } \n");
}

int main(){
	FILE* ipfile;
	ipfile = fopen("input.txt","r");
	if (!ipfile) {
		perror("fopen input.txt");
		return 1;
	}

	char state[20], state1[20], state2[20], input[20];
	int end, i = 0, n, k = 0;
	printf("\n Enter the no of states: ");
	if (scanf("%d", &n) != 1) {
		fprintf(stderr, "Invalid number\n");
		fclose(ipfile);
		return 1;
	}

	printf("\n Enter the states:");
	for(k = 0; k < n && k < 20; k++)
		scanf("%19s", states[k]);

	for(k = 0; k < n && k < 20; k++){
		i = 0;
		strcpy(state, states[k]);
		strcpy(copy, state);
		add_state(state, i++);
		while(1){
			end = fscanf(ipfile, "%19s%19s%19s", state1, input, state2);
			if (end != 3) break;
			if (strcmp(state, state1) == 0 && strcmp(input, "e") == 0) {
				add_state(state2, i++);
				strcpy(state, state2);
			}
		}
		display(i);
		rewind(ipfile);
	}
	fclose(ipfile);
	return 0;
}
