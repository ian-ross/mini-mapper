file "./build/semihosting.out"
target extended-remote localhost:3333
monitor reset halt
monitor arm semihosting enable
load
