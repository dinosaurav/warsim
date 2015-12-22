#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "warsim.h"

int verbose = 1;
int super_verbose = 0;

int main(int argc, char *argv[]) {
	int n = 0;
	if (argc == 1) {
		n = 1;
	} else if (argc >= 3 && !strcmp(argv[1], "-n")) {
		if (argc == 4 && !strcmp(argv[3], "-V")) {
			verbose = 0;
		} 
		n = atoi(argv[2]);
	} else {
		perror("USAGE: warsim [-n #trials] [-v] [-V]");
		exit(1);
	}

	srand(time(NULL));
	int i;
	int sum = 0;
	int success = 0;
	int val;
	Deck deck,p1,p2;
	for (i = 0; i < n; i++) {
		generate(&deck);
		randomize(&deck);
		split(&deck, &p1, &p2); 
		val = play(&p1,&p2);
		if (val < 0) {
			if (verbose) {
			printf("%d: failed\n", i);
			}
		} else {
			sum += val;
			if (verbose) {
			printf("%d: %d\n",i,val);
			}
			success++;
		}
	}
	if (success > 0) {
		printf("\nAverage of %d/%d successes: %0.2f\n",success,n,sum/(0.0f+success));
	}
}

int generate(Deck *deck) {
	int i,j;
	for (i = 0; i < 4; i++) {
		for (j = 2; j < 15; j++) {
			deck->cards[13*i+j-2] = j;
		}
	}
	deck->len = NUM_CARDS;
	deck->pos = 0;
	return 1;
}

int empty_deck(Deck *deck) {
	deck->pos = 0;
	deck->len = 0;
	return 1;
}

int randomize(Deck *deck) {
	int i, temp, randi;
	for (i = deck->len-1; i > 0; i--) {
		randi = rand()%(i+1);
		temp = deck->cards[randi];
		deck->cards[randi] = deck->cards[i];
		deck->cards[i] = temp;
	}
	return 0;
}

int sort(int *cards, int len) {
	return 1;
}

int split(Deck *deck, Deck *p1, Deck *p2) {
	p1->len = deck->len/2;
	p2->len = deck->len - p1->len;
	p1->pos = 0;
	p2->pos = 0;
	memcpy(p1->cards,deck->cards,p1->len*sizeof(int));
	memcpy(p2->cards,deck->cards+p1->len,p2->len*sizeof(int));

	return 1;
}

int play(Deck *p1, Deck *p2) {
	int turns = 0;
	if (verbose) {
	printf("Deck1 ");
	print_deck(p1);
	printf("Deck2 ");
	print_deck(p2);
	}

	for (turns = 0; turns < MAX_TURNS && p1->len > 0 && p2->len > 0; turns++) {
		if (p1->pos >= NUM_CARDS/2) {
			memmove(p1->cards, p1->cards+p1->pos, p1->len);
			p1->pos = 0;
		}
		if (p2->pos >= NUM_CARDS/2) {
			memmove(p2->cards, p2->cards+p2->pos, p2->len);
			p2->pos = 0;
		}
		//printf("%d vs %d",p1->cards[p1->pos],p2->cards[p2->pos]);
		if (0 > battle(p1,p2)) {
			return -1;
		}
		//printf(" -- %d to %d left\n",p1->len, p2->len);
		if (turns%10000000 == 0 && turns && verbose) {
			printf("-----------\n---ON TURN: %d\n-----------\n",turns);
		}
	}
	if (turns >= MAX_TURNS) {
		return -1;
	}

	return turns;
}

int battle(Deck *p1, Deck *p2) {
	int a = p1->cards[p1->pos];
	int b = p2->cards[p2->pos];
	int skipped = 1;
	while (a == b && skipped < NUM_CARDS) {
		skipped += WAR_SKIP;
		if (skipped > p1->len) {
			a = p1->cards[p1->pos+p1->len];
		} else { 
			a = p1->cards[p1->pos+skipped];
		}
		if (skipped > p2->len) {
			b = p2->cards[p2->pos+p2->len];
		} else {
			b = p2->cards[p2->pos+skipped];
		}
	}
	if (skipped >= NUM_CARDS) { // if there is a failure
		return -1;
	}
	if (a > b) {
		p1->len += skipped;
		p2->len -= skipped;
	} else if (b > a) {
		p1->len -= skipped;
		p2->len += skipped;
	}
	if (p1->len <= 0 || p2->len <=0) {
		return 0;
	}
	Deck pool;
	memcpy(pool.cards,p2->cards+p2->pos,skipped);
	memcpy(pool.cards+skipped,p1->cards+p1->pos,skipped);
	pool.len = skipped*2;
	//randomize(&pool);
	if (a > b) {
		memcpy(p1->cards+p1->pos+p1->len-skipped,pool.cards,skipped*2);
	} else if (b > a) {
		memcpy(p2->cards+p2->pos+p2->len-skipped,pool.cards,skipped*2);
	}
	p1->pos += skipped;
	p2->pos += skipped;
	return 1;
}

void print_deck(Deck *deck) {
	int i;
	printf("length: %d\nDeck: ",deck->len);
	for (i = 0; i < deck->len; i++) {
		printf("%d ",deck->cards[i+deck->pos]);
	}
	printf("\n");
}
