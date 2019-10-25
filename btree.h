/*
 * 
 * You will need to write your B+Tree almost entirely from scratch. 
 * 
 * B+Trees are dynamically balanced tree structures that provides efficient support for insertion, deletion, equality, and range searches. 
 * The internal nodes of the tree direct the search and the leaf nodes hold the base data..
 * 
 * For a basic rundown on B+Trees, we will refer to parts of Chapter 10 of the textbook Ramikrishnan-Gehrke 
 * (all chapters and page numbers in this assignment prompt refer to the 3rd edition of the textbook).
 *
 * Read Chapter 10 which is on Tree Indexing in general. In particular, focus on Chapter 10.3 on B+Tree.
 */

#ifndef BTREE_H
#define BTREE_H

#include "data_types.h"
#include "query.h"
#include<stdbool.h>


/* 
Designing your C Structs for B+Tree nodes (Chapter 10.3.1)
How will you represent a B+Tree as a C Struct (or series of C structs that work together)? There are many valid ways to do this part of your design, and we leave it open to you to try and tune this as you progress through the project.
How will you account for a B+Tree node being an internal node or a leaf node? Will you have a single node type that can conditionally be either of the two types, or will you have two distinct struct types?
How many children does each internal node have? This is called the fanout of the B+Tree.
What is the maximum size for a leaf node? How about for an internal node?
What is the minimum threshold of content for a node, before it has to be part of a rebalancing?
*/

// TODO: here you will need to define a B+Tree node(s) struct(s)
typedef struct node{
    struct node *parent;
    int keys[3];
    int keypointer[3 + 1];
    bool isLeaf;
    int num_keys;
};

// B+ tree create
struct node*
BtreekCreate(void)
{
    struct node* s;

    s = malloc(sizeof(*s));
    assert(s);

    

    return s;
}
struct node* root;


/* The following are methods that can be invoked on B+Tree node(s).
 * Hint: You may want to review different design patterns for passing structs into C functions.
 */

/* FIND (Chapter 10.4)
This is an equality search for an entry whose key matches the target key exactly.
How many nodes need to be accessed during an equality search for a key, within the B+Tree? 
*/

// TODO: here you will need to define FIND/SEARCH related method(s) of finding key-values in your B+Tree.
//This method finds a key by traversing from the root
struct node* find(int key){
    return tree_search(root, key);
}

// This method returns a nodepointer for which a key can be inserted
struct node* node_search(struct node* root,int key){
    struct node* nodepointer;
     if(root->isLeaf == true){
        return root;
     }
     while(!root->isLeaf){
        if(key < root->keys[0]){
            nodepointer = root->keypointer[0];
            return node_search(nodepointer, key);
            }
            else if (key >= root->keys[root->num_keys-1]){
                nodepointer = root->keypointer[root->num_keys];
                return node_search(nodepointer, key);
            }
            else{
                for(int i = 0; i < root->num_keys; i++){
                    if(key<=root->keys[i]){
                            nodepointer = root->keypointer[i];
                            return node_search(nodepointer, key);
                            }
                }
            }

    }
    

}

// This method takes in  nodepointer and returns the nodepointer if the key is in the nodepointer
struct node* tree_search(struct node* search, int keyvalue){
    struct node *nodepointer;

// This checks if a node is a leaf
    if(search->isLeaf == true){
        for(int i = 0; i < search->num_keys; i++ ){
            // This checks if the key is available in the array of keys
            if(keyvalue ==search->keys[i]){
                nodepointer = search->keypointer[i];
                return nodepointer;
                            }
            else{
                return 0;
            }
                }


    }
    while(!search->isLeaf){
        // /This checks if the key value is less than the first key value in key array
            if(keyvalue < search->keys[0]){
                nodepointer = search->keypointer[0];
                return tree_search(nodepointer, keyvalue);
            }
            
        // /This checks if the key value is greater than the first key value in key array
            else if (keyvalue >= search->keys[search->num_keys - 1]){
                 nodepointer = search->keypointer[search->num_keys];
                 return tree_search(nodepointer, keyvalue);
            }
            else{
                for(int i = 0; i < search->num_keys; i++ ){
                    if(keyvalue<= search->keys[i]){
                            nodepointer = search->keypointer[i];
                            return tree_search(nodepointer, keyvalue);
                            }
                }
            }
    }


}


/* INSERT (Chapter 10.5)
How does inserting an entry into the tree differ from finding an entry in the tree?
When you insert a key-value pair into the tree, what happens if there is no space in the leaf node? What is the overflow handling algorithm?
For Splitting B+Tree Nodes (Chapter 10.8.3)
*/

