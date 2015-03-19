//Luciana Elena Viziru - 312CA

#include"list.h"
//stiva implementata cu lista
// T1 lista inlantuita
template<typename T1, typename T2>
class Stack {
	public:
		// constructor
		Stack() {
			s = new T1; 
		}
 
		// destructor
		~Stack() {
			delete s;
		}
 
		// operator de adaugare
		void push(T2 x) {
			s->addLast(x);
		}
 
		// operatorul de stergere
		T2 pop() {
			return s->removeLast();
		}
 
		// operatorul de consultare
		T2 peek() {
			T2 x;
			x = s->removeLast();
			s->addLast(x);
			return x;
			
		}
 
		// operatorul de verificare dimensiune
		int isEmpty() {
			return s->isEmpty();
		}

		void empty_stack(){
			while(!isEmpty()){
				pop();
			}
		}
 
	private:
		T1 *s;	
};
template class Stack<LinkedList<string>, string>;
