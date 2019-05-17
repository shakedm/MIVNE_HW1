#ifndef MIVNE_WET1_ARENA_H
#define MIVNE_WET1_ARENA_H

#include "LinkedList.h"
#include "Splay_Tree.h"
#include "library1.h"



class Arena {
public:
    //Declaretion of classes forward.
    class LvlNode;
    class Trainer;
    class Gladiator;
    class IDNode;

    /*
     * LvlNode includes a pointer to a Node of a gladiator in our custom made linked list.
     */
    class LvlNode{
    public:
        LinkedList<Gladiator>::Node* gladiator;
    public:
        LvlNode();
        LvlNode(LinkedList<Gladiator>::Node* gladiator);
        friend bool operator==(const LvlNode& l, const LvlNode& r);
        friend bool operator<(const LvlNode& l, const LvlNode& r);
        friend bool operator>(const LvlNode& l , const LvlNode& r);
    };

    /*
     * Trainer holds his own unique ID, a tree of Lvlnodes and holds the top_gladiator LvlNode to the best gladiator
     * in the trainer's list (by Level).
     */
    class Trainer{
    private:
        friend class Arena;
        int TrainerID;
        SPlay_Tree<LvlNode> gladiators;
        Arena::LvlNode Top_Gladiator;
    public:
        Trainer(int ID);
        SPlay_Tree<LvlNode>* trainerTree();
        void ChangeTop(LvlNode& new_top);
        void Insert(LvlNode& glad_pointer); //inserts a new pointer to the tree.
        friend bool operator==(const Trainer& l,const Trainer& r);
        friend bool operator<(const Trainer& l, const Trainer& r);
        friend bool operator>(const Trainer& l, const Trainer& r);
        friend bool operator!=(const Trainer& l, const Trainer& r);
    };

    /*
     * Gladiator class that holds his unique ID, the ID of the trainer and it's level, all that is stored in a
     * Linkedlist of hte system.
     */
    class Gladiator{
    public:
        friend class Arena;
        int GladiatorID;
        int Level;
        int TrainerID;
    public:
        Gladiator(int ID, int Level, int TrainerID);
        void UpdateID(int newID);
        void UpdateLvl(int newLvl);
        void MultiplyLvl(int factor);
        int GetID() const;
        int GetLvl() const;

        friend bool operator==(const Gladiator& l, const Gladiator& r);
        friend bool operator<(const Gladiator& l, const Gladiator& r);
        friend bool operator>(const Gladiator& l, const Gladiator& r);
        friend bool operator!=(const Gladiator& l, const Gladiator& r);
    };
    /*
     * Stores a pointer to a said gladiator Node and a pointer to the gladiator's trainer.
     */
    class IDNode{
    public:
        LinkedList<Gladiator>::Node* gladiator;
        Trainer* trainer;
    public:
        IDNode(LinkedList<Gladiator>::Node* gladiator,
                      Trainer* trainer);

        friend bool operator==(const IDNode& l,const IDNode& r);
        friend bool operator<(const IDNode& l, const IDNode& r);
        friend bool operator>(const IDNode& l, const IDNode& r);
        friend bool operator!=(const IDNode& l, const IDNode& r);

    };

private:
    LinkedList<Trainer> trainers;
    LinkedList<Gladiator> glad_list;
    SPlay_Tree<IDNode> glad_ID_tree;
    SPlay_Tree<LvlNode> glad_Lvl_tree;

    LvlNode Top_Gladiator;

