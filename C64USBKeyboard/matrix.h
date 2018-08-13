#ifndef MATRIX_H
#define MATRIX_H

#include <Keyboard.h>
// Missing from Keyboard.h
#define KEY_PRNT_SCRN 0xCE

// Some keyboard matrix positions
#define POS_INSERT_DELETE 0
#define POS_RETURN 1
#define POS_CRSR_LR 2
#define POS_F7 3
#define POS_F1 4
#define POS_F3 5
#define POS_F5 6
#define POS_CRSR_UD 7
#define POS_LEFT_SHIFT 15
#define POS_RIGHT_SHIFT 52
#define POS_COMMODORE 61

#define LAYER_SIZE 64

// The C64 keyboard matrix (with shifted values)
//Ins/Del        Return           CRSR LR          F7/F8            F1/F2            F3/F4            F5/F6            CRSR UD          Null
//3/#            W                A                4/$              Z                S                E                LShift           Null
//5/%            R                D                6/&              C                F                T                X                Null
//7/'            Y                G                8/(              B                H                U                V                Null
//9/)            I                J                0                M                K                O                N                Null
//+              P                L                -                ./>              :/[              @                ,/<              Null
//£              *                ;/]              Clr/Home         RShift           =                ↑                //?              Null
//1/!            ←               Control          2/"              Space            Commodore        Q                Run/Stop         Null
//Null           Null             Null             Null             Null             Null             Null             Null             Restore

