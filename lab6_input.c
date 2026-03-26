/* LAB 6: GRAMMAR AMBIGUITY DEMO - User Input Version */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Ambiguous: left-to-right, no precedence */
char *ia; int pa;
void sa(){while(isspace(ia[pa]))pa++;}
int amb(){
  sa(); int v=0;
  while(isdigit(ia[pa]))v=v*10+(ia[pa++]-'0');
  sa();
  while(ia[pa]=='+'||ia[pa]=='-'||ia[pa]=='*'||ia[pa]=='/'){
    char op=ia[pa++]; int r=0; sa();
    while(isdigit(ia[pa]))r=r*10+(ia[pa++]-'0');
    if(op=='+')v+=r; else if(op=='-')v-=r;
    else if(op=='*')v*=r; else if(r)v/=r;
    sa();
  }
  return v;
}

/* Unambiguous: correct precedence */
char *iu; int pu;
void su(){while(isspace(iu[pu]))pu++;}
int ue_f(){su();int v=0;while(isdigit(iu[pu]))v=v*10+(iu[pu++]-'0');return v;}
int ue_t(){int v=ue_f();su();while(iu[pu]=='*'||iu[pu]=='/'){char o=iu[pu++];int r=ue_f();v=(o=='*')?v*r:(r?v/r:0);su();}return v;}
int ue_e(){int v=ue_t();su();while(iu[pu]=='+'||iu[pu]=='-'){char o=iu[pu++];int r=ue_t();v=(o=='+')?v+r:v-r;su();}return v;}

int main(){
  char line[200];
  printf("========================================\n");
  printf("   LAB 6: GRAMMAR AMBIGUITY DEMO\n");
  printf("========================================\n");
  printf("Enter a numeric expression like: 2+3*4\n");
  printf("(Only integers and +,-,*,/ supported)\n");
  printf("Type 'exit' to quit.\n");
  printf("========================================\n\n");
  while(1){
    printf("Enter expression: ");
    fgets(line,sizeof(line),stdin);
    int len=strlen(line);
    if(len>0&&line[len-1]=='\n')line[len-1]='\0';
    if(strcmp(line,"exit")==0) break;

    ia=line; pa=0; int a=amb();
    iu=line; pu=0; int u=ue_e();

    printf("\n  Expression        : %s\n",line);
    printf("  ----------------------------------------\n");
    printf("  Ambiguous  (L->R) : %d\n",a);
    printf("  Unambiguous (prec): %d\n",u);
    if(a!=u){
      printf("  ** AMBIGUITY DETECTED! **\n");
      printf("  Ambiguous ignores precedence (*,/ before +,-)\n");
      printf("  Correct answer is : %d\n",u);
    } else {
      printf("  Both give same result (no ambiguity here)\n");
    }
    printf("\n");
  }
  printf("========================================\n");
  return 0;
}
