#include <malloc.h>
#include "Arena.h"

/*
 * Trainer's functions
 */
Arena::Trainer::Trainer(int ID) : TrainerID(ID), gladiators(){
    this->Top_Gladiator.gladiator = NULL;
}

SPlay_Tree<Arena::LvlNode>* Arena::Trainer::trainerTree() {
    return &this->gladiators;
}

void Arena::Trainer::ChangeTop(LvlNode& new_top){
    if(this->trainerTree()->isempty()){
        this->Top_Gladiator.gladiator = NULL;
        return;
    }
    if(this->Top_Gladiator.gladiator == NULL){
        this->Top_Gladiator = new_top;
    }
    else if(this->Top_Gladiator < new_top) {
        this->Top_Gladiator = new_top;
    }
}

void Arena::Trainer::Insert(LvlNode& glad_pointer){
    this->gladiators.Insert(glad_pointer);
}

bool operator==(const Arena::Trainer& l,const Arena::Trainer& r) {
    return l.TrainerID == r.TrainerID;
}
bool operator<(const Arena::Trainer& l, const Arena::Trainer& r){
    return  l.TrainerID < r.TrainerID;
}
bool operator>(const Arena::Trainer& l, const Arena::Trainer& r){
    return !(l<r);
}
bool operator!=(const Arena::Trainer& l, const Arena::Trainer& r){
    return !(l==r);
}

//Gladiator Functions:

Arena::Gladiator::Gladiator(int ID, int Level, int TrainerID)
        : GladiatorID(ID), Level(Level), TrainerID(TrainerID){}

void Arena::Gladiator::UpdateID(int newID){
    this->GladiatorID = newID;
}
void Arena::Gladiator::UpdateLvl(int newLvl){
    this->Level = newLvl;
}
void Arena::Gladiator::MultiplyLvl(int factor){
    this->Level = (this->Level * factor);
}
int Arena::Gladiator::GetID() const{
    return GladiatorID;
}
int Arena::Gladiator::GetLvl() const{
    return Level;
}

bool operator==(const Arena::Gladiator& l, const Arena::Gladiator& r){
    return l.GetID() == r.GetID();
}
bool operator<(const Arena::Gladiator& l, const Arena::Gladiator& r){
    return l.GetID() < r.GetID();
}
bool operator>(const Arena::Gladiator& l, const Arena::Gladiator& r){
    return !(l<r);
}
bool operator!=(const Arena::Gladiator& l, const Arena::Gladiator& r){
    return !(l==r);
}

//IDNode functions:

Arena::IDNode::IDNode(LinkedList<Gladiator>::Node* gladiator,
                                    Trainer* trainer)
        : gladiator(gladiator), trainer(trainer){}

bool operator==(const Arena::IDNode& l,const Arena::IDNode& r){
    return l.gladiator->getData() == r.gladiator->getData();
}
bool operator<(const Arena::IDNode& l, const Arena::IDNode& r){
    return l.gladiator->getData() < r.gladiator->getData();
}
bool operator>(const Arena::IDNode& l, const Arena::IDNode& r){
    return !(l<r);
}
bool operator!=(const Arena::IDNode& l, const Arena::IDNode& r){
    return !(l==r);
}

//LvlNode funtions

Arena::LvlNode::LvlNode() : gladiator(NULL){}

Arena::LvlNode::LvlNode(LinkedList<Gladiator>::Node* gladiator)
        : gladiator(gladiator){}

bool operator==(const Arena::LvlNode& l, const Arena::LvlNode& r){
    return ((l.gladiator->getData().GetID() == r.gladiator->getData().GetID()) &&
            (l.gladiator->getData().GetLvl() == r.gladiator->getData().GetLvl()));
}
bool operator<(const Arena::LvlNode& l, const Arena::LvlNode& r){
    if(l.gladiator->getData().GetLvl() == r.gladiator->getData().GetLvl()){
        return ((l.gladiator->getData().GetID() > r.gladiator->getData().GetID()));
    }
    else{
        return l.gladiator->getData().GetLvl() < r.gladiator->getData().GetLvl();
    }
}
bool operator>(const Arena::LvlNode& l , const Arena::LvlNode& r){
    return !(l<r);
}

//Arena's Static functions:
/*
 * Typical merge that takes two arrays and put them into one array, O(n1+n2).
 */
