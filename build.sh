echo Generating Target Folder if not existing...
sleep 1
mkdir -p target/ && echo "Success!" || echo "Error creating Target Folder!"

echo

echo Assembling the Bootloader...
sleep 1
nasm -f bin -o ./target/skibidimbr.bin ./src/boot/loader.asm && echo "Success!" || echo "Error Assembling Bootloader!"

echo

echo Compiling the Image Processor...
sleep 1
gcc -o ./target/image_processor ./src/bitmap/image_processor.c -lpng -lm && echo "Success!" || echo "Error Compiling Image Processor!"

echo

echo Building Complete!
exit
