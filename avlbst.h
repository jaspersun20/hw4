#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
	virtual void insertFix (AVLNode<Key,Value>* p, AVLNode<Key,Value>* n); 
	virtual void removeFix (AVLNode<Key,Value>* n, int diff); 
	virtual void leftRotate (AVLNode<Key,Value>* x); 
	virtual void rightRotate (AVLNode<Key,Value>* z); 

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    // BinarySearchTree<Key, Value>::
		// If empty tree, set root
		// AVLNode<Key, Value>* avlroot = static_cast<AVLNode<Key, Value>*>(this->root_);
		if(this->root_==NULL){
			AVLNode<Key, Value>* new_root = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
			this->root_=new_root;
			new_root->setBalance(0);
			return;
    }
		// Only modify value if key is already in tree
		else if(BinarySearchTree<Key, Value>::internalFind(new_item.first)!=NULL){
			BinarySearchTree<Key, Value>::internalFind(new_item.first)->setValue(new_item.second);
			return;
		}
		else{
			AVLNode<Key, Value>* avlroot = static_cast<AVLNode<Key, Value>*>(this->root_);
			AVLNode<Key, Value>* temp = avlroot;
			AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
			while(temp!=NULL){
					if(new_item.first < temp->getKey()&&temp->getLeft()==NULL){
						temp->setLeft(new_node);
						new_node->setParent(temp);
						new_node->setBalance(0);
						int bp = temp->getBalance();
						if(bp==1){
							temp->setBalance(0);
						}
						else{
							temp->updateBalance(-1);
							insertFix(temp, new_node);
						}
						// this->print();
						return;
					}
					else if(new_item.first < temp->getKey()){
						temp=temp->getLeft();
					}
					else if((new_item.first > temp->getKey()) && (temp->getRight()==NULL)){
						temp->setRight(new_node);
						new_node->setParent(temp);
						new_node->setBalance(0);
						int bp = temp->getBalance();
						if(bp==-1){
							temp->setBalance(0);
						}
						else{
							temp->updateBalance(1);
							insertFix(temp, new_node);
						}
						// this->print();
						return;
					}
					else if(new_item.first > temp->getKey()){
						temp=temp->getRight();
					}
			}
			

		}


}