static void merge(Arena::LvlNode*& arr1,int n1, Arena::LvlNode *&arr2,int n2, Arena::LvlNode*& arr3){
    int i=0, k=0, j=0;
    while(i<n1 && j<n2){
        if(arr1[i]>arr2[j]){
            arr3[k++]=arr1[i++];
        } else{
            arr3[k++]=arr2[j++];
        }
    }
    while (i<n1){
        arr3[k++]=arr1[i++];
    }
    while (j<n2){
        arr3[k++]=arr2[j++];
    }
}
/*
 * Goes over an array and multiply if a condition is met, O(num);
 */
static void Multiply_array(Arena::LvlNode*& array, int num, int factor){
    for(int i = 0; i < num; i++){
        array[i].gladiator->getData().MultiplyLvl(factor);
    }
}
/*
 * Recieves an array at size of Num
 * Splits it to two arrays under a certain condition O(n)
 */
static void Emperor_Dealer_AUX(int Num, Arena::LvlNode*& array ,Arena::LvlNode*& Drugged
        ,Arena::LvlNode*& NoToDrugs ,int* NUMDrugged,int* NumNoDrugged
        ,int StimulateCode) {
    if(Num == 0){
        return;
    }
    int* changed= new int[Num];
    int count = 0;
    for (int i = 0; i < Num; ++i) {
        if (array[i].gladiator->getData().GetID() % StimulateCode == 0) {
            changed[i] = 1;
            count++;
        } else{
            changed[i]=0;
        }
    }
    Drugged = new Arena::LvlNode[count];
    NoToDrugs = new Arena::LvlNode[Num-count];
    *NUMDrugged = 0;
    *NumNoDrugged = 0;

    for (int j = 0; j < Num; ++j) {
        if (changed[j] == 1) {
            Drugged[*NUMDrugged].gladiator = array[j].gladiator;
            (*NUMDrugged)++;
        } else {
            NoToDrugs[*NumNoDrugged].gladiator = array[j].gladiator;
            (*NumNoDrugged)++;
        }
    }
    delete[] (changed);
}
/*
 * Recieves a tree, then
 * 1) splits it into two arrays under a condition O(n) n being the size of the array
 * 2) Multiply the array O(n)
 * 3) merge the two arrays back in. O(n)
 * 4) creates a new BST tree out of the given array O(n).
 * Total of O(n)
 */
static void StimulateTree(SPlay_Tree<Arena::LvlNode>& tree,
                          int StimulateCode, int StimulateFactor){
    Arena::LvlNode *cur_array = NULL;
    int Num = 0;

    tree.inOrderRL(&Num, cur_array);
    if(Num == 0){
        return;
    }

    Arena::LvlNode *Drugged;
    Arena::LvlNode *NoDrugged;

    int NumDrugged = 0;
    int NumNoDrugged = 0;

    Emperor_Dealer_AUX(Num, cur_array, Drugged, NoDrugged, &NumDrugged,
                       &NumNoDrugged, StimulateCode);

    Multiply_array(Drugged, NumDrugged, StimulateFactor); //multiply the given array

    merge(Drugged, NumDrugged, NoDrugged, NumNoDrugged, cur_array); //merges the two arrrays

    tree.SortedArrayToTree(cur_array, 0, Num); //creates new tree

    delete[] (NoDrugged);
    delete[] (Drugged);
    delete[] (cur_array);

}

/*
 * Takes an array of LvlNode and turns it into an array of int of gladiator IDS.
 * takes total of O(n) for that operator as we go element, element in the array.
 */
static int* LvlNodeToId(int* numOfGladiators,
                        Arena::LvlNode*& info){


    int num = *numOfGladiators;
    int* array = (int*)(malloc(sizeof(*array)*(num)));

    for(int i = 0; i < num; i++){

        array[i] = info[i].gladiator->getData().GetID();

    }

    return array;
}
//Arena's Aux functions:

bool Arena::GladExist(int ID){

    Gladiator glad(ID, 0, 0);

    LinkedList<Gladiator>::Node to_find(glad);

    IDNode pointer(&to_find, NULL);

        IDNode* exists= glad_ID_tree.Get(pointer);
    if(!exists){
        return false;
    }


    if(ID == exists->gladiator->data.GladiatorID){
        return true;
    }
    return false;
}

bool Arena::TrainerExist(int ID){
    Trainer to_find(ID);
    if(trainers.Find(to_find)){
        return true;
    }
    return false;
}

