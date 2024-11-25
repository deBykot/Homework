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
    std::vector<double> P(n + 1);
    P[0] = 1;
    P[1] = x;

    for (int i = 2; i <= n; ++i) {
        P[i] = ((2 * i - 1) * x * P[i - 1] - (i - 1) * P[i - 2]) / i;
    }

    return P[n];
}

double Gauss_method(double a, double b, int n) {
    std::vector<double> nodes(n), weights(n);
    double sum = 0;

    for (int i = 0; i < n; ++i) {
        double root = cos(M_PI * (i + 0.75) / (n + 0.5));
        double root_next;
        double member_polynomial;
        double derivative_polynomial;

        do {
            member_polynomial = polinomial_root(root, n);
            derivative_polynomial = n * (polinomial_root(root, n - 1) - root * member_polynomial) / (1 - root * root);
            root_next = root;
            root = root_next - member_polynomial / derivative_polynomial;
        } while (fabs(root - root_next) > 1e-15);

        nodes[i] = root;
        // std::cout << root << std::endl;
        weights[i] = 2.0 / ((1.0 - root * root) * derivative_polynomial * derivative_polynomial);
    }

    for (int i = 0; i < n; ++i) {
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

    std::cout << "Метод трапеций для a = " << a << ", b = " << b << ":" << std::endl;

    for (int n : n_values) {
        double result = trapezium_method(a, b, n);
        std::cout << "Приближенное значение интеграла для n = " << n << ": " << result << std::endl;
    }

    a = 0.0;
    b = 3.0; 

    std::cout << std::endl << "Метод Гаусса для a = " << a << ", b = " << b << ":" << std::endl;
    
    for (int n : n_values) {
        double result = Gauss_method(a, b, n);
        std::cout << "Приближенное значение интеграла для n = " << n << ": " << result << std::endl;
    }

    return 0;
}
