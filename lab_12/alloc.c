#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int* A) {
}

void set(int cols, int row, int col, int* A, int value) {
}

void prod_mat(int rowsA, int colsA, int colsB, int* A, int* B, int* AB) {
	int* result = AB;
	int* r = AB;
	for (int i = 0; i < rowsA * colsB; i++) {
		*r = 0;
		r++;
	}
	int* a = A;
	int* b = B;
	for (int i = 0; i < rowsA; i++) {
		for (int j = 0; j < colsB; j++) {
			for (int k = 0; k < colsA; k++) {
				*result += (*(a + (i * colsA) + k))* (*(b + (k * colsB) + j));
			}
			result++;
		}
	}
}

void read_mat(int rows, int cols, int* t) {
	int *x = t;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			scanf("%d", x);
			x++;
		}
	}
}

void print_mat(int rows, int cols, int* t) {
	int* x = t;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			printf("%d ", *x);
			x++;
		}
		printf("\n");
	}
}

int read_char_lines(char* array[]) {
	int lines_counter = 0;
	char buffer[BUF_SIZE];

	while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
		// Usuniêcie znaku nowej linii z koñca bufora
		buffer[strcspn(buffer, "\n")] = '\0';

		// Alokacja pamiêci dla ci¹gu znaków
		char* line = (char*)malloc((strlen(buffer) + 1) * sizeof(char));

		// Kopiowanie ci¹gu znaków do przydzielonej pamiêci
		strcpy(line, buffer);

		// Zapisanie adresu pocz¹tkowego elementu wiersza w tablicy wskaŸników
		array[lines_counter] = line;

		lines_counter++;
	}

	return lines_counter;
}

void write_char_line(char* array[], int n) {
	printf("%s\n", array[n]);
}

void delete_lines(char* array[]) {
	int i = 0;
	while (array[i] != NULL) {
		free(array[i]);
		array[i] = NULL;
		i++;
	}
}

int read_int_lines_cont(int* ptr_array[]) {
	int lines_counter = 0;
	char buffer[BUF_SIZE];
	char* token;

	while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
		int* numbers = (int*)malloc(TAB_SIZE * sizeof(int));
		int count = 0;

		token = strtok(buffer, " \n");
		while (token != NULL) {
			numbers[count] = atoi(token);
			count++;
			token = strtok(NULL, " \n");
		}

		ptr_array[lines_counter] = numbers;
		lines_counter++;
	}

	return lines_counter;
}

void write_int_line_cont(int* ptr_array[], int n) {
	int* line = ptr_array[n];

	while (*line != 0) {
		printf("%d ", *line);
		line++;
	}

	printf("\n");
}

typedef struct {
	int* values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
	int line_count = 0;
	char line_buf[BUF_SIZE];
	char* token;

	while (fgets(line_buf, BUF_SIZE, stdin)) {
		// Allocate memory for the line values
		int* values = malloc(TAB_SIZE * sizeof(int));
		int value_count = 0;

		// Split the line into individual numbers using strtok()
		token = strtok(line_buf, " \t\n");
		while (token != NULL) {
			values[value_count++] = atoi(token);
			token = strtok(NULL, " \t\n");
		}

		// Calculate the average of the line values
		double sum = 0;
		for (int i = 0; i < value_count; i++) {
			sum += values[i];
		}
		double average = sum / value_count;

		// Save the line values, length, and average to the lines_array
		lines_array[line_count].values = values;
		lines_array[line_count].len = value_count;
		lines_array[line_count].average = average;

		line_count++;
	}

	return line_count;
}

void write_int_line(line_type lines_array[], int n) {
	n--; // Adjusting the input line number to array index
	if (n >= 0 && n < TAB_SIZE && lines_array[n].values != NULL) {
		line_type line = lines_array[n];
		int len = line.len;

		for (int i = 0; i < len; i++) {
			printf("%d\n", line.values[i]);
		}
		printf("%.2lf", line.average);
	}
}

