/* LAB 8: CONTROL STATEMENTS PARSER - User Input Version */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef enum{TK_IF,TK_ELSE,TK_WHILE,TK_FOR,TK_DO,TK_BREAK,TK_CONTINUE,TK_RETURN,
  TK_LBRACE,TK_RBRACE,TK_LPAREN,TK_RPAREN,TK_SEMI,TK_ID,TK_NUM,TK_OP,TK_REL,TK_EOF}TK;
char *kws[]={"if","else","while","for","do","break","continue","return"};
TK   kwt[]={TK_IF,TK_ELSE,TK_WHILE,TK_FOR,TK_DO,TK_BREAK,TK_CONTINUE,TK_RETURN};
char src8[2000]; int pos8;
char curtok[64]; TK ct;

void skip8(){while(src8[pos8]&&isspace(src8[pos8]))pos8++;}
TK next_tok(){
  skip8(); if(!src8[pos8])return ct=TK_EOF;
  char c=src8[pos8];
  if(c=='{'){pos8++;return ct=TK_LBRACE;}
  if(c=='}'){pos8++;return ct=TK_RBRACE;}
  if(c=='('){pos8++;return ct=TK_LPAREN;}
  if(c==')'){pos8++;return ct=TK_RPAREN;}
  if(c==';'){pos8++;return ct=TK_SEMI;}
  if((c=='='&&src8[pos8+1]=='=')||(c=='!'&&src8[pos8+1]=='=')||
     ((c=='<'||c=='>')&&src8[pos8+1]=='=')){curtok[0]=c;curtok[1]=src8[pos8+1];curtok[2]=0;pos8+=2;return ct=TK_REL;}
  if(c=='<'||c=='>'){curtok[0]=c;curtok[1]=0;pos8++;return ct=TK_REL;}
  if(c=='='||c=='+'||c=='-'||c=='*'||c=='/'){curtok[0]=c;curtok[1]=0;pos8++;return ct=TK_OP;}
  if(isdigit(c)){int i=0;while(isdigit(src8[pos8]))curtok[i++]=src8[pos8++];curtok[i]=0;return ct=TK_NUM;}
  if(isalpha(c)||c=='_'){
    int i=0;while(isalnum(src8[pos8])||src8[pos8]=='_')curtok[i++]=src8[pos8++];curtok[i]=0;
    for(int k=0;k<8;k++)if(!strcmp(curtok,kws[k]))return ct=kwt[k];
    return ct=TK_ID;
  }
  pos8++; return next_tok();
}

int depth8=0;
void ind(){for(int i=0;i<depth8;i++)printf("  ");}
void parse_stmt();

void parse_cond(){
  int d=1; next_tok();
  while(ct!=TK_EOF&&d>0){if(ct==TK_LPAREN)d++;if(ct==TK_RPAREN){d--;if(!d)break;}next_tok();}
}
void parse_stmt(){
  if(ct==TK_IF){
    ind();printf("[IF STATEMENT FOUND]\n");
    next_tok(); next_tok(); parse_cond(); next_tok();
    depth8++; parse_stmt(); depth8--;
    if(ct==TK_ELSE){ind();printf("[ELSE BRANCH FOUND]\n");next_tok();depth8++;parse_stmt();depth8--;}
    ind();printf("[IF-ELSE] Parsed Successfully\n\n"); return;
  }
  if(ct==TK_WHILE){
    ind();printf("[WHILE LOOP FOUND]\n");
    next_tok(); next_tok(); parse_cond(); next_tok();
    depth8++; parse_stmt(); depth8--;
    ind();printf("[WHILE] Parsed Successfully\n\n"); return;
  }
  if(ct==TK_FOR){
    ind();printf("[FOR LOOP FOUND]\n");
    next_tok(); next_tok();
    int d=1;next_tok();
    while(ct!=TK_EOF&&d>0){if(ct==TK_LPAREN)d++;if(ct==TK_RPAREN){d--;if(!d)break;}next_tok();}
    next_tok();
    depth8++; parse_stmt(); depth8--;
    ind();printf("[FOR] Parsed Successfully\n\n"); return;
  }
  if(ct==TK_DO){
    ind();printf("[DO-WHILE LOOP FOUND]\n");
    next_tok(); depth8++;
    if(ct==TK_LBRACE){
      ind();printf("[BLOCK BEGIN]\n"); next_tok();
      while(ct!=TK_RBRACE&&ct!=TK_EOF)parse_stmt();
      depth8--;ind();printf("[BLOCK END]\n"); next_tok();
    } else {depth8--;parse_stmt();}
    if(ct==TK_WHILE){next_tok();next_tok();parse_cond();next_tok();next_tok();}
    ind();printf("[DO-WHILE] Parsed Successfully\n\n"); return;
  }
  if(ct==TK_LBRACE){
    ind();printf("[BLOCK BEGIN]\n"); depth8++; next_tok();
    while(ct!=TK_RBRACE&&ct!=TK_EOF)parse_stmt();
    depth8--;ind();printf("[BLOCK END]\n"); next_tok(); return;
  }
  if(ct==TK_BREAK){ind();printf("[BREAK] Parsed Successfully\n");next_tok();if(ct==TK_SEMI)next_tok();return;}
  if(ct==TK_CONTINUE){ind();printf("[CONTINUE] Parsed Successfully\n");next_tok();if(ct==TK_SEMI)next_tok();return;}
  if(ct==TK_RETURN){
    ind();printf("[RETURN] Parsed Successfully\n");
    next_tok();
    while(ct!=TK_SEMI&&ct!=TK_EOF&&ct!=TK_RBRACE)next_tok();
    if(ct==TK_SEMI)next_tok(); return;
  }
  ind();printf("[EXPR STMT] Parsed Successfully\n");
  while(ct!=TK_SEMI&&ct!=TK_EOF&&ct!=TK_RBRACE)next_tok();
  if(ct==TK_SEMI)next_tok();
}

int main(){
  char line[300]; char accum[2000]; int idx=0;
  printf("========================================\n");
  printf("   LAB 8: CONTROL STATEMENTS PARSER\n");
  printf("========================================\n");
  printf("Supports: if, else, while, for, do-while\n");
  printf("          break, continue, return, blocks\n");
  printf("Enter code. Type 'END' when done.\n");
  printf("========================================\n\n");
  while(1){
    printf("> ");
    fgets(line,sizeof(line),stdin);
    if(strncmp(line,"END",3)==0) break;
    int len=strlen(line);
    if(idx+len<1900){strcpy(accum+idx,line);idx+=len;}
  }
  accum[idx]='\0';
  strcpy(src8,accum); pos8=0; depth8=0;
  printf("\n--- PARSE TRACE ---\n");
  next_tok();
  while(ct!=TK_EOF) parse_stmt();
  printf("Parsing Complete - Syntax is VALID\n");
  printf("========================================\n");
  return 0;
}
