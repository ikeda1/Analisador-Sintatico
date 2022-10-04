#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


/*
    Pedro Henrique Ikeda - 32016344

    Estrategia de recuperacao de erro adotado foi a de Modo Pânico,
    assim que for encontrado um erro, o codigo para imediatamente e reporta
    o erro encontrado ao programador, para que o mesmo resolva o problema

    Para utilizar o programa passamos um arquivo de texto, sem conter acentos,
    pois a linguagem C nao trabalha muito bem com eles

    Link com os arquivos: https://drive.google.com/file/d/1d147lQcapHxj4QHXy1hIEhoKbiimFRA8/view?usp=sharing

    Exemplo de execucao:
*/

char lookahead; /* Excepcionalmente variavel global */

// Inicializando as funções
int id(char texto[], int *pos);
int dcl(char texto[], int *pos);
int ini(char texto[], int *pos);
int digito(char texto[], int *pos);
int tpo(char texto[], int *pos);
int sub(char texto[], int *pos);
int param(char texto[], int *pos);
int bco(char texto[], int *pos);
int cmd(char texto[], int *pos);
int wr(char texto[], int *pos);
int rd(char texto[], int *pos);
int retorna(char texto[], int *pos);
int IF(char texto[], int *pos);
int fr(char texto[], int *pos);
int wh(char texto[], int *pos);
int rpt(char texto[], int *pos);
int atr(char texto[], int *pos);
int f(char texto[], int *pos);
int e(char texto[], int *pos);
int expr(char texto[], int *pos);
int exps(char texto[], int *pos);
int tmo(char texto[], int *pos);
int ftr(char texto[], int *pos);
int op_cat3(char texto[], int *pos);
int op_cat2(char texto[], int *pos);
int op_cat1(char texto[], int *pos);


int check(char t, char texto[], int *pos){ //Verifica o simbolo e casa
  if (lookahead == t) {
    // printf("%c ", lookahead); // DEBUG
    lookahead = texto[++(*pos)];
    return (1);
  }
  return (0);
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
                            }
                        } else return 0;

                    } else if (bco(texto, pos)) {
                        if (check('.', texto, pos)){
                            return 1;
                        } else return 0;
                    }

                } else if (sub(texto, pos)) {
                    if (bco(texto, pos)) {
                        if (check('.', texto, pos)) {
                            return 1;
                        }
                    } else return 0;

                } 
                else if (bco(texto, pos)) {
                    if (check('.', texto, pos)) {
                        return 1;
                    }
                } else return 0;
            } 
        }
    }
    return 0;
}

int id(char texto[], int *pos){ // <id> -> sIDENT
  if(isalpha(lookahead) || (lookahead >= '0' && lookahead <= '9') || lookahead == '_')
  {
    printf("  ");
    char tmp = lookahead;
    if(check(lookahead, texto, pos) && id(texto, pos))
    {
      printf("%c", tmp);
      return (1);
    }
    else return (1);
  }
  else if(lookahead == ';' || lookahead == ')' || lookahead == ',') //Para ajudar o codigo a evitar falhas por comparacoes possivelmente problematicas
  {
    return (1);
  }
  else 
    return(0);
}

// int id(char texto[], int *pos) {
//     printf("id eh ");
//     while (isalpha(lookahead) || isdigit(lookahead) || (lookahead == '_')) {
//         printf("%c", lookahead);
//         check(lookahead, texto, pos);
//         if(lookahead == ';' || lookahead == '[' || lookahead == '(') {
//             return 1;
//         }
//     }     
//     printf("\nfim do id\n");
//     return 0;
// }