void delete_int_lines(line_type array[], int line_count) {
	for (int i = 0; i < line_count; i++) {
		free(array[i].values);
		array[i].values = NULL;
		array[i].len = 0;
		array[i].average = 0.0;
	}
}

int cmp(const void* a, const void* b) {
	const line_type* lineA = (const line_type*)a;
	const line_type* lineB = (const line_type*)b;

	if (lineA->average < lineB->average) {
		return -1;
	}
	else if (lineA->average > lineB->average) {
		return 1;
	}
	else {
		return 0;
	}
}

void sort_by_average(line_type lines_array[], int line_count) {
	qsort(lines_array, line_count, sizeof(line_type), cmp);
}

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet* triplet_array, int n_triplets) {
	for (int i = 0; i < n_triplets; i++) {
		if (scanf("%d %d %d", &triplet_array[i].r, &triplet_array[i].c, &triplet_array[i].v) != 3) {
			return i;
		}
	}
	return n_triplets;
}

int cmp_triplets(const void* t1, const void* t2) {
	triplet* a = (triplet*)t1;
	triplet* b = (triplet*)t2;
	if (a->r == b->r) {
		return a->c - b->c;
	}
	return a->r - b->r;
}

void make_CSR(triplet* triplet_array, int n_triplets, int rows, int* V, int* C, int* R) {
	qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);

	int current_row = -1;
	int current_row_start = 0;
	int nonzero_count = 0;

	for (int i = 0; i < n_triplets; i++) {
		triplet t = triplet_array[i];
		if (t.r != current_row) {
			for (int j = current_row + 1; j <= t.r; j++) {
				R[j] = nonzero_count;
			}
			current_row = t.r;
			current_row_start = i;
		}
		V[nonzero_count] = t.v;
		C[nonzero_count] = t.c;
		nonzero_count++;
	}

	for (int j = current_row + 1; j <= rows; j++) {
		R[j] = nonzero_count;
	}

}

void multiply_by_vector(int rows, const int* V, const int* C, const int* R, const int* x, int* y) {
	for (int i = 0; i < rows; i++) {
		y[i] = 0;
		for (int j = R[i]; j < R[i + 1]; j++) {
			y[i] += V[j] * x[C[j]];
		}
	}

}

void read_vector(int* v, int n) {
	for (int i = 0; i < n; i++) {
		scanf("%d", &v[i]);
	}
}

void write_vector(int* v, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char* char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int* ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
	case 1:
		scanf("%d %d", &rowsA, &colsA);
		read_mat(rowsA, colsA, A);
		scanf("%d %d", &rowsB, &colsB);
		read_mat(rowsB, colsB, B);
		prod_mat(rowsA, colsA, colsB, A, B, AB);
		print_mat(rowsA, colsB, AB);
		break;
	case 2:
		n = read_int() - 1; // we count from 1 :)
		ptr_array[0] = continuous_array;
		read_int_lines_cont(ptr_array);
		write_int_line_cont(ptr_array, n);
		break;
	case 3:
		n = read_int() - 1;
		read_char_lines(char_lines_array);
		write_char_line(char_lines_array, n);
		delete_lines(char_lines_array);
		break;
	case 4:
		n = read_int() - 1;
		lines_counter = read_int_lines(int_lines_array);
		sort_by_average(int_lines_array, lines_counter);
		write_int_line(int_lines_array, n);
		delete_int_lines(int_lines_array, lines_counter);
		break;
	case 5:
		scanf("%d %d %d", &rows, &cols, &n_triplets);
		n_triplets = read_sparse(triplet_array, n_triplets);
		read_vector(x, cols);
		make_CSR(triplet_array, n_triplets, rows, V, C, R);
		multiply_by_vector(rows, V, C, R, x, y);
		write_vector(V, n_triplets);
		write_vector(C, n_triplets);
		write_vector(R, rows + 1);
		write_vector(y, rows);
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
		break;
	}
	return 0;
}