/* 
Guilherme Shigueo Kumagae- 32041111
Pedro Henrique Ikeda - 32016344
 */

/* O analisador desenvolvido retornará Erro Léxico caso utilize palavras com acentuação ou outros símbolos especiais como ç
* caso estejam fora dos delimitadores de char (') ou string ("")
*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

 /*
  * Tipos de tokens (classificação)
  */
typedef enum {
	ERRO,			// símbolo inválido
	sIDENT,		// identificador
	OP_ATRIB,		// <-
	OP_SOMA,		// +
	OP_SUB,			// -
	OP_MULT,		// *
	OP_DIV,			// /
	IGUALDADE,		// ==
	DIFERENCA,		// !=
	MAIOR,			// >
	MAIOR_IGUAL,	// >=
	MENOR,			// <
	MENOR_IGUAL,	// <=
	STRING,			// " "
	CHAR,			// ' '
	CHAR_VAR,		// declaracao de variavel tipo char
	INTEIRO,		// números inteiros
	INT_VAR,		// declaracao de variavel tipo int
	FLOAT,			// números reais
	FLOAT_VAR,		// declaracao de variavel tipo float
	ABRE_PARENT,	// (
	FECHA_PARENT,	// )
	ABRE_COM,		// {
	FECHA_COM,		// }
	ABRE_ARR,		// [
	FECHA_ARR,		// ]
	BEGIN,			// comeca trecho de codigo
	END,			// encerra trecho de codigo
	WRITE,			// escreve expressoes na tela
	READ,			// le o input do teclado
	IF,				// condicional if
	THEN,			// complemento do if
	ELSE,			// condicional else
	FOR,			// laco de repeticao for
	WHILE,			// laco de repeticao while
	REPEAT,			// laco de repeticao repeat
	UNTIL,			// condicao de parada do repeat
	AND,			// conjuncao
	OR,				// disjuncao
	NOT,			// negacao
	VOID,			// tipo void, sem retorno
	sPRG,			// nome do programa
	RETURN,			// retorna uma expressao
	SUBROT,			// sub-rotina
	sVAR,		// var
	PONTO_VIRG,		// ;
	DOIS_PONTOS,  // :
	PONTOF,			// .
	VIRGULA,		// ,
	CHAR_CONTEUDO,	// 'CONTEUDO'
	STRING_CONTEUDO,// "CONTEUDO"
	EOS				// fim de sentença/arquivo
} TOKEN_TYPE;

char* tokenToStr[] = {
	"Erro lexico",
	"Identificador",
	"Operador de atribuicao",
	"Operador de adicaoo",
	"Operador de subtracao",
	"Operador de multiplicacao",
	"Operador de divisao",
	"Igualdade",
	"Diferenca",
	"Maior",
	"Maior igual",
	"Menor",
	"Menor igual",
	"Delimitador de string",
	"Delimitador de char",
	"Declaracao de variavel char",
	"Numero inteiro",
	"Declaracao de variavel int",
	"Numero real",
	"Declaracao de variavel char",
	"Abre parenteses",
	"Fecha parenteses",
	"Abre comentario",
	"Fecha comentario",
	"Abre colchete",
	"Fecha colchete",
	"Inicio de um trecho de codigo",
	"Fim de um trecho de codigo",
	"Escreve expressoes na tela",
	"Le o input do teclado",
	"Condicional if",
	"Complemento do if",
	"Condicional else",
	"Laco de repeticao for",
	"Laco de repetical while",
	"Laco de repetical repeat",
	"Condicao de parada do repeat",
	"Conjuncao",
	"Disjuncao",
	"Negacao",
	"Declaracao de sub-rotina sem retorno",
	"Declaracao do nome do programa",
	"Retorna uma expressao",
	"Declaracao de uma sub-rotina",
	"Declaracao de uma variavel",
	"Ponto e virgula",
	"Dois pontos",
	"Ponto final",
	"Virgula",
	"Char",
	"String",
	"Fim de conteudo",
};

