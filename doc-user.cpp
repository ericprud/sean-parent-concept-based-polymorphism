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
 * Note that the draw template in doc-library means that anything with an
 * ostream operator<< is a has a draw function.
 */

// my_class_t - fulfills the draw API with the help of the draw template
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
