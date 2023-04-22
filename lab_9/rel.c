#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation(pair*, int, pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair* relation, int size) {
	int flag1 = 0;
	int flag2 = 0;
	int x = 0;
	int y = 0;

	for (int i = 0; i < size; i++) {
		flag1 = 0;
		flag2 = 0;
		x = relation[i].first;
		y = relation[i].second;

		for (int j = 0; j < size; j++) {
			if (x == relation[i].first && x == relation[i].second) {
				flag1 = 1;
				break;
			}
		}
		if (flag1 == 0) {
			return 0;
		}
		for (int j = 0; j < size; j++) {
			if (y == relation[i].first && y == relation[i].second) {
				flag2 = 1;
				break;
			}
		}
		if (flag2 == 0) {
			return 0;
		}
	}
	return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair* relation, int size) {
	if (is_reflexive(relation, size) == 0)
		return 1;
	return 0;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair* relation, int size) {
	int x = 0;
	int y = 0;
	int flag = 0;

	for (int i = 0; i < size; i++) {
		flag = 0;
		x = relation[i].first;
		y = relation[i].second;
		for (int j = 0; j < size; j++) {
			if (y == relation[j].first && x == relation[j].second) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			return 0;
		}
	}
	return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair* relation, int size) {
	int x = 0;
	int y = 0;

	for (int i = 0; i < size; i++) {
		x = relation[i].first;
		y = relation[i].second;
		for (int j = 0; j < size; j++) {
			if (y == relation[j].first && x == relation[j].second) {
				if (x != y) {
					return 0;
				}
			}
		}
	}
	return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair* relation, int size) {
	int x = 0;
	int y = 0;

	for (int i = 0; i < size; i++) {
		x = relation[i].first;
		y = relation[i].second;
		for (int j = 0; j < size; j++)
		{
			if (y == relation[j].first && x == relation[j].second)
			{
				return 0;
			}
		}
	}
	return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair* relation, int size) {
	int x = 0;
	int y = 0;
	int z = 0;
	int flag = 0;

	for (int i = 0; i < size; i++) {
		x = relation[i].first;
		y = relation[i].second;
		for (int j = 0; j < size; j++) {
			if (y == relation[j].first) {
				z = relation[j].second;
				flag = 0;
				for (int k = 0; k < size; k++) {
					if (x == relation[k].first && z == relation[k].second) {
						flag = 1;
						break;
					}
				}
				if (flag == 0) {
					return 0;
				}
			}
		}
	}
	return 1;
}

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair* relation, int size) {
	if (is_reflexive(relation, size) == 1 && is_antisymmetric(relation, size) == 1 && is_transitive(relation, size) == 1) {
		return 1;
	}
	return 0;
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair* relation, int size) {
	if (is_partial_order(relation, size) == 1 && is_connected(relation, size) == 1) {
		return 1;
	}
	return 0;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair* relation, int size) {
}

int find_max_elements(pair*, int, int*);
int find_min_elements(pair*, int, int*);
int get_domain(pair*, int, int*);

// Case 3:

int composition(pair*, int, pair*, int, pair*);

// Comparator for pair
int cmp_pair(const void* a, const void* b) {
	pair* a_rzut = (pair*)a;
	pair* b_rzut = (pair*)b;

	if (a_rzut->first == b_rzut->first) {
		return a_rzut->second - b_rzut->second;
	}
	return a_rzut->first - b_rzut->first;
}

// Add pair to existing relation if not already there
int add_relation(pair* tab, int n, pair new_pair) {
	int result = 0;
	for (int i = 0; i < n; i++)
	{
		result = cmp_pair(&tab[i], &new_pair);
		if (result != 0)
		{
			if (tab[i].first == -1 && tab[i].second == -1)
			{
				tab[i].first = new_pair.first;
				tab[i].second = new_pair.second;
				return i;
			}
		}
		else
			return i;
	}
	return n;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair* relation) {
	int size = 0;
	pair para;
	int result = 0;
	int max_size = 0;

	scanf("%d", &size);

	for (int i = 0; i < size; i++) {
		relation[i].first = -1;
		relation[i].second = -1;
	}
	for (int i = 0; i < size; i++) {
		scanf("%d", &para.first);
		scanf("%d", &para.second);

		result = add_relation(relation, size, para);
		if (result > max_size)
			max_size = result;
	}
	return max_size + 1;
}

void print_int_array(int* array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d", &to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
	case 1:
		printf("%d ", is_reflexive(relation, size));
		printf("%d ", is_irreflexive(relation, size));
		printf("%d ", is_symmetric(relation, size));
		printf("%d ", is_antisymmetric(relation, size));
		printf("%d ", is_asymmetric(relation, size));
		printf("%d\n", is_transitive(relation, size));
		break;
	case 2:
		ordered = is_partial_order(relation, size);
		n_domain = get_domain(relation, size, domain);
		printf("%d %d\n", ordered, is_total_order(relation, size));
		print_int_array(domain, n_domain);
		if (!ordered) break;
		int no_max_elements = find_max_elements(relation, size, max_elements);
		int no_min_elements = find_min_elements(relation, size, min_elements);
		print_int_array(max_elements, no_max_elements);
		print_int_array(min_elements, no_min_elements);
		break;
	case 3:
		size_2 = read_relation(relation_2);
		printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
		break;
	}
	return 0;
}

