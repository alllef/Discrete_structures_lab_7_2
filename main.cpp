#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include<Windows.h>
#include<iomanip>

using namespace std;

struct Rib {
public:
    int start;
    int end;
};

void initializeGraph(int &picks, int &ribs, vector<Rib> &structRibs);

void sortRibs(int &picks, int &ribs, vector<Rib> &structRibs);

int *getAdjacenceMatrix(int &picks, vector<Rib> &structRibs);

int *getSpecialMatrix(int *matrix, int &picks);

void printMatrix(const int *matrix, int picks);

bool checkRows(int *matrix, int picks);

bool checkColumns(int *matrix,int picks);

void findIdealMatching(int *matrix, int picks, int row);


int *matrixClone(const int *matrix, int &picks);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    vector<Rib> ribsList;
    int n = 0, m = 0;
    initializeGraph(n, m, ribsList);
    sortRibs(n, m, ribsList);
    int *specialMatrix=getSpecialMatrix(getAdjacenceMatrix(n,ribsList),n);
findIdealMatching(specialMatrix,n,0);
    return 0;
}

void initializeGraph(int &picks, int &ribs, vector<Rib> &structRibs) {
    Rib myRib{};
    ifstream inFile;
    inFile.open("myGraph.txt");

    if (!inFile.is_open()) cout << "It is not open" << endl;
    inFile >> picks >> ribs;

    for (int i = 0; i < ribs; i++) {
        inFile >> myRib.start >> myRib.end;

        structRibs.push_back(myRib);
    }
    inFile.close();
}

void sortRibs(int &picks, int &ribs, vector<Rib> &structRibs) {

    Rib tmp{};
    for (int i = 0; i < ribs - 1; i++) {
        for (int j = 0; j < ribs - 1; j++) {
            if ((structRibs[j].start + structRibs[j].end) > (structRibs[j + 1].start + structRibs[j + 1].end)) {

                tmp = structRibs[j];
                structRibs[j] = structRibs[j + 1];
                structRibs[j + 1] = tmp;

            }
        }
    }
}

void findIdealMatching(int *matrix, int picks, int row) {
    if (row < picks) {
        for (int i = 0; i < picks; i++) {
            if (*(matrix + row * picks + i) == 0 && checkColumns(matrix,picks)) {
                int *newMatrix = matrixClone(matrix, picks);
                *(newMatrix + row * picks + i) = 1;
                findIdealMatching(newMatrix, picks, row + 1);
            }
        }
    } else {
        if (checkRows(matrix,picks)&&checkColumns(matrix,picks)) {
            printMatrix(matrix, picks);
        }
    }
}

bool checkRows(int *matrix, int picks) {
    int counter=0;
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if(*(matrix+i*picks+j)==1)counter++;
        }
        if(counter>1)return false;
        counter=0;
    }
    return true;
}

bool checkColumns(int *matrix,int picks){
    int counter=0;
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if(*(matrix+j*picks+i)==1)counter++;
        }
        if(counter>1)return false;
        counter=0;
    }
    return true;
}

int *getAdjacenceMatrix(int &picks, vector<Rib> &structRibs) {
    int *adjacenceMatrix = new int[picks * picks];

    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            *(adjacenceMatrix + i * picks + j) = -1;
        }
    }

    for (auto &structRib : structRibs) {
        *(adjacenceMatrix + (structRib.start - 1) * picks + (structRib.end - 1)) = 1;
    }

    return adjacenceMatrix;
}

int *getSpecialMatrix(int *matrix, int &picks) {
    int *specialMatrix = matrix;
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if (*(specialMatrix + i * picks + j) == 1)*(specialMatrix + i * picks + j) = 0;
        }
    }
    return specialMatrix;
}


int *matrixClone(const int *matrix, int &picks) {
    int *clone = new int[picks * picks];
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            *(clone + i * picks + j) = *(matrix + i * picks + j);
        }
    }
    return clone;
}

void printMatrix(const int *matrix, int picks) {
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            cout << setw(3) << *(matrix + i * picks + j);
        }
        cout << endl;
    }
    cout << endl;
}