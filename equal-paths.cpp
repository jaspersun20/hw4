using namespace std;
#include "equal-paths.h"
#include <algorithm>


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    // Add your code below
    int lheight =0;
    int rheight = 0;
    if(calculatePaths(root, lheight, rheight)==-1){
        return false;
    }
    else {
        return true;
    }
}

int calculatePaths(Node * root, int lheight, int rheight) {
    // Base case: empty tree has equal paths
	if(root==NULL){
        return 0;
    }
    if(root->left==NULL&&root->right==NULL){
        return 0;
    }
    int l = calculatePaths(root->left, ++lheight, rheight);
    int r = calculatePaths(root->right, lheight, ++rheight);
    if(l!=r){
        return -1;
    }
    return max(lheight, rheight);
}


