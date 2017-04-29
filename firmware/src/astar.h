/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <stddef.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */
#define LIST_SIZE 410

    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************

    /** Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
    
    int MIN_X;
    int MIN_Y;
    int MAX_X;
    int MAX_Y;
    
    typedef struct Node {
        int x;
        int y;
        unsigned gn;
        unsigned hn;
        struct Node* ptr;
    } node;
    
    typedef struct start_node {
        int sx;
        int sy;
    } start;

    typedef struct goal_node {
        int gx;
        int gy;
    } goal;
    
    typedef struct coordinate_t {
        int x;
        int y;
    } coordinate;
    
    node* open_list [LIST_SIZE];
    node* close_list [LIST_SIZE];
    node* traceback_list [LIST_SIZE];
    node node_list [LIST_SIZE];
    coordinate obsticle_list[LIST_SIZE];
    coordinate rover_list[LIST_SIZE];
    int boundary_list[4];

    start startPosition;
    goal goalPosition;
    
    coordinate flag;
    
    int defenseLength;
    int centerLength;

    int openListLen;
    int closeListLen;
    int traceBackListLen;
    int nodeListLen;
    int obsticleListLen;
    int roverListLen;
    int boundaryListLen;

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************
    /**
      @Function
        int ExampleFunctionName ( int param1, int param2 ) 

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
    // Functions
    void explorePath(int sx, int sy, int gx, int gy);
    void backTrace(node* goalNode);
    void sortOpenList();
    void pushClosedList(node* nodePtr);
    void pushOpenList(int x, int y, int g, node* pre);
    void pushNeighborsOpenList(node* origNode);
    node* popOpenList();
    node* findPtr(int x, int y, int g, node* pre);
    int findMinNodeOpenList();
    void initObsticles();
    void printTrace();


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
