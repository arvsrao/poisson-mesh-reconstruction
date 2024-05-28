#include "fd_partial_derivative.h"

void fd_partial_derivative(
  const int nx,
  const int ny,
  const int nz,
  const double h,
  const int dir,
  SparseMatrixType& D)
{
    // Eigen SparseMatrix reference: https://eigen.tuxfamily.org/dox/group__SparseQuickRefPage.html
    int ndx = nx - (dir == 0 ? 1 : 0);
    int ndy = ny - (dir == 1 ? 1 : 0);
    int ndz = nz - (dir == 2 ? 1 : 0);

    auto gridIdx= [&](int i, int j, int k) { return i + nx*(j + k * ny); };
    auto derIdx = [&](int i, int j, int k) { return i + ndx*(j + k * ndy); };

    auto nextGridIdx = [&]( int i, int j, int k) {
            switch (dir) {
                case 0:
                    return gridIdx(i+1, j, k);
                case 1:
                    return gridIdx(i, j+1, k);
                default:
                    return gridIdx(i, j, k+1);
            }
    };

    // loop over the derivative nodes
    int idx, prevIdx, nextIdx;
    for (int i = 0; i < ndx; i++) {
        for (int j = 0; j < ndy; j++) {
            for (int k = 0; k < ndz; k++) {

                idx = derIdx(i, j, k); // index in the derivative grid space
                D.insert(idx, gridIdx(i,j,k)) = -1;
                D.insert(idx, nextGridIdx(i,j,k)) = 1;
            }
        }
    }

    D /= h;
}
