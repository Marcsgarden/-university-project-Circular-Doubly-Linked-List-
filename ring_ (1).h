#include <iostream>

using namespace std;

template <typename Key, typename Info>
class MBRing {
private:
	struct Node {
		Key ID;
		Info Information;
		Node* Next;
		Node* Prev;
		
		Node() {}
		
		Node(const Key& ExistKey, const Info& ExistInfo) :
				ID(ExistKey), Information(ExistInfo), Next(nullptr), Prev(nullptr) {}
		
		Node(const Key& ExistKey, const Info& ExistInfo, Node* ExistNext, Node* ExistPrev) :
				ID(ExistKey), Information(ExistInfo), Next(ExistNext), Prev(ExistPrev) {}
		
		~Node() {
		
		}
		
		friend ostream& operator<<(ostream& out, const Node& src) {
			out << src.ID << "\t\t" << src.Information;
			return out;
		}
	};
	
	Node* Head;
	int Size;  // New: Size variable to store the size of the ring
	
	void insert(const Node*& src) { insert(src->ID, src->Information); }
	
	void extract(Node* src) {
		src->Prev->Next = src->Next;
		src->Next->Prev = src->Prev;
	}
	
	void copy_all(const MBRing<Key, Info>& src) {
		typename MBRing<Key, Info>::iterator it = src.const_begin();
		do {
			insert(it);
			it++;
		} while (it != src.const_begin());
	}

public:
	class iterator {
		Node* Current;
	
	public:
		iterator(Node* source) : Current(source) {}
		iterator() : Current(nullptr) {}
		iterator(const iterator& source) : Current(source.Current) {}
		
		~iterator() {}
		
		iterator& operator++() {
			if (!Current) {
				cout<<"Cannot increment a non-existing element!";
			}
			goto_next();
			return *this;
		}
		
		iterator operator++(int) {
			if (!Current) {
				cout<<"Cannot increment a non-existing element!";
			}
			iterator Copy(Current);
			goto_next();
			return Copy;
		}
		
		iterator& operator--() {
			if (!Current) {
				cout<<"Cannot de-increment a non-existing element!";
			}
			goto_prev();
			return *this;
		}
		
		iterator operator--(int) {
			if (!Current) {
				cout<<"Cannot de-increment a non-existing element!";
			}
			iterator Copy = iterator(Current);
			goto_prev();
			return Copy;
		}
		
		iterator operator=(const iterator& source) {
			if (this != &source) {
				Current = source.Current;
			}
			return *this;
		}
		
		iterator operator+(int distance) {
			if (distance < 0) {
				cout<<"iterator operator+ - distance not valid";
			}
			for (int i = 0; Current && i < distance; i++) goto_next();
			return *this;
		}
		
		iterator operator-(int distance) {
			if (distance < 0) {
				cout<<"iterator operator- - distance not valid";
			}
			for (int i = 0; Current && i < distance; i++) goto_prev();
			return *this;
		}
		
		bool operator==(const iterator& src) { return Current == src.Current; }
		bool operator!=(const iterator& src) { return Current != src.Current; }
		Node& operator*() const { return *Current; }
		Node* operator->() { return Current; }
		
		friend ostream& operator<<(ostream& out, const iterator& it) {
			out << *it;
			return out;
		}
		
		bool is_iter_valid() {
			return (Current != nullptr);
		}
		
		void goto_next() { Current = Current->Next; }
		void goto_next(int i) { for (int j = 0; j < i; j++) goto_next(); }
		void goto_prev() { Current = Current->Prev; }
		void goto_prev(int i) { for (int j = 0; j < i; j++) goto_prev(); }
		
		Key ID() const {
			return Current->ID;
		}
		
		Info Information() const {
			return Current->Information;
		}
	};
	
	iterator begin() { return iterator(Head); }
	iterator last() {
		if (!Head) return iterator(nullptr);
		else return iterator(Head->Prev);
	}
	
	iterator const_begin() const { return iterator(Head); }
	iterator const_last() const {
		if (!Head) return iterator(nullptr);
		return iterator(Head->Prev);
	}
	
	MBRing() : Head(nullptr), Size(0) {}
	
	~MBRing() { clear_all(); }
	
	MBRing(const MBRing<Key, Info>& src) : Head(nullptr), Size(0) {
		if (!src.is_empty()) copy_all(src);
	}
	
	MBRing<Key, Info>& operator=(const MBRing<Key, Info>& src) {
		clear_all();
		if (!src.is_empty()) copy_all(src);
		return *this;
	}
	
	MBRing<Key, Info> operator+(const MBRing<Key, Info>& src) {
		MBRing res(*this);
		res += src;
		return res;
	}
	
	MBRing<Key, Info>& operator+=(const MBRing<Key, Info>& rhs) {
		this->copy_all(rhs);
		return *this;
	}
	
	iterator erase(iterator src) {
		iterator Temp = iterator(src->Current->Next);
		extract(src->Current);
		delete src->Current;
		Size--;  // Updated: Decrement the size
		return Temp;
	}
	
