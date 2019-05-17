#ifndef MIVNE_WET1_LINKEDLIST_H
#define MIVNE_WET1_LINKEDLIST_H
#include <malloc.h>

template <class T>
class LinkedList{
public:
    class Node{
    public:
        T data;
        Node* prev;
        Node* next;

        Node(T data) : data(data), prev(NULL), next(NULL){}
        ~Node(){
            delete next;
        };
        T& getData(){
            return this->data;
        }
    };
    //Constructor, defines every pointer to NULL
    LinkedList() : root(NULL), Iterator(NULL){};
    //Destructor will work recursive from the root to the rest. O(k)
    //k being the number of elements in the linkedlist.
    ~LinkedList(){
        delete root;
    }
    //Inserts data as the first Node in the list, therefor O(1).
    Node* Insert(T data){
        Node* node = new Node(data);
        if(this->root == NULL){
            this->root = node;
            node->next = NULL;
            node->prev = NULL;
        }
        else if(this->root != NULL){
            Node* next_node = this->root;
            this->root = node;
            node->next = next_node;
            next_node->prev = node;
        }
        return node;
    };
    //Given a node, deletes that node from the tree. O(1), because we are
    //given the actual root.
    void Delete(Node* node){
        if(this->root == node){
            this->root = node->next;
        }
        else{
            node->prev->next = node->next;
        }
        node->next = NULL;
        delete node;
    }
    //Iterates through all the tree and checks if a certain data exists O(k)
    //k being the number of nodes in the tree.
    bool Find(T data){
        Node* node = this->root;
        while(node){
            if(node->getData() == data){
                return true;
            };
            node = node->next;
        }
        return false;
    }
    //Iterates through all the tree and searchs for data, gives it out if it
    //exists as a pointer. NULL if data doesn't exist. O(k)
    //while k being the number of nodes in the list.
    T* Get(T data){
        Node* node = this->root;
        while(node){
            if(node->getData() == data){
                return &node->getData();
            }
            node = node->next;
        }
        return NULL;
    }
    //Simple Iterator, sets the iterator to the first node, and gives out
    //the pointer of that node, O(1). if there is no node, returns NULL.
    T* IteratorStart(){
        this->Iterator = this->root;
        if(this->root == NULL){
            return NULL;
        }
        return &Iterator->data;
    }
    //Moves to the next Node (if it doesn't exist then NULL). O(1)
    T* IsIteratorNextValid(){
        if(Iterator == NULL || Iterator->next == NULL){
            return NULL;
        }
        else{
            T* to_return = &this->Iterator->next->data;
            Iterator = Iterator->next;
            return to_return;
        }
    }

    //Gets the current Node that the iterator points at O(1).
    T* GetIterator(){
        return &this->Iterator->data;
    }
private:
    Node* root; //The first element of the tree
    Node* Iterator;
};

#endif //MIVNE_WET1_LINKEDLIST_H
