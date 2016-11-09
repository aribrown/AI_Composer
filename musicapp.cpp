// Music Composer
//
// By: Ari Brown
//
//
//////////// GOALS //////////////
/*

create rhythmic patterns
create a better harmonic calculator:
7th chords
leading tone resolution
harmonize the bassline

secondary chords
modulation
modality
melody
repetition, themes
periods and sentences
sequences
sections, piece form
stress beats in performer using velocities

create different sounds

*/
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<SDL2/SDL.h>
#include<SDL2_image/SDL_image.h>
#include<SDL2_mixer/SDL_mixer.h>
#include<SDL2_ttf/SDL_ttf.h>
#include<cmath>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// CONSTANTS ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 850;

const int SCALE = 4;

const int MINUTE = 60000;

//////////// NON-CONSTANT /////////////////
string NAME;
string TITLE;
int TEMPO;
string KEY;
bool MINOR;
int BEATS;
int PER_MEASURE;
///////////////////////////////////////////

const int OCTAVES = 7;

const int TOP_OF_STAVES = 150;
const int STAFF_INDENT = 50;

const int ROW_DIST = SCALE; // pixels
const int STAFF_ROWS = 33;
const int INCREMENT_KEYSIG = 8;
const int ACC_DIM = SCALE*2.5;

const int NOTE_WIDTH = ROW_DIST*2.5;
const int NOTE_HEIGHT = ROW_DIST*2;
const int LINE_WIDTH = NOTE_WIDTH*1.6;
const int STEM_LENGTH = NOTE_HEIGHT*3;
const int DOT_DIM = NOTE_WIDTH/3;
const int STEM_THICKNESS = 5;
const int TAIL_LENGTH = ROW_DIST*2;
const int TAIL_OFFSET = 8;
const int DOT_OFFSET = NOTE_WIDTH * 1.5;

const int STAFF_INCREMENT = 300; // 350 allows for all notes
const int GAP = 50;
const int NOTE_INCREMENT = 40 * SCALE/5;
const int ACC_SHIFT = 10 * SCALE/5 *1.5;
/*
const float SIXTEENTH = 0.25;
const float DOTTED_SIXTEENTH = 0.375;
const float EIGHTH = 0.5;
const float DOTTED_EIGHTH = 0.75;
const float QUARTER = 1.0;
const float DOTTED_QUARTER = 1.5;
const float HALF = 2.0;
const float DOTTED_HALF = 3.0;
const float WHOLE = 4.0;
*/
bool init();
bool loadMedia();
void close();
SDL_Texture* loadTexture(std::string path);

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
TTF_Font *gFont = NULL;


Mix_Chunk* gNotes[OCTAVES][12][3]; // 3d array octave, pitch, velocity

int *gKeysig;

enum NotePitches 
{
        C,
        CS,
        D,
        DS,
        E,
        F,
        FS,
        G,
        GS,
        A,
        AS,
        B,
        TOTAL_PITCHES
};
/*
// flats
const int CF = B;
const int DF = CS;
const int ES = F;
const int EF = DS;
const int FF = E;
const int GF = FS;
const int AF = GS;
const int BF = AS;
const int BS = C;
*/
const int m2 = 1;
const int M2 = 2;
const int m3 = 3;
const int M3 = 4;
const int P4 = 5;
const int TT = 6;
const int P5 = 7;
const int m6 = 8;
const int M6 = 9;
const int m7 = 10;
const int M7 = 11;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// SDL FUNCTIONS ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

int random_int(int lo, int hi);