// Restore is handled separately, and cannot be mapped, so the layers are 8*8 in size.
char layers[] = {
// C64 layer
//Ins/Del        Return           CRSR LR          F7               F1               F3               F5               CRSR UD          
KEY_DELETE,      KEY_RETURN,      KEY_RIGHT_ARROW, KEY_F7,          KEY_F1,          KEY_F3,          KEY_F5,          KEY_DOWN_ARROW,  
//3              W                A                4                Z                S                E                LShift           
'3',             'w',             'a',             '4',             'z',             's',             'e',             KEY_LEFT_SHIFT,  
//5              R                D                6                C                F                T                X                
'5',             'r',             'd',             '6',             'c',             'f',             't',             'x',             
//7              Y                G                8                B                H                U                V                
'7',             'y',             'g',             '8',             'b',             'h',             'u',             'v',             
//9              I                J                0                M                K                O                N                
'9',             'i',             'j',             '0',             'm',             'k',             'o',             'n',             
//+              P                L                -                .                :                @                ,                
'+',             'p',             'l',             '-',             '.',             ':',             '@',             ',',             
//£              *                ;                Clr/Home         RShift           =                ↑                /                
'#',             '*',             ';',             KEY_HOME,        KEY_RIGHT_SHIFT, '=',             KEY_PAGE_UP,     '/',             
//1              ←               Control          2                Space            Commodore        Q                Run/Stop         
'1',             KEY_BACKSPACE,   KEY_LEFT_CTRL,   '2',             ' ',             KEY_RIGHT_GUI,   'q',             KEY_ESC,         

// C64 layer - function
//Ins/Del        Return           CRSR LR          F7               F1               F3               F5               CRSR UD          
KEY_INSERT,      KEY_RETURN,      KEY_LEFT_ARROW,  KEY_F8,          KEY_F2,          KEY_F4,          KEY_F6,          KEY_UP_ARROW,    
//3              W                A                4                Z                S                E                LShift           
'#',             'w',             'a',             '$',             'z',             's',             'e',             KEY_LEFT_SHIFT,  
//5              R                D                6                C                F                T                X                
'%',             'r',             'd',             '&',             'c',             'f',             't',             'x',             
//7              Y                G                8                B                H                U                V                
'\'',            'y',             'g',             '(',             'b',             'h',             'u',             'v',             
//9              I                J                0                M                K                O                N                
')',             'i',             'j',             '0',             'm',             'k',             'o',             'n',             
//+              P                L                -                .                :                @                ,                
'+',             'p',             'l',             '-',             '>',             '[',             '@',             '<',             
//£              *                ;                Clr/Home         RShift           =                ↑                /                
'#',             '*',             ']',             KEY_END,         KEY_RIGHT_SHIFT, '=',             KEY_PAGE_DOWN,   '?',             
//1              ←               Control          2                Space            Commodore        Q                Run/Stop         
'!',             KEY_DELETE,      KEY_LEFT_CTRL,   '"',             ' ',             KEY_RIGHT_GUI,   'q',             KEY_ESC,         

// 60% layout layer
//Ins/Del        Return           CRSR LR          F7/F8            F1/F2            F3/F4            F5/F6            CRSR UD          
KEY_BACKSPACE,   KEY_RETURN,      KEY_RIGHT_ARROW, KEY_END,         KEY_HOME,        KEY_PAGE_UP,     KEY_PAGE_DOWN,   KEY_DOWN_ARROW,  
//3/#            W                A                4/$              Z                S                E                LShift           
'3',             'w',             'a',             '4',             'z',             's',             'e',             KEY_LEFT_SHIFT,  
//5/%            R                D                6/&              C                F                T                X                
'5',             'r',             'd',             '6',             'c',            'f',              't',             'x',             
//7/'            Y                G                8/(              B                H                U                V                
'7',             'y',             'g',             '8',             'b',             'h',             'u',             'v',             
//9/)            I                J                0                M                K                O                N                
'9',             'i',             'j',             '0',             'm',             'k',             'o',             'n',             
//+              P                L                -                ./>              :/[              @                ,/<              
'-',             'p',             'l',             '=',             '.',             ';',             '[',             ',',             
//£              *                ;/]              Clr/Home         RShift           =                ↑                //?              
KEY_RIGHT_CTRL,  ']',             '\'',            KEY_RIGHT_GUI,   KEY_RIGHT_SHIFT, KEY_RIGHT_ALT,   '\\',            '/',             
//1/!            ←               Control          2/"              Space            Commodore        Q                Run/Stop         
'1',             KEY_ESC,         KEY_TAB,        '2',              ' ',             KEY_LEFT_CTRL,   'q',             KEY_LEFT_ALT,    

// 60% layout layer function
//Ins/Del        Return           CRSR LR          F7/F8            F1/F2            F3/F4            F5/F6            CRSR UD          
KEY_DELETE,      KEY_RETURN,      KEY_LEFT_ARROW,  KEY_F8,          KEY_F2,          KEY_F4,          KEY_F6,          KEY_UP_ARROW,    
//3/#            W                A                4/$              Z                S                E                LShift           
KEY_F3,          KEY_UP_ARROW,    KEY_LEFT_ARROW,  KEY_F4,          'z',             KEY_DOWN_ARROW,  'e',             KEY_LEFT_SHIFT,  
//5/%            R                D                6/&              C                F                T                X                
KEY_F5,          'r',             KEY_RIGHT_ARROW, KEY_F6,          'c',            'f',              't',             'x',             
//7/'            Y                G                8/(              B                H                U                V                
KEY_F7,          'y',             'g',             KEY_F8,          'b',             'h',             'u',             'v',             
//9/)            I                J                0                M                K                O                N                
KEY_F9,          'i',             'j',             KEY_F10,         'm',             'k',             'o',             'n',             
//+              P                L                -                ./>              :/[              @                ,/<              
KEY_F11,         KEY_PRNT_SCRN,   'l',             KEY_F12,         '.',             ';',             '[',             ',',             
//£              *                ;/]              Clr/Home         RShift           =                ↑                //?              
KEY_RIGHT_GUI,   ']',             '\'',            KEY_RIGHT_GUI,   KEY_RIGHT_SHIFT, KEY_RIGHT_ALT,   '\\',            '/',             
//1/!            ←               Control          2/"              Space            Commodore        Q                Run/Stop         
KEY_F1,          '`',             KEY_TAB,         KEY_F2,          ' ',             KEY_LEFT_CTRL,   'q',             KEY_LEFT_ALT,    
};

#endif
