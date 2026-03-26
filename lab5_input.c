/* LAB 5: ARITHMETIC EXPRESSION EVALUATOR - User Input Version */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char *input; int pos;
void skip_ws(){while(isspace(input[pos]))pos++;}
int expr(); int term(); int factor();

int factor(){
  skip_ws();
  if(input[pos]=='('){pos++;int v=expr();skip_ws();if(input[pos]==')')pos++;return v;}
  int neg=0; if(input[pos]=='-'){neg=1;pos++;}
  int v=0; while(isdigit(input[pos]))v=v*10+(input[pos++]-'0');
  return neg?-v:v;
}
int term(){
  int v=factor(); skip_ws();
  while(input[pos]=='*'||input[pos]=='/'||input[pos]=='%'){
    char op=input[pos++]; int r=factor();
    if(op=='*')v*=r;
    else if(op=='/'){if(r==0){printf("  ERROR: Division by zero!\n");return 0;}v/=r;}
    else v%=r;
    skip_ws();
  }
  return v;
}
int expr(){
  int v=term(); skip_ws();
  while(input[pos]=='+'||input[pos]=='-'){
    char op=input[pos++]; int r=term();
    if(op=='+')v+=r; else v-=r;
    skip_ws();
  }
  return v;
}

int main(){
  char line[200];
  printf("========================================\n");
  printf("   LAB 5: ARITHMETIC EXPRESSION\n");
  printf("          EVALUATOR\n");
  printf("========================================\n");
  printf("Operators: +  -  *  /  %%  ( )\n");
  printf("Precedence: ()  >  *,/,%%  >  +,-\n");
  printf("Type 'exit' to quit.\n");
  printf("========================================\n\n");
  while(1){
    printf("Enter expression: ");
    fgets(line,sizeof(line),stdin);
    int len=strlen(line);
    if(len>0&&line[len-1]=='\n')line[len-1]='\0';
    if(strcmp(line,"exit")==0) break;
    input=line; pos=0;
    int result=expr();
    printf("  Expression : %s\n",line);
    printf("  Result     : %d\n\n",result);
  }
  printf("========================================\n");
  return 0;
}
