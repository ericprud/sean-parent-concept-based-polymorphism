/* Use document and document history library, an expansion of Sean Parent's:
 * https://sean-parent.stlab.cc/papers-and-presentations/#inheritance-is-the-base-class-of-evil
 * source kinda aligns with:
 * https://sean-parent.stlab.cc/presentations/2013-03-06-value_semantics/value-semantics.cpp
 * This maintains the original MIT License.
 *
 * ericP <eric.prudhommeaux@janeirodigital.com>, <eric@w3.org>
 */

#include "doc-library.hh"

using namespace std;

/* user-defined drawable types.
 *
 * These would be more complex classes in a reallistic use case.
 */

class my_string_t {
    std::string str;

public:
    my_string_t (std::string str) : str(str) { }
    void draw(ostream& out, size_t position) const
    { out << string(position, ' ') << str << endl; }
};

class my_int_t {
    int i;

public:
    my_int_t (int i) : i(i) { }
    void draw(ostream& out, size_t position) const
    { out << string(position, ' ') << i << endl; }
};

class my_class_t {
public:
    void draw(ostream& out, size_t position) const
    { out << string(position, ' ') << "my_class" << endl; }
};

int main ()
{
    history_t h;

    h.current().add(my_string_t("Hello"));
    h.current().add(my_int_t(0));
    h.current().add(my_class_t());

    h.draw(cout, 0);
    cout << "--------------------------" << endl;

    h.commit();

    h.current()[1] = my_string_t("World!");
    h.current().add(h);

    h.draw(cout, 0);
    cout << "--------------------------" << endl;

    h.undo();

    h.draw(cout, 0);
}
