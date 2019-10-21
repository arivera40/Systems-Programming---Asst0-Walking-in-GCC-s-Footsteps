#include<stdio.h>
#include<stdlib.h>

int arithCount = 0;
int logicCount = 0;
int totalCount = 0;

char *tokenizer(char *expr, int *index, char delimiter);
void identifier(char *expr, int *rule, char delimiter, int index);
int tokenCheck(char *token);
void exprCheck(char *expr, int index);
int NOT_ANDCheck(char *token);
int ORCheck(char *token);
int arithOpCheck(char *token);
int trueCheck(char *token);
int falseCheck(char *token);

int NOT_ANDCheck(char *token){	//checks AND, NOT
	if(token[0] == 'N' && token[1] == 'O' && token[2] == 'T'){
		return 1;	//true NOT
	}else if(token[0] == 'A' && token[1] == 'N' && token[2] == 'D'){
		return 2;	//true AND
	}
	return 0;		//false
}

int ORCheck(char *token){	//checks OR
	if(token[0] && token[1]){
		return 1;	//true
	}
	return 0;		//false
}

int arithOpCheck(char *token){	//checks 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, +, -, *, /
	if(token[0] == '0' || token[0] == '1' || token[0] == '2' || token[0] == '3' || token[0] == '4' || token[0] == '5' || token[0] == '6' || token[0] == '7' || token[0] == '8' || token[0] == '9'){
		return 1;	//true for arith Operands
	}else if(token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/'){
		return 2;	//true for arith Operators
	}
	return 0;
}

int trueCheck(char *token){	//checks true
	if(token[0] == 't' && token[1] == 'r' && token[2] == 'u' && token[3] == 'e'){
		return 1;
	}
	return 0;
}

int falseCheck(char *token){	//checks false
	if(token[0] == 'f' && token[1] == 'a' && token[2] == 'l' && token[3] == 's' && token[4] == 'e'){
		return 1;
	}
	return 0;
}

char *tokenizer(char *expr, int *index, char delimiter){
	int i;
	char *token = (char*)malloc(sizeof(char) * 50);
	for(i=0; expr[*index] != delimiter && expr[*index] != '\0'; i++){
		token[i] = expr[*index];
		*index = *index + 1;
	}
	if(expr[*index] == '\0'){
		*index = *index - 1;	//Should prevent segfault when returns to original loop
	}			//While still allowing to properly start after previous whitespace
	token[i] = '\0';

	return token;	
}
void identifier(char *expr, int *rule, char delimiter, int index){
	int i;
	int j;
	char *token = (char*)malloc(sizeof(char) * 50);
	for(i=0; expr[i] != '\0'; i++){
		token = tokenizer(expr, &i, delimiter);
		rule[index] = tokenCheck(token);
		if(rule[index] == 1){
			arithCount++;
			return;
		}
		if(rule[index] == 2 || rule[index] == 3){
			logicCount++;
			return;
		}
	}
	totalCount++;
	free(token);
	return;
}

