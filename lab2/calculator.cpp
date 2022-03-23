#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <math.h>

using namespace std;

class Calculator 
{
    public:
        double oper1,oper2;
        char op;

        Calculator(double a, char b, double c) : oper1(a),op(b),oper2(c){};
        Calculator() : oper1(0),op(0),oper2(0){};
        void initialize(double a, char b, double c) {
          oper1 = a; op = b; oper2 = c; return;}
        double solve()
        {
            switch(op)
            {
                case '+' : return (oper1+oper2);
                case '-' : return (oper1-oper2);
                case '*' : return (oper1*oper2);
                case '/' : return (oper1/oper2);
                case '^' : return (pow(oper1,oper2));
                case 'r' : return (pow(oper2,1/oper1));
            }
        }
};

string expression;
deque<double> numbers;
deque<char> operators;

void addsub() //Does adding and subtracting.
{
        Calculator calc;
        double temp = numbers.at(0);

    while (true)
    {
        if (operators.at(0) == '+' || operators.at(0) == '-')
        {
                numbers.erase(numbers.begin());
                calc.initialize(temp ,operators.at(0), numbers.at(0));
                temp = calc.solve();

                if (operators.size() == 1)
                {
                    operators.clear();
                    numbers.at(0) = temp;
                    return;
                }

                else
                operators.erase(operators.begin());
        }
    }
}

void multidiv() //Does multiplication and division.
{
        Calculator calc;
        double temp;
        unsigned int i;

    for (i=0; i<operators.size(); i++)
    {
        if (operators.at(i) == '*' || operators.at(i) == '/')
        {
            calc.initialize(numbers.at(i) ,operators.at(i), numbers.at(i+1));
            temp = calc.solve();

            if (operators.size() == 1)
                {
                    operators.clear();
                    numbers.at(0) = temp;
                    return;
                }

                else
                {
                    operators.erase(operators.begin()+i);
                    numbers.erase(numbers.begin()+i+1);
                    numbers.at(i) = temp;
                }
            --i;
        }
    }
    addsub();
}

void calculate() //Does powers and roots.
{
        Calculator calc;
        double temp;
        unsigned int i;

    for (i=0; i<operators.size(); i++)
    {
        if (operators.at(i) == '^' || operators.at(i) == 'r')
        {
            calc.initialize(numbers.at(i) ,operators.at(i), numbers.at(i+1));
            temp = calc.solve();

            if (operators.size() == 1)
                {
                    operators.clear();
                    numbers.at(0) = temp;
                    return;
                }

                else
                {
                    operators.erase(operators.begin()+i);
                    numbers.erase(numbers.begin()+i+1);
                    numbers.at(i) = temp;
                }
            --i;
        }
    }
    multidiv();
}

void express()
{
    cout << "'+' to add\n'-' to subtract\n'*' to multiply\n'/' to divide\n'^' for powers\n'r' for roots\n\n";
    cout << "Enter your expression:\n";
    cin >> expression;
    return;
}

void extract() //Turns an expression into a list of numbers and operators.
{
    vector<int> numbers;
    unsigned int i,k; int temp=0;

    for (i=0; i<=expression.size(); i++)
    {
        if (expression[i]>='0' && expression[i] <='9') numbers.push_back(expression[i]-'0');
        else {
                operators.push_back(expression[i]);
                for (k=0; k<numbers.size(); k++) temp+=numbers.at(k)*pow(10,(numbers.size()-1-k));
                numbers.push_back(temp);
                numbers.clear();
                temp=0;
             }
    }
    operators.erase(operators.end());
    return;
}

int main ()
{
    express();
    extract();
    calculate();

    cout << "Final answer: " << numbers.at(0) << endl << endl;

    return 0;
}
