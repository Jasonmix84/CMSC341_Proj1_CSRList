// CMSC 341 - Fall 2023 - Project 1
#include "csr.h"
///////////////CSR Class Implementation///////////////
CSR::CSR(){
    //initializing all member variables
    // creating an empty object
    m_values = nullptr;
    m_col_index = nullptr;
    m_row_index = nullptr;
    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;
    m_next = nullptr;
}
CSR::~CSR(){
    clear();
}
CSR::CSR(const CSR & rhs){
    //create new int array pointers of the same size as the arrays of rhs
    if (rhs.empty()){
        m_values = nullptr;
        m_col_index = nullptr;
        m_row_index = nullptr;
        m_nonzeros = 0;
        m_m = 0;
        m_n = 0;
        m_next = nullptr;
        return;
    }
    m_values = new int[rhs.m_nonzeros];
    m_col_index = new int[rhs.m_nonzeros];
    m_row_index = new int[rhs.m_m + 1];

    //set other int variables to the same as rhs
    m_nonzeros = rhs.m_nonzeros;
    m_m = rhs.m_m;
    m_n = rhs.m_n;
    //keep pointer on null and change it when creating a linked list
    m_next = nullptr;

    //iterate and populate m_values and m_col_index
    for (int i = 0; i < m_nonzeros; i++){
        m_values[i] = rhs.m_values[i];
        m_col_index[i] = rhs.m_col_index[i];
    }
    //different for loop to iterate through m_row_index in case number of rows is larger than the nonzero values
    for(int i = 0; i < m_m + 1; i++){
        m_row_index[i] = rhs.m_row_index[i];
    }
}
void CSR::clear(){
    //deallocate dynamically created arrays
    delete [] m_values;
    delete [] m_col_index;
    delete [] m_row_index;

    //set member variables to zero
    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;
    
    //set member pointer variables to nullptr

    m_values = nullptr;
    m_col_index = nullptr;
    m_row_index = nullptr;
    m_next = nullptr;
}
bool CSR::empty() const{
    //check that all conditions of an empty object are met
    if (m_values == nullptr && m_nonzeros == 0 && m_m == 0 && m_n == 0){
        return true;
    }
    return false;
}
void CSR::compress(int m, int n, int array[], int arraySize){
    //this function will initialize all the member variables
    //important the function can be called on an already compressed matrix it still makes a new compressed matrix and must create that new object

    // case where m X n ! = arraySize
    // if mXn is bigger than arraySize then you initialize list and the remaining spaces are filled with ZERO
    // prevents a segfault when Later i iterate through the array 
    if ((m*n) > arraySize){
        int loops = ((m*n) - arraySize);
        for (int i = 1; i < loops + 1; i++){
            array[arraySize + i] = 0;
        }
    }
    // if m == 0 and n ==0 initialize an EMPTY Object
    if ((m == 0) || (n == 0)){
        m_values = nullptr;
        m_col_index = nullptr;
        m_row_index = nullptr;
        m_nonzeros = 0;
        m_m = 0;
        m_n = 0;
        m_next = nullptr;
    }
    // if arraySize == 0 but mxn is good fill in all members with 0
    else if ((arraySize == 0) && ((m > 0) && (n > 0))){
        m_values = new int[arraySize];
        m_col_index = new int[arraySize];
        m_row_index = new int[m + 1];
        m_nonzeros = 0;
        m_m = 0;
        m_n = 0;
        m_next = nullptr;
    }
    // if mxn is smaller than arraySize you only initialize the numbers up to MxN and ignore the rest
    // will be the same as the normal case instead the for loop goes for M*N
    //any other case is just a normal case

    //implementation of normal case and a larger arraySize thean M*N here
    else{
        m_values = new int[(m*n)]; // initialize dynamically created arrays
        m_col_index = new int[(m*n)];
        m_row_index = new int[m+1]; //set array size to rows + 1 because arr[0] always == 0
        m_nonzeros = 0;
        m_m = m;
        m_n = n;
        m_next = nullptr;

        //helper variables to keep track of rows and columns
        int index = 0;
        int colIndex = 0;
        int rowIndex = 0;

        //iterate through the size of rows * columns to get nonzzero values
        // dont use arraySize incase arraySize is bigger than the matrix size and we only hold up to m*n values
        for (int i = 0; i < (m_m * m_n); i++){
            //every column number of times i reset my column index to be able to keep track of column number for m_col_index
            // also add the sum of the number on nonzero values for each row 
            if ((i % m_n) == 0){
                colIndex = 0;
                m_row_index[rowIndex] = m_nonzeros;
                rowIndex++;
            }
            // when there is a nonzero value
            if(array[i] > 0){
                //add that value to m_value and add the current column number to m_col_index at the same index to match each other
                m_values[index] = array[i];
                m_col_index[index] = colIndex;
                index++;
                //make sure to update m_nonzeros
                m_nonzeros++;
            }
            //keep track of column index of each number from array[]
            colIndex++;
        }
        // In order to get the last number in the array if it is a nonzero value
        if (array[(m_m * m_n) - 1] > 0){
            m_row_index[m_m] = m_nonzeros;
        }
    }
}
int CSR::getAt(int row, int  col) const{
    //try is made in driver/mytest.cpp
    //catch catches string then prints it out
    //row and col validation
    if (row < 0 || col < 0){
        throw runtime_error("Exception Error: Object is not in the list!");
    }
    if (row > m_m || col > m_n){
        throw runtime_error("Exception Error: Object is not in the list!");
    }

    //Get the range of the row
    int rowStart = m_row_index[row];
    int rowEnd = m_row_index[row + 1];

    //check if the column requested is the same for each number in a row
    for (int i = rowStart; i < rowEnd; i++){
        //check that the column matches the one given 
        //if so return the corresponding value at that column
        if (m_col_index[i] == col){
            return m_values[i];
        }
    }

    //whenever the row and col correspond to a ZERO
    return 0;
    
} 
bool CSR::operator==(const CSR & rhs) const{
    bool result = true;
    // no need to loop if either of these values arent the same
    if (m_values == nullptr && m_nonzeros == 0 && m_m == 0 && m_n == 0 && rhs.empty()){
        return result;
    }
    else{
        //result should have the same number at the corresponding index in m_values
        for (int i = 0; i < rhs.m_nonzeros; i++){
            result = result && (m_values[i] == rhs.m_values[i]);
        }

        //result should have the same number at the corresponding index in m_col_index
        for (int i = 0; i < rhs.m_nonzeros; i++){
            result = result && (m_col_index[i] == rhs.m_col_index[i]);
        }

        //result should have the same number at the corresponding index in m_row_index
        for (int i = 0; i < rhs.m_m; i++){
            result = result && (m_row_index[i] == rhs.m_row_index[i]);
        }

        //testing rest of the member variables
        result = result && (m_nonzeros == rhs.m_nonzeros); 
        result = result && (m_m == rhs.m_m); 
        result = result && (m_n == rhs.m_n); 
    }
    //did not fail any tests
    return result;
}
int CSR::sparseRatio(){
    //constants to return 0 or 100 based on certain edge cases
    const int ZERO = 0;
    const int HUNDRED = 100;
    if(m_values == nullptr){
        // if passed an empty object
        return ZERO;
    }
    else if(m_nonzeros == ZERO){
        //if called on an object with only zeros
        return HUNDRED;
    }
    else {
        //normal case
        double totalNumMemb = (m_m * m_n);
        double numOfZero = (totalNumMemb - m_nonzeros);
        double sparseRat = numOfZero/totalNumMemb;
        sparseRat *= 100;
        return int(sparseRat);
    }
}
void CSR::dump(){
    cout << endl;
    if (!empty()){
        for (int i=0;i<m_nonzeros;i++)
            cout << m_values[i] << " ";
        cout << endl;
        for (int i=0;i<m_nonzeros;i++)
            cout << m_col_index[i] << " ";
        cout << endl;
        for (int i=0;i<m_m+1;i++)
            cout << m_row_index[i] << " ";
    }
    else
        cout << "The object is empty!";
    cout << endl;
}

