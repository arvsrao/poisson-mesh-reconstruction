#include "fd_grad.h"
#include "fd_partial_derivative.h"
#include <iostream>

void fillGradientMatrix(int rowStart, SparseMatrixType& G, SparseMatrixType& D) {
    for (int j = 0; j < D.outerSize(); ++j) {
        for (SparseMatrixType::InnerIterator it(D, j); it; ++it) {
            G.insert(it.row() + rowStart, it.col()) = it.value();
        }
    }
}

void fd_grad(
  const int nx,
  const int ny,
  const int nz,
  const double h,
  SparseMatrixType& G) {
    // Eigen SparseMatrix reference: https://eigen.tuxfamily.org/dox/group__SparseQuickRefPage.html

    SparseMatrixType
            Dx((nx - 1) * ny * nz, nx * ny * nz),
            Dy(nx * (ny - 1) * nz, nx * ny * nz),
            Dz(nx * ny * (nz - 1),nx * ny * nz);

    fd_partial_derivative(nx, ny, nz, h, 0, Dx);
    fd_partial_derivative(nx, ny, nz, h, 1, Dy);
    fd_partial_derivative(nx, ny, nz, h, 2, Dz);

    fillGradientMatrix(0, G, Dx);
    fillGradientMatrix(Dx.rows(), G, Dy);
    fillGradientMatrix(Dx.rows() + Dy.rows(), G, Dz);
}
