	.text
	.globl	test_and_set

/**
 * test_and_set: assume it's called with test_and_set(&lock);
 * @param lock pointer to a mutex_t variable
 *
 * Add a comment each line describing its action
 **/
test_and_set:
	# %ebx will hold the address of the lock
	# %eax is a temporary register
	# %esp is the stack pointer
	pushl	%ebx		# preserve contents of the %ebx register (holds base pointer), which is about to be modified
	movl	8(%esp), %ebx	# load the address of the lock into %ebx
	movl	$1, %eax	#load the value 1 into %eax register
	xchg	%eax, (%ebx)	# switch values between %eax register and the memory
											# address of the lock...
											# the lock will always be set to 1
											# the eax register now holds the previous value of the
											# lock
	popl	%ebx		# restore %ebx register from the stack
	ret			# subroutine return mechanism -- instruction first pops a code location off the hardware supported in-memory stack. Then it performs an unconditional jump to the retrieved code location.
	# the previous value of the lock is returned
