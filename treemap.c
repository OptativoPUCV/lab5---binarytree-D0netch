#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * newMap = (TreeMap *)malloc(sizeof(TreeMap));
    if (newMap == NULL) return NULL;
    newMap->lower_than = lower_than;
    newMap->root=NULL;
    newMap->current = NULL;
    return newMap;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  if(tree->root == NULL){
    tree->root = createTreeNode(key,value);
  }
  tree->current = tree->root;
  while(tree->current){
    if(is_equal(tree, tree->current->pair->key, key)){
          return;
        }
    else{
      if(tree->lower_than(tree->current->pair->key,key)){
        if(tree->current->right == NULL){
          tree->current->right = createTreeNode(key,value);
          tree->current->right->parent = tree->current;
        }
        tree->current = tree->current->right;
      }
      else{
        if(tree->current->left == NULL){
        tree->current->left = createTreeNode(key,value);
        tree->current->left->parent = tree->current;
        }
        tree->current = tree->current->left;
      }
    }
  }
}

TreeNode * minimum(TreeNode * x){
  if(x == NULL) return NULL;
  while(x->left!=NULL){
    x = x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if(tree->root == NULL){
    return;
  }
  tree->current = tree->root;
  while(tree->current){
    if(is_equal(tree, tree->current->pair->key, node->pair->key)){
      if(node->right==NULL && node->left==NULL){
        if(tree->lower_than(node->parent->pair->key,node->pair->key)){
          node->parent->right = NULL;
        }
        else{
        node->parent->left = NULL;
        }
        free(node);
        return;  
      }
      else{
        if(node->right==NULL || node->left==NULL){
          if(tree->lower_than(node->parent->pair->key,node->pair->key)){
            if(node->right==NULL){
              node->parent->right = node->left;
              node->left->parent = node->parent;
            }
            else{
              node->parent->right = node->right;
              node->right->parent = node->parent;               
            }
            free(node);
            return;
          }
          else{
            if(node->right==NULL){
              node->parent->left = node->left;
              node->left->parent = node->parent;
            }
            else{
              node->parent->left = node->right;
              node->right->parent = node->parent;              
            }
            free(node);
            return;
          }
        }
        else{
          TreeNode* sucesor = minimum(node->right);
          node->pair->key = sucesor->pair->key;
          node->pair->value = sucesor->pair->value;
          node = sucesor;
          removeNode(tree, sucesor);
          return;
        }
      }
    }
    else{
      if(tree->lower_than(tree->current->pair->key,node->pair->key)){
        tree->current = tree->current->right;
      }
      else{
        tree->current = tree->current->left;
      }
    }
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if(tree->root != NULL){
      tree->current = tree->root;
      while(tree->current){
        if(is_equal(tree, tree->current->pair->key, key)){
          return tree->current->pair;
        }
        else{
          if(tree->lower_than(tree->current->pair->key,key)){
            tree->current = tree->current->right;
          }
          else{
            tree->current = tree->current->left;
          }
        }
      }
    }
    return NULL;
}

Pair* upperBound(TreeMap* tree, void* key) {
    if (tree->root == NULL) {
        return NULL;
    }

    TreeNode* current = tree->root;
    TreeNode* ub_node = NULL;

    while (current != NULL) {
        int cmp = tree->lower_than(current->pair->key, key);

        if (cmp >= 0) {
            if (ub_node == NULL || tree->lower_than(current->pair->key, ub_node->pair->key) < 0) {
                ub_node = current;
            }
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (ub_node != NULL) {
        if (tree->lower_than(ub_node->pair->key, key) == 0) {
            return ub_node->pair;
        }
    }

    return ub_node != NULL ? ub_node->pair : NULL; 
}

Pair * firstTreeMap(TreeMap * tree) {
  if(tree->root == NULL){
    return NULL;
  }
  TreeNode* first = minimum(tree->root);
  return first->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if(tree->root != NULL && tree->current != NULL){
    TreeNode* successor = NULL;
    if (tree->current->right != NULL) {
        successor = minimum(tree->current->right);
    }
    else {
        TreeNode* parent = tree->current->parent;
        while (parent != NULL && tree->current == parent->right) {
            tree->current = parent;
            parent = parent->parent;
        }
        successor = parent;
    }
    if (successor != NULL) {
        tree->current = successor;
        return successor->pair;
    }
  }
  return NULL;
}
