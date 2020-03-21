#include <iostream>
#include <string>
#include <vector>
#include <memory> // shared_ptr
#include <assert.h>

using namespace std;

/* draw - API function for T to render itself
 *
 * For single-line entities (e.g. PODs), provide an ostream operator<<.
 * For multi-line, overload draw (or live without position parameter).
 */
template <typename T> // overload for multi-line ostream << x
void draw(const T& x, ostream& out, size_t position)
{ out << string(position, ' ') << x << endl; }

class document_t {
private:
    /* drawable_ptr - shared_ptr to a vtable which invokes the draw API
     */
    struct drawable_ptr {
        /* invokes_draw_API - vtable of functions which invoke the draw API
         */
        struct draw_API_vtable {
            virtual ~draw_API_vtable() = default;
            virtual void call_draw(ostream&, size_t) const = 0;
        };

        /* draw_API_binding - templated binding of type to draw API
         */
        template <typename T>
        struct draw_API_binding : draw_API_vtable {
            draw_API_binding(T x) : data_(move(x)) { }

            void call_draw(ostream& out, size_t position) const
            { draw(data_, out, position); }

            T data_;
        };

        template <typename T>
        drawable_ptr(T x) : self_(make_shared<draw_API_binding<T>>(move(x))) { }

        shared_ptr<const draw_API_vtable> self_;
    };

    vector<drawable_ptr> drawable_ptrs;

public:
    void add (drawable_ptr p) { drawable_ptrs.emplace_back(move(p)); }

    // document_t is multi-line so overload draw instead of operator<<
    friend void draw(const document_t& d, ostream& out, size_t position)
    {
        out << string(position, ' ') << "<document>" << endl;
        for (const auto& e : d.drawable_ptrs)
            e.self_->call_draw(out, position + 2);
        out << string(position, ' ') << "</document>" << endl;
    }
    drawable_ptr& operator[](int i) { return drawable_ptrs[i]; }
};

class history_t {
public:
    history_t(size_t size = 1) : x(size) {  }

    void commit() { assert(x.size()); x.push_back(x.back()); }
    void undo() { assert(x.size()); x.pop_back(); }
    document_t& current() { assert(x.size()); return x.back(); }
    const document_t& current() const { assert(x.size()); return x.back(); }

    // history_t is multi-line so overload draw instead of operator<<
    friend void draw(const history_t& h, ostream& out, size_t position)
    {
        out << string(position, ' ') << "<history>" << endl;
        for (const auto& e : h.x)
            draw(e, out, position + 2);
        out << string(position, ' ') << "</history>" << endl;
    }

private:
    vector<document_t> x;
};

class my_class_t {
    // my_class is single-line so just overload operator<<
    friend ostream& operator<<(ostream& out, const my_class_t& h)
    { return out << "my class"; }
};

int main ()
{
    history_t h;

    h.current().add(string("Hello"));
    h.current().add(0);
    h.current().add(my_class_t());

    draw(h, cout, 0);
    cout << "--------------------------" << endl;

    h.commit();

    h.current()[1] = string("World!");
    h.current().add(h);

    draw(h, cout, 0);
    cout << "--------------------------" << endl;

    h.undo();

    draw(h, cout, 0);
}
