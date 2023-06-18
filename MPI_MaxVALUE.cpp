#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    const int n = 1000;
    int max_val = 0;

    MPI_Init(&argc, &argv);

    int r, s;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &s);

    const int local_n = n / s;

    vector<int> a(local_n);

    for (int i = 0; i < local_n; i++)
    {
        a[i] = r * local_n + i;
    }

    int max = a[0];

    for (int i = 1; i < local_n; i++) {
        if (a[i] > max) {
            max = a[i];
        }
    }

    if (r != 0) {
        MPI_Send(&max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else {
        max_val = max;
        for (int i = 1; i < s; i++) {
            int temp;
            MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (temp > max_val) {
                max_val = temp;
            }
        }
    }

    if (r == 0) {
        cout << "Maximum value: " << max_val << endl;
    }

    MPI_Finalize();

    return 0;
}
