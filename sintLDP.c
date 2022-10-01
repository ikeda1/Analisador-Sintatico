#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


// Dado a Gramática
// G = ( N, E, P, <ini>)

// Inicializando funções

int ini(char texto[], int *pos);
int id(char texto[], int *pos);
int dcl(char texto[], int *pos);
int tpo(char texto[], int *pos);
int sub(char texto[], int *pos);
int param(char texto[], int *pos);
int bco(char texto[], int *pos);
int cmd(char texto[], int *pos);
int wr(char texto[], int *pos);
int rd(char texto[], int *pos);
int IF(char texto[], int *pos);
int fr(char texto[], int *pos);
int wh(char texto[], int *pos);
int rpt(char texto[], int *pos);
int atr(char texto[], int *pos);
int f(char texto[], int *pos);
int e(char texto[], int *pos);
int exp(char texto[], int *pos);
int exps(char texto[], int *pos);
int tmo(char texto[], int *pos);
int ftr(char texto[], int *pos);
int op_cat3(char texto[], int *pos);
int op_cat2(char texto[], int *pos);
int op_cat1(char texto[], int *pos);

char lookahead;

int check(char tk, char texto[], int *pos) {
    if (lookahead == tk) {
        *pos++;
        lookahead = texto[*pos];
        return 1;
    }
    return 0;
}

int ini(char texto[], int *pos) {
    if (lookahead == 'p'){
        if (check('p', texto, pos) && check('r', texto, pos) && check('g', texto, pos)){ // sPRG
            if (id(texto, pos) && check(';', texto, pos)) { // <id>;
                if (dcl(texto, pos)) {
                    if (sub(texto, pos)){
                        if (bco(texto, pos)) {
                            if (check('.', texto, pos)) {
                                return 1;
                            } else return 0;
                        } else return 0;
                    } else if (bco(texto, pos) && check('.', texto, pos)) {
                        return 1;
                    } else return 0;
                } else return 0;
            } return 0;
        } return 0;
    }
}

int id(char texto[], int *pos) {
 while (isalpha(lookahead) || isdigit(lookahead) || (lookahead == '_')) {
    check(lookahead, texto, pos);
 }
 
}