// TODO: here you will need to define INSERT related method(s) of adding key-values in your B+Tree.
// This method is for inserting a key
void insertLeaf(int key, struct node* root){
    if(root->isLeaf == true){
        // Checks whether node is full or not
        if(root->num_keys <= 3){
            for(int i = 0; i < root->num_keys - 1; i++){
                if(key < root->keys[i]){
                    for(int i = 0; root->num_keys - 1 - i; i++){
                        root->keys[i+1] = root->keys[i];
                    }
                    root->keys[i] = key;
                }
                else if (i == root->num_keys -1){
                    root->keys[i] = key;
                }
            }
            root->num_keys+=1;
        }
        
    }
    else{
        // Searches for available node you can insert a key
        struct node* leaf = node_search(root, key);
        // Checks if the leaf node is full
        if(leaf->num_keys <= 3){
            for(int i = 0; i < leaf->num_keys - 1; i++){
                if(key < root->keys[i]){
                    for(int i = 0;i < leaf->num_keys - 1 - i; i++){
                       leaf->keys[i+1] = leaf->keys[i];
                    }
                   leaf->keys[i] = key;
                }
                else if (i == leaf->num_keys -1){
                    leaf->keys[i] = key;
                }
            }
            leaf->num_keys+=1;
        }
        // splits the node if it is full
        else{
            struct node* splitnode;
            splitnode->num_keys = 0;
            splitnode->isLeaf = true;
            splitnode->parent = leaf->parent;
            int splitter = 4/2;
            for(int i = 0; i < 4; i++){
                if(key < leaf->keys[i] && i < splitter){
                    for(int x = 0; i < splitter; x++){
                        leaf->keys[x+1] = leaf->keys[x];
                    }
                    leaf->keys[i] = key;


                }
                splitnode->keys[0] = leaf->keys[splitter - 1];
                leaf->num_keys= splitter;
                if(key < leaf->keys[i] && i >= splitter){
                    for(int x = 0; i < splitter; x++){
                        splitnode->keys[i+1] = splitnode->keys[i];
                    }
                    splitnode->num_keys = splitter;
                    splitnode->keys[i] = key;}
                else{
                splitnode->keys[splitter - 1] = key;

                // }
            }
            return balancetree(splitnode, leaf->parent, leaf);
        }

    }

}

// balances tree and connects child node to parent node
void balancetree(struct node* split, struct node* parent, struct node* Child){
    int key = split->keys[0];
    struct node* pnode = parent;
    if(pnode == 0){
        pnode->keys[3];
        pnode->keypointer[3 + 1];
        pnode->isLeaf = false;
        pnode->keys[0]= key;
        pnode->keypointer[0] = Child;
        pnode->keypointer[1] = split;
        pnode->num_keys = 1;

    }
    else{
        for(int i =0; i < pnode->num_keys; i++){
            if(key < pnode->keys[i]){
                for(int i = 0; i < pnode->num_keys - i; i++){
                    pnode->keys[i+1] = pnode->keys[i];
                }
                for(int i = 0; i < pnode->num_keys - i + 1; i++){
                pnode->keypointer[i+2] = pnode->keypointer[i+1];
                    }
                    pnode->keys[i] = key;
                    pnode->keypointer[i+1] = pnode;
                    break;
            }
            else{
                    pnode->keys[pnode->num_keys - 1] = key;
                    pnode->keypointer[pnode->num_keys] = pnode;

            }

        }
    }
}


/* BULK LOAD (Chapter 10.8.2)
Bulk Load is a special operation to build a B+Tree from scratch, from the bottom up, when beginning with an already known dataset.
Why might you use Bulk Load instead of a series of inserts for populating a B+Tree? Compare the cost of a Bulk Load of N data entries versus that of an insertion of N data entries? What are the tradeoffs?
*/

// TODO: here you will need to define BULK LOAD related method(s) of initially adding all at once some key-values to your B+Tree.
// BULK LOAD only can happen at the start of a workload


/*RANGE (GRADUATE CREDIT)
Scans are range searches for entries whose keys fall between a low key and high key.
Consider how many nodes need to be accessed during a range search for keys, within the B+Tree?
Can you describe two different methods to return the qualifying keys for a range search? 
(Hint: how does the algorithm of a range search compare to an equality search? What are their similarities, what is different?)
Can you describe a generic cost expression for Scan, measured in number of random accesses, with respect to the depth of the tree?
*/

// TODO GRADUATE: here you will need to define RANGE for finding qualifying keys and values that fall in a key range.



#endif