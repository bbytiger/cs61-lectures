	.file	"f16.cc"
	.text
	.globl	_Z12array_accessP2prl
	.type	_Z12array_accessP2prl, @function
_Z12array_accessP2prl:
.LFB0:
	endbr64
	movl	(%rdi,%rsi,8), %eax
	ret
.LFE0:
	.size	_Z12array_accessP2prl, .-_Z12array_accessP2prl
	.ident	"GCC: (Ubuntu 10.2.0-5ubuntu2) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