Arena::IDNode* Arena::getIDNode(int gladiatorID){
    Gladiator glad(gladiatorID, 0, 0);

    LinkedList<Gladiator>::Node to_find(glad);

    IDNode pointer(&to_find, NULL);

    return this->glad_ID_tree.Get(pointer);
}

void Arena::ChangeTop(LvlNode& new_top){
    if(this->glad_Lvl_tree.isempty()){
        this->Top_Gladiator.gladiator = NULL;
        return;
    }
    if(this->Top_Gladiator.gladiator == NULL){
        this->Top_Gladiator = new_top;
    }
    else if(this->Top_Gladiator < new_top) {
        this->Top_Gladiator = new_top;
    }
}

void Arena::InsertGladiator(int gladiatorID, int level, int trainerID) {

    Gladiator new_glad(gladiatorID, level, trainerID);

    LinkedList<Gladiator>::Node *glad_pointer
            = this->glad_list.Insert(new_glad);

    Trainer trainer_to_find = Trainer(trainerID);

    Trainer* trainer = this->trainers.Get(trainer_to_find);

    LvlNode Lvl_Node_Input(glad_pointer);

    trainer->Insert(Lvl_Node_Input);

    IDNode new_ID_pointer(glad_pointer, trainer);

    glad_ID_tree.Insert(new_ID_pointer);

    this->glad_Lvl_tree.Insert(Lvl_Node_Input);

    trainer->ChangeTop(Lvl_Node_Input);

    this->ChangeTop(Lvl_Node_Input);
}

void Arena::DeleteGladiator(int gladiatorID) {

    IDNode* ID_pointer = this->getIDNode(gladiatorID);

    LvlNode Lvl_pointer(ID_pointer->gladiator);

    Trainer trainer_to_find(Lvl_pointer.gladiator->getData().TrainerID);

    Trainer* trainer = this->trainers.Get(trainer_to_find);

    if(Lvl_pointer == this->Top_Gladiator){
        this->Top_Gladiator.gladiator = NULL;
    }
    if(trainer != NULL) {

        if (Lvl_pointer == trainer->Top_Gladiator) {
            trainer->Top_Gladiator.gladiator = NULL;
        }
    }

    this->glad_Lvl_tree.Delete(Lvl_pointer);

    ID_pointer->trainer->trainerTree()->Delete(Lvl_pointer); //delete from trainer tree

    IDNode backup(*ID_pointer);

    this->glad_ID_tree.Delete(*ID_pointer);

    this->glad_list.Delete(backup.gladiator);

    if(trainer != NULL) {
        trainer->ChangeTop(trainer->trainerTree()->getMax());
    }

    this->ChangeTop(this->glad_Lvl_tree.getMax());
}

void Arena::LevelUp_aux(int gladiatorID, int levelIncrease) {

    IDNode* ID_pointer = this->getIDNode(gladiatorID);

    LvlNode Lvl_pointer(ID_pointer->gladiator);

    ID_pointer->trainer->trainerTree()->Delete(Lvl_pointer);

    this->glad_Lvl_tree.Delete(Lvl_pointer);

    int prev = ID_pointer->gladiator->getData().GetLvl();

    ID_pointer->gladiator->getData().UpdateLvl(prev+levelIncrease);

    ID_pointer->trainer->trainerTree()->Insert(Lvl_pointer);

    this->glad_Lvl_tree.Insert(Lvl_pointer);

    this->ChangeTop(Lvl_pointer);

}

void Arena::Upgrade(int gladiatorID, int upgradedID){

    IDNode* ID_pointer = this->getIDNode(gladiatorID);

    IDNode Input(*ID_pointer);

    this->glad_ID_tree.Delete(*ID_pointer);

    Input.gladiator->getData().UpdateID(upgradedID);
    this->glad_ID_tree.Insert(Input);
}

int Arena::TopGladID(int trainerID){
    Trainer to_find(trainerID);
    Trainer* trainer = this->trainers.Get(to_find);
    if(trainer->trainerTree()->isempty()){
        return -1;
    }
    return trainer->Top_Gladiator.gladiator->getData().GetID();
}



//Arena's functions, as explained in the h file

