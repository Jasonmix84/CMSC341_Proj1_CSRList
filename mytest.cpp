/*
    Filename: mytest.cpp
    Author: Jason Rojas
    Date: 9/26/23
    Email: jasonr2@umbc.edu
    Description: testing functions for csr.cpp
*/

#include "csr.h"

const string  FAIL_STATEMENT = "*****TEST FAILED\n";
const string  PASS_STATEMENT = "*****TEST PASSED\n";

class Tester{
    public:
    bool compressTest(CSR & obj, int row, int col, int array[], int arraySize, int nonZeroValues){
        bool result = true;
        //edge case where either rows or column numbers are zero should create an empty object
        if (row == 0 || col == 0){
            if (obj.empty()){
                return result;
            }
            else{
                result = false;
                cout << "Did not create an empty object" <<endl;
                return result;
            }
        }
        //if mXn is bigger than arraySize
        if ((row * col) > arraySize ){
            if (array[arraySize - 1] == 0){
                return result;
            }
            else{
                result = false;
                cout << "array was not filled with zeros" << endl;
                return result;
            }
        }
        //array size is 0 but rows and cols are good
        if ((arraySize == 0) && ((row > 0) && (col > 0))){
            if (obj.empty()){
                cout << "should not create an empty obj" << endl;
                result = false;
                return result;
            }
            else{
                return result;
            }
        }
        if ((row * col) < arraySize){
            if (obj.empty()){
                cout << "Should not creat an empty obj" << endl;
                result = false;
                return result;
            }
            else{
                return result;
            }
        }
        //normal case
        if (obj.m_nonzeros == nonZeroValues){
            return result;
        }
        else{
            cout << "number of nonzeroValues is different" << endl;
            result = false;
            return result;
        }
    }

    bool overloadedEqualityTest(CSR & lhs, CSR & rhs){
        //expect lhs to be an exact copy of rhs
        bool result = true;

        //check if two objects are empty
        if (lhs.empty() && rhs.empty()){
            return result;
        }
        if (rhs.m_nonzeros != lhs.m_nonzeros){
            result = false;
            return result;
        }
        //result should have the same number at the corresponding index in m_values
        for (int i = 0; i < rhs.m_nonzeros; i++){
            result = result && (lhs.m_values[i] == rhs.m_values[i]);   
        }

        //result should have the same number at the corresponding index in m_col_index
        for (int i = 0; i < rhs.m_nonzeros; i++){
            result = result && (lhs.m_col_index[i] == rhs.m_col_index[i]);   
        }

        //result should have the same number at the corresponding index in m_row_index
        for (int i = 0; i < rhs.m_m + 1; i++){
            result = result && (lhs.m_row_index[i] == rhs.m_row_index[i]);
        }

        //testing rest of the member variables
        result = result && (lhs.m_nonzeros == rhs.m_nonzeros);
        result = result && (lhs.m_m == rhs.m_m);
        result = result && (lhs.m_n == rhs.m_n);

        return result;
    }

    bool sparseTest(CSR & obj, int expectedOutput){
        bool result = true;

        if (obj.sparseRatio() == expectedOutput){
            return result;
        }
        else{
            return false;
        }
    }

    bool getAtTest(CSR & obj, int row, int col, int expectedValue){
        bool result = true;

        if (obj.getAt(row, col) == expectedValue){
            return result;
        }
        else{
            return false;
        }
    }
    bool listGetAtTest(CSRList & LL, int matrixIndex, int row, int col, int expectedValue){
        bool result = true;
        if (LL.getAt(matrixIndex, row, col) == expectedValue){
            return result;
        }
        else{
            return false;
        }
    }

    bool avgSparseTest(CSRList & LL, int expectedOutput){
        bool result = true;

        if (LL.averageSparseRatio() == expectedOutput){
            return result;
        }
        else{
            return false;
        }
    }
};

