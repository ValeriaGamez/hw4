#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"

using namespace std;


// You may add any prototypes of helper functions here
int height(Node* root);


bool equalPaths(Node* root)
{
    // Add your code below
    if(root == NULL){
        return true;
    }
    
    if(root->left == NULL && root->right != NULL){
        return equalPaths(root->right);
    }

    if(root->right == NULL && root->left != NULL){
        return equalPaths(root->left); 
    }
    
    if((height(root->left) != height(root->right))){
        return false; 
    }
 
    return true;
}

int height(Node *root) {
	if(root == NULL){
		return 0; 
	}
	return 1 + max(height(root->left), height(root->right));
}