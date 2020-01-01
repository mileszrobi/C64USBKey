#ifndef MATRIX_H
#define MATRIX_H

#include <Keyboard.h>
// Missing from Keyboard.h
#define KEY_PRNT_SCRN 0xCE

// Some keyboard matrix positions
#define POS_F1 24
#define POS_F3 16
#define POS_F5 8
#define POS_F7 0
#define POS_COMMODORE 23

#define MATRIX_SIZE 64

// The C64 keyboard matrix (with shifted values). The table headers show the corresponding pins of the C64 ribbon cable.
//
//    |  13             14               15               16               17              18                19               20 <- Columns, output pins
// ------------------------------------------------------------------------------------------------------------------------------------
// 5  |  F7 F8          Clr Home         -                0                8 (              6 &              4 $              2 "
// 6  |  F5 F6          ↑                @                O                U                T                E                Q
// 7  |  F3 F4          =                : [              K                H                F                S                C=
// 8  |  F1 F2          RShift           . >              M                B                C                Z                Space
// 9  |  Crsr U/D       / ?              , <              N                V                X                LShift           Run/Stop 
// 10 |  Crsr L/R       ; ]              L                J                G                D                A                Ctrl
// 11 |  Return         *                P                I                Y                R                W                ←
// 12 |  Ins Del        £                +                9 )              7 '              5 %              3 #              1 !
// ^
// Rows, input pins
//
// Restore is handled separately:
//       1
// 3     Restore
// We'll use Restore as a function key, and thus it cannot be re-mapped, it's not included in the matrix definition below.

