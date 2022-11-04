#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void print(const vector<vector<float>>& array) {
    cout << endl;
    for (auto & k : array) {
        for (float l : k) {
            cout << l << " ";
        }
        cout << endl;
    }
}

bool check_for_zeros(vector<vector<float>> array) {
    for(int k = 0; k < array.size(); k++) {
        if(array[k][k] == 0) {
            cout << "0 na przekątnej macierzy" << endl;
            return true;
        }
    }
    return false;
}

vector<vector<float>> to_triangular_matrix(vector<vector<float>> array) {
    for(int n = 0; n < array.size() - 1; n++) {
        float multiplier = 0;
        for (int k = n + 1; k < array.size(); k++) {
            multiplier = array[k][n] / array[n][n];
            for (int l = n; l < array[n].size(); l++) {
                array[k][l] -= (array[n][l] * multiplier);
            }
        }
    }
    return array;
}

void gause(vector<vector<float>> array) {
    vector<float> xi(array.size(), 0);

    for(long k = array.size() - 1; k >= 0; k--) {
        float b = array[k][array[k].size() - 1],
                a = array[k][k],
                sum = 0;

        for(unsigned long n = array.size() - 1; n > k; n--) {
            sum += array[k][n] * xi[n];
        }

        xi[k] = (b - sum) / a;
        cout << "x" << k << " = " << xi[k] << endl;
    }
}

vector<vector<float>> to_lower_upper_triangular_matrix(vector<vector<float>> array) {
    unsigned long size = min(array.size(), array[0].size());
    vector<vector<float>> arr(size, vector<float>(size, 0));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i != j) {
                arr[i][j] = array[i][j];
            }
        }
    }
    return arr;
}

vector<vector<float>> to_diagonal_matrix(vector<vector<float>> array) {
    unsigned long size = min(array.size(), array[0].size());

    vector<vector<float>> arr(size, vector<float>(size, 0));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                arr[i][j] = array[i][j];
            }
        }
    }
    return arr;
}

vector<vector<float>> to_reversed_matrix(vector<vector<float>> array) {
    unsigned long size = min(array.size(), array[0].size());

    vector<vector<float>> arr(size, vector<float>(size, 0));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if(array[i][j] != 0) {
                arr[i][j] = 1 / array[i][j];
            }
        }
    }

    return arr;
}

vector<vector<float>> get_result_matrix(vector<vector<float>> array) {
    vector<vector<float>> arr(array.size(), vector<float>(1, 0));

    for (int i = 0; i < array.size(); i++) {
        arr[i][0] = array[i][array[i].size() - 1];
    }

    return arr;
}

vector<vector<float>> multiply_by_value(float value, vector<vector<float>> array) {
    vector<vector<float>> arr(array.size(), vector<float>(array[0].size(), 0));

    for (int i = 0; i < array.size(); i++) {
        for (int j = 0; j < array[i].size(); j++) {
            if(array[i][j] != 0) {
                arr[i][j] = array[i][j] * value;
            }
        }
    }

    return arr;
}

vector<vector<float>> multiply_by_matrix(vector<vector<float>> multiply_by, vector<vector<float>> array) {
    vector<vector<float>> arr(array.size(), vector<float>(multiply_by[0].size(), 0));

    for (int i = 0; i < arr.size(); i++) {
        for (int j = 0; j < arr[0].size(); j++) {
            float value = 0;
            for (int k = 0; k < max(arr.size(), arr[0].size()); k++) {
                value += array[i][k] * multiply_by[k][j];
            }
            arr[i][j] = value;
        }
    }

    return arr;
}

vector<vector<float>> sum_matrix(vector<vector<float>> first_matrix, vector<vector<float>> second_matrix) {
    vector<vector<float>> arr(first_matrix.size(), vector<float>(first_matrix[0].size(), 0));

    for (int i = 0; i < first_matrix.size(); i++) {
        for (int j = 0; j < first_matrix[i].size(); j++) {
            arr[i][j] = first_matrix[i][j] + second_matrix[i][j];
        }
    }

    return arr;
}

