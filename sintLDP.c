#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>




// Inicializando funções

int ini(char texto[], int *pos);
int id(char texto[], int *pos);
int dcl(char texto[], int *pos);
int num(char texto[], int *pos);
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
int symbols(char texto[], int *pos);
int validStrings(char texto[], int *pos);
int e(char texto[], int *pos);
int expr(char texto[], int *pos);
int exps(char texto[], int *pos);
int tmo(char texto[], int *pos);
int ftr(char texto[], int *pos);
int op_cat3(char texto[], int *pos);
int op_cat2(char texto[], int *pos);
int op_cat1(char texto[], int *pos);

char lookahead;

int check(char tk, char texto[], int *pos) {
    if (lookahead == tk) {
        lookahead = texto[++(*pos)];
        return 1;
    }
    return 0;
}

int ini(char texto[], int *pos) {
    printf("inside\n");
    if (lookahead == 'p'){
        printf("achou o p\n");
        if (check('p', texto, pos) && check('r', texto, pos) && check('g', texto, pos)){ // sPRG
            printf("achou prg\n");
            if (id(texto, pos) && check(';', texto, pos)) { // <id>;
                printf("dentro do id\n");
                if (dcl(texto, pos)) {
                    printf("achou dcl\n");
                    
                    if (sub(texto, pos)){
                        printf("achou sub\n");
                        if (bco(texto, pos)) {
                            printf("achou bco\n");
                            if (check('.', texto, pos)) {
                                return 1;
                            }
                        } else return 0;

                    } else if (bco(texto, pos)) {
                        printf("segundo bco\n");
                        if (check('.', texto, pos)){
                            return 1;
                        } else return 0;
                    }

                } else if (sub(texto, pos)) {
                    printf("segundo sub\n");
                    if (bco(texto, pos)) {
                        if (check('.', texto, pos)) {
                            return 1;
                        }
                    } else return 0;

                } 
                else if (bco(texto, pos)) {
                    printf("segundo bco\n");
                    if (check('.', texto, pos)) {
                        return 1;
                    }
                } else return 0;
            } 
        }
    }
    return 0;
}

// int bco(char texto[], int *pos) { // <bco> → sBEGIN {<cmd>;} sEND
//     printf("entrou no bco\n");
//     if (lookahead == 'b') {
//         printf("lookahead eh %c\n", lookahead);
//         if (check('b', texto, pos) && check('e', texto, pos) && check('g', texto, pos) && check('i', texto, pos) && check('n', texto, pos)) {
//             printf("checked begin %c\n", lookahead);
//             if (cmd(texto, pos)) { // Se tiver <cmd>
//                 printf("checked cmd\n");
//                 if (lookahead == ';') {
//                     if (check(';', texto, pos)) {
//                         return 1;
//                     }
//                 } //return 0;

//             } else if (lookahead == 'e') {
//                 if (check('e', texto, pos) && check('n', texto, pos) && check('d', texto, pos)) { // Se não tiver <cmd>, pula direto pro sEND
//                     printf("checked end\n");
//                     return 1;
//                 }
//             }
//             printf("lookahead eh %c", lookahead);
//         }
//     } 
//     printf("saiu do bco\n");
//     return 0;
// }


int bco(char texto[], int *pos){ //<bco> → sBEGIN {<cmd>;} sEND
  if(lookahead == 'b')
  {
    BeginNovo:
    if(check('b', texto, pos) && check('e', texto, pos) && check('g', texto, pos) && check('i', texto, pos) 
      && check('n', texto, pos))
    {
    continuaComandos:
      if(cmd(texto, pos) && check(';', texto, pos))
      {
        NovoComando:
        if(lookahead == 'e')
        {
          if(check('e', texto, pos) && check('n', texto, pos) && check('d', texto, pos) && check(';', texto,pos)) //Como visto nos exemplos temos dois tipos de end caso tenha mais de 1 o primeiro termina com ; e o segundo com .
          {
            if(lookahead == 'b') //Caso tenha um begin novamente
            {
              goto BeginNovo;
            }
            else if(cmd(texto, pos) && check(';', texto, pos)) //Caso tenha um comando depois do end
            {
              goto NovoComando;
            }
            else
              return (1);
          }
          else
            goto continuaComandos;
        }
        else
          goto continuaComandos;
      }
      else if(lookahead == 'e')
      {
        if(check('e', texto, pos) && check('n', texto, pos) && check('d', texto, pos))
        {
          return (1);
        }
        else
          goto continuaComandos;
      }
      else if(lookahead == '.') // fim do último begin
        return (1);
      else
        return(0);//goto continuaComandos;
    }
    else
      return (0);
  }
  else
    return (0);
}

