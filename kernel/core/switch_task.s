.section .text
.global switch_task
.type switch_task, "function"

switch_task:
	mov 0x3c(%eax),%ebx
	mov %ebx,0x305f24			# Change TSS.s0.esp
	mov 0x40(%eax),%ebx
	mov %ebx,%cr3				# Change cr3
	mov 0x3c(%eax),%esp			# Change kernel stack
	
	# Interrupt context
	pushl 0x38(%eax)			# push ss
	pushl 0x34(%eax)			# push esp
	pushl 0x30(%eax)			# push eflags
	pushl 0x2c(%eax)			# push cs
	pushl 0x28(%eax)			# push eip

	# Reegisters
	pushl 0x1c(%eax)			# eax
	pushl 0x18(%eax)			# ecx
	pushl 0x14(%eax)			# edx
	pushl 0x10(%eax)			# ebx
	pushl 0xc(%eax)				# esp
	pushl 0x8(%eax)				# ebp
	pushl 0x4(%eax)				# esi
	pushl (%eax)				# edi
	popa
	iret
