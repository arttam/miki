### RAII "**R**esource **A**cquisition **I**s **I**nitialization"

A resource is something that must be acquired and later (explicitly or implicitly) released. \
Examples are memory, locks, sockets, thread handles, and file handles. \
For a long-running program, failing to release a resource in a timely manner (‘‘a leak’’) \
can cause serious performance degradation and possibly even a miserable crash.

The standard library components are designed not to leak resources. \
To do this, they rely on the basic language support for resource management \
using constructor/destructor pairs to ensure \
that a resource doesn’t outlive an object responsible for it. 

Containers (such as vector and **map**, **string**, and **iostream**) \
manage their resources (such as file handles and buffers) similarly.

