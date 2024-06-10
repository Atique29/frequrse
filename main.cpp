#include "sound_gen.h"
#include <curses.h>
#include <string>
#include <stdexcept>

const char *toneAttr[] = {

        " FREQUENCY ",
        " DURATION ",
        " GAP ",
        " REPEAT ",
};


const char *chirpAttr[] = {

        " FREQUENCY-START ",
        " FREQUENCY-STOP ",
        " SWEEP-TIME ",
        " GAP ",
        " REPEAT ",
};

size_t toneArraySize = sizeof(toneAttr)/sizeof(toneAttr[0]);
size_t chirpArraySize = sizeof(chirpAttr)/sizeof(chirpAttr[0]);

int modeSelected = 0; // selected window- tone or chirp

void printMain(WINDOW *win,const char *attrArray[],int *valArray,size_t arraySize,int selected);

int main(int argc, char **argv)
{
    int toneVal[] = {490,10,3,5,}; //freq,duration(sec),gap(sec),repeat
    int chirpVal[] = {490,630,10,3,5,}; //freq_start,freq_stop,sweep(sec),gap,repeat
    char buffer[20];
    int toneSelected = 0;
    int chirpSelected = 0;

    initscr();

    noecho();
    cbreak();
    keypad(stdscr,1);

    start_color();
    init_pair(1,COLOR_RED,COLOR_BLACK); //fg,bg
    init_pair(2,COLOR_GREEN,COLOR_BLACK); //fg,bg
    curs_set(0);
    int ymax, xmax;
    getmaxyx(stdscr,ymax,xmax);
    int winLines = ymax * 0.95;
    int winCols = xmax * 0.95;

    //outer box
    WINDOW *win = newwin(winLines,winCols,(LINES-winLines)/2,(COLS-winCols)/2);
    box(win,0,0);

    //inner stuff
    int innerWinLines = 0.5 * winLines;
    int innerWinCols = 0.5 * winCols;
    WINDOW *innerWin = subwin(win,innerWinLines,innerWinCols,(LINES-innerWinLines)/2,(COLS-innerWinCols)/2);

    wattron(innerWin,COLOR_PAIR(2)|A_ITALIC);
    mvwprintw(innerWin,innerWinLines/2,innerWinCols/2 - 8,"[ : FREQURSE : ]");
    wattroff(innerWin,COLOR_PAIR(2)|A_ITALIC);

    char triggerKey;
    while ((triggerKey=wgetch(win)) != 'Q') {

        switch (triggerKey) {
            case 't':

                wattron(win,A_STANDOUT|A_BOLD);
                mvwprintw(win,0,2,"[TONE]");
                wattroff(win,A_STANDOUT|A_BOLD);
                mvwprintw(win,0,9,"[CHIRP]");
                wrefresh(win);

                werase(innerWin);
                wattron(innerWin,COLOR_PAIR(2));
                box(innerWin,0,0);
                wattroff(innerWin,COLOR_PAIR(2));

                printMain(innerWin,toneAttr,toneVal,toneArraySize,toneSelected);

                modeSelected = 0; //this mode is current mode
                break;

            case 'c':

                wattron(win,A_STANDOUT|A_BOLD);
                mvwprintw(win,0,9,"[CHIRP]");
                wattroff(win,A_STANDOUT|A_BOLD);
                mvwprintw(win,0,2,"[TONE]");
                wrefresh(win);

                werase(innerWin);
                wattron(innerWin,COLOR_PAIR(2));
                box(innerWin,0,0);
                wattroff(innerWin,COLOR_PAIR(2));

                printMain(innerWin,chirpAttr,chirpVal,chirpArraySize,chirpSelected);

                modeSelected = 1;
                break;

            case '\t':

                if (modeSelected == 0) {    // tone mode
                    if (toneSelected == toneArraySize - 1) { toneSelected = 0; } //roll back
                    else {
                        toneSelected++;
                    }

                werase(innerWin);
                wattron(innerWin,COLOR_PAIR(2));
                box(innerWin,0,0);
                wattroff(innerWin,COLOR_PAIR(2));
                printMain(innerWin,toneAttr,toneVal,toneArraySize,toneSelected);

                }
                else {
                    if (chirpSelected == chirpArraySize - 1) { chirpSelected = 0; }
                    else {
                        chirpSelected++;
                    }

                    printMain(innerWin,chirpAttr,chirpVal,chirpArraySize,chirpSelected);

                }

                break;

            case 10:    //enter

                wmove(win,LINES/4 -2,COLS/4);
                wclrtoeol(win);     //clear if INVALID is written
                mvwprintw(win,LINES/4 - 2,COLS/4,"ENTER VAL > ");
                echo();
                wgetnstr(win,buffer,20);
                noecho();
                wmove(win,LINES/4 -2,COLS/4);
                wclrtoeol(win);     //clear after input done


                int inputVal;

                try {
                        inputVal = std::stoi(buffer); // Convert string to integer

                    } catch (const std::invalid_argument& e) {

                        inputVal = 0;
                        mvwprintw(win, LINES/4 - 2, COLS/4, "INVALID INPUT!");
                    }

                if (modeSelected == 0) {

                    toneVal[toneSelected] = inputVal;       //update value

                }
                else {

                    chirpVal[chirpSelected] = inputVal;       //update value

                }

                ungetch('\t'); //simulate a tab press so the value gets updates on innerWin
                break;

            case 'p':   // generate sine waves

                wrefresh(innerWin);
                if (modeSelected == 0) {

                    gen_tone(toneVal[0],toneVal[1],toneVal[2],toneVal[3]);

                }

                else {
                    gen_sweep(chirpVal[0],chirpVal[1],chirpVal[2],chirpVal[3],chirpVal[4]);
                }
                wrefresh(innerWin);
                break;


            default:
                break;
        }
    }

    endwin();
    return 0;
}

void printMain(WINDOW *win,const char *attrArray[],int *valArray,size_t arraySize,int selected){

    int spaces = (getmaxy(win) - arraySize)/2 - 2 ;

    for (size_t i = 0; i < arraySize; i++) {

        if (i == selected) {

            wattron(win,A_STANDOUT|A_BOLD|COLOR_PAIR(2));

            mvwprintw(win,i+spaces,(getmaxx(win)-34)/2,"%-19s  >  %10d ",attrArray[i],valArray[i]);

            wattroff(win,A_STANDOUT|A_BOLD|COLOR_PAIR(2));
            wrefresh(win);
        }
        else {
            mvwprintw(win,i+spaces,(getmaxx(win)-34)/2,"%-19s  >  %10d ",attrArray[i],valArray[i]);
        }

        spaces += 1;
    }


    wrefresh(win);

}