// initializes SDL and the window
bool init()
{
        bool success = true;
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
                cout << "couldn't initialize sdl";
                success = false;
        }
        else {
                if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") ){
                        cout << "linear texture failed";
                }
                gWindow = SDL_CreateWindow( "Ari's Program", SDL_WINDOWPOS_CENTERED, 
                                SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
                if ( gWindow == NULL ) {
                        cout << "no window";
                        success = false;
                }
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                        cout << "couldn't initialize music!" << endl;
                        success = false;
                }
                
                else {
                        // create renderer for window
                        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED );
                        if (gRenderer == NULL) {
                                cout << "renderer couldn't be created";
                                success = false;
                        }
                        else {
                                // init renderer color
                                SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255);
                                SDL_RenderClear( gRenderer );
                                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL);
                                if (!(IMG_Init(IMG_INIT_PNG))){
                                        cout << "couldn't initialize image";
                                        success = false;
                                }
                                if (TTF_Init() == -1)
                                {
                                        success = false;
                                }                         
                        }
                }
        }
        return success;
}
// loads background image
bool loadMedia()
{
        bool success = true;
        
        gTexture = loadTexture( "visuals/gray.png");
        if (gTexture == NULL){
                success = false;
        }
        
        gNotes[0][C][0] = Mix_LoadWAV("sounds/C1.wav");
        gNotes[0][CS][0] = Mix_LoadWAV("sounds/CS1.wav");
        gNotes[0][D][0] = Mix_LoadWAV("sounds/D1.wav");
        gNotes[0][DS][0] = Mix_LoadWAV("sounds/DS1.wav");
        gNotes[0][E][0] = Mix_LoadWAV("sounds/E1.wav");
        gNotes[0][F][0] = Mix_LoadWAV("sounds/F1.wav");
        gNotes[0][FS][0] = Mix_LoadWAV("sounds/FS1.wav");
        gNotes[0][G][0] = Mix_LoadWAV("sounds/G1.wav");
        gNotes[0][GS][0] = Mix_LoadWAV("sounds/GS1.wav");
        gNotes[0][A][0] = Mix_LoadWAV("sounds/A1.wav");
        gNotes[0][AS][0] = Mix_LoadWAV("sounds/AS1.wav");
        gNotes[0][B][0] = Mix_LoadWAV("sounds/B1.wav");
        gNotes[1][C][0] = Mix_LoadWAV("sounds/C2.wav");
        gNotes[1][CS][0] = Mix_LoadWAV("sounds/CS2.wav");
        gNotes[1][D][0] = Mix_LoadWAV("sounds/D2.wav");
        gNotes[1][DS][0] = Mix_LoadWAV("sounds/DS2.wav");
        gNotes[1][E][0] = Mix_LoadWAV("sounds/E2.wav");
        gNotes[1][F][0] = Mix_LoadWAV("sounds/F2.wav");
        gNotes[1][FS][0] = Mix_LoadWAV("sounds/FS2.wav");
        gNotes[1][G][0] = Mix_LoadWAV("sounds/G2.wav");
        gNotes[1][GS][0] = Mix_LoadWAV("sounds/GS2.wav");
        gNotes[1][A][0] = Mix_LoadWAV("sounds/A2.wav");
        gNotes[1][AS][0] = Mix_LoadWAV("sounds/AS2.wav");
        gNotes[1][B][0] = Mix_LoadWAV("sounds/B2.wav");
        gNotes[2][C][0] = Mix_LoadWAV("sounds/C3.wav");
        gNotes[2][CS][0] = Mix_LoadWAV("sounds/CS3.wav");
        gNotes[2][D][0] = Mix_LoadWAV("sounds/D3.wav");
        gNotes[2][DS][0] = Mix_LoadWAV("sounds/DS3.wav");
        gNotes[2][E][0] = Mix_LoadWAV("sounds/E3.wav");
        gNotes[2][F][0] = Mix_LoadWAV("sounds/F3.wav");
        gNotes[2][FS][0] = Mix_LoadWAV("sounds/FS3.wav");
        gNotes[2][G][0] = Mix_LoadWAV("sounds/G3.wav");
        gNotes[2][GS][0] = Mix_LoadWAV("sounds/GS3.wav");
        gNotes[2][A][0] = Mix_LoadWAV("sounds/A3.wav");
        gNotes[2][AS][0] = Mix_LoadWAV("sounds/AS3.wav");
        gNotes[2][B][0] = Mix_LoadWAV("sounds/B3.wav");
        gNotes[3][C][0] = Mix_LoadWAV("sounds/C4.wav");
        gNotes[3][CS][0] = Mix_LoadWAV("sounds/CS4.wav");
        gNotes[3][D][0] = Mix_LoadWAV("sounds/D4.wav");
        gNotes[3][DS][0] = Mix_LoadWAV("sounds/DS4.wav");
        gNotes[3][E][0] = Mix_LoadWAV("sounds/E4.wav");
        gNotes[3][F][0] = Mix_LoadWAV("sounds/F4.wav");
        gNotes[3][FS][0] = Mix_LoadWAV("sounds/FS4.wav");
        gNotes[3][G][0] = Mix_LoadWAV("sounds/G4.wav");
        gNotes[3][GS][0] = Mix_LoadWAV("sounds/GS4.wav");
        gNotes[3][A][0] = Mix_LoadWAV("sounds/A4.wav");
        gNotes[3][AS][0] = Mix_LoadWAV("sounds/AS4.wav");
        gNotes[3][B][0] = Mix_LoadWAV("sounds/B4.wav");
        gNotes[4][C][0] = Mix_LoadWAV("sounds/C5.wav");
        gNotes[4][CS][0] = Mix_LoadWAV("sounds/CS5.wav");
        gNotes[4][D][0] = Mix_LoadWAV("sounds/D5.wav");
        gNotes[4][DS][0] = Mix_LoadWAV("sounds/DS5.wav");
        gNotes[4][E][0] = Mix_LoadWAV("sounds/E5.wav");
        gNotes[4][F][0] = Mix_LoadWAV("sounds/F5.wav");
        gNotes[4][FS][0] = Mix_LoadWAV("sounds/FS5.wav");
        gNotes[4][G][0] = Mix_LoadWAV("sounds/G5.wav");
        gNotes[4][GS][0] = Mix_LoadWAV("sounds/GS5.wav");
        gNotes[4][A][0] = Mix_LoadWAV("sounds/A5.wav");
        gNotes[4][AS][0] = Mix_LoadWAV("sounds/AS5.wav");
        gNotes[4][B][0] = Mix_LoadWAV("sounds/B5.wav");
        gNotes[5][C][0] = Mix_LoadWAV("sounds/C6.wav");
        gNotes[5][CS][0] = Mix_LoadWAV("sounds/CS6.wav");
        gNotes[5][D][0] = Mix_LoadWAV("sounds/D6.wav");
        gNotes[5][DS][0] = Mix_LoadWAV("sounds/DS6.wav");
        gNotes[5][E][0] = Mix_LoadWAV("sounds/E6.wav");
        gNotes[5][F][0] = Mix_LoadWAV("sounds/F6.wav");
        gNotes[5][FS][0] = Mix_LoadWAV("sounds/FS6.wav");
        gNotes[5][G][0] = Mix_LoadWAV("sounds/G6.wav");
        gNotes[5][GS][0] = Mix_LoadWAV("sounds/GS6.wav");
        gNotes[5][A][0] = Mix_LoadWAV("sounds/A6.wav");
        gNotes[5][AS][0] = Mix_LoadWAV("sounds/AS6.wav");
        gNotes[5][B][0] = Mix_LoadWAV("sounds/B6.wav");
        gNotes[6][C][0] = Mix_LoadWAV("sounds/C7.wav");
        gNotes[6][CS][0] = Mix_LoadWAV("sounds/CS7.wav");
        gNotes[6][D][0] = Mix_LoadWAV("sounds/D7.wav");
        gNotes[6][DS][0] = Mix_LoadWAV("sounds/DS7.wav");
        gNotes[6][E][0] = Mix_LoadWAV("sounds/E7.wav");
        gNotes[6][F][0] = Mix_LoadWAV("sounds/F7.wav");
        gNotes[6][FS][0] = Mix_LoadWAV("sounds/FS7.wav");
        gNotes[6][G][0] = Mix_LoadWAV("sounds/G7.wav");
        gNotes[6][GS][0] = Mix_LoadWAV("sounds/GS7.wav");
        gNotes[6][A][0] = Mix_LoadWAV("sounds/A7.wav");
        gNotes[6][AS][0] = Mix_LoadWAV("sounds/AS7.wav");
        gNotes[6][B][0] = Mix_LoadWAV("sounds/B7.wav");
        
        gNotes[0][C][1] = Mix_LoadWAV("sounds/C1.wav");
        gNotes[0][CS][1] = Mix_LoadWAV("sounds/CS1.wav");
        gNotes[0][D][1] = Mix_LoadWAV("sounds/D1.wav");
        gNotes[0][DS][1] = Mix_LoadWAV("sounds/DS1.wav");
        gNotes[0][E][1] = Mix_LoadWAV("sounds/E1.wav");
        gNotes[0][F][1] = Mix_LoadWAV("sounds/F1.wav");
        gNotes[0][FS][1] = Mix_LoadWAV("sounds/FS1.wav");
        gNotes[0][G][1] = Mix_LoadWAV("sounds/G1.wav");
        gNotes[0][GS][1] = Mix_LoadWAV("sounds/GS1.wav");
        gNotes[0][A][1] = Mix_LoadWAV("sounds/A1MED.wav");
        gNotes[0][AS][1] = Mix_LoadWAV("sounds/AS1MED.wav");
        gNotes[0][B][1] = Mix_LoadWAV("sounds/B1MED.wav");
        gNotes[1][C][1] = Mix_LoadWAV("sounds/C2MED.wav");
        gNotes[1][CS][1] = Mix_LoadWAV("sounds/CS2MED.wav");
        gNotes[1][D][1] = Mix_LoadWAV("sounds/D2MED.wav");
        gNotes[1][DS][1] = Mix_LoadWAV("sounds/DS2MED.wav");
        gNotes[1][E][1] = Mix_LoadWAV("sounds/E2MED.wav");
        gNotes[1][F][1] = Mix_LoadWAV("sounds/F2MED.wav");
        gNotes[1][FS][1] = Mix_LoadWAV("sounds/FS2MED.wav");
        gNotes[1][G][1] = Mix_LoadWAV("sounds/G2MED.wav");
        gNotes[1][GS][1] = Mix_LoadWAV("sounds/GS2MED.wav");
        gNotes[1][A][1] = Mix_LoadWAV("sounds/A2MED.wav");
        gNotes[1][AS][1] = Mix_LoadWAV("sounds/AS2MED.wav");
        gNotes[1][B][1] = Mix_LoadWAV("sounds/B2MED.wav");
        gNotes[2][C][1] = Mix_LoadWAV("sounds/C3MED.wav");
        gNotes[2][CS][1] = Mix_LoadWAV("sounds/CS3MED.wav");
        gNotes[2][D][1] = Mix_LoadWAV("sounds/D3MED.wav");
        gNotes[2][DS][1] = Mix_LoadWAV("sounds/DS3MED.wav");
        gNotes[2][E][1] = Mix_LoadWAV("sounds/E3MED.wav");
        gNotes[2][F][1] = Mix_LoadWAV("sounds/F3MED.wav");
        gNotes[2][FS][1] = Mix_LoadWAV("sounds/FS3MED.wav");
        gNotes[2][G][1] = Mix_LoadWAV("sounds/G3MED.wav");
        gNotes[2][GS][1] = Mix_LoadWAV("sounds/GS3MED.wav");
        gNotes[2][A][1] = Mix_LoadWAV("sounds/A3MED.wav");
        gNotes[2][AS][1] = Mix_LoadWAV("sounds/AS3MED.wav");
        gNotes[2][B][1] = Mix_LoadWAV("sounds/B3MED.wav");
        gNotes[3][C][1] = Mix_LoadWAV("sounds/C4MED.wav");
        gNotes[3][CS][1] = Mix_LoadWAV("sounds/CS4MED.wav");
        gNotes[3][D][1] = Mix_LoadWAV("sounds/D4MED.wav");
        gNotes[3][DS][1] = Mix_LoadWAV("sounds/DS4MED.wav");
        gNotes[3][E][1] = Mix_LoadWAV("sounds/E4MED.wav");
        gNotes[3][F][1] = Mix_LoadWAV("sounds/F4MED.wav");
        gNotes[3][FS][1] = Mix_LoadWAV("sounds/FS4MED.wav");
        gNotes[3][G][1] = Mix_LoadWAV("sounds/G4MED.wav");
        gNotes[3][GS][1] = Mix_LoadWAV("sounds/GS4MED.wav");
        gNotes[3][A][1] = Mix_LoadWAV("sounds/A4MED.wav");
        gNotes[3][AS][1] = Mix_LoadWAV("sounds/AS4MED.wav");
        gNotes[3][B][1] = Mix_LoadWAV("sounds/B4MED.wav");
        gNotes[4][C][1] = Mix_LoadWAV("sounds/C5MED.wav");
        gNotes[4][CS][1] = Mix_LoadWAV("sounds/CS5MED.wav");
        gNotes[4][D][1] = Mix_LoadWAV("sounds/D5MED.wav");
        gNotes[4][DS][1] = Mix_LoadWAV("sounds/DS5MED.wav");
        gNotes[4][E][1] = Mix_LoadWAV("sounds/E5MED.wav");
        gNotes[4][F][1] = Mix_LoadWAV("sounds/F5MED.wav");
        gNotes[4][FS][1] = Mix_LoadWAV("sounds/FS5MED.wav");
        gNotes[4][G][1] = Mix_LoadWAV("sounds/G5MED.wav");
        gNotes[4][GS][1] = Mix_LoadWAV("sounds/GS5MED.wav");
        gNotes[4][A][1] = Mix_LoadWAV("sounds/A5MED.wav");
        gNotes[4][AS][1] = Mix_LoadWAV("sounds/AS5MED.wav");
        gNotes[4][B][1] = Mix_LoadWAV("sounds/B5MED.wav");
        gNotes[5][C][1] = Mix_LoadWAV("sounds/C6MED.wav");
        gNotes[5][CS][1] = Mix_LoadWAV("sounds/CS6MED.wav");
        gNotes[5][D][1] = Mix_LoadWAV("sounds/D6MED.wav");
        gNotes[5][DS][1] = Mix_LoadWAV("sounds/DS6MED.wav");
        gNotes[5][E][1] = Mix_LoadWAV("sounds/E6MED.wav");
        gNotes[5][F][1] = Mix_LoadWAV("sounds/F6MED.wav");
        gNotes[5][FS][1] = Mix_LoadWAV("sounds/FS6MED.wav");
        gNotes[5][G][1] = Mix_LoadWAV("sounds/G6MED.wav");
        gNotes[5][GS][1] = Mix_LoadWAV("sounds/GS6MED.wav");
        gNotes[5][A][1] = Mix_LoadWAV("sounds/A6MED.wav");
        gNotes[5][AS][1] = Mix_LoadWAV("sounds/AS6MED.wav");
        gNotes[5][B][1] = Mix_LoadWAV("sounds/B6MED.wav");
        gNotes[6][C][1] = Mix_LoadWAV("sounds/C7MED.wav");
        gNotes[6][CS][1] = Mix_LoadWAV("sounds/CS7MED.wav");
        gNotes[6][D][1] = Mix_LoadWAV("sounds/D7MED.wav");
        gNotes[6][DS][1] = Mix_LoadWAV("sounds/DS7MED.wav");
        gNotes[6][E][1] = Mix_LoadWAV("sounds/E7MED.wav");
        gNotes[6][F][1] = Mix_LoadWAV("sounds/F7MED.wav");
        gNotes[6][FS][1] = Mix_LoadWAV("sounds/FS7MED.wav");
        gNotes[6][G][1] = Mix_LoadWAV("sounds/G7MED.wav");
        gNotes[6][GS][1] = Mix_LoadWAV("sounds/GS7MED.wav");
        gNotes[6][A][1] = Mix_LoadWAV("sounds/A7MED.wav");
        gNotes[6][AS][1] = Mix_LoadWAV("sounds/AS7MED.wav");
        gNotes[6][B][1] = Mix_LoadWAV("sounds/B7MED.wav");
        
        gNotes[0][C][2] = Mix_LoadWAV("sounds/C1.wav");
        gNotes[0][CS][2] = Mix_LoadWAV("sounds/CS1.wav");
        gNotes[0][D][2] = Mix_LoadWAV("sounds/D1.wav");
        gNotes[0][DS][2] = Mix_LoadWAV("sounds/DS1.wav");
        gNotes[0][E][2] = Mix_LoadWAV("sounds/E1.wav");
        gNotes[0][F][2] = Mix_LoadWAV("sounds/F1.wav");
        gNotes[0][FS][2] = Mix_LoadWAV("sounds/FS1.wav");
        gNotes[0][G][2] = Mix_LoadWAV("sounds/G1.wav");
        gNotes[0][GS][2] = Mix_LoadWAV("sounds/GS1.wav");
        gNotes[0][A][2] = Mix_LoadWAV("sounds/A1LOW.wav");
        gNotes[0][AS][2] = Mix_LoadWAV("sounds/AS1LOW.wav");
        gNotes[0][B][2] = Mix_LoadWAV("sounds/B1LOW.wav");
        gNotes[1][C][2] = Mix_LoadWAV("sounds/C2LOW.wav");
        gNotes[1][CS][2] = Mix_LoadWAV("sounds/CS2LOW.wav");
        gNotes[1][D][2] = Mix_LoadWAV("sounds/D2LOW.wav");
        gNotes[1][DS][2] = Mix_LoadWAV("sounds/DS2LOW.wav");
        gNotes[1][E][2] = Mix_LoadWAV("sounds/E2LOW.wav");
        gNotes[1][F][2] = Mix_LoadWAV("sounds/F2LOW.wav");
        gNotes[1][FS][2] = Mix_LoadWAV("sounds/FS2LOW.wav");
        gNotes[1][G][2] = Mix_LoadWAV("sounds/G2LOW.wav");
        gNotes[1][GS][2] = Mix_LoadWAV("sounds/GS2LOW.wav");
        gNotes[1][A][2] = Mix_LoadWAV("sounds/A2LOW.wav");
        gNotes[1][AS][2] = Mix_LoadWAV("sounds/AS2LOW.wav");
        gNotes[1][B][2] = Mix_LoadWAV("sounds/B2LOW.wav");
        gNotes[2][C][2] = Mix_LoadWAV("sounds/C3LOW.wav");
        gNotes[2][CS][2] = Mix_LoadWAV("sounds/CS3LOW.wav");
        gNotes[2][D][2] = Mix_LoadWAV("sounds/D3LOW.wav");
        gNotes[2][DS][2] = Mix_LoadWAV("sounds/DS3LOW.wav");
        gNotes[2][E][2] = Mix_LoadWAV("sounds/E3LOW.wav");
        gNotes[2][F][2] = Mix_LoadWAV("sounds/F3LOW.wav");
        gNotes[2][FS][2] = Mix_LoadWAV("sounds/FS3LOW.wav");
        gNotes[2][G][2] = Mix_LoadWAV("sounds/G3LOW.wav");
        gNotes[2][GS][2] = Mix_LoadWAV("sounds/GS3LOW.wav");
        gNotes[2][A][2] = Mix_LoadWAV("sounds/A3LOW.wav");
        gNotes[2][AS][2] = Mix_LoadWAV("sounds/AS3LOW.wav");
        gNotes[2][B][2] = Mix_LoadWAV("sounds/B3LOW.wav");
        gNotes[3][C][2] = Mix_LoadWAV("sounds/C4LOW.wav");
        gNotes[3][CS][2] = Mix_LoadWAV("sounds/CS4LOW.wav");
        gNotes[3][D][2] = Mix_LoadWAV("sounds/D4LOW.wav");
        gNotes[3][DS][2] = Mix_LoadWAV("sounds/DS4LOW.wav");
        gNotes[3][E][2] = Mix_LoadWAV("sounds/E4LOW.wav");
        gNotes[3][F][2] = Mix_LoadWAV("sounds/F4LOW.wav");
        gNotes[3][FS][2] = Mix_LoadWAV("sounds/FS4LOW.wav");
        gNotes[3][G][2] = Mix_LoadWAV("sounds/G4LOW.wav");
        gNotes[3][GS][2] = Mix_LoadWAV("sounds/GS4LOW.wav");
        gNotes[3][A][2] = Mix_LoadWAV("sounds/A4LOW.wav");
        gNotes[3][AS][2] = Mix_LoadWAV("sounds/AS4LOW.wav");
        gNotes[3][B][2] = Mix_LoadWAV("sounds/B4LOW.wav");
        gNotes[4][C][2] = Mix_LoadWAV("sounds/C5LOW.wav");
        gNotes[4][CS][2] = Mix_LoadWAV("sounds/CS5LOW.wav");
        gNotes[4][D][2] = Mix_LoadWAV("sounds/D5LOW.wav");
        gNotes[4][DS][2] = Mix_LoadWAV("sounds/DS5LOW.wav");
        gNotes[4][E][2] = Mix_LoadWAV("sounds/E5LOW.wav");
        gNotes[4][F][2] = Mix_LoadWAV("sounds/F5LOW.wav");
        gNotes[4][FS][2] = Mix_LoadWAV("sounds/FS5LOW.wav");
        gNotes[4][G][2] = Mix_LoadWAV("sounds/G5LOW.wav");
        gNotes[4][GS][2] = Mix_LoadWAV("sounds/GS5LOW.wav");
        gNotes[4][A][2] = Mix_LoadWAV("sounds/A5LOW.wav");
        gNotes[4][AS][2] = Mix_LoadWAV("sounds/AS5LOW.wav");
        gNotes[4][B][2] = Mix_LoadWAV("sounds/B5LOW.wav");
        gNotes[5][C][2] = Mix_LoadWAV("sounds/C6LOW.wav");
        gNotes[5][CS][2] = Mix_LoadWAV("sounds/CS6LOW.wav");
        gNotes[5][D][2] = Mix_LoadWAV("sounds/D6LOW.wav");
        gNotes[5][DS][2] = Mix_LoadWAV("sounds/DS6LOW.wav");
        gNotes[5][E][2] = Mix_LoadWAV("sounds/E6LOW.wav");
        gNotes[5][F][2] = Mix_LoadWAV("sounds/F6LOW.wav");
        gNotes[5][FS][2] = Mix_LoadWAV("sounds/FS6LOW.wav");
        gNotes[5][G][2] = Mix_LoadWAV("sounds/G6LOW.wav");
        gNotes[5][GS][2] = Mix_LoadWAV("sounds/GS6LOW.wav");
        gNotes[5][A][2] = Mix_LoadWAV("sounds/A6LOW.wav");
        gNotes[5][AS][2] = Mix_LoadWAV("sounds/AS6LOW.wav");
        gNotes[5][B][2] = Mix_LoadWAV("sounds/B6LOW.wav");
        gNotes[6][C][2] = Mix_LoadWAV("sounds/C7LOW.wav");
        gNotes[6][CS][2] = Mix_LoadWAV("sounds/CS7LOW.wav");
        gNotes[6][D][2] = Mix_LoadWAV("sounds/D7LOW.wav");
        gNotes[6][DS][2] = Mix_LoadWAV("sounds/DS7LOW.wav");
        gNotes[6][E][2] = Mix_LoadWAV("sounds/E7LOW.wav");
        gNotes[6][F][2] = Mix_LoadWAV("sounds/F7LOW.wav");
        gNotes[6][FS][2] = Mix_LoadWAV("sounds/FS7LOW.wav");
        gNotes[6][G][2] = Mix_LoadWAV("sounds/G7LOW.wav");
        gNotes[6][GS][2] = Mix_LoadWAV("sounds/GS7LOW.wav");
        gNotes[6][A][2] = Mix_LoadWAV("sounds/A7LOW.wav");
        gNotes[6][AS][2] = Mix_LoadWAV("sounds/AS7LOW.wav");
        gNotes[6][B][2] = Mix_LoadWAV("sounds/B7LOW.wav");
        return success;
}
void open()
{
        cout << ("\033[H");
        cout << ("\033[2J");
        
        cout << endl << endl << endl;
        cout << "Welcome to Ari's AI Composer" << endl << endl;
        
        cout << "Name? ";
        getline(cin, NAME);
        cout << "Title? ";
        getline(cin, TITLE);
        /*
        cout << "Key? (format: b-flat) ";
        getline(cin, KEY);
        string m;
        cout << "Major? (yes/no) ";
        getline(cin, m);
        if (m == "no")
        {
                MINOR = true;
                if (KEY == "c") {KEY = "e-flat";}
                if (KEY == "g") {KEY = "b-flat";}
                if (KEY == "d") {KEY = "f";}
                if (KEY == "a") {KEY = "c";}
                if (KEY == "e") {KEY = "g";}
                if (KEY == "b") {KEY = "d";}
                if (KEY == "f-sharp") {KEY = "a";}
                if (KEY == "c-sharp") {KEY = "e";}
                
                if (KEY == "f") {KEY = "a-flat";}
                if (KEY == "b-flat") {KEY = "d-flat";}
                if (KEY == "e-flat") {KEY = "g-flat";}
                if (KEY == "a-flat") {KEY = "c-flat";}
                if (KEY == "d-flat") {KEY = "e";}
                if (KEY == "g-flat") {KEY = "b";}
                if (KEY == "c-flat") {KEY = "d";}
        }
        if (m == "yes") {MINOR = false;}
        cout <<"Time Signature... "<< endl;
        cout << "Beats: ";
        cin >> BEATS;
        cout << "Per-measure: ";
        cin >> PER_MEASURE;
        */
        cout <<"Tempo? q-note = ";
        cin >> TEMPO;
        string keys[15];
        keys[0] = "c";
        keys[1] = "g";
        keys[2] = "d";
        keys[3] = "a";
        keys[4] = "e";
        keys[5] = "b";
        keys[6] = "f-sharp";
        keys[7] = "c-sharp";
        keys[8] = "f";
        keys[9] = "b-flat";
        keys[10] = "e-flat";
        keys[11] = "a-flat";
        keys[12] = "d-flat";
        keys[13] = "g-flat";
        keys[14] = "c-flat";
        
        KEY = keys[random_int(0, 14)];
        if (random_int(0, 1) == 1) {MINOR = false;}
        else {MINOR = true;}
        BEATS = random_int(2, 6);
        PER_MEASURE = random_int(2, 8);

        init();loadMedia();
       /* 
        // Intro
        Mix_PlayChannel( -1, gNotes[3][C], 0);
        usleep(TIMESTEP);
        Mix_PlayChannel( -1, gNotes[3][E], 0);
        usleep(TIMESTEP);
        Mix_PlayChannel( -1, gNotes[3][G], 0);
        usleep(TIMESTEP);
        Mix_PlayChannel( -1, gNotes[4][C], 0);
        usleep(TIMESTEP);
        Mix_PlayChannel( -1, gNotes[4][E], 0);
        usleep(TIMESTEP);
        Mix_PlayChannel( -1, gNotes[4][G], 0);
        usleep(TIMESTEP);
        Mix_PlayChannel( -1, gNotes[5][C], 0);
        usleep(TIMESTEP);
*/
}

