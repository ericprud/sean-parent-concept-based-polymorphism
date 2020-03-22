This repo captures the finished product in Sean Parent's `Inheritance is the Base Class of Evil
<https://channel9.msdn.com/Events/GoingNative/2013/Inheritance-Is-The-Base-Class-of-Evil>`_
and `Value Semantics and Concepts-based Polymorphism
<https://www.youtube.com/watch?v=_BpMYeUFXv8>`_ talks.


Transformations
===============

renamed classes
-------------
I renamed classes to allow for multiple uses of what Sean called a `concept`.
The new names describe what it is rather than describe it's role in the architecture:

=========== ================ ===
Sean's code Now              Description
=========== ================ ===
document_t  document_t       a container of objects that conform to the draw API
history_t   history_t        a container of documents
object_t    drawable_ptr     pointer to a vtable which invokes the draw API
concept_t   draw_API_vtable  vtable of functions which invoke the draw API
model_t     draw_API_binding templated binding of (inferred) type to draw API
=========== ================ ===

data hiding
-----------
*drawable_ptr*, *draw_API_vtable*, *draw_API_binding* are all scoped to the *document_t*.
To aid in comprehension, everything that can be private is.

renamed methods
---------------
*draw_* is called *call_draw* ('cause that's what it does).

changed output
--------------
The original demo code called ``draw(current(h), cout, 0)``.
It now calls ``draw(h, cout, 0);`` which captures the entire history object.

method-y alternative
--------------------
The ``member-functions`` branch treats *draw* function as an object method.
This simplifies the library at the cost of requiring more ceremony for PODs.
(The presumption is that, in normal use, most concept-based polymorphism will be applied to more complex structures.)
See the `difference <https://github.com/ericprud/sean-parent-concept-based-polymorphism/compare/member-functions>`_ to compare the syntax.

How it Works
============
The *document_t* object holds a vector of pointers to things it needs to be able to draw (*drawable_ptr*).
These point to an object with an interface contract defined by `draw_API_vtable`.
When invoked in the calling code, the templated constructror for *drawable_ptr* invokes the appropriate (templated) *draw_API_binding* for the calling type.
This barfs if the calling type doesn't have an associated *draw* function (a method in the ``member-functions`` branch or a global function in ``master``).
If it doesn't barf, the compiler has ensured the appropriate binding and plugged it into the vtable.
