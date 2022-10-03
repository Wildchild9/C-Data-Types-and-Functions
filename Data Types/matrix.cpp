//
// Created by Noah Wilder on 2022-10-03.
//

#include "matrix.h"
#include <cmath>

using namespace std;

inline bool isAlmostZero(double x) {
    const double epsilon = std::numeric_limits<double>::epsilon();/* some small number such as 1e-5 */;
    return std::abs(x) <= epsilon;
    // see Knuth section 4.2.2 pages 217-218
}




template <size_t numberOfRows, size_t numberOfColumns>
void matrix<numberOfRows, numberOfColumns>::decrementAndResetSharedReducedRowEchelonFormInstancesCounter() {
    *_sharedReducedRowEchelonFormInstancesCounter -= 1;
    _sharedReducedRowEchelonFormInstancesCounter = (unsigned int*) malloc(sizeof(unsigned int));
    *_sharedReducedRowEchelonFormInstancesCounter = 0;
}

template <size_t numberOfRows, size_t numberOfColumns>
void matrix<numberOfRows, numberOfColumns>::decrementAndResetSharedGridInstancesCounter() {
    *_sharedGridInstancesCounter -= 1;
    _sharedGridInstancesCounter = (unsigned int*) malloc(sizeof(unsigned int));
    *_sharedGridInstancesCounter = 0;
}

template <size_t numberOfRows, size_t numberOfColumns>
double& matrix<numberOfRows, numberOfColumns>::unsafeSet(int row, int column) {
//        if (*_sharedGridInstancesCounter > 0 || *_sharedReducedRowEchelonFormInstancesCounter > 0) {
//            cout << "Mutating" << endl;
//        }
    if (*_sharedGridInstancesCounter > 0) {
        decrementAndResetSharedGridInstancesCounter();
        double* tempGrid = (double*) malloc(numberOfElements() * sizeof(double));
        memcpy(tempGrid, _grid, numberOfElements() * sizeof(double));
        _grid = tempGrid;
    }
    if (*_sharedReducedRowEchelonFormInstancesCounter > 0) {
        _reducedRowEchelonFormStorage = nullptr;
        decrementAndResetSharedReducedRowEchelonFormInstancesCounter();
    }
    return _grid[row * numberOfColumns + column];
}

template <size_t numberOfRows, size_t numberOfColumns>
double matrix<numberOfRows, numberOfColumns>::unsafeGet(int row, int column) {
    return _grid[row * numberOfColumns + column];
}

template <size_t numberOfRows, size_t numberOfColumns>
void matrix<numberOfRows, numberOfColumns>::updateReducedRowEchelonForm() {
//        std::cout << "Recalculating..." << std::endl;
    _reducedRowEchelonFormNeedsUpdating = false;
    if (_reducedRowEchelonFormStorage == nullptr) {
        _reducedRowEchelonFormStorage = (double*) malloc(numberOfElements() * sizeof(double));
    }
    for (int i = 0; i < numberOfElements(); i++) {
        _reducedRowEchelonFormStorage[i] = _grid[i];
    }


    if (numberOfRows <= 1 || numberOfColumns <= 1) {
        return;
    }
    double* m = _reducedRowEchelonFormStorage;

    auto idx = [](int row, int column) {
        return row * numberOfColumns + column;
    };

    if (numberOfRows == 1 && numberOfColumns == 2) {
        m[idx(0, 1)] /= m[idx(0, 0)];
        m[idx(0, 0)] = 1;
        return;
    }


    int finishedRows = 0;
    // Not including solution column
    for (int currentColumn = 0; currentColumn < numberOfColumns - 1; ++currentColumn) {
        // Find the first non-zero value in the column
        for (int currentRow = finishedRows; currentRow < numberOfRows; currentRow++) {

            double value = m[idx(currentRow, currentColumn)];
            if (value != 0) {
                // Divide row by first non-zero value
                for (int col = currentColumn; col < numberOfColumns; col++) {
                    m[idx(currentRow, col)] /= value;
                }

                for (int row = 0; row < numberOfRows; row++) {
                    if (row == currentRow) {
                        continue;
                    }
                    double multiplier = m[idx(row, currentColumn)];
                    m[idx(row, currentColumn)] = 0;
                    for (int col = currentColumn + 1; col < numberOfColumns; col++) {
                        m[idx(row, col)] -= m[idx(currentRow, col)] * multiplier;
                        if (isAlmostZero(m[idx(row, col)])) {
                            m[idx(row, col)] = 0;
                        }
                    }
                }
                if (finishedRows != currentRow) {
                    for (int i = 0; i < numberOfColumns; i++) {
                        swap(m[idx(finishedRows, i)], m[idx(currentRow, i)]);
                    }
                }
                finishedRows++;
                break;
            }
        }
    }
};

