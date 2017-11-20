
/** Definition(s) **/

#define NLOCK 10 /** the max number of locks that xinu OS can support **/
#define NPROC 20 /** the max number of processes that xinu OS can have **/
#define NRAGNODE NLOCK + NPROC

/** Globals **/
// G = (V,E) [V = set of verices, E = set of edges]
int rag[NRAGNODE][NRAGNODE]; // automatc allocation, all fields placed stack.

/** Function Prototype(s) **/

int node_proc(int pid); /** translate a process pid to a RAG node id **/

int node_lock(int lockid); /** translate a lock id to a RAG node id **/

void rag_request(int pid, int lockid); /**  adds a request edge to the RAG from pid to lockid **/
void rag_alloc(int pid, int lockid); /** adds an allocation edge to the RAG from lockid to pid. Removes the request edge from pid to lockid **/
void rag_dealloc(int pid, int lockid); /** removes the request or allocation edge from lockid to pid **/
void rag_print(); /** prints the adjacent matrix or list representing the current RAG **/
int deadlock_detect(); /** Checks the RAG for cycle(s). For each cycle found, print out DEADLOCK followed by the nodes involved in the cycle. If not cycles are found, then the system is deadlock-free. You should produce no output in this case (Depth-first-search) **/
