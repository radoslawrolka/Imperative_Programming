#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int day;
	int month;
	int year;
} Date;

// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
	char name[20];
	float price;
	int amount;
	Date valid_date;
} Food;

typedef int (*ComparFp)(const void*, const void*);

// compare dates
int cmp_date(const void* d1, const void* d2) {
	Date* date1 = (Date*)d1;
	Date* date2 = (Date*)d2;

	if (date1->year > date2->year) { return 1; }
	else if (date1->year < date2->year) { return -1; }
	else {
		if (date1->month > date2->month) { return 1; }
		else if (date1->month < date2->month) { return -1; }
		else {
			if (date1->day > date2->day) { return 1; }
			else if (date1->day < date2->day) { return -1; }
			else { return 0; }
		}
	}
}


// compare foods
int cmp(const void* a, const void* b) {
	const Food* food1 = (Food*)a;
	const Food* food2 = (Food*)b;

	if (cmp_date(&food1->valid_date, &food2->valid_date) == 0) {
		int name_cmp = 0;
		name_cmp = strcmp(food1->name, food2->name);
		if (name_cmp == 0) {
			if (food1->price > food2->price) { return 1; }
			else if (food1->price < food2->price) { return -1; }
			else return 0;
		}
		else return name_cmp;
	}
	else {
		return cmp_date(&food1->valid_date, &food2->valid_date);
	}
}



// bsearch returning address where to insert new element
void* bsearch2(const void* key, const void* base, size_t nmemb, size_t size, ComparFp compar, char* result) {
	const char* base_ptr = base;
	size_t low = 0, high = nmemb;
	while (low < high) {
		size_t mid = (low + high) / 2;
		const char* mid_ptr = base_ptr + mid * size;
		int cmpr = cmp(key, mid_ptr);
		if (cmpr == 0) {
			memcpy(result, "FOUND", 6);
			return (void*)mid_ptr;
		}
		else if (cmpr == -1) {
			high = mid;
		}
		else {
			low = mid + 1;
		}
	}
	memcpy(result, "NOT FOUND", 10);
	return (void*) (base_ptr + low * size);
}


