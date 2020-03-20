#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <assert.h>

using namespace std;

/* drawTemplate - API function for T to render itself
 *
 * For single-line entities (e.g. PODs), provide an ostream operator<<.
 * For multi-line, overload drawTemplate (or live without position parameter).
 */
template <typename T> // overload for multi-line ostream << x
void drawTemplate(const T& x, ostream& out, size_t position)
{ out << string(position, ' ') << x << endl; }

class document_t {
    struct drawable_t {
    public:
        template <typename T>
        drawable_t(T x) : self_(make_shared<model<T>>(move(x))) { }

        void draw1(ostream& out, size_t position) const
        { self_->drawConcept(out, position); }

    private:
        struct calls_drawTemplate_t {
            virtual ~calls_drawTemplate_t() = default;
            virtual void drawConcept(ostream&, size_t) const = 0;
        };
        template <typename T>
        struct model : calls_drawTemplate_t {
            model(T x) : data_(move(x)) { }

            void drawConcept(ostream& out, size_t position) const
            { drawTemplate(data_, out, position); }

            T data_;
        };

        shared_ptr<const calls_drawTemplate_t> self_;
    };

public:
    void emplace_back (drawable_t p) { d.emplace_back(move(p)); }
    // document_t in multi-line so overload drawTemplate instead of operator<<
    friend void drawTemplate(const document_t& d, ostream& out, size_t position)
    {
        out << string(position, ' ') << "<document>" << endl;
        for (const auto& e : d.d)
            e.draw1(out, position + 2);
        out << string(position, ' ') << "</document>" << endl;
    }
    drawable_t& operator[](int i) { return d[i]; }

private:
    vector<drawable_t> d;
};

class history_t {
public:
    history_t(size_t size = 1) : x(size) {  }

    void commit() { assert(x.size()); x.push_back(x.back()); }
    void undo() { assert(x.size()); x.pop_back(); }
    document_t& current() { assert(x.size()); return x.back(); }
    const document_t& current() const { assert(x.size()); return x.back(); }

    // history_t in multi-line so overload drawTemplate instead of operator<<
    friend void drawTemplate(const history_t& h, ostream& out, size_t position)
    {
        out << string(position, ' ') << "<history>" << endl;
        for (const auto& e : h.x)
            drawTemplate(e, out, position + 2);
        out << string(position, ' ') << "</history>" << endl;
    }

private:
    vector<document_t> x;
};

class my_class_t {
    // my_class is single-line so overload operator<<
    friend ostream& operator<<(ostream& out, const my_class_t& h)
    { return out << "my class"; }
};

int main ()
{
    history_t h;

    h.current().emplace_back(string("Hello"));
    h.current().emplace_back(0);
    h.current().emplace_back(my_class_t());

    drawTemplate(h, cout, 0);
    cout << "--------------------------" << endl;

    h.commit();

    h.current()[1] = string("World!");
    h.current().emplace_back(h);

    drawTemplate(h, cout, 0);
    cout << "--------------------------" << endl;

    h.undo();

    drawTemplate(h, cout, 0);
}