template <size_t numberOfRows, size_t numberOfColumns>
void matrix<numberOfRows, numberOfColumns>::requiresUpdate() {
    _reducedRowEchelonFormNeedsUpdating = true;
}

//    matrix(unsigned int* sharedGridInstancesCounter, unsigned int* sharedReducedRowEchelonFormInstancesCounter) {
//        _sharedGridInstancesCounter = sharedGridInstancesCounter;
//        _sharedReducedRowEchelonFormInstancesCounter = sharedReducedRowEchelonFormInstancesCounter;
//    }
//
//    static matrix<numberOfRows, numberOfColumns> sharedReducedRowEchelonFormMatrix(double* reducedRowEchelonFormStorage, unsigned int* sharedReducedRowEchelonFormInstancesCounter) {
//
//        *sharedReducedRowEchelonFormInstancesCounter += 2;
//
//        matrix<numberOfRows, numberOfColumns> m = matrix<numberOfRows, numberOfColumns>(sharedReducedRowEchelonFormInstancesCounter, sharedReducedRowEchelonFormInstancesCounter);
//
//        m._grid = reducedRowEchelonFormStorage;
//        m._reducedRowEchelonFormStorage = reducedRowEchelonFormStorage;
//
//        m._reducedRowEchelonFormNeedsUpdating = false;
//
//        return m;
//    }
//
//    static matrix<numberOfRows, numberOfColumns> sharedGridMatrix(double* grid, unsigned int* sharedGridInstancesCounter) {
//
//        *sharedGridInstancesCounter += 1;
//        unsigned int* sharedReducedRowEchelonFormInstancesCounter = (unsigned int*) malloc(sizeof(unsigned int));
//        *sharedReducedRowEchelonFormInstancesCounter = 0;
//
//        matrix<numberOfRows, numberOfColumns> m = matrix<numberOfRows, numberOfColumns>(sharedGridInstancesCounter, sharedReducedRowEchelonFormInstancesCounter);
//
//        m._grid = grid;
//        m._reducedRowEchelonFormStorage = nullptr;
//
//        m._reducedRowEchelonFormNeedsUpdating = true;
//
//        return m;
//    }
//
//    static matrix<numberOfRows, numberOfColumns> sharedMatrix(double* grid, double* reducedRowEchelonFormStorage, unsigned int* sharedGridInstancesCounter, unsigned int* sharedReducedRowEchelonFormInstancesCounter) {
//
//        *sharedGridInstancesCounter += 1;
//        *sharedReducedRowEchelonFormInstancesCounter += 1;
//
//        matrix<numberOfRows, numberOfColumns> m = matrix<numberOfRows, numberOfColumns>(sharedGridInstancesCounter, sharedReducedRowEchelonFormInstancesCounter);
//
//        m._grid = grid;
//        m._reducedRowEchelonFormStorage = reducedRowEchelonFormStorage;
//
//        m._reducedRowEchelonFormNeedsUpdating = false;
//
//        return m;
//    }

template <size_t numberOfRows, size_t numberOfColumns>
void matrix<numberOfRows, numberOfColumns>::initializeSharedInstancesCounters() {
    _sharedGridInstancesCounter = (unsigned int*) malloc(sizeof(unsigned int));
    *_sharedGridInstancesCounter = 0;
    _sharedReducedRowEchelonFormInstancesCounter = (unsigned int*) malloc(sizeof(unsigned int));
    *_sharedReducedRowEchelonFormInstancesCounter = 0;
}

template <size_t numberOfRows, size_t numberOfColumns>
matrix<numberOfRows, numberOfColumns>::matrix(double* grid, double* reducedRowEchelonFormStorage, bool reducedRowEchelonFormNeedsUpdating, unsigned int* sharedReducedRowEchelonFormInstancesCounter, unsigned int* sharedGridInstancesCounter) {
    _grid = grid;
    _reducedRowEchelonFormStorage = reducedRowEchelonFormStorage;
    _reducedRowEchelonFormNeedsUpdating = reducedRowEchelonFormNeedsUpdating;
    _sharedReducedRowEchelonFormInstancesCounter = sharedReducedRowEchelonFormInstancesCounter;
    _sharedGridInstancesCounter = sharedGridInstancesCounter;
}