// insertFix
template<class Key, class Value>
void  AVLTree<Key, Value>::insertFix (AVLNode<Key,Value>* p, AVLNode<Key,Value>* n){
	// base case
	if(p==NULL||p->getParent()==NULL){
		return;
	}
	AVLNode<Key,Value>* g = p->getParent();
	// update grandparents' balance
	if(g->getLeft()==p){
		g->updateBalance(-1);
	}
	else if(g->getRight()==p){
		g->updateBalance(1);
	}
	// go left
	if(p==g->getLeft()){
		if(g->getBalance()==0){
			return;
		}
		else if(g->getBalance()==-1){
			insertFix(g,p);
		}
		// zig zig
		else if(g->getBalance()==-2){
			if(p->getLeft()==n){
				rightRotate(g);
				g->setBalance(0);
				p->setBalance(0);
			}
			// zig zag
			else if(p->getRight()==n){
				leftRotate(p);
				rightRotate(g);
				if(n->getBalance()==-1){
					p->setBalance(0);
					g->setBalance(1);
					n->setBalance(0);
				}
				else if(n->getBalance()==0){
					p->setBalance(0);
					g->setBalance(0);
					n->setBalance(0);
				}
				else if(n->getBalance()==1){
					p->setBalance(-1);
					g->setBalance(0);
					n->setBalance(0);
				}
			}
		}
	}
	else if(p==g->getRight()){
		// go right
		if(g->getBalance()==0){
			return;
		}
		else if(g->getBalance()==1){
			insertFix(g,p);
		}
		else if(g->getBalance()==2){
			// zig zig
			if(p->getRight()==n){
				leftRotate(g);
				g->setBalance(0);
				p->setBalance(0);
				return;
			}
			else if(p->getLeft()==n){
				// zig zag
				rightRotate(p);
				leftRotate(g);
				if(n->getBalance()==1){
					p->setBalance(0);
					g->setBalance(-1);
					n->setBalance(0);
				}
				else if(n->getBalance()==0){
					p->setBalance(0);
					g->setBalance(0);
					n->setBalance(0);
				}
				else if(n->getBalance()==-1){
					p->setBalance(1);
					g->setBalance(0);
					n->setBalance(0);
				}
			}
		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate (AVLNode<Key,Value>* x){
	AVLNode<Key,Value>* y = x->getRight();
	AVLNode<Key,Value>* p = x->getParent();
	AVLNode<Key,Value>* b = y->getLeft();
	y->setParent(p);
	//if x isn't root
	if(x->getParent()!=NULL){
		// fixing y
		if(p->getLeft()==x){
			p->setLeft(y);
		}
		else{
			p->setRight(y);
		}
		// fixing x
		x->setParent(y);
		x->setRight(b);
		y->setLeft(x);
		if(b!=NULL){
			b->setParent(x);
		}
	}
	else{
		this->root_=y;
		x->setParent(y);
		x->setRight(b);
		y->setLeft(x);
		if(b!=NULL){
			b->setParent(x);
		}
	}
} 

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate (AVLNode<Key,Value>* z){
	AVLNode<Key,Value>* y = z->getLeft();
	AVLNode<Key,Value>* p = z->getParent();
	AVLNode<Key,Value>* c = y->getRight();
	y->setParent(p);
	//if z isn't root
	if(p!=NULL){
		// fixing y
		if(p->getLeft()==z){
			p->setLeft(y);
		}
		else{
			p->setRight(y);
		}
		// fixing x
		z->setParent(y);
		z->setLeft(c);
		y->setRight(z);
		if(c!=NULL){
			c->setParent(z);	
		}
	}
	// if z is root
	else{
		// AVLNode<Key, Value>* avlroot = static_cast<AVLNode<Key, Value>*>(this->root_);
		this->root_=y;
		z->setParent(y);
		z->setLeft(c);
		y->setRight(z);
		if(c!=NULL){
			c->setParent(z);	
		}
	}
} 


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  // TODO
	// base case
    if(BinarySearchTree<Key, Value>::internalFind(key)==NULL){
			return;
    }
    Node<Key, Value>* bstnode = BinarySearchTree<Key, Value>::internalFind(key);
		AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(bstnode);
    // one child or 0 child
		// AVLNode<Key, Value>* avlroot = static_cast<AVLNode<Key, Value>*>(this->root_);
		int diff=0;
		// set diff for remove fix
		if(temp->getParent()!=NULL){
			if(temp->getParent()->getLeft()==temp){
				diff=1;
			}
			else if(temp->getParent()->getRight()==temp){
				diff=-1;
			}
		}
    if(temp->getLeft()==NULL){	
			AVLNode<Key, Value>* p = temp->getParent();
      // 1child case in right
			if(temp->getRight()!=NULL){
				if(this->root_==temp){
					this->root_=temp->getRight();
					temp=NULL;
					delete temp;
					return;
				}
				AVLNode<Key, Value>* r = temp->getRight();
				if(temp==temp->getParent()->getRight()){
					p->setRight(r);
				}
				if(temp==temp->getParent()->getLeft()){
					p->setLeft(r);
				}
				r->setParent(p);
				temp=NULL;
				delete temp;
				removeFix(p,diff);
				return;
				}
        // 0 child case
				if(temp->getParent()==NULL){
					this->root_=NULL;
					temp=NULL;
					delete temp;
					return;
				}
				if(temp->getParent()->getLeft()==temp){
					temp->getParent()->setLeft(NULL);
				}
				else if(temp->getParent()->getRight()==temp){
					temp->getParent()->setRight(NULL);
				}
				temp=NULL;
				delete temp;
				removeFix(p,diff);
			return;
    }
    else if(temp->getRight()==NULL){
      // 1child case in left
			if(temp->getLeft()!=NULL){
				if(this->root_==temp){
					this->root_=temp->getLeft();
					temp=NULL;
					delete temp;
					return;
				}
				AVLNode<Key, Value>* l = temp->getLeft();
				AVLNode<Key, Value>* p = temp->getParent();
				if(temp==temp->getParent()->getRight()){
					p->setRight(l);
				}
				if(temp==temp->getParent()->getLeft()){
					p->setLeft(l);
				}
				l->setParent(p);
				// this->print();
				temp=NULL;
				delete temp;
				removeFix(p,diff);
				return;
				}
			}
    
    // two children case 
    else{
			Node<Key, Value>* pre = BinarySearchTree<Key, Value>::predecessor(bstnode);
			AVLNode<Key, Value>* avlpre = static_cast<AVLNode<Key, Value>*>(pre);
			nodeSwap(temp,avlpre);
			avlpre->setValue(temp->getValue());
			remove(temp->getKey());
			return;
    }

}

template<class Key, class Value>
void  AVLTree<Key, Value>::removeFix (AVLNode<Key,Value>* n, int diff){
	if(n==NULL){return;}
	int ndiff=0;
	AVLNode<Key,Value>* p = n->getParent();
	if(p!=NULL&&p->getLeft()==n){
		ndiff=1;
	}
	else if(p!=NULL&&p->getRight()==n){
		ndiff=-1;
	}
	int bn=n->getBalance();
	if(diff==-1){
		// case 1
		if(bn+diff==-2){
			AVLNode<Key,Value>* c = n->getLeft();
			// case 1a
			if(c->getBalance()==-1){
				rightRotate(n);
				n->setBalance(0);
				c->setBalance(0);
				removeFix(p,ndiff);
			}
			// case 1b
			else if(c->getBalance()==0){
				rightRotate(n);
				n->setBalance(-1);
				c->setBalance(1);
				return;
			}
			// case 1c
			else if(c->getBalance()==1){
				AVLNode<Key,Value>* g = c->getRight();
				leftRotate(c);
				rightRotate(n);
				if(g->getBalance()==1){
					n->setBalance(0);
					c->setBalance(-1);
					g->setBalance(0);
				}
				else if(g->getBalance()==0){
					n->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				}
				else if(g->getBalance()==-1){
					n->setBalance(1);
					c->setBalance(0);
					g->setBalance(0);
				}
				removeFix(p,ndiff);
			}
		}
		else if(bn+diff==-1){
			n->setBalance(-1);
		}
		else if(bn+diff==0){
			n->setBalance(0);
			removeFix(p,ndiff);
		}
	}
	else if(diff==1){
		// case 1
		if(bn+diff==2){
			AVLNode<Key,Value>* c = n->getRight();
			// case 1a
			if(c->getBalance()==1){
				leftRotate(n);
				n->setBalance(0);
				c->setBalance(0);
				removeFix(p,ndiff);
			}
			// case 1b
			else if(c->getBalance()==0){
				leftRotate(n);
				n->setBalance(1);
				c->setBalance(-1);
				return;
			}
			// case 1c
			else if(c->getBalance()==-1){
				AVLNode<Key,Value>* g = c->getLeft();
				rightRotate(c);
				leftRotate(n);
				if(g->getBalance()==-1){
					n->setBalance(0);
					c->setBalance(1);
					g->setBalance(0);
				}
				else if(g->getBalance()==0){
					n->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				}
				else if(g->getBalance()==1){
					n->setBalance(-1);
					c->setBalance(0);
					g->setBalance(0);
				}
				removeFix(p,ndiff);
			}
		}
		else if(bn+diff==1){
			n->setBalance(1);
		}
		else if(bn+diff==0){
			n->setBalance(0);
			removeFix(p,ndiff);
		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