//////////////CSRList Class Implementation///////////////
CSRList::CSRList(){
    m_head = nullptr;
    m_size = 0;
}
CSRList::CSRList(const CSRList & rhs){
    //create two pointers to iterate through linked list
    //curr goes through rhs linked list
    //temp will call copyconstructor to create identical nodes then set the m_next to a new CSR object
    // and create a linked list of CSR Objects
    if(rhs.empty()){
        m_head = nullptr;
        m_size = 0;
        return;
    }
    CSR* curr = rhs.m_head;
    //changed line below
    CSR* temp = new CSR(*curr);
    m_head = temp;
    m_size++;

    //iterate and copy
    while (curr->m_next != nullptr){
        curr = curr->m_next;
        CSR* nextCSR = new CSR(*curr);
        temp->m_next = nextCSR;
        m_size++;
        temp = temp->m_next;
    }
    m_size = rhs.m_size;
}
CSRList::~CSRList(){
    clear();
}
bool CSRList::empty() const{
    //if size is bigger than 0 it is not empty
    if (m_size > 0 || m_head != nullptr){
        return false;
    }
    //iterate through and make sure each node is empty
    // not sure if needed but doing it incase
    CSR* curr = m_head;
    while(curr != nullptr){
        if (curr->empty()){
            curr = curr->m_next;
        }
        else{
            return false;
        }
    }
    return true;
}
void CSRList::insertAtHead(const CSR & matrix){
    //copy and allocate a new matrix with the same data as the matrix passed
    CSR* copyMatrix = new CSR(matrix); 
    //set the matrix to point to current m_head
    copyMatrix->m_next = m_head; 
    //make m_head now equal the matrix which is pointing to previous m_head
    m_head = copyMatrix;                          
    
    //increase size
    m_size++;

}
void CSRList::clear(){
    //create a pointer to iterate through LL
    CSR* curr = m_head;
    while (curr != nullptr){
        m_head = curr;
        curr = curr->m_next;
        delete m_head;
    }

    //cleanup
    m_head = nullptr;
    m_size = 0;
}

