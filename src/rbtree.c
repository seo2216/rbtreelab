#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) { // 노드생성-1.노드(부모)생성 2. 왼쪽자식생성 3. 오른쪽 자식생성
// TODO: initialize struct if needed
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *NIL = (node_t *)calloc(1,sizeof(node_t));

  NIL->color = RBTREE_BLACK;
  p->root = NIL; //sentinel node가 없다면 NULL 대입
  p->nil = NIL;

  return p;
}

void left_rocate(rbtree *t, node_t *x){ // x를 기준으로 왼쪽 회전
  node_t *y;
  y = x->right;
  x->right = y->left;
  if(y->left != t->nil){
    y->left->parent =x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil){
    t->root = y;
  }
  else if(x==x->parent->left){
    x->parent->left = y;
  }
  else{
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void right_rocate(rbtree *t, node_t *x){ //x를 기준으로 오른쪽회전
// y는 x의 왼쪽자식노드, x는 y의 오른쪽 자식이 되고, y의 오른쪽 자식은 x의 왼쪽 자식이됨
  node_t *y;
  y = x->left;
  x->left = y->right;

  if(y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil){
    t->root = y;
  }
  else if(x == x->parent->right){
    x->parent->right = y;
  }
  else{
    x->parent->left = y;  
  }
  y->right = x;
  x->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *z){

  node_t *y;

  while (z->parent->color == RBTREE_RED) //target의 부모가 적색
  {
    //z의 부모가 조부모의 왼쪽 서브트리인 경우
    if(z->parent == z->parent->parent->left){ 
      y= z->parent->parent->right; //y은 삼촌(부모의 형제)

      //case1: z의 삼촌y가 red인 경우
      if(y->color == RBTREE_RED){ 
        z->parent->color = RBTREE_BLACK; 
        y->color = RBTREE_BLACK;
        z->parent->parent->color= RBTREE_RED;
        z= z->parent->parent;
      }

      else{ //case2 : z의 삼촌 y가 black이며, z가 오른쪽 자식일 경우
        if(z == z->parent->right){
          z = z->parent; 
          left_rocate(t,z); 
        }
        //case3 :  
        z->parent->color = RBTREE_BLACK; //case3
        z->parent->parent->color = RBTREE_RED;
        right_rocate(t,z->parent->parent);
      } 
    }

    else{ //z의 부모가 조부모의 오른쪽 서브트리인 경우
      y  = z->parent->parent->left;

      //case 1
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{
        if(z == z->parent->left){ //case2 :z의 삼촌 y가 black이며, z가 왼쪽 자식일 경우
          z = z->parent;
          right_rocate(t,z);
        }
        //case3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rocate(t,z->parent->parent);
        } 
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *y = t->nil;
  node_t *x = t->root;
  node_t *z = (node_t *)calloc(1, sizeof(node_t));

  z->key = key;

  while(x != t->nil){
    y=x;
    if(z->key < x->key){
      x= x->left;
    }
    else{
      x=x->right;
    }
  }

  z->parent = y;

  if (y == t->nil){
    t->root = z;
  }
  else if(z->key < y->key){
    y->left = z;
  }
  else{
    y->right =z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color= RBTREE_RED;

  rbtree_insert_fixup(t,z);

  return t->root;
}

void delete_node(rbtree *t, node_t *x){
  // if (x->right == NULL || x->left == NULL){
  //   printf("%p",t->nil);
  //   return;
  // }
  if(x == t->nil){
    return;
  }

  delete_node(t,x->left);
  delete_node(t,x->right);
  free(x);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  //RB tree 구조체가 차지했던 메모리 반환
  if(t->root == t->nil){
    free(t->nil);
    free(t);
    return;
  }
  node_t *x = t->root;

  delete_node(t,x);
  free(t->nil);
  free(t);
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x = t->root;

  while(x != t->nil){
    if(x->key == key)
      return x;
    else if(x->key > key)
      x = x->left;
    else
      x= x->right;
  }
  return NULL; // 찾는 값이 없으면 null 반환
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;

  if(x == t->nil){
    return x;
  }
  while (x->left != t->nil)
  {
    x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;

  if(x == t->nil)
    return x;

  while(x->right != t->nil)
    x = x->right;

  return x;
}

//삭제할 노드의 오른쪽 자식 중 가장 작은값(대체할 값)
node_t *rbtree_minmum(const rbtree *t, node_t *subtree){
  node_t *x = subtree;

  if(x == t->nil)
    return x;
  while(x->left != t->nil)
    x= x->left;

  return x;
}

void rbtree_transplant(rbtree *t,node_t *u, node_t *v){
  //u: 삭제할 노드, v: 대체할 노드
  if(u->parent == t->nil) //삭제할 노드의 부모가 t->nill 이라면
    t->root = v;
  else if(u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree *t, node_t *x){

  while(x !=t->root && x->color == RBTREE_BLACK){ //삭제되는 색이 black이면 #2,#4,#5 위반가능
    if( x == x->parent->left){//나는야 왼쪽트리
       node_t *w = x->parent->right; //형제노드

      if(w->color == RBTREE_RED){ //case1 : x의 형제인 w color가 red이면
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rocate(t,x->parent);
        w = x->parent->right; //->doublyblack의 형제를 w에 다시 대입
      }
      //case2 : w color가 black이고, w의 두 자식이 black인 경우
      if((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK)){ 
        w->color = RBTREE_RED;
        x= x->parent; //extrablack의 위치를 변경
      }
      //case3 : w color가 black, w의 왼쪽자식 red, 오른쪽 자식 black
      else{
        if(w->right->color == RBTREE_BLACK){ //오른쪽 자식을 red로 만들어 case4적용
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rocate(t,w);
          w=x->parent->right; //->doublyblack의 형제를 w에 다시 대입
        }
        //case 4 :w color가 black, w의 왼쪽자식 black, 오른쪽 자식 red
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rocate(t,x->parent); // red-and-black이 됨 => x->color를 black으로만 바꿔주면됨
        x= t->root; // 종료조건
      }
    }
    else{ //case 5~8 right case
      node_t *w = x->parent->left; //나의 형제(이번에 왼쪽)

      if(w->color == RBTREE_RED){ //case5: 형제 w가 red 일때
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rocate(t,x->parent);
        w = x->parent->left;
      }
      //case6 : w은 black, 두 자식은 black
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){ 
        w->color = RBTREE_RED;
        x= x->parent;
      }
      else{ 
        //case 7: w은 black, w의 왼쪽자식은 black, 오른쪽 자식은 red?
        if(w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rocate(t,w);
          w =x->parent->left;
        }
        //case 8 :w는 black, w의 왼쪽 자식 red, 오른쪽 black?
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rocate(t,x->parent);
        x= t->root;
      }      
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y = p;
  color_t y_original_color = y->color; //삭제할 target노드 색 저장
  node_t *x; //대체할 노드를 저장할 용도
  if(p->left == t->nil){ //target의 자식정보 확인
    x= p->right;
    rbtree_transplant(t,p,p->right);
  }
  else if(p->right == t->nil){
    x = p->left;
    rbtree_transplant(t,p,p->left);
  }
  else{ //삭제할 target의 양쪽자식이 있을때
    y= rbtree_minmum(t,p->right); // 삭제할 target을 대체할 노드를 y에 저장
    y_original_color = y->color; //대체할 노드의 color를 저장(삭제될 color임)
    x=y->right; //대체할 노드의 오른쪽 저장(왼쪽 자식은 nil이지만, 오른쪽 자식은 존재 가능)

    if(y->parent == p) {
      x->parent = y; }//y가 삭제할 노드 바로 밑에 붙어있는 경우
    else{
      rbtree_transplant(t,y,y->right); //y자리에 y->right 가 오도록 함
      y->right = p->right;
      y->right->parent = y;
    }
    rbtree_transplant(t,p,y); //삭제될 p와 y의 자리를 바꿈(이식)
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if(y_original_color == RBTREE_BLACK){ //red라면 속성위반X
    rbtree_delete_fixup(t,x); 
  }
  free(p);
  return 0;
}

int rbtree_inorder(node_t *root, key_t *arr, const rbtree *t, int i){
  if(root == t->nil){
    return i;
  } 
  i = rbtree_inorder(root->left,arr,t,i);
  arr[i] = root->key;
  i += 1;
  i = rbtree_inorder(root->right,arr,t,i);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if(t->root == t->nil){
    return -1;
  }
  rbtree_inorder(t->root,arr,t,0);
  return 0;
}