// closes SDL
void close()
{
        /*
        //closing audio
                        Mix_PlayChannel( -1, gNotes[5][C], 0);
                        usleep(TIMESTEP);
                        Mix_PlayChannel( -1, gNotes[4][G], 0);
                        usleep(TIMESTEP);
                        Mix_PlayChannel( -1, gNotes[4][E], 0);
                        usleep(TIMESTEP);
                        Mix_PlayChannel( -1, gNotes[4][C], 0);
                        usleep(TIMESTEP);
                        Mix_PlayChannel( -1, gNotes[3][G], 0);
                        usleep(TIMESTEP);
                        Mix_PlayChannel( -1, gNotes[3][E], 0);
                        usleep(TIMESTEP);
                        Mix_PlayChannel( -1, gNotes[3][C], 0);
                        usleep(TIMESTEP*2);
        */
        SDL_DestroyTexture(gTexture);
        gTexture = NULL;

        for (int i = 0; i < OCTAVES; i++)
        {
                for (int j = 0; j < TOTAL_PITCHES; j++)
                {
                        for (int k = 0; k < 3; k++)
                        {
                                Mix_FreeChunk( gNotes[i][j][k] );
                        }
                }
        }

        TTF_CloseFont( gFont );
        gFont = NULL;

        SDL_DestroyRenderer( gRenderer );
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
        gRenderer = NULL;

        delete []gKeysig;
        
        TTF_Quit();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();

}
// loads the background image to the surface
SDL_Texture* loadTexture(std::string path)
{
        SDL_Texture* newTexture = NULL;
        SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
        if (loadedSurface ==NULL) {
               cout << "can't load image";
        }
        else
        {
                newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface);
                if (newTexture == NULL){
                        cout << "couldn't create texture";
                }
                SDL_FreeSurface(loadedSurface);
        }
        return newTexture;
}

void replaceTexture(std::string path, SDL_Texture *pointer)
{
        SDL_Texture* newTexture = NULL;
        SDL_Surface* surface = IMG_Load( path.c_str() );

        newTexture = SDL_CreateTextureFromSurface( gRenderer, surface );

        SDL_DestroyTexture( pointer );
        pointer = newTexture;
}

static int rand_seed = -1;
void seed_random(int s)
{
        if (s>0){
                srand(s);
                rand_seed = s;
        }
}

// function used to generate a random integer between a range
int random_int(int lo, int hi)
{
        int range = hi - lo + 1;
        char *seed_str;
        if (rand_seed == -1 ) {
                seed_str = getenv("SNAKE_SEED");
                if (seed_str != NULL)
                        seed_random( (unsigned) atoi(seed_str) );
                else
                        seed_random( (unsigned) time(NULL) );
        }
        if ( range <= 0)
                return 17;
        return lo + ( rand() % range );
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// CLASSES //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// contains color components
struct Color {
        unsigned short r;
        unsigned short g;
        unsigned short b;
        unsigned short a;
};

struct Position {
        int x;
        int y;
        int w;
        int h;
};



////////////////////////////////////////////////// Line /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


// the Line class takes in a (float) angle from the +x axis,
//                         a (float) length of the line,
//                         an (integer) for x coordinate,
//                         an (integer) for y coordinate,
//                         and a Color struct for line color
//
class Line {
        public:
                Line(); // default constructor
                Line(float a, float l, int start_x, int start_y, Color color);

                void draw_line(); // draws line

                // setters of private variables
                void set_color(Color color);
                void set_angle(float a);
                void set_length(float l);
                void set_position(int xx, int yy);
                void set_alpha(unsigned short a);
                void set_thickness(int t);
                void set_y(int yy);
                void set_x(int xx);

                // getters of private variables
                float get_length();
                float get_angle();
                int get_x();
                int get_y();
                unsigned short get_alpha();

        private:

                // calculates distance
                float pythag(int x1, int y1, int x2, int y2);

                // private variables
                Color c; 
                float angle;
                float length;
                int x;
                int y;
                int thickness;
};
// default constructor
Line::Line()
{
        thickness = 1;
}

// constructor takes in angle, length, x, y, and color
Line::Line(float a, float l, int start_x, int start_y, Color color)
{
        c.r = color.r;
        c.g = color.g;
        c.b = color.b;
        c.a = color.a;
        angle = a;
        length = l;
        x = start_x;
        y = start_y;
        thickness = 1;
}

// draws line based on unit vector and length
void Line::draw_line()
{
        SDL_SetRenderDrawColor(gRenderer, c.r, c.g, c.b, c.a); // sets SDL to a draw color

        // gets directional components of line
        float dx = cos (angle * M_PI/180);
        float dy = -1*sin (angle * M_PI/180);

        // determins magnitude of the directional vector
        float mag = sqrt( pow(dx, 2) + pow(dy, 2) );

        // calculates unit vector in the direction
        float unit_x = dx/mag;
        float unit_y = dy/mag;

        // starts at the x and y point
        float current_x = (float)x;
        float current_y = (float)y;
        // increments x and y by unit vector and draws pixels until length is achieved
        while ((int)pythag(x, y, (int)current_x, (int)current_y) <= length)
        {
                for (int i = 0; i < thickness; i++)
                {
                                SDL_RenderDrawPoint (gRenderer, current_x, current_y + i);
                }
                current_x += unit_x;
                current_y += unit_y;
        }
}

// returns the length of the line
float Line::pythag(int x1, int y1, int x2, int y2)
{
        return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));  
}

//////////////////////// setters /////////////////////////
void Line::set_color(Color color)
{
        c.r = color.r;
        c.g = color.g;
        c.b = color.b;
        c.a = color.a;
}
void Line::set_angle(float a)
{
        angle = a;
}
void Line::set_length(float l)
{
        length = l;
}
void Line::set_position(int xx, int yy)
{
        x = xx;
        y = yy;
}
void Line::set_alpha(unsigned short a)
{
        c.a = a;
}
void Line::set_thickness(int t)
{
        thickness = t;
}
void Line::set_y(int yy)
{
        y = yy;
}
void Line::set_x(int xx)
{
        x = xx;
}
//////////////////////// getters /////////////////////////
float Line::get_angle()
{
        return angle;
}
float Line::get_length()
{
        return length;
}
int Line::get_x()
{
        return x;
}
int Line::get_y()
{
        return y;
}
unsigned short Line::get_alpha()
{
        return c.a;
}
/////////////////////////////////////////////// RECTANGLE ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Rectangle {
        public:
                Rectangle();
                Rectangle(bool random, bool outline, Position p, Color c);

                void draw_rectangle();
        private: 
                SDL_Rect rect;
                Color color;
                Position pp;
                bool o;
};


Rectangle::Rectangle()
{
}

Rectangle::Rectangle(bool random, bool outline, Position p, Color c)
{
        if (random){
                int num;
                if (SCREEN_WIDTH >= SCREEN_HEIGHT) {
                        num = SCREEN_WIDTH;
                }
                else {num = SCREEN_HEIGHT;}
                pp.x = random_int(0,num);
                pp.y = random_int(0,num);
                num = num/10;
                pp.w = random_int(0,num);
                pp.h = random_int(0,num);
                color.r = random_int(0,255);
                color.g = random_int(0,255);
                color.b = random_int(0,255);
                color.a = random_int(0,255);
                o = outline;
        }
        else {
                pp.x = p.x;
                pp.y = p.y;
                pp.w = p.w;
                pp.h = p.h;
                color.r = c.r;
                color.g = c.g; 
                color.b = c.b;
                color.a = c.a;
                o = outline;
        }
        draw_rectangle();
}
void Rectangle::draw_rectangle()
{
        rect.x = pp.x;
        rect.y = pp.y;
        rect.w = pp.w;
        rect.h = pp.h;
        SDL_SetRenderDrawColor( gRenderer, color.r, color.g, color.b, color.a);
        if (o) {
                SDL_RenderDrawRect(gRenderer, &rect);       
        }
        else {
                SDL_RenderFillRect(gRenderer, &rect);
        }
}


/////////////////////////////////////////////// OBJECT //////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Object{
        public:
                Object();
                Object(std::string s, Position pos);
                Object(std::string s); 

                void print_object();
               
                int get_x();
                int get_y();
                SDL_Texture* get_pointer(); 
                void set_coordinates(int x, int y);
                void set_dims(int w, int h); 
                void set_pic(std::string s);
                Position get_position();
        private:
                Position p;
                SDL_Rect area;
                SDL_Texture* object_area;
};

Object::Object(){}

Object::Object(std::string s)
{
        object_area = loadTexture(s);
}
Object::Object(std::string s, Position pos)
{
        p.x = pos.x;
        p.y = pos.y;
        p.w = pos.w;
        p.h = pos.h;
        area.x = p.x;
        area.y = p.y;
        area.w = p.w;
        area.h = p.h;
        object_area = loadTexture(s);
}

void Object::print_object()
{
        SDL_RenderCopy( gRenderer, object_area, NULL, &area);
}

void Object::set_pic(std::string s)
{
        if (object_area != NULL)
        {
                SDL_DestroyTexture(object_area);
                object_area = loadTexture(s);
        }
        object_area = loadTexture(s);
}
void Object::set_coordinates(int x, int y)
{
        area.x = x;
        area.y = y;
}

int Object::get_x()
{
        return area.x;
}
int Object::get_y()
{
        return area.y;
}
void Object::set_dims(int w, int h)
{
        area.w = w;
        area.h = h;
}
SDL_Texture* Object::get_pointer()
{
        return object_area;
}
Position Object::get_position()
{
        return p;
}
/////////////////////////////////////////////// TEXTBOX /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Textbox{
        public:
                Textbox();
                Textbox(std::string text, int xx, int yy, SDL_Color c, int size, char *file);
                ~Textbox();

                void print_text();

                int get_width();
                int get_height();
                void set_position(int xx, int yy);
        private:

                void make_text();
                
                SDL_Rect area;
                SDL_Texture* text_area;
                
                std::string phrase;
                SDL_Color color;
                int width, height;
                int x, y;
};

Textbox::Textbox(){}

