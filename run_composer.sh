echo “RUN PROGRAM - Ari’s AI Composer”
clang++ -Wall -Wextra musicapp.cpp -o musicapp -F /Library/Frameworks -framework SDL2_image -framework SDL2 -framework SDL2_mixer -framework SDL2_ttf
./musicapp
echo “PROCESS COMPLETE”