void jacob(vector<vector<float>> array) {
    cout << endl;
    unsigned long height = array.size();
    bool testIfWeakFirstCondition = true;
    bool testIfWeakSecondCondition = false;

    for (int i = 0; i < height; i++) {
        float sum = 0;

        for (int j = 0; j < height; j++) {
            if (i != j) {
                sum += array[i][j];
            }
        }

        if (array[i][i] < sum) {
            testIfWeakFirstCondition = false;
        }
        if (array[i][i] > sum) {
            testIfWeakSecondCondition = true;
        }
    }

    if (testIfWeakFirstCondition && testIfWeakSecondCondition) {
        cout << "Słabo dominująca" << endl;
    } else {
        cout << "Blad, niespelniony warunek slabo dominujacej macierzy" << endl;
    }

    vector<vector<float>> LplusU = to_lower_upper_triangular_matrix(array);
    cout << endl << "macierz L + U: ";
    print(LplusU);

    vector<vector<float>> D = to_diagonal_matrix(array);

    vector<vector<float>> reversedD = to_reversed_matrix(D);
    cout << endl << "macierz D^-1: ";
    print(reversedD);

    vector<vector<float>> b = get_result_matrix(array);

    vector<vector<float>> multipliedD = multiply_by_value(-1.0, reversedD);

    vector<vector<float>> multipliedDLU = multiply_by_matrix(LplusU, multipliedD);

    vector<vector<float>> multipliedDB = multiply_by_matrix(b, reversedD);

    int iterations;
    cout << "Podaj ilosc iteracji: " << endl;
    cin >> iterations;

    vector<vector<float>> x = multipliedDB;

    for (int i = 0; i < iterations - 1; i++) {
        vector<vector<float>> temp = multiply_by_matrix(x, multipliedDLU);
        x = sum_matrix(temp, multipliedDB);
    }

    for (int i = 0; i < x.size(); i++) {
        cout << "X" << i << " = " << x[i][0] << endl;
    }
}

void gaussWithPivoting(vector<vector<float>> matrix) {
    int max_index;

    for (int i = 0; i < matrix[0].size(); i++) {
        max_index = i;
        for (int j = i; j < matrix.size(); j++) {
            if (matrix[j][i] > matrix[max_index][i]) {
                max_index = j;
            }
        }
        if (max_index != i) {
            for (int k = 0; k < matrix[0].size(); k++) {
                float from_value = matrix[i][k];
                float to_value = matrix[max_index][k];
                matrix[i][k] = to_value;
                matrix[max_index][k] = from_value;
            }
        }
    }

    cout << endl << "Pivoting: ";
    print(matrix);

    matrix = to_triangular_matrix(matrix);

    cout << endl << "Trójkątna: ";
    print(matrix);

    cout << endl;

    gause(matrix);
}

int main() {
    int height = 0, width;

    ifstream file;
    file.open("RURL_dane4.txt");

    if(file.is_open()) {
        file >> height;
        width = height + 1;

        vector<vector<float>> matrix(height, vector<float>(width, 0));

        int i = 0, j = 0;

        while (!file.eof()) {
            file >> matrix[i][j];
            j++;

            if(j >= width) {
                j = 0;
                i++;
            }
        }

        cout << "Wybierz rozwiązanie: (1 (Gaussa), 2 (Jacobiego), 3 (Gaussa z pivotingiem))" << endl;
        int solution_number = 0;
        cin >> solution_number;

        switch (solution_number) {
            case 1:
                if (check_for_zeros(matrix)) {
                    return 0;
                }

                print(matrix);

                matrix = to_triangular_matrix(matrix);

                print(matrix);

                cout << endl;

                gause(matrix);

                break;
            case 2:
                print(matrix);

                jacob(matrix);

                break;
            case 3:
                print(matrix);

                gaussWithPivoting(matrix);

                break;
            default:
                break;
        }
    }

    return 0;
}
