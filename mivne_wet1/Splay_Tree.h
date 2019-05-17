#ifndef MIVNE_WET1_SPLAY_TREE_H
#define MIVNE_WET1_SPLAY_TREE_H

#include <iostream>
#include <malloc.h>

/*
 * This enum is used in the Find function to determite from where the root went
 * If it went Left or Right. will help Find_aux to identify if to ouse
 * LL or LR (in case of LEFT), or RR or RL (in case of RIGHT).
 */
enum Location {RIGHT, LEFT};

/**
 * The following function is a Splay tree implementation.
 * Following functions and their O (meshuharah):
 *
 * 1) Constructors - O(1);
 * 2) Copy Constructor - O(n), n being the elements in the tree
 * 3) Delete Constructor - O(n), n being the elements in the tree.
 * 4) Get(T&  key) - O(logn), Finding an element in BST, and splaying
 * 5) getMin() - O(logn), finding the minimal element in the BST
 * 6) getMax() - O(logn), finding the maximum element in the BST
 * to the root
 * 7) isempty() - O(1), checks if there's atleast one element in the tree
 * 8) Insert(T& value) - inserts a copy of that value into the tree. O(Logn)
 * because gotta find the location it belong to and splay it up.
 * 9) Delete(T& key) - deletes an element from the tree (if it doesnt exist)
 * then splays the closest value to the top of the tree.
 * 10) Count() - O(n) goes in inorder and counts all the elements in the tree
 * 11) inOrderRL(int *numOfT, T*& data) - O(n), copies the tree information
 * into an array for future use and gives out the number of elements.
 * 12) SortedArrayToTree(T*& array, int start, int end) - O(n), given an array of
 * T and a tree of T, creates a BST tree out of it using the binary search
 * Algorimth.
 */
template <class T>
class SPlay_Tree{ //serves as a permanent head for the tree
private:
    class Node{ //Nodes themselves in the root.
    public:
        T info; //The data it holds.
        Node* left; //left child
        Node* right; //right child
        //constructors
        Node(T data) : info(data), left(NULL), right(NULL){};
        Node(const Node& node) : info(node.info){
            if(node.left != NULL){
                this->left = new Node(*(node.left));
            }
            if(node.right != NULL){
                this->right = new Node(*(node.right));
            }
        };
        //destructor destroys recursive all the node to the rest it connected.
        ~Node(){
            delete(this->right);
            this->right = NULL;
            delete(this->left);
            this->left = NULL;
        };
    };

    Node* root; //head of the tree.