/*
 * Registro do token
 */
typedef struct {
	TOKEN_TYPE tipo;
	int linha;
	char valor[64];
} TOKEN;

/*
 * Variáveis globais
 */
char* buffer;	// Buffer de memória com o conteúdo lido do arquivo
int linha = 1;	// Contador de linhas
short debugMode = FALSE;

/*
 * Protótipos de funções
 */
TOKEN scanner();

/*
 * Função principal (programa)
 * 
 * Implementa um analisador léxico simples para processamento de expressões
 * aritméticas parentizadas que envolvam variáveis e elementos numéricos.
 */
// int main(int argc, char* argv[]) {

// 	if (argc < 2) {
// 		printf("Analisador lexico para a LPD\n");
// 		printf("Uso:\n");
// 		printf("  lexLPD.exe <arquivo_fonte> [/v]\n");
// 		printf("\n");
// 		printf("<arquivo_fonte> pode ser qualquer arquivo texto cujo conteudo sera\n");
// 		printf("analisado quanto a linguagem LPD.\n");
// 		return 0;
// 	}
// 	else {
// 		if ((argc == 3) && (strcmp(argv[2], "/v") == 0))
// 			debugMode = TRUE;

// 		printf("Iniciando a analise lexica do arquivo: %s\n\n", argv[1]);
// 	}

// 	// Abertura do arquivo e carregamento do seu conteúdo para o buffer
// 	FILE* arq = fopen(argv[1], "r");
// 	if (arq == NULL) {
// 		fprintf(stderr, "Erro ao tentar abrir o aquivo \"%s\"", argv[1]);
// 		return -1;
// 	}
// 	fseek(arq, 0, SEEK_END);
// 	int tam_arq = ftell(arq);
// 	fseek(arq, 0, SEEK_SET);

// 	buffer = (char*)calloc(tam_arq, sizeof(char));
// 	if (buffer == NULL)
// 		exit(-1);
// 	else
// 		fread(buffer, sizeof(char), tam_arq, arq);

// 	fclose(arq);

// 	TOKEN tk;
// 	do {
// 		tk = scanner();
		
// 		if (tk.tipo == ABRE_COM) {
// 			printf("\nLinha:%3d | %-40s | %5s", tk.linha, tokenToStr[tk.tipo], tk.valor);
// 			while (1) {
// 				tk = scanner();
// 				// printf("\nIGNORANDO:%3d | %-40s | %5s", tk.linha, tokenToStr[tk.tipo], tk.valor);
// 				if (tk.tipo == FECHA_COM) {
// 					break;
// 				}
// 			}
// 		}

// 		printf("\nLinha:%3d | %-40s | %5s", tk.linha, tokenToStr[tk.tipo], tk.valor);
// 	} while ((tk.tipo != EOS) && (tk.tipo != ERRO));

// 	return 0;

// }

/*
 * Analisador Léxico de expressões envolvendo variáveis, números e alguns operadores.
 */
