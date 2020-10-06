#include <stdio.h>

void ex1() {
  int a, b;
  printf("a b: ");
  scanf("%d %d", &a, &b);
  if (a % b) {
    printf("non multiple\n");
  } else {
    printf("multiple\n");
  }
}

void ex2() {
  int i,n,somme;
  printf("n: ");
  scanf("%d", &n);

  somme = 0;
  for (i=1; i<n; i++) {
    somme += i;
  }

  somme = 0;
  i=1;
  while (i<n) {
    somme += i;
    i++;
  }

  somme = 0;
  i=0;
  do {
    somme += i;
    i++;
  } while (i<n);
}

void ex3() {
  int a;
  printf("a: ");
  scanf("%d", &a);
  if (a < 2) {
    printf("pas premier\n");
  }

  for (int i = 2; i < a; i++) {
    if (a % i == 0) {
      printf("pas premier\n");
      return;
    }
  }
  printf("premier\n");
}

int find_in_list(int value, int *list, size_t list_size) {
  for (int i = 0; i < list_size; i++) {
    if (list[i] == value) {
      return i;
    }
  }
  return -1;
}

void ex4() {
  int l[] = {0,4,7,8,9,5};

  printf("%d\n", find_in_list(7, l, 6));
}

int prompt_numbers(int *numbers_buffer, size_t buffer_size) {
  int n;
  printf("Combien ?: ");
  scanf("%d", &n);
  if (n > buffer_size) {
    printf("trop grand\n");
    return -1;
  }
  for (int i = 0; i < n; i++) {
    printf("Nombre %d: ", i);
    scanf("%d", numbers_buffer + i);
  }
  return n;
}

void ex5() {
  int buf[10];
  printf("%d\n", prompt_numbers(buf, 10));
}

int find_dicho(int value, int *list, size_t list_size) {
  int mid;
  int start = 0;
  int end = list_size - 1;
  while (start <= end) {
    mid = (start + end) / 2;
    if (list[mid] == value) {
      return mid;
    }
    if (list[mid] > value) {
      end = mid - 1;
    } else {
      start = mid + 1;
    }
  }
  return -1;
}


int main() {
  ex5();
}
