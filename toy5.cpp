#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <assert.h>

using namespace std;

template <typename T>
void draw(const T& x, ostream& out, size_t position)
{ out << string(position, ' ') << x << endl; }

class object_t {
public:
    template <typename T>
    object_t(T x) : self_(make_shared<model<T>>(move(x))) { }

    friend void draw(const object_t& x, ostream& out, size_t position)
    { x.self_->draw_(out, position); }

private:
    struct concept_t {
        virtual ~concept_t() = default;
        virtual void draw_(ostream&, size_t) const = 0;
    };
    template <typename T>
    struct model : concept_t {
        model(T x) : data_(move(x)) { }

        void draw_(ostream& out, size_t position) const
        { draw(data_, out, position); }

        T data_;
    };

    shared_ptr<const concept_t> self_;
};

using document_t = vector<object_t>;

void draw(const document_t& x, ostream& out, size_t position)
{
    out << string(position, ' ') << "<document>" << endl;
    for (const auto& e : x)
        draw(e, out, position + 2);
    out << string(position, ' ') << "</document>" << endl;
}

using history_t = vector<document_t>;

void commit(history_t& x) { assert(x.size()); x.push_back(x.back()); }
void undo(history_t& x) { assert(x.size()); x.pop_back(); }
document_t& current(history_t& x) { assert(x.size()); return x.back(); }
const document_t& current(const history_t& x) { assert(x.size()); return x.back(); }

class my_class_t { };

void draw(const my_class_t&, ostream& out, size_t position)
{ out << string(position, ' ') << "my_class_t" << endl; }

ostream& operator<<(ostream& out, const history_t& h)
{ draw(current(h), out, 0); return out; }

int main ()
{
#ifdef FISHFOOD
    document_t document;
    document.emplace_back(0);
    document.emplace_back(string("Hello!"));
    document.emplace_back(document);
    document.emplace_back(my_class_t());

    draw(document, cout, 0);
#endif /* FISHFOOD */

    history_t h(1);

    current(h).emplace_back(0);
    current(h).emplace_back(string("Hello!"));

    draw(current(h), cout, 0);
    cout << "--------------------------" << endl;

    commit(h);

    current(h).emplace_back(h);
    current(h).emplace_back(my_class_t());
    current(h)[1] = string("World");

    draw(current(h), cout, 0);
    cout << "--------------------------" << endl;

    undo(h);

    draw(current(h), cout, 0);
}
