#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  // rbtree자체를 메모리 할당해줌
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // node_t struct 의 NIL 생성
  node_t *SENTINEL = (node_t *)calloc(1, sizeof(node_t));
  // NIL의 색은 검정
  SENTINEL->color = RBTREE_BLACK;
  // 여기서 문제인데 NULL을 반환하라고 했으니
  p->nil = SENTINEL;
  p->root = SENTINEL; 
  return p;
}

void left_rotate(rbtree *t, node_t *x)
{
  // x->right가 nil이 아니라는 가정 하에 함수 진행
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil)// 여기서 왜 nil을 SENTINEL을 쓰면 안되는 거지?
  {
    y->left->parent = x;      
  }
  y->parent = x->parent;
  if (x->parent == t->nil){
    t->root = y;
  }
  else if(x == x->parent->left){
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *y)
{
  // 
  node_t *x = y->left;
  //y.left = x.right
  y->left = x->right;
  if (x->right != t->nil)
  {
    x->right->parent = y;
  }
  x->parent = y->parent;
  if (y->parent == t->nil)
  {
    t->root = x;
  }
  else if(y == y->parent->left)
  {
    y->parent->left = x;
  }
  else
  {
    y->parent->right = x;
  }
  x->right = y;
  y->parent = x;
}

void rbtree_insert_fixup(rbtree *t, node_t *z)
{
  while (z->parent->color == RBTREE_RED)
  {
    //#1 삼촌이가 어디있니
    if (z->parent == z->parent->parent->left)
    {
      node_t *uncle = z->parent->parent->right;
      //case1
      if (uncle->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->right)
        {
          z= z->parent;
          left_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    //#2
    else
    {
      node_t *uncle = z->parent->parent->left;
      //case1
      if (uncle->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z=z->parent->parent;
      }
      else// case2, 3
      {
        if (z==z->parent->left)
        {
          z = z->parent;
          right_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  //세팅해놓기
  node_t *z = (node_t *)malloc(sizeof(node_t));
  // y = tree의 nil값
  node_t *tmp_parent = t->nil;
  // tmp = child tree의 root 값
  node_t *tmp_child = t->root;
  // 찾아 들어가기
  while (tmp_child != t->nil)
  {
    tmp_parent = tmp_child;
    if (key < tmp_child->key)
      tmp_child = tmp_child->left;
    else
      tmp_child = tmp_child->right;
  }
  //z.p == y; 즉 tmp parent로 찾아들어감
  z->parent = tmp_parent;
  //만약 tmp parent가 nil 즉 아무것도 없어
  if (tmp_parent == t->nil)
  {
    t->root = z;
  }
  else if (key < tmp_parent->key)
  tmp_parent->left = z;
  else
  tmp_parent->right = z;
  //z의 값을 처음부터 초기화하기
  z->key = key;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED; // 빨간색으로 넣음 =
  rbtree_insert_fixup(t, z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t* temp = t->root;
  while(temp != t->nil)
  {
    if(temp->key > key)
      temp = temp->left;
    else if(temp->key < key)
      temp = temp->right;
    else
      return temp;
  }
  return NULL;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void delete_node(rbtree *t, node_t* n)
{
  if (n== t->nil){
    return;
  }
  delete_node(t, n->left);
  delete_node(t, n->right);
  free(n);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t, t->root);
  free(t->nil);
  free(t);
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  if (t->nil == t->root)
  {
    return NULL;
  }
  while(current->left != t->nil)
  {
    current = current->left;
  }
  return current;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  if (t->nil == t->root)
  {
    return NULL;
  }
  while(current->right != t->nil)
  {
    current = current->right;
  }
  return current;
}

node_t *subtree_min(rbtree *t, node_t* z)
{
  node_t* current = z;
  if (z == t->nil)
  {
    return NULL;
  }
  while(current->left != t->nil)
  {
    current = current->left;
  }
  return current;
}

void rbtree_erase_fixup(rbtree *t, node_t *x)
{
  // 루트까지 가지 않고 색이 검은색일때까지?
  // 왜 빨간색은? ... 
while(x!= t->root && x->color == RBTREE_BLACK)
{
  if (x==x->parent->left)
  // sibling 찾기
  {
    node_t* w = x->parent->right;
    if (w->color == RBTREE_RED)
    {
      //case1: 
      w->color = RBTREE_BLACK; 
      x->parent->color = RBTREE_RED;
      left_rotate(t, x->parent);
      w = x->parent->right;
    }
    //case2 
    if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
    {
      w->color = RBTREE_RED;
      x = x->parent;
    }
    //case3
    else 
    {
      if (w->right->color == RBTREE_BLACK)
      {
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        right_rotate(t, w);
        w = x->parent->right;
      }
      w->color = x->parent->color;
      x->parent->color = RBTREE_BLACK;
      w->right->color = RBTREE_BLACK;
      left_rotate(t, x->parent);
      x = t->root;
    }
  }
  else // other direction
  {
    node_t* w = x->parent->left;
    if (w->color == RBTREE_RED)
    {
      w->color = RBTREE_BLACK;
      x->parent->color = RBTREE_RED;
      right_rotate(t, x->parent);
      w = x->parent->left;
    }
    if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
    {
      w->color = RBTREE_RED;
      x = x->parent;
    }
    else 
    {
      if (w->left->color == RBTREE_BLACK)
    {
      w->right->color = RBTREE_BLACK;
      w->color = RBTREE_RED;
      left_rotate(t, w);
      w= x->parent->left;
    }
    w->color = x->parent->color;
    x->parent->color = RBTREE_BLACK;
    w->left->color = RBTREE_BLACK;
    right_rotate(t, x->parent);
    x = t->root;
    }
  }
  x->color = RBTREE_BLACK;
}
}

int rbtree_erase(rbtree *t, node_t *p) 
{
  // TODO: implement erase
  node_t* y = p;
  node_t* x;
  color_t y_orig_color = y->color;
  //1. right 밖에 없으니까 오른쪽의 것을 심음
  if (p->left == t->nil)
  {
    x = p->right;
    //nil이니까 그것에 다가 넣는다 어차피 오른 쪽은 무엇이 있든간에
    //어쨋든 넣는 것! 
    rbtree_transplant(t, p, p->right);
  }
  
  //2. left 밖에 없으니까 왼쪽의 것을 심음
  else if (p->right == t->nil)
  {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else
  {
    // 서브트리를 만들어서 그것의 최하를 구해주자! --> 이뜻은 지울것보다 큰 것의 가장 가까운것 
    y = subtree_min(t, p->right);
    y_orig_color = y->color;
    // 여기서 x는 subtree minimum의 오른쪽
    node_t *x = y->right;
    //한개만 있을 떄 즉 p에서 바로 내려갔는데 그게 y야
    if (y->parent == p)
    {
      x->parent = y;
    }
    //여러개 있을 때
    else
    {
      // 한마디로 p없앤 자리에 y, y없앤 자리에 y->right 넣어버리기
      //rbtree y minimum 그 자리에다가 y right를 넣어버리고
      rbtree_transplant(t,y,y->right);
      y->right = p->right;
      y->right->parent = y;
    }
      // p룰 없앤 자리에 y심기
      rbtree_transplant(t,p,y);
      y->left = p->left;
      y->left->parent = y;
      //색도 동일하게 바꿔주기
      y->color = p->color;
  if(y_orig_color == RBTREE_BLACK)
  {
    rbtree_erase_fixup(t, x);
  }
  } 
  free(p);
  return 0;
}

int insert_node_array(const rbtree* t, node_t *cur, key_t *arr, int i, const size_t n)
{

}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) 
{
  // TODO: implement to_array
  return 0;
}