/* LAB 9: DAG OPTIMIZATION - User Input Version */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NODES 100
#define MAX_VARS  50
#define MAX_NAME  20

typedef struct{
  int id; char op[5]; char val[MAX_NAME];
  int left,right;
  char labels[10][MAX_NAME]; int label_count;
}DagNode;

DagNode dag[MAX_NODES]; int node_count=0;
typedef struct{char name[MAX_NAME];int node_idx;}VarMap;
VarMap var_map[MAX_VARS]; int var_count=0;

typedef struct{char result[MAX_NAME];char op[5];char arg1[MAX_NAME];char arg2[MAX_NAME];}TAC;

int find_var(const char*n){for(int i=0;i<var_count;i++)if(!strcmp(var_map[i].name,n))return var_map[i].node_idx;return -1;}
void set_var(const char*n,int idx){
  for(int i=0;i<var_count;i++){if(!strcmp(var_map[i].name,n)){var_map[i].node_idx=idx;return;}}
  strcpy(var_map[var_count].name,n);var_map[var_count].node_idx=idx;var_count++;
}
void add_label(int idx,const char*n){
  DagNode*d=&dag[idx];
  for(int i=0;i<d->label_count;i++)if(!strcmp(d->labels[i],n))return;
  strcpy(d->labels[d->label_count++],n);
}
int find_interior(const char*op,int l,int r){
  for(int i=0;i<node_count;i++)if(!strcmp(dag[i].op,op)&&dag[i].left==l&&dag[i].right==r)return i;
  return -1;
}
int new_leaf(const char*v){
  for(int i=0;i<node_count;i++)if(!strcmp(dag[i].op,"LEAF")&&!strcmp(dag[i].val,v))return i;
  DagNode*n=&dag[node_count];n->id=node_count;strcpy(n->op,"LEAF");strcpy(n->val,v);
  n->left=n->right=-1;n->label_count=0;return node_count++;
}
int new_node(const char*op,int l,int r){
  DagNode*n=&dag[node_count];n->id=node_count;strcpy(n->op,op);strcpy(n->val,"");
  n->left=l;n->right=r;n->label_count=0;return node_count++;
}
int try_fold(const char*op,const char*a,const char*b,int*res){
  char*ea,*eb;int ia=strtol(a,&ea,10),ib=strtol(b,&eb,10);
  if(*ea||*eb)return 0;
  if(!strcmp(op,"+"))*res=ia+ib;else if(!strcmp(op,"-"))*res=ia-ib;
  else if(!strcmp(op,"*"))*res=ia*ib;else if(!strcmp(op,"/")&&ib)*res=ia/ib;else return 0;
  return 1;
}
void process(TAC*t){
  int nl,nr,nres;
  if(!strcmp(t->op,"=")){nl=find_var(t->arg1);if(nl<0)nl=new_leaf(t->arg1);set_var(t->result,nl);add_label(nl,t->result);return;}
  int folded;
  if(try_fold(t->op,t->arg1,t->arg2,&folded)){
    char buf[32];sprintf(buf,"%d",folded);
    printf("  [Constant Fold] %s %s %s = %s\n",t->arg1,t->op,t->arg2,buf);
    nres=new_leaf(buf);set_var(t->result,nres);add_label(nres,t->result);return;
  }
  nl=find_var(t->arg1);if(nl<0)nl=new_leaf(t->arg1);
  nr=find_var(t->arg2);if(nr<0)nr=new_leaf(t->arg2);
  int ex=find_interior(t->op,nl,nr);
  if(ex>=0){printf("  [CSE Eliminated] %s = %s %s %s (reuses node %d)\n",t->result,t->arg1,t->op,t->arg2,ex);nres=ex;}
  else nres=new_node(t->op,nl,nr);
  set_var(t->result,nres);add_label(nres,t->result);
}

/* Parse a TAC line: "result = arg1 op arg2" or "result = arg1" */
int parse_tac(char*line,TAC*t){
  char a[20],b[20],op[5],c[20],eq[3];
  /* try: result = arg1 op arg2 */
  if(sscanf(line,"%s %s %s %s %s",a,eq,b,op,c)==5&&!strcmp(eq,"=")){
    strcpy(t->result,a);strcpy(t->op,op);strcpy(t->arg1,b);strcpy(t->arg2,c);return 1;
  }
  /* try: result = arg1 */
  if(sscanf(line,"%s %s %s",a,eq,b)==3&&!strcmp(eq,"=")){
    strcpy(t->result,a);strcpy(t->op,"=");strcpy(t->arg1,b);strcpy(t->arg2,"");return 1;
  }
  return 0;
}
void regen(int idx,int visited[]){
  if(idx<0||visited[idx])return;visited[idx]=1;
  DagNode*n=&dag[idx];
  if(!strcmp(n->op,"LEAF"))return;
  regen(n->left,visited);regen(n->right,visited);
  if(n->label_count>0){
    char*lv=(dag[n->left].label_count>0)?dag[n->left].labels[0]:dag[n->left].val;
    char*rv=(dag[n->right].label_count>0)?dag[n->right].labels[0]:dag[n->right].val;
    printf("  %s = %s %s %s\n",n->labels[0],lv,n->op,rv);
  }
}

int main(){
  TAC block[20]; int n=0;
  char line[200];
  printf("========================================\n");
  printf("   LAB 9: DAG-BASED CODE OPTIMIZATION\n");
  printf("========================================\n");
  printf("Enter Three-Address Code (TAC) lines.\n");
  printf("Format: result = arg1 op arg2\n");
  printf("   e.g: t1 = a + b\n");
  printf("   e.g: t2 = a + b  (CSE will detect)\n");
  printf("   e.g: t3 = 3 + 5  (constant fold)\n");
  printf("Type 'END' when done.\n");
  printf("========================================\n\n");
  while(n<20){
    printf("> ");
    fgets(line,sizeof(line),stdin);
    int len=strlen(line);
    if(len>0&&line[len-1]=='\n')line[len-1]='\0';
    if(!strcmp(line,"END")) break;
    if(parse_tac(line,&block[n])) n++;
    else printf("  (Invalid format, try: t1 = a + b)\n");
  }
  printf("\n--- Original TAC ---\n");
  for(int i=0;i<n;i++){
    TAC*t=&block[i];
    if(!strcmp(t->op,"="))printf("  %s = %s\n",t->result,t->arg1);
    else printf("  %s = %s %s %s\n",t->result,t->arg1,t->op,t->arg2);
  }
  node_count=0;var_count=0;
  printf("\n--- Building DAG (Optimizations) ---\n");
  for(int i=0;i<n;i++) process(&block[i]);
  printf("\n--- DAG Nodes ---\n");
  printf("  %-4s %-6s %-8s %-5s %-5s Labels\n","ID","Op","Val","Left","Right");
  printf("  ------------------------------------------\n");
  for(int i=0;i<node_count;i++){
    DagNode*d=&dag[i];
    printf("  %-4d %-6s %-8s %-5d %-5d ",d->id,d->op,d->val,d->left,d->right);
    for(int j=0;j<d->label_count;j++)printf("%s ",d->labels[j]);printf("\n");
  }
  printf("\n--- Optimized Code ---\n");
  int visited[MAX_NODES]={0};
  for(int i=node_count-1;i>=0;i--)regen(i,visited);
  printf("\n========================================\n");
  return 0;
}