int id(char texto[], int *pos) {
    printf("id eh ");
    while (isalpha(lookahead) || isdigit(lookahead) || (lookahead == '_')) {
        printf("%c", lookahead);
        check(lookahead, texto, pos);
        if(lookahead == ';' || lookahead == '[' || lookahead == '(') {
            return 1;
        }
    }     
    printf("\nfim do id\n");
    return 0;
}

int dcl(char texto[], int *pos) { //<dcl> → sVAR <tpo><id>[[sNUM]]{,<id>[[sNUM]]} {<tpo><id>[[sNUM]]{,<id>[[sNUM]]};}

  if(lookahead == 'v')
  {
    if(check('v', texto, pos) && check('a', texto, pos) && check('r', texto, pos)) //sVAR
    {
      if(tpo(texto, pos) && id(texto, pos)) // <tpo><id>
      {
    novaVar:
        if(lookahead == '[') // [[sNUM]]
        { 
          if (check('[', texto, pos) && num(texto, pos) && check(']', texto, pos)) // sVAR <tpo><id>[[sNUM]]
            {
              if(lookahead == ';') // sVAR <tpo><id>[[sNUM]];
              {
                if(check(';', texto, pos))
                {
                  return (1);
                }
                else
                  return (0);
              }
              else if(lookahead == ',') //Caso tenha mais id
              {
                if(check(',', texto, pos) && id(texto, pos)) // , <id>
                {
                  if(lookahead == '[')
                  { // ,<id>[[sNUM]]
                    if(check('[', texto, pos) && num(texto, pos) && check(']', texto, pos))
                    {
                      if(lookahead == ';')
                      {
                        if(check(';', texto, pos)) // , <id>[[sNUM]] ;
                        {
                          if(tpo(texto, pos) && id(texto, pos))
                          {
                            goto novaVar;
                          }
                          else
                            return (1);
                        }
                      }
                    }
                  }
                  else if(lookahead == ';')
                  {
                    if(check(';', texto, pos))
                    {
                      if(tpo(texto, pos) && id(texto, pos)) //{<tpo><id>}
                      {
                        goto novaVar;
                      }
                      else
                        return (1);
                    }
                  }
                }
              }
            }
        }
        else if(lookahead == ',') //Caso tenha mais id
        {
          if(check(',', texto, pos) && id(texto, pos))
          {
            if(lookahead == '[')
            {
              if(check('[', texto, pos) && num(texto, pos) && check(']', texto, pos)) // id[[sNUM]]
              {
                if(lookahead == ';')
                {
                  if(check(';', texto, pos))
                  {
                    if(tpo(texto, pos) && id(texto, pos)) //{<tpo>}{<id>}
                    {
                      goto novaVar;
                    }
                    else
                      return (1);
                  }
                }
                else if(lookahead == ',') // {, <id>}
                {
                  if(check(',', texto, pos) && id(texto, pos))
                  {
                    goto novaVar; //Caso tenha nova variavel
                  }
                }
              }
            }
            else if(lookahead == ';')
            {
              if(check(';', texto, pos))
              {
                if(tpo(texto, pos) && id(texto, pos)) //{<tpo>}{id}
                {
                  goto novaVar; //Caso tenha uma nova var
                }
                else
                   return (1);
              }
            }
            else if(lookahead == ',')
            {
              if(check(',', texto, pos) && id(texto, pos)) //{, <id>}
              {
                goto novaVar;
              }
            }
            else
              return (0);
          }
          else
            return (0);
        }
        else if(lookahead == ';')
        {
          if(check(';', texto, pos))
          {
            if(tpo(texto, pos) && id(texto, pos)) // {<tpo>}{<id>}
            {
              goto novaVar; //Caso tenha uma nova variavel
            }
            else
              return (1);
          }
        }
        else
          return (0);
      }
      else
        return (0);
    }
    else
      return (0);
  }
  else
    return (0);
}

int tpo(char texto[], int *pos) { // sINT || sFLOAT || sCHAR
    if (lookahead == 'c') {
        if (check('c', texto, pos) && check('h', texto, pos) && check('a', texto, pos) && check('r', texto, pos)) {
            return 1;
        } else return 0;
    } else if (lookahead == 'i') {
        if (check('i', texto, pos) && check('n', texto, pos) && check('t', texto, pos)) {
            return 1;
        } else return 0;
    } else if (lookahead == 'f') {
        if (check('f', texto, pos) && check('l', texto, pos) && check('o', texto, pos) && check('a', texto, pos) && check('t', texto, pos)) {
            return 1;
        } else return 0;
    } else return 0;
}