Textbox::Textbox(std::string text, int xx, int yy, SDL_Color c, int size, char *file)
{
        gFont = TTF_OpenFont(file, size);
        color = c;
        x = xx;
        y = yy;
        phrase = text;

        make_text(); 
        
        area.x = x;
        area.y = y;
        area.w = width;
        area.h = height;
}

Textbox::~Textbox()
{
//        SDL_DestroyTexture( text_area );
}

void Textbox::make_text()
{
        SDL_Surface* text_surface = TTF_RenderText_Solid( gFont, phrase.c_str(), color);
        text_area = SDL_CreateTextureFromSurface( gRenderer, text_surface);
        width = text_surface->w;
        height = text_surface->h;
        SDL_FreeSurface( text_surface );
}

void Textbox::print_text()
{
        SDL_RenderCopy( gRenderer, text_area, NULL, &area);
}

int Textbox::get_width()
{
        return width;
}       
int Textbox::get_height()
{
        return height;
}
void Textbox::set_position(int xx, int yy)
{
        area.x = xx;
        area.y = yy;
}

/////////////////////////////////////////////// KEYSIG //////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


// keysig class takes in a single string for a key, formatted in lower case,
//      if flat or sharp ---> "a-flat" or "c-sharp"

// populates array of integer pitches that should be sharpened or flattened 

class Keysig{
        public:
                Keysig();
                Keysig(string k);

                bool get_sharp_key();
                int get_cap();

        private:
                char sharps[7];
                char flats[7];
                void populate_arrays();
                void initialize_keysig();
                
                string key;
                void determine_accidentals();
                
                int cap;
                int num;
                void expand();

                bool sharp_key;
                int accidental_amount;

                void make_keysig();
                void fill(int integer);
};

Keysig::Keysig(){}

Keysig::Keysig(string k)
{
        cap = 10;
        num = 0;
        gKeysig = new int[cap];
        
        initialize_keysig();
        populate_arrays();
       
        key = k; 
        determine_accidentals();

        make_keysig();
}

void Keysig::initialize_keysig() // initializes the pitches to -1
{
        for (int i = 0; i < cap; i++)
        {
                gKeysig[i] = -1;
        }
}

void Keysig::populate_arrays()
{
        sharps[0] = 'f';
        sharps[1] = 'c';
        sharps[2] = 'g';
        sharps[3] = 'd';
        sharps[4] = 'a';
        sharps[5] = 'e';
        sharps[6] = 'b';

        flats[0] = 'b';
        flats[1] = 'e';
        flats[2] = 'a';
        flats[3] = 'd';
        flats[4] = 'g';
        flats[5] = 'c';
        flats[6] = 'f';
}

void Keysig::determine_accidentals()
{
        if (key == "c")
        {
                sharp_key = true;
                accidental_amount = 0;
                // nothing happens, zero accidentals (all are -1);
        }

        // sharp keys

        if (key == "g")
        {
                sharp_key = true;
                accidental_amount = 1;
        }
        if (key == "d")
        {
                sharp_key = true;
                accidental_amount = 2;
        }
        if (key == "a")
        {
                sharp_key = true;
                accidental_amount = 3;
        }
        if (key == "e")
        {
                sharp_key = true;
                accidental_amount = 4;
        }
        if (key == "b")
        {
                sharp_key = true;
                accidental_amount = 5;
        }
        if (key == "f-sharp")
        {
                sharp_key = true;
                accidental_amount = 6;
        }
        if (key == "c-sharp")
        {
                sharp_key = true;
                accidental_amount = 7;
        }

        // flat keys

        if (key == "f")
        {
                sharp_key = false;
                accidental_amount = 1;
        }
        if (key == "b-flat")
        {
                sharp_key = false;
                accidental_amount = 2;
        }
        if (key == "e-flat")
        {
                sharp_key = false;
                accidental_amount = 3;
        }
        if (key == "a-flat")
        {
                sharp_key = false;
                accidental_amount = 4;
        }
        if (key == "d-flat")
        {
                sharp_key = false;
                accidental_amount = 5;
        }
        if (key == "g-flat")
        {
                sharp_key = false;
                accidental_amount = 6;
        }
        if (key == "c-flat")
        {
                sharp_key = false;
                accidental_amount = 7;
        }
}

void Keysig::make_keysig()
{
        for (int i = 0; i < accidental_amount; i++)
        {
                if (sharp_key)
                {
                        if (sharps[i] == 'c')
                        {
                               fill(C);
                        }           
                        if (sharps[i] == 'd')
                        {
                               fill(D);
                        }           
                        if (sharps[i] == 'e')
                        {
                               fill(E);
                        }           
                        if (sharps[i] == 'f')
                        {
                               fill(F);
                        }           
                        if (sharps[i] == 'g')
                        {
                               fill(G);
                        }           
                        if (sharps[i] == 'a')
                        {
                               fill(A);
                        }           
                        if (sharps[i] == 'b')
                        {
                               fill(B);
                        }           
                }
                else
                {
                        if (flats[i] == 'c')
                        {
                               fill(C);
                        }           
                        if (flats[i] == 'd')
                        {
                               fill(D);
                        }           
                        if (flats[i] == 'e')
                        {
                               fill(E);
                        }           
                        if (flats[i] == 'f')
                        {
                               fill(F);
                        }           
                        if (flats[i] == 'g')
                        {
                               fill(G);
                        }           
                        if (flats[i] == 'a')
                        {
                               fill(A);
                        }           
                        if (flats[i] == 'b')
                        {
                               fill(B);
                        }           
                }
        }
}

void Keysig::fill(int integer)
{
        if (num == cap) {expand();}
        gKeysig[num] = integer;
        num++;
}

void Keysig::expand()
{
        int *temp = new int[cap*2];
        
        for (int i = 0; i < cap*2; i++)
        {
                temp[i] = -1;
        }

        for (int i = 0; i < num; i++)
        {
                temp[i] = gKeysig[i];
        }

        delete []gKeysig;
        gKeysig = temp;
        cap = cap*2;
}

bool Keysig::get_sharp_key()
{
        return sharp_key;        
}

int Keysig::get_cap()
{
        return cap;
}
/////////////////////////////////////////////// STEM ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* The stem class takes in a note's duration, a position of the note (x,y), and
 *      a boolean of whether the note stem is upwards or downwards.
 *
 *      Stem class does not do the following:
 *
 *              The staff class will determine whether the note stem is up or down based on position
 *              The staff class will also determine if a note is singular or part of a group (subdivision)
 *      
 *
 */

class Stem{
        public:
                Stem();
                Stem(float dur, int xx, int yy, bool up_up);

                void draw_stem();
                void set_length(int l);
                void set_position(int xx, int yy);
                void set_direction(bool dir);

                void set_single(bool s);
        private:
                Color color;
                Line *stem;
                int length;
                int x, y;
                bool up;
                float duration;
                bool single;
                bool eighth;
                bool sixteenth;
                
                void create_stem();
                Line *line8;
                Line *line16;
};

Stem::Stem(){}
/*
 *              Constructor with duration, position, up bool:
 *                      decides:
 *                              color
 *                              stem length as global
 *                              bool single as true
 *                              note type based on duration
 *                      saves:
 *                              position
 *                              bool up
 *                      creates:
 *                              stem Line
 *                      calls:
 *                              create_stem()
*/
 Stem::Stem(float dur, int xx, int yy, bool up_up)
{
        color.r = 0; color.g = 0; color.b = 0; color.a = 255;
        
        x = xx;
        y = yy + 3;
        up = up_up;
        length = STEM_LENGTH; 
        duration = dur;
        single = true;
        eighth = false;
        sixteenth = false; 

        int factor = duration * 10;
        if (factor < 10) {eighth = true;}
        if (factor < 5) {sixteenth = true;}
        
        create_stem();
}

/*
 * called by the constructor
 * 
 * creates new lines for tails
 * changes the line specific variables if line points downward
 */
void Stem::create_stem()
{
        int x_lines = x;
        if (up) {
                x_lines += NOTE_WIDTH;
        }
        // initialize these
        line8 = new Line(330, TAIL_LENGTH, x_lines, y - length, color);
        line8->set_thickness(STEM_THICKNESS);
        line16 = new Line(330, TAIL_LENGTH, x_lines, y - length + TAIL_OFFSET, color);
        line16->set_thickness(STEM_THICKNESS);
        
        if (up)
        { 
                stem = new Line(90, length, x_lines, y, color); 
        }
        else 
        { 
                stem = new Line(270, length, x_lines, y, color);
        
                line8->set_angle(150);
                line8->set_y(y+length);
                line16->set_angle(150);
                line16->set_y(y+length - TAIL_OFFSET);
        }
}

/* draws the stem line 
 * 
 */
void Stem::draw_stem()
{
        stem->draw_line();
        if (single && eighth) 
        {
                line8->draw_line();
        }
        if (single && sixteenth)
        {
                line16->draw_line();
        }
}
void Stem::set_length(int l)
{
        stem->set_length(l);
}
void Stem::set_position(int xx, int yy)
{
        stem->set_position(xx, yy);
}
void Stem::set_direction(bool dir)
{
        up = dir;
}

void Stem::set_single(bool s)
{
        single = s;
}

/////////////////////////////////////////////// NOTE ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Note takes in pitch (int), length (float), and if rest (bool)
 *
 */
class Note{
        public:
                Note();
                Note(int p, int o, string s, float l, bool r, int staf);
                
                void draw_note();

                void set_object(int x, int y, int w, int h);
                

                int get_pitch();
                bool get_stem();
                bool get_dot();
                int get_y();
                int get_octave();
                string get_accidental();
                bool get_rest();
                int get_x();
                
                void set_pitch(int pit);
                void set_x(int xx);
                
                void set_position(int n);
                int get_position();

        private:
                int staff;
                bool stem_up;
               
                string accidental; 
                int octave;
                int pitch;
                float length; // 4 is whole note
                bool rest;
                
                int x_pos, y_pos;
                
                void determine_stem_direction();               
                Stem *note_stem;
                Object *note_dot;
                void create_stem_and_dot();

                Object *note;
                bool dot;
                bool stem;
                
                void configure_note();
                
                // stores the position of each note for Performer to use
                int position_on_staff;

};

Note::Note(){}

Note::Note(int p, int o, string s, float l, bool r, int staf)
{
        position_on_staff = 0;
        pitch = p;
        octave = o;
        length = l;
        accidental = s;
        rest = r;
        dot = false;
        stem = false;
        stem_up = true;
        staff = staf;
        note = new Object("visuals/note.png"); 
        configure_note();
}

void Note::configure_note()
{
        int factor = length * 100;
        /*
         * 400 whole
         * 300 dotted half
         * 200 half
         * 150 dotted quarter
         * 100 quarter
         * 75 dotted eighth
         * 50 eighth
         * 37 dotted sixteenth
         * 25 sixteenth
         * 
         */
        
        if (factor >= 200){note->set_pic("visuals/note_rim.png");}
        if (factor == 37 || factor == 75 || factor == 150 || factor == 300) {dot = true;}
        if (factor < 400){ stem = true;}


        if (rest)
        {
                if (factor < 50) // 16th
                {
                        note->set_pic("visuals/16th_rest.png");
                }
                if (factor >= 50 && factor < 100) // 8th
                {
                        note->set_pic("visuals/8th_rest.png");
                }
                if (factor >= 100 && factor < 200) // quarter
                {        
                        note->set_pic("visuals/rest.png");
                }
                if (factor >= 200 && factor < 400) // half
                {
                        note->set_pic("visuals/half_rest.png");
                }
                if (factor >= 400) // whole
                {
                        note->set_pic("visuals/whole_rest.png");
                }
        }
}


void Note::set_object(int x, int y, int w, int h)
{
        x_pos = x;
        y_pos = y;

        note->set_coordinates(x_pos, y_pos);
        if (rest){note->set_dims(w*1.5, h*2);}
        else {note->set_dims(w, h);}
        
        create_stem_and_dot();
}
void Note::create_stem_and_dot()
{
        determine_stem_direction();
        note_stem = new Stem(length, x_pos, y_pos, stem_up);
        
        Position p_dot = {x_pos + DOT_OFFSET, y_pos + ROW_DIST, DOT_DIM, DOT_DIM};
        if (rest) {
                p_dot.x = x_pos + NOTE_HEIGHT*2;
        }
        note_dot = new Object("visuals/dot.png", p_dot);
}
void Note::determine_stem_direction()
{
        if ((octave == 4 && pitch <= B) || octave <= 2)
        {
                stem_up = false;
        }
}
void Note::draw_note()
{
        if (rest) {stem = false;}
        if (stem)
        {
                note_stem->draw_stem();
        }
        if (dot)
        {
                note_dot->print_object();
        }
        note->print_object();
}

/////////// setters and getters /////////////

int Note::get_pitch()
{
        return pitch;
}
void Note::set_pitch(int pit)
{
        pitch = pit;
}
bool Note::get_stem()
{
        return stem;
}
bool Note::get_dot()
{
        return dot;
}
int Note::get_y()
{
        return y_pos;
}

int Note::get_octave()
{
        return octave;
}
string Note::get_accidental()
{
        return accidental;
}
bool Note::get_rest()
{
        return rest;
}
int Note::get_x()
{
        return x_pos;
}
void Note::set_x(int xx)
{
        x_pos = xx;
}
void Note::set_position(int n)
{
        position_on_staff = n;
}
int Note::get_position()
{
        return position_on_staff;
}
/////////////////////////////////////////////// STAFF ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////// expandable array of lines ///////////////////////////////

int num_lines = 0;
int cap_lines = 10;
Line *gLines = new Line[cap_lines];

void expand_lines()
{
        Line *temp = new Line[cap_lines*2];
        
        for (int i = 0; i < num_lines; i++)
        {
                temp[i] = gLines[i];
        }

        delete []gLines;
        gLines = temp;
        cap_lines = cap_lines*2;
}
///////////////////////////////////////////////////////////////////////////////

