// 2016603044 김동현

#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>
using namespace std;

char operationAt[3][3];
char operationSharp[3][3];
char operationAnd[3][3];

void operation() {
	ifstream operfile;
	operfile.open("operations.txt");
	char temp;

	operfile >> temp;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			operfile >> temp;
			operationAt[i][j] = temp;
		}
	}

	operfile >> temp;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			operfile >> temp;
			operationSharp[i][j] = temp;
		}
	}
	operfile >> temp;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			operfile >> temp;
			operationAnd[i][j] = temp;
		}
	}
	operfile.close();
}

void evaluate_stack(stack<char>& numbers, char operations)
{
	int operand1, operand2;
	operand2 = (int)(numbers.top() - 'a');
	numbers.pop();
	operand1 = (int)(numbers.top() - 'a');
	numbers.pop();

	switch (operations) {
	case '@': numbers.push(operationAt[operand1][operand2]); break;
	case '#': numbers.push(operationSharp[operand1][operand2]); break;
	case '&': numbers.push(operationAnd[operand1][operand2]); break;
	}
	
}

void read_and_evaluate(istream& ins)
{
		stack<char> numbers;
		stack<char> operations;
		stack<char> reverse;
		char number;
		char symbol;

		while (ins && ins.peek() != '\n')
		{
			if (ins.peek() == 'a' || ins.peek() == 'b' || ins.peek() == 'c') {
				ins >> number;
				numbers.push(number);
			}
			else if (ins.peek() == '@' || ins.peek() == '#' || ins.peek() == '&' ||
				ins.peek() == '(' || ins.peek() == '{' || ins.peek() == '[') {
				if (operations.empty() != true) {
					ins >> symbol;
					if (symbol == '(' || symbol == '{' || symbol == '[') {
						operations.push(symbol);
					}
					else {
						if (operations.top() == '@' || operations.top() == '#' || operations.top() == '&') {
							if (symbol == '&') {
								if (operations.top() == '&') {
									numbers.push(operations.top());
									operations.pop();
									operations.push(symbol);
								}
								else {
									operations.push(symbol);
								}
							}
							else {
								numbers.push(operations.top());
								operations.pop();
								operations.push(symbol);
							}
						}
						else
							operations.push(symbol);
					}
				}
				else {
					ins >> symbol;
					operations.push(symbol);
				}
			}
			else if (ins.peek() == ')' || ins.peek() == '}' || ins.peek() == ']') {
				ins >> symbol;
				char pair = 0;
				switch (symbol) {
				case ')' : pair = '('; break;
				case '}' : pair = '{'; break;
				case ']' : pair = '['; break;
				}
				for (;;) {
					if (operations.empty() != true) {
						if (operations.top() == pair) {
							operations.pop();
							break;
						}
						else if (symbol == ')') {
							if (operations.top() == '{' || operations.top() == '[') {
								cout << "Error!" << endl;
								while (ins && ins.peek() != '\n') {
									ins.ignore();
								}
								return;
							}
							else {
								numbers.push(operations.top());
								operations.pop();
							}
						}
						else if (symbol == '}') {
							if (operations.top() == '(' || operations.top() == '[') {
								cout << "Error!" << endl;
								while (ins && ins.peek() != '\n') {
									ins.ignore();
								}
								return;
							}
							else {
								numbers.push(operations.top());
								operations.pop();
							}
						}
						else if (symbol == ']') {
							if (operations.top() == '(' || operations.top() == '{') {
								cout << "Error!" << endl;
								while (ins && ins.peek() != '\n') {
									ins.ignore();
								}
								return;;
							}
							else {
								numbers.push(operations.top());
								operations.pop();
							}
						}
					}
					else {
						cout << "Error!" << endl;
						while (ins && ins.peek() != '\n') {
							ins.ignore();
						}
						return;
					}
				}
			}
			else
				ins.ignore();
		}
		while (operations.empty() != true) {
			numbers.push(operations.top());
			operations.pop();
		}

		while (numbers.empty() != true) {
			reverse.push(numbers.top());
			numbers.pop();
		}

		while (reverse.empty() != true) {
			if (reverse.top() == 'a' || reverse.top() == 'b' || reverse.top() == 'c') {
				numbers.push(reverse.top());
				reverse.pop();
			}
			else if (reverse.top() == '@' || reverse.top() == '#' || reverse.top() == '&') {
				evaluate_stack(numbers, reverse.top());
				reverse.pop();
			}
		}

		cout << numbers.top() << endl;
}

int main() {
	stringstream ss;
	string tmp;
	operation();

	while (cin >> tmp && tmp != "EOI") {
		ss << tmp << endl;
	}

	cout << "\n";
	cout << "계산 결과" << endl;
	cout << "\n";

	while(!ss.eof()) {
		read_and_evaluate(ss);

		while (ss.peek() == '\n')
			ss.get();
	}


	return 0;
}

