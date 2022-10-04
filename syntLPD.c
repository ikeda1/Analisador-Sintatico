/* 
Guilherme Shigueo Kumagae- 32041111
Pedro Henrique Ikeda - 32016344
 */

/* O analisador desenvolvido retornará Erro Léxico caso utilize palavras com acentuação ou outros símbolos especiais como ç
* caso estejam fora dos delimitadores de char (') ou string ("")
*
* A recuperação de erro adotada foi a de Modo Pânico, encerrando a execução do código
assim que encontrar um erro.
*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "lexLPD.h"


void parseINI();
void parseID();
void parseDCL();
void parseNUM();
void parseTPO();
void parseSUB();
void parsePARAM();
void parseBCO();
void parseCMD();
void parseWR();
void parseRD();
void parseIF();
void parseFR();
void parseWH();
void parseRPT();
void parseATR();
void parseF();
void parseSymbols();
void parseE();
void parseEXP();
void parseEXPS();
void parseTMO();
void parseFTR();
void parseOP_CAT3();
void parseOP_CAT2();
void parseOP_CAT1();
void reportarErro(TOKEN tmp);
void parseRETURN();


TOKEN lookahead;

void calcErro(TOKEN tmp) {
	float tmpN = tmp.linha;
	while (tmp.tipo != EOS) {
		tmp = scanner();
	}

	float totalN = tmp.linha;
	float result = tmpN / totalN;

	printf("\n\nO programa inserido esta %.2f %% correto (%.0f/%.0f linhas)\n", result*100, tmpN, totalN);
}

void reportarErro(TOKEN tmp) {
	char error[64] = {};
	// int i = tmp.linha+1;
	// while(tmp.linha != i) {
	// 	strcat(error, tmp.valor);
	// 	tmp = scanner();
	// 	printf("linha: %d\n", tmp.linha);
	// }
	printf("\n\n** Encontrado erro na linha:%3d | valor: [%5s] **", tmp.linha, tmp.valor);

	calcErro(tmp);
	


	printf("\n\nEncerrando o programa...");
	exit(0);
}

void check(TOKEN_TYPE tipo) {
	TOKEN tmp = lookahead;
	if (lookahead.tipo == tipo) {
		if (debugMode == TRUE)
			printf("\nLinha:%3d | %-40s | %5s", lookahead.linha, tokenToStr[lookahead.tipo], lookahead.valor);
		lookahead = scanner();
	}
	else {
		// printf("\n**erro no check**\n");
		reportarErro(tmp);
	}
}

void parseINI() {
	check(sPRG);
	parseID();
	check(PONTO_VIRG);

	if(lookahead.tipo == sVAR) {
		parseDCL();
	}

	if (lookahead.tipo == SUBROT) {
		parseSUB();
	}

	parseBCO();

	check(PONTOF);
}

void parseID() {
	check(sIDENT);
}

void parseRETURN() {
	check(RETURN);
	parseEXP();
}

void parseDCL() {
	// printf("\ninside parseDCL\n");
	check(sVAR);
	repeteParseVAR:
	parseTPO();
	parseID();
	if (lookahead.tipo == ABRE_ARR) {
		check(ABRE_ARR);
		check(INTEIRO);
		check(FECHA_ARR);
		if (lookahead.tipo == VIRGULA) {
			// check(VIRGULA);
			// printf("\nrepeteVar\n");
			goto repeteVar;
		}
	}
	repeteVar:
	if (lookahead.tipo == VIRGULA) {
		check(VIRGULA);
		parseID();

		if (lookahead.tipo == ABRE_ARR) {
			check(ABRE_ARR);
			check(INTEIRO);
			check(FECHA_ARR);

			if (lookahead.tipo == VIRGULA) {
				// printf("\nrepetevar2\n");
				goto repeteVar;
			}
		} else if (lookahead.tipo == VIRGULA) {
			// printf("\nrepetevar2\n");
			goto repeteVar;
		}
	}

	check(PONTO_VIRG);
	// printf("\nLinha:%3d | %-40s | %5s", lookahead.linha, tokenToStr[lookahead.tipo], lookahead.valor);
	if (lookahead.tipo == INT_VAR || lookahead.tipo == FLOAT_VAR || lookahead.tipo == CHAR_VAR) {
		// printf("\ngoto repeteParseVAR\n");
		goto repeteParseVAR;
	}
}

void parseTPO() {
	// printf("\ninside parseTPO\n");
	TOKEN tmp = lookahead;
	if (lookahead.tipo = INT_VAR)
		check(INT_VAR);
	else if (lookahead.tipo == FLOAT_VAR)
		check(FLOAT_VAR);
	else if (lookahead.tipo == CHAR_VAR)
		check(CHAR_VAR);

	else
		reportarErro(tmp);
}

void parseSUB() {
	// printf("\ninside parseSUB\n");
	TOKEN tmp = lookahead;
	check(SUBROT);

	if (lookahead.tipo = INT_VAR || lookahead.tipo == FLOAT_VAR || lookahead.tipo == CHAR_VAR)
		check(lookahead.tipo);

	else if (lookahead.tipo == VOID)
		check(VOID);
	else
		reportarErro(tmp);
	
	parseID();
	
	check(ABRE_PARENT);
	if (lookahead.tipo = INT_VAR || lookahead.tipo == FLOAT_VAR || lookahead.tipo == CHAR_VAR)
		parsePARAM();
	check(FECHA_PARENT);

	if(lookahead.tipo == sVAR)
		parseDCL();
	
	if (lookahead.tipo == SUBROT)
		parseSUB();

	parseBCO();
	check(PONTO_VIRG);
}

void parsePARAM() {
	// printf("\ninside parsePARAM\n");
	parseTPO();
	parseID();
	
	repetePARAM:
	if (lookahead.tipo == VIRGULA) {
		check(VIRGULA);
		parseTPO();
		parseID();
		if (lookahead.tipo == VIRGULA)
			goto repetePARAM;
	}
}

void parseBCO() {
	// printf("\ninside parseBCO\n");
	check(BEGIN);
	repeteCMD:
	if (lookahead.tipo == WRITE || lookahead.tipo == READ || lookahead.tipo == IF || lookahead.tipo == FOR || lookahead.tipo == WHILE ||
			lookahead.tipo == REPEAT || lookahead.tipo == sIDENT || lookahead.tipo == BEGIN || lookahead.tipo == RETURN)
	{
		parseCMD();
		// printf("\n***saiu do CMD**\n");
		check(PONTO_VIRG);
		if (lookahead.tipo == WRITE || lookahead.tipo == READ || lookahead.tipo == IF || lookahead.tipo == FOR || lookahead.tipo == WHILE ||
				lookahead.tipo == REPEAT || lookahead.tipo == sIDENT || lookahead.tipo == BEGIN || lookahead.tipo == RETURN) {

			goto repeteCMD;

			}
	}
	if (lookahead.tipo == END)
	{
		check(END);
	}
	else {
		TOKEN tmp = lookahead;
		reportarErro(tmp);
	}
}

void parseCMD() {
	// printf("\ninside parseCMD\n");
	switch (lookahead.tipo)
	{
	case WRITE:
		parseWR();
		break;
	
	case READ:
		parseRD();
		break;
	
	case IF:
		parseIF();
		break;

	case FOR:
		parseFR();
		break;

	case WHILE:
		parseWH();
		break;

	case REPEAT:
		parseRPT();
		break;
	
	case sIDENT:
		parseID();
		if (lookahead.tipo == ABRE_PARENT)
			parseF();
		else if (lookahead.tipo == ABRE_ARR || lookahead.tipo == OP_ATRIB)
			parseATR();
		else {
			// printf("\nERROR ON LINE %d, %s\n", tmp.linha, tmp.valor);
			TOKEN tmp = lookahead;
			reportarErro(tmp);
		}
		break;

	case BEGIN:
		parseBCO();
		break;

	case RETURN:
		parseRETURN();
		break;

	default:
		TOKEN tmp = lookahead;
		tmp = lookahead;
		reportarErro(tmp);
		break;
	
	}

	// printf("\n == END OF CMD ==\n");
}


void parseWR() {
	// printf("\ninside parseWR\n");
	check(WRITE);
	check(ABRE_PARENT);
	parseE();
	
	repeatWR:
	if (lookahead.tipo == VIRGULA) {
		check(VIRGULA);
		parseE();
		if (lookahead.tipo == VIRGULA) {
			// printf("repeatWR\n");
			goto repeatWR;
		}
	}

	check(FECHA_PARENT);
}

void parseRD() {
	// printf("\ninside parseRD\n");
	check(READ);
	check(ABRE_PARENT);
	parseID();
	if(lookahead.tipo == ABRE_ARR) {
		check(ABRE_ARR);
		check(INTEIRO);
		check(FECHA_ARR);
	}
	check(FECHA_PARENT);
}

void parseIF() {
	// printf("\ninside parseIF\n");
	check(IF);
	check(ABRE_PARENT);
	parseEXP();
	check(FECHA_PARENT);
	check(THEN);
	parseCMD();
	if (lookahead.tipo == PONTO_VIRG) {
		reportarErro(lookahead);
	}
	if (lookahead.tipo == ELSE) {
		check(ELSE);
		parseCMD();
	}
	// printf("\n***fim do if***\n\n");
}

void parseFR() {
	// printf("\ninside parseFR\n");
	check(FOR);
	check(ABRE_PARENT);
	check(sIDENT);
	parseATR();
	check(PONTO_VIRG);
	parseEXP();
	check(PONTO_VIRG);
	check(sIDENT);
	parseATR();
	check(FECHA_PARENT);
	parseCMD();
}

void parseWH() {
	// printf("\ninside parseWH\n");
	check(WHILE);
	check(ABRE_PARENT);
	parseEXP();
	check(FECHA_PARENT);
	parseCMD();
}

void parseRPT() {
	// printf("\ninside parseRPT\n");
	check(REPEAT);

	repeteRPT:
	if (lookahead.tipo == WRITE || lookahead.tipo == READ || lookahead.tipo == IF || lookahead.tipo == FOR || lookahead.tipo == WHILE ||
			lookahead.tipo == REPEAT || lookahead.tipo == sIDENT || lookahead.tipo == BEGIN )
	{
		parseCMD();
		if (lookahead.tipo == WRITE || lookahead.tipo == READ || lookahead.tipo == IF || lookahead.tipo == FOR || lookahead.tipo == WHILE ||
			lookahead.tipo == REPEAT || lookahead.tipo == sIDENT || lookahead.tipo == BEGIN )
				goto repeteRPT;
	}

	check(UNTIL);
	check(ABRE_PARENT);
	parseEXP();
	check(FECHA_PARENT);
}

void parseATR() {
	// parseID();
	// if (lookahead.tipo == ABRE_ARR) {
	// 	check(ABRE_ARR);
	// 	check(INTEIRO);
	// 	check(FECHA_ARR);
	// }
	check(OP_ATRIB);
	parseEXP();

}

void parseF() {
	// printf("\ninside parseF\n");
	// parseID();
	check(ABRE_PARENT);
	parseEXP();
	repeteParseF:
	if(lookahead.tipo == VIRGULA) {
		check(VIRGULA);
		parseEXP();
		if(lookahead.tipo == VIRGULA)
			goto repeteParseF;
	}
	
	check(FECHA_PARENT);
}

void parseE() {
	// printf("\ninside parseE\n");
	if (lookahead.tipo == STRING_CONTEUDO)
		check(STRING_CONTEUDO);
	else if (lookahead.tipo == FLOAT) 
		check(FLOAT);
	else if (lookahead.tipo == INTEIRO)
		check(INTEIRO);
	else if (lookahead.tipo == CHAR_CONTEUDO)
		check(CHAR_CONTEUDO);
	else if (lookahead.tipo == sIDENT) {
		parseID();

		// tenta fazer o parseID fora do parseF(), removendo o parseID do parseF();

		if (lookahead.tipo == ABRE_ARR) {
			check(ABRE_ARR);
			check(INTEIRO);
			check(FECHA_ARR);
		} else if (lookahead.tipo == ABRE_PARENT){
			parseF();
		}
	} else {
		TOKEN tmp = lookahead;
		reportarErro(tmp);
	}
	// printf("end of parseE\n");
}

void parseEXP() {
	// printf("\ninside parseEXP\n");
	parseEXPS();
	repeteParseEXP:
	if (lookahead.tipo == MAIOR || lookahead.tipo == MAIOR_IGUAL || lookahead.tipo == MENOR || lookahead.tipo == MENOR_IGUAL || 
			lookahead.tipo == IGUALDADE || lookahead.tipo == DIFERENCA) {
				parseOP_CAT3();
				parseEXPS();
				if (lookahead.tipo == MAIOR || lookahead.tipo == MAIOR_IGUAL || lookahead.tipo == MENOR || lookahead.tipo == MENOR_IGUAL || 
						lookahead.tipo == IGUALDADE || lookahead.tipo == DIFERENCA)
					goto repeteParseEXP;
			}
}

void parseEXPS() {
	// printf("\ninside parseEXPS\n");
	parseTMO();
	repeteParseEXPS:
	if (lookahead.tipo == OP_SOMA || lookahead.tipo == OP_SUB || lookahead.tipo == OR) {
		parseOP_CAT2();
		parseTMO();
		if (lookahead.tipo == OP_SOMA || lookahead.tipo == OP_SUB || lookahead.tipo == OR)
			goto repeteParseEXPS;
	}
}

void parseTMO() {
	// printf("\ninside parseTMO\n");
	parseFTR();
	repeteParseTMO:
	if (lookahead.tipo == OP_MULT || lookahead.tipo == OP_DIV || lookahead.tipo == AND) {
		parseOP_CAT1();
		parseFTR();
		if (lookahead.tipo == OP_MULT || lookahead.tipo == OP_DIV || lookahead.tipo == AND)
			goto repeteParseTMO;
	}
}

void parseFTR() {
	// printf("\ninside parseFTR\n");
	
	if (lookahead.tipo == STRING_CONTEUDO || lookahead.tipo == FLOAT || lookahead.tipo == INTEIRO || lookahead.tipo == CHAR_CONTEUDO || lookahead.tipo == sIDENT)
		parseE();
	
	else if (lookahead.tipo == NOT) {
		check(NOT);
		parseFTR();
	} else if (lookahead.tipo == ABRE_PARENT) {
		check(ABRE_PARENT);
		parseEXP();
		check(FECHA_PARENT);
	}
}

void parseOP_CAT3() {
	// printf("\ninside parseOP_CAT3\n");
	if (lookahead.tipo == MAIOR)
		check(MAIOR);
	else if (lookahead.tipo == MAIOR_IGUAL) 
		check(MAIOR_IGUAL);
	else if (lookahead.tipo == MENOR) 
		check(MENOR);
	else if (lookahead.tipo == MENOR_IGUAL)
		check(MENOR_IGUAL);
	else if (lookahead.tipo == IGUALDADE)
		check(IGUALDADE);
	else if (lookahead.tipo == DIFERENCA)
		check(DIFERENCA);
}

void parseOP_CAT2() {
	// printf("\ninside parseOP_CAT2\n");
	if (lookahead.tipo == OP_SOMA)
		check(OP_SOMA);
	else if (lookahead.tipo == OP_SUB) 
		check(OP_SUB);
	else if (lookahead.tipo == OR) 
		check(OR);
}

void parseOP_CAT1() {
	// printf("\ninside parseOP_CAT1\n");
	if (lookahead.tipo == OP_MULT)
		check(OP_MULT);
	else if (lookahead.tipo == OP_DIV) 
		check(OP_DIV);
	else if (lookahead.tipo == AND) 
		check(AND);
}


int main(int argc, char* argv[]) {

	if (argc < 2) {
		printf("Analisador sintatico para a LPD\n");
		printf("Uso:\n");
		printf("  syntLPD.exe <arquivo_fonte> [/v]\n");
		printf("\n");
		printf("<arquivo_fonte> pode ser qualquer arquivo texto cujo conteudo sera\n");
		printf("analisado quanto a linguagem LPD.\n");
		return 0;
	}
	else {
		if ((argc == 3) && (strcmp(argv[2], "/v") == 0))
			debugMode = TRUE;

		printf("Iniciando a analise sintatica do arquivo: %s\n\n", argv[1]);
	}

	// Abertura do arquivo e carregamento do seu conteúdo para o buffer
	FILE* arq = fopen(argv[1], "r");
	if (arq == NULL) {
		fprintf(stderr, "Erro ao tentar abrir o aquivo \"%s\"", argv[1]);
		return -1;
	}
	fseek(arq, 0, SEEK_END);
	int tam_arq = ftell(arq);
	fseek(arq, 0, SEEK_SET);

	buffer = (char*)calloc(tam_arq, sizeof(char));
	if (buffer == NULL)
		exit(-1);
	else
		fread(buffer, sizeof(char), tam_arq, arq);

	fclose(arq);


	lookahead = scanner();
	parseINI();
	printf("\n\n=== O programa inserido esta correto! ===\n\n");
	return 0;

}