struct Row {
        int pos_y; // pixel position, translated into line
        int pitch; // current pitch content
        int pitch_flat;
        int pitch_sharp;
        int octave;

        // accidentals
        bool sharp; // if both are false, natural
        bool flat;
        bool double_s;
        bool double_f;
        
        bool line_bool; // line or space
        bool ledger;
};

struct Ledger_Node{
        Line line;
        Ledger_Node *next;
};

class Staff{
        public:
                Staff();
                ~Staff();
                Staff(string key, int x, int y, int beats, int division);
                
                void draw_staff();
                void place_note(Note *note, int pos);
                
                int get_keysig_pos();
                 
        private:
                Row rows[STAFF_ROWS];
                int pos_y, pos_x;
                string key_string;
                Color line_color;

                bool sharps;
                int capacity_keysig;

                void make_staff();
                void make_lines();
                
                Position p_treb;
                Object *treble;

                Position p_bass;
                Object *bass;

                void determine_accidentals();
                int keysig_pos;
                int keysig_pos_increment;

                Object *accidentals;
                int cap_acc;
                int num_acc;
                void expand_acc();
                int acc_dim;
                
                void make_timesig();
                int top;
                int bottom;
                Textbox *timesig_beats;
                Textbox *timesig_division;
                Textbox *timesig_beats_copy;
                Textbox *timesig_division_copy;
                
                Object object_on_row(Object o, Row r, int x, int offset_x, int offset_y);
                
                Ledger_Node *head;
                Ledger_Node *iterator;
                void add_ledger();
                void print_ledgers();
};

Staff::Staff(){}

Staff::~Staff()
{
        delete treble;
        delete bass;
}

Staff::Staff(string key, int x, int y, int beats, int division)
{
        key_string = key;
        Keysig k(key_string);
        sharps = k.get_sharp_key(); 
        capacity_keysig = k.get_cap();
        pos_y = y;
        pos_x = x;
        line_color.r = 0;
        line_color.g = 0;
        line_color.b = 0;
        line_color.a = 255;
        
        p_treb.x = pos_x;
        p_treb.y = pos_y + ROW_DIST*5;
        p_treb.w = 40 *ROW_DIST/10;
        p_treb.h = 110 *ROW_DIST/10;;
        treble = new Object("visuals/treble.png", p_treb);

        p_bass.x = pos_x;
        p_bass.y = pos_y + ROW_DIST*18;
        p_bass.w = 40 *ROW_DIST/10;
        p_bass.h = 80 *ROW_DIST/10;
        bass = new Object("visuals/bass.png", p_bass);

        keysig_pos = p_treb.x + p_treb.w + 10;
        keysig_pos_increment = INCREMENT_KEYSIG;

        cap_acc = 10;
        num_acc = 0;
        accidentals = new Object[cap_acc];
        acc_dim = ACC_DIM;
        
        top = beats;
        bottom = division;
        
        make_staff();
        make_lines();
        determine_accidentals();
        make_timesig();

        head = NULL;
        iterator = NULL;
}

void Staff::make_staff()
{
                int temp_pos = pos_y;
                for (int i = 0; i < STAFF_ROWS; i++)
                {
                        rows[i].pos_y = temp_pos;
                        temp_pos += ROW_DIST;
                        
                        rows[i].sharp = false;
                        rows[i].flat = false;
                        rows[i].double_s = false;
                        rows[i].double_f = false;
                        rows[i].line_bool = false;
                        rows[i].ledger = false;
                    //    rows[i].pitch_sharp = rows[i].pitch + 1;
                     //   rows[i].pitch_flat = rows[i].pitch - 1;
                }

                // pitches and lines
                rows[0].pitch = E; rows[0].ledger = true;
                rows[1].pitch = D;
                rows[2].pitch = C; rows[2].ledger = true;
                rows[3].pitch = B;
                rows[4].pitch = A; rows[4].ledger = true;
                rows[5].pitch = G;
                rows[6].pitch = F; rows[6].line_bool = true;
                rows[7].pitch = E;
                rows[8].pitch = D; rows[8].line_bool = true;
                rows[9].pitch = C;
                rows[10].pitch = B; rows[10].line_bool = true;
                rows[11].pitch = A;
                rows[12].pitch = G; rows[12].line_bool = true;
                rows[13].pitch = F;
                rows[14].pitch = E; rows[14].line_bool = true;
                rows[15].pitch = D;
                rows[16].pitch = C; rows[16].ledger = true;
                rows[17].pitch = B;
                rows[18].pitch = A; rows[18].line_bool = true;
                rows[19].pitch = G;
                rows[20].pitch = F; rows[20].line_bool = true;
                rows[21].pitch = E;
                rows[22].pitch = D; rows[22].line_bool = true;
                rows[23].pitch = C;
                rows[24].pitch = B; rows[24].line_bool = true;
                rows[25].pitch = A;
                rows[26].pitch = G; rows[26].line_bool = true;
                rows[27].pitch = F;
                rows[28].pitch = E; rows[28].ledger = true;
                rows[29].pitch = D;
                rows[30].pitch = C; rows[30].ledger = true;
                rows[31].pitch = B;
                rows[32].pitch = A; rows[32].ledger = true;
                for (int i = 0; i <= 2; i++)
                {
                        rows[i].octave = 6;
                }
                for (int i = 3; i <= 9; i++)
                {
                        rows[i].octave = 5;
                }
                for (int i = 10; i <= 16; i++)
                {
                        rows[i].octave = 4;
                }
                for (int i = 17; i <= 23; i++)
                {
                        rows[i].octave = 3;
                }
                for (int i = 24; i <= 30; i++)
                {
                        rows[i].octave = 2;
                }
                for (int i = 31; i <= 32; i++)
                {
                        rows[i].octave = 1;
                }
                for (int i = 0; i < STAFF_ROWS; i++)
                {
                        rows[i].pitch_sharp = rows[i].pitch + 1;
                        rows[i].pitch_flat = rows[i].pitch - 1;
                }

}

void Staff::make_lines()
{
        for (int i = 0; i < STAFF_ROWS; i++)
        {
                if (rows[i].line_bool) 
                {
                        if (num_lines == cap_lines) {expand_lines();}
                        gLines[num_lines].set_color(line_color);
                        gLines[num_lines].set_angle(0.0);
                        gLines[num_lines].set_length((float)SCREEN_WIDTH - pos_x*2);
                        gLines[num_lines].set_position(pos_x, rows[i].pos_y);
                       
                        num_lines++;
                }
        }
}

void Staff::determine_accidentals()
{
        for (int i = 0; i < capacity_keysig; i++)
        {
                if (sharps)
                {
                                for (int j = 0; j < STAFF_ROWS; j++)
                                {
                                        if (rows[j].pitch == gKeysig[i])
                                        {
                                                rows[j].sharp = true;
                                        }
                                }
                                for (int j = 5; j <= 11; j++)
                                {
                                        if (rows[j].pitch == gKeysig[i])
                                        {
                                                if (num_acc == cap_acc){expand_acc();}
                                                Object acc("visuals/sharp.png");
                                                acc = object_on_row(acc, rows[j], keysig_pos, 0, 0);
                                                accidentals[num_acc] = acc;
                                                num_acc++;
                                                keysig_pos += keysig_pos_increment;

                                        }
                                }
                                for (int j = 19; j <= 25; j++)
                                {
                                        if (rows[j].pitch == gKeysig[i])
                                        {
                                                if (num_acc == cap_acc){expand_acc();}
                                                Object acc("visuals/sharp.png");
                                                acc = object_on_row(acc, rows[j], keysig_pos, 0, 0);
                                                accidentals[num_acc] = acc;
                                                num_acc++;
                                                keysig_pos += keysig_pos_increment;

                                        }
                                }
                }
                else
                {
                                for (int j = 0; j < STAFF_ROWS; j++)
                                {
                                        if (rows[j].pitch == gKeysig[i])
                                        {
                                                rows[j].flat = true;
                                        }
                                }
                                for (int j = 7; j <= 13; j++)
                                {
                                        if (rows[j].pitch == gKeysig[i])
                                        {
                                                if (num_acc == cap_acc){expand_acc();}
                                                Object acc("visuals/flat.png");
                                                acc = object_on_row(acc, rows[j], keysig_pos, 0, -3*SCALE/6);
                                                accidentals[num_acc] = acc;
                                                num_acc++;
                                                keysig_pos += keysig_pos_increment;
                                        }
                                }
                                for (int j = 21; j <= 27; j++)
                                {
                                        if (rows[j].pitch == gKeysig[i])
                                        {
                                                if (num_acc == cap_acc){expand_acc();}
                                                Object acc("visuals/flat.png");
                                                acc = object_on_row(acc, rows[j], keysig_pos, 0, -3*SCALE/6);
                                                accidentals[num_acc] = acc;
                                                num_acc++;
                                                keysig_pos += keysig_pos_increment;
                                        }
                                }
                }
        }
}

void Staff::make_timesig()
{        
        std::string times = "visuals/times.ttf";
        char *filepath_times =  &times[0];

        SDL_Color textcolor;
        textcolor.r = 0;
        textcolor.g = 0;
        textcolor.b = 0;

        std::string top_s = std::to_string(top);
        std::string bottom_s = std::to_string(bottom);

        timesig_beats = new Textbox(top_s, keysig_pos + keysig_pos_increment*2, 
                        TOP_OF_STAVES + GAP + ROW_DIST*5 + 3, textcolor, 45 * ROW_DIST/10, filepath_times);
        timesig_division = new Textbox(bottom_s, keysig_pos + keysig_pos_increment*2, 
                        TOP_OF_STAVES + GAP + ROW_DIST*9 + 3, textcolor, 45 * ROW_DIST/10, filepath_times);
        timesig_beats_copy = new Textbox(top_s, keysig_pos + keysig_pos_increment*2, 
                        TOP_OF_STAVES + GAP + ROW_DIST*17 + 3, textcolor, 45 * ROW_DIST/10, filepath_times);
        timesig_division_copy = new Textbox(bottom_s, keysig_pos + keysig_pos_increment*2, 
                        TOP_OF_STAVES + GAP + ROW_DIST*21 + 3, textcolor, 45 * ROW_DIST/10, filepath_times);
}


void Staff::draw_staff()
{
        for (int i = 0; i < num_lines; i++)
        {
                gLines[i].draw_line();
        }
        
        treble->print_object();
        bass->print_object();
        
        for (int i = 0; i < num_acc; i++)
        {
                accidentals[i].print_object();
        }
        
        timesig_beats->print_text();
        timesig_division->print_text();
        timesig_beats_copy->print_text();
        timesig_division_copy->print_text();

        print_ledgers();
}

Object Staff::object_on_row(Object o, Row r, int x, int offset_x, int offset_y)
{
        o.set_coordinates(x + offset_x, r.pos_y - ROW_DIST + offset_y);
        o.set_dims(acc_dim, acc_dim);
        return o;
}

void Staff::expand_acc()
{
        Object *temp = new Object[cap_acc*2];
        
        for (int i = 0; i < num_acc; i++)
        {
                temp[i] = accidentals[i];
        }

        delete []accidentals;
        accidentals = temp;
        cap_acc = cap_acc*2;
}
int Staff::get_keysig_pos()
{
        return keysig_pos;
}

void Staff::add_ledger()
{
        Ledger_Node *temp = new Ledger_Node;
        temp->line.set_angle(0);
        temp->line.set_color(line_color);
        temp->line.set_length(LINE_WIDTH);
        temp->next = head;
        head = temp;
}

void Staff::print_ledgers()
{
        iterator = head;
        while (iterator != NULL)
        {
                iterator->line.draw_line();
                iterator = iterator->next;
        }
}


void Staff::place_note(Note *n, int pos)
{
//      for now, staff considers notes as white keys in order to populate staff.
//      staff considers string accidental of note when populate accidentals before or after
//      the note.
        
        for (int i = 0; i < STAFF_ROWS; i++)
        {
                if (rows[i].pitch == n->get_pitch() && rows[i].octave == n->get_octave())
                {
                        if (!n->get_rest())
                        {
                                if ((rows[i].sharp || rows[i].flat) && n->get_accidental() == "natural")
                                {
                                        if (num_acc == cap_acc){expand_acc();}
                                        Object acc("visuals/natural.png");
                                        acc = object_on_row(acc, rows[i], pos - ACC_SHIFT, 0, 0);
                                        accidentals[num_acc] = acc;
                                        num_acc++;
                                }
                                if (n->get_accidental() == "sharp" && !rows[i].sharp)
                                {
                                        if (num_acc == cap_acc){expand_acc();}
                                        Object acc("visuals/sharp.png");
                                        acc = object_on_row(acc, rows[i], pos - ACC_SHIFT, 0, 0);
                                        accidentals[num_acc] = acc;
                                        num_acc++;
                                }
                                if (n->get_accidental() == "flat" && !rows[i].flat)
                                {
                                        if (num_acc == cap_acc){expand_acc();}
                                        Object acc("visuals/flat.png");
                                        acc = object_on_row(acc, rows[i], pos - ACC_SHIFT, 0, -3*SCALE/6);
                                        accidentals[num_acc] = acc;
                                        num_acc++;
                                }
                        }
                        n->set_object(pos, rows[i].pos_y - ROW_DIST, NOTE_WIDTH, NOTE_HEIGHT);
                        n->set_position(pos);
                        
                        int line_pos = pos - LINE_WIDTH/5;
                        // ledgers
                        if (rows[i].ledger)
                        {
                                add_ledger();
                                head->line.set_position(line_pos, rows[i].pos_y);
                        }
                        if (n->get_octave() == 6)
                        {
                                if (n->get_pitch() == C)
                                {
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i + 2].pos_y);
                                }
                                if (n->get_pitch() == D)
                                {
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i + 1].pos_y);
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i + 3].pos_y);
                                }
                                if (n->get_pitch() == E)
                                {
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i + 2].pos_y);
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i + 4].pos_y);
                                }
                        }
                        if (n->get_octave() == 5)
                        {
                                if (n->get_pitch() == A)
                                {
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i].pos_y);
                                }
                                if (n->get_pitch() == B)
                                {
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i + 1].pos_y);
                                }
                        }
                        if (n->get_octave() == 2)
                        {
                                if (n->get_pitch() == C)
                                {
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i - 2].pos_y);
                                }
                                if (n->get_pitch() == D)
                                {
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i - 1].pos_y);
                                }
                        }
                        if (n->get_octave() == 1)
                        {
                                if (n->get_pitch() == B)
                                {
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i - 1].pos_y);
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i - 3].pos_y);
                                }
                                if (n->get_pitch() == A)
                                {
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i - 2].pos_y);
                                        add_ledger();
                                        head->line.set_position(line_pos, rows[i - 4].pos_y);
                                }
                        }
                } 
        }
}
/////////////////////////////////////////////// CHORD ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

