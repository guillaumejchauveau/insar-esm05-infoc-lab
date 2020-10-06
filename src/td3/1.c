#include <string.h>
#include <stdlib.h>

void concat1(char *str1, char *str2, char *dest) {
  strcpy(dest, str1);
  strcpy(dest + strlen(str1), str2);
}

char *concat2(char *str1, char *str2) {
  char *dest = malloc((1 + strlen(str1) + strlen(str2)) * sizeof(char));
  if (dest == NULL) {
    return NULL;
  }
  concat1(str1, str2, dest);
  return dest;
}

void concat3(char *str1, char *str2, char **dest_p) {
  *dest_p = malloc((1 + strlen(str1) + strlen(str2)) * sizeof(char));
  if (*dest_p == NULL) {
    return;
  }
  concat1(str1, str2, *dest_p);
}

int main() {
  char *dest;
  concat3("hehe", "hihi", &dest);
  free(dest);
  return 0;
}
