#define NUM_CARDS 52
#define WAR_SKIP 4
#define MAX_TURNS 10000000

typedef struct {
	int cards[2*NUM_CARDS];
	int pos;
	int len;
} Deck;

int generate(Deck *deck);

int empty_deck(Deck *deck);

int randomize(Deck *deck);

int sort(int *cards, int len);

int split(Deck *deck, Deck *p1, Deck *p2);

int play(Deck *p1, Deck *p2);

int battle(Deck *p1, Deck *p2);

void print_deck(Deck *deck);