    /*
     * The next functions are ZigZags that we've learnt in class, I've decided
     * to name them LL,LR,RR,RL,L,R because I kept on getting confused while
     * writing the code between Zig and Zag.
     */
    //LL
    static void LLsplay(Node* origin, Node* cur_node){
        Node* X = cur_node->left;
        Node* B = X->right;
        origin->left = X;
        X->right = cur_node;
        cur_node->left = B;
    }
    //LR
    static void LRsplay(Node* origin, Node* cur_node){
        Node* X = cur_node->right;
        Node* B = X->left;
        origin->left = X;
        X->left = cur_node;
        cur_node->right = B;
    }
    //RL
    static void RLsplay(Node* origin, Node* cur_node){
        Node* X = cur_node->left;
        Node* B = X->right;
        origin->right = X;
        X->right = cur_node;
        cur_node->left = B;
    }
    //RR
    static void RRsplay(Node* origin, Node* cur_node){
        Node* X = cur_node->right;
        Node* B = X->left;
        origin->right = X;
        X->left = cur_node;
        cur_node->right = B;
    }
    //L - used by the root of the tree.
    void Lsplay(){
        Node* org_root = this->root;
        if(org_root->left == NULL){
            return;
        }
        Node* X = org_root->left;
        Node* B = X->right;
        this->root = X;
        X->right = org_root;
        org_root->left = B;

    }
    //R - used by the root of the tree.
    void Rsplay(){
        Node* org_root = this->root;
        if(org_root->right == NULL){
            return;
        }
        Node* X = org_root->right;
        Node* B = X->left;
        this->root = X;
        X->left = org_root;
        org_root->right = B;
    }
    /*
     * A recursive function to find the wanted Node and Splay (using LL, RR,
     * LR, RL) up. O(logn) because it will call itself as many times as the
     * height of the said tree. n being the number of nodes in the tree.
     */
    static void Find_aux(T key, Node* cur_node, Node* origin, Location parent){
        if(cur_node == NULL){
            return;
        }
        if(cur_node->info == key){
            return;
        }
        T cur_info = cur_node->info;
        if(cur_info > key){ //going to the left side of the tree

            if(cur_node->left == NULL){
                return;
            }

            Find_aux(key, cur_node->left, cur_node, LEFT);

            if(parent == RIGHT){
                RLsplay(origin, cur_node);
                //RL
            }
            else if(parent == LEFT){
                LLsplay(origin, cur_node);
                //LL
            }
        }
        else if(cur_info < key){ //Going to the right side of the tree

            if(cur_node->right == NULL){
                return;
            }
            Find_aux(key, cur_node->right, cur_node, RIGHT);
            if(parent == RIGHT){
                RRsplay(origin, cur_node);
                //RR
            }
            else if(parent == LEFT){
                LRsplay(origin, cur_node);
                //LR
            }
        }
    }
    /*
     * Find function as describes in the PDF sheets of splay.pdf.
     * It checks all the needed information about the tree and if it's in need
     * of going deep (empty tree, only one node in the tree).
     * L or R the needed node.
     * O(logn) because of the recursion that it calls, while n stands for
     * the number of nodes in the tree.
     * Returns the wanted Node of the tree.
     */
    Node* Find(T key){
        if(root == NULL){ //checks for empty tree
            return NULL;
        }
        if(root->right == NULL && root->left == NULL){//checks for one node only
            return this->root;
        }
        T cur_info = root->info;

        if(cur_info == key){ //information located in the root situation
            return root;
        }
        Node* root = this->root;
        if(cur_info > key){ //Going Left in the tree
            Find_aux(key, root->left, root, LEFT);
            this->Lsplay();
        }
        else if(cur_info < key){ //Going right in the tree
            Find_aux(key, root->right, root, RIGHT);
            this->Rsplay();
        }
        return this->root;
    }
    /*
     * As described in the PDF, find the min of the tree, O(Log(n)).
     * Then splays it up using the Find function.
     */
    Node* FindMin(){
        if(root == NULL){
            return NULL;
        }
        Node* node = root;
        while(node->left != NULL){
            node = node->left;
        }
        return Find(node->info);
    }
    /*
     * As described in the PDF, find hte max of the tree, O(log(n)).
     * then splays it up using the find function.
     */
    Node* FindMax(){
        if(root == NULL){
            return NULL;
        }
        Node* node = root;
        while(node->right != NULL){
            node = node->right;
        }
        return Find(node->info);
    }
    /*
     * As described in the PDF, given two trees, it combines them into
     * the main tree (this), when calling Join all the previous tree is lost.
     * and the lesser and bigger tree are getting absorbed by that tree.
     * O(logn) because of the searching for the min or max of the trees
     * to put them back together.
     */
    void Join(SPlay_Tree& lesser, SPlay_Tree& bigger) {

        if (lesser.root == NULL) {
            bigger.FindMin();
            this->root = bigger.root;
            bigger.root = NULL;
            if (this->root != NULL) {
                this->root->left = NULL;
            }
        } else {
            lesser.FindMax();
            this->root = lesser.root;
            lesser.root = NULL;

            if (this->root != NULL) {
                this->root->right = bigger.root;
                bigger.root = NULL;
            }
        }
    }
    /*
     * As described in the pdf, takes a tree and splits it into two trees.
     * "this" tree is getting emptied as it moves all it's elements to the
     * other two trees.
     * O(1) because it takes only 4 commands to finish this function.
     */
    void Split(T pivot, SPlay_Tree& lesser, SPlay_Tree& bigger){
        this->Find(pivot);
        lesser.root = this->root;
        bigger.root = this->root->right;
        this->root->right = NULL;
        this->root = NULL; //this func turns this tree into an empty tree.
    }
    /*
     * Auxilirity function of Inorder that goes all over the tree
     * and counts how much Nodes does the tree hold.
     */
    void Count_aux(int* count, Node* cur){
        if(cur == NULL){
            return;
        }

        Count_aux(count, cur->right);
        (*count)++;
        Count_aux(count, cur->left);
    }
    /*
     * Auxilirity function of inOrderRL that puts all the nodes into an
     * Array of T.
     */
    void inOrderRL_aux(int* loc, T*& data, Node* cur_info ){
        if(cur_info == NULL){
            return;
        }
        inOrderRL_aux(loc, data, cur_info->right);
        data[*loc] = cur_info->info;
        *loc += 1;
        inOrderRL_aux(loc, data, cur_info->left);
    }