int sub(char texto[], int *pos) { // <sub> → sSUBROT (sVOID|<tpo>)<id>([<param>])[<dcl>][<sub>]<bco>;
                                  //{(sVOID|<tpo>)<id>([<param>])[<dcl>][<sub>]<bco>;}
    if (lookahead == 's') {
        if (check('s', texto, pos) && check('u', texto, pos) && check('b', texto, pos) && check('r', texto, pos) && check('o', texto, pos) && check('t', texto, pos)) {
            subrotina:
            printf("entrou na subrot\n");

            if (lookahead == 'v') { 
                if (check('v', texto, pos) && check('o', texto, pos) && check('i', texto, pos) && check('d', texto, pos)) { // sVOID
                    goto continuation;
                } else return 0;

            } else if(tpo(texto, pos)) { // <tpo>
                printf("\nachou o tpo\n");
                goto continuation;

            } else return 0;

            continuation:
            if (id(texto, pos)) { //<id>
                if (lookahead == '(') {
                    newParam:
                    if (check('(', texto, pos)) {

                        if (param(texto, pos)) { // se tiver parâmetros
                            if (check(')', texto, pos)) {
                                goto continuation2;
                            } else return 0;

                        } else if (lookahead == ')') { // se não tiver nenhum parâmetro
                            if (check(')', texto, pos)) {
                                goto continuation2;
                            } else return 0;

                        } else return 0;

                        continuation2:
                        if (dcl(texto, pos)) { // se tiver <dcl>
                            if (sub(texto, pos)) { // se tiver <sub>
                                if (bco(texto, pos)){
                                    if (lookahead == ';') {
                                        if (check(';', texto, pos)) {
                                            if (lookahead == 'v' || tpo(texto, pos)) {
                                                goto subrotina;
                                            } else return 1;
                                        } 
                                    } 
                                } else return 0;
                            
                            } else if (bco(texto, pos)){ // se não tiver <sub>, pula pra <bco>
                                if (lookahead == ';') {
                                    if (check(';', texto, pos)) {
                                        if (lookahead == 'v' || tpo(texto, pos)) { // Se tiver mais subrotinas
                                            goto subrotina;
                                        } else return 1;
                                    } 
                                } 

                            } else return 0; 

                        } else if (sub(texto, pos)) { // se não tiver <dcl>, pula direto para <sub>
                            if (bco(texto, pos)){ // se não tiver <sub>, pula pra <bco>
                                if (lookahead == ';') {
                                    if (check(';', texto, pos)) {
                                        if (lookahead == 'v' || tpo(texto, pos)) { // Se tiver mais subrotinas
                                            goto subrotina;
                                        } else return 1;
                                    } 
                                } 
                            } else return 0;

                        } else if (bco(texto, pos)){ // se não tiver <dcl><sub>, pula pra <bco>
                            if (lookahead == ';') {
                                if (check(';', texto, pos)) {
                                    if (lookahead == 'v' || tpo(texto, pos)) { // Se tiver mais subrotinas
                                        goto subrotina;
                                    } else return 1;
                                } 
                            } else return 0; 

                        } else return 0; // se não tiver <dcl>,<sub> e nem <bco>
                    } 
                }
            } 
        }
    }
    else return 0;
}

int param(char texto[], int *pos) { // <param> → <tpo><id>{;<tpo><id>}
    newParam:
    if (tpo(texto, pos)) {
        if (id(texto, pos)) {
            if (lookahead == ';') { // se tiver mais parâmetros
                if (check(';', texto, pos)) {
                    goto newParam;
                } else return 0; 

            } else return 1; // se não tiver mais parâmetros
        } 
    }
    else return 0;
}


int cmd(char texto[], int *pos) { // <cmd> → <wr>|<rd>|<if>|<fr>|<wh>|<rpt>|<atr>|<f>|<bco>
    if (wr(texto, pos) || rd(texto, pos) || IF(texto, pos) || fr(texto, pos) || wh(texto, pos) || rpt(texto, pos) || atr(texto, pos) || f(texto, pos) || bco(texto, pos)) {
        return 1;
    } else return 0;
}



