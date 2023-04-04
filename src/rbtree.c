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
//남음
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  
  return t->root;
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

//남음
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
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

}

//남음
int rbtree_erase(rbtree *t, node_t *p) 
{
  // TODO: implement erase
  node_t* y = p;
  color_t y_orig_color = y->color;
  //1. left 밖에 없으니까 오른쪽의 것을 심음
  if (p->left == t->nil)
  {
    node_t *x = p->right;
    //nil이니까 그것에 다가 넣는다 어차피 오른 쪽은 무엇이 있든간에
    //어쨋든 넣는 것! 
    rbtree_transplant(t, p, p->right);
  }
  
  //2. right 밖에 없으니까 왼쪽의 것을 심음
  else if (p->right == t->nil)
  {
    node_t *x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else
  {
    // 서브트리를 만들어서 그것의 최하를 구해주자!
    y = subtree_min(t, p->right);
    y_orig_color = y->color;
    node_t *x = y->right;
    //한개만 있을 떄
    if (y->parent == p)
    {
      x->parent = y;
    }
    else
    {
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
    rbtree_delete_fixup(t, x);
  }
  } 
  return 0;
}

//남음
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}


/**my functions*/