TOKEN scanner() {

	TOKEN tk;

	restart:
	tk.tipo = ERRO;
	strcpy(tk.valor, "");
	tk.linha = linha;
	// Espaços, tabulações e outros elementos decorativos (comentários)
	while ((*buffer == ' ') || (*buffer == '\t') || (*buffer == '\r') || (*buffer == '\n')) {
		if (*buffer == '\n')
			linha++;
		buffer++;
	}
	// Operadores: soma, incremento e somaAtrib
	if (*buffer == '+') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		buffer++;
		tk.tipo = OP_SOMA;
	}
	// Operador de subtração
	else if (*buffer == '-') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		buffer++;
		tk.tipo = OP_SUB;
	}
	// Operador de multiplicação
	else if (*buffer == '*') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		buffer++;
		tk.tipo = OP_MULT;
	}
	// Operador de multiplicação
	else if (*buffer == '/') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		buffer++;
		tk.tipo = OP_DIV;
	}
	// Igualdade
	else if (*buffer == '=') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		buffer++;
		if (*buffer == '=') {
			if (debugMode) strncat(tk.valor, buffer, 1);
			buffer++;
			tk.tipo = IGUALDADE;
		}
	}
	// Diferente
	else if (*buffer == '!') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		buffer++;
		if (*buffer == '=') {
			if (debugMode) strncat(tk.valor, buffer, 1);
			buffer++;
			tk.tipo = DIFERENCA;
		}
	}
	// Maior
	else if (*buffer == '>') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		buffer++;
		if (*buffer == '=') {
			if (debugMode) strncat(tk.valor, buffer, 1);
			buffer++;
			tk.tipo = MAIOR_IGUAL;
		}
		else tk.tipo = MAIOR;
	}
	// Menor
	else if (*buffer == '<') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		buffer++;
		if (*buffer == '=') {
			if (debugMode) strncat(tk.valor, buffer, 1);
			buffer++;
			tk.tipo = MENOR_IGUAL;
		}
		else if (*buffer == '-') {
			if (debugMode) strncat(tk.valor, buffer, 1);
			buffer++;
			tk.tipo = OP_ATRIB;
		}
		else tk.tipo = MENOR;
	}
	// Parenteses
	else if (*buffer == '(') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		tk.tipo = ABRE_PARENT;
		buffer++;
	}
	else if (*buffer == ')') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		tk.tipo = FECHA_PARENT;
		buffer++;
	}
	// Comentarios
	else if (*buffer == '{') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		// tk.tipo = ABRE_COM;
		buffer++;
		while (*buffer != '}') {
			if (debugMode) strncat(tk.valor, buffer, 1);
			if (*buffer == '\n') 
				linha++;
			// printf("No while\n");
			buffer++;
		}

		if (*buffer == '}') {
			if (debugMode) strncat(tk.valor, buffer, 1);
			buffer++;
			goto restart;
			// tk.tipo = FECHA_COM;
		}
	}
	// else if (*buffer == '}') {
	// 	if (debugMode) strncat(tk.valor, buffer, 1);
	// 	tk.tipo = FECHA_COM;
	// 	buffer++;
	// }
	// Colchetes
	else if (*buffer == '[') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		tk.tipo = ABRE_ARR;
		buffer++;
	}
	else if (*buffer == ']') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		tk.tipo = FECHA_ARR;
		buffer++;
	}
	// Ponto virgula
	else if (*buffer == ';') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		tk.tipo = PONTO_VIRG;
		buffer++;
	}
	// Ponto virgula
	else if (*buffer == ':') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		tk.tipo = DOIS_PONTOS;
		buffer++;
	}
	// Ponto Final
	else if (*buffer == '.') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		tk.tipo = PONTOF;
		buffer++;
	}
	// Virgula
	else if (*buffer == ',') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		tk.tipo = VIRGULA;
		buffer++;
	}

	// Conteudo é char
	else if (*buffer == '\'') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		// tk.tipo = CHAR;
		buffer++;
		while(*buffer != '\'') {
			if (debugMode) strncat(tk.valor, buffer, 1);
			buffer++;
			if (*buffer == '\n') {
				tk.tipo = ERRO;
				break;
			}
		}

		if (*buffer == '\'') {
			if (debugMode) strncat(tk.valor, buffer, 1);
			buffer++;
			tk.tipo = CHAR_CONTEUDO;
		}
		
		else tk.tipo = ERRO;
	}

	// Delimitador char
	// else if (*buffer == '\'') {
	// 	if (debugMode) strncat(tk.valor, buffer, 1);
	// 	tk.tipo = CHAR;
	// 	buffer++;
	// }
	// Delimitador string
	// else if (*buffer == '\"') {
	// 	if (debugMode) strncat(tk.valor, buffer, 1);
	// 	tk.tipo = STRING;
	// 	buffer++;
	// }

	// Conteudo é string
	else if (*buffer == '\"') {
		if (debugMode) strncat(tk.valor, buffer, 1);
		buffer++;
		while(*buffer != '\"') {
			if (*buffer == '\n') {
				tk.tipo = ERRO;
				break;
			}
			if (debugMode) strncat(tk.valor, buffer, 1);
			buffer++;
		}

		if (*buffer == '\"') {
			if (debugMode) strncat(tk.valor, buffer, 1);
			buffer++;
			tk.tipo = STRING_CONTEUDO;
		}
		
		else tk.tipo = ERRO;
	}

	// Números
	else if (isdigit(*buffer)) {
		while ((*buffer >= '0') && (*buffer <= '9')) {
			strncat(tk.valor, buffer, 1);
			buffer++;
		}
		tk.tipo = INTEIRO;
		if (*buffer == '.') {
			strncat(tk.valor, buffer, 1);
			buffer++;
			if ((*buffer >= '0') && (*buffer <= '9'))
				while ((*buffer >= '0') && (*buffer <= '9')) {
					strncat(tk.valor, buffer, 1);
					buffer++;
				}
			else
				strcat(tk.valor, "0");
			tk.tipo = FLOAT;
		}
	}
	// Identificadores
	else if (isalpha(*buffer)) {
		while (isalpha(*buffer) || isdigit(*buffer) || (*buffer == '_')) {
			strncat(tk.valor, buffer, 1);
			buffer++;
		}

		// Palavras reservadas
		if (strcmp(tk.valor, "and")== 0) tk.tipo = AND;
		else if (strcmp(tk.valor,"begin" )== 0) tk.tipo = BEGIN;
		else if (strcmp(tk.valor,"char" )== 0) tk.tipo = CHAR;
		else if (strcmp(tk.valor,"else" )== 0) tk.tipo = ELSE;
		else if (strcmp(tk.valor,"end" )== 0) tk.tipo = END;
		else if (strcmp(tk.valor,"float" )== 0) tk.tipo = FLOAT_VAR;
		else if (strcmp(tk.valor,"for" )== 0) tk.tipo = FOR;
		else if (strcmp(tk.valor,"if" )== 0) tk.tipo = IF;
		else if (strcmp(tk.valor,"int" )== 0) tk.tipo = INT_VAR;
		else if (strcmp(tk.valor,"not" )== 0) tk.tipo = NOT;
		else if (strcmp(tk.valor,"or" )== 0) tk.tipo = OR;
		else if (strcmp(tk.valor,"prg" )== 0) tk.tipo = sPRG;
		else if (strcmp(tk.valor,"read" )== 0) tk.tipo = READ;
		else if (strcmp(tk.valor,"repeat" )== 0) tk.tipo = REPEAT;
		else if (strcmp(tk.valor,"return" )== 0) tk.tipo = RETURN;
		else if (strcmp(tk.valor,"subrot" )== 0) tk.tipo = SUBROT;
		else if (strcmp(tk.valor,"then" )== 0) tk.tipo = THEN;
		else if (strcmp(tk.valor,"until" )== 0) tk.tipo = UNTIL;
		else if (strcmp(tk.valor,"var" )== 0) tk.tipo = sVAR;
		else if (strcmp(tk.valor,"void" )== 0) tk.tipo = VOID;
		else if (strcmp(tk.valor,"while" )== 0) tk.tipo = WHILE ;
		else if (strcmp(tk.valor,"write" )== 0) tk.tipo = WRITE;
		else tk.tipo = sIDENT;
	}
	else if ((*buffer == EOF) || (*buffer == '\x0')) {
		if (debugMode) strncat(tk.valor, "EOF", 1);
		tk.tipo = EOS;
	}
	else {
		strncat(tk.valor, buffer, 1);
		tk.tipo = ERRO;
	}

	tk.linha = linha;
	return tk;
}