int CSRList::getAt(int CSRIndex, int row, int col) const{
    if (CSRIndex < 0 || CSRIndex >= m_size){
        throw runtime_error("Exception Error: Object is not in the list!");
    }
    //iterate to the index
    CSR* curr = m_head;
    for (int i = 0; i < CSRIndex; i++){
        curr = curr->m_next;
    }
    if (curr->empty()){
        throw runtime_error("Exception Error: Object Empty!");
    }
    //store value
    int value = 0;

    //below statement should validate itself and throw an exception if there is an error
    value = curr->getAt(row, col);
    return value;

}
bool CSRList::operator== (const CSRList & rhs) const{
    //if sizes are different they definitely are not equivalent
    // if(m_size != rhs.m_size){
    //     return false;
    // }
    if (m_size == 0 && rhs.m_size == 0){
        if(this->m_head == nullptr && rhs.m_head == nullptr){
            return true;
        }
    }
    //Two currs to iterate through both LL
    CSR* curr = m_head;
    CSR* rhsCurr = rhs.m_head;

    while(curr != nullptr && rhsCurr != nullptr){
        //use overloaded == in CSR object class to check if they have the same information
        if (*curr == *rhsCurr){
            curr = curr->m_next;
            rhsCurr = rhsCurr->m_next;
        }
        else{
            //if the two nodes ever have different information
            return false;
        }
    }
    return true;
}
const CSRList& CSRList::operator=(const CSRList & rhs){
    //prevent self assignment
    if (this == &rhs){
        return *this;
    }

    //clear out data of LHS
    CSR* curr = m_head;
    while(curr != nullptr){
        m_head = curr;
        curr = curr->m_next;
        m_head->clear();
        delete m_head;
    }
    //clean up
    m_head = nullptr;
    m_size = 0;

    //use copy constructor to set lhs equal to rhs
    CSRList copiedList(rhs);
    return *this;
}
int CSRList::averageSparseRatio(){
    //create csr object to iterate through the LL
    CSR* curr = m_head;
    //use a double to add all the Sparse Ratio from all objects in LL
    double AvgSparseRatio = 0.0;
    while(curr != nullptr){
        AvgSparseRatio += curr->sparseRatio();
        curr = curr->m_next;
    }
    //get rid of decimals 
    double finalAvg = (AvgSparseRatio / double(m_size));
    return int (finalAvg);
}
void CSRList::dump(){
    if (!empty()){
        CSR* temp = m_head;
        while (temp != nullptr){
            temp->dump();
            temp = temp->m_next;
        }
    }
    else
        cout << "Error: List is empty!" << endl;
}