struct NoteSpec{
        int pitch;
        int octave;
        string accidental;
        float duration;
        bool rest;
        string four_parts; // saves whether note is bass, tenor, alto, suprano
                                // in order to later manipulate by reading through 
                                // part lines
};

struct ChordStruct{
        NoteSpec root;
        NoteSpec third;
        NoteSpec fifth;
        NoteSpec seventh;
        string quality;
        bool raised;
        bool sharp_three;
        int scale_degree;
};

class Chord{
        public:
                Chord();
                Chord(int cord);

                ChordStruct get_chordstruct();
                void replace_chordstruct(ChordStruct cord);

        private:
                ChordStruct c;
                int chord;
                void determine_notes();

                int shrink(int num);
                int* make_white_scale(int n);
};

Chord::Chord(){}

Chord::Chord(int cord)
{
        chord = cord;

        determine_notes();
}

void Chord::determine_notes()
{
        Keysig keysig(KEY);
        int *scale;
        scale = make_white_scale(C);
        if (KEY == "g") {scale = make_white_scale(G);} 
        if (KEY == "d") {scale = make_white_scale(D);}
        if (KEY == "a") {scale = make_white_scale(A);}
        if (KEY == "e") {scale = make_white_scale(E);}
        if (KEY == "b") {scale = make_white_scale(B);}
        if (KEY == "f-sharp") {scale = make_white_scale(F);}
        if (KEY == "c-sharp") {scale = make_white_scale(C);}
        
        if (KEY == "f") {scale = make_white_scale(F);}
        if (KEY == "g-flat") {scale = make_white_scale(G);} 
        if (KEY == "d-flat") {scale = make_white_scale(D);}
        if (KEY == "a-flat") {scale = make_white_scale(A);}
        if (KEY == "e-flat") {scale = make_white_scale(E);}
        if (KEY == "b-flat") {scale = make_white_scale(B);}
        if (KEY == "c-flat") {scale = make_white_scale(C);}
        
        c.raised = false;
        c.sharp_three = false;
        int root_index = 0;
        if (MINOR)
        {
                if (chord >= 8) {chord -= 7;}
                
                if (chord == 1) {c.root.pitch = scale[5]; c.quality = "min"; root_index = 5;}
                if (chord == 2) {c.root.pitch = scale[6]; c.quality = "dim"; root_index = 6;}
                if (chord == 3) {c.root.pitch = scale[0]; c.quality = "maj"; root_index = 0;}
                if (chord == 4) {c.root.pitch = scale[1]; c.quality = "min"; root_index = 1;}
                if (chord == 5) {c.root.pitch = scale[2];
                        c.quality = "maj";
                        c.sharp_three = true;
                        root_index = 2;
                }
                if (chord == 6) {c.root.pitch = scale[3]; c.quality = "maj"; root_index = 3;}
                if (chord == 7) {c.root.pitch = scale[4];
                        c.quality = "maj";
                        c.raised = true;
                        root_index = 4;
                }
        }
        else {
                if (chord == 1) {c.root.pitch = scale[0]; c.quality = "maj"; root_index = 0;}
                if (chord == 2) {c.root.pitch = scale[1]; c.quality = "min"; root_index = 1;}
                if (chord == 3) {c.root.pitch = scale[2]; c.quality = "min"; root_index = 2;}
                if (chord == 4) {c.root.pitch = scale[3]; c.quality = "maj"; root_index = 3;}
                if (chord == 5) {c.root.pitch = scale[4]; c.quality = "maj"; root_index = 4;}
                if (chord == 6) {c.root.pitch = scale[5]; c.quality = "min"; root_index = 5;}
                if (chord == 7) {c.root.pitch = scale[6]; c.quality = "dim"; root_index = 6;}
        }
        
        c.scale_degree = chord;
        
        int root_index_two = root_index - 7; 
        if (root_index + 2 < 7){c.third.pitch = scale[root_index + 2];}
        else {
                c.third.pitch = scale[root_index_two + 2];
        }      
        if (root_index + 4 < 7){c.fifth.pitch = scale[root_index + 4];}
        else {
                c.fifth.pitch = scale[root_index_two + 4];
        }

        c.seventh.pitch = shrink(c.root.pitch + m7);

        
        // loop through gKeySig to see if the note is in there
        c.root.accidental = "natural";
        c.third.accidental = "natural";
        c.fifth.accidental = "natural";
        
        int cap = keysig.get_cap();
        bool sharp = keysig.get_sharp_key();
        for (int i = 0; i < cap; i++)
        {
                if (sharp)
                {
                        if (c.root.pitch == gKeysig[i]){c.root.accidental = "sharp";}
                        if (c.third.pitch == gKeysig[i]){c.third.accidental = "sharp";}
                        if (c.fifth.pitch == gKeysig[i]){c.fifth.accidental = "sharp";}
                }
                else
                {
                        if (c.root.pitch == gKeysig[i]){c.root.accidental = "flat";}
                        if (c.third.pitch == gKeysig[i]){c.third.accidental = "flat";}
                        if (c.fifth.pitch == gKeysig[i]){c.fifth.accidental = "flat";}
                }
        }
        if (sharp)
        {
                if (c.sharp_three){c.third.accidental = "sharp";}
                if (c.raised){c.root.accidental = "sharp";}
        }
        else
        {
                if (c.sharp_three){c.third.accidental = "natural";}
                if (c.raised){c.root.accidental = "natural";}
        }
        
        delete [] scale;
}

int* Chord::make_white_scale(int n)
{
        int *scale = new int[7];
        scale[0] = C;
        scale[1] = D;
        scale[2] = E;
        scale[3] = F;
        scale[4] = G;
        scale[5] = A;
        scale[6] = B;
        int index = 0;
        for (int i = 0; i < 7; i++)
        {
                if (n == scale[i])
                {
                        index = i;
                }
        }
        int *reorder = new int[7];
        int i = 0;
        while (index < 7)
        {
                reorder[i] = scale[index];
                i++;
                index++;
        }
        index = 0;
        while (i < 7)
        {
                reorder[i] = scale[index];
                i++;
                index++;
        }
        return reorder;
}


// shrinks the range to every note less than an octave
int Chord::shrink(int num)
{
        if (num == 12) {return 0;}
        while (num < 0 || num > 11)
        {
                if (num < 0)
                {
                        num += 12;
                }
                if (num > 12)
                {
                        num -= 12;
                }
        }
        return num;
}
ChordStruct Chord::get_chordstruct()
{
        return c;
}
void Chord::replace_chordstruct(ChordStruct cord) // used to modify in composer class
{
        c = cord;
}
/////////////////////////////////////////////// PROGRESSION //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class Progression{
        public:
                Progression();
                int* get_progression();
                int get_num();
        private:
                void make_progression();
                
                int *chords;
                int num_chords;
                int cap_chords;
                void expand_progression();

                void tonic_prolongation();
                void cadence();

                void fill(int n);
                int previous_chord;
};

Progression::Progression()
{
        cap_chords = 5;
        num_chords = 0;
        chords = new int[cap_chords];
        make_progression();
}

void Progression::make_progression()
{
        tonic_prolongation();
        cadence();
}

void Progression::tonic_prolongation()
{
        fill(1);
        fill(5);
        fill(1);
        fill(1);
        fill(6);
        fill(4);
        fill(5);
        fill(1);
        fill(1);
        fill(5);
        fill(1);
        fill(4);
        fill(2);
        fill(5);
        fill(1);
        fill(1);
        
        fill(1);
        fill(5);
        fill(1);
        fill(1);
        fill(6);
        fill(4);
        fill(5);
        fill(1);
        fill(1);
        fill(5);
        fill(1);
        fill(4);
        fill(2);
        fill(5);
        fill(1);


        fill(1);
        fill(5);
        fill(1);
        fill(1);
        fill(6);
        fill(4);
        fill(5);
        fill(1);
        fill(1);
        fill(5);
        fill(1);
        fill(4);
        fill(2);
        fill(5);
        fill(1);
        fill(1);
        
        fill(1);
        fill(5);
        fill(1);
        fill(1);
        fill(6);
        fill(4);
        fill(5);
        fill(1);
        fill(1);
        fill(5);
        fill(1);
        fill(4);
        fill(2);
        fill(5);
        fill(1);

        //        previous_chord = 1;
}
void Progression::fill(int n)
{
        if (num_chords == cap_chords) {expand_progression();}

        chords[num_chords] = n;
        num_chords++;
}

void Progression::cadence()
{
}

void Progression::expand_progression()
{
        int *temp = new int[cap_chords*2];
        
        for (int i = 0; i < num_chords; i++)
        {
                temp[i] = chords[i];
        }

        delete []chords;
        chords = temp;
        cap_chords = cap_chords*2;
}
int* Progression::get_progression()
{
        return chords;
}
int Progression::get_num()
{
        return num_chords;
}
/////////////////////////////////////////////// COMPOSER /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class Composer{
        public:
                Composer(); // compiles chords and considers 4 part lines as stored in NoteSpec
                NoteSpec* get_note_array_pointer();
                int get_num_notes();
                void compose();
                
        private:
                Progression progression;
                NoteSpec *notes; // compiles an array of note information to be translated by the piece
                int notes_num;
                int notes_cap;  
                void expand_notes();
                NoteSpec temp_notes[4];

                void make_chord(int n);
                void unpackage_chord(Chord* chord);
                void smooth_lines();
                
                void limit_boundaries();
                int order(int pitch, int octave);

                // tracks current pitch-octave ordered int in each line
                int line_suprano;
                int line_alto;
                int line_tenor;
                int line_bass;

                bool used[4]; // if used, skip
                void clear_used();
                
                void set_lines();
                void first_chord();

                int get_nearest_octave(int location1, int pitch2);
                int note_distance(int one, int two);
                void populate_next_note(int passed_line);

                int bass_interval;
                int num_const;
                bool motion; // true is similar
                bool bass_direction; // true is up

                void make_bass();
                void make_suprano();
                void make_alto();
                void make_tenor();

                bool seventh;
                int random_indexing();
};

/// have a temporary array of notes in the chord, decide where they each go in 
// the actual notes array
// algorithm: decides how many of each note, and what octave based on previous notes

