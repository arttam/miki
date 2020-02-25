### Filling containers from std::istream

```cpp
#include <iostream>
#include <iomanip>
#include <map>
#include <iterator>
#include <algorithm>
#include <numeric>

using namespace std;

struct meme {
    string description;
    size_t year;
};

istream& operator>>(istream &is, meme &m) {
    return is >> quoted(m.description) >> m.year;
}

istream& operator >>(istream &is, pair<string, meme> &p) {
    return is >> quoted(p.first) >> p.second;
}

int main()
{
    map<string, meme> m;

    copy(istream_iterator<pair<string, meme>>{cin}, {}, inserter(m, end(m)));

    auto max_func ([](size_t old_max, const auto &b) {
        return max(old_max, b.first.length());
    });
    const size_t width {accumulate(begin(m), end(m), 0u, max_func)};

    for (const auto &[meme_name, meme_desc] : m) {
        const auto &[desc, year] = meme_desc;

        cout << left << setw(width) << meme_name
             << " : " << desc
             << ", " << year << '\n';
    }
}
```

Output:
```bash
$ cat memes.txt
"Doge" "Very Shiba Inu. so dog. much funny. wow." 2013
"Pepe" "Anthropomorphic frog" 2016
"Gabe" "Musical dog on maximum borkdrive" 2016
"Honey Badger" "Crazy nastyass honey badger" 2011
"Dramatic Chipmunk" "Chipmunk with a very dramatic look" 2007

# cat memes.txt | ./bin/filling_containers
$ ./bin/filling_containers < memes.txt
Doge              : Very Shiba Inu. so dog. much funny. wow., 2013
Dramatic Chipmunk : Chipmunk with a very dramatic look, 2007
Gabe              : Musical dog on maximum borkdrive, 2016
Honey Badger      : Crazy nastyass honey badger, 2011
Pepe              : Anthropomorphic frog, 2016
```

Notes:
- **std::istream_iterator** reading custom structure into the map
- **std::quoted** - Quotation marks used as delimiters and dropped afterwards.\
If we have a string, *s*, printing it using *cout << quoted(s)* will put it in quotes. \
If we deserialize a string from a stream, for example, via *cin >> quoted(s)*, it will read the next quotation mark, fill the string with what is following, and continue until it sees the next quotation mark, no matter how many whitespace are involved.
- using of **std::accumulate** to get max length of key for nice printout at the end.