    /*
     * Takes an array and builds an entire Binary search tree out of it,
     * goes recursive and creates all the needed nodes using the binary search
     * algorimth for it.
     */
    Node* SortedArrayToTree_aux(T*& array,int start, int end){
        if (start>= end)
            return NULL;
        int mid= (start+end)/2;
        Node* node= new Node(array[mid]);
        node->right=SortedArrayToTree_aux(array,start,mid);
        node->left=SortedArrayToTree_aux(array,mid+1,end);
        return node;
    }
public:
    SPlay_Tree() : root(NULL){}; //constructor
    SPlay_Tree(const SPlay_Tree& origin){ //copy const of the tree.

        if(origin.root == NULL){
            this->root = NULL;
        }
        else{
            this->root = new Node(*(origin.root));
        }
    }
    ~SPlay_Tree(){ //destructor, sets the root to NULL after taking out all the
                //Nodes out.
        delete this->root;
        this->root = NULL;
    }
    /*
     * using the Find function to find the needed Node then gives out
     * info inside the node as a pointer, (logn)
     */
    T* Get(T& key){
        if(Find(key) == NULL){
            return NULL;
        }
        return &this->Find(key)->info;
    }
    /*
     * checks if the tree is empty
     */
    bool isempty(){
        if(this->root == NULL){
            return true;
        }
        else{
            return false;
        }
    }
    /*
     * Using findMin to give out the data in the min node.
     */
    T& getMin(){
        return this->FindMin()->info;
    }
    /*
     * Using findMax to give out the data in the max node.
     */
    T& getMax(){
        return this->FindMax()->info;
    }
    /*
     * As describes in the PDF:
     * Inserts a new element to the tree, splits the trees around that node
     * logn, makes that T value into a node and put it as a root.
     * Then goes on as joining the trees back together.
     */
    void Insert(T& value){
        if(this->root == NULL){
            this->root = new Node(value);
            return;
        }
        SPlay_Tree lesser;
        SPlay_Tree bigger;

        this->Split(value, lesser, bigger);
        this->Join(lesser, bigger);

        Node* save = this->root;
        this->root = new Node(value);
        if(value > save->info){
            this->root->left = save;
            this->root->right = save->right;
            save->right = NULL;
        }
        else{
            this->root->right = save;
            this->root->left = save->left;
            save->left = NULL;
        }

    }
    /*
     * As the pdf describes, searchs for an element in the tree and removes it
     * if it exists, the search takes O(logn) as needed
     */
    void Delete(T& key){
        this->Find(key); //splays the wanted Node or the one closest to it.

        if(key == this->root->info) { //checks if the key splayed is ours
            SPlay_Tree lesser;
            SPlay_Tree bigger;

            lesser.root = this->root->left;
            bigger.root = this->root->right;

            this->root->left = NULL;
            this->root->right = NULL;
            delete this->root;
            this->root = NULL;

            this->Join(lesser, bigger); //O(1)
        }

    }
    /*
     * Goes over the tree in an Inorder and counts how much elements
     * are in that tree, O(n).
     */
    int Count(){
        int count = 0;
        if(root == NULL){
            return 0;
        }
        Count_aux(&count,this->root->right);
        count++;
        Count_aux(&count,this->root->left);
        return count;
    }

    /*
     * Goes from biggest to lowest value and puts all the data in the nodes
     * into an array. the array is allocated inside that function and should
     * be freed outside.
     */
    void inOrderRL(int *numOfT, T*& data){
        *numOfT = this->Count();
        if(*numOfT == 0){
            return;
        }
        int loc = 0;
        data = new T[*numOfT];
        inOrderRL_aux(&loc, data, this->root->right);
        data[loc] = this->root->info;
        loc++;
        inOrderRL_aux(&loc, data, this->root->left);
    }


/*
 * Using the binary search algorimth, destroys the input tree and puts into it
 * a newly created tree from the information inside the array.
 * Since each node will be visited once, then O(n) while n being the amount
 * of T elements in the array.
 */
    void SortedArrayToTree(T*& array,int start, int end){
        delete (this->root);
        this->root = NULL;
        this->root=SortedArrayToTree_aux(array, start, end);
    }
};
#endif //MIVNE_WET1_SPLAY_TREE_H