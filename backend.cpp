//Luciana Elena Viziru - 312CA

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include "backend.h"

//Constructor
Backend::Backend()
{
	text = new LinkedList<string>;	
	clipboard = new LinkedList<string>;
	undo = new Stack<LinkedList<string>, string>;
	redo = new Stack<LinkedList<string>, string>;
	stop_text = -1;	//indicele de final al textului	
	p = NULL;
}

//Destructor
Backend::~Backend()
{
	delete text;
	delete clipboard;
	delete undo;
	delete redo;
	if(p != NULL){
		free(p);
	}
}

void Backend::Cut(const int start, const int stop)
{	
	//se goleste stiva de redo
	redo->empty_stack();
	//se salveaza textul curent in stringul full_text
	string full_text;
	Node<string>* it = text->get_pFirst();
	while(it != NULL){
		full_text += text->get_value(it);
		it = it->next;
	}
	//se salveaza in stiva de undo
	undo->push(full_text);
	int pos = 0;
	//se goleste clipboardul
	clipboard->empty_list();
	if(!text->isEmpty()){
		//daca exista text, se cauta indicele de start
		Node<string> *it, *aux;
		int pstart = 1; //indicele valid de start
		int pstop; //indicele valid de stop
		int count;
		if(start <= 0){
			//aux salveaza un pointer catre primul cuvant 
			//care va fi decupat
			aux = text->get_pFirst();
			//indicele valid de start este cel initial
			pstart = 0;
		}
		if(stop <= start || stop <= 0){
			//daca indicii sunt invalizi, iese din functie
			return;
		}
		//se cauta pozitia primului cuvant care va fi scos
		it = text->get_pFirst();	
		//un pointer catre cuvant se salveaza in aux
		aux = it;
		count = it->value.size();
		it = it->next;
		while(stop > count && it != NULL){
			count += it->value.size();
			aux = it;		
			it = it->next;
		}
		//daca indexul de stop se afla in interiorul ultimului cuvant
		if(it == NULL && aux != text->get_pFirst() && stop < stop_text){
			//se trece la ultimul index valid
			count -= aux->value.size();
		}
		//daca indexul se afla in interiorul unui cuvant(nu este ultimul)
		if(stop < count){
			//se trece la indexul anterior
			count -= aux->value.size();
		}
		pstop = count;	//pstop memoreaza indicele valid de final
		if(pstart){
			//daca indicele valid de start nu a fost 
			//deja fixat, il caut
			it = text->get_pFirst();
			count = it->value.size();
			//aux salveaza un pointer la primul cuvant care va fi scos
			aux = it;
			pos = 0;
			while(start > count && it != NULL){
				if(it != text->get_pFirst()){
					count += it->value.size();
				}
				aux = it;
				pos ++;		
				it = it->next;
			}
			//salvez indicele valid de start
			pstart = count;
		} 
		//daca exista cuvinte intre cei doi indici valizi
		if(pstop > pstart){
			count = pstart;
			//caut cuvantul care incepe de la pstart
			if(pstart){
				pos ++;
				aux = aux->next;
			}
			else{
				pos = 0;
				aux = text->get_pFirst();
			}
			//se decupeaza cuvintele si se adauga
			//in clipboard, pana ajunge la pstop 
			//la intrarea in bucla, aux are deja valoarea 
			//primului cuvant care va fi scos
			while(count < pstop){
				//contor al pozitiei
				count += aux->value.size();
				//adaugarea in clipboard
				clipboard->addLast(aux->value);
				//trecerea la urmatorul cuvant
				aux = aux->next;
				//decuparea din text
				text->removeSomewhere(pos);
			}
		}
	}
	
	std::cout << "Cut from " << start << " to " << stop << "\n";
}

void Backend::Copy(const int start, const int stop)
{
	//se goleste clipboardul
	clipboard->empty_list();
	if(!text->isEmpty()){
		//daca exista text
		Node<string> *it, *aux;
		int pstart = 1, pstop, count;
		//caut un indice valid de start
		if(start <= 0){
			aux = text->get_pFirst();
			pstart = 0;
		}
		if(stop <= start){
			//daca indicii sunt invalizi, iese din functie
			return;
		}
		//caut indicele valid de stop
		it = text->get_pFirst();
		aux = it;
		count = it->value.size();
		it = it->next;
		while(stop > count && it != NULL){
			count += it->value.size();
			aux = it;		
			it = it->next;
		}
		//daca stop se afla in interiorul ultimului cuvant
		if(it == NULL && aux != text->get_pFirst() && stop < stop_text){
			//count scade la ultimul indice valid
			count -= aux->value.size();
		}
		//salvez indicele valid de stop
		pstop = count;
		if(pstart){
			//daca indicele valid de start nu este 0
			it = text->get_pFirst();
			count = it->value.size();
			aux = it;
			//caut indicele de start
			while(start > count && it != NULL){
				if(it != text->get_pFirst()){
					count += it->value.size();
				}
				//aux salveaza pointer la primul cuvant 
				//care va fi copiat
				aux = it;		
				it = it->next;
			}
			//salvez indicele valid de start
			pstart = count;
		}
		if(pstop > pstart){
			//daca exista cuvinte intre cei doi indecsi
			count = pstart;
			if(pstart){
				aux = aux->next;
			}
			else{
				aux = text->get_pFirst();
			}
			//fixez aux pointer la primul cuvant care va fi copiat
			while(count < pstop){
				//contor al pozitiei
				count += aux->value.size();
				//copiere in clipboard
				clipboard->addLast(aux->value);
				//trecerea la urmatorul cuvant
				aux = aux->next;
			}
		}
	}
	std::cout << "Copy from " << start << " to " << stop << "\n";
}

