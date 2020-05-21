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

bool hasIdealMatching(int *matrix, int &picks);

bool isIdealMatching(int *matrix, int &picks);

int *getSpecialMatrix(int *matrix, int &picks);

void printMatrix(const int *matrix, int picks);

vector<const int *> findIdealMatching(int &picks, int &ribs, vector<Rib> &structRibs);

int *matrixClone(const int *matrix, int &picks);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    vector<Rib> ribsList;
    int n = 0, m = 0;
    initializeGraph(n, m, ribsList);
    sortRibs(n, m, ribsList);
    findIdealMatching(n, m, ribsList);
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

vector<const int *> findIdealMatching(int &picks, int &ribs, vector<Rib> &structRibs) {

    int *adjacenceMatrix = getAdjacenceMatrix(picks, structRibs);
    int *specialMatrix = getSpecialMatrix(getAdjacenceMatrix(picks, structRibs), picks);
    int *matchingMatrix = specialMatrix;

    vector<const int *> idealMatchingMatrixes;
    vector<vector<int>> helpVector;
    vector<int> tmpVector;

    if (!hasIdealMatching(adjacenceMatrix, picks))return {};

    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if (*(adjacenceMatrix + i * picks + j) == 1)tmpVector.push_back(j);
        }
        helpVector.push_back(tmpVector);
        tmpVector.clear();
    }

    int combinationNumber = helpVector[0].size();

    for (int i = 1; i < helpVector.size(); i++) {
        combinationNumber = combinationNumber * helpVector[i].size();
    }

    vector<int> counter(picks, 0);

    for (int i = 0; i < combinationNumber; i++) {

        for (int j = 0; j < picks; j++) {
            *(matchingMatrix + j * picks + helpVector[j][counter[j]]) = 1;
        }

        int p = i;
        bool b = true;
        while (b) {

            if (counter[p % picks] < helpVector[p % picks].size() - 1) {
                counter[p % picks]++;
                b = false;
            } else {
                counter[p % picks] = 0;
                p++;
            }
        }
printMatrix(matchingMatrix,picks);
        if (isIdealMatching(matchingMatrix, picks)) {
            idealMatchingMatrixes.push_back(matrixClone(matchingMatrix,picks));
             //printMatrix(idealMatchingMatrixes[idealMatchingMatrixes.size()-1],picks);
        }
        matchingMatrix = getSpecialMatrix(specialMatrix, picks);
    }
    for(int i=0; i<idealMatchingMatrixes.size(); i++){
     //printMatrix(idealMatchingMatrixes[i], picks);
         }
    return idealMatchingMatrixes;
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

bool hasIdealMatching(int *matrix, int &picks) {
    int columnCounter = 0;
    int rowCounter = 0;
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if (*(matrix + i * picks + j) == -1)rowCounter++;
            if (*(matrix + j * picks + i) == -1)columnCounter++;
        }
        if (columnCounter == picks || rowCounter == picks)return false;
        rowCounter = 0;
        columnCounter = 0;
    }
    return true;
}

bool isIdealMatching(int *matrix, int &picks) {
    int columnCounter = 0;
    int rowCounter = 0;
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if (*(matrix + i * picks + j) == 1)rowCounter++;
            if (*(matrix + j * picks + i) == 1)columnCounter++;
        }
        if (columnCounter > 1 || rowCounter > 1)return false;
        rowCounter = 0;
        columnCounter = 0;
    }
    return true;
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
            cout <<setw(3) <<*(matrix + i * picks + j)  ;
        }
        cout << endl;
    }
    cout << endl;
}