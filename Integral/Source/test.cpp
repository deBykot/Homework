#include <iostream>
#include <cmath>
#include <vector>



double function(double x) {
    return 1.0 / ((25 * x * x + 1) * sqrt(3 * x - x * x));
}

double trapezium_method(double a, double b, int n) {
    double height = (b - a) / n; 
    double sum = 0.5 * (function(a) + function(b));

    for (int i = 1; i < n; ++i) {
        double x = a + i * height;
        sum += function(x);
    }

    return sum * height;
}
double polinomial_root(double x, int n) {
    int i;
    double *P;
    P = (double*)malloc(n*sizeof(double));

    for (i = 0; i <= n; i++) {
        P[0] = 1;
        P[1] = x;
        P[i] = ((2 * i - 1) * x * P[i - 1] - (i - 1) * P[i - 2]) / i;
    }

    return P[n];
}

double Gauss_method(double a, double b, int n) {
    int i;
    double nodes[n], weights[n], root, root_next, member_polynomial,pred_member_polynomial, derivative_polynomial;
    double sum = 0;

    //узлы и весы
    for (i = 1; i <= n; i++) {
        root = cos(M_PI * (i - 0.25) / (n + 0.5));
        root_next = root + 1.0;
        member_polynomial = 1.0;

        while (fabs(root - root_next) > 1e-10) {
            pred_member_polynomial = member_polynomial;
            member_polynomial = polinomial_root(root, i);

            derivative_polynomial = n * (pred_member_polynomial - member_polynomial * root) / (1 - root * root);
            root_next = root;
            root = root_next - member_polynomial / derivative_polynomial;
        }
        nodes[i - 1] = root;
        weights[i - 1] = 2.0 / ((1.0 - root * root) * derivative_polynomial * derivative_polynomial);
    }
    
    //!
    for (i = 0; i < n; i++) {
        double x = ((b - a) * nodes[i] + a + b) / 2.0;
        sum += weights[i] * function(x);
    }

    sum *= (b - a) / 2.0;
    
    return sum;
}

int main() {
    double a = 1.0;
    double b = 2.0;

    int n_values[] = {16, 32, 64}; 
    std::cout << "Метод трапеций для a = 1, b = 2:" << std::endl;

    for (int n : n_values) {
        double result = trapezium_method(a, b, n);
        std::cout << "Приближенное значение интеграла для n = " << n << ": " << result << std::endl;
    }

    for (int n : n_values) {
        double result = Gauss_method(a, b, n);
        std::cout << "Приближенное значение интеграла для n = " << n << ": " << result << std::endl;
    }

    return 0;
}
