#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include <ctime>
using namespace std;

typedef pair<string,string> bigram;

struct transition;

struct state {
	string txt;
	int totalfreq;
	map<string, transition*> out;
	state(string txt) { this->txt = txt; totalfreq = 0; out.clear(); }
} initial("");
map <bigram, state*> states; // 2-gram model, each state is 2 words

struct transition {
	int freq = 0;
	state *st = nullptr;
};

void update(state* &s, string str) {
	s->totalfreq++;
	if(s->out[str] == nullptr) {
		s->out[str] = new transition;
		if(!states[{s->txt,str}]) states[{s->txt,str}] = new state(str);
		s->out[str]->st = states[{s->txt,str}];
	}
	s->out[str]->freq++;
	s = s->out[str]->st;
}

void gen() {
	fstream fin("corpus.txt");
	string word, end;
	state *cur = &initial;
	while(1) {
		fin >> word;
		if(fin.eof()) break;
		update(cur,word);
		if(word[word.length() - 1] == '.') {
			update(cur, end);
			cur = &initial;
		}
	}
	fin.close();
}

string random_sentence() {
	state *cur = &initial;
	string sout;
	while(cur->totalfreq) {
		int pos = rand()%(cur->totalfreq);
		int ind = 0;
		for(pair<string,transition*> it : cur->out) {
			ind += it.second->freq;
			if(ind > pos) { // move cur pointer to random transition
				cur = it.second->st;
				if(cur->totalfreq) // if there's still more text available put a space
					sout += it.first + " ";
				else
					sout.resize(sout.length() - 1); // else cut off trailing space
				break;
			}
		}
	}
	return sout;
}

int main() {
	srand (time(NULL));
	gen();
	cout << "\nType \'x\' to exit, press enter without 'x' to make another sentence...\n\n";
	while(getchar() != 'x') {
		cout << random_sentence() << '\n';
	}
	return 0;
}
