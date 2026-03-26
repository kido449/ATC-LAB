#include <stdio.h>
#include <string.h>
#include <ctype.h>
const char *kw[]={"int","float","char","double","if","else","while","for","return","void","main","printf","scanf","break","continue","switch","case","default","do","struct"};
int is_kw(char*s){for(int i=0;i<20;i++)if(!strcmp(s,kw[i]))return 1;return 0;}
void identify(char*t){
  if(!t[0])return;
  int is_n=1,dot=0;
  for(int i=0;t[i];i++){if(t[i]=='.')dot=1;else if(!isdigit(t[i]))is_n=0;}
  if(is_n){printf("  %-20s -> %s\n",t,dot?"FLOAT":"INTEGER");return;}
  if(isalpha(t[0])||t[0]=='_'){printf("  %-20s -> %s\n",t,is_kw(t)?"KEYWORD":"IDENTIFIER");return;}
  if(!strcmp(t,"+")||!strcmp(t,"-")||!strcmp(t,"*")||!strcmp(t,"/")||!strcmp(t,"%"))printf("  %-20s -> ARITHMETIC OP\n",t);
  else if(!strcmp(t,"==")||!strcmp(t,"!=")||!strcmp(t,"<")||!strcmp(t,">")||!strcmp(t,"<=")||!strcmp(t,">="))printf("  %-20s -> RELATIONAL OP\n",t);
  else if(!strcmp(t,"&&")||!strcmp(t,"||")||!strcmp(t,"!"))printf("  %-20s -> LOGICAL OP\n",t);
  else if(!strcmp(t,"="))printf("  %-20s -> ASSIGNMENT OP\n",t);
  else if(!strcmp(t,"(")||!strcmp(t,")")||!strcmp(t,"{")||!strcmp(t,"}")||!strcmp(t,"[")||!strcmp(t,"]"))printf("  %-20s -> DELIMITER\n",t);
  else if(!strcmp(t,";")||!strcmp(t,","))printf("  %-20s -> PUNCTUATION\n",t);
  else printf("  %-20s -> UNKNOWN\n",t);
}
void tokenize(char*line){
  char tok[100];int ti=0;
  for(int i=0;line[i];i++){
    char c=line[i];
    if((c=='='||c=='!'||c=='<'||c=='>')&&line[i+1]=='='){if(ti){tok[ti]=0;identify(tok);ti=0;}tok[0]=c;tok[1]=line[i+1];tok[2]=0;identify(tok);i++;continue;}
    if((c=='&'&&line[i+1]=='&')||(c=='|'&&line[i+1]=='|')){if(ti){tok[ti]=0;identify(tok);ti=0;}tok[0]=c;tok[1]=line[i+1];tok[2]=0;identify(tok);i++;continue;}
    if(isalnum(c)||c=='.'||c=='_')tok[ti++]=c;
    else{if(ti){tok[ti]=0;identify(tok);ti=0;}if(!isspace(c)){tok[0]=c;tok[1]=0;identify(tok);}}
  }
  if(ti){tok[ti]=0;identify(tok);}
}
int main(){
  char code[][80]={"int x = 10;","float y = 3.14;","if (x > 5) { return x + y; }","while (x != 0) { x = x - 1; }"};
  printf("========================================\n");
  printf("   LAB 1: TOKEN IDENTIFICATION\n");
  printf("========================================\n");
  for(int i=0;i<4;i++){printf("\nSource: %s\n",code[i]);tokenize(code[i]);}
  printf("\n========================================\n");
  return 0;
}
 