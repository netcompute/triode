#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "ir/generator.h"

extern triode::NBlock* programBlock;
extern int yyparse();
extern FILE *yyin;

int main(int argc, char* argv[]) {
//  printf("%s", argv[1]);
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("cannot open %s\n", argv[1]);
        return -1;
    }
    yyin = fp;
    yyparse();

    triode::ir::Generator irgen;
    irgen.Generate(*programBlock);

    fclose(fp);

    return 0;
}