Composer::Composer()
{
        seventh = false;
        clear_used();
        notes_num = 0;
        notes_cap = 10;
        notes = new NoteSpec[notes_cap];
        first_chord();
}
void Composer::first_chord()
{
        Chord chord(1);
        unpackage_chord(&chord);

        notes[0] = temp_notes[0]; // root
        notes[0].octave = 2;
       
        // upper voices

        notes[1] = temp_notes[1]; // third
        notes[1].octave = 4;
        
        notes[2] = temp_notes[2]; // fifth
        notes[2].octave = 4;
        
        notes[3] = temp_notes[0]; // doubled root
        notes[3].octave = random_int(3, 5);

        notes[4].pitch = -1;

        notes_num += 5;
        
        line_bass = order(notes[0].pitch, notes[0].octave);
        line_tenor = order(notes[1].pitch, notes[1].octave);
        line_alto = order(notes[2].pitch, notes[2].octave);
        line_suprano = order(notes[3].pitch, notes[3].octave);
}
void Composer::clear_used()
{
        for (int i = 0; i < 3; i++)
        {
                used[i] = false;
        }
}
void Composer::make_chord(int n)
{
        Chord chord(n);
        unpackage_chord(&chord);
        
        // operate on chord, make it more than three notes
        
        clear_used();
        // reads through previous chord, makes a note for previous pitch
        make_bass();
        make_suprano();
        make_alto();
        make_tenor();

        // adds a chord of pitch -1 to the end of every chord
        NoteSpec chordend;
        chordend.pitch = -1; // SENTINEL for chord reading 
        if (notes_num == notes_cap) {expand_notes();}
        notes[notes_num] = chordend;
        notes_num++;

}
void Composer::make_bass()
{
        // 3 random choices, any note in next chord
        int random_index = random_indexing();
        int octave = get_nearest_octave(line_bass, temp_notes[random_index].pitch);
        
        if (notes_num == notes_cap) {expand_notes();}
        notes[notes_num] = temp_notes[random_index];
        notes[notes_num].octave = octave;
        bass_interval = note_distance(line_bass, order(notes[notes_num].pitch, notes[notes_num].octave));
        
        if (line_bass > order(notes[notes_num].pitch, notes[notes_num].octave)) {bass_direction = false;}
        if (line_bass < order(notes[notes_num].pitch, notes[notes_num].octave)) {bass_direction = true;}
       
        // update bass line
        line_bass = order(notes[notes_num].pitch, notes[notes_num].octave);

        notes_num++;
        
        if (bass_interval == P5) {motion = false;}
        else if (bass_interval == P4) {motion = true;}
        else if (bass_interval == m3 || bass_interval == M3) {motion = false;}
        else if (bass_interval == m2 || bass_interval == M2) {motion = false;}
        
       // if(motion) {cout << "similar" << endl;}
       // else {cout << "contrary" << endl;}
        // check if bass_interval = 0 later
}
void Composer::make_suprano()
{
        int random_index = random_indexing();
        int octave = get_nearest_octave(line_suprano, temp_notes[random_index].pitch);
        
        if (notes_num == notes_cap) {expand_notes();}
        notes[notes_num] = temp_notes[random_index];
        notes[notes_num].octave = octave;
        
        line_suprano = order(notes[notes_num].pitch, notes[notes_num].octave);

        notes_num++; 

}
void Composer::make_alto()
{
        int random_index = random_indexing();
        int octave = get_nearest_octave(line_alto, temp_notes[random_index].pitch);
        
        if (notes_num == notes_cap) {expand_notes();}
        notes[notes_num] = temp_notes[random_index];
        notes[notes_num].octave = octave;
        
        line_alto = order(notes[notes_num].pitch, notes[notes_num].octave);

        notes_num++; 

}
void Composer::make_tenor()
{
        int random_index = random_int(0, 2);
        int octave = get_nearest_octave(line_tenor, temp_notes[random_index].pitch);
        
        if (notes_num == notes_cap) {expand_notes();}
        notes[notes_num] = temp_notes[random_index];
        notes[notes_num].octave = octave;
        
        line_tenor = order(notes[notes_num].pitch, notes[notes_num].octave);

        notes_num++; 

}
int Composer::random_indexing()
{
        int random_index = random_int(0, 2);
        if (seventh) // adds 7th
        {
                while (used[random_index] == true)
                {
                        random_index = random_int(0, 3);
                }
        }
        else
        {
                while (used[random_index] == true)
                {
                        random_index = random_int(0, 2);
                }
        }
        used[random_index] = true;
        return random_index;
}
// takes in a chord and fills *notes with the notes
void Composer::unpackage_chord(Chord* chord)
{
        seventh = false;
        ChordStruct structure = chord->get_chordstruct();
        
        temp_notes[0] = structure.root;
        temp_notes[1] = structure.third;
        temp_notes[2] = structure.fifth;
        temp_notes[3] = structure.seventh;
        if (structure.scale_degree == 2 || structure.scale_degree == 7)
        {
                seventh = true;
        }
}

void Composer::limit_boundaries()
{
        for (int i = 0; i < notes_num; i++)
        {
                if (notes[i].pitch != -1)
                {
                        if (order(notes[i].pitch, notes[i].octave) < order(A, 1))
                        {
                                notes[i].octave++;
                        }
                        if (order(notes[i].pitch, notes[i].octave) > order(E, 6))
                        {
                                notes[i].octave--;
                        }
                }
        }
}


void Composer::set_lines()
{
        // extremes, suprano and bass
        if (order(notes[0].pitch, notes[0].octave) >=  
                order(notes[1].pitch, notes[1].octave) &&  
                order(notes[0].pitch, notes[0].octave) >=  
                order(notes[2].pitch, notes[2].octave))
        {
                used[0] = true;
                line_suprano = order(notes[0].pitch, notes[0].octave);
        }
        if (order(notes[1].pitch, notes[1].octave) >=  
                order(notes[0].pitch, notes[0].octave) &&  
                order(notes[1].pitch, notes[1].octave) >=  
                order(notes[2].pitch, notes[2].octave))
        {
                used[1] = true;
                line_suprano = order(notes[1].pitch, notes[1].octave);
        }
        if (order(notes[2].pitch, notes[2].octave) >=  
                order(notes[1].pitch, notes[1].octave) &&  
                order(notes[2].pitch, notes[2].octave) >=  
                order(notes[0].pitch, notes[0].octave))
        {
                used[2] = true;
                line_suprano = order(notes[2].pitch, notes[2].octave);
        }

        if (order(notes[0].pitch, notes[0].octave) <=  
                order(notes[1].pitch, notes[1].octave) &&  
                order(notes[0].pitch, notes[0].octave) <=  
                order(notes[2].pitch, notes[2].octave))
        {
                used[0] = true;
                line_bass = order(notes[0].pitch, notes[0].octave);
        }
        if (order(notes[1].pitch, notes[1].octave) <=  
                order(notes[0].pitch, notes[0].octave) &&  
                order(notes[1].pitch, notes[1].octave) <=  
                order(notes[2].pitch, notes[2].octave))
        {
                used[1] = true;
                line_bass = order(notes[1].pitch, notes[1].octave);
        }
        if (order(notes[2].pitch, notes[2].octave) <=  
                order(notes[1].pitch, notes[1].octave) &&  
                order(notes[2].pitch, notes[2].octave) <=  
                order(notes[0].pitch, notes[0].octave))
        {
                used[2] = true;
                line_bass = order(notes[2].pitch, notes[2].octave);
        }

        for (int i = 0; i < 3; i++)
        {
                if(used[i] == false)
                {
                        line_alto = order(notes[i].pitch, notes[i].octave);
                        used[i] = true;
                }
        }
        clear_used();
}
int Composer::get_nearest_octave(int location1, int pitch2)
{
        int octave = 0;
        int dist = note_distance(location1, order(pitch2, octave));
        for (int i = 0; i < 7; i++)
        {
                if (note_distance(location1, order(pitch2, i)) <= dist)
                {
                        dist = note_distance(location1, order(pitch2, i));
                        octave = i;
                }
        }  
        return octave;
}

int Composer::note_distance(int one, int two)
{
        return abs(one - two);
}

//////// compose the piece ///////////////

void Composer::compose()
{
        int* pointer = progression.get_progression();
        int num = progression.get_num();
        for (int i = 0; i < num; i++)
        {
                make_chord(pointer[i]);
        }
        limit_boundaries();
}

/*
void Composer::smooth_lines()
{
        clear_used();
        if (notes_num > 4) // checks that its not the first chord
        {
                populate_next_note(line_bass);
                populate_next_note(line_suprano);
                populate_next_note(line_alto);

                // tests if the octave assignment works 
                if (notes[chord_last].octave == 0) {notes[chord_last].pitch = -1;}
                if (notes[chord_last-1].octave == 0) {notes[chord_last-1].pitch = -1;}
                if (notes[chord_last-2].octave == 0) {notes[chord_last-2].pitch = -1;}
                if (notes[chord_last].octave == 32767) {notes[chord_last].pitch = -1;}
                if (notes[chord_last-1].octave == 32767) {notes[chord_last-1].pitch = -1;}
                if (notes[chord_last-2].octave == 32767) {notes[chord_last-2].pitch = -1;}

                cout <<  chord_last << " " << notes[chord_last].octave << endl;
                cout << chord_last - 1 << " " << notes[chord_last - 1].octave << endl;
                cout << chord_last - 2 << " " << notes[chord_last - 2].octave << endl << endl;
                
                cout << "used:" << endl;
                for (int k = 0; k < 3; k++)
                {
                        cout << used[k] << endl;
                }
                cout << endl; 
        }
}

// populates the octave of appropriate note and fills the used array
void Composer::populate_next_note(int passed_line)
{
        // nearest index initialized to the most recent note in the list that isnt used
        int nearest_index = chord_last;
        for (int i = 0; i < 3; i++)
        {
                //if (used[i] == true) 
                //{
                //}
                if (note_distance(order(notes[chord_last - i].pitch, 
                                                        get_nearest_octave(passed_line, notes[chord_last - i].pitch)), passed_line)
                         <= note_distance(order(notes[nearest_index].pitch, 
                                                        get_nearest_octave(passed_line, notes[nearest_index].pitch)), passed_line)
                         )
                {
                        nearest_index = chord_last - i;
                }
        }
        used[chord_last - nearest_index] = true;
        notes[nearest_index].octave = get_nearest_octave(passed_line, notes[nearest_index].pitch); // assigns the octave
        
        // update lines
        if (passed_line == line_bass){line_bass = order(notes[nearest_index].pitch, notes[nearest_index].octave);}
        if (passed_line == line_suprano){line_suprano = order(notes[nearest_index].pitch, notes[nearest_index].octave);}
        if (passed_line == line_alto){line_alto = order(notes[nearest_index].pitch, notes[nearest_index].octave);}
}
*/
//////// Smooth lines original idea:
/*
        
        // populate bass by function takes in random 1 of 3 pitches and line puts it closest to
        // previous pitch in line
        // populate other lines by determining which 2 pitches weren't used and placing them with
        // other lines
        //
        //

        if (chord_previous - 2 >= 0) // if not the first chord
        {
                int j = chord_previous - 2;
                int closest_index_chord_last;
                        for (int i = chord_last; i >= chord_last - 2; i--)
                        {
                                for (int k = 0; k < 7; k++)
                                {
                                        int closest_interval = abs(order(notes[j].pitch, notes[j].octave) 
                                                - order(notes[i].pitch, k));
                                        if (abs(order(notes[j].pitch, notes[j].octave) 
                                                - order(notes[i].pitch, k)) <= closest_interval)
                                        {
                                                closest_interval = abs(order(notes[j].pitch,
                                                notes[j].octave) - order(notes[i].pitch, k));
                                                notes[i].octave = k;
                                        }
                                }
                        }
                        int closest_note = chord_last;
                        int closest_distance = abs(order(notes[chord_last].pitch, notes[chord_last].octave)
                                        - order(notes[j].pitch, notes[j].octave));
                        for (int i = chord_last; i >= chord_last - 2; i--)
                        {
                                if (abs(order(notes[i].pitch, notes[i].octave) - order(notes[j].pitch, notes[j].octave))
                                                <= closest_distance)
                                {
                                        closest_note = i;
                                        closest_index_chord_last = i;
                                }
                        }
                // you now have octave assigned to all three chord_last notes, which are closest to chord_previous note j
                // you also have the index of the closest chord_last note to note j
                int second_index_chord_last;
                int l = chord_previous - 1;
                int m = chord_previous;
                int third_index_chord_last;
                int closest_octave_third;
                if (closest_index_chord_last == chord_last - 2)
                {
                        for (int i = chord_last; i >= chord_last - 1; i--)
                        {
                                for (int k = 0; k < 7; k++)
                                {
                                        int closest_interval = abs(order(notes[l].pitch, notes[l].octave) 
                                                - order(notes[i].pitch, k));
                                        if (abs(order(notes[l].pitch, notes[l].octave) 
                                                - order(notes[i].pitch, k)) <= closest_interval)
                                        {
                                                closest_interval = abs(order(notes[l].pitch,
                                                notes[l].octave) - order(notes[i].pitch, k));
                                                notes[i].octave = k;
                                        }
                                }
                        }
                        int closest_note_two = chord_last;
                        int closest_distance_two = abs(order(notes[chord_last].pitch, notes[chord_last].octave)
                                        - order(notes[l].pitch, notes[l].octave));
                        for (int i = chord_last; i >= chord_last - 1; i--)
                        {
                                if (abs(order(notes[i].pitch, notes[i].octave) - order(notes[l].pitch, notes[l].octave))
                                                <= closest_distance_two)
                                {
                                        closest_note_two = i;
                                        second_index_chord_last = i;
                                }
                        }
                        // now you have found the index of note 2, which is closest to note l
                        //
                        // last index: go through octaves to see which octave fits best for last note
                        if (second_index_chord_last == chord_last)
                        {
                                third_index_chord_last = chord_last - 1;                        
                        }
                        if (second_index_chord_last == chord_last - 1)
                        {
                                third_index_chord_last = chord_last;
                        }
                        int closest_octave_dist = abs(order(notes[m].pitch, notes[m].octave) 
                                        - order(notes[third_index_chord_last].pitch, 0));
                        for (int k = 0; k < 7; k++)
                        {
                                if (abs(order(notes[m].pitch, notes[m].octave) 
                                        - order(notes[third_index_chord_last].pitch, k)) <= closest_octave_dist)
                                {
                                        closest_octave_dist = abs(order(notes[m].pitch, notes[m].octave) 
                                                - order(notes[third_index_chord_last].pitch, k));
                                        closest_octave_third = k;
                                }
                        }
                        notes[third_index_chord_last].octave = closest_octave_third;
                } 
                if (closest_index_chord_last == chord_last)
                {
                        for (int i = chord_last  - 1; i >= chord_last - 2; i--)
                        {
                                for (int k = 0; k < 7; k++)
                                {
                                        int closest_interval = abs(order(notes[l].pitch, notes[l].octave) 
                                                - order(notes[i].pitch, k));
                                        if (abs(order(notes[l].pitch, notes[l].octave) 
                                                - order(notes[i].pitch, k)) <= closest_interval)
                                        {
                                                closest_interval = abs(order(notes[l].pitch,
                                                notes[l].octave) - order(notes[i].pitch, k));
                                                notes[i].octave = k;
                                        }
                                }
                        }
                        int closest_note_two = chord_last;
                        int closest_distance_two = abs(order(notes[chord_last].pitch, notes[chord_last].octave)
                                        - order(notes[l].pitch, notes[l].octave));
                        for (int i = chord_last - 1; i >= chord_last - 2; i--)
                        {
                                if (abs(order(notes[i].pitch, notes[i].octave) - order(notes[l].pitch, notes[l].octave))
                                                <= closest_distance_two)
                                {
                                        closest_note_two = i;
                                        second_index_chord_last = i;
                                }
                        }
                        // now you have found the index of note 2, which is closest to note l
                        //
                        // last index: go through octaves to see which octave fits best for last note
                        if (second_index_chord_last == chord_last - 1)
                        {
                                third_index_chord_last = chord_last - 2;                        
                        }
                        if (second_index_chord_last == chord_last - 2)
                        {
                                third_index_chord_last = chord_last - 1;
                        }
                        int closest_octave_dist = abs(order(notes[m].pitch, notes[m].octave) 
                                        - order(notes[third_index_chord_last].pitch, 0));
                        for (int k = 0; k < 7; k++)
                        {
                                if (abs(order(notes[m].pitch, notes[m].octave) 
                                        - order(notes[third_index_chord_last].pitch, k)) <= closest_octave_dist)
                                {
                                        closest_octave_dist = abs(order(notes[m].pitch, notes[m].octave) 
                                                - order(notes[third_index_chord_last].pitch, k));
                                        closest_octave_third = k;
                                }
                        }
                        notes[third_index_chord_last].octave = closest_octave_third;
                }
                if (closest_index_chord_last == chord_last - 1) // middle number
                {
                        for (int i = chord_last  - 1; i >= chord_last - 2; i -= 2) // skip middle
                        {
                                for (int k = 0; k < 7; k++)
                                {
                                        int closest_interval = abs(order(notes[l].pitch, notes[l].octave) 
                                                - order(notes[i].pitch, k));
                                        if (abs(order(notes[l].pitch, notes[l].octave) 
                                                - order(notes[i].pitch, k)) <= closest_interval)
                                        {
                                                closest_interval = abs(order(notes[l].pitch,
                                                notes[l].octave) - order(notes[i].pitch, k));
                                                notes[i].octave = k;
                                        }
                                }
                        }
                        int closest_note_two = chord_last;
                        int closest_distance_two = abs(order(notes[chord_last].pitch, notes[chord_last].octave)
                                        - order(notes[l].pitch, notes[l].octave));
                        for (int i = chord_last - 1; i >= chord_last - 2; i-= 2)
                        {
                                if (abs(order(notes[i].pitch, notes[i].octave) - order(notes[l].pitch, notes[l].octave))
                                                <= closest_distance_two)
                                {
                                        closest_note_two = i;
                                        second_index_chord_last = i;
                                }
                        }
                        // now you have found the index of note 2, which is closest to note l
                        //
                        // last index: go through octaves to see which octave fits best for last note
                        if (second_index_chord_last == chord_last)
                        {
                                third_index_chord_last = chord_last - 2;                        
                        }
                        if (second_index_chord_last == chord_last - 2)
                        {
                                third_index_chord_last = chord_last;
                        }
                        int closest_octave_dist = abs(order(notes[m].pitch, notes[m].octave) 
                                        - order(notes[third_index_chord_last].pitch, 0));
                        for (int k = 0; k < 7; k++)
                        {
                                if (abs(order(notes[m].pitch, notes[m].octave) 
                                        - order(notes[third_index_chord_last].pitch, k)) <= closest_octave_dist)
                                {
                                        closest_octave_dist = abs(order(notes[m].pitch, notes[m].octave) 
                                                - order(notes[third_index_chord_last].pitch, k));
                                        closest_octave_third = k;
                                }
                        }
                        notes[third_index_chord_last].octave = closest_octave_third;
                }
        }*/