void Backend::Paste(const int start)
{
	//se goleste stiva de redo
	redo->empty_stack();
	//se salveaza textul curent in stiva de undo
	string full_text;
	Node<string>* it = text->get_pFirst();
	while(it != NULL){
		full_text += text->get_value(it);
		it = it->next;
	}
	undo->push(full_text);
	//daca exista text in clipboard
	if(!clipboard->isEmpty()){
		int pstart;	//indicele valid de start
		int count;	//contor auxiliar de pozitie
		Node<string> *it;
		if(start <= 0){
			//daca este cazul, se adauga continutul din
			//clipboard la inceputul textutlui
			it = clipboard->get_pLast();
			while(it != NULL){
				text->addFirst(it->value);
				it = it->prev;
			}
			return;
		}
		if(start >= stop_text){
			//daca este cazul, se adauga continutul din
			//clipboard la finalul textutlui
			it = clipboard->get_pFirst();
			while(it != NULL){
				text->addLast(it->value);
				it = it->next;
			}
			return;
		}
		//altfel, caut indicele valid de start	
		it = text->get_pFirst();
		count = it->value.size();
		it = it->next;
		while(start > count && it != NULL){
			count += it->value.size();		
			it = it->next;
		}
		pstart = count;
		//se adauga continutul clipboardului in text
		it = clipboard->get_pLast();
		while(it != NULL){
			if(p != NULL){
				free(p);
			}
			p = strdup(it->value.c_str());
			Add(pstart,  p);
			it = it->prev;
		}
	}		
	std::cout << "Paste from " << start << "\n";
}

void Backend::Undo(void)
{
	if(!undo->isEmpty()){
		//daca striva de undo nu este vida
		string full_text;
		Node<string>* it = text->get_pFirst();
		while(it != NULL){
			full_text += text->get_value(it);
			it = it->next;
		}
		//se salveaza continutul actual in stiva de redo
		redo->push(full_text);
		//se goleste lista textului
		text->empty_list();
		//se adauga continutul din stiva de undo in text
		full_text = undo->pop();
		cout<<"in undo am "<<full_text<<endl;
		if(full_text.size() == 0){
			//daca textul din stiva este nul, se iese din functie
			return;
		}
		//se creeaza noua lista de cuvinte
		char* s = strdup(full_text.c_str());
		char *word = strtok(s, " ");
		while(word != NULL){
			string aux(word);
			aux += ' ';  
			text->addLast(aux);
			word = strtok(NULL, " ");
		}
		free(s);
	}	
	std::cout << "Undo!\n";
}

void Backend::Redo(void)
{
	if(!redo->isEmpty()){
		//daca stiva de redo nu este vida
		string full_text;
		Node<string>* it = text->get_pFirst();
		while(it != NULL){
			full_text += text->get_value(it);
			it = it->next;
		}
		//se adauga continutul actual in stiva de undo
		undo->push(full_text);
		//se goleste lista de text
		text->empty_list();
		full_text = redo->pop();
		if(full_text.size() == 0){
			//daca textul dinstiva este nul, se iese din functie
			return;
		}
		//se adauga fiecare cuvant din redo in lista de text
		char* s = strdup(full_text.c_str());
		char *word = strtok(s, " ");
		while(word != NULL){
			string aux(word);
			aux += ' ';  
			text->addLast(aux);
			word = strtok(NULL, " ");
		}
		free(s);
	}	
	std::cout << "Redo!\n";
}

void Backend::Add(const int index, const char *word)
{
	//se goleste stiva de redo
	redo->empty_stack();
	//inainte de adaugare, se salveaza textul curent in stiva de undo
	string full_text;
	Node<string>* it = text->get_pFirst();
	while(it != NULL){
		full_text += text->get_value(it);
		it = it->next;
	}
	undo->push(full_text);
	//apoi, se creeaza un string corespunzator
	//si se adauga in lista
	string str(word);
	if(str[str.size() - 1] != ' '){
		clipboard->empty_list();
		str += ' ';
	}
	if(index <= 0){
		//daca este cazul, se adauga la inceputul listei
		text->addFirst(str);
	}
	else{
		if(index >= stop_text){
			//daca este cazul, se adauga la finalul listei
			text->addLast(str);
		}
		else{
			//altfel,cauta pozitia la care trebuie inserat cuvantul
			Node<string>* it = text->get_pFirst();
			int count = it->value.size();
			//am verificat deja daca trebuie inserat 
			//la inceptutul listei
			int pos = 1;
			it = it->next;
			while(count < index && it != NULL){
				count += it->value.size();
				pos ++;
				it = it->next;
			}
			//cuvantul este adaugat in lista
			text->addSomewhere(str, pos);
		}
	}
	std::cout << "Adding \"" << word << "\" from " << index << "\n";
}

const char *Backend::GetText(void)
{
	//este adaugat textul listei intr-un string
	string full_text;
	Node<string>* it = text->get_pFirst();
	while(it != NULL){
		full_text += text->get_value(it);
		it = it->next;
	}
	//este salvata dimensiunea textului
	stop_text = full_text.size();
	//este returnat un const char * cu continutul stringului
	if(p != NULL){
		free(p);
	}
	p = strdup(full_text.c_str());
	return p;
	std::cout << "Sending back text!\n";
	
	return "Not my full stored text";
}
