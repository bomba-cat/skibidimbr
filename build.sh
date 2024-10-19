mkdir -p target/

nasm -f bin -o target/skibidimbr.bin ./src/boot/loader.asm