int Composer::order(int pitch, int octave)
{
        return pitch + (octave * 12);
}
void Composer::expand_notes()
{
        NoteSpec *temp = new NoteSpec[notes_cap*2];
        
        for (int i = 0; i < notes_num; i++)
        {
                temp[i] = notes[i];
        }

        delete []notes;
        notes = temp;
        notes_cap = notes_cap*2;
}
NoteSpec* Composer::get_note_array_pointer()
{
        return notes;
}
int Composer::get_num_notes()
{
        return notes_num;
}

/////////////////////////////////////////////// PIECE ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////// expandable array of pointers to notes ///////////////////////////////

int cap_notes = 10;
int num_notes = 0;
Note **gAll_Notes = new Note*[cap_notes];

void expand_notes()
{
        Note **temp = new Note*[cap_notes*2];
        
        for (int i = 0; i < num_notes; i++)
        {
                temp[i] = gAll_Notes[i];
        }
        
        delete []gAll_Notes;
        gAll_Notes = temp;
        cap_notes = cap_notes*2;
}


///////////////////////////////////////////////////////////////////////////////

class Piece{
        public:
                Piece();
                Piece(string key, int numtop, int numbot);
                ~Piece(); 

                void play();
        private:

                void note_on_staff(int pitch, int octave, 
                                string accidental, float duration, 
                                bool rest, int staff);
                
                void draw_notes();
                int staff_position;
                Staff *staff1;
                Staff *staff2;
                int beats;
                int division;                
                
                Composer composer;
                void populate_composition();

                int note_position;
                int staff_end;
                int current_staff;

                int chord_shift;
};

//      Later: in the class that plays the music, 
//      if there is an accidental in the note, then change the note to be that pitch
//      so that the pitch works with the loaded pitches
Piece::Piece(){}

Piece::Piece(string key, int numtop, int numbot)
{
        beats = numtop;
        division = numbot;

        staff_end = SCREEN_WIDTH - STAFF_INDENT*2;
        current_staff = 1;
        staff_position = TOP_OF_STAVES + GAP;
        
        staff1 = new Staff(key, STAFF_INDENT, staff_position, beats, division);
        staff_position += STAFF_INCREMENT;
        staff2 = new Staff(key, STAFF_INDENT, staff_position, beats, division);
       
        chord_shift = 0; 
        note_position = staff1->get_keysig_pos() + 50;
        populate_composition();
}

Piece::~Piece(){}

void Piece::draw_notes()
{
        for (int i = 0; i < num_notes; i++)
        {
                gAll_Notes[i]->draw_note();
        }
}
void Piece::note_on_staff(int pitch, int octave, string accidental, float duration, bool rest, int staff)
{
        if (num_notes >= cap_notes) {expand_notes();}
        gAll_Notes[num_notes] = new Note(pitch, octave, accidental, duration, rest, staff);
        if (staff == 1) 
        {
                staff1->place_note(gAll_Notes[num_notes], note_position + chord_shift);
        }
        if (staff == 2) 
        {
                staff2->place_note(gAll_Notes[num_notes], note_position + chord_shift);
        }
        num_notes++;
}

///////////////// compose and play the piece /////////////////

void Piece::populate_composition()
{
        cout << "calling composer" << endl;
        composer.compose();
        cout << "piece composed" << endl;

        NoteSpec* notes = composer.get_note_array_pointer();       
        int amount_of_notes = composer.get_num_notes();
        
        // piece class will handle staff number and will keep track of durations
        // to create measures
        for (int i = 0; i < amount_of_notes; i++)
        {
                if (notes[i].pitch == -1)
                {
                        note_position += NOTE_INCREMENT;
                }
                else
                {
                        note_on_staff(notes[i].pitch, notes[i].octave, notes[i].accidental,
                                1.0, false, current_staff);
                }
                if (note_position - 30 >= staff_end) 
                {
                        current_staff++;
                        note_position = staff1->get_keysig_pos() + 50;
                }
        }
}
void Piece::play()
{
        draw_notes();
        staff1->draw_staff();
        staff2->draw_staff();
}

/////////////////////////////////////////////// PERFORMER ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class Performer{
        public:
                Performer();
                void perform();
        private:
                int tempo;
                bool white_key(int n);
                void fix_accidentals();
};

Performer::Performer()
{
        tempo = MINUTE/TEMPO;
        fix_accidentals();
}
void Performer::perform()
{
        int current_position;
        
        int i = 0;
        while (i < num_notes)
        {
                current_position = gAll_Notes[i]->get_position();
        
                while (gAll_Notes[i]->get_position() == current_position)
                {
                        if (!gAll_Notes[i]->get_rest())
                        {
                                Mix_PlayChannel( -1, gNotes[gAll_Notes[i]->get_octave()]
                                                [gAll_Notes[i]->get_pitch()][2], 0);
                        }
                        i++;
                        if (i == num_notes) {break;}
                }

                SDL_Delay(tempo);
        }
}

void Performer::fix_accidentals()
{
        for (int i = 0; i < num_notes; i++)
        {
                if (white_key(gAll_Notes[i]->get_pitch()) && gAll_Notes[i]->get_accidental() == "sharp")
                                {gAll_Notes[i]->set_pitch(gAll_Notes[i]->get_pitch() + 1);}
                if (white_key(gAll_Notes[i]->get_pitch()) && gAll_Notes[i]->get_accidental() == "flat")
                                {gAll_Notes[i]->set_pitch(gAll_Notes[i]->get_pitch() - 1);}
        }
}

bool Performer::white_key(int n)
{
        bool yes = true;
        if (n == CS
                || n == DS
                || n == FS
                || n == GS
                || n == AS)
        {
                yes = false;
        }
        return yes;
}
/////////////////////////////////////////////// DESIGN ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class Design{
        public:
                Design();

                void print_design();
                Object *play_button;

        private:

                Rectangle *head;
                Rectangle *foot;
                Rectangle *side1;
                Rectangle *side2;
                Textbox *tempo;
                Textbox *title;
                Textbox *composer;
                Textbox *footer_message;

                char *filepath_times;
                
                void set_black();
                void set_white();
                SDL_Color textcolor;
                Color rect_color;
};

Design::Design()
{
        std::string times = "visuals/times.ttf";
        filepath_times =  &times[0];

        rect_color.r = 0;
        rect_color.g = 10;
        rect_color.b = 30;
        rect_color.a = 255;

        
        //header
        Position p_head = {0, 0, SCREEN_WIDTH, TOP_OF_STAVES};
        head = new Rectangle(false, false, p_head, rect_color);

        //footer
        Position p_foot = {0, SCREEN_HEIGHT - STAFF_INDENT + 10, SCREEN_WIDTH, STAFF_INDENT - 10};
        foot = new Rectangle(false, false, p_foot, rect_color);
        
        //side1
        Position p_side1 = {0, TOP_OF_STAVES, STAFF_INDENT - 10, SCREEN_HEIGHT-TOP_OF_STAVES};
        side1 = new Rectangle(false, false, p_side1, rect_color);

        //side3
        Position p_side2 = {SCREEN_WIDTH - STAFF_INDENT + 10, TOP_OF_STAVES, STAFF_INDENT - 10, SCREEN_HEIGHT-TOP_OF_STAVES};
        side2 = new Rectangle(false, false, p_side2, rect_color);

        //play button
        Position p_play_button = {STAFF_INDENT*6, TOP_OF_STAVES - 25, 15, 15};
        play_button = new Object("visuals/play_button.png", p_play_button);       

        set_white();

        string pace = to_string(TEMPO);
        string message = "Quarter Note = ";
        tempo = new Textbox(message + pace, STAFF_INDENT, TOP_OF_STAVES - 30, textcolor, 20, filepath_times);

        title = new Textbox(TITLE, SCREEN_WIDTH/2, 10, textcolor, 40, filepath_times);
        title->set_position(SCREEN_WIDTH/2 - ( title->get_width() / 2 ), 10);
        
        string comp = "Composer: ";
        composer = new Textbox(comp + NAME, SCREEN_WIDTH, TOP_OF_STAVES - 30, textcolor, 20, filepath_times);
        composer->set_position(SCREEN_WIDTH - composer->get_width() - STAFF_INDENT, TOP_OF_STAVES - 30);

        footer_message = new Textbox("Ari Brown, 2016", 0, SCREEN_HEIGHT, textcolor, 12, filepath_times);
        footer_message->set_position(0, SCREEN_HEIGHT-footer_message->get_height());
}

void Design::print_design()
{
        head->draw_rectangle();
        foot->draw_rectangle();
        side1->draw_rectangle();
        side2->draw_rectangle();
        play_button->print_object();
        tempo->print_text();
        title->print_text();
        composer->print_text();
        footer_message->print_text();
}

void Design::set_black()
{
        textcolor.r = 0;
        textcolor.g = 0;
        textcolor.b = 0;
}
void Design::set_white()
{
        textcolor.r = 255;
        textcolor.g = 255;
        textcolor.b = 255;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// MAIN IMPLEMENTATION //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

int main( int argc, char* args[])
{
        open();


        ///////////// INITIAL VARIABLES //////////////
        bool quit = false;
        
        ///////////// CONTENTS ///////////////////////
                  
        SDL_Event e;


        Design design; 

        
        Piece my_piece(KEY, BEATS, PER_MEASURE);
        
        Performer performer;
                
        ///////////// TIMER //////////////////////////
        int timer = 0;
                
        ///////////// RUN LOOP ///////////////////////
        while (!quit)
        {
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL);
                                
                // manages interface events through SDL
                while (SDL_PollEvent(&e) != 0)
                {
                        if (e.type == SDL_QUIT){
                                quit = true;
                        }
                        if (e.type == SDL_MOUSEBUTTONDOWN)
                        {
                                if (e.button.button == SDL_BUTTON_LEFT)
                                {
                                        int x = e.button.x;
                                        int y = e.button.y;
                                        
                                        Position b_p = design.play_button->get_position();
                                                
                                        if (x > b_p.x && x < b_p.x + b_p.w 
                                                && y > b_p.y && y < b_p.y + b_p.h)
                                        {
                                                performer.perform(); 
                                        }
                                }
                        }
                }

                ///////////// OBJECT RENDER ///////////////////
                                
                design.print_design();

                my_piece.play();

                ///////////////////////////////////////////////
                
                // update the SDL screen
                SDL_RenderPresent( gRenderer );
                                
                // increment the timer
                timer++;
                if (timer > 30000) {timer = 0;} // resets timer
        }
        close();
        return 0;
}