template <size_t numberOfRows, size_t numberOfColumns>
matrix<numberOfRows, numberOfColumns>::matrix(const matrix<numberOfRows, numberOfColumns> &otherMatrix) {
//        std::cout << "Copying..." << std::endl;
    if (otherMatrix._reducedRowEchelonFormNeedsUpdating) {
        *(otherMatrix._sharedGridInstancesCounter) += 1;
        _sharedGridInstancesCounter = otherMatrix._sharedGridInstancesCounter;
        _sharedReducedRowEchelonFormInstancesCounter = (unsigned int*) malloc(sizeof(unsigned int));
        *_sharedReducedRowEchelonFormInstancesCounter = 0;
        _grid = otherMatrix._grid;
        _reducedRowEchelonFormStorage = nullptr;
        _reducedRowEchelonFormNeedsUpdating = true;

    } else {
        *(otherMatrix._sharedGridInstancesCounter) += 1;
        *(otherMatrix._sharedReducedRowEchelonFormInstancesCounter) += 1;
        _grid = otherMatrix._grid;
        _reducedRowEchelonFormStorage = otherMatrix._reducedRowEchelonFormStorage;
        _reducedRowEchelonFormNeedsUpdating = otherMatrix._reducedRowEchelonFormNeedsUpdating;
        _sharedReducedRowEchelonFormInstancesCounter = otherMatrix._sharedReducedRowEchelonFormInstancesCounter;
        _sharedGridInstancesCounter = otherMatrix._sharedGridInstancesCounter;
//            matrix<numberOfRows, numberOfColumns> newMatrix = matrix<numberOfRows, numberOfColumns>::sharedMatrix(otherMatrix._grid, otherMatrix._reducedRowEchelonFormStorage, otherMatrix._sharedGridInstancesCounter, otherMatrix._sharedReducedRowEchelonFormInstancesCounter);
//            _grid = newMatrix._grid;
//            _reducedRowEchelonFormStorage = newMatrix._reducedRowEchelonFormStorage;
//            _reducedRowEchelonFormNeedsUpdating = newMatrix._reducedRowEchelonFormNeedsUpdating;
//            _sharedReducedRowEchelonFormInstancesCounter = newMatrix._sharedReducedRowEchelonFormInstancesCounter;
//            _sharedGridInstancesCounter = newMatrix._sharedGridInstancesCounter;
    }

}
template <size_t numberOfRows, size_t numberOfColumns>
matrix<numberOfRows, numberOfColumns>::matrix() {
    int n = numberOfRows * numberOfColumns;
    _grid = (double*) malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        _grid[i] = 0.0;
    }
    _reducedRowEchelonFormStorage = nullptr;
    _reducedRowEchelonFormNeedsUpdating = true;
    initializeSharedInstancesCounters();
}

template <size_t numberOfRows, size_t numberOfColumns>
matrix<numberOfRows, numberOfColumns>::matrix(double initialValue) {
    int n = numberOfRows * numberOfColumns;
    _grid = (double*) malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        _grid[i] = initialValue;
    }
    _reducedRowEchelonFormStorage = nullptr;
    _reducedRowEchelonFormNeedsUpdating = true;
    initializeSharedInstancesCounters();
}

template <size_t numberOfRows, size_t numberOfColumns>
matrix<numberOfRows, numberOfColumns>::matrix(std::initializer_list<std::initializer_list<double>> list) {
    _grid = (double*) malloc(numberOfRows * numberOfColumns * sizeof(double));
    _reducedRowEchelonFormStorage = nullptr;
    _reducedRowEchelonFormNeedsUpdating = true;
    initializeSharedInstancesCounters();

    assert(list.size() == numberOfRows && "Cannot initialize matrix with an initializer list with different dimensions");
    int i = 0, j = 0;
    for (const auto& row : list) {
        assert(row.size() == numberOfColumns && "Cannot initialize matrix with an initializer list with different dimensions");

        if (i >= numberOfRows) {
            break;
        }
        for (const auto& value : row) {
            if (j >= numberOfColumns) {
                break;
            }
            unsafeSet(i, j) = value;
            j++;
        }
        j = 0;
        i++;
    }
}