    /*
     * Goes over the IDNode tree, O(logn), n is number of gladiators in the system.
     * returns true if yes, otherwise false.
     */
    bool GladExist(int ID);
    /*
     * Goes over the trainer linkedlist and check if the said trainer exist O(k), k is num of trainers.
     * returns true if yes, otherwise false.
     */
    bool TrainerExist(int ID);
    /*
     * Goes over the IDNode tree and gets out the Node that talks about specific gladiator (that stored in IDNode)
     * O(logn), n being the number of gladiators
     */
    IDNode* getIDNode(int gladiatorID);
    /*
     * compares a current LvlNode with the saved LvlNode and changes it if needed O(1).
     */
    void ChangeTop(LvlNode& new_top);
    /*
     * Inserts a gladiator into the system by the following algorimth
     * 1) checks if trainer and gladiator exist, using exist function O(k+logn) (if yes, stops).
     * 2) Insert gladiator into the linkedlist. O(1) (because puts it in the front of the linkedlist)
     * 3) Use the Node that the insert gladiator (into a list) return and uses it to create appropriate
     * LvlNode and IDNode O(1) for LvlNode and O(k) for IDNode, because we need to get the trainer's pointer.
     * 4) Insert into the IDNode and LvlNode, each O(logn), as the SPlay tree works as O(Logn) "meshuharah"
     * 5) update top gladiators if needed.
     */
    void InsertGladiator(int gladiatorID, int level, int trainerID);
    /*
     * Deletes a gladiator from the system by the following algorimth
     * 1) find the gladiator in the IDNode tree (logn)
     * 2) Extract the trainer's pointer from the IDNode and use it to find LvlNode, (logn)
     * 3) extract the Gladiator Node from the IDNode. O(1)
     * 4) Delete LvlNode, IDNode, Gladiator. O(1)
     * 5) Update the top gladiator O(logn)
     */
    void DeleteGladiator(int gladiatorID);
    /*
     * LevelUp_aux
     * 1) finds the GladiatorNode in the IDNode tree (logn), n number of glads.
     * 2) extract the trainer, and the it's personal tree O(1) from the IDNode.
     * 3) Find the LvlNode and takes it out from the tree (both trainer's and overall big tree). O(logn)
     * 4) upgrade the gladiator Lvl through IDNode O(1)
     * 5) put the LvlNode back into the tree (Both trainers and overall big tree). O(logn)
     * 6) update the top gladiator in the trainer and overall big LvlNode tree). O(logn)
     */
    void LevelUp_aux(int gladiatorID, int levelIncrease);
    /*
     * Upgrade
     * 1) find a gladiator in IDNode O(logn)
     * 2) delete the IDNode out of the tree. O(logn)
     * 3) update the gladiator's ID in the IDNode
     * 4) put the gladiator back into the tree. O(logn)
     */
    void Upgrade(int gladiatorID, int upgradedID);
    /*
     * Depends on the input, if -1 then:
     * 1) returns the gladiator's ID from the top gladiator in the big tree O(1)
     * if the value is positive
     * 1) searchs for a trainer and returns the top gladiator ID from him. O(k)
     */
    int TopGladID(int trainerID);
    /*
     * This function takes a LvlNode tree and then:
     * 1) extracts the tree's information into an array O(n), and count it O(n)
     * using the following functions from the SPlayTree: Count, InOrderRL.
     * 2) extract LvlNode array's ID information into the given int array O(n)
     */
    StatusType GladiatorsByLevel_aux(int **gladiators, int *numOfGladiator,
                                            SPlay_Tree<LvlNode>& tree);

public:
    /*
     * Adds trainer as the pdf tells by adding a new trainer into the linked list O(k0
     */
    StatusType AddTrainer(int trainerID);
    /*
     * Checks if the gladiator doesnt exist and trainer exist O(logn+k), if that holds true then calls
     * the function "InsertGladiator" above, otherwise returns FAILURE. O(logn+k) overall.
     */
    StatusType BuyGladiator(int gladiatorID, int trainerID, int level);
    /*
     * Checks if the gladiator exists, if he does then calls the above function "DeleteGladiator"
     * O(logn), otherwise return FAILURE
     */
    StatusType FreeGladiator(int gladiatorID);
    /*
     * Checks if gladiator exists, if not then return FAILURE. otherwise calls the function above
     * "LevelUp_aux" that runs on O(logn)
     */
    StatusType LevelUp(int gladiatorID, int levelIncrease);
    /*
     * Checks if gladiator exists and if UpgradedID as a gladiator doesn't exist (logn)
     * Then uses the aux function "Upgrade" that runs by O(logn) that upgrades the said gladiator.
     */
    StatusType UpgradeGladiator(int gladiatorID, int upgradedID);
    /*
     * Get top gladiator, checks if trainer exists O(k), if he doesn't then return FAILURE. otherwise
     * uses the function TopGladID that runs on O(n) to return the said gladiator.
     * (Depending on the gladiatorID input, if -1, give the gladiator out of the big tree, otherwise out of
     * the trainer's tree).
     */
    StatusType GetTopGladiator(int trainerID, int *gladiatorID);

    /*
     * Checks if said trainer exists O(k), if trainerID is -1 then calls the function
     * GetAllGladiatorsByLevel using the main LvlNode Tree, otherwise find the trainer and use it's private
     * LvlNode Tree.
     * There it's O(n+k)
     */
    StatusType GetAllGladiatorsByLevel(int trainerID, int **gladiators, int *numOfGladiator);
    /*
     * UpdateLevels goes over all the trainers, and the main tree and updates, O(n+k), updates them using the following
     * Algorimth:
     * 1) Extract the LvlNode Gladiators from the main LvlNode tree O(n) to an array
     * 2) Go over array and mark which gladiators are going to get effected. O(n)
     * 3) Split them into two arrays, gladiators that are going to get drugged and once that not, O(n)
     * 4) go over all the trainers and do this for each trainer: O(k)
     * 5) extract each of their gladiator and seperate them to two arrays like the main tree's (nglads,list);
     * 6) multiply the drugged array by the factor
     * 7) put them back into a tree using a function array to BST (implemented into the Splay_Tree and is O(ntrainers);
     * 8) once finish going over all the gladiators, get back into the main LvlNode tree
     * 9) put the main tree back, O(n)
     * Because By going through all the trainers require O(k), and because the max operations that we will do on all
     * the trainers together is on O(n), therefor the complexcity is O(n+k).
     */
    StatusType UpdateLevels(int stimulateCode, int stimulantFactor);
};
#endif //MIVNE_WET1_ARENA_H