// print goods of given name
void print_art(Food* p, int n, char* art) {
	for (int i = 0; i < n; i++) {
		if (strcmp(p[i].name, art) == 0) {
			if (p[i].valid_date.month < 10) {
				if (p[i].valid_date.day < 10) {printf("%.2f %d 0%d.0%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);}
				else { printf("%.2f %d %d.0%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year); }
			}
			else if (p[i].valid_date.day < 10) {printf("%.2f %d 0%d.%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);}
			else {printf("%.2f %d %d.%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);}
		}
	}
}

// add record to table if absent
Food* add_record(Food* tab, int* np, ComparFp compar, Food* new) {
	char result[10];
	Food* found = bsearch2(new, tab, *np, sizeof(Food), compar, result);
	if (strcmp(result, "FOUND") == 0) {
		found->amount += new->amount;
		return found;
	}
	else {
		if (*np >= FOOD_MAX) { return NULL; }
		int index = found - tab;
		memmove(found + 1, found, (*np - index) * sizeof(Food));
		memcpy(found, new, sizeof(Food));
		(*np)++;
		return found;
	}
}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food* tab, int no, FILE* stream, int sorted) {
	int i = 0;
	int ile = 0;

	for (i; i < no; i++) {
		Food new_food;
		fscanf(stream, "%s %f %d %d.%d.%d", &new_food.name, &new_food.price, &new_food.amount, &new_food.valid_date.day, &new_food.valid_date.month, &new_food.valid_date.year);
		add_record(tab, &ile, cmp, &new_food);
	}
	return ile;
}

int cmp_qs(const void* a, const void* b) {
	Food* fa = (Food*)a, * fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void* a, const void* b) {
	Date* pd = (Date*)a;
	Food* fb = (Food*)b;
	return cmp_date(pd, &fb->valid_date);
}

// finds the value of goods due on 'curr_date'
float value(Food* food_tab, size_t n, Date curr_date, int days) {
	qsort(food_tab, n, sizeof(Food), cmp);

	struct tm cdate = { 0 };
	cdate.tm_year = curr_date.year - 1900;
	cdate.tm_mon = curr_date.month - 1;
	cdate.tm_mday = curr_date.day;
	time_t ctime = mktime(&cdate);

	float val = 0.0;

	for (int i = 0; i < n; i++) {
		struct tm fdate = { 0 };
		fdate.tm_year = food_tab[i].valid_date.year - 1900;
		fdate.tm_mon = food_tab[i].valid_date.month - 1;
		fdate.tm_mday = food_tab[i].valid_date.day;
		time_t ftime = mktime(&fdate);

		if (ctime + ((days) * 24 * 60 * 60) == ftime) {
			val += ((food_tab[i].amount) * (food_tab[i].price));
		}
	}
	return val;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex { F, M };
enum BOOL { no, yes };

struct Bit_data {
	enum Sex sex :1;
	enum BOOL in_line :1;
};

typedef struct {
	char* name;
	struct Bit_data bits;
	Date born;
	char* parent;
} Person;

typedef struct {
	char* par_name;
	int index;
	int noc;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person* person1, Person* person2) {
	if (person1->bits.sex == person2->bits.sex) return 0;
	if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
	if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
	return person2->bits.sex - person1->bits.sex;
}

// compare persons
int cmp_person(const void* a, const void* b) {
	const Person* p1 = (Person*)a;
	const Person* p2 = (Person*)b;

	if (p1->parent == NULL) { return -1; }
	else if (p2->parent == NULL) { return 1; }

	int parrent = strcmp(p1->parent, p2->parent);
	if (parrent < 0) { return -1; }
	else if (parrent > 0) { return 1; }
	else {
		int born_date = cmp_date(&p1->born, &p2->born);
		if (cmp_date(&p1->born, &primo_date) < 0 && cmp_date(&p2->born, &primo_date) < 0) {
			if ((p1->bits.sex == 'M') && (p2->bits.sex == 'F')) { return -1; }
			else if ((p1->bits.sex == 'F') && (p2->bits.sex == 'M')) { return 1; }
		}
		if (born_date < 0) { return -1; }
		else if (born_date > 0) { return 1; }
		else {
			if (p1->bits.sex > p2->bits.sex) { return 1; }
			else if (p1->bits.sex < p2->bits.sex) { return -1; }
			else { return 0; }
		}
	}
}

int fill_indices_tab(Parent* idx_tab, Person* pers_tab, int size) {
	int nop = 0;
	for (int i = 0; i < size; i++) {
		idx_tab[i].par_name = pers_tab[i].name;
	}
	for (int i = 0; i < size; i++) {
		idx_tab[i].index = 0;
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (pers_tab[j].parent == idx_tab[i].par_name) {
				idx_tab[i].index = j;
				break;
			}
		}
	}
	for (int i = 0; i < size; i++) {
		int start = idx_tab[i].index;
		int ile = 0;
		for (int j = start; j < size; j++) {
			if (pers_tab[j].parent == pers_tab[i].name) {
				ile += 1;
			}
			else {
				break;
			}
		}
		idx_tab[i].noc = ile;
	}
	for (int i = 0; i < size; i++) {
		if (idx_tab[i].noc > 0) { nop += 1; }
	}
	return nop;
}

void persons_shifts(Person* person_tab, int size, Parent* idx_tab, int no_parents) {
	for (int i = 0; i < size; i++) {
		if (idx_tab[i].noc > 0) {
			Person* shifted_arr = malloc((idx_tab[i].noc) * sizeof(Person));
			memmove(shifted_arr, &person_tab[idx_tab[i].index], (idx_tab[i].noc) * sizeof(Person));
			int ile = idx_tab[i].index - i - 1;
			int gdzie = idx_tab[i].index - ile + idx_tab[i].noc;
			memmove(&person_tab[gdzie], &person_tab[i + 1], ile * sizeof(Person));
			memmove(&person_tab[i + 1], shifted_arr, idx_tab[i].noc * sizeof(Person));
			free(shifted_arr);
			no_parents = fill_indices_tab(idx_tab, person_tab, size);
		}
	}
}

int cleaning(Person* person_tab, int n) {
	for (int i = 0; i < n; i++) {
		if (person_tab[i].bits.in_line == no) {
			memmove(&person_tab[i], &person_tab[i + 1], (n - (i + 1)) * sizeof(Person));
			n -= 1;
			i -= 1;
		}
	}
	return n;
}

void print_person(const Person* p) {
	printf("%s\n", p->name);
}

int create_list(Person* person_tab, int n) {
	qsort(person_tab, n, sizeof(Person), cmp_person);
	Parent id_tab[33];
	int num_par = 0;
	num_par = fill_indices_tab(id_tab, person_tab, n);
	persons_shifts(person_tab, n, id_tab, num_par);
	int num_per = 0;
	num_per = cleaning(person_tab, n);
	return num_per;
}

int main(void) {
	Person person_tab[] = {
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"}
	};

	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[ART_MAX];
	fgets(buff, ART_MAX, stdin);
	sscanf(buff, "%d", &to_do);

	switch (to_do) {
	case 1:  // bsearch2
		fgets(buff, ART_MAX, stdin);
		sscanf(buff, "%d", &no);
		n = read_goods(food_tab, no, stdin, 1);
		scanf("%s", buff);
		print_art(food_tab, n, buff);
		break;
	case 2: // qsort
		fgets(buff, ART_MAX, stdin);
		sscanf(buff, "%d", &no);
		n = read_goods(food_tab, no, stdin, 0);
		Date curr_date;
		int days;
		scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
		scanf("%d", &days);
		printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
		break;
	case 3: // succession
		scanf("%d", &no);
		int no_persons = sizeof(person_tab) / sizeof(Person);
		no_persons = create_list(person_tab, no_persons);
		print_person(person_tab + no - 1);
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}