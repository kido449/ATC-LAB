/* LAB 7: AST BUILDER - User Input Version */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum{NUM,BINOP}NType;
typedef struct Node{
  NType type; int val; char op;
  struct Node *left,*right;
}Node;

Node* mknum(int v){Node*n=malloc(sizeof(Node));n->type=NUM;n->val=v;n->left=n->right=NULL;return n;}
Node* mkop(char o,Node*l,Node*r){Node*n=malloc(sizeof(Node));n->type=BINOP;n->op=o;n->left=l;n->right=r;return n;}

void print_ast(Node*n,char*prefix,int is_last){
  if(!n)return;
  printf("%s%s",prefix,is_last?"└── ":"├── ");
  if(n->type==NUM) printf("NUM(%d)\n",n->val);
  else             printf("BINOP('%c')\n",n->op);
  char np[200]; strcpy(np,prefix); strcat(np,is_last?"    ":"│   ");
  if(n->type==BINOP){print_ast(n->left,np,0);print_ast(n->right,np,1);}
}
int eval(Node*n){
  if(!n)return 0;
  if(n->type==NUM)return n->val;
  int l=eval(n->left),r=eval(n->right);
  switch(n->op){case'+':return l+r;case'-':return l-r;case'*':return l*r;case'/':return r?l/r:0;}
  return 0;
}
void free_ast(Node*n){if(!n)return;free_ast(n->left);free_ast(n->right);free(n);}

char *src7; int p7;
void s7(){while(isspace(src7[p7]))p7++;}
Node* ae7(); Node* at7(); Node* af7();
Node* af7(){
  s7();
  if(src7[p7]=='('){p7++;Node*v=ae7();s7();if(src7[p7]==')')p7++;return v;}
  int v=0,neg=0;
  if(src7[p7]=='-'){neg=1;p7++;}
  while(isdigit(src7[p7]))v=v*10+(src7[p7++]-'0');
  return mknum(neg?-v:v);
}
Node* at7(){Node*v=af7();s7();while(src7[p7]=='*'||src7[p7]=='/'){char o=src7[p7++];Node*r=af7();v=mkop(o,v,r);s7();}return v;}
Node* ae7(){Node*v=at7();s7();while(src7[p7]=='+'||src7[p7]=='-'){char o=src7[p7++];Node*r=at7();v=mkop(o,v,r);s7();}return v;}

int main(){
  char line[200];
  printf("========================================\n");
  printf("   LAB 7: ABSTRACT SYNTAX TREE (AST)\n");
  printf("========================================\n");
  printf("Enter an arithmetic expression.\n");
  printf("Operators: +  -  *  /  ( )\n");
  printf("Type 'exit' to quit.\n");
  printf("========================================\n\n");
  while(1){
    printf("Enter expression: ");
    fgets(line,sizeof(line),stdin);
    int len=strlen(line);
    if(len>0&&line[len-1]=='\n')line[len-1]='\0';
    if(strcmp(line,"exit")==0) break;
    src7=line; p7=0;
    Node*tree=ae7();
    printf("\n  Expression : %s\n",line);
    printf("  AST:\n");
    print_ast(tree,"  ",1);
    printf("  Evaluated  : %d\n\n",eval(tree));
    free_ast(tree);
  }
  printf("========================================\n");
  return 0;
}
