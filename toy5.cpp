#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <assert.h>

using namespace std;

template <typename T> // overload for multi-line ostream << x
void drawTemplate(const T& x, ostream& out, size_t position)
{ out << string(position, ' ') << x << endl; }

class object_t {
public:
    template <typename T>
    object_t(T x) : self_(make_shared<model<T>>(move(x))) { }

    friend void drawTemplate(const object_t& x, ostream& out, size_t position)
    { x.self_->drawConcept(out, position); }

private:
    struct concept_t {
        virtual ~concept_t() = default;
        virtual void drawConcept(ostream&, size_t) const = 0;
    };
    template <typename T>
    struct model : concept_t {
        model(T x) : data_(move(x)) { }

        void drawConcept(ostream& out, size_t position) const
        { drawTemplate(data_, out, position); }

        T data_;
    };

    shared_ptr<const concept_t> self_;
};

using document_t = vector<object_t>;

// document_t in multi-line so overload drawTemplate instead of operator<<
void drawTemplate(const document_t& d, ostream& out, size_t position)
{
    out << string(position, ' ') << "<document>" << endl;
    for (const auto& e : d)
        drawTemplate(e, out, position + 2);
    out << string(position, ' ') << "</document>" << endl;
}

using history_t = vector<document_t>;

void commit(history_t& x) { assert(x.size()); x.push_back(x.back()); }
void undo(history_t& x) { assert(x.size()); x.pop_back(); }
document_t& current(history_t& x) { assert(x.size()); return x.back(); }
const document_t& current(const history_t& x) { assert(x.size()); return x.back(); }

class my_class_t { };

// my_class is single-line so overload operator<<
ostream& operator<<(ostream& out, const my_class_t& h)
{ return out << "my class"; }

// history_t in multi-line so overload drawTemplate instead of operator<<
void drawTemplate(const history_t& h, ostream& out, size_t position)
{
    out << string(position, ' ') << "<history>" << endl;
    for (const auto& e : h)
        drawTemplate(e, out, position + 2);
    out << string(position, ' ') << "</history>" << endl;
}

int main ()
{
    history_t h(1);

    current(h).emplace_back(string("Hello"));
    current(h).emplace_back(0);
    current(h).emplace_back(my_class_t());

    drawTemplate(current(h), cout, 0);
    cout << "--------------------------" << endl;

    commit(h);

    current(h)[1] = string("World!");
    current(h).emplace_back(h);

    drawTemplate(current(h), cout, 0);
    cout << "--------------------------" << endl;

    undo(h);

    drawTemplate(current(h), cout, 0);
}
