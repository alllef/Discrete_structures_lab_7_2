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

void printMatrixes(vector<int*>idealMatchings,int picks);

bool checkRows(int *matrix, int picks);

bool checkColumns(int *matrix, int picks);

void findIdealMatching(vector<int*>&idealMatchings,int *matrix, int picks, int row);

int Permanent(int **mas, int m);

void GetMatr(int **mas, int **p, int i, int j, int m);

int**getMatrixForPermanent(int *matrix, int picks);

int *matrixClone(const int *matrix, int &picks);

int main() {
    SetConsoleOutputCP(CP_UTF8);

    vector<Rib> ribsList;
    int n = 0, m = 0;
    initializeGraph(n, m, ribsList);
    sortRibs(n, m, ribsList);

    int *specialMatrix = getSpecialMatrix(getAdjacenceMatrix(n, ribsList), n);
    int **MatrixForPermanent= getMatrixForPermanent(getAdjacenceMatrix(n, ribsList), n);

    if(Permanent(MatrixForPermanent, n)) {
        vector<int *> idealMatchings;
        findIdealMatching(idealMatchings, specialMatrix, n, 0);
        printMatrixes(idealMatchings,n);
    }

    else cout<< "В графі немає ідеальних паросполучень";
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

void findIdealMatching(vector<int*>&idealMatchings, int *matrix, int picks, int row) {
    if (row < picks) {
        for (int i = 0; i < picks; i++) {
            if (*(matrix + row * picks + i) == 0 && checkColumns(matrix, picks)) {
                int *newMatrix = matrixClone(matrix, picks);
                *(newMatrix + row * picks + i) = 1;
                findIdealMatching(idealMatchings,newMatrix, picks, row + 1);
            }
        }
    } else {
        if (checkRows(matrix, picks) && checkColumns(matrix, picks)) {
            idealMatchings.push_back(matrix);
        }
    }

}

bool checkRows(int *matrix, int picks) {
    int counter = 0;
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if (*(matrix + i * picks + j) == 1)counter++;
        }
        if (counter > 1)return false;
        counter = 0;
    }
    return true;
}

bool checkColumns(int *matrix, int picks) {
    int counter = 0;
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if (*(matrix + j * picks + i) == 1)counter++;
        }
        if (counter > 1)return false;
        counter = 0;
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

void printMatrixes(vector<int*>idealMatchings,int picks) {
    cout<< "Матриці ідеальних паросполучень\n";
    for(auto & idealMatching : idealMatchings) {
        for (int i = 0; i < picks; i++) {
            for (int j = 0; j < picks; j++) {
                if (!(*(idealMatching + i * picks + j) == -1)) cout << setw(3) << *(idealMatching + i * picks + j);
                else cout << setw(3) << "*";
            }
            cout << endl;
        }
        cout << endl;
    }
}

void GetMatr(int **mas, int **p, int i, int j, int m) {
    int ki, kj, di, dj;
    di = 0;
    for (ki = 0; ki<m - 1; ki++) {
        if (ki == i) di = 1;
        dj = 0;
        for (kj = 0; kj<m - 1; kj++) {
            if (kj == j) dj = 1;
            p[ki][kj] = mas[ki + di][kj + dj];
        }
    }
}



int Permanent(int **mas, int m) {
    int i, j, d, k, n;
    int **p;
    p = new int*[m];
    for (i = 0; i<m; i++)
        p[i] = new int[m];
    j = 0; d = 0;
    k = 1;
    n = m - 1;
    if (m == 1) {
        d = mas[0][0];
        return(d);
    }
    if (m == 2) {
        d = mas[0][0] * mas[1][1] + (mas[1][0] * mas[0][1]);
        return(d);
    }
    if (m>2) {
        for (i = 0; i<m; i++) {
            GetMatr(mas, p, i, 0, m);
            d = d + k * mas[i][0] * Permanent(p, n);
        }
    }
    return(d);
}

int**getMatrixForPermanent(int *matrix, int picks){
    int **p;
    p = new int*[picks];
    for (int i=0;  i<picks; i++)
        p[i] = new int[picks];
    for(int i=0; i<picks; i++){
        for(int j=0; j<picks;j++){
            if(*(matrix+i*picks+j)==1)p[i][j]=1;
            else p[i][j]=0;
        }
    }
    return p;
}