StatusType Arena::AddTrainer(int trainerID){
    if(TrainerExist(trainerID)){
        return FAILURE;
    }
    Trainer new_trainer(trainerID);
    this->trainers.Insert(new_trainer);
    return SUCCESS;
}

StatusType Arena::BuyGladiator(int gladiatorID, int trainerID, int level){
    if(GladExist(gladiatorID) || !TrainerExist(trainerID)){
        return FAILURE;
    }

    InsertGladiator(gladiatorID, level, trainerID);
    return SUCCESS;
}

StatusType Arena::FreeGladiator(int gladiatorID){
    if(!GladExist(gladiatorID)){
        return FAILURE;
    }

    DeleteGladiator(gladiatorID);
    return SUCCESS;
}

StatusType Arena::LevelUp(int gladiatorID, int levelIncrease){
    if(!GladExist(gladiatorID)){
        return FAILURE;
    }

    LevelUp_aux(gladiatorID, levelIncrease);
    return SUCCESS;

}

StatusType Arena::UpgradeGladiator(int gladiatorID, int upgradedID){
    if(!GladExist(gladiatorID)) {
        return FAILURE;
    }
    if(GladExist(upgradedID)){
        return FAILURE;
    }
    Upgrade(gladiatorID, upgradedID);
    return SUCCESS;
}

StatusType Arena::GetTopGladiator(int trainerID, int *gladiatorID){
    if(trainerID < 0){
        if(this->Top_Gladiator.gladiator == NULL){
            *gladiatorID = -1;
        }
        else{
            *gladiatorID = this->Top_Gladiator.gladiator->getData().GetID();
        }
    }
    else{
        if(TrainerExist(trainerID)){
            *gladiatorID = TopGladID(trainerID);
        }
        else{
            return FAILURE;
        }
    }
    return SUCCESS;
}

StatusType Arena::GladiatorsByLevel_aux(int **gladiators, int *numOfGladiator,
                                        SPlay_Tree<LvlNode>& tree){

    LvlNode* info_array = NULL;
    tree.inOrderRL(numOfGladiator, info_array);

    if(*numOfGladiator == 0){
        *gladiators = NULL;
        return SUCCESS;
    }

    *gladiators = NULL;
    *gladiators = LvlNodeToId(numOfGladiator, info_array);
    delete[] (info_array);
    return SUCCESS;
}

StatusType Arena::GetAllGladiatorsByLevel(int trainerID, int **gladiators, int *numOfGladiator){


    if(trainerID < 0){
        return GladiatorsByLevel_aux(gladiators, numOfGladiator,
                                        this->glad_Lvl_tree);
    }
    else{
        Trainer to_find(trainerID);
        Trainer* trainer = this->trainers.Get(to_find);
        if(!trainer){
            return FAILURE;
        }
        return GladiatorsByLevel_aux(gladiators, numOfGladiator,
                                     *trainer->trainerTree());
    }

}

StatusType Arena::UpdateLevels(int StimulateCode, int stimulantFactor) {

    if(this->glad_Lvl_tree.isempty()){
        return SUCCESS;
    }

    LvlNode *big_tree_array = NULL;
    int Num = 0;
    this->glad_Lvl_tree.inOrderRL(&Num, big_tree_array);
    LvlNode* Drugged;
    LvlNode* NoDrugged;
    int NumDrugged = 0;
    int NumNoDrugged = 0;
    Emperor_Dealer_AUX(Num, big_tree_array, Drugged, NoDrugged, &NumDrugged,
                        &NumNoDrugged, StimulateCode);

    Trainer* trainer = this->trainers.IteratorStart();
    if(trainer != NULL){
        StimulateTree((*trainer->trainerTree()), StimulateCode, stimulantFactor);
        trainer->ChangeTop(trainer->trainerTree()->getMax());
        while(this->trainers.IsIteratorNextValid()){
            trainer = this->trainers.GetIterator();
            StimulateTree((*trainer->trainerTree()), StimulateCode, stimulantFactor);
            trainer->ChangeTop(trainer->trainerTree()->getMax());
        }
    }
    merge(Drugged, NumDrugged, NoDrugged, NumNoDrugged, big_tree_array);

    this->glad_Lvl_tree.SortedArrayToTree(big_tree_array, 0, Num);

    delete[] (NoDrugged);
    delete[] (Drugged);
    delete[] (big_tree_array);

    this->ChangeTop(this->glad_Lvl_tree.getMax());

    return SUCCESS;
}


