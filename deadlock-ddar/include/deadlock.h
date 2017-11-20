#define NRAGNODE NLOCK + NPROC

/** Function Prototype(s) **/

int node_proc(int pid); /** translate a process pid to a RAG node id **/

int node_lock(int lockid); /** translate a lock id to a RAG node id **/

void rag_request(int pid, int lockid); /**  adds a request edge to the RAG from pid to lockid **/
void rag_alloc(int pid, int lockid); /** addmos an allocation edge to the RAG from lockid to pid. Removes the request edge from pid to lockid **/
void rag_dealloc(int pid, int lockid); /** removes the request or allocation edge from lockid to pid **/
void rag_print(); /** prints the adjacent matrix or list representing the current RAG **/
int deadlock_detect(); /** Checks the RAG for cycle(s). For each cycle found, print out DEADLOCK followed by the nodes involved in the cycle. If not cycles are found, then the system is deadlock-free. You should produce no output in this case (Depth-first-search) **/
int *get_el_ptr(int i, int j);
int deadlock_recover(); 

/* Lock table entry */
struct  rag_table
{
	int	graph[(NRAGNODE) * (NRAGNODE)];
};

extern	struct rag_table rag;

extern lid32 deadlockid;
extern int deadlockflag;
