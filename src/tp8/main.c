#include <stdio.h>
#include <stdlib.h>
#include "myalloc.h"
#include "test.h"

int main() {
  /* MALLOC 1, 2 */
  /* CALLOC 1, 2 */
  /* REALLOC 1, 2, 3, 4 */
  procedure_test(MALLOC, 1);
  exit(1);
}