// The 'layers' array below will contain the character mapped for each intersection of the matrix. The value to be sent will depend on the 
// current 'mode' that the keyboard is in, and wheter Restore (our function key) is down. So, for each mode, there are 2 mappings (normal, and
// with function), each containing 8x8 characters.
char layers[] = {
// C64 mode - normal
//    |  13             14               15               16               17              18                19               20
// ------------------------------------------------------------------------------------------------------------------------------------
// 5  |  F7 F8          Clr Home         -                0                8 (              6 &              4 $              2 "
         KEY_F7,        KEY_HOME,        '-',             '0',             '8',             '6',             '4',             '2',
// 6  |  F5 F6          ↑                @                O                U                T                E                Q
         KEY_F5,        KEY_PAGE_UP,     '@',             'o',             'u',             't',             'e',             'q',
// 7  |  F3 F4          =                : [              K                H                F                S                C=
         KEY_F3,        '=',             ':',             'k',             'h',             'f',             's',             KEY_RIGHT_GUI,
// 8  |  F1 F2          RShift           . >              M                B                C                Z                Space
         KEY_F1,        KEY_RIGHT_SHIFT, '.',             'm',             'b',             'c',             'z',             ' ',
// 9  |  Crsr U/D       / ?              , <              N                V                X                LShift           Run/Stop 
         KEY_DOWN_ARROW,'/',             ',',             'n',             'v',             'x',             KEY_LEFT_SHIFT,  KEY_ESC,
// 10 |  Crsr L/R       ; ]              L                J                G                D                A                Ctrl
         KEY_RIGHT_ARROW,';',            'l',             'j',             'g',             'd',             'a',             KEY_LEFT_CTRL,
// 11 |  Return         *                P                I                Y                R                W                ←
         KEY_RETURN,    '*',             'p',             'i',             'y',             'r',             'w',             KEY_BACKSPACE,
// 12 |  Ins Del        £                +                9 )              7 '              5 %              3 #              1 !
         KEY_DELETE,    '£',             '+',             '9',             '7',             '5',             '3',             '1',

// C64 layer - with restore (function) pressed. We'll use this to send the original shifted values on the C64 keys.
//    |  13             14               15               16               17              18                19               20
// ------------------------------------------------------------------------------------------------------------------------------------
// 5  |  F7 F8          Clr Home         -                0                8 (              6 &              4 $              2 "
         KEY_F8,        KEY_END,         '-',             '0',             '(',             '&',             '$',             '"',
// 6  |  F5 F6          ↑                @                O                U                T                E                Q
         KEY_F6,        KEY_PAGE_DOWN,   '@',             'o',             'u',             't',             'e',             'q',
// 7  |  F3 F4          =                : [              K                H                F                S                C=
         KEY_F4,        '=',             '[',             'k',             'h',             'f',             's',             KEY_RIGHT_GUI,
// 8  |  F1 F2          RShift           . >              M                B                C                Z                Space
         KEY_F2,        KEY_RIGHT_SHIFT, '>',             'm',             'b',             'c',             'z',             ' ',
// 9  |  Crsr U/D       / ?              , <              N                V                X                LShift           Run/Stop 
         KEY_UP_ARROW,  '?',             '<',             'n',             'v',             'x',             KEY_LEFT_SHIFT,  KEY_ESC,
// 10 |  Crsr L/R       ; ]              L                J                G                D                A                Ctrl
         KEY_LEFT_ARROW,']',            'l',              'j',             'g',             'd',             'a',             KEY_LEFT_CTRL,
// 11 |  Return         *                P                I                Y                R                W                ←
         KEY_RETURN,    '*',             'p',             'i',             'y',             'r',             'w',             KEY_DELETE,
// 12 |  Ins Del        £                +                9 )              7 '              5 %              3 #              1 !
         KEY_INSERT,    '£',             '+',             ')',             '\'',            '%',             '#',             '!',



// 60% mode - normal layer - similar to how a Poker II is laid out.
//    |  13             14               15               16               17              18                19               20
// ------------------------------------------------------------------------------------------------------------------------------------
// 5  |  F7 F8          Clr Home         -                0                8 (              6 &              4 $              2 "
         KEY_END,       KEY_RIGHT_GUI,   '=',             '0',             '8',             '6',             '4',             '2',
// 6  |  F5 F6          ↑                @                O                U                T                E                Q
         KEY_PAGE_DOWN, '\\',            '[',             'o',             'u',             't',             'e',             'q',
// 7  |  F3 F4          =                : [              K                H                F                S                C=
         KEY_PAGE_UP,   KEY_RIGHT_ALT,   ';',             'k',             'h',             'f',             's',             KEY_LEFT_CTRL,
// 8  |  F1 F2          RShift           . >              M                B                C                Z                Space
         KEY_HOME,      KEY_RIGHT_SHIFT, '.',             'm',             'b',             'c',             'z',             ' ',
// 9  |  Crsr U/D       / ?              , <              N                V                X                LShift           Run/Stop 
         KEY_DOWN_ARROW,'/',             ',',             'n',             'v',             'x',             KEY_LEFT_SHIFT,  KEY_LEFT_ALT,
// 10 |  Crsr L/R       ; ]              L                J                G                D                A                Ctrl
         KEY_RIGHT_ARROW,'\'',           'l',             'j',             'g',             'd',             'a',             KEY_TAB,
// 11 |  Return         *                P                I                Y                R                W                ←
         KEY_RETURN,    ']',             'p',             'i',             'y',             'r',             'w',             KEY_ESC,
// 12 |  Ins Del        £                +                9 )              7 '              5 %              3 #              1 !
         KEY_BACKSPACE, KEY_RIGHT_CTRL,  '-',             '9',             '7',             '5',             '3',             '1',


// 60% layout layer function
//    |  13             14               15               16               17              18                19               20
// ------------------------------------------------------------------------------------------------------------------------------------
// 5  |  F7 F8          Clr Home         -                0                8 (              6 &              4 $              2 "
         KEY_END,       KEY_RIGHT_GUI,   KEY_F12,         KEY_F10,         KEY_F8,          KEY_F6,          KEY_F4,          KEY_F2,
// 6  |  F5 F6          ↑                @                O                U                T                E                Q
         KEY_PAGE_DOWN, '\\',            '[',             'o',             'u',             't',             'e',             'q',
// 7  |  F3 F4          =                : [              K                H                F                S                C=
         KEY_PAGE_UP,   KEY_RIGHT_ALT,   ';',             'k',             'h',             'f',             KEY_DOWN_ARROW,  KEY_LEFT_CTRL,
// 8  |  F1 F2          RShift           . >              M                B                C                Z                Space
         KEY_HOME,      KEY_RIGHT_SHIFT, '.',             'm',             'b',             'c',             'z',             ' ',
// 9  |  Crsr U/D       / ?              , <              N                V                X                LShift           Run/Stop 
         KEY_UP_ARROW,  '/',             ',',             'n',             'v',             'x',             KEY_LEFT_SHIFT,  KEY_LEFT_ALT,
// 10 |  Crsr L/R       ; ]              L                J                G                D                A                Ctrl
         KEY_LEFT_ARROW,'\'',            'l',             'j',             'g',             KEY_RIGHT_ARROW, KEY_LEFT_ARROW,  KEY_TAB,
// 11 |  Return         *                P                I                Y                R                W                ←
         KEY_RETURN,    ']',             KEY_PRNT_SCRN,   'i',             'y',             'r',             KEY_UP_ARROW,    '`',
// 12 |  Ins Del        £                +                9 )              7 '              5 %              3 #              1 !
         KEY_DELETE,    KEY_RIGHT_CTRL,  KEY_F11,         KEY_F9,          KEY_F7,          KEY_F5,          KEY_F3,          KEY_F1,
};

#endif