int main(){
    Tester tester;
    CSR aCSR;
    int array1[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    aCSR.compress(4,6,array1,24);//initialize object aCSR
    //Compress normal case
    cout << "TESTING Compress normal case" << endl;
    if (tester.compressTest(aCSR, 4, 6, array1, 24, 8)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //Compress edge case
    CSR bCSR;
    bCSR.compress(3, 3, array1, 24);
    cout << "TESTING Compress Edge Case row*col < array size" << endl;
    if (tester.compressTest(bCSR, 3, 3, array1, 24, 8)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //Compress error Case
    CSR cCSR;
    cCSR.compress(0, 1, array1, 24);
    cout << "TESTING Compress error case row = 0 Pass occurs if object is empty" << endl;
    if (tester.compressTest(cCSR, 0, 1, array1, 24, 8)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //testing Overloaded equality for normal case
    cout << "testing Overloaded equality for normal case" << endl;
    if (tester.overloadedEqualityTest(aCSR, aCSR)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //testing copy constructor
    CSR dCSR(aCSR);
    //result should be equal
    cout << "Testing if copy constructor created an equal object" << endl;
    if (tester.overloadedEqualityTest(aCSR, dCSR)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    CSR eCSR;
    int array2[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,80,0,90,0,100};
    eCSR.compress(4,6,array2,24);//initialize object aCSR
    //testing Overloaded equality for error case
    cout << "Testing Overloaded equality case for error; Passes if they are not equivalent" << endl;
    if (tester.overloadedEqualityTest(dCSR, eCSR)){
        cout << FAIL_STATEMENT << endl;
    }
    else{
        cout << PASS_STATEMENT << endl;
    }

    //testing Overloaded equality for edge case 2 empty objs
    CSR fCSR;
    CSR gCSR;
    int array3[] = {10, 4 , 0, 0, 6, 0, 70, 2, 5, 0};
    int array4[] = {0, 0, 0, 0, 40, 0, 70, 0, 5, 0};
    fCSR.compress(2, 0, array3, 10);
    gCSR.compress(0, 0, array4, 10);
    cout << "Testing if two objects are empty; Passes if they are equivalent" << endl;
    if (tester.overloadedEqualityTest(fCSR, gCSR)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //testing copy constructor on empty object
    CSR hCSR(fCSR);
    cout << "Testing that copy constructor on empty object makes an empty obj; passes if both equal empty" << endl;
    if (tester.overloadedEqualityTest(fCSR, hCSR)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    // normal case for overloaded == 
    CSR iCSR(aCSR);

    cout << "Testing overloaded == operator; passes if copy constructor works " << endl; 
    if (aCSR == iCSR){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //error case for overloaded ==
    CSR jCSR;
    int array5[] = {0, 2, 2, 2, 51, 0, 0, 5, 8, 0, 2, 12};
    jCSR.compress(3, 4, array5, 12);
    cout << "Testing overloaded == operator error case; passes if jCSR == aCSR results in false " << endl;
    if(jCSR == aCSR){
        cout << FAIL_STATEMENT << endl;
    }
    else{
        cout << PASS_STATEMENT << endl;
    }

    //test Sparse Ratio normal case expected result is 66
    CSR kCSR;
    int array6[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    kCSR.compress(4,6,array6,24);//initialize object aCSR
    cout << "Testing Sparse Ratio; passes if sparse ratio == 66" << endl;
    if (tester.sparseTest(kCSR, 66)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //test Sparse Ratio on error case expected result is 0
    CSR lCSR;
    lCSR.compress(0, 6, array6, 24);
    cout << "Testing Sparse Ratio on empty obj; passes if sparse ratio == 0" << endl;
    if (tester.sparseTest(lCSR, 0)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //test Sparse Ratio on error case expected result is 0
    CSR mCSR;
    int array7[] = {0, 0, 0, 0, 0, 0};
    mCSR.compress(2, 3, array7, 6);
    cout << "Testing Sparse Ratio on an array with only 0; passes if sparse ratio == 100" << endl;
    if (tester.sparseTest(mCSR, 100)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //testing getAt CSR normal case 
    cout << "Testing getAT on CSR Objects; passes if getAt(2, 4) == 70" << endl;
    if (tester.getAtTest(aCSR, 2, 4, 70)){
        cout << PASS_STATEMENT << endl;
    } //returns the value 70
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //testing getAt CSR error case
    try{
        cout << "Testing getAt on out of bounds; passes if returns exception" << endl;
        if (tester.getAtTest(aCSR, 6, 5, 0)){
            cout << FAIL_STATEMENT << endl;
        }
    }
    catch (exception &e){
        cout << e.what() << endl;
        cout << PASS_STATEMENT << endl;
    }

    //testing getAt CSR edgeCase
    cout << "Testing getAt on CSR edge case; passes if getAt(3, 5) == 80" << endl;
    if (tester.getAtTest(aCSR, 3, 5, 80)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //TESTING CSRLIST

    //test insertAtHead
    CSRList aCSRList;
    aCSRList.insertAtHead(cCSR);
    aCSRList.insertAtHead(bCSR);
    aCSRList.insertAtHead(aCSR);
    cout << "test if insert at head creates a linked list; passes if aCSR is not empty" <<endl;
    if (aCSRList.empty()){
        cout << FAIL_STATEMENT << endl;
    }
    else{
        cout << PASS_STATEMENT << endl;
    }
    

    //testing CSRList copy constructor
    CSRList bCSRList(aCSRList);
    cout << "testing copy constructor passes if copy of bCSRList == aCSRLIST" << endl;
    cout << "also testing that == works for empty objects" << endl; 
    if (bCSRList == aCSRList){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //testing error case
    aCSRList.insertAtHead(dCSR);
    cout << "Testing an error case of overloaded ==; passes if they are not equal" << endl;
    if(aCSRList == bCSRList){
        cout << FAIL_STATEMENT << endl;
    }
    else{
        cout << PASS_STATEMENT << endl;
    }

    //test GetAt for CSRLIST
    CSRList cCSRList;//create cCSRList
    cCSRList.insertAtHead(aCSR);
    cCSRList.insertAtHead(cCSR);

    //testing normal case CSRList GetAt
    cout << "Testing normal case CSRList GetAt; passes if getat(1,2,4) == 70" << endl;
    if (tester.listGetAtTest(cCSRList, 1, 2, 4, 70)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //testing error case CSRList GetAt
    cout << "Testing an error case CSRList GetAt; passes if getAt(5, 2, 2) throws an exception" << endl;
    try{
        if (tester.listGetAtTest(cCSRList, 5, 2, 2, 0)){
            cout << FAIL_STATEMENT << endl;
        }
    }
    catch (exception &e){
        cout << e.what() << endl;
        cout << PASS_STATEMENT << endl;
    }

    //testing edge error case CSRList GetAt
    cout << "Testing an error case CSRList GetAt; passes if getAt(2, 2, 2) throws an exception" << endl;
    try{
        if (tester.listGetAtTest(cCSRList, 5, 2, 2, 0)){
            cout << FAIL_STATEMENT << endl;
        }
    }
    catch (exception &e){
        cout << e.what() << endl;
        cout << PASS_STATEMENT << endl;
    }

    //testing error case CSRList GetAt on an empty object
    cout << "Testing an error case CSRList GetAt();  passes if getAT(1, 2, 4) throws an exception as index 1 is an empty obj" << endl;
    try{
        if (tester.listGetAtTest(cCSRList, 0, 2, 4, 0)){
            cout << FAIL_STATEMENT << endl;
        }
        else{
            cout << FAIL_STATEMENT << endl;
        }
    }
    catch (exception &e){
        cout << e.what() << endl;
        cout << PASS_STATEMENT << endl;
    }

    //Testing average SparseRatio()
    cout << "testing average sparse ratio for cCSRList; passes if output is 33 as expected" << endl;
    if(tester.avgSparseTest(cCSRList, 33)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }
    //testing edge case of SPARSERATIO()
    //all empty matrix
    CSRList dCSRList;
    dCSRList.insertAtHead(fCSR);
    dCSRList.insertAtHead(gCSR);
    CSR nCSR(gCSR);
    dCSRList.insertAtHead(nCSR);

    cout << "testing average sparse ratio edge case for dCSRList; passes if output is 0 as expected" << endl;
    if(tester.avgSparseTest(dCSRList, 0)){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //Testing overloaded = operator for CSRList
    //testing normal case
    CSRList fCSRList;
    fCSRList.insertAtHead(aCSR);
    fCSRList.insertAtHead(kCSR);
    fCSRList.insertAtHead(eCSR);
    aCSRList = fCSRList;

    cout << "testing if overloaded assignment operator works in normal case; passes if bCSR == aCSR" << endl;
    if (aCSRList == bCSRList){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }

    //testing edge case 
    aCSRList = dCSRList;
    cout << "testing if overloaded assignment operator works in edge case; passes if aCSR == dCSR after calling i.e both list are empty" << endl;
    if (dCSRList == aCSRList){
        cout << PASS_STATEMENT << endl;
    }
    else{
        cout << FAIL_STATEMENT << endl;
    }
    return 0;
}