int wr(char texto[], int *pos) { // <wr> → sWRITE(<e>{,<e>})
    if (lookahead == 'w') {
        if (check('w', texto, pos) && check('r', texto, pos) && check('i', texto, pos) && check('t', texto, pos) && check('e', texto, pos)) {
            if (check == '(') {
                if (e(texto, pos)) { 

                    if (lookahead == ',') { // se repetir <e>
                        if (check(',', texto, pos)) {
                            if (e(texto, pos)) {
                                if (lookahead == ')') {
                                    if (check(')', texto, pos)) {
                                        return 1;
                                    }
                                }
                            } return 0;
                        }
                        

                    } else if (lookahead == ')') { // se não repetir
                        if (check(')', texto, pos)) {
                            return 1;
                        }

                    } else return 0; // se não encontrar , nem )

                }  
            }
        }
    }
    return 0;
}

int rd(char texto[], int *pos) { // <rd> → sREAD(<id>[[sNUM]])
    if (lookahead == 'r') {
        if (check('r', texto, pos) && check('e', texto, pos) && check('a', texto, pos) && check('d', texto, pos)) {
            if (check('(', texto, pos)) {
                if (id(texto, pos)) {

                    if (lookahead == '[') { // se for um array
                        if (check('[', texto, pos)) {
                            if (num(texto, pos)) {
                                if (check(']', texto, pos)) {
                                    if (check(')', texto, pos)) {
                                        return 1;
                                    }
                                }
                            }
                        } 
                        return 0;
                    } else if (lookahead == ')') {
                        if (check(')', texto, pos)) {
                            return 1;
                        }
                    } else return 0;

                } 
            }
        }
    }
    return 0;
}

