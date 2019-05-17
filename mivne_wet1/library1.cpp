#include "library1.h"
#include "Arena.h"


/**
 * Each one of the following functions checks for the Proper input in term
 * Of NULL pointers, or wrong int input and returns INVALID_INPUT.
 * The function that it calls works with proper values but returns FAILURE in
 * case something goes work (such as trainer exist in AddTrainer) or Allocation
 * Error in case the malloc or free failed.
 */

void * Init(){
    Arena* DS = new Arena;
    return (void*)DS;
}

StatusType AddTrainer(void *DS, int trainerID){
        if (!DS || trainerID <= 0) {
            return INVALID_INPUT;
        }
    Arena* arena = (Arena*)(DS);
    try {
        if(arena->AddTrainer(trainerID) == FAILURE){
            return FAILURE;
        };
    }
    catch(const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType BuyGladiator(void *DS, int gladiatorID, int trainerID, int level){
    if(!DS || gladiatorID <= 0 || trainerID<=0 || level<=0){
        return INVALID_INPUT;
    }
    Arena* arena = (Arena*)(DS);
    try{
        if(arena->BuyGladiator(gladiatorID, trainerID, level) == FAILURE){
            return FAILURE;
        }
    }
    catch(const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType FreeGladiator(void *DS, int gladiatorID){
    if(!DS || gladiatorID<=0){
        return INVALID_INPUT;
    }
    Arena* arena = (Arena*)(DS);
    try{
        if(arena->FreeGladiator(gladiatorID) == FAILURE){
            return FAILURE;
        }
    }
    catch(const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType LevelUp(void *DS, int gladiatorID, int levelIncrease){
    if(!DS || gladiatorID <= 0 || levelIncrease <=0){
        return INVALID_INPUT;
    }
    Arena* arena = (Arena*)(DS);
    try {
        if(arena->LevelUp(gladiatorID, levelIncrease) == FAILURE){
            return FAILURE;
        }
    }
    catch(const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType UpgradeGladiator(void *DS, int gladiatorID, int upgradedID){
    if(!DS || gladiatorID <= 0 || upgradedID <= 0){
        return INVALID_INPUT;
    }
    Arena* arena = (Arena*)(DS);
    try{
        if(arena->UpgradeGladiator(gladiatorID, upgradedID) == FAILURE){
            return FAILURE;
        }
    }
    catch(const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType GetTopGladiator(void *DS, int trainerID, int *gladiatorID){
    if(!DS || !gladiatorID || trainerID == 0){
        return INVALID_INPUT;
    }
    Arena* arena = (Arena*)(DS);
    try{
        if(arena->GetTopGladiator(trainerID, gladiatorID)){
            return FAILURE;
        }
    }
    catch(const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType GetAllGladiatorsByLevel(void *DS, int trainerID, int **gladiators, int *numOfGladiator){
    if(!DS || !gladiators || !numOfGladiator || trainerID == 0){
        return INVALID_INPUT;
    }
    Arena* arena = (Arena*)(DS);
    try{
        if(arena->GetAllGladiatorsByLevel(trainerID, gladiators, numOfGladiator) == FAILURE){
            return FAILURE;
        }
    }
    catch(const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType UpdateLevels(void *DS, int stimulantCode, int stimulantFactor){
    if(DS == NULL || stimulantCode < 1 || stimulantFactor <1){
        return INVALID_INPUT;
    }
    Arena* arena = (Arena*)(DS);
    try{
        if(arena->UpdateLevels(stimulantCode, stimulantFactor)){
            return FAILURE;
        }
    }
    catch(const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

void Quit(void** DS){
    delete (Arena*)(*DS);
    *DS=NULL;
}
