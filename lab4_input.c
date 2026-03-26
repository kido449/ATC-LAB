/* LAB 4: REGEX VALIDATOR - User Input Version */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_integer(char *s){
  int i=0; if(s[i]=='+'||s[i]=='-')i++;
  if(!s[i])return 0;
  for(;s[i];i++)if(!isdigit(s[i]))return 0;
  return 1;
}
int is_float(char *s){
  int i=0,dot=0; if(s[i]=='+'||s[i]=='-')i++;
  if(!s[i])return 0;
  for(;s[i];i++){if(s[i]=='.'){if(dot)return 0;dot=1;}else if(!isdigit(s[i]))return 0;}
  return dot;
}
int is_identifier(char *s){
  if(!s[0]||(!(isalpha(s[0])||s[0]=='_')))return 0;
  for(int i=1;s[i];i++)if(!isalnum(s[i])&&s[i]!='_')return 0;
  return 1;
}
int is_email(char *s){
  int at=-1,dot=-1,len=strlen(s);
  for(int i=0;s[i];i++){if(s[i]=='@'){if(at>=0)return 0;at=i;}if(s[i]=='.'&&at>=0)dot=i;}
  return at>0&&dot>at+1&&dot<len-1;
}
int is_phone(char *s){
  if(strlen(s)!=10)return 0;
  for(int i=0;s[i];i++)if(!isdigit(s[i]))return 0;
  return 1;
}
int is_date(char *s){
  if(strlen(s)!=10)return 0;
  for(int i=0;i<10;i++){
    if(i==2||i==5){if(s[i]!='/')return 0;}
    else if(!isdigit(s[i]))return 0;
  }
  return 1;
}
void validate(char *s){
  printf("  Input    : %s\n",s);
  if(is_email(s))        printf("  Result   : VALID EMAIL ADDRESS\n");
  else if(is_phone(s))   printf("  Result   : VALID PHONE NUMBER (10 digits)\n");
  else if(is_date(s))    printf("  Result   : VALID DATE (DD/MM/YYYY)\n");
  else if(is_float(s))   printf("  Result   : VALID FLOAT NUMBER\n");
  else if(is_integer(s)) printf("  Result   : VALID INTEGER\n");
  else if(is_identifier(s))printf("  Result   : VALID IDENTIFIER\n");
  else                   printf("  Result   : INVALID TOKEN\n");
  printf("  ----------------------------------------\n");
}

int main(){
  char input[100]; char clean[100];
  printf("========================================\n");
  printf("   LAB 4: REGULAR EXPRESSION VALIDATOR\n");
  printf("========================================\n");
  printf("Validates: Email, Phone, Date, Float,\n");
  printf("           Integer, Identifier\n");
  printf("Type 'exit' to quit.\n");
  printf("========================================\n\n");
  while(1){
    printf("Enter token to validate: ");
    fgets(input,sizeof(input),stdin);
    /* strip newline */
    int len=strlen(input);
    if(len>0&&input[len-1]=='\n')input[len-1]='\0';
    if(strcmp(input,"exit")==0) break;
    printf("  ----------------------------------------\n");
    validate(input);
    printf("\n");
  }
  printf("========================================\n");
  return 0;
}