int IF(char texto[], int *pos) { // <if> → sIF(<exp>)sTHEN<cmd>[sELSE<cmd>]
    if (lookahead == 'i') {
        if (check('i', texto, pos) && check('f', texto, pos)) {
            if (check('(', texto, pos)) {
                if (expr(texto, pos)) {
                    if (check(')', texto, pos)) {
                        if (check('t', texto, pos) && check('h', texto, pos) && check('e', texto, pos) && check('n', texto, pos)) {
                            if (cmd(texto, pos)) {

                                if (lookahead == 'e') { // se tiver sELSE
                                    if (check('e', texto, pos) && check('l', texto, pos) && check('s', texto, pos) && check('e', texto, pos)) {
                                        if (cmd(texto, pos)) {
                                            return 1;
                                        }
                                    }

                                } else return 1; // se não tiver sELSE
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int fr(char texto[], int *pos) { // <fr> → sFOR(<atr>;<exp>;<atr>)<cmd>
    if (lookahead == 'f') {
        if (check('f', texto, pos) && check('o', texto, pos) && check('r', texto, pos)) {
            if (check('(', texto, pos)) {
                if (atr(texto, pos)) {
                    if (check(';', texto, pos)) {
                        if (expr(texto, pos)) {
                            if (check(';', texto, pos)) {
                                if (atr(texto, pos)) {
                                    if (check(')', texto, pos)) {
                                        if (cmd(texto, pos)) {
                                            return 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int wh(char texto[], int *pos) { // <wh> → sWHILE(<exp>)<cmd>
    if (lookahead == 'w') {
        if ((check('w', texto, pos) && check('h', texto, pos) && check('i', texto, pos) && check('l', texto, pos) && check('e', texto, pos))) {
            if (check('(', texto, pos)) {
                if (expr(texto, pos)) {
                    if (check(')', texto, pos)) {
                        if (cmd(texto, pos)) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int rpt(char texto[], int *pos) { // <rpt> → sREPEAT{<cmd>}sUNTIL(<exp>)
    if (lookahead == 'r') {
        if ((check('r', texto, pos) && check('e', texto, pos) && check('p', texto, pos) && check('e', texto, pos) && check('a', texto, pos) && check('t', texto, pos))) {
            
            if (cmd(texto, pos)) { // se tiver <cmd>
                if (check('u', texto, pos) && check('n', texto, pos) && check('t', texto, pos) && check('i', texto, pos) && check('l', texto, pos)) {
                    if (check('(', texto, pos)) {
                        if (expr(texto, pos)) {
                            if (check(')', texto, pos)) {
                                return 1;
                            }
                        }
                    }
                }
            return 0;
            // se não tiver <cmd>, pula para sUNTIL
            } else if (check('u', texto, pos) && check('n', texto, pos) && check('t', texto, pos) && check('i', texto, pos) && check('l', texto, pos)) {
                if (check('(', texto, pos)) {
                    if (expr(texto, pos)) {
                        if (check(')', texto, pos)) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int atr(char texto[], int *pos) { // <atr> → <id>[[sNUM]]sATRIB<exp>
    if (id(texto, pos)) {

        if (lookahead == '[') { // se for um array
            if (check('[', texto, pos)) {
                if (num(texto, pos)) {
                    if (check(']', texto, pos)) {
                        if (lookahead == '<') {
                            if (check('<', texto, pos) && check('-', texto, pos)) {
                                if (expr(texto, pos)) {
                                    return 1;
                                }
                            }
                        }
                    }
                }
            } 
            return 0;

        } else if (lookahead == '<') { // se não for um array
            if (check('<', texto, pos) && check('-', texto, pos)) {
                if (expr(texto, pos)) {
                    return 1;
                }
            }   
        } else return 0;
    }
    return 0;
}

int f(char texto[], int *pos) { // <f> → <id>(<exp>{,<exp>})
    if (id(texto, pos)) {
        if (check('(', texto, pos)) {
            if (expr(texto, pos)) {

                if (lookahead == ',') { // se tiver mais <expr>
                    if (check(',', texto, pos)) {
                        if (expr(texto, pos)) {
                            if (check(')', texto, pos)) {
                                return 1;
                            }
                        }
                    } return 0;

                } else if (lookahead == ')') { // se não tiver mais <expr>
                    if (check(')', texto, pos)) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int symbols(char texto[], int *pos) {
    if (lookahead == '_' || lookahead == ' ' || lookahead == ':' || lookahead == '~' || lookahead == '^'
     || lookahead == '-' || lookahead == '+' || lookahead == '*' || lookahead == '/'  || lookahead == '>' 
     || lookahead == ':' || lookahead == '[' || lookahead == '.' || lookahead == ']' || lookahead == '{' 
     || lookahead == '}' || lookahead == ';' || lookahead == '#') {
        return 1;
     }
     else return 0;
}

int validStrings(char texto[], int *pos){ //Letra ou simbolo
    if(isalpha(lookahead) || isdigit(lookahead) || symbols(texto, pos)) {
        if(check(lookahead, texto, pos) && validStrings(texto, pos)) {
            return (1);
        }
        else return (0);

    } else if(lookahead == '\'' || lookahead == '\"') {
        return (1);
    }
    return(0);
}

int num(char texto[], int *pos) {
    if (isdigit(lookahead)) {
        return 1;
    }
    else return 0;
}

int e(char texto[], int *pos) { // <e> → sSTRING|sNUM|sCHAR|<id>[[sNUM]]|<f>

    if (lookahead == '\"') { // sSTRING
        if (check('\"', texto, pos) && validStrings(texto, pos) && check('\"', texto, pos)) {
            return 1;
        }
        return 0;

    } else if (num(texto, pos)) { // sNUM
        return 1;

    } else if (lookahead == '\'') { // sCHAR
        if (check('\'', texto, pos) && validStrings(texto, pos) && check('\'', texto, pos)) {
            return 1;
        }
        return 0;

    } else if (id(texto, pos)) { // <id>[[sNUM]]
        if (lookahead == '[') {
            if (check('[', texto, pos) && num(texto, pos) && check(']', texto, pos)) {
                return 1;
            } else return 0;

        } else if (lookahead == '(') {
            if (f(texto, pos)) {
                return 1;
            } 
        } else return 1;

    } else if (f(texto, pos)) { // <f>
        return 1;

    }
    else return 0;
}

int expr(char texto[], int *pos) { // <exp> → <exps>{<op_cat3><exps>}
    repete:
    if (exps(texto, pos)) {
        if (op_cat3(texto, pos) && exps(texto, pos)) { // se <op_cat3>
            if (op_cat3(texto, pos)) {
                goto repete;
            }

        } else {
            return 1;
        }
    }
    return 0;
}

int exps(char texto[], int *pos) { // <exps> → <tmo>{<op_cat2><tmo>}
    if (tmo(texto, pos)) {
        
        if (op_cat2(texto, pos)) { // se op_cat2
            if (tmo(texto, pos)) {
                return 1;
            }
            return 0;

        } else return 1;
    } 
    return 0;
}

int tmo(char texto[], int *pos) { // <tmo> → <ftr>{<op_cat1><ftr>}
    if (ftr(texto, pos)) {

        if (op_cat1(texto, pos)) { // se op_cat1
            if (ftr(texto, pos)) {
                return 1;
            }
            return 0;

        } else {
            return 1;
        }
    }

    return 0;
}

int ftr(char texto[], int *pos) { // <ftr> → <e>|sNOT<ftr>|(<exp>)

    if (e(texto, pos)) { // se <e>
        return 1;

    } else if (lookahead == 'n') { // se sNOT<ftr>
        if (check('n', texto, pos) && check('o', texto, pos) && check('t', texto, pos)) {
            if (ftr(texto, pos)) {
                return 1;
            }
        }
        return 0;

    } else if (lookahead == '(') { // se (<expr>)
        if (check('(', texto, pos)) {
            if (expr(texto, pos)) {
                if (check(')', texto, pos)) {
                    return 1;
                }
            }
        }
        return 0;
    }

    return 0;
}

int op_cat3(char texto[], int *pos) { // <op_cat3> → sMAIOR|sMAIOR_IGUAL|sMENOR|sMENOR_IGUAL|sIGUAL|sDIFERENTE

    if (lookahead == '>') { // se >
        if (check('>', texto, pos)) { 

            if (lookahead == '=') { // se >=
                if (check('=', texto, pos)) {
                    return 1; // se >=
                } return 0;

            } else return 1; // se >
        }
        return 0;

    } else if (lookahead == '<') { // se <
        if (check('<', texto, pos)) {

            if (lookahead == '=') { // se <=
                if (check('=', texto, pos)) {
                    return 1; // se <=
                } return 0;

            } else return 1; // se <
        }

    } else if (lookahead == '=') { // se ==
        if (check('=', texto, pos)) {
            if (lookahead == '=') {
                if (check('=', texto, pos)) {
                    return 1; // se ==
                } 
            }
        }
        return 0;

    } else if (lookahead == '!') {
        if (check('!', texto, pos)) {
            if (lookahead == '=') {
                if (check('=', texto, pos)) {
                    return 1;
                }
            }
        }
        return 0;
    }
    return 0;
}

int op_cat2(char texto[], int *pos) { // <op_cat2> → sSOMA|sSUBTR|sOR
    if (lookahead == '+' || lookahead == '-') { // se sSoma || sSUBTR
        return 1;

    } else if (lookahead == 'o') { // se sOR
        if (check('o', texto, pos) && check('r', texto, pos)) {
            return 1;
        }
    }
    return 0;
}

int op_cat1(char texto[], int *pos) { // <op_cat1> → sMULT|sDIV|sAND
    if (lookahead == '*' || lookahead == '/') {
        return 1;

    } else if (lookahead == 'a') {
        if (check('a', texto, pos) && check('n', texto, pos) && check('d', texto, pos)) {
            return 1;
        }
    } else {
        return 0;
    }
}

char *buffer; // Buffer de memória com o conteúdo lido do arquivo

int main() {  
  
    char textoFormatada[1000000];
  
    // FILE *arq = fopen(argv[1], "r");
    FILE *arq = fopen("ex4.txt", "r");
    if (arq == NULL)
    {
        fprintf(stderr, "Erro ao tentar abrir o aquivo ex0.txt\n");
        return -1;
    }
    fseek(arq, 0, SEEK_END);
    int tam_arq = ftell(arq);
    fseek(arq, 0, SEEK_SET);

    size_t retornoFread;

    buffer = (char *)calloc(tam_arq, sizeof(char));
    if (buffer == NULL)
        exit(-1);
    else
        retornoFread = fread(buffer, sizeof(char), tam_arq, arq);

    fclose(arq);

    int i = 0;
    printf("Pre loop\n");
    while((*buffer != EOF && *buffer != '\0')) //Remove espacos do codigo
    {
    if((*buffer != ' ') && (*buffer != '\t') && (*buffer != '\r') && (*buffer != '\n'))
    {
        if(*buffer == *"{") //Remove comentarios
        {
        while(*buffer != *"}")
        {
            buffer++;
        }
        }
        else if ((*buffer != ' ') && (*buffer != '\t') && (*buffer != '\r') && (*buffer != '\n'))
        { //Retira os espacos colocados no lugar dos comentarios
        textoFormatada[i] = *buffer;
        i++;
        }
    }
    buffer++;
    }
    printf("fim do loop\n");
  
    int pos = 0;

    lookahead = textoFormatada[pos];
    int DeuCerto = ini(textoFormatada, &pos);
    if (DeuCerto == 1)
        printf("\nCodigo \n\n%s\n\nreconhecida\n\n", textoFormatada);
    else printf("Deu ruim\n");
    return (0);
}