	void insert(const Key& NewKey, const Info& NewInfo) {
		Node* CreateNode = new Node(NewKey, NewInfo);
		if (!Head) {
			CreateNode->Next = CreateNode;
			CreateNode->Prev = CreateNode;
			Head = CreateNode;
		}
		else {
			Node* trav = Head->Prev;
			CreateNode->Next = Head;
			CreateNode->Prev = trav;
			trav->Next = CreateNode;
			Head->Prev = CreateNode;
		}
		Size++;  // Updated: Increment the size
	}
	
	void insert(iterator& src) {
		insert(src->ID, src->Information);
	}
	
	iterator find_key(Key ExistKey) {
		if (!Head) return iterator(nullptr);
		Node* Current = Head;
		do {
			if (Current->ID == ExistKey) return iterator(Current);
			Current = Current->Next;
		} while (Current != Head);
		return iterator(nullptr);
	}
	
	void clear_all() {
		while (Head) clear_single();
	}
	
	void clear_single() {
		if (Head->Next == Head) {
			delete Head;
			Head = nullptr;
		}
		else {
			Node* temp = Head;
			Node* Precedessor = temp->Prev;
			Node* Successor = temp->Next;
			Precedessor->Next = Successor;
			Successor->Prev = Precedessor;
			delete temp;
			Head = Successor;  // Updated: Fixing the head assignment
			Size--;  // Updated: Decrement the size
		}
	}
	
	bool is_empty() const {
		return Head == nullptr;
	}
	
	int size() const {
		return Size;
	}
	
	void print() const {
		if (is_empty()) {
			cout << "Empty object of class MBRing" << endl;
			return;
		}
		auto it = const_begin();
		do {
			cout << it << endl;
			it++;
		} while (it != const_begin());
	}
};

template <typename Key, typename Info>
ostream& operator<<(ostream& out, const MBRing<Key, Info>& ring) {
	ring.print();
	return out;
}

template <typename Key, typename Info>
void split_by_key(const MBRing<Key, Info>& seq, int direction, Key start_key, int len1, int len2, int count,
                  MBRing<Key, Info>& seq1, MBRing<Key, Info>& seq2) {
	if (count < 1 || len1 < 1 || len2 < 1) {
		cout << "Wrong starting count or length input" << endl;
		return;
	}
	typename MBRing<Key, Info>::iterator Iter = seq.const_begin();
	typename MBRing<Key, Info>::iterator IterH = Iter;
	if (direction == 0) {
		while (Iter->ID != start_key) {
			Iter++;
			if (Iter == IterH->Prev) {
				cout << "There is no such a key" << endl;
				return;
			}
		}
		seq1.clear_all();
		seq2.clear_all();
		for (int k = 0; k < count; k++) {
			for (int i = 0; i < len1; i++) {
				seq1.insert(Iter);
				Iter++;
			}
			for (int j = 0; j < len2; j++) {
				seq2.insert(Iter);
				Iter++;
			}
		}
	}
	else if (direction == 1) {
		while (Iter->ID != start_key) {
			Iter--;
			if (Iter == IterH->Next) {
				cout << "There is no such a key" << endl;
				return;
			}
		}
		seq1.clear_all();
		seq2.clear_all();
		for (int k = 0; k < count; k++) {
			for (int i = 0; i < len1; i++) {
				seq1.insert(Iter);
				Iter--;
			}
			for (int j = 0; j < len2; j++) {
				seq2.insert(Iter);
				Iter--;
			}
		}
	}
	else {
		cout << "Wrong direction input" << endl;
	}
}

template <typename Key, typename Info>
void split_by_pos(const MBRing<Key, Info>& seq, int direction, int start_pos, int len1, int len2, int count,
                  MBRing<Key, Info>& seq1, MBRing<Key, Info>& seq2) {
	typename MBRing<Key, Info>::iterator Iter = seq.const_begin();
	typename MBRing<Key, Info>::iterator IterH = Iter;
	if (start_pos < 1 || count < 1 || len1 < 1 || len2 < 1) {
		cout << "Wrong starting position, length, or count input" << endl;
		return;
	}
	if (direction == 0) {
		for (int i = 1; i < start_pos; i++) {
			Iter++;
		}
		seq1.clear_all();
		seq2.clear_all();
		for (int k = 0; k < count; k++) {
			for (int i = 0; i < len1; i++) {
				seq1.insert(Iter);
				Iter++;
			}
			for (int j = 0; j < len2; j++) {
				seq2.insert(Iter);
				Iter++;
			}
		}
	}
	else if (direction == 1) {
		for (int i = 1; i < start_pos; i++) {
			Iter--;
		}
		seq1.clear_all();
		seq2.clear_all();
		for (int k = 0; k < count; k++) {
			for (int i = 0; i < len1; i++) {
				seq1.insert(Iter);
				Iter--;
			}
			for (int j = 0; j < len2; j++) {
				seq2.insert(Iter);
				Iter--;
			}
		}
	}
}

