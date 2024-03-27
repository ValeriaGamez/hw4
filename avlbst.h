#ifndef AVLBST_H
#define AVLBST_H

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
* An explicit constructor to initialize the elements by calling the base class constructor
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
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* p, int diff ); 
    void rotateRight(AVLNode<Key, Value>* n);
    void rotateLeft(AVLNode<Key, Value>* n);



};

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{

    if (p == NULL || p->getParent() == NULL){
		return;
	}

	AVLNode<Key, Value>* parent = p->getParent();
	if ( parent->getLeft() == p ) { //if p is left child of parent
        parent->updateBalance(-1); //update parent's balance to new value
		if (parent->getBalance() == 0 ) { //CASE 1
			return;
		}
		else if ( parent->getBalance() == -1 ) { //CASE 2: RECURSE
			insertFix(parent, p);
		}
		else if(parent->getBalance() == -2) { //CASE 3: Rotate->balance tree
			if ( p->getRight() == n ) { //zig zag 
                rotateLeft(p);
				rotateRight(parent);

				if(n->getBalance() == -1){ //CASE 3A
						p->setBalance(0);
						parent->setBalance(1);
				}
				else if(n->getBalance() == 0){ //CASE 3B
					p->setBalance(0);
					parent->setBalance(0);
					
				}
				else if(n->getBalance() == 1){ //CASE 3C
					p->setBalance(-1);
					parent->setBalance(0);
					
				}
                n->setBalance(0);
				return;
			}
			else { //zig-zig
                rotateRight(parent);
				p->setBalance(0);
				parent->setBalance(0);
				return;
				
			}
		}
	}
	else{ //if p is right child of parent, swap left/right, +/-
        parent->updateBalance(1); //setting new balance
		if (parent->getBalance() == 0 ) return; //CASE 1:balanced
		else if(parent->getBalance() == 1){ //CASE 2: RECURSE
			insertFix(parent, p);
		}
		else if(parent->getBalance() == 2){ //CASE 3:Rotate->balance
			if ( p->getLeft() == n ) { //zig-zag
                rotateRight(p);
				rotateLeft(parent);

				if(n->getBalance() == 1){ //CASE 3A
						p->setBalance(0);
						parent->setBalance(-1);
				}
				else if(n->getBalance() == 0){ //CASE 3B
					p->setBalance(0);
					parent->setBalance(0);
					
				}
				else if(n->getBalance() == -1){ //CASE 3C
					p->setBalance(1);
					parent->setBalance(0);
				}
                n->setBalance(0);
				return;
			}
			else { //zig-zig
                rotateLeft(parent);
				parent->setBalance(0);
				p->setBalance(0);
				return;
				
			}
		}
		
	}
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* p, int diff)
{
    if(p == NULL) return; 

    //compute next recursive call's arguments before altering tree
    AVLNode<Key, Value>* parent = p->getParent(); 

    //int balance = p->getBalance + diff; 
    
    int ndiff = 0; 
    if(parent != NULL){
        if(parent->getLeft() == p){ //if p is a left child
            ndiff = 1; 
        }
        else{ //p is a right child
            ndiff = -1; 
        }
    }

    if(p->getBalance() + diff == 2){ //CASE 1 == 2
        //cout << "-2/2" << endl;
        AVLNode<Key, Value>* rightC  = p->getRight();
        if(rightC->getBalance() == 1){
            rotateLeft(p);
            p->setBalance(0);
            rightC->setBalance(0);
            removeFix(parent, ndiff);

        }
        else if(rightC->getBalance() == 0){
            rotateLeft(p);
            p->setBalance(1);
            rightC->setBalance(-1);

        }
        else if(rightC->getBalance() == -1){
            AVLNode<Key, Value>* g = rightC->getLeft(); //grandchild of n
            rotateRight(rightC);
            rotateLeft(p);

            if(g->getBalance() == 1){
                p->setBalance(-1);
                rightC->setBalance(0);

            }
            else if(g->getBalance() == 0){
                p->setBalance(0);
                rightC->setBalance(0);

            }
            else if(g->getBalance() == -1){
                p->setBalance(0);
                rightC->setBalance(1);

            }
            g->setBalance(0);
            removeFix(parent, ndiff);

        }
    }
    else if(p->getBalance() + diff == -2){ //CASE 1 == -2
        AVLNode<Key, Value>* leftC  = p->getLeft();
        if(leftC->getBalance() == -1){ //CASE 1A: zig-zig 
            //cout << "1A" << endl;
            rotateRight(p);
            p->setBalance(0);
            leftC->setBalance(0);
            removeFix(parent, ndiff);
        }
        else if(leftC->getBalance() == 0){ //CASE 1B: zig-zig case
            //Done 
            //cout << "1B" << endl;
            rotateRight(p);
            p->setBalance(-1);
            leftC->setBalance(1); 

        }
        else if(leftC->getBalance() == 1){ //CASE 1C: zig-zag case
            //cout << "1C" << endl;
            AVLNode<Key, Value>* g = leftC->getRight(); //grandchild of n
            rotateLeft(leftC);
            rotateRight(p);
            //cout << "rotate" << endl;

            if(g->getBalance() == 1){
                p->setBalance(0);
                leftC->setBalance(-1); 

            }
            else if(g->getBalance() == 0){
                p->setBalance(0);
                leftC->setBalance(0);

            }
            else if(g->getBalance() == -1){
                p->setBalance(1);
                leftC->setBalance(0);

            }
            g->setBalance(0);
            removeFix(parent, ndiff);
        }
    }
    else if(p->getBalance() + diff == -1 || p->getBalance() + diff == 1){ //CASE 2
        //cout << "-1/1" << endl;
        //Done
        if(p->getBalance() + diff == 1){
            p->setBalance(1); 

        }
        else{
            p->setBalance(-1);
        }
        return; 
        
    }
    else if(p->getBalance() + diff == 0){ //CASE 3
        
        p->updateBalance(diff);
        removeFix(parent, ndiff); 
    }
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n)
{ 
    AVLNode<Key, Value>* left = n->getLeft();
    if(n->getParent() == NULL){ //n is the root
        this->root_ = n->getLeft(); //set root
    }
    else{
        if(n->getParent()->getRight() == n){ //if n is right child 
            n->getParent()->setRight(n->getLeft()); 
        }
        else{
            n->getParent()->setLeft(n->getLeft()); //n is left child 
        }
    }

    n->getLeft()->setParent(n->getParent()); //take left child, make it the parent
    n->setParent(n->getLeft()); //set parent of n to the left node
    if(n->getLeft()->getRight() != NULL){ 
        n->getLeft()->getRight()->setParent(n);
    }
    n->setLeft(n->getLeft()->getRight()); 
    left->setRight(n); //n is right child of old left child

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* n)
{
    AVLNode<Key, Value>* right = n->getRight();
	if ( n->getParent() == NULL ) {  //n is the root
        this->root_ = n->getRight(); //set root to be the right child of n 
	}
	else { 
        if ( n->getParent()->getLeft() == n ) { // n is a left child 
            n->getParent()->setLeft(n->getRight());
			
		}
		else {                                   // n is a right child
			n->getParent()->setRight(n->getRight());
		}
	}
	n->getRight()->setParent(n->getParent()); //take right child, make it the parent
	n->setParent(n->getRight());
	
	if ( n->getRight()->getLeft() != NULL ) { //if right child has a left child
		n->getRight()->getLeft()->setParent(n);
	}
	n->setRight(n->getRight()->getLeft());
	right->setLeft(n); //set the left of OG right node to be n
    
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->empty()){
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return; 
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);

    while(true){
        if(curr->getKey() == new_item.first){ //key is already in tree
            curr->setValue(new_item.second); //OVERWRITE
            break;
        }
        else if(new_item.first < curr->getKey()){ //go left
            if(curr->getLeft() == NULL){ //if it's empty
                AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first , new_item.second, curr);
                curr->setLeft(newNode);
                //std::cout << "LEFT" << curr->getLeft()->getItem().second << std::endl;
                if(curr->getBalance() == 0){
                    curr->setBalance(-1);
                    insertFix(curr, newNode);
                }
                else if(curr->getBalance() == 1){
                    curr->setBalance(0);
                }
                break;
            }
            curr = curr->getLeft(); //keep walking

        }
        else if(new_item.first > curr->getKey()){
            //cout << "RIGHT" << endl;
            if(curr->getRight() == NULL){ //if it's empty
                AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first , new_item.second, curr);
                curr->setRight(newNode);
                //std::cout << "LEFT" << curr->getLeft()->getItem().second << std::endl;
                if(curr->getBalance() == 0){
                    curr->setBalance(1);
                    insertFix(curr, newNode);
                }
                else if(curr->getBalance() == -1){
                    curr->setBalance(0);
                }
                
                break;
            }
            curr = curr->getRight(); //keep walking
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

    if(this->empty()){ //if tree is empty
        return;
    }

    
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    while(curr){
        if(key == curr->getKey()){
            if(curr->getLeft() == NULL && curr->getRight() == NULL){ //0 children

                if(curr == this->root_){
                    //std::cout << curr->getItem().second << std::endl;
                    curr->setBalance(0);
                    delete this->root_;
                    this->root_ = NULL; 
                    break;
                }
                
                if(curr->getParent()->getRight() == curr){ //n is right child
                    curr->getParent()->setRight(NULL);
                    removeFix(curr->getParent(), -1);
                }
                else if(curr->getParent()->getLeft() == curr){ //n is a left child
                    //cout << "left Child" << endl;
                    curr->getParent()->setLeft(NULL);
                    removeFix(curr->getParent(), 1);
                }
                //std::cout << curr->getItem().second << std::endl;
                curr->setParent(NULL);
                delete curr; 
                curr = NULL;
                break;
            }
            else if(curr->getLeft() == NULL || curr->getRight() == NULL){ // 1 children

                if(curr->getRight() == NULL){ //if the 1 child is the left 

                    if(curr->getParent() == NULL){ //if the node to be removed is at the root
                        this->root_ = curr->getLeft();
                        curr->getLeft()->setParent(NULL);
                        curr->setParent(NULL);
                        curr->setLeft(NULL);
                        delete curr; 
                        curr = NULL;
                        break;
                    }

                    if(curr->getParent()->getRight() == curr){ //if node is child to the right 
                        curr->getParent()->setRight(curr->getLeft());
                        curr->getLeft()->setParent(curr->getParent());
                        removeFix(curr->getParent(), -1);

                    }
                    else{ //node is child to the left
                        curr->getParent()->setLeft(curr->getLeft());
                        curr->getLeft()->setParent(curr->getParent());
                        removeFix(curr->getParent(), 1);

                    }
                    curr->setParent(NULL);
                    curr->setLeft(NULL);

                }
                else if(curr->getLeft() == NULL){ //if the 1 child is the right 
                    if(curr->getParent() == NULL){ //if the node to be removed is at the root
                        this->root_ = curr->getRight();
                        curr->getRight()->setParent(NULL);
                        curr->setParent(NULL);
                        curr->setLeft(NULL);
                        delete curr; 
                        curr = NULL;
                        break;
                    }
                    
                    if(curr->getParent()->getRight() == curr){ //if node is child to the right 
                        curr->getParent()->setRight(curr->getRight());
                        curr->getRight()->setParent(curr->getParent());
                        removeFix(curr->getParent(), -1);

                    }
                    else{ //node is child to the left
                        curr->getParent()->setLeft(curr->getRight());
                        curr->getRight()->setParent(curr->getParent());
                        removeFix(curr->getParent(), 1);

                    }
                    curr->setParent(NULL);
                    curr->setRight(NULL);
        
                }
                //std::cout << curr->getItem().second << std::endl;
                delete curr; 
                curr = NULL;
                break;

            }
            else{ //2 children
                //swap value with its in order successor or predecessor and 
                //remove from its new location
                nodeSwap(curr, static_cast<AVLNode<Key, Value>*>(this->predecessor(curr)));
                //std::cout << "heehe" << curr->getItem().second << std::endl;
                remove(curr->getKey());
            }    
        }
        else if(key < curr->getKey()){
            if(curr->getLeft() == NULL){
				break;
			}
				curr = curr->getLeft();

        }
        else if(key > curr->getKey()){
            if(curr->getRight() == NULL){
                break;
            }
            curr = curr->getRight();
        }
        else{ //key is not found
    
            return;
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
