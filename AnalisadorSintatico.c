#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


/*
    Luis Henrique Bastos Tamura TIA: 32050151

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
int id(char palavra[], int *pos);
int dcl(char palavra[], int *pos);
int ini(char palavra[], int *pos);
int digito(char palavra[], int *pos);
int tpo(char palavra[], int *pos);
int sub(char palavra[], int *pos);
int param(char palavra[], int *pos);
int bco(char palavra[], int *pos);
int cmd(char palavra[], int *pos);
int wr(char palavra[], int *pos);
int rd(char palavra[], int *pos);
int retorna(char palavra[], int *pos);
int sIF(char palavra[], int *pos);
int fr(char palavra[], int *pos);
int wh(char palavra[], int *pos);
int rpt(char palavra[], int *pos);
int atr(char palavra[], int *pos);
int f(char palavra[], int *pos);
int e(char palavra[], int *pos);
int exp1(char palavra[], int *pos);
int exps(char palavra[], int *pos);
int tmo(char palavra[], int *pos);
int ftr(char palavra[], int *pos);
int op_cat3(char palavra[], int *pos);
int op_cat2(char palavra[], int *pos);
int op_cat1(char palavra[], int *pos);


int match(char t, char palavra[], int *pos){ //Verifica o simbolo e casa
  if (lookahead == t) {
    lookahead = palavra[++(*pos)];
    return (1);
  }
  return (0);
}

int ini(char palavra[], int *pos) // sPRG<id>;[<dcl>][<sub>]<bco>.
{ 
  if (lookahead == 'p')
  {
    if(match('p', palavra, pos) && match('r', palavra, pos) && match('g', palavra, pos)) //Match do simbolo do prg
    {
      if(id(palavra, pos) && match(';', palavra, pos)) //<id>;
      {
        if(dcl(palavra, pos)) //[<dcl]
        {
          if(sub(palavra, pos)) //[<sub>]
          {
            if(bco(palavra, pos) && match('.', palavra, pos)){ //<bco>.
              return (1);
            }
            else
              return (0);
          }
          else if(bco(palavra, pos)) // <bco>
          {
            if(lookahead == ('.')) // .
            {
              if(match('.', palavra, pos)){
                return (1);
              }
              else
                return (0);
            }
            else
              return (0);
          }
          else
            return(0);
        }
        else if(sub(palavra, pos)) // [<sub]
        {
          if(bco(palavra, pos) && match('.', palavra, pos)){ // <bco>.
            return (1);
          }
          else
            return (0);
        }
        else if(bco(palavra, pos) && match('.', palavra, pos)) // <bco>.
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
      return(0);
  }
  else
    return (0);
}

int id(char palavra[], int *pos){ // <id> -> sIDENT
  if(isalpha(lookahead) || (lookahead >= '0' && lookahead <= '9') || lookahead == '_')
  {
    if(match(lookahead, palavra, pos) && id(palavra, pos))
    {
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

int let(char palavra[], int *pos){ //Letra ou simbolo
  if(isalpha(lookahead) || (lookahead >= '0' && lookahead <= '9') || lookahead == '_'
    || lookahead == ' ' || lookahead == ':' || lookahead == '~' || lookahead == '^'
    || lookahead == '-' || lookahead == '+' || lookahead == '*' || lookahead == '/' 
    || lookahead == '>' || lookahead == ':' || lookahead == '[' || lookahead == '.' 
    || lookahead == ']' || lookahead == '{' || lookahead == '}' || lookahead == ';' || lookahead == '#')
  {
    if(match(lookahead, palavra, pos) && let(palavra, pos)) //Caso seja frase.
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

int dcl(char palavra[], int *pos){ //<dcl> → sVAR <tpo><id>[[sNUM]]{,<id>[[sNUM]]} {<tpo><id>[[sNUM]]{,<id>[[sNUM]]};}

  if(lookahead == 'v')
  {
    if(match('v', palavra, pos) && match('a', palavra, pos) && match('r', palavra, pos)) //sVAR
    {
      if(tpo(palavra, pos) && id(palavra, pos)) // <tpo><id>
      {
    novaVar:
        if(lookahead == '[') // [[sNUM]]
        { 
          if (match('[', palavra, pos) && digito(palavra, pos) && match(']', palavra, pos)) // sVAR <tpo><id>[[sNUM]]
            {
              if(lookahead == ';') // sVAR <tpo><id>[[sNUM]];
              {
                if(match(';', palavra, pos))
                {
                  return (1);
                }
                else
                  return (0);
              }
              else if(lookahead == ',') //Caso tenha mais id
              {
                if(match(',', palavra, pos) && id(palavra, pos)) // , <id>
                {
                  if(lookahead == '[')
                  { // ,<id>[[sNUM]]
                    if(match('[', palavra, pos) && digito(palavra, pos) && match(']', palavra, pos))
                    {
                      if(lookahead == ';')
                      {
                        if(match(';', palavra, pos)) // , <id>[[sNUM]] ;
                        {
                          if(tpo(palavra, pos) && id(palavra, pos))
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
                    if(match(';', palavra, pos))
                    {
                      if(tpo(palavra, pos) && id(palavra, pos)) //{<tpo><id>}
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
          if(match(',', palavra, pos) && id(palavra, pos))
          {
            if(lookahead == '[')
            {
              if(match('[', palavra, pos) && digito(palavra, pos) && match(']', palavra, pos)) // id[[sNUM]]
              {
                if(lookahead == ';')
                {
                  if(match(';', palavra, pos))
                  {
                    if(tpo(palavra, pos) && id(palavra, pos)) //{<tpo>}{<id>}
                    {
                      goto novaVar;
                    }
                    else
                      return (1);
                  }
                }
                else if(lookahead == ',') // {, <id>}
                {
                  if(match(',', palavra, pos) && id(palavra, pos))
                  {
                    goto novaVar; //Caso tenha nova variavel
                  }
                }
              }
            }
            else if(lookahead == ';')
            {
              if(match(';', palavra, pos))
              {
                if(tpo(palavra, pos) && id(palavra, pos)) //{<tpo>}{id}
                {
                  goto novaVar; //Caso tenha uma nova var
                }
                else
                   return (1);
              }
            }
            else if(lookahead == ',')
            {
              if(match(',', palavra, pos) && id(palavra, pos)) //{, <id>}
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
          if(match(';', palavra, pos))
          {
            if(tpo(palavra, pos) && id(palavra, pos)) // {<tpo>}{<id>}
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

int digito(char palavra[], int *pos) { // Numero intero ou decimal
  if (lookahead >= '0' && lookahead <= '9')
  {
    if (match(lookahead, palavra, pos) && digito(palavra, pos))
    {
      return (1);
    } 
    else
      return (0);
  }
  else if(lookahead == '.')
  {
    if(match(lookahead , palavra, pos) && digito(palavra, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else
    return (1); // D -> Palavra Vazia
}
  
int tpo(char palavra[], int *pos){ //<tpo> → sINT|sFLOAT|sCHAR
  if(lookahead == 'i')
  {
    if(match('i', palavra, pos) && match('n', palavra, pos) && match('t', palavra, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if(lookahead == 'f')
  {
    if(match('f', palavra, pos) && match('l', palavra, pos) && match('o', palavra, pos) && match('a', palavra, pos) && match('t', palavra, pos))
    {
      return (1);
    }
    else 
      return (0);
  }
  else if(lookahead == 'c')
  {
    if(match('c', palavra, pos) && match('h', palavra, pos) && match('a', palavra, pos) && match('r', palavra, pos))
    {
      return (1);
    }
    else return (0);
  }
  else
    return (0);
}

int sub(char palavra[], int *pos){ //<sub> → sSUBROT (sVOID|<tpo>)<id>([<param>])[<dcl>][<sub>]<bco>; {(sVOID|<tpo>)<id>([<param>])[<dcl>][<sub>]<bco>;}
  if(lookahead == 's')
  {
    if(match('s', palavra, pos) && match('u', palavra, pos) && match('b', palavra, pos) 
      && match('r', palavra, pos) && match('o', palavra, pos) && match('t', palavra, pos))
    {
    subrotina:
      if(lookahead == 'v')
      {
        if(match('v', palavra, pos) && match('o', palavra, pos) && match('i', palavra, pos) && match('d', palavra, pos))
        {
          if(id(palavra, pos) && match('(', palavra, pos))
          {
            NovoParam:
            if(lookahead == ')')
            {
              if(match(')', palavra, pos))
              {
                if(dcl(palavra, pos))
                {
                  if(sub(palavra, pos))
                  {
                    if(bco(palavra, pos) && match(';', palavra, pos))
                    {
                      if(lookahead == 'v' || tpo(palavra, pos))
                      {
                        goto subrotina; // Para fazer o {(sVOID|<tpo>)<id>([<param>])[<dcl>][<sub>]<bco>;}
                      }
                      else
                        return (1);
                    }
                    else
                      return (0);
                  }
                  else if(bco(palavra, pos) && match(';', palavra, pos))
                  {
                    if(lookahead == 'v' || tpo(palavra, pos)) // (sVOID | <tpo>)
                    {
                      goto subrotina;
                    }
                    else
                      return (1);
                  }
                  else
                    return (0);
                }
                else if(sub(palavra, pos)) //[<sub>]
                {
                  if(bco(palavra, pos) && match(';', palavra, pos)) // <bco>;
                  {
                    if(lookahead == 'v' || tpo(palavra, pos))
                    {
                      goto subrotina;
                    }
                    else
                      return (1);   
                  }
                  else
                    return (0);
                }
                else if(bco(palavra, pos))
                {
                  if(lookahead == 'v' || tpo(palavra, pos)) // (sVOID | <tpo>)
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
            else if(param(palavra, pos)) //[param]
            {
              if(lookahead == ')')
              {
                if(match(')', palavra, pos))
                {
                  if(dcl(palavra, pos))
                  {
                    if(sub(palavra, pos))
                    {
                      if(bco(palavra, pos) && match(';', palavra, pos))
                      {
                        if(lookahead == 'v' || tpo(palavra, pos))
                        {
                          goto subrotina;
                        }
                        else
                          return (1);
                      }
                      else
                        return (0);
                    }
                    else if(bco(palavra, pos) && match(';', palavra, pos))
                    {
                        if(lookahead == 'v' || tpo(palavra, pos))
                        {
                          goto subrotina;
                        }
                        else
                          return (1);
                    }
                    else
                      return (0);
                  }
                  else if(sub(palavra, pos))
                  {
                    if(bco(palavra, pos) && match(';', palavra, pos))
                    {
                      if(lookahead == 'v' || tpo(palavra, pos))
                      {
                        goto subrotina;
                      }
                      else
                        return (1);
                    }
                    else
                      return (0);
                  }
                  else if(bco(palavra, pos) && match(';', palavra, pos))
                  {
                      if(lookahead == 'v' || tpo(palavra, pos))
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
                if(match(',', palavra, pos))
                {
                  goto NovoParam;
                }
              }
              else
                return (0);
            }
            else if(lookahead == ',')
            {
              if(match(',', palavra, pos))
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
      else if(tpo(palavra, pos))
      {
        if(id(palavra, pos) && match('(', palavra, pos))
          {
            NovoParam2:
            if(lookahead == ')')
            {
              if(match(')', palavra, pos))
              {
                if(dcl(palavra, pos))
                {
                  if(sub(palavra, pos))
                  {
                    if(bco(palavra, pos) && match(';', palavra, pos))
                    {
                      if(lookahead == 'v' || tpo(palavra, pos) || id(palavra, pos))
                      {
                        goto subrotina;
                      }
                      else
                        return (1);
                    }
                  }
                }
                else if(sub(palavra, pos))
                {
                  if(bco(palavra, pos) && match(';', palavra, pos))
                  {
                    if(lookahead == 'v' || tpo(palavra, pos) || id(palavra, pos))
                    {
                      goto subrotina;
                    }
                    else
                      return (1);   
                  }
                }
              }
            }
            else if(param(palavra, pos))
            {
            if(lookahead == ')')
              {
                if(match(')', palavra, pos))
                {
                  if(dcl(palavra, pos))
                  {
                    if(sub(palavra, pos))
                    {
                      if(bco(palavra, pos) && match(';', palavra, pos))
                      {
                        if(lookahead == 'v' || tpo(palavra, pos) || id(palavra, pos))
                        {
                          goto subrotina;
                        }
                        else
                          return (1);
                      }
                    }
                  }
                  else if(sub(palavra, pos))
                  {
                    if(bco(palavra, pos) && match(';', palavra, pos))
                    {
                      if(lookahead == 'v' || tpo(palavra, pos) || id(palavra, pos))
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
                if(match(',', palavra, pos))
                {
                  goto NovoParam2;
                }
              }
            }
            else if(lookahead == ',')
            {
              if(match(',', palavra, pos))
              {
                goto NovoParam2;
              }
            }
          }
      }
      else if(id(palavra, pos) && match('(', palavra, pos))
      {
        NovoParam3:
        if(lookahead == ')')
        {
          if(match(')', palavra, pos))
          {
            if(dcl(palavra, pos))
            {
              if(sub(palavra, pos))
              {
               if(bco(palavra, pos) && match(';', palavra, pos))
               {
                if(lookahead == 'v' || tpo(palavra, pos) || id(palavra, pos))
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
            else if(sub(palavra, pos))
            {
              if(bco(palavra, pos) && match(';', palavra, pos))
              {
                if(lookahead == 'v' || tpo(palavra, pos) || id(palavra, pos))
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
        else if(param(palavra, pos))
        {
          if(lookahead == ')')
          {
            if(match(')', palavra, pos))
            {
              if(dcl(palavra, pos))
              {
                if(sub(palavra, pos))
                {
                 if(bco(palavra, pos) && match(';', palavra, pos))
                 {
                  if(lookahead == 'v' || tpo(palavra, pos) || id(palavra, pos))
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
              else if(sub(palavra, pos))
              {
                if(bco(palavra, pos) && match(';', palavra, pos))
                {
                  if(lookahead == 'v' || tpo(palavra, pos) || id(palavra, pos))
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
            if(match(',', palavra, pos))
            {
              goto NovoParam3;
            }
          }
          else
            return (0);
        }
        else if(lookahead == ',')
        {
          if(match(',', palavra, pos))
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
  
int param(char palavra[], int *pos){ //<param> → <tpo><id>{;<tpo><id>}
  if(tpo(palavra, pos) && id(palavra, pos))
  {
    if(lookahead == ';')
    {
      if(match(';', palavra, pos) && tpo(palavra, pos) && id(palavra, pos))
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
  
int cmd(char palavra[], int *pos){ //<cmd> → <wr>|<rd>|<if>|<fr>|<wh>|<rpt>|<atr>|<f>|<bco>
  if(wr(palavra, pos))
  {
    return (1);
  }
  else if(rd(palavra, pos))
  {
    return (1);
  }
  else if(retorna(palavra, pos))
  {
    return (1);
  }
  else if(sIF(palavra, pos))
  {
    return (1);
  }
  else if(fr(palavra, pos))
  {
    return (1);
  }
  else if(wh(palavra, pos))
  {
    return (1);
  }
  else if(rpt(palavra, pos))
  {
    return (1);
  }
  else if(bco(palavra, pos))
  {
    return (1);
  }
  else if(atr(palavra, pos))
  {
    return (1);
  }
  else if(f(palavra, pos))
  {
    return (1);
  }
  else
    return (0);
}

int bco(char palavra[], int *pos){ //<bco> → sBEGIN {<cmd>;} sEND
  if(lookahead == 'b')
  {
    BeginNovo:
    if(match('b', palavra, pos) && match('e', palavra, pos) && match('g', palavra, pos) && match('i', palavra, pos) 
      && match('n', palavra, pos))
    {
    continuaComandos:
      if(cmd(palavra, pos) && match(';', palavra, pos))
      {
        NovoComando:
        if(lookahead == 'e')
        {
          if(match('e', palavra, pos) && match('n', palavra, pos) && match('d', palavra, pos) && match(';', palavra,pos)) //Como visto nos exemplos temos dois tipos de end caso tenha mais de 1 o primeiro termina com ; e o segundo com .
          {
            if(lookahead == 'b') //Caso tenha um begin novamente
            {
              goto BeginNovo;
            }
            else if(cmd(palavra, pos) && match(';', palavra, pos)) //Caso tenha um comando depois do end
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
        if(match('e', palavra, pos) && match('n', palavra, pos) && match('d', palavra, pos))
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
  
int wr(char palavra[], int *pos){ //<wr> → sWRITE(<e>{,<e>})
  if(lookahead == 'w')
  {
    if(match('w', palavra, pos) && match('r', palavra, pos) && match('i', palavra, pos) && match('t', palavra, pos) && match('e', palavra, pos))
    {
      if(lookahead == '(')
      {
        if(match('(', palavra, pos) && e(palavra, pos))
        {
          if(lookahead == ',')
          {
            if(match(',', palavra, pos) && e(palavra, pos))
            {
              if(lookahead == ')')
              {
                if(match(')', palavra, pos))
                {
                  return 1;
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
          else if(lookahead == ')')
          {
            if(match(')', palavra, pos))
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

int rd(char palavra[], int *pos){ //<rd> → sREAD(<id>[[sNUM]])
  if(lookahead == 'r')
  {
    if(match('r', palavra, pos) && match('e', palavra, pos) && match('a', palavra, pos) && match('d', palavra, pos))
    {
      if(lookahead == '(')
      {
        if(match('(', palavra, pos) && id(palavra, pos))
        {
          if(lookahead == '[')
          {
            if(match('[', palavra, pos) && digito(palavra, pos) && match(']', palavra, pos))
            {
              if(lookahead == ')')
              {
                if(match(')', palavra, pos))
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
          else if(lookahead == ')')
          {
            if(match(')', palavra, pos))
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

int retorna(char palavra[], int *pos) //Função não prevista na linguagem mas necessária por conta dos exemplos fornecidos na LPD
{
  if(lookahead == 'r')
  {
    if(match('r', palavra, pos) && match('e', palavra, pos) && match('t', palavra, pos) && match('u', palavra, pos) && match('r', palavra, pos) && match('n', palavra, pos))
    {
      if(e(palavra, pos))
      {
        return (1);
      }
    }
  }
  else if(lookahead == 't')
  {
    if(match('t', palavra, pos) && match('u', palavra, pos) && match('r', palavra, pos) && match('n', palavra, pos)) //Por conta do read as vezes é marcado o re de return então para funcao funcionar temos isso de forma auxiliar
    {
      if(exp1(palavra, pos))
      {
        return (1);
      }
    }
  }
  else
    return (0);
}

int sIF(char palavra[], int *pos){ //<if> → sIF(<exp>)sTHEN<cmd>[sELSE<cmd>]
  if(lookahead == 'i')
  {
    if(match('i', palavra, pos) && match('f', palavra, pos) && match('(', palavra, pos)
      && exp1(palavra, pos) && match(')', palavra, pos))
    {
      if(lookahead == 't')
      {
        if(match('t', palavra, pos) && match('h', palavra, pos) && match('e', palavra, pos) && match('n', palavra, pos))
        {
          if(cmd(palavra, pos) && match(';', palavra, pos))
          {
            if(lookahead == 'e')
            {
              if(match('e', palavra, pos) && match('l', palavra, pos) && match('s', palavra, pos) && match('e', palavra, pos))
              {
                if(cmd(palavra, pos))
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

int fr(char palavra[], int *pos){ //<fr> → sFOR(<atr>;<exp>;<atr>)<cmd>
  if(lookahead == 'f')
  {
    if(match('f', palavra, pos) && match('o', palavra, pos) && match('r', palavra, pos))
    {
      if(lookahead == '(')
      {
        if(match('(', palavra, pos) && atr(palavra, pos) && match(';', palavra, pos) && exp1(palavra, pos) && 
          match(';', palavra, pos) && atr(palavra,pos) &&
          match(')', palavra, pos) && cmd(palavra, pos))
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
  
int wh(char palavra[], int *pos){ //<wh> → sWHILE(<exp>)<cmd>
  if(lookahead == 'w')
  {
    if(match('w', palavra, pos) && match('h', palavra, pos) && match('i', palavra, pos) && match('l', palavra, pos) && match('e', palavra, pos))
    {
      segueWhile:
     if(match('(', palavra, pos) && exp1(palavra, pos) && match(')', palavra, pos))
     {
       if(cmd(palavra, pos))
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
    if(match('h', palavra, pos) && match('i', palavra, pos) && match('l', palavra, pos) && match('e', palavra, pos))
      {
       goto segueWhile;
      }
  }
  else
    return (0);
}
  
int rpt(char palavra[], int *pos){ //<rpt> → sREPEAT{<cmd>}sUNTIL(<exp>)
  if(lookahead == 'r')
  {
    if(match('r', palavra, pos) && match('e', palavra, pos) && match('p', palavra, pos) && match('e', palavra, pos) &&
      match('a', palavra, pos) && match ('t', palavra, pos))
    {
        if(cmd(palavra, pos))
        {
          if(lookahead == 'u')
          {
            if(match('u', palavra, pos) && match('n', palavra, pos) && match('t', palavra, pos) && match('i', palavra, pos),
            match('l', palavra, pos))
            {
              if(lookahead == '(')
              {
                if(match('(', palavra, pos) && exp1(palavra, pos) && match(')', palavra, pos))
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
          if(match('u', palavra, pos) && match('n', palavra, pos) && match('t', palavra, pos) && match('i', palavra, pos)
          && match('l', palavra, pos))
          {
            if(lookahead == '(')
            {
              if(match('(', palavra, pos) && exp1(palavra, pos) && match(')', palavra, pos))
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

int atr(char palavra[], int *pos){ //<atr> → <id>[[sNUM]]sATRIB<exp>
  if(id(palavra, pos))
  {
    if(lookahead == '[')
    {
      if(match('[', palavra, pos) && digito(palavra, pos) && match(']', palavra, pos))
      {
        if(lookahead == '<')
        {
          if(match('<', palavra, pos) && match ('-', palavra, pos))
          {
            if(exp1(palavra, pos))
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
      if(match('<', palavra, pos) && match ('-', palavra, pos))
      {
        if(exp1(palavra, pos))
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
      if(match('[', palavra, pos) && digito(palavra, pos) && match(']', palavra, pos)){
        if(lookahead == '<'){
          if(match('<', palavra, pos) && match ('-', palavra, pos)){
            if(exp1(palavra, pos)){
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
      if(match('<', palavra, pos) && match ('-', palavra, pos)){
        if(exp1(palavra, pos))
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
  
int f(char palavra[], int *pos){ //<f> → <id>(<exp>{,<exp>})
  if(id(palavra, pos))
  {
    if(lookahead == '(')
    {
      if(match('(', palavra, pos) && exp1(palavra, pos))
      {
        if(lookahead == ')')
        {
          if(match(')', palavra, pos))
          {
            return (1);
          }
          else
            return (0);
        }
        else if (lookahead == ',')
        {
          if(match(',', palavra, pos) && exp1(palavra, pos) && match(')', palavra, pos))
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
      if(match('(', palavra, pos) && exp1(palavra, pos))
      {
        if(lookahead == ')')
        {
          if(match(')', palavra, pos))
          {
            return (1);
          }
          else
            return (0);
        }
        else if (lookahead == ',')
        {
          if(match(',', palavra, pos) && exp1(palavra, pos) && match(')', palavra, pos))
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

int e(char palavra[], int *pos){ //<e> → sSTRING|sNUM|sCHAR|<id>[[sNUM]]|<f>
  if(lookahead == '\'')  // sCHAR
  {
    if(match('\'', palavra, pos) && let(palavra, pos) && match('\'', palavra, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if((lookahead >= '0' && lookahead <='9') || lookahead == '.') //sNUM 
  {
    if(digito(palavra, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if(lookahead == '\"')
  {
    if(match('\"', palavra, pos) && let(palavra, pos) && match('\"', palavra, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if(id(palavra, pos))
  {
    if(lookahead == '[')
    {
      if(match('[', palavra, pos) && digito(palavra, pos) && match(']', palavra, pos))
      {
        return (1);
      }
      else
        return (0);
    }
    else if(lookahead == '(') //Caso tenha um ID mas na verdade seja um f
    {
      if(f(palavra, pos))
      {
        return (1);
      }
    }
    else
      return (1);
  }
  else if(f(palavra, pos))
  {
    return (1);
  }
  else
    return (0);
}
  
int exp1(char palavra[], int *pos){ //<exp> → <exps>{<op_cat3><exps>} 
  NovaExp:
  if(exps(palavra, pos))
  {
    if(op_cat3(palavra, pos)&& exps(palavra, pos))
    {
      if(op_cat3(palavra, pos)) //Caso tenha mais operadores
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
  
int exps(char palavra[], int *pos){ //<exps> → <tmo>{<op_cat2><tmo>}
  NovaExp:
  if(tmo(palavra, pos))
  {
    if(op_cat2(palavra, pos) && tmo(palavra, pos))
    {
      if(op_cat2(palavra, pos))
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

int tmo(char palavra[], int *pos){ //<tmo> → <ftr>{<op_cat1><ftr>}
  NovaExp:
  if(ftr(palavra, pos))
  {
    if(op_cat1(palavra, pos) && ftr(palavra, pos))
    {
      if(op_cat1(palavra, pos))
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
  
int ftr(char palavra[], int *pos){ //<ftr> → <e>|sNOT<ftr>|(<exp>)
  if(lookahead == 'n') // ftr -> not<ftr>
  { 
    if(match('n', palavra, pos) && match('o', palavra, pos) && match('t', palavra, pos)){
      if(ftr(palavra, pos)){
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
    if(match('(', palavra, pos) && exp1(palavra, pos) && match(')', palavra, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if(e(palavra, pos)) // ftr -> <e>
  {
    return(1);
  }
  else
    return (0);
}
  
int op_cat3(char palavra[], int *pos){ //<op_cat3> → sMAIOR|sMAIOR_IGUAL|sMENOR| sMENOR_IGUAL|sIGUAL|sDIFERENTE
  if(lookahead == '>')
  {
    if(match('>', palavra, pos))
    {
      if(lookahead == '=')
      {
        if(match('=', palavra, pos)) //Simbolo maior igual
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
    if(match('<', palavra, pos)) // sMenor
    {
      if(lookahead == '=')
      {
        if(match('=', palavra, pos)){ //sMenorIgual
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
    if(match('=', palavra, pos)) // sIgual
    {
      if(lookahead == '=')
      {
        if(match('=', palavra, pos))
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
    if(match('!', palavra, pos)) //sDiferente
    {
      if(lookahead == '=')
      {
        if(match('=', palavra, pos))
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
  
int op_cat2(char palavra[], int *pos){ //<op_cat2> → sSOMA|sSUBTR|sOR
  if(lookahead == '+')
  {
    if(match('+', palavra, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if(lookahead == '-')
  {
    if(match('-', palavra, pos))
    {
      return (1);
    }
    else
      return (0);
  }
  else if(lookahead == 'o')
  {
    if(match('o', palavra, pos) && match('r', palavra, pos)){
      return (1);
    }
    else
      return (0);
  }
  else
    return (0); //Simb opcional
}
  
int op_cat1(char palavra[], int *pos){ //<op_cat2> → sSOMA|sSUBTR|sOR
  if(lookahead == '*'){
    if(match('*', palavra, pos)){
      return (1);
    }
    else
      return (0);
  }
  else if(lookahead == '/'){
    if(match('/', palavra, pos)){
      return (1);
    }
    else
      return (0);
  }
  else if(lookahead == 'a'){
    if(match('a', palavra, pos) && match('n', palavra, pos) && match('d', palavra, pos)){
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
  
    FILE *arq = fopen(argv[1], "r");
    //FILE *arq = fopen("codigo5.txt", "r");
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