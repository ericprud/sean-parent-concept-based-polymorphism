#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <assert.h>

using namespace std;

template <typename T>
void drawTemplate(const T& x, ostream& out, size_t position)
{
    out << string(position, ' ') << x << endl;
}

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

#if 0
ostream& operator<<(ostream& out, const document_t& d)
{
    out << "<document>" << endl;
    for (const auto& e : d)
        drawTemplate(e, out, 2);
    return out << "</document>" << endl;
}
#else
void drawTemplate(const document_t& d, ostream& out, size_t position)
{
    out << string(position, ' ') << "<document>" << endl;
    for (const auto& e : d)
        drawTemplate(e, out, position + 2);
    out << string(position, ' ') << "</document>" << endl;
}
#endif

using history_t = vector<document_t>;

void commit(history_t& x) { assert(x.size()); x.push_back(x.back()); }
void undo(history_t& x) { assert(x.size()); x.pop_back(); }
document_t& current(history_t& x) { assert(x.size()); return x.back(); }
const document_t& current(const history_t& x) { assert(x.size()); return x.back(); }

class my_class_t { };

void drawTemplate(const my_class_t&, ostream& out, size_t position)
{ out << string(position, ' ') << "my_class_t" << endl; }

#if 0
ostream& operator<<(ostream& out, const history_t& h)
{ drawTemplate(current(h), out, 0); return out; }
#else
void drawTemplate(const history_t& h, ostream& out, size_t position)
{
    #if 1
    out << string(position, ' ') << "<history>" << endl;
    for (const auto& e : h)
        drawTemplate(e, out, position + 2);
    out << string(position, ' ') << "</history>" << endl;
    #else
    drawTemplate(current(h), out, position);
    #endif
}
#endif

int main ()
{
    history_t h(1);

    current(h).emplace_back(0);
    current(h).emplace_back(string("Hello!"));

    drawTemplate(h, cout, 0);
    cout << "--------------------------" << endl;

    commit(h);

    current(h).emplace_back(my_class_t());
    current(h).emplace_back(h);
    current(h)[1] = string("World");

    drawTemplate(h, cout, 0);
    cout << "--------------------------" << endl;

    undo(h);

    drawTemplate(h, cout, 0);
}
