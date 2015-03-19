//Luciana Elena Viziru - 312CA

#include<iostream>
using namespace std;
	
template <typename T>struct Node {
	T value;
	Node<T> *next;
	Node<T> *prev;
};
 
template <typename T>class LinkedList {
private:
	Node<T> *pFirst, *pLast;
 
public:
	// Constructor
	LinkedList(){
		pFirst = NULL;
		pLast = NULL;
	}
	// Destructor
	~LinkedList(){
		if(!isEmpty()){
			Node<T>* aux = pFirst;
			while(aux->next != NULL){
				aux = aux->next;
				delete aux->prev;
			}
			delete aux;
		}
	}
 
	/* Adauga un nod cu valoarea == value la inceputul listei. */
	void addFirst(T value){
		if(pFirst == NULL){
			pFirst = new Node<T>;
			pFirst->value = value;
			pFirst->next = NULL;
			pFirst->prev = NULL;
			pLast = pFirst;
		}
		else{
			Node<T> *aux = new Node<T>;
			aux->value = value;
			aux->next = pFirst;
			aux->prev = NULL;
			pFirst->prev = aux;
			pFirst = aux;
		}
	}
 
	/* Adauga un nod cu valoarea == value la sfarsitul listei. */
	void addLast(T value){
		Node<T> *aux = new Node<T>;
		aux->value = value;
		aux->prev = pLast;
		aux->next = NULL;
		if(isEmpty()){
			pLast = aux;
			pFirst = pLast;
		}	
		else{
			pLast->next = aux;
			pLast = aux;
		}
	}
 
	/* Elimina elementul de la inceputul listei si intoarce valoarea acestuia. */
	T removeFirst(){
		T x;
		if(!isEmpty()){
			if(pLast == pFirst){
				x = pFirst->value;
				delete pFirst;
				pFirst = NULL;
				pLast = NULL;
			}
			else{
				x = pFirst->value;
				pFirst = pFirst->next;
				delete pFirst->prev;
				pFirst->prev = NULL;
			}
		}
		else{
			cout<<"Error! List is empty!\n";
		}
		return x; 		
	}
 
	/* Elimina elementul de la sfarsitul listei si intoarce valoarea acestuia. */
	T removeLast(){
		T x;
		if(!isEmpty()){
			if(pFirst == pLast){
				x = pLast->value;
				delete pLast;
				pFirst = NULL;
				pLast = NULL;
			}
			else{
				x = pLast->value;
				pLast = pLast->prev;
				delete pLast->next;
				pLast->next = NULL;
			}
		}
		else{
			cout<<"Error! List is empty!\n";
		}
		return x;
	}
 
	/* Adauga un nod cu valoarea == value la pozitia pos din lista. */
	/*indexarea incepe de la 0 */
	void addSomewhere(T value, int pos){
		if(!isEmpty()){
			//daca lista nu este goala, cauta pozitia pos
			if(pos == 0){
				addFirst(value);
			}
			else{
				int count = 0;
				Node<T> *it = pFirst;
				while(count != pos){
					if(it->next == NULL){
						break;
					}
					it = it->next;
					count ++;
				}
				//atunci cand a fost gasita, adauga nodul
				if(count == pos){
					Node<T> *aux = new Node<T>;
					aux->value = value;
					aux->next = it;
					aux->prev = it->prev;
					it->prev = aux;
					aux->prev->next = aux;
				}
				else{
				//daca pozitia nu exista
				//(este mai mare decat numarul de noduri)
				//nodul este adaugat la finalul listei
					addLast(value);
				}
			}
		}
		else{
			addFirst(value);
		}
	}
	/* Elimina nodul de pe pozitia pos din lista*/
	T removeSomewhere(int pos){
		T x;
		if(!isEmpty()){
			//daca lista nu este vida
			if(pos == 0){
				x = removeFirst();
			}
			else{
				//cauta pozitia pos
				int count = 0;
				Node<T> *it;
				it = pFirst;
				while(count != pos){
					it = it->next;
					count ++;
				}
				//se retine valoarea nodului
				x = it->value;
				it->prev->next = it->next;
				if(it != pLast){
					//daca exista it->next(nu este ultimul nod),
					//se refac toate legaturile
					it->next->prev = it->prev;
				}
				else{
					//daca este ultimul nod, se mute pLast
					pLast = it->prev;
				}
				//se sterge nodul
				delete it;	
			}
		}
		else{
			cout<<"Error! List is empty, nothing to remove!"<<endl;	
		}
		return x;
	}	
 
	/* Intoarce true daca lista este vida, false altfel. */
	bool isEmpty(){
		return pFirst == NULL;
	}

	Node<T>* get_pFirst(){
		return pFirst;
	}
		
	Node<T>* get_pLast(){
		return pLast;
	}

	T get_value(Node<T> *it){
		return it->value;
	}
	
	//metoda de golire a listei	
	void empty_list(){
		while(!isEmpty()){
			removeFirst();
		}
	}

	//suprascrierea operatorului assign
	LinkedList<T> operator=(LinkedList<T> *l){
		while(!l->isEmpty()){
			this->addFirst(l->removeLast());
		}
		return *this;
	}
	
};
template class LinkedList<string>;
template struct Node<string>;

