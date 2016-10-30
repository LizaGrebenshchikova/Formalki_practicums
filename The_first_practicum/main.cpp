/*
Даны α, буква x и натуральное число k. Вывести, есть ли в языке L слова,
содержащие суффикс x^k .
WARNING: сначала вводим x, затем k. Потом вводим регулярное выражение.
*/
#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <algorithm>
#include <string>
#include <cassert>

char eps = '1';

bool checkregexcorrection (std::string expression) { // проверяет корректность входного выражения
    size_t stacksize = 0;
    std::string::iterator it = expression.begin();
    std::string::iterator it_end = expression.end();
    while (it != it_end) {
        if (*it == '.' || *it=='+') {
            assert (stacksize > 0);
            --stacksize;
        } else {
            if (*it != '*')
                ++stacksize;
        }
        ++it;
    }
    return (stacksize == 1);
}

bool is_symb (char symbol) {
    return symbol == eps || symbol == 'a' || symbol == 'b' ||symbol == 'c';
}

class Element {
public:
    std::vector<bool>a; // a[i] = true, если в языке есть слово l, содержащее слово x^i.
    std::vector<bool>b; // b[i] = true, если в языке есть слово x^i.
    Element(unsigned int k) {
        a.resize(k+1);
        b.resize(k+1);
        for(unsigned int i = 0; i <= k; ++i) {
            a[i] = false;
            b[i] = false;
        }
    }
    Element(char c1, char x, unsigned int k) { // конструктор, если очередной считанный символ - буква
        a.resize(k+1);
        b.resize(k+1);
        if (c1 == x) {
            a[0] = true; a[1] = true;
            b[0] = false; b[1] = true;
            for ( unsigned int i = 2; i <= k; ++i) {
                a[i] = false;
                b[i] = false;
            }
        } else if (c1 == eps ) {
            a[0] = true;
            b[0] = true;
            for(unsigned int i = 1; i <= k; ++i) {
                a[i] = false;
                b[i] = false;
            }
        } else if(c1 != x) {
            a[0] = true; a[1] = false;
            b[0] = false; b[1] = false;
            for (unsigned int i = 2; i <= k; ++i) {
                a[i] = false;
                b[i] = false;
            }
        }
    }
};
Element plus_ (Element& x, Element& y, unsigned int k) {
    Element res(k);
    for(unsigned int i = 0; i <= k; ++i) {
        res.a[i] = x.a[i] || y.a[i];
        res.b[i] = x.b[i] || y.b[i];
    }
    return res;
}
Element multip (Element& x, Element& y, unsigned int k) {
    Element res(k);
    for(unsigned int i = 0; i <= k; ++i) {
        if (y.a[i] == true) {
            res.a[i] = true;
        }

        for (unsigned int j = 0; j <= i; ++j) {
            if((y.b[j] == true) && (x.a[i-j] == true)) {
                res.a[i] = true;
            }
        }

        for (unsigned int j = 0; j <= i; ++j) {
            if((y.b[j] == true) && (x.b[i-j] == true)) {
                res.b[i] = true;
            }

        }
    }
    return res;
}
Element star (Element& x, unsigned int k, char x__) {
    Element res(eps, x__, k);
    for (unsigned int i = 1 ; i <= k+1; ++i) {
        Element tmp(eps, x__, k);
        for(unsigned int j = 1; j <= i; ++j) {
            tmp = multip (tmp, x, k);
        }
        res = plus_(res, tmp, k);
    }
    return res;
}

int main() {
    std::set<char> Operators; // множество возможных операторов
    Operators.insert('+');
    Operators.insert('*');
    Operators.insert('.');
    char x__; // x из условия
    int k; // k из условия
    std::cin>>x__;
    std::cin>>k;

    std::stack<Element>OurStack; // стек, при помощи которого будем осуществлять все операции

    std::string expression;
    std::cin>>expression;
    std::string::iterator it = expression.begin();
    std::string::iterator it_end = expression.end();
    if ( !checkregexcorrection(expression) ) {
        std::cout << "Wrong input" << std::endl;
        return 0;
    }
    while (it != it_end) {
        if (Operators.find(*it) != Operators.end()) {
            switch (*it) {
                case '+':
                {
                    Element y1 = OurStack.top();
                    OurStack.pop();
                    Element x1 = OurStack.top();
                    OurStack.pop();
                    Element res1 = plus_(x1, y1, k);
                    OurStack.push(res1);
                    break;
                }
                case '*':
                {
                    Element x3 = OurStack.top();
                    OurStack.pop();
                    Element res3 = star(x3, k, x__);
                    OurStack.push(res3);
                    break;
                }
                case '.':
                {
                    Element y2 = OurStack.top();
                    OurStack.pop();
                    Element x2 = OurStack.top();
                    OurStack.pop();
                    Element res2 = multip(x2, y2, k);
                    OurStack.push(res2);
                    break;
                }
                default:
                    std::cout << "Ups, you have some problems with operators\n";
            }
        } else {
            if(is_symb(*it)) {
            Element tmp(*it, x__, k);
            OurStack.push(tmp);
            }
        }
        ++it;
    }
    if (OurStack.top().a[k] == true) {
        std::cout<<"YES";
    } else {
        std::cout<<"NO";
    }
    return 0;
}