int tokenCheck(char *token){
	int i;	
	int increment = 0;

	if(token[1] == '\0' && (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/')){
			return 1;
	}
	for(i=0; token[i] != '\0'; i++){
		increment++;
	}
	if(increment == 2){
		if(token[0] == 'O' && token[1] == 'R'){
			return 2;
		}
	}else if(increment == 3){
		if(token[0] == 'A' && token[1] == 'N' && token[2] == 'D'){
			return 2;
		}else if(token[0] == 'N' && token[1] == 'O' && token[2] == 'T'){
			return 3;
		}
	}
	return 0;
}

void exprCheck(char *expr, int index){
	int i;
	int j;
	int increment = 0;
	int check;
	int exprEnd = 0;
	int NOTinit = 0;
	char *token = (char*)malloc(sizeof(char) * 50);
	char *prevToken = (char*)malloc(sizeof(char) * 50);
	for(i=0; expr[i] != '\0'; i++){
		exprEnd++;
		if(exprEnd == 4 && NOTinit == 0 || exprEnd == 3 && NOTinit == 1){
			printf("Error: Parse error in expression %d: expression was not ended\n\"%s\"\n", index, expr);
		}
		if(i != 0){
			prevToken = token;
		}
		token = tokenizer(expr, &i, ' ');
		check = 0;
		for(j=0; j < token[j]; j++){
			check++;
		}
		if(increment == 0 ){
			if(check == 5){
				if(falseCheck(token) == 1){
					increment = 1;
					continue;
				}
			}else if(check == 4){
				if(trueCheck(token) == 1){
					increment = 1;
					continue;
				}
			}else if(check == 3){
				if(NOT_ANDCheck(token) == 1){
					increment = 3;
					NOTinit = 1;
					continue;
				}else if(NOT_ANDCheck(token) == 2){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s\"\n", index, token);
					increment = 4;
					continue;
				}
			}else if(check == 2){
				if(ORCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s\"\n", index, token);
					increment = 4;
					continue;
				}
			}else if(check == 1){
				if(arithOpCheck(token) == 1){
					increment = 2;
					continue;
				}else if(arithOpCheck(token) == 2){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s\"\n", index, token);
					increment = 5;
					continue;
				}
			}
			printf("Error: Parse error in expression %d: unknown identifier\n\"%s\"\n", index, token);
			increment = 6;
		}else if(increment == 1){	//detected true, false
			if(check == 5){
				if(falseCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operand\n\"%s %s\"\n", index, prevToken, token);
					increment = 1;
					continue;
				}
			}else if(check == 4){
				if(trueCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operand\n\"%s %s\"\n", index, prevToken, token);
					increment = 1;
					continue;
				}
			}else if(check == 3){
				if(NOT_ANDCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 3;
					continue;
				}else if(NOT_ANDCheck(token) == 2){
					increment = 4;
					continue;
				}
			}else if(check == 2){
				if(ORCheck(token) == 1){
					increment = 4;
					continue;
				}
			}else if(check == 1){
				if(arithOpCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operand\n\"%s %s\"\n", index, prevToken, token);
					increment = 2;
					continue;
				}else if(arithOpCheck(token) == 2){
					printf("Error: Parse error in expression %d: operand type mismatch\n\"%s %s\"\n", index, prevToken, token);
					increment = 5;
					continue;
				}
			}
			printf("Error: Parse error in expression %d: unknown operator\n\"%s\"\n", index, token);
			increment = 6;
		}else if(increment == 2){	//detected 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
			if(check == 5){
				if(falseCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operand\n\"%s %s\"\n", index, prevToken, token);
					increment = 1;
					continue;
				}
			}else if(check == 4){
				if(trueCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operand\n\"%s %s\"\n", index, prevToken, token);
					increment = 1;
					continue;
				}
			}else if(check == 3){
				if(NOT_ANDCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken, token);
					increment = 3;
					continue;
				}else if(NOT_ANDCheck(token) == 2){
					printf("Error: Parse error in expression %d: operand type mismatch\n\"%s %s\"\n", index, prevToken, token);
					increment = 4;
					continue;
				}
			}else if(check == 2){
				if(ORCheck(token) == 1){
					printf("Error: Parse error in expression %d: operand type mismatch\n\"%s %s\"\n", index, prevToken, token);
					increment = 4;
					continue;
				}
			}else if(check == 1){
				if(arithOpCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operand\n\"%s %s\"\n", index, prevToken, token);
					increment = 2;
					continue;
				}else if(arithOpCheck(token) == 2){
					increment = 5;
					continue;
				}
			}
			printf("Error: Parse error in expression %d: unknown operator\n\"%s\"\n", index, token);
			increment = 6;
		}else if(increment == 3){	//detected NOT
			if(check == 5){
				if(falseCheck(token) == 1){
					increment = 1;
					continue;
				}
			}else if(check == 4){
				if(trueCheck(token) == 1){
					increment = 1;
					continue;
				}
			}else if(check == 3){
				if(NOT_ANDCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken, token);
					increment = 3;
					continue;
				}else if(NOT_ANDCheck(token) == 2){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken, token);
					increment = 4;
					continue;
				}
			}else if(check == 2){
				if(ORCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken, token);
					increment = 4;
					continue;
				}
			}else if(check == 1){
				if(arithOpCheck(token) == 1){
					printf("Error: Parse error in expression %d: operator type mismatch\n\"%s %s\"\n", index, prevToken,token);
					increment = 2;
					continue;
				}else if(arithOpCheck(token) == 2){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 5;
					continue;
				}
			}
			printf("Error: Parse error in expression %d: unknown operand\n\"%s\"\n", index, token);
			increment = 6;
		}else if(increment == 4){	//detected AND, OR
			if(check == 5){
				if(falseCheck(token) == 1){
					increment = 1;
					continue;
				}
			}else if(check == 4){
				if(trueCheck(token) == 1){
					increment = 1;
					continue;
				}
			}else if(check == 3){
				if(NOT_ANDCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 3;
					continue;
				}else if(NOT_ANDCheck(token) == 2){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 4;
					continue;
				}
			}else if(check == 2){
				if(ORCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 4;
					continue;
				}
			}else if(check == 1){
				if(arithOpCheck(token) == 1){
					printf("Error: Parse error in expression %d: operator type mismatch\n\"%s %s\"\n", index, prevToken,token);
					increment = 2;
					continue;
				}else if(arithOpCheck(token) == 2){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 5;
					continue;
				}
			}
			printf("Error: Parse error in expression %d: unknown operand\n\"%s\"\n", index, token);
			increment = 6;
		}else if(increment == 5){	//detected +, -, *, /
			if(check == 5){
				if(falseCheck(token) == 1){
					printf("Error: Parse error in expression %d: operator type mismatch\n\"%s %s\"\n", index, prevToken,token);
					increment = 1;
					continue;
				}
			}else if(check == 4){
				if(trueCheck(token) == 1){
					printf("Error: Parse error in expression %d: operator type mismatch\n\"%s %s\"\n", index, prevToken,token);
					increment = 1;
					continue;
				}
			}else if(check == 3){
				if(NOT_ANDCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 3;
					continue;
				}else if(NOT_ANDCheck(token) == 2){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 4;
					continue;
				}
			}else if(check == 2){
				if(ORCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 4;
					continue;
				}
			}else if(check == 1){
				if(arithOpCheck(token) == 1){
					increment = 2;
					continue;
				}else if(arithOpCheck(token) == 2){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 5;
					continue;
				}
			}
			printf("Error: Parse error in expression %d: unknown operand\n\"%s\"\n", index, token);
			increment = 6;
		}else{		//detected unknown
			if(check == 5){
				if(falseCheck(token) == 1){
					increment = 1;
					continue;
				}
			}else if(check == 4){
				if(trueCheck(token) == 1){
					increment = 1;
					continue;
				}
			}else if(check == 3){
				if(NOT_ANDCheck(token) == 1){
					increment = 3;
					continue;
				}else if(NOT_ANDCheck(token) == 2){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 4;
					continue;
				}
			}else if(check == 2){
				if(ORCheck(token) == 1){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 4;
					continue;
				}
			}else if(check == 1){
				if(arithOpCheck(token) == 1){
					increment = 2;
					continue;
				}else if(arithOpCheck(token) == 2){
					printf("Error: Parse error in expression %d: unexpected operator\n\"%s %s\"\n", index, prevToken,token);
					increment = 5;
					continue;
				}
			}
			exprEnd = 1;
			printf("Error: Parse error in expression %d: unknown identifier\n\"%s\"\n", index, token);
			increment = 6;
		}
	}
	if(exprEnd == 2 && NOTinit == 0){
		printf("Error: Parse error in expression %d: missing operand\n\"%s\"\n", index, expr);
	}else if(exprEnd == 1 && check == 3){
		if(NOT_ANDCheck(token) == 1){
			printf("Error: Parse error in expression %d: missing operand\n\"%s\"\n", index, expr);
		}else{
			printf("Error: Parse error in expression %d: incomplete expression\n\"%s\"\n", index, expr);
		}
	}else if(exprEnd == 1 || exprEnd == 0){
		printf("Error: Parse error in expression %d: incomplete expression\n\"%s\"\n", index, expr);
	}
	free(prevToken);
	free(token);
	return;
}

