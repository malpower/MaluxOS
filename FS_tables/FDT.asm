db 1
db 1
db 1
db 4
dd 512*20
times 16 db 0
dd 100
dd 0
db 'BUI.SYS'
times 60-($-$$) db 0
db 1
db 1
db 1
db 4
dd 512
times 16 db 0
dd 100
dd 96
db 'SAT.SYS'
times 120-($-$$) db 0
db 1
db 1
db 1
db 2
dd 5120
times 16 db 0
dd 100
dd 240
db 'mal.pgm'
times 180-($-$$) db 0
times 512 db 0
