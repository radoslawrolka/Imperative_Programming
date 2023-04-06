#define _CTR_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
//--------------------------------------------------------------------------
//dzialanie kolejki cyklicznej

int out[2] = { 1, 1 }, len[2] = { 0 , 0 };

int cbuff_push(int cli_nr, int array[], int player) {
	if (len[player] >= 52) {
		return OVERFLOW;
	}
	array[(out[player] + len[player]) % 52] = cli_nr;
	len[player]++;
	return OK;
}
int cbuff_pop(int array[], int player) {
	if (len[player] == 0) {
		return UNDERFLOW;
	}
	len[player]--;
	return array[out[player]++];
}
int cbuff_state(int player) {
	return len[player];
}
void cbuff_print(int array[], int player) {
	for (int i = 0; i < len[player]; i++) {
		printf("%d ", array[(out[player] + i) % 52]);
	}
}
*/
int seed, flag = 0;
int game_version, maks_wars;
int deck[52], A[52], B[52];
int wars_happened = 0;
//--------------------------------------------------------------------------
// mechanika kart w rece (kolejka - queue)
int in[2] = { 0, 0 };

int queue_push(int card, int hand[], int player) {
	hand[in[player]] = card;
	in[player] += 1;
}
int queue_pop(int hand[], int player) {
	if (in[player] == 0) {
		return -1;
	}
	else {
		int result = hand[0];
		for (int i = 0; i < in[player] - 1; i++) {
			hand[i] = hand[i + 1];
		}
		in[player] -= 1;
		return result;
	}
}
int queue_state(int player) {
	return in[player];
}
void queue_print(int hand[], int player) {
	for (int i = 0; i < in[player]; i++) {
		printf("%d ", hand[i]);
	}
}

//---------------------------------------------------------------------------
//tasowanie kart
int rand_from_interval(int a, int b) {
	if (a > b) {
		return INT_MIN;
	}
	else if (b - a > RAND_MAX) {
		return INT_MAX;
	}
	else if (a == b) {
		return a;
	}
	else {
		return ((rand() % (b - a + 1)) + a);
	}
}
void swap(int array[], int i, int k) {
	int tmp = array[i];
	array[i] = array[k];
	array[k] = tmp;
}
void rand_permutation(int n, int array[]) {
	for (int i = 0; i < n; i++) {
		array[i] = i;
	}
	for (int i = 0; i < n - 1; i++) {
		int k = rand_from_interval(i, n - 1);
		swap(array, i, k);
	}
}

//--------------------------------------------------------------------------
//endgame - warunek koncowy
int if_end_war() {
	if ((queue_state(0) == 0) || (queue_state(1) == 0)) {
		printf("1 ");
		printf("%d %d", queue_state(0), queue_state(1));
		return 1;
	}
	return 0;
}

//---------------------------------------------------------------------------
//mechanika gry (game engine)
void play_1() {
	if (A[0] / 4 == B[0] / 4) {
		int c = queue_pop(A, 0);
		queue_push(c, A, 0);
		c = queue_pop(B, 1);
		queue_push(c, B, 1);
	}
	else if (A[0] / 4 > B[0] / 4) {
		int c = queue_pop(A, 0);
		queue_push(c, A, 0);
		c = queue_pop(B, 1);
		queue_push(c, A, 0);
	}
	else {
		int c = queue_pop(B, 1);
		queue_push(c, B, 1);
		c = queue_pop(A, 0);
		queue_push(c, B, 1);
	}
	wars_happened += 1;
	maks_wars -= 1;
}
void play_0() {
	flag = 0;
	if (A[0] / 4 > B[0] / 4) {
		int c = queue_pop(A, 0);
		queue_push(c, A, 0);
		c = queue_pop(B, 1);
		queue_push(c, A, 0);
		wars_happened += 1;
		maks_wars -= 1;
	}
	else if (A[0] / 4 < B[0] / 4) {
		int c = queue_pop(B, 1);
		queue_push(c, B, 1);
		c = queue_pop(A, 0);
		queue_push(c, B, 1);
		wars_happened += 1;
		maks_wars -= 1;
	}
	else {
		int buffA[52], buffB[52], inside[2] = { 0,0 };
		while (A[0] / 4 == B[0] / 4) {
			wars_happened += 1;
			maks_wars -= 1;
			int c = queue_pop(A, 0);
			queue_push(c, buffA, 0);
			c = queue_pop(B, 1);
			queue_push(c, buffB, 1);
			if (if_end_war == 0) {
				c = queue_pop(A, 0);
				queue_push(c, buffA, 0);
				c = queue_pop(B, 1);
				queue_push(c, buffB, 1);
			}
			else {
				flag = 1;
				break;
			}
			if (if_end_war() == 1) {
				flag = 1;
				break;
			}
		}
		if (flag == 1) {
			return;
		}
		else {
			if (A[0] / 4 > B[0] / 4) {
				int c = queue_pop(A, 0);
				queue_push(c, buffA, 0);
				c = queue_pop(B, 1);
				queue_push(c, buffB, 1);
				wars_happened += 1;
				maks_wars -= 1;
				for (int i = 0; i < inside[0]; i++) {
					int c = queue_pop(buffA, 0);
					queue_push(c, A, 0);
				}
				for (int i = 0; i < inside[1]; i++) {
					int c = queue_pop(buffB, 1);
					queue_push(c, A, 0);
				}
			}
			else {
				int c = queue_pop(B, 1);
				queue_push(c, B, 1);
				c = queue_pop(A, 0);
				queue_push(c, B, 1);
				wars_happened += 1;
				maks_wars -= 1;
				for (int i = 0; i < inside[1]; i++) {
					int c = queue_pop(buffB, 1);
					queue_push(c, B, 1);
				}
				for (int i = 0; i < inside[0]; i++) {
					int c = queue_pop(buffA, 0);
					queue_push(c, B, 1);
				}
			}
		}
	}


}

//---------------------------------------------------------------------------
int main(void) {
	//wejscie
	scanf("%d", &seed);
	srand((unsigned)seed); // set the seed
	scanf("%d %d", &game_version, &maks_wars);

	//wypelnianie talii kartami i tasowanie kart
	rand_permutation(52, deck);

	//wstawianie kart do reki graczy
	for (int i = 0; i < 26; i++) {
		queue_push(deck[i], A, 0);
		queue_push(deck[i + 26], B, 1);
	}

	//gra
	if (game_version == 1) {
		while (maks_wars > 0) {
			if (queue_state(0) == 0) {
				printf("3\n");
				queue_print(B, 1);
				return 3;
			}
			else if (queue_state(1) == 0) {
				printf("2 ");
				printf("%d", wars_happened);
				return 2;
			}
			else {
				play_1();
			}
		}
	}
	else {
		while (maks_wars > 0) {
			play_0();
			if (flag == 0) {
				return 0;
			}
			else {
				if (queue_state(0) == 0) {
					printf("3\n");
					queue_print(B, 1);
					return 3;
				}
				else if (queue_state(1) == 0) {
					printf("2 ");
					printf("%d", wars_happened);
					return 2;
				}
			}
		}
		printf("0 ");
		printf("%d %d", queue_state(0), queue_state(1));
	}
	return 0;
}
