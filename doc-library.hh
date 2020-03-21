/* Example document and document history library, an expansion of Sean Parent's:
 * https://sean-parent.stlab.cc/papers-and-presentations/#inheritance-is-the-base-class-of-evil
 * source kinda aligns with:
 * https://sean-parent.stlab.cc/presentations/2013-03-06-value_semantics/value-semantics.cpp
 * This maintains the original MIT License.
 *
 * ericP <eric.prudhommeaux@janeirodigital.com>, <eric@w3.org>
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory> // shared_ptr
#include <assert.h>

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
            { data_.draw(out, position); }

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

    void draw(std::ostream& out, size_t position) const
    {
        out << std::string(position, ' ') << "<document>" << std::endl;
        for (const auto& ptr : drawable_ptrs)
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

    void draw(std::ostream& out, size_t position) const
    {
        out << std::string(position, ' ') << "<history>" << std::endl;
        for (const auto& doc : documents)
            doc.draw(out, position + 2);
        out << std::string(position, ' ') << "</history>" << std::endl;
    }
};
