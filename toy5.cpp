#include <iostream>
#include <string>
#include <vector>
#include <memory> // shared_ptr
#include <assert.h>

/* draw - API function for T to render itself
 *
 * For single-line entities (e.g. PODs), provide an ostream operator<<.
 * For multi-line, overload draw (or live without position parameter).
 */
template <typename T> // overload for multi-line ostream << x
void draw(const T& x, std::ostream& out, size_t position)
{ out << std::string(position, ' ') << x << std::endl; }

/* document_t - a container of objects that conform to the draw API.
 */
class document_t {
    /* drawable_ptr - shared_ptr to a vtable which invokes the draw API
     */
    struct drawable_ptr {
        /* invokes_draw_API - vtable of functions which invoke the draw API
         */
        struct draw_API_vtable {
            virtual ~draw_API_vtable() = default;
            virtual void call_draw(std::ostream&, size_t) const = 0;
        };

        /* draw_API_binding - templated binding of type to draw API
         */
        template <typename T>
        struct draw_API_binding : draw_API_vtable {
            draw_API_binding(T x) : data_(std::move(x)) { }

            void call_draw(std::ostream& out, size_t position) const
            { draw(data_, out, position); }

            T data_;
        };

        /* constructor - invokes a draw_API_binding with inferred type
         */
        template <typename T>
        drawable_ptr(T x)
            : binding(std::make_shared<draw_API_binding<T>>(std::move(x)))
        { }

        std::shared_ptr<const draw_API_vtable> binding;
    };

    std::vector<drawable_ptr> drawable_ptrs;

public:
    void add (drawable_ptr p)
    { drawable_ptrs.emplace_back(std::move(p)); }
    drawable_ptr& operator[](int i)
    { return drawable_ptrs[i]; }

    // document_t is multi-line so overload draw instead of operator<<
    friend void draw(const document_t& doc, std::ostream& out, size_t position)
    {
        out << std::string(position, ' ') << "<document>" << std::endl;
        for (const auto& ptr : doc.drawable_ptrs)
            ptr.binding->call_draw(out, position + 2);
        out << std::string(position, ' ') << "</document>" << std::endl;
    }
};

/* history_t - a container of documents.
 */
class history_t {
    std::vector<document_t> documents;

public:
    history_t(size_t size = 1) : documents(size) {  }

    void commit()
    { assert(documents.size()); documents.push_back(documents.back()); }
    void undo()
    { assert(documents.size()); documents.pop_back(); }
    document_t& current()
    { assert(documents.size()); return documents.back(); }
    const document_t& current() const
    { assert(documents.size()); return documents.back(); }

    // history_t is multi-line so overload draw instead of operator<<
    friend void draw(const history_t& h, std::ostream& out, size_t position)
    {
        out << std::string(position, ' ') << "<history>" << std::endl;
        for (const auto& doc : h.documents)
            draw(doc, out, position + 2);
        out << std::string(position, ' ') << "</history>" << std::endl;
    }
};

// ^^- doc library -^^
// vv-  user code  -vv

/* my_class_t - fulfills the draw API with the help of the draw template
 */
class my_class_t {
    // my_class is single-line so just overload operator<<
    friend std::ostream& operator<<(std::ostream& out, const my_class_t& h)
    { return out << "my class"; }
};

int main ()
{
    history_t h;

    h.current().add(std::string("Hello"));
    h.current().add(0);
    h.current().add(my_class_t());

    draw(h, std::cout, 0);
    std::cout << "--------------------------" << std::endl;

    h.commit();

    h.current()[1] = std::string("World!");
    h.current().add(h);

    draw(h, std::cout, 0);
    std::cout << "--------------------------" << std::endl;

    h.undo();

    draw(h, std::cout, 0);
}
