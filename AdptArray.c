#include "AdptArray.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct AdptArray_{
    COPY_FUNC copyFunc;
    DEL_FUNC delFunc;
    PRINT_FUNC printFunc;
    int size;
    PElement *array;
} Adparray, *PAdptArray;

PAdptArray CreateAdptArray(COPY_FUNC copy, DEL_FUNC del, PRINT_FUNC print){
    PAdptArray newAdpArray = (PAdptArray)malloc(sizeof(Adparray));
    // check if the malloc succeed
    if (newAdpArray == NULL)
    {
        return NULL;
    }
    int size = 1;
    newAdpArray -> array = (PElement*)calloc(sizeof(PElement),size);

    //check if the malloc succeed
    if (newAdpArray -> array  == NULL)
    {
        free(newAdpArray);
        return NULL;
    }
    newAdpArray -> copyFunc = copy;
    newAdpArray -> delFunc = del;
    newAdpArray -> printFunc = print;
    newAdpArray -> size = size;

    return newAdpArray;
}

void DeleteAdptArray(PAdptArray pAdptArray){
    //delete any element in the array
    for(size_t i=0; i<pAdptArray->size; i++){
        if (pAdptArray->array[i] != NULL){
            pAdptArray->delFunc(pAdptArray->array[i]);
        }
    }
    //free the array
    free(pAdptArray->array);
    //free pAdptArray
    free(pAdptArray);
}

Result SetAdptArrayAt(PAdptArray pAdptArray, int index, PElement pElement){
    //in case we need to realocc the array
    if(pAdptArray->size <= index){
        pAdptArray->array = (PElement*)realloc(pAdptArray->array,sizeof(PElement)*(index+1));
        //check if the realloc succeed
        if(pAdptArray->array == NULL){
            return FAIL;
        }
        pAdptArray->array[index] =  pAdptArray->copyFunc(pElement);
         //check if the copy succeed
        if(pAdptArray->array[index] == NULL){
            return FAIL;
        }
        //reset all the empty values to NULL
        for(size_t i=pAdptArray->size; i<index; i++){
            pAdptArray -> array[i] = NULL;
        }
        pAdptArray->size = index + 1;
    }
    else{
        //in case there is an elemnt in the requested index - so first we delete the old element:
        if(pAdptArray->array[index] != NULL){
            pAdptArray->delFunc(pAdptArray->array[index]);
        }
        pAdptArray->array[index] =  pAdptArray->copyFunc(pElement);
         //check if the copy succeed
        if(pAdptArray->array[index] == NULL){
            return FAIL;
        }
    }
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray pAdptArray, int index){
    //the index isn't  valid
    if (index >= pAdptArray->size){
        return NULL;
    }
    if (pAdptArray->array[index] == NULL){
        return NULL;
    }
    return pAdptArray->copyFunc(pAdptArray->array[index]);
}

int GetAdptArraySize(PAdptArray pAdptArray){
    if (pAdptArray ==  NULL){
        return -1;
    }
    return pAdptArray->size;
}

void PrintDB(PAdptArray pAdptArray){
    if (pAdptArray == NULL){
        return;
    }
    for(size_t i =0; i < pAdptArray->size; i++){
        if(pAdptArray->array[i] != NULL){
            printf("The element in index %ld is:\n", i);
            pAdptArray->printFunc(pAdptArray->array[i]);
            printf("\n");
        }
    }
}