template <size_t numberOfRows, size_t numberOfColumns>
matrix<numberOfRows, numberOfColumns>::~matrix() {

    if (*_sharedGridInstancesCounter == 0) {
//            if (_grid != nullptr) {
//                free(_grid);
//            }
        _sharedGridInstancesCounter = nullptr;
    } else {
        *_sharedGridInstancesCounter -= 1;
        _sharedGridInstancesCounter = nullptr;
    }
    if (*_sharedReducedRowEchelonFormInstancesCounter == 0) {
//            if (_reducedRowEchelonFormStorage != nullptr) {
//                free(_reducedRowEchelonFormStorage);
//            }
//            free(_sharedReducedRowEchelonFormInstancesCounter);
        _sharedReducedRowEchelonFormInstancesCounter = nullptr;
    } else {
        *_sharedReducedRowEchelonFormInstancesCounter -= 1;
        _sharedReducedRowEchelonFormInstancesCounter = nullptr;
    }
}


template <size_t numberOfRows, size_t numberOfColumns>
constexpr int matrix<numberOfRows, numberOfColumns>::numberOfElements() {
    return numberOfRows * numberOfColumns;
}

//    double* operator[] (int idx) {
//        return grid[idx];
//    }

template <size_t numberOfRows, size_t numberOfColumns>
double matrix<numberOfRows, numberOfColumns>::get(int row, int column) {
    assert(row >= 0 && row < numberOfRows && "Index out of range");
    assert(column >= 0 && row < numberOfColumns && "Index out of range");
    return unsafeGet(row, column);
}

template <size_t numberOfRows, size_t numberOfColumns>
void matrix<numberOfRows, numberOfColumns>::set(int row, int column, double newValue) {
    assert(row >= 0 && row < numberOfRows && "Index out of range");
    assert(column >= 0 && row < numberOfColumns && "Index out of range");
    requiresUpdate();
    unsafeSet(row, column) = newValue;
}

template <size_t numberOfRows, size_t numberOfColumns>
matrix<numberOfRows, numberOfColumns> matrix<numberOfRows, numberOfColumns>::reducedRowEchelonForm() {
    if (_reducedRowEchelonFormNeedsUpdating) {
        updateReducedRowEchelonForm();
    }

    _sharedReducedRowEchelonFormInstancesCounter += 2;

    return matrix<numberOfRows, numberOfColumns>(_reducedRowEchelonFormStorage, _reducedRowEchelonFormStorage, false, _sharedReducedRowEchelonFormInstancesCounter, _sharedReducedRowEchelonFormInstancesCounter);
}

template <size_t numberOfRows, size_t numberOfColumns>
void matrix<numberOfRows, numberOfColumns>::print() {
    string rows[numberOfRows];

    for (int col = 0; col < numberOfColumns; col++) {
        unsigned long maxWidth = 0;
        string rowStrs[numberOfRows];
        for (int row = 0; row < numberOfRows; row++) {
            const double value = unsafeGet(row, col);
            string str;
            char buffer[200];
            if (isAlmostZero(fmod(value, 1.0))) {
                str = std::to_string((int) round(value));
            } else if (isAlmostZero(fmod(value * 10, 1.0))) {
                sprintf(buffer, "%.1f", value);
                str = buffer;
            } else {
                const double roundedValue = round(value * 100) / 100;
                sprintf(buffer, "%.2f", roundedValue);
                str = buffer;
            }

            maxWidth = max(maxWidth, str.length());
            rowStrs[row] = str;
        }

        string spacer = col == numberOfColumns ? "" : " ";
        for (int row = 0; row < numberOfRows; row++) {
            rows[row] += std::string(maxWidth - rowStrs[row].length(), ' ') + rowStrs[row] + spacer;
        }
    }
    for (const string &row : rows) {
        std::cout << row << std::endl;
    }
}

template <size_t numberOfRows, size_t numberOfColumns>
void matrix<numberOfRows, numberOfColumns>::fill(double value) {
    for (int i = 0; i < numberOfRows; i++) {
        for (int j = 0; j < numberOfColumns; j++) {
            unsafeSet(i, j) = value;
        }
    }
}

template <size_t numberOfRows, size_t numberOfColumns>
void matrix<numberOfRows, numberOfColumns>::fillRow(int row, double value) {
    assert(row >= 0 && row < numberOfRows && "Index out of bounds");
    for (int i = 0; i < numberOfColumns; i++) {
        unsafeSet(row, i) = value;
    }
}

template <size_t numberOfRows, size_t numberOfColumns>
void matrix<numberOfRows, numberOfColumns>::fillColumn(int column, double value) {
    assert(column >= 0 && column < numberOfColumns && "Index out of bounds");
    for (int i = 0; i < numberOfRows; i++) {
        unsafeSet(i, column) = value;
    }
}
