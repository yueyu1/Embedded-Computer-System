/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include "astar.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */
void explorePath(int sx, int sy, int gx, int gy) {
    pushOpenList(sx, sy, 0, NULL);
    
    while (openListLen > 0) {
        node* visitedNode = popOpenList();
        pushClosedList(visitedNode);
        if (visitedNode->x == gx && visitedNode->y == gy){
            backTrace(visitedNode);
            return;
        }
        else {
            pushNeighborsOpenList(visitedNode);
            sortOpenList();
        }
    }
}

void pushClosedList(node* nodePtr) {
    close_list[closeListLen] = nodePtr;
    closeListLen++;
}

void pushOpenList(int x, int y, int g, node* pre) {
    
    int i;
    
    // Check boundaries
    if (x < MIN_X || x > MAX_X || y < MIN_Y || y > MAX_Y) {
        return;
    }
    
    // Check obsticles
    for (i = 0; i < obsticleListLen; i++) {
        if (x == obsticle_list[i].x && y == obsticle_list[i].y) {
            return;
        }
    }
        
    node* newPtr = findPtr(x, y, g, pre);
    
    for (i = 0; i < openListLen; i++) {
        if (open_list[i] == newPtr) {
            return;
        }
    }
    for (i = 0; i < closeListLen; i++) {
        if (close_list[i] == newPtr) {
            if (close_list[i]->gn > g) {
                close_list[i]->gn = g;
                close_list[i]->ptr = pre;
            }
            return;
        }
    }
    open_list[openListLen] = newPtr;
    openListLen++;
}

node* popOpenList() {
    openListLen--;
    return open_list[openListLen];
}

void backTrace(node* goalNode) {
    node* trace = goalNode;
    while (trace->ptr != NULL) {
        traceback_list[traceBackListLen] = trace;
        traceBackListLen++;
        trace = trace->ptr;
    }
}

// Place the minimum (gn + hn) to the last position
void sortOpenList() {
    if (openListLen < 2) {
        return;
    }
    
    int minInd = findMinNodeOpenList();
    
    if (minInd == (openListLen - 1)) {
        return;
    }
    
    if (minInd < (openListLen - 1)) {
        node* temp = open_list[openListLen - 1];
        open_list[openListLen - 1] = open_list[minInd];
        open_list[minInd] = temp;
    }
    
}

void pushNeighborsOpenList(node* origNode) {
    pushOpenList(origNode->x - 1, origNode->y, origNode->gn + 1, origNode);
    pushOpenList(origNode->x + 1, origNode->y, origNode->gn + 1, origNode);
    pushOpenList(origNode->x, origNode->y + 1, origNode->gn + 1, origNode);
    pushOpenList(origNode->x, origNode->y - 1, origNode->gn + 1, origNode);
}

node* findPtr(int x, int y, int g, node* pre) {
    int i;
    for (i = 0; i < nodeListLen; i++) {
        if (x == node_list[i].x && y == node_list[i].y) {
            return &node_list[i];
        }
    }
    node_list[nodeListLen].x = x;
    node_list[nodeListLen].y = y;
    node_list[nodeListLen].gn = g;
    node_list[nodeListLen].hn = abs(goalPosition.gx - x) + abs(goalPosition.gy - y);
    node_list[nodeListLen].ptr = pre;
    nodeListLen++;
    return &node_list[nodeListLen - 1];
}

int findMinNodeOpenList() {
    int min, temp, currfn;
    int i;
    int index = 0;
    min = open_list[0]->gn + open_list[0]->hn;
    temp = open_list[1]->gn + open_list[1]->gn;
    
    if (temp < min) {
        min = temp;
        index = 1;
    }
    
    for (i = 2; i < openListLen; i++) {
        currfn = open_list[i]->gn + open_list[i]->hn;
        if (currfn < min) {
            min = currfn;
            index = i;
        }
    }
    return index;
}

void printTrace() {
    int i;
    for (i = 0; i < traceBackListLen; i++) {
        int x = traceback_list[i]->x;
        int y = traceback_list[i]->y;
        //printf("(%d, %d)\n", x, y);
    }
    //printf("(%d, %d)\n", startPosition.sx, startPosition.sy);
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */


/* *****************************************************************************
 End of File
 */