int main(int argc, char** argv){
	int i;
	int j;
	int k;

	if(argv[1][0] == '\0'){
		printf("Found 1 expressions: 0 logical and arithmetic.\nError: Scan error in expression 0: incomplete expression\n \"\"\n");
		return 0;
	}
	if(argc > 2){
		printf("Error: too many arguments\n");
		return 0;
	}
	if(argc < 2){
		printf("Error: No argument\n");
	}
	
	int incompleteError = 0;
	int exprCount = 1;
	int size = 0;
	while(argv[1][size] != '\0'){
		size++;
	}
	size++;

	for(j=0; argv[1][j] != '\0'; j++){
		if(argv[1][j] == ';' && j != size - 2){
			exprCount++;
		}
	}

	char **expr = (char**)malloc(sizeof(char*) * (exprCount));
	int index = 0;
	int exprLength;

	int rule[exprCount];
	for(i=0; i < exprCount; i++){
		rule[i] = 0;
	}

	for(j=0; j < size; j=k+1){
		exprLength = 1;		//Starts at 1 to include '\0' 
		if(argv[1][j] == ' '){
			j++;
		}else if(j != 0){
			printf("Error: Scan error in expression %d: missing white space after previous semicolon\n", index);
		}
		//First loop: Counts expression length in order to malloc index in 2D array
		for(k=j; argv[1][k] != ';' && argv[1][k] != '\0'; k++){
			exprLength++;
		}
		expr[index] = (char*)malloc(sizeof(char) * exprLength);
		//Second loop: traverses and transfers expression to 2D array
		i=0;
		for(k=j; argv[1][k] != ';' && argv[1][k] != '\0'; k++){
			expr[index][i] = argv[1][k];
			i++;
		}
		expr[index][i+1] = '\0';
		
		identifier(expr[index], rule, ' ', index);
		index++;
	}
	printf("Found %d expressions: %d logical and %d arithmetic.\n", exprCount, logicCount, arithCount);

	index = 0;
	for(i=0; i < exprCount; i++){
		exprCheck(expr[i], i);
	}
	for(i=0; i < exprCount; i++){
		free(expr[i]);
	}
	free(expr);
	return 0;

}
