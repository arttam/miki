### Predicating word seach (prefix tree)

```cpp
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <vector>

using namespace std;

template<typename T>
class trie {
	map<T, trie> tries;

public:
	template<typename It>
	void insert(It it, It end_it)
	{
		if (it == end_it) {
			return;
		}
		tries[*it].insert(next(it), end_it); // See 1)
	}

	template<typename C>
	void insert(const C &container)
	{
		insert(begin(container), end(container));
	}

	void insert(const initializer_list<T> &il)
	{
		insert(begin(il), end(il));
	}

	void print(vector<T> &v) const
	{
		if (tries.empty()) {
			copy(begin(v), end(v), ostream_iterator<T>{cout, " "});
			cout << '\n';
		}
		for (const auto &p : tries) {
			v.push_back(p.first);
			p.second.print(v);
			v.pop_back();
		}
	}

	void print() const
	{
		vector<T> v;
		print(v);
	}

	template<typename It>
	optional<reference_wrapper<const trie>> subtrie(It it, It end_it) const
	{
		if (it == end_it) {
			return ref(*this);
		}

		auto found(tries.find(*it));
		if (found == end(tries)) {
			return {};
		}

		return found->second.subtrie(next(it), end_it);
	}

	template<typename C>
	auto subtrie(const C &c) const
	{
		return subtrie(begin(c), end(c));
	}
};

int main()
{
	trie<string> t;

	t.insert({"hi", "how", "are", "you"});
	t.insert({"hi", "how", "are", "they", "doing"});
	t.insert({"hi", "i", "am", "great", "thanks"});
	t.insert({"what", "are", "you", "doing"});
	t.insert({"i", "am", "watching", "a", "movie"});

	cout << "recorded sentences:\n";
	t.print();

	cout << "\npossible suggestions after \"hi\":\n";

	if (auto st(t.subtrie(initializer_list<string>{"hi"})); st) {
		st->get().print();
	}

	if (auto st(t.subtrie(initializer_list<string>{"hi", "how"})); st) {
		st->get().print();
	}
}
```

1) *\*tries[\*it].insert(next(it), end_it)* - short version of:
```cpp
		if (auto alreadyThere(tries.find(*it)); alreadyThere == tries.end()) {
			std::cout << "[ " << *it << " ]: Not in tree yet, adding" << std::endl;
			const auto &[newEl, isNew] = tries.try_emplace(*it);
			newEl->second.insert(next(it), end_it);
		}
		else {
			std::cout << "[ " << *it << " ]: Already in the tree, continue" << std::endl;
			alreadyThere->second.insert(next(it), end_it);
		}
```

#### How it works

##### Insertion
The pair of iterators, *it* and *end_it*, represent the word sequence to be inserted. \
The *tries[\*it]* element looks up the first word in the sequence in the subtrie, and then, *.insert(next(it), end_it)* \
restarts the same function on that lower subtrie, with the iterator one word *further* advanced.

##### Look-up
This code basically revolves around the *auto found (tries.find(\*it));* statement.

Another scary looking detail is the return type, *optional<reference_wrapper<const trie>>*. \
We chose *std::optional* as the wrapper because it is possible that there is no such subtrie for the input sequence we are looking for. \
Why we use *reference_wrapper* instead of just writing *optional<const trie &>*. \
The point here is that an *optional* instance with a member variable of the *trie\&* type is not reassignable and hence would not compile. \
Implementing a reference using *reference_wrapper* leads to reassignable objects.

#### It also possible to use same tree against dictionary

For example:
```cpp

static void prompt()
{
    cout << "Next input please:\n > ";
}

int main()
{
    trie<string> t;

    fstream infile {"db.txt"};
    for (string line; getline(infile, line);) {
        istringstream iss {move(line)};
        t.insert(istream_iterator<string>{iss}, {});
    }


    prompt();
    for (string line; getline(cin, line);) {
        istringstream iss {move(line)};

        if (auto st (t.subtrie(istream_iterator<string>{iss}, {})); st) {
            cout << "Suggestions:\n";
            st->get().print();
        } else {
            cout << "No suggestions found.\n";
        }
        cout << "----------------\n";
        prompt();
    }
}
```

Points to notice 
- reading entire line using *getline*, then *istream_iterator* to iterate through words in line.
- using *std::move* to avoid unnecessary memory allocation

