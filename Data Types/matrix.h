//
// Created by Noah Wilder on 2022-10-03.
//

#ifndef C___DATA_TYPES_AND_FUNCTIONS_MATRIX_H
#define C___DATA_TYPES_AND_FUNCTIONS_MATRIX_H

#include <iostream>
using namespace std;

template <size_t numberOfRows, size_t numberOfColumns>
struct matrix {
private:
//    double grid[numberOfRows * numberOfColumns];
    double* _grid;
    double* _reducedRowEchelonFormStorage;
    bool _reducedRowEchelonFormNeedsUpdating;
    unsigned int* _sharedReducedRowEchelonFormInstancesCounter;
    unsigned int* _sharedGridInstancesCounter;


    void decrementAndResetSharedReducedRowEchelonFormInstancesCounter();
    void decrementAndResetSharedGridInstancesCounter();

    double& unsafeSet(int row, int column);
    double unsafeGet(int row, int column);

    void updateReducedRowEchelonForm();

    void requiresUpdate();

    void initializeSharedInstancesCounters();

    matrix(double* grid, double* reducedRowEchelonFormStorage, bool reducedRowEchelonFormNeedsUpdating, unsigned int* sharedReducedRowEchelonFormInstancesCounter, unsigned int* sharedGridInstancesCounter);
public:

    matrix(const matrix<numberOfRows, numberOfColumns> &otherMatrix);

    matrix();

    explicit matrix(double initialValue);

    matrix(std::initializer_list<std::initializer_list<double>> list);

    ~matrix();

    constexpr int numberOfElements();

    double get(int row, int column);

    void set(int row, int column, double newValue);

    matrix<numberOfRows, numberOfColumns> reducedRowEchelonForm();

    void print();

    void fill(double value);
    void fillRow(int row, double value);
    void fillColumn(int column, double value);
};

#endif //C___DATA_TYPES_AND_FUNCTIONS_MATRIX_H
