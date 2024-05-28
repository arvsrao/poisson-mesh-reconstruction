Notes taken while reading ["Poisson Surface Reconstruction" by Kazhdan et al. 2006.](papers/poisson-surface-reconstruction.pdf) 

## Compute $\nabla (\chi_M \ast \eta_{\epsilon})$

Let $(M^n, \partial M)$ be a compact Riemannian manifold embedded in $\mathbb{R}^{n}$ and $\eta_{\epsilon}$ is a unit integral smoothing function with support on $ B_{\epsilon}(0) \subset \mathbb{R}^n$​. Lets try to calculate the smoothed characteristic function of $M$
$$
\begin{align*}
(\chi_M \ast \eta_{\epsilon}) (y) & = \int_{\mathbb{R}^n} \chi_M \ \eta_{\epsilon}(y - x) dx \\
                                  & = \int_{M} \eta_{\epsilon}(y - x) dV \\
                                  & = \int_{M \cap  B_{\epsilon}(y)} \eta_{\epsilon}(y - x) dV .\\
\end{align*}
$$
We end up with an integral of $\eta_{\epsilon}$ over its support on $M$. Naturally, when the support ball $B_{\epsilon}(y)$ is completely contained in $M$, the smoothed characteristic function is $1$,  because $\eta_{\epsilon}$ has unit integral. So then only in an $\epsilon-$neighborhood of $\partial M$ does the function get interesting. Lets summarize.
$$
(\chi_M \ast \eta_{\epsilon}) (y) =
\begin{cases}
 1 & \text{if } B_{\epsilon}(y) \subset \text{int}(M) \\
 \int_{M \cap  B_{\epsilon}(y)} \eta_{\epsilon}(y - x) dV & \text{o.w.} \\
\end{cases}
$$


The corresponding gradient of $\chi_M \ast \eta_{\epsilon}$​ is 
$$
\nabla (\chi_M \ast \eta_{\epsilon}) (y) =
\begin{cases}
  0 & \text{if } B_{\epsilon}(y) \subset \text{int}(M) \\
 \int_{M \cap  B_{\epsilon}(y)} \nabla_y \eta_{\epsilon}(y - x) dV & \text{o.w.} \\
\end{cases}
$$
As a vector field $\nabla (\chi_M \ast \eta_{\epsilon})$ is zero in the interior of $M$​ some $\epsilon$  away from the boundary and non-zero within some $\epsilon$ of $\partial M$. Taking a closer look at the region near the boundary, we can exchange the variable in which the gradient is taken.
$$
\begin{equation*}
\frac{\partial }{\partial y_i} \eta_{\epsilon}(\underbrace{y - x}_{u}) = \frac{\partial }{\partial u_i}  \eta_{\epsilon}(u_1, \cdots, u_i, \cdots ,u_n) \cancelto{1}{\frac{\partial u_i}{\partial y_i}} \\
\end{equation*}
$$

$$
\begin{equation*}
\frac{\partial }{\partial x_i} \eta_{\epsilon}(\underbrace{y - x}_{u}) = \frac{\partial }{\partial u_i}  \eta_{\epsilon}(u_1, \cdots, u_i, \cdots ,u_n) \cancelto{-1}{\frac{\partial u_i}{\partial y_i}} \\ 
\end{equation*}
$$
Therefore, 
$$
\begin{equation*}
\frac{\partial }{\partial y_i} \eta_{\epsilon}(y - x) = - \frac{\partial }{\partial x_i} \eta_{\epsilon}(y - x)
\end{equation*}
$$
And
$$
\begin{equation*}
\int_{M \cap B_{\epsilon}} \nabla_y \eta_{\epsilon}(y - x) dV = -\int_{M \cap B_{\epsilon}} \nabla_x \eta_{\epsilon}(y - x) dV \\
\end{equation*}
$$

The minus sign comes into play later when the [Gauss-Green theorem][1] is applied. In order to use that theorem the region of integration, $\text{int}(M) \cap B_{\epsilon}$ must be an open set in $\mathbb{R}^n$. Let's further stipulating that we can choose an $\epsilon > 0$ small enough so that $\text{int}(M) \cap B_{\epsilon}$​​ is open.

Then by the [Gauss-Green theorem][1], 
$$
\begin{align*}
\int_{M \cap B_{\epsilon}} \nabla \eta_{\epsilon}(y - x) dV &= \int_{\partial (M \cap B_{\epsilon})} \eta_{\epsilon}(y - x) N(x) dS \\
&= \int_{\partial M \cap B_{\epsilon}} \eta_{\epsilon}(y - x) N(x) dS + \cancelto{0}{\int_{ M \cap \partial B_{\epsilon}} \eta_{\epsilon}(y - x) N(x) dS} \ ,\\
\end{align*}
$$

where $N$ is the outward point unit normal. Since $\eta_{\epsilon}$ is $0$ on the boundary of its support, $\partial B_{\epsilon}$, the second integral vanishes, leaving only the integral along a portion of $\partial M$.
$$
\begin{align*}
\int_{M \cap B_{\epsilon}} \nabla \eta_{\epsilon}(y - x) dV
&= \int_{\partial M \cap B_{\epsilon}} \eta_{\epsilon}(y - x) N(x) dS \\
\end{align*}
$$
Redefining $N(x)$ to be the inward point normal, the left-hand side can absorb the minus sign. 
$$
\begin{align*}
(\chi_M \ast \eta_{\epsilon}) (y) &= \int_{M \cap B_{\epsilon}} \nabla_y \eta_{\epsilon}(y - x) dV \\
&= -\int_{M} \nabla_x \eta_{\epsilon}(y - x) dV \\
&= \int_{\partial M \cap B_{\epsilon}} \eta_{\epsilon}(y - x) N(x) dS \\
\end{align*}
$$



## Problem Discretization 

Use an [adaptive octree][2] to accurately represent the implicit function of the surface, $\chi_M$. 

[1]: https://en.wikipedia.org/wiki/Divergence_theorem#For_bounded_open_subsets_of_Euclidean_space
[2]: https://observablehq.com/@2talltim/spatial-data-structures-octrees-bsp-and-k-d-trees

 
