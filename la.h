#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INTIAL_ARRAY_SIZE 1
#define STRING_BUFFER_SIZE 1000

int curr_size = 0;
int curr_max_size = INTIAL_ARRAY_SIZE;
int line_nums = 1;
char* buffer_pointer;
int ScannerTerminated = 0;

enum TokenKind { KEYWORDS, IDENTIFIERS, DELIMITORS, OPERATORS, LITERALS, STRINGS};

char* TokenStrings[] = { "KEYWORDS", "IDENTIFIERS", "DELIMITORS", "OPERATORS", "LITERALS", "STRINGS"};
char* LexicalErrors[] = {"String cannot contain multiple lines.", "Illegal character present.", "Bad excape sequence character present."};

char string_buffer[STRING_BUFFER_SIZE];

typedef struct{
    enum TokenKind TokenType;
    char* lexeme;
    int count;
    // Other attributes not needed as of now :)
} Token;

Token* Tokens;

void init_array(){
    Tokens = (Token*)malloc(INTIAL_ARRAY_SIZE * sizeof(Token));    
}

void pushTokenUtil(Token tk){
    char* lex = tk.lexeme;
    for(int i=0;i<curr_size;i++){
        if(!strcmp(Tokens[i].lexeme, lex)){
            Tokens[i].count+=1;
            return ;
        }
    }
    if(curr_size == curr_max_size){
        Tokens = (Token*)realloc(Tokens, 2*curr_max_size*sizeof(Token));
        curr_max_size*=2;
    }
    Tokens[curr_size].count = tk.count;
    Tokens[curr_size].lexeme = tk.lexeme;
    Tokens[curr_size].TokenType = tk.TokenType;
    curr_size++;
    return ;
}

void pushToken(enum TokenKind TokenType, char* lex){
    Token* tk = (Token*)malloc(sizeof(Token));  
    tk->count = 1;
    tk->lexeme = lex;
    tk->TokenType = TokenType;
    pushTokenUtil(*tk); 
    return ;
}

void showError(char* temp, int errorCode){
    ScannerTerminated = 1;
    printf("Error at line num: %d\nError: %s\n", line_nums, LexicalErrors[errorCode]);
    if(strcmp(temp, "")){
        printf("Illegal Character present is: %c", temp);
    }
    return ;
}

void writeToCSV(){
    printf("Number of distinct tokens: %d\n", curr_size);

    FILE* fpt;
    fpt = fopen("output.csv", "w+");
    fprintf(fpt, "Lexeme, Token, Count\n");
    for(int i=0;i<curr_size;i++){
        fprintf(fpt, "\"%s\", %s, %d\n", Tokens[i].lexeme, TokenStrings[Tokens[i].TokenType], Tokens[i].count);
    }
    fclose(fpt);
    return ;
}