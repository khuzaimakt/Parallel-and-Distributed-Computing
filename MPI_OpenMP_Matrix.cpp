#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <mpi.h>


using namespace std;

void merge_sort(int* a, int l, int r) {
    if (l >= r) {
        return;
    }
    int m = l + (r - l) / 2;
#pragma omp parallel sections
    {
#pragma omp section
        {
            merge_sort(a, l, m);
        }
#pragma omp section
        {
            merge_sort(a, m + 1, r);
        }
    }
    int* temp = (int*)malloc((r - l + 1) * sizeof(int));
    int i = l, j = m + 1, k = 0;
    while (i <= m && j <= r) {
        if (a[i] > a[j]) {
            temp[k++] = a[i++];
        }
        else {
            temp[k++] = a[j++];
        }
    }
    while (i <= m) {
        temp[k++] = a[i++];
    }
    while (j <= r) {
        temp[k++] = a[j++];
    }
    for (i = l; i <= r; i++) {
        a[i] = temp[i - l];
    }
    free(temp);
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int s, r;
    MPI_Comm_size(MPI_COMM_WORLD, &s);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    srand(time(NULL));

    int m, n;
    m = 5;
    n = 4;

    int* mat = (int*)malloc(m * n * sizeof(int));

    for (int i = 0; i < m * n; i++)
    {
        mat[i] = rand() % 100;
    }

    int local_r = m / s;
    int* row = (int*)malloc(n * sizeof(int));
    MPI_Scatter(mat, local_r * n, MPI_INT, row, local_r * n, MPI_INT, 0, MPI_COMM_WORLD);

    merge_sort(row, 0, n - 1);

    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += row[i];
    }
    cout << "Sum at rank " << r << " is: " << sum << endl << endl;
    int* sorted_m = NULL;
    if (r == 0) {
        sorted_m = (int*)malloc(m * n * sizeof(int));
    }
    MPI_Gather(row, local_r* n, MPI_INT, sorted_m, local_r* n, MPI_INT, 0, MPI_COMM_WORLD);

    
    int total_sum = 0;
    MPI_Reduce(&sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (r == 0) 
    {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                cout << mat[i * n + j] << " ";
            }
            cout << endl<<endl;
        }

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                cout << sorted_m[i * n + j] << " ";
            }
            cout << endl << endl;
        }

        cout << total_sum << endl;
    }

    free(mat);
    free(row);
    if (r == 0) {
        free(sorted_m);
    }


    MPI_Finalize();

    return 0;
}

