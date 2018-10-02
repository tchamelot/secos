# Reponse TP1

# 1

Une gdt a été configuré par grub. Elle comporte 4 entrées flat:
1. Entrée NULL
2. Entrée code RX, 32 bits, ring 0,  base 0, limit 0xfffff
3. Entrée data RW, 32 bits, ring 0, base 0, limit 0xfffff
4. Entrée code CXRA, 16 bits, ring 0, base 0, limit 0x0ffff