int let(char texto[], int *pos){ //Letra ou simbolo
  if(isalpha(lookahead) || (lookahead >= '0' && lookahead <= '9') || lookahead == '_'
    || lookahead == ' ' || lookahead == ':' || lookahead == '~' || lookahead == '^'
    || lookahead == '-' || lookahead == '+' || lookahead == '*' || lookahead == '/' 
    || lookahead == '>' || lookahead == ':' || lookahead == '[' || lookahead == '.' 
    || lookahead == ']' || lookahead == '{' || lookahead == '}' || lookahead == ';' || lookahead == '#')
  {
    if(check(lookahead, texto, pos) && let(texto, pos)) //Caso seja frase.
    {
      return (1);
    }
    else return (0);
  }
  else if(lookahead == '\'' || lookahead == '\"') //Para auxiliar string e char
  {
    return (1);
  }
  else 
    return(0);
}

int dcl(char texto[], int *pos){ //<dcl> → sVAR <tpo><id>[[sNUM]]{,<id>[[sNUM]]} {<tpo><id>[[sNUM]]{,<id>[[sNUM]]};}

  if(lookahead == 'v')
  {
    if(check('v', texto, pos) && check('a', texto, pos) && check('r', texto, pos)) //sVAR
    {
      if(tpo(texto, pos) && id(texto, pos)) // <tpo><id>
      {
    novaVar:
        if(lookahead == '[') // [[sNUM]]
        { 
          if (check('[', texto, pos) && digito(texto, pos) && check(']', texto, pos)) // sVAR <tpo><id>[[sNUM]]
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
                    if(check('[', texto, pos) && digito(texto, pos) && check(']', texto, pos))
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
              if(check('[', texto, pos) && digito(texto, pos) && check(']', texto, pos)) // id[[sNUM]]
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

int digito(char texto[], int *pos) { // Numero intero ou decimal
  if (lookahead >= '0' && lookahead <= '9')
  {
    if (check(lookahead, texto, pos) && digito(texto, pos))
    {
      return (1);
    } 
    else
      return (0);
  }
  else if(lookahead == '.')
  {
    if(check(lookahead , texto, pos) && digito(texto, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else
    return (1); // D -> Palavra Vazia
}
  
int tpo(char texto[], int *pos){ //<tpo> → sINT|sFLOAT|sCHAR
  if(lookahead == 'i')
  {
    if(check('i', texto, pos) && check('n', texto, pos) && check('t', texto, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if(lookahead == 'f')
  {
    if(check('f', texto, pos) && check('l', texto, pos) && check('o', texto, pos) && check('a', texto, pos) && check('t', texto, pos))
    {
      return (1);
    }
    else 
      return (0);
  }
  else if(lookahead == 'c')
  {
    if(check('c', texto, pos) && check('h', texto, pos) && check('a', texto, pos) && check('r', texto, pos))
    {
      return (1);
    }
    else return (0);
  }
  else
    return (0);
}

int sub(char texto[], int *pos){ //<sub> → sSUBROT (sVOID|<tpo>)<id>([<param>])[<dcl>][<sub>]<bco>; {(sVOID|<tpo>)<id>([<param>])[<dcl>][<sub>]<bco>;}
  if(lookahead == 's')
  {
    if(check('s', texto, pos) && check('u', texto, pos) && check('b', texto, pos) 
      && check('r', texto, pos) && check('o', texto, pos) && check('t', texto, pos))
    {
    subrotina:
      if(lookahead == 'v')
      {
        if(check('v', texto, pos) && check('o', texto, pos) && check('i', texto, pos) && check('d', texto, pos))
        {
          if(id(texto, pos) && check('(', texto, pos))
          {
            NovoParam:
            if(lookahead == ')')
            {
              if(check(')', texto, pos))
              {
                if(dcl(texto, pos))
                {
                  if(sub(texto, pos))
                  {
                    if(bco(texto, pos) && check(';', texto, pos))
                    {
                      if(lookahead == 'v' || tpo(texto, pos))
                      {
                        goto subrotina; // Para fazer o {(sVOID|<tpo>)<id>([<param>])[<dcl>][<sub>]<bco>;}
                      }
                      else
                        return (1);
                    }
                    else
                      return (0);
                  }
                  else if(bco(texto, pos) && check(';', texto, pos))
                  {
                    if(lookahead == 'v' || tpo(texto, pos)) // (sVOID | <tpo>)
                    {
                      goto subrotina;
                    }
                    else
                      return (1);
                  }
                  else
                    return (0);
                }
                else if(sub(texto, pos)) //[<sub>]
                {
                  if(bco(texto, pos) && check(';', texto, pos)) // <bco>;
                  {
                    if(lookahead == 'v' || tpo(texto, pos))
                    {
                      goto subrotina;
                    }
                    else
                      return (1);   
                  }
                  else
                    return (0);
                }
                else if(bco(texto, pos))
                {
                  if(lookahead == 'v' || tpo(texto, pos)) // (sVOID | <tpo>)
                  {
                    goto subrotina;
                  }
                  else
                    return (1); 
                }
                else
                  return (0);
              }
              else
                return (0);
            }
            else if(param(texto, pos)) //[param]
            {
              if(lookahead == ')')
              {
                if(check(')', texto, pos))
                {
                  if(dcl(texto, pos))
                  {
                    if(sub(texto, pos))
                    {
                      if(bco(texto, pos) && check(';', texto, pos))
                      {
                        if(lookahead == 'v' || tpo(texto, pos))
                        {
                          goto subrotina;
                        }
                        else
                          return (1);
                      }
                      else
                        return (0);
                    }
                    else if(bco(texto, pos) && check(';', texto, pos))
                    {
                        if(lookahead == 'v' || tpo(texto, pos))
                        {
                          goto subrotina;
                        }
                        else
                          return (1);
                    }
                    else
                      return (0);
                  }
                  else if(sub(texto, pos))
                  {
                    if(bco(texto, pos) && check(';', texto, pos))
                    {
                      if(lookahead == 'v' || tpo(texto, pos))
                      {
                        goto subrotina;
                      }
                      else
                        return (1);
                    }
                    else
                      return (0);
                  }
                  else if(bco(texto, pos) && check(';', texto, pos))
                  {
                      if(lookahead == 'v' || tpo(texto, pos))
                      {
                        goto subrotina;
                      }
                      else
                        return (1);
                  }
                }
              }
              else if(lookahead == ',')
              {
                if(check(',', texto, pos))
                {
                  goto NovoParam;
                }
              }
              else
                return (0);
            }
            else if(lookahead == ',')
            {
              if(check(',', texto, pos))
              {
                goto NovoParam;
              }
            }
            else
              return(0);
          }
        }
        else
          return (0);
      }  
      else if(tpo(texto, pos))
      {
        if(id(texto, pos) && check('(', texto, pos))
          {
            NovoParam2:
            if(lookahead == ')')
            {
              if(check(')', texto, pos))
              {
                if(dcl(texto, pos))
                {
                  if(sub(texto, pos))
                  {
                    if(bco(texto, pos) && check(';', texto, pos))
                    {
                      if(lookahead == 'v' || tpo(texto, pos) || id(texto, pos))
                      {
                        goto subrotina;
                      }
                      else
                        return (1);
                    }
                  }
                }
                else if(sub(texto, pos))
                {
                  if(bco(texto, pos) && check(';', texto, pos))
                  {
                    if(lookahead == 'v' || tpo(texto, pos) || id(texto, pos))
                    {
                      goto subrotina;
                    }
                    else
                      return (1);   
                  }
                }
              }
            }
            else if(param(texto, pos))
            {
            if(lookahead == ')')
              {
                if(check(')', texto, pos))
                {
                  if(dcl(texto, pos))
                  {
                    if(sub(texto, pos))
                    {
                      if(bco(texto, pos) && check(';', texto, pos))
                      {
                        if(lookahead == 'v' || tpo(texto, pos) || id(texto, pos))
                        {
                          goto subrotina;
                        }
                        else
                          return (1);
                      }
                    }
                  }
                  else if(sub(texto, pos))
                  {
                    if(bco(texto, pos) && check(';', texto, pos))
                    {
                      if(lookahead == 'v' || tpo(texto, pos) || id(texto, pos))
                      {
                        goto subrotina;
                      }
                      else
                        return (1);
                    }
                  }
                }
              }
            else if(lookahead == ',')
              {
                if(check(',', texto, pos))
                {
                  goto NovoParam2;
                }
              }
            }
            else if(lookahead == ',')
            {
              if(check(',', texto, pos))
              {
                goto NovoParam2;
              }
            }
          }
      }
      else if(id(texto, pos) && check('(', texto, pos))
      {
        NovoParam3:
        if(lookahead == ')')
        {
          if(check(')', texto, pos))
          {
            if(dcl(texto, pos))
            {
              if(sub(texto, pos))
              {
               if(bco(texto, pos) && check(';', texto, pos))
               {
                if(lookahead == 'v' || tpo(texto, pos) || id(texto, pos))
                {
                  goto subrotina;
                }
                else
                  return (1);
               }
              else
                 return (0); 
              }
              else
                return(0);
            }
            else if(sub(texto, pos))
            {
              if(bco(texto, pos) && check(';', texto, pos))
              {
                if(lookahead == 'v' || tpo(texto, pos) || id(texto, pos))
                {
                  goto subrotina;
                }
                else
                  return (1);   
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
        else if(param(texto, pos))
        {
          if(lookahead == ')')
          {
            if(check(')', texto, pos))
            {
              if(dcl(texto, pos))
              {
                if(sub(texto, pos))
                {
                 if(bco(texto, pos) && check(';', texto, pos))
                 {
                  if(lookahead == 'v' || tpo(texto, pos) || id(texto, pos))
                  {
                   goto subrotina;
                  }
                  else
                    return (1);
                 }
                  else
                   return (0);
                }
                else
                  return (0);
              }
              else if(sub(texto, pos))
              {
                if(bco(texto, pos) && check(';', texto, pos))
                {
                  if(lookahead == 'v' || tpo(texto, pos) || id(texto, pos))
                  {
                    goto subrotina;
                  }
                  else
                    return (1);
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
          else if(lookahead == ',')
          {
            if(check(',', texto, pos))
            {
              goto NovoParam3;
            }
          }
          else
            return (0);
        }
        else if(lookahead == ',')
        {
          if(check(',', texto, pos))
          {
            goto NovoParam3;
          }
        }
        else
          return(0);
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
  
int param(char texto[], int *pos){ //<param> → <tpo><id>{;<tpo><id>}
  if(tpo(texto, pos) && id(texto, pos))
  {
    if(lookahead == ';')
    {
      if(check(';', texto, pos) && tpo(texto, pos) && id(texto, pos))
      {
        return (1);
      }
      else
        return (0);
    }
    else
      return (1);
  }
  else
    return (0);
}
  
int cmd(char texto[], int *pos){ //<cmd> → <wr>|<rd>|<if>|<fr>|<wh>|<rpt>|<atr>|<f>|<bco>
  if(wr(texto, pos)){
    return (1);
  }
  else if(rd(texto, pos)){
    return (1);
  }
  else if(retorna(texto, pos)){
    return (1);
  }
  else if(IF(texto, pos)){
    return (1);
  }
  else if(fr(texto, pos)){
    return (1);
  }
  else if(wh(texto, pos)){
    return (1);
  }
  else if(rpt(texto, pos)){
    return (1);
  }
  else if(atr(texto, pos)){
    return (1);
  }
  else if(f(texto, pos)){
    return (1);
  }
  else if(bco(texto, pos)){
    return (1);
  }
  else
    return (0);
}

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
                            if (digito(texto, pos)) {
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

int retorna(char texto[], int *pos) {
  if(lookahead == 'r')
  {
    if(check('r', texto, pos) && check('e', texto, pos) && check('t', texto, pos) && check('u', texto, pos) && check('r', texto, pos) && check('n', texto, pos))
    {
      if(e(texto, pos))
      {
        return (1);
      }
    }
  }
  else if(lookahead == 't')
  {
    if(check('t', texto, pos) && check('u', texto, pos) && check('r', texto, pos) && check('n', texto, pos)) //Por conta do read as vezes é marcado o re de return então para funcao funcionar temos isso de forma auxiliar
    {
      if(expr(texto, pos))
      {
        return (1);
      }
    }
  }
  else
    return (0);
}

int IF(char texto[], int *pos) { // <if> → IF(<exp>)sTHEN<cmd>[sELSE<cmd>]
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

int fr(char texto[], int *pos){ //<fr> → sFOR(<atr>;<exp>;<atr>)<cmd>
  if(lookahead == 'f')
  {
    if(check('f', texto, pos) && check('o', texto, pos) && check('r', texto, pos))
    {
      if(lookahead == '(')
      {
        if(check('(', texto, pos) && atr(texto, pos) && check(';', texto, pos) && expr(texto, pos) && 
          check(';', texto, pos) && atr(texto,pos) &&
          check(')', texto, pos) && cmd(texto, pos))
        {
          return (1);
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
  
int wh(char texto[], int *pos){ //<wh> → sWHILE(<exp>)<cmd>
  if(lookahead == 'w')
  {
    if(check('w', texto, pos) && check('h', texto, pos) && check('i', texto, pos) && check('l', texto, pos) && check('e', texto, pos))
    {
      segueWhile:
     if(check('(', texto, pos) && expr(texto, pos) && check(')', texto, pos))
     {
       if(cmd(texto, pos))
       {
         return (1);
       }
       else 
         return (0);
     }
     else
       return(0);
    }
    else
      return(0);
  }
  else if(lookahead == 'h') //Por conta da funcao write, as vezes acaba sendo marcado o w inicial
  {
    if(check('h', texto, pos) && check('i', texto, pos) && check('l', texto, pos) && check('e', texto, pos))
      {
       goto segueWhile;
      }
  }
  else
    return (0);
}
  
int rpt(char texto[], int *pos){ //<rpt> → sREPEAT{<cmd>}sUNTIL(<exp>)
  if(lookahead == 'r')
  {
    if(check('r', texto, pos) && check('e', texto, pos) && check('p', texto, pos) && check('e', texto, pos) &&
      check('a', texto, pos) && check ('t', texto, pos))
    {
        if(cmd(texto, pos))
        {
          if(lookahead == 'u')
          {
            if(check('u', texto, pos) && check('n', texto, pos) && check('t', texto, pos) && check('i', texto, pos),
            check('l', texto, pos))
            {
              if(lookahead == '(')
              {
                if(check('(', texto, pos) && expr(texto, pos) && check(')', texto, pos))
                {
                 return (1);
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
        else if(lookahead == 'u')
        {
          if(check('u', texto, pos) && check('n', texto, pos) && check('t', texto, pos) && check('i', texto, pos)
          && check('l', texto, pos))
          {
            if(lookahead == '(')
            {
              if(check('(', texto, pos) && expr(texto, pos) && check(')', texto, pos))
              {
               return (1);
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
    else
      return (0);
  }
  else
    return (0);
}

int atr(char texto[], int *pos){ //<atr> → <id>[[sNUM]]sATRIB<exp>
  if(id(texto, pos))
  {
    if(lookahead == '[')
    {
      if(check('[', texto, pos) && digito(texto, pos) && check(']', texto, pos))
      {
        if(lookahead == '<')
        {
          if(check('<', texto, pos) && check ('-', texto, pos))
          {
            if(expr(texto, pos))
            {
              return (1);
            }
            else return (0);
          }
          else return(0);
        }
        else return(0);
      }
      else return (0);
    }
    else if(lookahead == '<')
    {
      if(check('<', texto, pos) && check ('-', texto, pos))
      {
        if(expr(texto, pos))
        {
          return (1);
        }
        else return (0);
      }
      else return (0);
    }
    else 
      return (0);
  }
  else if(lookahead == '[')
  {
      if(check('[', texto, pos) && digito(texto, pos) && check(']', texto, pos)){
        if(lookahead == '<'){
          if(check('<', texto, pos) && check ('-', texto, pos)){
            if(expr(texto, pos)){
              return (1);
            }
            else return (0);
          }
          else return(0);
        }
        else return(0);
      }
      else return (0);
  }
  else if(lookahead == '<')
  {
      if(check('<', texto, pos) && check ('-', texto, pos)){
        if(expr(texto, pos))
        {
          return (1);
        }
        else return (0);
      }
      else return (0);
  }
  else
    return (0);
}
  
int f(char texto[], int *pos){ //<f> → <id>(<exp>{,<exp>})
  if(id(texto, pos))
  {
    if(lookahead == '(')
    {
      if(check('(', texto, pos) && expr(texto, pos))
      {
        if(lookahead == ')')
        {
          if(check(')', texto, pos))
          {
            return (1);
          }
          else
            return (0);
        }
        else if (lookahead == ',')
        {
          if(check(',', texto, pos) && expr(texto, pos) && check(')', texto, pos))
          {
            return (1);
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
  else if(lookahead == '(')
    {
      if(check('(', texto, pos) && expr(texto, pos))
      {
        if(lookahead == ')')
        {
          if(check(')', texto, pos))
          {
            return (1);
          }
          else
            return (0);
        }
        else if (lookahead == ',')
        {
          if(check(',', texto, pos) && expr(texto, pos) && check(')', texto, pos))
          {
            return (1);
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

int e(char texto[], int *pos){ //<e> → sSTRING|sNUM|sCHAR|<id>[[sNUM]]|<f>
  if(lookahead == '\'')  // sCHAR
  {
    if(check('\'', texto, pos) && let(texto, pos) && check('\'', texto, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if((lookahead >= '0' && lookahead <='9') || lookahead == '.') //sNUM 
  {
    if(digito(texto, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if(lookahead == '\"')
  {
    if(check('\"', texto, pos) && let(texto, pos) && check('\"', texto, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if(id(texto, pos))
  {
    if(lookahead == '[')
    {
      if(check('[', texto, pos) && digito(texto, pos) && check(']', texto, pos))
      {
        return (1);
      }
      else
        return (0);
    }
    else if(lookahead == '(') //Caso tenha um ID mas na verdade seja um f
    {
      if(f(texto, pos))
      {
        return (1);
      }
    }
    else
      return (1);
  }
  else if(f(texto, pos))
  {
    return (1);
  }
  else
    return (0);
}
  
int expr(char texto[], int *pos){ //<exp> → <exps>{<op_cat3><exps>} 
  NovaExp:
  if(exps(texto, pos))
  {
    if(op_cat3(texto, pos)&& exps(texto, pos))
    {
      if(op_cat3(texto, pos)) //Caso tenha mais operadores
      {
        goto NovaExp;
      }
      return (1);
    }
    else
      return (1);
  }
  else
    return (0);
}
  
int exps(char texto[], int *pos){ //<exps> → <tmo>{<op_cat2><tmo>}
  NovaExp:
  if(tmo(texto, pos))
  {
    if(op_cat2(texto, pos) && tmo(texto, pos))
    {
      if(op_cat2(texto, pos))
      {
        goto NovaExp; //Caso tenha novos operadores
      }
      return (1);
    }
    else
      return (1);
  }
  else
    return (0);
}

int tmo(char texto[], int *pos){ //<tmo> → <ftr>{<op_cat1><ftr>}
  NovaExp:
  if(ftr(texto, pos))
  {
    if(op_cat1(texto, pos) && ftr(texto, pos))
    {
      if(op_cat1(texto, pos))
      {
        goto NovaExp; //Caso tenha novo operador
      }
      return (1);
    }
    else
      return (1);
  }
  else
    return (0);
}
  
int ftr(char texto[], int *pos){ //<ftr> → <e>|sNOT<ftr>|(<exp>)
  if(lookahead == 'n') // ftr -> not<ftr>
  { 
    if(check('n', texto, pos) && check('o', texto, pos) && check('t', texto, pos)){
      if(ftr(texto, pos)){
        return (1);
      }
      else 
        return (0);
    }
    else 
      return (0);
  }
  else if(lookahead == '(') //ftr -> (exp)
  {
    if(check('(', texto, pos) && expr(texto, pos) && check(')', texto, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if(e(texto, pos)) // ftr -> <e>
  {
    return(1);
  }
  else
    return (0);
}
  
int op_cat3(char texto[], int *pos){ //<op_cat3> → sMAIOR|sMAIOR_IGUAL|sMENOR| sMENOR_IGUAL|sIGUAL|sDIFERENTE
  if(lookahead == '>')
  {
    if(check('>', texto, pos))
    {
      if(lookahead == '=')
      {
        if(check('=', texto, pos)) //Simbolo maior igual
        {
          return (1);
        }
        else
          return (0);
      }
      else
        return (1); //Simbolo Maior
    }
    else
      return (0);
  }
  else if(lookahead == '<') 
  {
    if(check('<', texto, pos)) // sMenor
    {
      if(lookahead == '=')
      {
        if(check('=', texto, pos)){ //sMenorIgual
          return (1);
        }
        else
          return (0);
      }
      else
        return (1);
    }
    else
      return (0);
  }
  else if(lookahead == '=')
  {
    if(check('=', texto, pos)) // sIgual
    {
      if(lookahead == '=')
      {
        if(check('=', texto, pos))
        {
          return (1);
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
  if(lookahead == '!')
  {
    if(check('!', texto, pos)) //sDiferente
    {
      if(lookahead == '=')
      {
        if(check('=', texto, pos))
        {
          return (1);
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
  
int op_cat2(char texto[], int *pos){ //<op_cat2> → sSOMA|sSUBTR|sOR
  if(lookahead == '+')
  {
    if(check('+', texto, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if(lookahead == '-')
  {
    if(check('-', texto, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if(lookahead == 'o')
  {
    if(check('o', texto, pos) && check('r', texto, pos)){
      return (1);
    }
    else
      return (0);
  }
  else
    return (0); //Simb opcional
}
  
int op_cat1(char texto[], int *pos){ //<op_cat2> → sSOMA|sSUBTR|sOR
  if(lookahead == '*'){
    if(check('*', texto, pos)){
      return (1);
    }
    else
      return (0);
  }
  else if(lookahead == '/'){
    if(check('/', texto, pos)){
      return (1);
    }
    else
      return (0);
  }
  else if(lookahead == 'a'){
    if(check('a', texto, pos) && check('n', texto, pos) && check('d', texto, pos)){
      return (1);
    }
    else
      return (0);
  }
  else
    return (0); 
}

  
void ErroPanico() { //Assim que o return 0 de uma funcao nao for opcional entao temos o erro apontado por essa funcao
  printf("\n\nPALAVRA NÃO ACEITA\n\n");
  printf("\n\nFOI ACHADO UM ERRO NA LETRA %c\n\n", lookahead);
 }


char *buffer; // Buffer de memória com o conteúdo lido do arquivo

int main(int argc, char *argv[]) {  
  
  char palavraFormatada[1000000];
  
    // FILE *arq = fopen(argv[1], "r");
    FILE *arq = fopen("codigo6.txt", "r");
    if (arq == NULL)
    {
     fprintf(stderr, "Erro ao tentar abrir o aquivo \"%s\"", argv[1]);
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
       palavraFormatada[i] = *buffer;
       i++;
     }
   }
  buffer++;
  }
  
   int pos = 0;

  lookahead = palavraFormatada[pos];
  int DeuCerto = ini(palavraFormatada, &pos);
  if (DeuCerto == 1)
    printf("\nCodigo \n\n%s\n\nreconhecida\n\n", palavraFormatada);
  else if(DeuCerto == 0)
    ErroPanico();
  return (0);
}