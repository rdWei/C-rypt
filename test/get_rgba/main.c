#include "../../src/read.c"

#include <png.h>
#include <stdio.h>


int main() {


  if(IsValid("/home/samuele/Dev/C-rypto/lain.png")) {
    char* message = generateEncodedAlphabet("/home/samuele/Dev/C-rypto/lain.png");
    printf("Message: %s", message);
  }
}
